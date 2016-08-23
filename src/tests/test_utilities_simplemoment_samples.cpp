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
    void test_utilities_simplemoment_samples()
    {
        const double epstol = 1.0E-10;
        std::vector<double> v(5);
        std::vector<std::vector<double> > x;

        v[0] = 1.0;
        v[1] = 1.0;
        v[2] = 1.0;
        v[3] = 1.0;
        v[4] = 1.0;
        
        x.push_back(v);

        v[0] = 1.0;
        v[1] = 2.0;
        v[2] = 3.0;
        v[3] = 4.0;
        v[4] = 5.0;
        x.push_back(v);

        v[0] = 1.0;
        v[1] = 1.5;
        v[2] = 2.0;
        v[3] = 2.5;
        v[4] = 3.0;

        try
        {
            std::vector<double> x_mean = aims::utilities::SimpleMoment(x, 1);
            if(!(x_mean.size() == v.size()))
                throw std::logic_error("Size of mean is different than expected.");
            for(uint64_t i = 0; i < x.size(); i++)
            {
                if(!(x_mean[i] == v[i]))
                    throw std::logic_error("Mean value is different from expected.");
            }
        }
        catch(std::logic_error &e)
        {
            std::cerr << "Test test_utilities_simplemoment_samples failed:" << std::endl;
            std::cerr << e.what() << std::endl;
        }

    }
}