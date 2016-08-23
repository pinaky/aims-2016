/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "numsamplesscheduler_constant.h"

namespace aims
{
    NumSamplesScheduler_Constant::NumSamplesScheduler_Constant(uint64_t num_samples_constant) :
        num_samples_constant_(num_samples_constant) {}

    uint64_t NumSamplesScheduler_Constant::Solve(ChainData const &data_previous, 
        ChainData const &, ChainServices const &) 
    {
        return this->num_samples_constant_; 
    }
}