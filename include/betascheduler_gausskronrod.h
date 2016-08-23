/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "betascheduler.h"
#include <stdint.h>
#include <string>
#include <map>
#include "chainservices.h"
#include <iostream>

namespace aims
{
	struct BetaData
	{
		uint64_t j;
		double beta;
		bool is_gauss_point;
		bool is_kronrod_point;
		int gauss_point_number;
		int kronrod_point_number;
		double mean_log_likelihood;

	};

	class BetaScheduler_GaussKronrod: public BetaScheduler
	{
		private:
			uint64_t last_gk_point_;
			bool done_;
			bool triggered_;
			std::vector<BetaData> beta_data_;
			std::map<std::string, double> integral_data_;
			std::vector<std::pair<double, double> > gauss_data_;
			std::vector<std::pair<double, double> > kronrod_data_;
			void Integrate();
		public:
			BetaScheduler_GaussKronrod(double);
			double Solve(ChainData const &, ChainData const &, ChainServices const &);
			bool Done();
			void Update(ChainData const &, ChainServices const &);
			void Print(ChainServices &);
			std::map<std::string, double> IntegralData();
			#ifdef DEBUG
				void PrintBetaData();
			#endif
	};
}