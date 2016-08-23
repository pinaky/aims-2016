/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chainbuilder.h"

namespace aims
{
    class ChainBuilderNext: public ChainBuilder
    {
        private:
            std::unique_ptr<aims::Chain> chain_previous_;
        public:
            ChainBuilderNext(): chain_previous_(nullptr), ChainBuilder() {}
            ChainBuilderNext& SetChainPrevious(std::unique_ptr<aims::Chain> chain_previous) { this->chain_previous_ = chain_previous; return *this;}

            std::unique_ptr<aims::Chain> Build()
            {
                try
                {
                    ChainBuilder::Checker();
                    if(!this->prior_) throw std::logic_error("Error: The chain_previous pointer is null.");
                }
                catch(std::logic_error &e)
                {
                    std::cerr << e.what() << std::endl;
                    std::cerr << "Error: One of the pointers in ChainBuilderNext is null." << std::endl;
                    exit(1);
                }
                
                std::unique_ptr<aims::Chain> x(new aims::Chain(this->chain_previous_, this->num_samples_scheduler_, this->forward_model_, this->proposal_scheduler_, this->beta_scheduler_);
                return x;
            }
    };
}
