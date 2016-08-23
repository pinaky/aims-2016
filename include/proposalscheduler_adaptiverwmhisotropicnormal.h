/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <memory>
#include "chaindata.h"
#include "chainservices.h"
#include "proposal_rwmhistropicnormal.h"

namespace aims
{
    class ProposalScheduler_AdaptiveRWMHIsotropicNormal
    {
        private:
            bool verbose_;
            double test_fraction_;
        public:
            ProposalScheduler_AdaptiveRWMHIsotropicNormal(bool verbose): verbose_(verbose) {}
            std::unique_ptr<Proposal> Solve(ChainData const &, ChainData const &, ChainServices const &);
    };
}