/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


 
#include "prior_generaluniform.h"

namespace aims
{
	Prior_GeneralUniform::Prior_GeneralUniform(std::vector<std::pair<double, double> > bounds)
	{
		uint64_t i;
		if(bounds.empty()) throw std::logic_error("Empty parameters in PriorGeneralUniform constructor.");
		this->d = bounds.size();

		this->fac = 0.0;

		for (i = 0u; i < this->d; i++)
		{
			double lower_bound = bounds[i].first;
			double upper_bound = bounds[i].second;
			fac -= log(upper_bound - lower_bound);
			//assert(p[2u * i] < p[2u * i + 1]);
			//fac -= log(p[2u * i + 1] - p[2u * i]);
		}


		std::uniform_real_distribution<double> distribution_uniform(0.0, 1.0);
		this->distribution_uniform_.param(distribution_uniform.param());
		
		this->bounds_ = bounds;
	}

	double Prior_GeneralUniform::LogPDF(std::vector<double> const &x) const
	{
		uint64_t i;

		bool cond = true;
		for (i = 0u; i < this->d; i++)
		{
			double lower_bound = this->bounds_[i].first;
			double upper_bound = this->bounds_[i].second;
			cond = cond && (x[i] > lower_bound && x[i] < upper_bound);
			if (!cond) return (-std::numeric_limits<double>::infinity());
		}

		return this->fac;
	}

	std::vector<std::vector<double> > Prior_GeneralUniform::Sample(uint64_t num_samples,
		std::mt19937_64 &random_engine)
	{
		uint64_t i, j;

		std::vector<double> v(this->d);
		std::vector<std::vector<double> > val(num_samples, v);

		for (i = 0u; i < num_samples; i++)
		{
			for (j = 0u; j < this->d; j++)
			{
				double lower_bound = this->bounds_[j].first;
				double upper_bound = this->bounds_[j].second;
				val[i][j] = lower_bound + (upper_bound - lower_bound) * this->distribution_uniform_(random_engine);
			}
		}

		return val;
	}
}
