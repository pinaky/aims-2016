/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "forwardmodel_gaussian.h"
#include <stdexcept>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _OPENMP
    #include <omp.h>
#endif

namespace aims
{
    ForwardModel_Gaussian::ForwardModel_Gaussian(uint64_t num_data, double mean, double stddev)
    {
        if(!(num_data > 0)) throw std::logic_error(
            "Error: Number of data points to ForwardModel_Gaussian constructor shoud be positive.");
        if(!(stddev > 0.0)) throw std::logic_error(
            "Error: Stddev in constructore of ForwardModel_Gaussian should be positive");

        this->num_data_ = num_data;
        this->mean_ = mean;
        this->stddev_ = stddev;

        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::normal_distribution<double> distribution_normal(mean, stddev);

        for(uint64_t n = 0; n < num_data; n++)
            this->data_.push_back(distribution_normal(gen));

    }

    double ForwardModel_Gaussian::LogLikelihood(sample_type const &x)
    {
        const uint64_t num_data = this->num_data_;
        const double num_samples = (double) num_data;
        const double stddev = exp(x[1]);
        const double variance = stddev * stddev;
        const double stddev_r = 1.0 / stddev;
        const double factor = M_SQRT1_2 * stddev_r;
        double value = -0.5 * num_samples * log(2.0 * M_PI * variance);

        #pragma omp parallel
        {
            double sum = 0.0;

            #pragma omp for
            for(uint64_t n = 0; n < num_data; n++)
            {
                double term = (x[0] - this->data_[n]) * factor;
                sum -= term * term;
            }

            #pragma omp atomic
            value += sum;
        }

        return value;
    }
}