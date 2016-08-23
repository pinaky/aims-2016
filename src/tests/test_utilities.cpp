/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "test_header.h"
#include "utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits> 

namespace aimstesting
{
    void CompareValues(std::string s, double a, double b, double epstol)
    {
        const double INF = std::numeric_limits<double>::infinity();

        std::ostringstream os("");

        double error = std::abs(a - b) / std::abs(a);
        if((std::abs(a) != INF && std::abs(b) != INF && error > epstol && !std::isnan(a) && !std::isnan(b)) ||
        ((std::abs(a) == INF || std::abs(b) == INF) && (a != b) && !std::isnan(a) && !std::isnan(b)) ||
        (std::isnan(a) && !std::isnan(b)) || (std::isnan(b) && !(std::isnan(a))))
        { 
            os << "Case " << s << " does not match" << std::endl; 
            os << "arg1: " << a << "; arg2: " << b << std::endl;
            throw std::logic_error(os.str()); 
        }
    }

    void test_utilities()
    {
        test_utilities_simplemoment_template();
        test_utilities_simplemoment_samples();
        test_utilities_l2misc();
        test_utilities_logsumexpmisc();
        test_utilities_betafunctionandderivative();
       /* test_utilities_integeratrandom();
        test_utilities_normalizedimportanceweights();
        test_utilities_betafunctionandderivative();
        test_utilities_betabinarysearch();
        test_utilities_betanewtonraphsonrefine();
        test_utilities_acceptanceprobabilitybasic();*/
    }
}