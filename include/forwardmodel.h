/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include <iostream>

namespace aims
{
	typedef std::vector<double> sample_type;
	class ForwardModel
	{
	public:
		virtual ~ForwardModel() {}
		virtual double LogLikelihood(std::vector<double> const &) = 0;
	};
}