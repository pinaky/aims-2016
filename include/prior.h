/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once

//#include "stdafx.h"
#include <stdint.h>
#include <vector>
#include <random>
#include <iostream>

namespace aims
{
	typedef std::vector<double> sample_type;
	class Prior
	{
		public:
			// 2 methods are needed for any distribution, usually:
			// 1) A method to evaluate the (log)PDF at a point
			virtual ~Prior() {}
			virtual double LogPDF(sample_type const &) const = 0;
			// 2) A method to draw a bunch of samples
			virtual std::vector<sample_type> Sample(uint64_t, std::mt19937_64 &) = 0;
	};
}