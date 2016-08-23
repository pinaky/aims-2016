/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once

//#include "prior.h"
//#include "numsamplesscheduler.h"
//#include "forwardmodel.h"
//#include "betascheduler.h"
//#include "proposalscheduler.h"
//#include "acceptanceprobabilityservice.h"
#include <random>
#include <memory>

namespace aims
{
	class AcceptanceProbabilityService;
	class NumSamplesScheduler;
	class ForwardModel;
	class BetaScheduler;
	class ProposalScheduler;
	class Prior;
	class ChainPrinter;

    class ChainServices
    {
        public:
        	std::unique_ptr<AcceptanceProbabilityService> acceptance_probability_service;
			std::unique_ptr<NumSamplesScheduler> num_samples_scheduler;
			std::unique_ptr<ForwardModel> forward_model;
			std::unique_ptr<BetaScheduler> beta_scheduler;
			std::unique_ptr<ProposalScheduler> proposal_scheduler;
			std::unique_ptr<Prior> prior;
			std::unique_ptr<std::mt19937_64> random_engine;
			std::unique_ptr<ChainPrinter> chain_printer;
    };
}