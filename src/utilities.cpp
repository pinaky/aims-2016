/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "utilities.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <limits>
#include <sstream>
//#include <pair>

namespace aims
{
	namespace utilities
	{
		std::vector<double> SimpleMoment(std::vector<std::vector<double> > const &x, uint64_t m)
		{
			uint64_t i, j, k;
			
			const uint64_t d = x[0].size();

			std::vector<double> val(d, 0.0);

			for (i = 0u; i < x.size(); i++)
			{
				std::vector<double> term(d, 1.0);

				for (k = 0u; k < d; k++)
				{
					for (j = 0u; j < m; j++)
						term[k] *= x[i][k];

					val[k] += term[k];
				}
			}

			for (k = 0u; k < d; k++)
				val[k] /= (double)(x.size());

			return val;
		}

		double L2Norm(std::vector<double> const &x)
		{
			double val = 0.0;
			std::vector<double>::const_iterator it;

			try 
			{
				if(x.empty()) throw std::logic_error("Vector argument empty in function L2Norm");
			}

			catch(std::exception &e) 
			{
				std::cerr << e.what() << std::endl;
				exit(1);
			}
			
			for (uint64_t i = 0u; i < x.size(); i++)
				val += x[i] * x[i];
			return sqrt(val);
		}

		double L2Distance(std::vector<double> const &x, std::vector<double> const &y)
		{
			try 
			{
				if(!(x.size() == y.size())) throw std::logic_error("Vectors not the same size in function L2Distance");
			}

			catch(std::exception &e) 
			{
				std::cerr << e.what() << std::endl;
				exit(1);
			}

			const uint64_t d = x.size();
			double term, val = 0.0;

			for (uint64_t i = 0u; i < d; i++)
			{
				term = y[i] - x[i];
				val += term * term;
			}

			return sqrt(val);
		}

		double L2NormSquared(std::vector<double> const &x)
		{
			try 
			{
				if(x.empty()) throw std::logic_error("Vector argument empty in function L2NormSquared");
			}
			catch(std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				exit(1);
			}
			double val = 0.0;

			for (uint64_t i = 0u; i < x.size(); i++)
				val += x[i] * x[i];
			return val;
		}

		double L2DistanceSquared(std::vector<double> const &x, std::vector<double> const &y)
		{
			if(!(x.size() == y.size())) throw std::logic_error("Vectors not the same size in function L2Distance");

			const uint64_t d = x.size();
			uint64_t i;
			double term, val = 0.0;

			for (i = 0u; i < d; i++)
			{
				term = y[i] - x[i];
				val += term * term;
			}

			return val;
		}

		double LogSumExp(std::vector<double> const &x)
		{
			double val = 0.0;
			std::vector<double>::const_iterator it;
			std::vector<double>::const_iterator mit = std::max_element(x.begin(), x.end());
			const double max_value = *mit;

			for (uint64_t i = 0u; i < x.size(); i++)
				val += exp(x[i] - max_value);

			val = max_value + log(val);

			return val;
		}

		signed_logarithm LogSumExpSign(std::vector<signed_logarithm> const &x)
		{
			const uint64_t num_terms = x.size();
			if(!(num_terms > 0)) throw std::logic_error("Non-positive number of terms in utilities::LogSumExpSign");

			signed_logarithm value(0.0, 0.0);

			std::vector<signed_logarithm>::const_iterator maximium_value_element_index = 
				std::max_element(x.begin(), x.end() /*, std::less<signed_logarithm>()*/); 

			const double maximum_value = maximium_value_element_index->first;

			for (uint64_t i = 0; i < num_terms; i++)
				value.first += x[i].second * exp(x[i].first - maximum_value);

			value.second = (double)(value.first > 0.0) - (double)(value.first < 0.0);
			value.first = maximum_value + log(std::abs(value.first));

			return value;
		}

		double LogSumExpScale(double alpha, std::vector<double> const &x)
		{
			double val = 0.0;
			std::vector<double>::const_iterator mit = std::max_element(x.begin(), x.end());
			const double max_value = *mit;

			// Scale the vector by alpha
			for (uint64_t i = 0u; i < x.size(); i++)
				val += exp(alpha * (x[i] - max_value));

			val = alpha * max_value + log(val);

			return val;
		}

