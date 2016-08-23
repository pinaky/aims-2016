/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <stdexcept>
#include <memory>
#include "chainservices.h"
#include "acceptanceprobabilityservice.h"
#include "numsamplesscheduler.h"
#include "forwardmodel.h"
#include "betascheduler.h"
#include <random>

namespace aims
{
	class ChainServicesBuilder
	{
		protected:
            std::unique_ptr<aims::Prior> prior_;
			std::unique_ptr<aims::NumSamplesScheduler> num_samples_scheduler_;
			std::unique_ptr<aims::ForwardModel> forward_model_;
			std::unique_ptr<aims::ProposalScheduler> proposal_scheduler_;
			std::unique_ptr<aims::BetaScheduler> beta_scheduler_;
			std::unique_ptr<aims::AcceptanceProbabilityService> acceptance_probability_service_;
			std::unique_ptr<std::mt19937_64> random_engine_;
			std::unique_ptr<aims::ChainPrinter> chain_printer_;
		public:
			ChainServicesBuilder(): prior_(nullptr), num_samples_scheduler_(nullptr), forward_model_(nullptr), proposal_scheduler_(nullptr), 
				beta_scheduler_(nullptr), acceptance_probability_service_(nullptr), random_engine_(nullptr) {}

            ChainServicesBuilder& SetPrior(std::unique_ptr<aims::Prior> prior) 
            { this->prior_ = std::move(prior); return *this; }

            ChainServicesBuilder& SetNumSamplesScheduler(std::unique_ptr<aims::NumSamplesScheduler> num_samples_scheduler)
			{this->num_samples_scheduler_ = std::move(num_samples_scheduler); return *this; }
			
			ChainServicesBuilder& SetForwardModel(std::unique_ptr<aims::ForwardModel> forward_model)
			{ this->forward_model_ = std::move(forward_model); return *this; }
			
			ChainServicesBuilder& SetProposalScheduler(std::unique_ptr<aims::ProposalScheduler> proposal_scheduler) 
			{this->proposal_scheduler_ = std::move(proposal_scheduler); return *this; }
			
			ChainServicesBuilder& SetBetaScheduler(std::unique_ptr<aims::BetaScheduler> beta_scheduler) 
			{this->beta_scheduler_ = std::move(beta_scheduler); return *this; }

			ChainServicesBuilder& SetAcceptanceProbabilityService(std::unique_ptr<AcceptanceProbabilityService> acceptance_probability_service)
			{this->acceptance_probability_service_ = std::move(acceptance_probability_service); return *this;}

			ChainServicesBuilder& SetRandomEngine(std::unique_ptr<std::mt19937_64> random_engine)
			{this->random_engine_ = std::move(random_engine); return *this; }

			ChainServicesBuilder& SetChainPrinter(std::unique_ptr<ChainPrinter> chain_printer)
			{this->chain_printer_ = std::move(chain_printer); return *this; }

            void Checker()
            {
				if(!this->num_samples_scheduler_) 
					throw std::logic_error("Error: NumSamplesScheduler pointer is null in ChainServicesBuilder.");
				if(!this->forward_model_) 
					throw std::logic_error("Error: ForwardModel pointer is null in ChainServicesBuilder.");
				if(!this->proposal_scheduler_) 
					throw std::logic_error("Error: ProposalScheduler pointer is null in ChainServicesBuilder.");
				if(!this->beta_scheduler_) 
					throw std::logic_error("Error: BetaScheduler pointer is null in ChainServicesBuilder.");
				if(!this->random_engine_) 
					throw std::logic_error("Error: RandomEngine pointer is null in ChainServicesBuilder.");
				if(!this->acceptance_probability_service_) 
					throw std::logic_error("Error: AcceptanceProbabilityService pointer is null in ChainServicesBuilder.");
				if(!this->chain_printer_) 
					throw std::logic_error("Error: ChainPrinter pointer is null in ChainServicesBuilder.");
                if(!this->prior_) 
                    throw std::logic_error("Error: The Prior pointer is null in ChainServicesBuilder.");
            }

            

            std::unique_ptr<ChainServices> Build()
            {
                try
                {
                    ChainServicesBuilder::Checker();
                    
                }
                catch(std::logic_error &e)
                {
                    std::cerr << "Error: One of the pointers in ChainServicesBuilder is null." << std::endl;
                    std::cerr << e.what() << std::endl;
                }
                
                std::unique_ptr<ChainServices> chain_services(new ChainServices());
                chain_services->acceptance_probability_service = std::move(this->acceptance_probability_service_);
                chain_services->num_samples_scheduler = std::move(this->num_samples_scheduler_);
                chain_services->forward_model = std::move(this->forward_model_);
                chain_services->beta_scheduler = std::move(this->beta_scheduler_);
                chain_services->proposal_scheduler = std::move(this->proposal_scheduler_);
                chain_services->prior = std::move(this->prior_);
                chain_services->random_engine = std::move(this->random_engine_);
                chain_services->chain_printer = std::move(this->chain_printer_);

                return std::move(chain_services);
            }
	};
}
