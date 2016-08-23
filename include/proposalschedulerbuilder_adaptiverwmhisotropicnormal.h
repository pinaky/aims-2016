/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "proposalscheduler_adaptiverwmhisotropicnormal.h"

namespace aims
{
    class ProposalSchedulerBuilder_AdaptiveRWMHIsotropicNormal
    {
        private:
            bool verbose_;
            double test_fraction_;
        public:
            ProposalSchedulerBuilder_AdaptiveRWMHIsotropicNormal(): verbose_(false) {}

            ProposalSchedulerBuilder_AdaptiveRWMHIsotropicNormal& SetVerbose(bool verbose)
            {this->verbose_ = verbose; return *this; }

            ProposalSchedulerBuilder_AdaptiveRWMHIsotropicNormal& SetTestFraction(double test_fraction)
            { this->test_fraction_ = test_fraction; return *this; }

            std::unique_ptr<ProposalScheduler> Build()
            {
                std::unique_ptr<ProposalScheduler> x(new ProposalScheduler_AdaptiveRWMHIsotropicNormal(this->verbose_));
                return x;
            }
    };
}