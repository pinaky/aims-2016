/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "prior_generaluniform.h"
#include <sstream>

namespace aims
{
    class PriorBuilder_GeneralUniform
    {
        private:
            std::vector<std::pair<double, double> > bounds_;
        public:
            PriorBuilder_GeneralUniform() {}
            std::unique_ptr<Prior> BuildFromInput()
            {
                double num_dim = InputToken<uint64_t>("Enter the number of dimensions: ").GetValue();
                for(uint64_t i = 0; i < num_dim; i++)
                {
                    std::ostringstream os("");
                    os << "Enter the lower bound for dimension " << i << ": ";
                    double lower_limit = InputToken<double>(os.str()).GetValue();
                    os.str("");
                    os.clear();

                    os << "Enter the upper bound for dimension " << i << ": ";
                    double upper_limit = InputToken<double>(os.str()).GetValue();

                    this->bounds_.push_back(std::pair<double, double>(lower_limit, upper_limit));
                }

                std::unique_ptr<Prior> x(new Prior_GeneralUniform(this->bounds_));
                return x;
            }
    };
}