/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include <stdexcept>
#include "chain.h"
#include "numsamplesscheduler.h"
#include "prior.h"
#include "forwardmodel.h"
#include "betascheduler.h"
#include "proposalscheduler.h"
#include "utilities.h"
#include "acceptanceprobabilityservice.h"
#include <algorithm>

// Check tag: Time stamp: 1471658267

namespace aims
{
    // This constructor is used to initialize the first chain with samples from the prior
    // The other constructor is used to create chains using previous chains
    Chain::Chain(ChainServices &services)
    {
        // This is the prior constructor
        // It is used for only one chain: that is for j = 0
        // Subsequent chains use other chains to accompish the same
        this->data_.j = 0;   // This is the first chain
        this->data_.beta = 0.0;  // This is the first chain; beta goes from 0.0 to 1.0
        this->data_.done = false;
        // Get the number of samples needed for this chain
        this->data_.num_samples = services.num_samples_scheduler->Solve(this->data_, this->data_, services); 
        // Set the proposal for the first chain to null, since no candidates need to be
        // drawn or evaluated.
        this->data_.proposal = nullptr;

        // Sample from the prior
        this->data_.sample = services.prior->Sample(this->data_.num_samples, *services.random_engine);
        std::vector<sample_type> const &x = this->data_.sample;   // Create a reference for succinctness

        // For each sample, do the following:
        for(uint64_t i = 0; i < x.size(); i++)
        {
            // Calculate the likelihood of the sample
            this->data_.log_likelihood.push_back(services.forward_model->LogLikelihood(x[i]));

            // For the first chain, some data is meaningless
            this->data_.accepted.push_back(true);
            this->data_.random_integer.push_back(this->data_.num_samples + 1);
            this->data_.acceptance_probability.push_back(1.0);
        }

        // Calculate the similarity of this set of samples with the posterior
        this->data_.importance_weight_posterior = utilities::NormalizedImportanceWeights(1.0 - this->data_.beta, this->data_);
        this->data_.num_samples_effective = utilities::LogSumExpScale(2.0, this->data_.importance_weight_posterior);

        services.beta_scheduler->Update(this->data_, services);
    }

    // This constructor uses a previous chain to construct itself and update the AIMS process
    Chain::Chain(ChainServices &services, Chain &previous)
    {
        try
        {
            // Increment stage number j from the previous step
            this->data_.j = previous.data_.j + 1;
            // Solve for beta using the previous chain's data
            this->data_.beta = services.beta_scheduler->Solve(previous.data_, this->data_, services);
            this->data_.done = services.beta_scheduler->Done();
            // Determine the new number of samples to be used
            this->data_.num_samples = services.num_samples_scheduler->Solve(previous.data_, this->data_, services);
            
            // Create a proposal distribution
            this->data_.proposal = services.proposal_scheduler->Solve(previous.data_, this->data_, services);

            // Update the previous chain's importance weights. This is used in determining
            // the acceptance probability for candidate samples of this chain.
            double delta_beta = this->data_.beta - previous.data_.beta; 
            previous.data_.importance_weight_local = utilities::NormalizedImportanceWeights(
                delta_beta, previous.data_);

            // Initialize the chain using the maximum likelihood sample from the previous chain
            std::vector<double>::iterator max_iterator = std::max_element(
                previous.data_.log_likelihood.begin(), previous.data_.log_likelihood.end());
            uint64_t max_index = std::distance(previous.data_.log_likelihood.begin(), max_iterator);
            std::vector<double> sample_base = previous.data_.sample[max_index];
            std::vector<double> sample_initial = this->data_.proposal->Sample(sample_base, *services.random_engine);
            double sample_initial_log_likelihood = services.forward_model->LogLikelihood(sample_initial);

            // Push back initial values for chain
            this->data_.sample.push_back(sample_initial);
            this->data_.log_likelihood.push_back(sample_initial_log_likelihood);
            this->data_.accepted.push_back(true);
            this->data_.random_integer.push_back(max_index);
            this->data_.acceptance_probability.push_back(1.0);

            // Uniform distribution for accepting / rejecting candidate
            std::uniform_real_distribution<double> distribution_uniform(0.0, 1.0);

            for(uint64_t n = 1; n < this->data_.num_samples; n++)
            {
                // Generate a candidate from the sample with the maximum importance weight from the previous chain
                uint64_t k = utilities::IntegerAtRandom(previous.data_.importance_weight_local, *services.random_engine, true);
                std::vector<double> candidate = this->data_.proposal->Sample(previous.data_.sample[k], *services.random_engine);

                // Check that the candidate lies in the bounds defined by the prior
                // This is important in the case of a candidate falling outside of a uniform hyperrectangle
                // Such a candidate should be given zero probability of acceptance
                //if(this->prior_->LogPDF(candidate) == -std::numeric_limits<double>::infinity())
                //{
                //    acceptance_probability = 0.0;
                //    candidate_log_likelihood = -std::numeric_limits<double>::infinity();
                //}
                //else
                //{
                double candidate_log_likelihood = services.forward_model->LogLikelihood(candidate);
                double acceptance_probability = services.acceptance_probability_service->Solve(
                    previous.data_, this->data_, services, candidate, candidate_log_likelihood, k);
                double uniform_number = distribution_uniform(*services.random_engine);
                //}
                // OR DO THIS CHECK IN THE ACCEPTANCE PROBABILITY SERVICE ITSELF!!!
                if(uniform_number < acceptance_probability)
                {
                    this->data_.sample.push_back(candidate);
                    this->data_.log_likelihood.push_back(candidate_log_likelihood);
                    this->data_.accepted.push_back(true);
                }
                else
                {
                    this->data_.sample.push_back(this->data_.sample.back());
                    this->data_.log_likelihood.push_back(this->data_.log_likelihood.back());
                    this->data_.accepted.push_back(false);
                }
                this->data_.random_integer.push_back(k);
                this->data_.acceptance_probability.push_back(acceptance_probability);
            } // END of for-loop over sample index, n

            // Calculate the importance weights relative to the posterior
            this->data_.importance_weight_posterior = utilities::NormalizedImportanceWeights(1.0 - this->data_.beta, this->data_);

            // Calculate the number of effective samples
            this->data_.num_samples_effective = exp(-utilities::LogSumExpScale(2.0, this->data_.importance_weight_posterior));

            services.beta_scheduler->Update(this->data_, services);

            if(this->data_.done)
            {
                this->data_.importance_weight_local.resize(this->data_.num_samples, -log(this->data_.num_samples));
            }

            // Consistency checks (OPTIONAL)
            // - Check if every vector in data_ is of the same size.
            // - Check that every element in data_.sample is of the same size.
            // - Check that every acceptance probability is non-NaN and positive.
        }
        catch(std::logic_error &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }

    void Chain::Print(ChainServices &services, Chain &chain)
    {
        services.chain_printer->Print(services, chain.data_);
    }

    bool Chain::Done()
    {
        return this->data_.done;
    }
}
