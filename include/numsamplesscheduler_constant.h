/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "numsamplesscheduler.h"
// An implementation of the number of samples scheduler
// that simply returns a constant.

namespace aims
{
    class NumSamplesScheduler_Constant: public NumSamplesScheduler
    {
        private:
            uint64_t num_samples_constant_;
        public:
            NumSamplesScheduler_Constant(uint64_t num_samples_constant_);
            uint64_t Solve(ChainData const &, ChainData const &, ChainServices const &);
    };
}