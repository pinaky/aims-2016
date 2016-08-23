/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "proposalscheduler_constantrwmhisotropicnormal.h"

namespace aims
{
    ProposalScheduler_ConstantRWMHIsotropicNormal::
        ProposalScheduler_ConstantRWMHIsotropicNormal(double stddev): stddev_(stddev) { }

    std::unique_ptr<Proposal> ProposalScheduler_ConstantRWMHIsotropicNormal::Solve(
        ChainData const &data_previous, ChainData const &data_next, ChainServices const &services)
    {
            std::unique_ptr<Proposal> x(new Proposal_RWMHIsotropicNormal(this->stddev_));
            return x; 
    }
}