/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chainbuilder.h"

namespace aims
{
    class ChainBuilder_Initial: public ChainBuilder
    {
        private:
            std::unique_ptr<aims::Prior> prior_;
        public:
            ChainBuilder_Initial() : ChainBuilder(), prior_(nullptr)  {}
            ChainBuilder_Initial& SetPrior(std::unique_ptr<aims::Prior> prior) { this->prior_ = std::move(prior); return *this; }

            std::unique_ptr<aims::Chain> Build()
            {
                try
                {
                    ChainBuilder::Checker();
                    if(!this->prior_) throw std::logic_error("Error: The Prior pointer is null in ChainBuilder_Initial.");
                }
                catch(std::logic_error &e)
                {
                    std::cerr << e.what() << std::endl;
                    std::cerr << "Error: One of the pointers in ChainBuilderInitial is null." << std::endl;
                    exit(1);
                }
                
                std::unique_ptr<ChainServices> chain_services(new ChainServices());
                chain_services->acceptance_probability_service = std::move(this->acceptance_probability_service_);
                chain_services->num_samples_scheduler = std::move(this->num_samples_scheduler_);
                chain_services->forward_model = std::move(this->forward_model_);
                chain_services->beta_scheduler = std::move(this->beta_scheduler_);
                chain_services->proposal_scheduler = std::move(this->proposal_scheduler_);
                chain_services->prior = std::move(this->prior_);
                chain_services->random_engine = std::move(this->random_engine_);
                chain_services->chain_printer = std::move(this->chain_printer_);

                #ifdef DEBUG
                    std::cout << "*** ChainBuilder_Initial ***" << std::endl;
                    if(!(chain_services->chain_printer)) std::cout << "ChainBuilder_Initial chain_printer is null." << std::endl;
                #endif

                std::unique_ptr<Chain> x(new Chain(std::move(chain_services)));
                return x;
            }
    };
}