/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <memory>
#include "inputtoken.h"
#include "proposalscheduler_constantrwmhisotropicnormal.h"

namespace aims
{
    class ProposalSchedulerBuilder_ConstantRWMHIsotropicNormal
    {
        private:
            double stddev_;
        public:
            ProposalSchedulerBuilder_ConstantRWMHIsotropicNormal(): stddev_(1.0) {}
            std::unique_ptr<ProposalScheduler> BuildFromInput()
            {
                this->stddev_ = 
                    InputToken<double>("Enter a value for the stddev of the RWMH Isotropic Normal proposal density: ").GetValue();
                std::unique_ptr<ProposalScheduler> x(new ProposalScheduler_ConstantRWMHIsotropicNormal(this->stddev_));
                return x;
            }
    };
}