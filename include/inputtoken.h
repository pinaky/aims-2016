/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace aims
{
	template <typename T>
	class InputToken
	{
		private:
			std::string message_;
			T value_;
		public:
			InputToken(std::string const &message) { this->message_ = message; std::cout << message_ << std::endl; std::cin >> value_; }
			InputToken(std::string const &message, std::ostream &os, std::istream &is) { this->message_ = message; os << message_ << std::endl; is >> value_; }
			T GetValue() { return value_; }
	};
}