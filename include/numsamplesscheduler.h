/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chaindata.h"
#include "chainservices.h"

// This class calculates the number of samples for the next
// AIMS chain based on information from the previous chain.

namespace aims
{
    class NumSamplesScheduler
    {
        public:
            virtual ~NumSamplesScheduler() {}
            virtual uint64_t Solve(ChainData const &, ChainData const &, ChainServices const &) = 0;
    };
}