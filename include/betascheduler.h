/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chainservices.h"
#include "chaindata.h"
#include <string>
#include <map>

namespace aims
{
	class BetaScheduler
	{
		protected:
			double gamma_;
		public:
			// Data flow would happen through the constructor
			BetaScheduler(double gamma): gamma_(gamma) {}
			virtual ~BetaScheduler() {};
			virtual double Solve(ChainData const &, ChainData const &, ChainServices const &) = 0;
			virtual bool Done() = 0;
			virtual void Update(ChainData const &, ChainServices const &) = 0;
			virtual void Print(ChainServices &) = 0;
			virtual std::map<std::string, double> IntegralData() = 0;
	};
}