/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include "acceptanceprobabilityservice.h"
#include "chaindata.h"
#include "chainservices.h"

namespace aims
{
    typedef std::vector<double> sample_type;
    class AcceptanceProbabilityService_Regular: public AcceptanceProbabilityService
    {
        public:
            AcceptanceProbabilityService_Regular() {}
            double Solve(ChainData const &, ChainData const &, ChainServices const &, 
                sample_type const &, double, uint64_t);
    };
}