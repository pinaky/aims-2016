/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "chainprinter_json.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include "utilities.h"
#include "betascheduler.h"

namespace aims
{
    ChainPrinter_JSON::ChainPrinter_JSON(std::unique_ptr<std::ostream> output_stream): output_stream_(std::move(output_stream)) 
	{
		std::ostream &os = *(this->output_stream_);
		os << "{" << std::endl;
	}

	ChainPrinter_JSON::~ChainPrinter_JSON()
	{
		#ifdef DEBUG
		std::cout << "In the chain_printer destructor" << std::endl;
		#endif

		std::ostream &os = *(this->output_stream_);
		os << "}" << std::endl;
	}

	void ChainPrinter_JSON::Print(BetaScheduler &b)
	{
		if(!b.Done())
			throw std::logic_error("ChainPrinter::Print(BetaScheduler) called prematurely.");

		std::map<std::string, double> integral_data = b.IntegralData();
		std::ostream &os = *(this->output_stream_);
		os << "\t\"log_evidence\":" << " {" << std::endl;

		std::map<std::string, double>::iterator it;
		std::map<std::string, double>::iterator fit = integral_data.end();
		--fit; // Last element

		for(it = integral_data.begin(); it != integral_data.end(); ++it)
		{
			os << "\t\t\"" << it->first << "\": " <<  it->second;
			if(it == fit) os << std::endl << "\t}" << std::endl;
			else os << "," << std::endl;
		}

		
	}

    void ChainPrinter_JSON::Print(ChainServices &services,ChainData const &data)
	{
		uint64_t i, k;
		const uint64_t d = data.sample[0].size();
		
		std::ostream &os = *(this->output_stream_);

		// First chain, do something special
		if(data.j == 0) os << "\t\"stages\": " << "[" << std::endl;

		// Output everything as JSON
		os << "\t{" << std::endl;

		// Print meta data
		os << "\t\t\"j\" : " << data.j << "," << std::endl;
		os << "\t\t\"beta\" : " << data.beta << "," << std::endl;
		os << "\t\t\"Ns\" : " << data.num_samples << "," << std::endl;
		os << "\t\t\"Neff\" : " << data.num_samples_effective << "," << std::endl;

		// Print vectors
		std::string name = "acc";
		utilities::PrintArrayJSON<bool>(os, std::string("acc"), data.accepted, 20, 2, true);
		name = "wcurr";
		utilities::PrintArrayJSON<double>(os, std::string("wcurr"), data.importance_weight_local, 8, 2, true);
		name = "wnext";
		utilities::PrintArrayJSON<double>(os, std::string("wnext"), data.importance_weight_posterior, 8, 2, true);
		name = "loglike";
		utilities::PrintArrayJSON<double>(os, std::string("loglike"), data.log_likelihood, 8, 2, true);
		name = "accprob";
		utilities::PrintArrayJSON<double>(os, std::string("accprob"), data.acceptance_probability, 8, 2, true);
		name = "k";
		utilities::PrintArrayJSON<uint64_t>(os, std::string("k"), data.random_integer, 10, 2, true);
		
					
		// Print the raw samples out
		os << "\t\t\"x\" : [" << std::endl << "\t\t";
		for (i = 0u; i < data.num_samples - 1; i++)
		{
			os << "\t\t\t[";
			for (k = 0u; k < d - 1; k++)
				os << data.sample[i][k] << ',';
			os << data.sample[i][d - 1] << "]," << std::endl;
		}
		os << "\t\t\t[";
		for (k = 0u; k < d - 1; k++)
			os << data.sample[i][k] << ',';
		os << data.sample[i][d - 1] << "]" << std::endl;

		os << "\t\t]" << std::endl;

		// Run this for the last stage
		if(!data.done) os << "\t}, " << std::endl;
		else os << "\t} ], " << std::endl;
	}
}