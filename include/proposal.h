/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include <stdint.h>
#include <random>

namespace aims
{
	typedef std::vector<double> sample_type;
	class Proposal
	{
		public:
			virtual ~Proposal() {}
			virtual double LogPDF(sample_type const &, sample_type const &) const = 0;
			virtual sample_type Sample(sample_type const &, std::mt19937_64 &) = 0;
	};
}
