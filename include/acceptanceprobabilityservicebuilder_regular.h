/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "acceptanceprobabilityservice_regular.h"
#include <memory>

namespace aims
{
    class AcceptanceProbabilityServiceBuilder_Regular
    {
        public:
            std::unique_ptr<AcceptanceProbabilityService> Build()
            {
                std::unique_ptr<AcceptanceProbabilityService> x(new AcceptanceProbabilityService_Regular());
                return x;
            }
    };
}
