/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#pragma once
#include <vector>
#include <string>

namespace aimstesting
{
    typedef std::pair<double, double> signed_logarithm;
    typedef std::pair<signed_logarithm, signed_logarithm> betafunc_t; 

    void CompareValues(std::string, double, double, double);
    void test_utilities(); //
    void test_utilities_simplemoment_template();
    void test_utilities_simplemoment_samples();
    void test_utilities_l2misc();
    void test_utilities_logsumexpmisc();
    void test_utilities_integeratrandom();
    void test_utilities_normalizedimportanceweights();
    void test_utilities_betafunctionandderivative();
    void test_utilities_betabinarysearch();
    void test_utilities_betanewtonraphsonrefine();
    void test_utilities_acceptanceprobabilitybasic();
    void test_utilities_betafunctionandderivative();

    void test_prior(); //
    void test_prior_generaluniform();

    void test_proposal(); //
    void test_proposal_rwmhisotropicnormal();

    
}