		signed_logarithm LogSumExpScaleSign(double alpha, std::vector<signed_logarithm> const &x)
		{
			signed_logarithm value(0.0, 0.0);
			std::vector<signed_logarithm>::const_iterator maximum_value_element_index = std::max_element(x.begin(), x.end(), 
				[](signed_logarithm const &lhs, signed_logarithm const &rhs) {return lhs.first > rhs.first; });
			const double maximum_value = maximum_value_element_index->first;

			// Scale the vector by alpha
			for (uint64_t i = 0; i < x.size(); i++)
				value.first += x[i].second * exp(alpha * (x[i].first - maximum_value));

			value.second = (double)(value.first > 0.0) - (double)(value.first < 0.0);

			value.first = alpha * maximum_value + log(std::abs(value.first));

			return value;
		}

		uint64_t IntegerAtRandom(std::vector<double> const &w, std::mt19937_64 &g, bool log_flag)
		{
			bool flag = false;
			double u;
			// A forgiving check for logarithmic precision
			const double EPSTOL = 1.0E-2;

			double s = 0.0;
			std::vector<double>::const_iterator it;
			uint64_t val;

			std::uniform_real_distribution<double> ud(0.0, 1.0);
			u = ud(g);
			for (uint64_t i = 0u; i < w.size(); i++)
			{
				if (log_flag) s += exp(w[i]);
				else s += w[i];
				if (u < s && !flag)
				{
					val = i;
					flag = true;
				}
			}

			// Make sure that all probabilities together sum to 1.0
			if(!(std::abs(s - 1.0) < EPSTOL)) throw std::logic_error("s does not sum to 1.0 in utilities::IntegerAtRandom(...)");

			return val;
		}

		std::pair<signed_logarithm, signed_logarithm> BetaFunctionAndDerivative(std::vector<double> const &log_likelihood, 
			std::vector<signed_logarithm> const &log_log_likelihood, double db, double constant, bool find_derivative)
		{
			const double nan_variable = std::numeric_limits<double>::quiet_NaN();
			const uint64_t num_terms = log_likelihood.size();
			std::vector<signed_logarithm> function_terms;

			double log_numerator_function_term = utilities::LogSumExpScale(2.0 * db, log_likelihood);
			double log_denominator_function_term = utilities::LogSumExpScale(db, log_likelihood);
			double log_function_term = log_numerator_function_term - 2.0 * log_denominator_function_term;

			function_terms.push_back(signed_logarithm(log_function_term, 1.0));
			function_terms.push_back(signed_logarithm(constant, -1.0));

			signed_logarithm log_derivative_value;
			signed_logarithm log_function_value = utilities::LogSumExpSign(function_terms);

			// Run this section if the derivative was requested for.
			if(find_derivative)
			{
				std::vector<signed_logarithm> derivative_terms;
				std::vector<signed_logarithm> term(num_terms);
				for(uint64_t i = 0; i < num_terms; i++)
				{
					term[i].first = log_log_likelihood[i].first + db * log_likelihood[i] + log(2);
					term[i].second = log_log_likelihood[i].second;
				}

				signed_logarithm log_denominator_derivative_term = utilities::LogSumExpSign(term);
				
				for(uint64_t i = 0; i < num_terms; i++)
					term[i].first += db * log_likelihood[i];

				signed_logarithm log_numerator_derivative_term = utilities::LogSumExpSign(term);

				signed_logarithm derivative_term_first = signed_logarithm(log_numerator_derivative_term.first - 
					2.0 * log_denominator_function_term, log_denominator_derivative_term.second);

				derivative_terms.push_back(derivative_term_first);

				signed_logarithm derivative_term_second = signed_logarithm(log_denominator_derivative_term.first + 
					log_numerator_function_term - 3.0 * log_denominator_function_term,
					-1.0 * log_numerator_derivative_term.second);

				derivative_terms.push_back(derivative_term_second);

				log_derivative_value = utilities::LogSumExpSign(derivative_terms);

				return std::pair<signed_logarithm, signed_logarithm>(log_function_value, log_derivative_value);
			}

			// Return NaNs for the derivative value if it wasn't requested
			log_derivative_value.first = nan_variable;
			log_derivative_value.second = nan_variable;

			return std::pair<signed_logarithm, signed_logarithm>(log_function_value, log_derivative_value);
		}

