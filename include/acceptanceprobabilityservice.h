/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include "chaindata.h"
#include "chainservices.h"
#include <stdint.h>
// This class calculates the AIMS acceptance probabilty.
// It is used as a service and passed to the constructor of aims::Chain.
// Derived classes implement this.

namespace aims
{
    typedef std::vector<double> sample_type;
    class AcceptanceProbabilityService
    {
        public:
            virtual ~AcceptanceProbabilityService() {}
            virtual double Solve(ChainData const &, ChainData const &, ChainServices const &,
                sample_type const &, double, uint64_t) = 0;
    };
}