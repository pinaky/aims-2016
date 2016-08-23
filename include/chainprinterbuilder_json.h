/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include "chainprinter_json.h"

namespace aims
{
    class ChainPrinterBuilder_JSON
    {
        public:
            std::unique_ptr<ChainPrinter> BuildFromInput()
            {
                std::string file_name = InputToken<std::string>("Enter the output JSON file name: ").GetValue();
                std::unique_ptr<std::ostream> file_output(new std::ofstream(file_name));
                //std::ofstream file_output(file_name);
                std::unique_ptr<ChainPrinter> x(new ChainPrinter_JSON(std::move(file_output)));
                return x;
            }
    };
}