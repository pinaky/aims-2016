/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <memory>
#include "chaindata.h"
#include "chainservices.h"
#include "proposalscheduler.h"
#include "proposal_rwmhisotropicnormal.h"


namespace aims
{
    class ProposalScheduler_ConstantRWMHIsotropicNormal: public ProposalScheduler
    {
        private:
            double stddev_;
        public:
            ProposalScheduler_ConstantRWMHIsotropicNormal(double);
            std::unique_ptr<Proposal> Solve(ChainData const &, ChainData const &, ChainServices const &);
    };
}