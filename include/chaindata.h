/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include "proposal.h"
#include <memory>
// This structure stores all the information related to the AIMS chain
// This can be passed by const-reference as and when needed

namespace aims
{
    
    struct ChainData
    {
        uint64_t j;
        uint64_t num_samples;
        uint64_t num_samples_effective;
        bool done;
        double beta;
        std::vector<sample_type> sample;
        std::vector<double> log_likelihood;
        std::vector<double> importance_weight_local;
        std::vector<double> importance_weight_posterior;
        std::vector<double> acceptance_probability;
        std::vector<bool> accepted;
        std::vector<uint64_t> random_integer;
        std::unique_ptr<Proposal> proposal;
    };
}