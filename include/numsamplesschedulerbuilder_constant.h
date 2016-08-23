/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "inputtoken.h"
#include "numsamplesscheduler_constant.h"

namespace aims
{
    class NumSamplesSchedulerBuilder_Constant
    {
        private:
            uint64_t num_samples_;
        public:

            NumSamplesSchedulerBuilder_Constant(): num_samples_(100) {}
            NumSamplesSchedulerBuilder_Constant& SetNumSamples(double const &num_samples) {this->num_samples_ = num_samples; return *this;}

            std::unique_ptr<NumSamplesScheduler> BuildFromInput()
            {
                std::cout << "SCHEDULER FOR NUMBER OF SAMPLES INFORMATION:" << std::endl;
                this->num_samples_ = InputToken<uint64_t>("Enter the number of samples for the constant sampler: ").GetValue();
                std::unique_ptr<NumSamplesScheduler> x(new NumSamplesScheduler_Constant(this->num_samples_));
                return x;
            }

            std::unique_ptr<NumSamplesScheduler> Build()
            {
                std::unique_ptr<NumSamplesScheduler> x(new NumSamplesScheduler_Constant(this->num_samples_));
                return x;
            }
    };
}