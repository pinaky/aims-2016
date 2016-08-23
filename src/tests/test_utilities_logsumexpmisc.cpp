/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "test_header.h"
#include "utilities.h"
#include <vector>
#include <stdexcept>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

namespace aimstesting
{
    typedef std::pair<double, double> signed_logarithm;
    void test_utilities_logsumexpmisc()
    {
        const double epstol = 1.0E-10;

        const uint64_t n = 10;
        std::vector<double> x(n, 1.0);
        double logsumexp_expected = log(n) + 1.0;
        double logsumexp = aims::utilities::LogSumExp(x);
        double logsumexpscale_expected = log(n) + 2.0;
        double logsumexpscale = aims::utilities::LogSumExpScale(2.0 ,x);

        std::vector<signed_logarithm> y;
        y.push_back(signed_logarithm(0.0, -1.0));
        y.push_back(signed_logarithm(1.0, 1.0));
        double logsumexpsign_expected = log(M_E - 1.0);
        signed_logarithm logsumexpsign = aims::utilities::LogSumExpSign(y);
        double logsumexpsignscale_expected = log(M_E * M_E - 1.0);
        signed_logarithm logsumexpsignscale = aims::utilities::LogSumExpScaleSign(2.0, y);

        try
        {
            if(!(std::abs(logsumexp_expected - logsumexp) < epstol))
                throw std::logic_error("LogSumExp did not match.");
            if(!(std::abs(logsumexpscale_expected - logsumexpscale) < epstol))
                throw std::logic_error("LogSumExpScale did not match");
            if(!(std::abs(logsumexpsign_expected - logsumexpsign.first) < epstol))
                throw std::logic_error("LogSumExpSign did not match.");
            if(!(std::abs(logsumexpsignscale_expected - logsumexpsignscale.first) < epstol))
                throw std::logic_error("LogSumExpScaleSign did not match.");
        }
        catch(std::logic_error &e)
        {
            std::cerr << "Test test_utilities_logsumexpmisc failed:" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}