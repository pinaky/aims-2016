/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "test_header.h"
#include "utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
namespace aimstesting
{
    void test_utilities_l2misc()
    {
        const double epstol = 1.0E-10;

        std::vector<double> x;
        x.push_back(1.0);
        x.push_back(1.0);
        x.push_back(1.0);
        x.push_back(1.0);
        x.push_back(2.0);

        std::vector<double> y;
        y.push_back(1.0);
        y.push_back(1.0);
        y.push_back(1.0);
        y.push_back(1.0);
        y.push_back(2.0);

        double x_l2norm_expected = 2.0 * M_SQRT2;
        double x_l2normsquared_expected = 8.0;
        double xy_l2distance_expected = 0.0;
        double xy_l2distancesquared_expected = 0.0;

        try
        {
            double x_l2norm = aims::utilities::L2Norm(x);
            double x_l2normsquared = aims::utilities::L2NormSquared(x);
            double xy_distance = aims::utilities::L2Distance(x, y);
            double xy_distancesquared = aims::utilities::L2DistanceSquared(x, y);
            double xy_distance_rev = aims::utilities::L2Distance(y, x);

            if(!(std::abs(x_l2norm - x_l2norm_expected) < epstol))
                throw std::logic_error("L2Norm did not match.");

            if(!(std::abs(x_l2normsquared - x_l2normsquared_expected) < epstol))
                throw std::logic_error("L2NormSquared did not match.");

            if(!(std::abs(xy_distance - xy_l2distance_expected) < epstol))
                throw std::logic_error("L2Distance did not match.");

            if(!(std::abs(xy_distancesquared - xy_l2distancesquared_expected) < epstol))
                throw std::logic_error("L2DistanceSquared did not match.");

            if(!(std::abs(xy_l2distance_expected - xy_distance_rev) < epstol))
                throw std::logic_error("L2Distance not symmetric."); 
        }
        catch(std::logic_error &e)
        {
            std::cerr << "Test test_utilities_l2misc failed:" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}