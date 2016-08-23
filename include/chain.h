/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <memory>
#include <vector>
#include <random>
#include "chaindata.h"
#include "chainservices.h"
#include "chainprinter.h"

namespace aims
{
	class Chain
	{
		private:
			ChainData data_;
			//std::unique_ptr<ChainServices> services_;
			// std::unique_ptr<Chain> TransferServices(std::unique_ptr<Chain>);
		public:
			Chain(ChainServices &);
			Chain(ChainServices &, Chain &);
			virtual void Print(ChainServices &, Chain &);
			virtual bool Done();
	};
}
