/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "forwardmodel_gaussian.h"
#include "inputtoken.h"

namespace aims
{
    class ForwardModelBuilder_Gaussian
    {
        private:
            double mean_;
            double stddev_;
            uint64_t num_samples_;
        public:
            ForwardModelBuilder_Gaussian(): mean_(0.0), stddev_(1.0) {}
            ForwardModelBuilder_Gaussian& SetMean(double mean) {this->mean_ = mean; return *this;}
            ForwardModelBuilder_Gaussian& SetStdDev(double stddev) {this->stddev_ = stddev; return *this;}
            ForwardModelBuilder_Gaussian& SetNumSamples(double num_samples) {this->num_samples_ = num_samples; return *this;}

            std::unique_ptr<ForwardModel> BuildFromInput()
            {
                std::cout << "GAUSSIAN FORWARD MODEL INFORMATION:" << std::endl;
                uint64_t num_samples = InputToken<uint64_t>("Enter a value for the number of data points: ").GetValue(); 
                double mean = InputToken<double>("Enter a value for the data-generating mean of the Gaussian: ").GetValue();
                double stddev = InputToken<double>("Enter a value for the data-generating stddev of the Gaussian: ").GetValue();
                std::unique_ptr<ForwardModel> x(new ForwardModel_Gaussian(num_samples, mean, stddev));
                return x;
            }
    };
}