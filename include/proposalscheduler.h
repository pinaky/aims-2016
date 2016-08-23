/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "proposal.h"
#include <memory>

namespace aims
{
    class ProposalScheduler
    {
        public:
            virtual std::unique_ptr<Proposal> Solve(ChainData const &, 
                ChainData const &, ChainServices const &) = 0;
    };
}
