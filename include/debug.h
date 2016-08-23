/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

namespace aims
{
    double DebugValue(std::vector<double> const &v, uint64_t pos)
    { return v[pos]; }

    void DebugPrint(std::vector<double> const &x)
    {
        for(uint64_t i = 0u; i < x.size(); i++)
            std::cout << "[" << i << ":" << x[i] << "]";
        std::cout << std::endl;
    }

    void DebugStats(std::vector<double> const &v)
    {
        const uint64_t n = v.size();
        double value_mean = 0.0;
        double value_max = v[0];
        double value_min = v[0];
        double value_stdv = 0.0;
        double value_cov = 0.0;

        for(uint64_t i = 0u; i < n; i++)
        {
            value_mean += v[i];
            value_stdv += v[i] * v[i];
            value_min = value_min < v[i] ? value_min : v[i];
            value_max = value_max > v[i] ? value_max : v[i];
        }
        value_mean /= (double) n;
        value_stdv = sqrt(value_stdv / (double)n  - value_mean * value_mean);
        value_cov = value_stdv / value_mean * 100.0;

        std::cout << "Mean = " << value_mean << std::endl;
        std::cout << "Stdv = " << value_stdv << std::endl;
        std::cout << "Coev = " << value_cov << "%" << std::endl;
        std::cout << "Min. = " << value_min << std::endl;
        std::cout << "Max. = " << value_max << std::endl;
        std::cout << std::endl;
    }
}