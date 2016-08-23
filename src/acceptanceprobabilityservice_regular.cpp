/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "acceptanceprobabilityservice_regular.h"
#include "prior.h"
#include "utilities.h"

namespace aims
{
    double AcceptanceProbabilityService_Regular::Solve(ChainData const &data_previous, 
			ChainData const &data_current, ChainServices const &services, sample_type const &candidate, 
			double candidate_log_likelihood, uint64_t k)
    {
			sample_type const &last_sample = data_current.sample.back();
			double const last_log_likelihood = data_current.log_likelihood.back();
			double const beta = data_current.beta;
			Proposal &proposal = *(data_current.proposal);
			Prior &prior = *(services.prior);

			const uint64_t num_samples_previous = data_previous.num_samples;
			std::vector<double> approx_logpdf_numerator_terms(num_samples_previous, 0.0);
			std::vector<double> approx_logpdf_denominator_terms(num_samples_previous, 0.0);

			// x is the candidate
			// prev contains samples from the previous annealing stage
			// next contains samples from the current annealing stage
			double log_density_candidate = prior.LogPDF(candidate) + beta * candidate_log_likelihood;
			double log_density_sample_k = prior.LogPDF(data_previous.sample[k]) + beta * data_previous.log_likelihood[k];
			double log_density_last = prior.LogPDF(last_sample) + beta * last_log_likelihood;

			for (uint64_t i = 0; i < num_samples_previous; i++)
			{
				approx_logpdf_numerator_terms[i] += std::min(0.0, prior.LogPDF(last_sample) + beta * last_log_likelihood
					- prior.LogPDF(data_previous.sample[i]) - beta * data_previous.log_likelihood[i]);//std::min(0.0, p->logpdf(*ls) + beta * (*ll) - p->logpdf(prev->x[i]) - beta * prev->loglike[i]);
				approx_logpdf_numerator_terms[i] += proposal.LogPDF(last_sample, data_previous.sample[i]);//next->q->logpdf(*ls, prev->x[i]);
				approx_logpdf_numerator_terms[i] += data_previous.importance_weight_local[i];//prev->wcurr[i];

				approx_logpdf_denominator_terms[i] += std::min(0.0, prior.LogPDF(candidate) + beta * candidate_log_likelihood
					- prior.LogPDF(data_previous.sample[i]) - beta * data_previous.log_likelihood[i]);//std::min(0.0, p->logpdf(x) + beta * l - p->logpdf(prev->x[i]) - beta * prev->loglike[i]);
				approx_logpdf_denominator_terms[i] += proposal.LogPDF(candidate, data_previous.sample[i]);//next->q->logpdf(x, prev->x[i]);
				approx_logpdf_denominator_terms[i] += data_previous.importance_weight_local[i];//prev->wcurr[i];
			}

			double approx_logpdf_numerator = utilities::LogSumExp(approx_logpdf_numerator_terms);
			double approx_logpdf_denominator = utilities::LogSumExp(approx_logpdf_denominator_terms);

			double log_value = std::min(0.0, log_density_candidate - log_density_sample_k) 
				+ std::min(0.0, log_density_candidate + approx_logpdf_numerator - log_density_last - approx_logpdf_denominator);
			double value = exp(log_value);

			return value;


/*		double value = utilities::AcceptanceProbabilityBasic(data_previous, 
			data_current, services, candidate, candidate_log_likelihood, k);

		return value;*/
    }
}
