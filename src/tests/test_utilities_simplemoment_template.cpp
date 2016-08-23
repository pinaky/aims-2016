/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "test_header.h"
#include "utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>

namespace aimstesting
{
    // Test aims::utilities::simplemoment<T>
    void test_utilities_simplemoment_template()
    {
        const double epstol = 1.0E-10;
        std::vector<int> array_int;
        std::vector<double> array_dbl;

        array_int.push_back(-2);
        array_int.push_back(-1);
        array_int.push_back(0);
        array_int.push_back(1);
        array_int.push_back(2);
        double array_int_expected_mean = 0.0;

        array_dbl.push_back(1.0);
        array_dbl.push_back(-1.0);
        array_dbl.push_back(2.0);
        double array_dbl_expected_mean = 2.0 / 3.0;

        try
        {
            double array_int_mean = aims::utilities::SimpleMoment<int>(array_int, 1);
            double array_dbl_mean = aims::utilities::SimpleMoment<double>(array_dbl, 1);

            if(!(std::abs(array_int_mean - array_int_expected_mean) < epstol))
                throw std::logic_error("Error: Integer array mean failed.");
            if(!(std::abs(array_dbl_mean - array_dbl_expected_mean) < epstol))
                throw std::logic_error("Error: Double array mean failed.");

        }
        catch(std::logic_error &e)
        {
            std::cerr << "Test test_utilities_simplemoment_template failed:" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}