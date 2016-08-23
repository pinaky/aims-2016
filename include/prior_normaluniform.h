/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once

#include <stdint.h>
#include <vector>
#include <random>
#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "prior.h"

namespace aims
{
	class Prior_NormalUniform : public Prior
	{
	private:
		std::pair<std::pair<double, double> > parameters_;
		std::normal_distribution<double> distribution_normal_;
		std::uniform_real_distribution<double> distribution_uniform_;
	public:
		PriorNormalUniform(std::pair<std::pair<double, double> > const &);
		double LogPDF(std::vector<double> const &);
		std::vector<std::vector<double> > Sample(uint64_t, std::mt19937_64 &);
	};
}
