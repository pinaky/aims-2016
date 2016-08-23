/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chainprinter.h"

namespace aims
{
    class ChainPrinter_JSON: public ChainPrinter
    {
        private:
            std::unique_ptr<std::ostream> output_stream_;
        public:
            ~ChainPrinter_JSON();
            ChainPrinter_JSON(std::unique_ptr<std::ostream>);
            void Print(ChainServices &, ChainData const &);
            void Print(BetaScheduler &);
    };
}