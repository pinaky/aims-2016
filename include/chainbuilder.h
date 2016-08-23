/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <stdexcept>
#include <memory>
#include "chain.h"
#include "acceptanceprobabilityservice.h"
#include "numsamplesscheduler.h"
#include "forwardmodel.h"
#include "betascheduler.h"
#include <random>

namespace aims
{
	class ChainBuilder
	{
		protected:
			std::unique_ptr<aims::NumSamplesScheduler> num_samples_scheduler_;
			std::unique_ptr<aims::ForwardModel> forward_model_;
			std::unique_ptr<aims::ProposalScheduler> proposal_scheduler_;
			std::unique_ptr<aims::BetaScheduler> beta_scheduler_;
			std::unique_ptr<aims::AcceptanceProbabilityService> acceptance_probability_service_;
			std::unique_ptr<std::mt19937_64> random_engine_;
			std::unique_ptr<aims::ChainPrinter> chain_printer_;
		public:
			ChainBuilder(): num_samples_scheduler_(nullptr), forward_model_(nullptr), proposal_scheduler_(nullptr), 
				beta_scheduler_(nullptr), acceptance_probability_service_(nullptr), random_engine_(nullptr) {}

            ChainBuilder& SetNumSamplesScheduler(std::unique_ptr<aims::NumSamplesScheduler> num_samples_scheduler)
			{this->num_samples_scheduler_ = std::move(num_samples_scheduler); return *this; }
			
			ChainBuilder& SetForwardModel(std::unique_ptr<aims::ForwardModel> forward_model)
			{ this->forward_model_ = std::move(forward_model); return *this; }
			
			ChainBuilder& SetProposalScheduler(std::unique_ptr<aims::ProposalScheduler> proposal_scheduler) 
			{this->proposal_scheduler_ = std::move(proposal_scheduler); return *this; }
			
			ChainBuilder& SetBetaScheduler(std::unique_ptr<aims::BetaScheduler> beta_scheduler) 
			{this->beta_scheduler_ = std::move(beta_scheduler); return *this; }

			ChainBuilder& SetAcceptanceProbabilityService(std::unique_ptr<AcceptanceProbabilityService> acceptance_probability_service)
			{this->acceptance_probability_service_ = std::move(acceptance_probability_service); return *this;}

			ChainBuilder& SetRandomEngine(std::unique_ptr<std::mt19937_64> random_engine)
			{this->random_engine_ = std::move(random_engine); return *this; }

			ChainBuilder& SetChainPrinter(std::unique_ptr<ChainPrinter> chain_printer)
			{this->chain_printer_ = std::move(chain_printer); return *this; }

            void Checker()
            {
				if(!num_samples_scheduler_) 
					throw std::logic_error("Error: NumSamplesScheduler pointer is null in ChainBuilder.");
				if(!forward_model_) 
					throw std::logic_error("Error: ForwardModel pointer is null in ChainBuilder.");
				if(!proposal_scheduler_) 
					throw std::logic_error("Error: ProposalScheduler pointer is null in ChainBuilder.");
				if(!beta_scheduler_) 
					throw std::logic_error("Error: BetaScheduler pointer is null in ChainBuilder.");
				if(!random_engine_) 
					throw std::logic_error("Error: RandomEngine pointer is null in ChainBuilder.");
				if(!acceptance_probability_service_) 
					throw std::logic_error("Error: AcceptanceProbabilityService pointer is null in ChainBuilder.");
				if(!chain_printer_) 
					throw std::logic_error("Error: ChainPrinter pointer is null in ChainBuilder.");
            }

			virtual std::unique_ptr<Chain> Build() = 0;
	};
}
