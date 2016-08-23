/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once

#include "prior.h"

namespace aims
{
	class Prior_GeneralUniform: public Prior
	{
	private:
		uint64_t d;
		double fac;
		std::vector<std::pair<double, double> > bounds_;
		std::uniform_real_distribution<double> distribution_uniform_;
	public:
		Prior_GeneralUniform(std::vector<std::pair<double, double> >);
		double LogPDF(std::vector<double> const &) const;
		std::vector<std::vector<double> > Sample(uint64_t, std::mt19937_64 &);
	};
}
