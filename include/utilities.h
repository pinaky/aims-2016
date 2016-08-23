/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include <random>
#include "chaindata.h"
#include "chainservices.h"
#include "prior.h"
#include "proposal.h"
#include <memory>

namespace aims
{
	typedef std::pair<double, double> signed_logarithm;
	typedef std::vector<double> sample_type;

	namespace utilities
	{
		template<typename T> void PrintArrayJSON(
			std::ostream &os, std::string name, std::vector<T> const &val,
			uint64_t num_per_line, uint64_t num_tabs, bool end_comma)
		{
			uint64_t i, k, n = val.size();
			
			std::string tabs(""), tabspp, comma("");
			for(i = 0u; i < num_tabs; i++) tabs += "\t";
			tabspp = tabs + "\t";
			if(end_comma) comma += ",";

			os << tabs << "\"" << name << "\" : [" << std::endl << tabspp;
			bool flag = false;
			for (i = 0u; i < n; i += num_per_line)
			{
				for (k = 0u; k < num_per_line; k++)
				{
					if (i + k == n - 1)
					{
						os << val[i + k] << std::endl;
						os << tabs << "]" << comma << std::endl;
						flag = true;
						break;
					}
					else os << val[i + k] << ",";
				}
				if (flag == true) break;
				os << std::endl << tabspp;
			}
		}

		template<typename T> double SimpleMoment(std::vector<T> const &x, uint64_t m)
		{
			uint64_t i, j;
			double val = 0.0;

			for (i = 0u; i < x.size(); i++)
			{
				double term = 1.0;
				for (j = 0u; j < m; j++)
					term *= (double)x[i];

				val += term;
			}

			val /= (double)(x.size());

			return val;
		}

		sample_type SimpleMoment(std::vector<sample_type> const &, uint64_t);
		double L2Norm(std::vector<double> const &);
		double L2Distance(std::vector<double> const &, std::vector<double> const &);
		double L2NormSquared(std::vector<double> const &);
		double L2DistanceSquared(std::vector<double> const &, std::vector<double> const &);
		double LogSumExp(std::vector<double> const &);
		signed_logarithm LogSumExpSign(std::vector<signed_logarithm> const &);
		double LogSumExpScale(double, std::vector<double> const &);
		signed_logarithm LogSumExpScaleSign(double, std::vector<signed_logarithm> const &);
		uint64_t IntegerAtRandom(std::vector<double> const &, std::mt19937_64 &, bool);
		std::vector<double> NormalizedImportanceWeights(double, aims::ChainData const &);
		std::pair<signed_logarithm, signed_logarithm> BetaFunctionAndDerivative(std::vector<double> const &,
			std::vector<signed_logarithm> const &, double, double , bool);
		double BetaBinarySearch(aims::ChainData const &, double );
		double BetaNewtonRaphsonRefine(aims::ChainData const &, double);
		double AcceptanceProbabilityBasic(ChainData const &, ChainData const &, ChainServices const &,
			sample_type const &, double , uint64_t);
		
	}
}