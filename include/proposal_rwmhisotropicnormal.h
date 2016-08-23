/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <iostream>
#include "proposal.h"

namespace aims
{
	class Proposal_RWMHIsotropicNormal : public Proposal
	{
	private:
		double parameter_stddev;
		std::normal_distribution<double> distribution_normal_;
	public:
		Proposal_RWMHIsotropicNormal(double const &);
		double LogPDF(sample_type const &, sample_type const &) const;
		sample_type Sample(sample_type const &, std::mt19937_64 &);
		double GetStdDev() const { return this->parameter_stddev; }
		void SetStdDev(double stddev) {this->parameter_stddev = stddev; return;}
	};
}