		double BetaBinarySearch(aims::ChainData const &data, double constant)
		{
			const uint64_t maximum_iterations = 1080u;
			// 100 max iterations is plenty. You need 53 to get a tolerance of 10^-16 for interval [0, 1]
			// Required iterations n = 
			std::pair<signed_logarithm, signed_logarithm> beta_function_value;
			std::vector<signed_logarithm> null_signed_logarithm;
			std::vector<double> signs(3), interval(3), value(3);
			interval[0] = 0.0;
			interval[2] = 1.1;
			double delta_previous = 1.0, delta_next = 1.0;
			const double error_tolerance = 1.0E-10;
			double error = 1.0, beta;

			for(uint64_t i = 0; i < maximum_iterations && error > error_tolerance; i++)
			{
				beta_function_value = BetaFunctionAndDerivative(data.log_likelihood, 
					null_signed_logarithm, interval[0], constant, false);
				signs[0] = beta_function_value.first.second;
				value[0] = beta_function_value.first.first;

				interval[1] = 0.5 * (interval[0] + interval[2]);
				beta_function_value = BetaFunctionAndDerivative(data.log_likelihood, 
					null_signed_logarithm, interval[1], constant, false);
				signs[1] = beta_function_value.first.second;
				value[1] = beta_function_value.first.first;

				beta_function_value = BetaFunctionAndDerivative(data.log_likelihood, 
					null_signed_logarithm, interval[2], constant, false);
				signs[2] = beta_function_value.first.second;
				value[2] = beta_function_value.first.first;

				if (signs[0] * signs[2] > 0.0) return data.beta;

				if(signs[1] * signs[2] < 0.0) interval[0] = interval[1];
				else if(signs[0] * signs[1] < 0.0) interval[2] = interval[1];

				delta_next = interval[1];
				error = std::abs(delta_previous - delta_next) / delta_previous;
				delta_previous = delta_next;
			}

			if(error > error_tolerance)
			{
				std::ostringstream os;
				os << "Convergence problem in utilities::BetaBinarySearch(). ";
				os << "Relative error = " << error * 100.0 << "%";
				throw std::logic_error(os.str());
			}

			beta = data.beta + interval[0];
			return beta;
		}

		double BetaNewtonRaphsonRefine(aims::ChainData const &data, double constant)
		{
			const uint64_t maximum_iterations = 100u;
			const double error_tolerance = 1.0E-3;
			const double beta_previous = data.beta;

			std::pair<signed_logarithm, signed_logarithm> beta_function_derivative;
			std::vector<signed_logarithm> log_log_likelihood(data.num_samples);

			double delta_beta_next;
			double delta_beta_initial = BetaBinarySearch(data, constant);
			double delta_beta_previous = delta_beta_initial;
			double beta_intermediate = beta_previous + delta_beta_initial;

			if(beta_intermediate >= 1.0 || delta_beta_initial <= 0.0)
				return beta_intermediate;

			for(uint64_t i = 0; i < data.num_samples; i++)
			{
				log_log_likelihood[i].first = log(std::abs(data.log_likelihood[i]));
				log_log_likelihood[i].second = (double)(data.log_likelihood[i] > 0.0) - (double)(data.log_likelihood[i] < 0.0);
			}

			double error = 1.0;
			for(uint64_t i = 0; i < maximum_iterations && error > error_tolerance; i++)
			{
				beta_function_derivative = BetaFunctionAndDerivative(data.log_likelihood,
					log_log_likelihood, delta_beta_previous, constant, true);

				double delta_delta_beta = -beta_function_derivative.first.second * 
					beta_function_derivative.second.second *
					exp(beta_function_derivative.first.first - beta_function_derivative.second.first);
				
				 delta_beta_next = delta_beta_previous + delta_delta_beta;
				 error = std::abs(delta_beta_next - delta_beta_previous) / delta_beta_previous;
				 delta_beta_previous = delta_beta_next;
			}

			if(!(error < error_tolerance))
			{
				std::ostringstream os;
				os << "Error in BetaNewtonRaphsonRefine. ";
				os << "Relative error = " << error * 100.0 << "%";
				throw std::logic_error(os.str());
			}

			double beta = beta_previous + delta_beta_next;
			
			return beta;
		}

		std::vector<double> NormalizedImportanceWeights(double delta_beta, aims::ChainData const &data)
		{
			const double lsum = utilities::LogSumExpScale(delta_beta, data.log_likelihood);

			std::vector<double> value(data.num_samples);
			
			for(uint64_t i = 0; i < data.num_samples; i++)
				value[i] = data.log_likelihood[i] * delta_beta - lsum;

			return value;
		}

/*		double AcceptanceProbabilityBasic(ChainData const &data_previous, ChainData const &data_current, ChainServices const &services,
			sample_type const &candidate, double candidate_log_likelihood, uint64_t k)
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

			double approx_logpdf_numerator = LogSumExp(approx_logpdf_numerator_terms);
			double approx_logpdf_denominator = LogSumExp(approx_logpdf_denominator_terms);

			double log_value = std::min(0.0, log_density_candidate - log_density_sample_k) 
				+ std::min(0.0, log_density_candidate + approx_logpdf_numerator - log_density_last - approx_logpdf_denominator);
			double value = exp(log_value);

			return value;
		}*/
	}
}
