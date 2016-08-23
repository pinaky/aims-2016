/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chaindata.h"
#include "chainservices.h"
#include "betascheduler.h"

namespace aims
{
    class ChainPrinter
    {
        public:
            virtual ~ChainPrinter() {}
            virtual void Print(ChainServices &, ChainData const &) = 0;
            virtual void Print(BetaScheduler &) = 0;
    };
}