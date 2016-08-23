/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "forwardmodel.h"
#include <vector>
#include <random>

namespace aims
{
    class ForwardModel_Gaussian: public ForwardModel
    {
        private:
            uint64_t num_data_;
            double mean_;
            double stddev_;
            sample_type data_;
        public:
            ForwardModel_Gaussian(uint64_t, double, double);
            double LogLikelihood(sample_type const &);
    };
}