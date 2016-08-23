/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "utilities.h"
#include "proposal_rwmhisotropicnormal.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace aims
{
		// Constructor:
		// - Takes in the isotropic standard deviation for this isotropic normal
		Proposal_RWMHIsotropicNormal::Proposal_RWMHIsotropicNormal(double const &s)
		{
			// Check that the isotropic stddev is positive
			if(!(s > 0.0)) throw std::logic_error("Expected positive standard deviation parameter in Proposal_RWMHIsotropicNormal constructor");

			// Initialize the sampler with the supplied isotropic stdv in p[0][0]
			std::normal_distribution<double> distribution_normal_(0.0, s);
			this->distribution_normal_.param(distribution_normal_.param());
		}

		double Proposal_RWMHIsotropicNormal::LogPDF(sample_type const &sample, sample_type const &sample_base) const
		{
			// //q(sample|sample_base) = normpdf(sample|mean = sample_base, variance = parameter_stddev ^ 2)
			const uint64_t num_dimensions = sample.size();
			if(!(num_dimensions > 0)) throw std::logic_error("Number of dimensions to Proposal_RWHMIsotropicNormal::LogPDF should be positive");
			if(!(num_dimensions == sample_base.size())) throw std::logic_error("Inconsistent sizes in Proposal_RWMHIsotropicNormal::LogPDF");

			const double num_dimensions_double = (double) num_dimensions;
			const double variance = parameter_stddev * parameter_stddev;
			const double variance_reciprocal = 1.0 / variance;
			const double factor = num_dimensions_double * log(2.0 * M_PI * variance);

			double distance_squared = utilities::L2DistanceSquared(sample, sample_base);

			double value = factor + distance_squared * variance_reciprocal;
			value = -0.5 * value;
			return value;
		}

		sample_type Proposal_RWMHIsotropicNormal::Sample(sample_type const &sample_base, std::mt19937_64 &random_engine)
		{
			uint64_t num_dimensions = sample_base.size();
			// Draw a sample from the RWMH proposal about the mean sample_base (supplied as an argument)
			// value ~ Normal(.|mean = sample_base, co-variance = (variance = parameter_stddev ^ 2) * Identity)
			sample_type value(num_dimensions);

			for (uint64_t n = 0; n < num_dimensions; n++)
				value[n] = sample_base[n] + this->distribution_normal_(random_engine);

			return value;
		}
}
