/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "betascheduler_gausskronrod.h"
#include "inputtoken.h"

namespace aims
{
    class BetaSchedulerBuilder_GaussKronrod
    {
        private:
            double gamma_;
        public:
            BetaSchedulerBuilder_GaussKronrod(): gamma_(0.8) {}

            std::unique_ptr<BetaScheduler> BuildFromInput()
            {
                std::cout << "GAUSS KRONROD BETA SCHEDULER INFORMATION:" << std::endl;
                this->gamma_ = InputToken<double>("Enter a value for AIMS gamma: ").GetValue();
                std::unique_ptr<BetaScheduler> x(new BetaScheduler_GaussKronrod(this->gamma_));
                return x;
            }

            std::unique_ptr<BetaScheduler> Build()
            {
                std::unique_ptr<BetaScheduler> x(new BetaScheduler_GaussKronrod(this->gamma_));
                return x;
            }
    };
}