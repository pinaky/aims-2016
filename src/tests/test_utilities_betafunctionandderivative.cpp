/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "test_header.h"
#include "utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <string>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

#include <sstream>

namespace aimstesting
{

/* test_utilities_betafunctionandderivative():
*    This function makes sure that the expression in Eq.(23) of the IJUQ version of the AIMS paper
*    evaluates correctly. The function is implemented as double utilities::BetaFunctionAndDerivative(...)
*    and is capable of evaluating the expression and its derivative. This test-function tests both
*    capabilities
*/

/* The equation for the function is:
*  f(x) = (sum_i a_i ^ (2x)) / (sum_i a_i ^ x) ^ 2 - 1.0 / gamma / num_samples
*       = A / B - C
*  Test conditions:
*  1 - For a_i = a forall i, f(x) = 1.0 / n - 1.0 / gamma / num_samples
*  2 - For a_i = 0 except i = j and a_j = a =/= 0, f(x) = 1.0 - 1.0 / gamma / num_samples
*  
*  For the derivative, we have:
*  f'(x) = A' / B - AB' / B ^ (3/2)
*  A' = 2 sum_i (a_i ^ (2x) log a_i)
*  B' = 2 sum_i (a_i ^ x log a_i)
*/

    void test_utilities_betafunctionandderivative()
    {
        const uint64_t n = 100;
        const double gamma = 0.8;
        const double log_constant = -log(gamma * n);
        const double epstol = 1.0E-8;

        std::vector<double> testvec_a(n, -log((double) n) - 1000.0), testvec_b(n, -1.0E+10), testvec_c(n);
        std::vector<double> tempvec_a(n), tempvec_b(n);
        testvec_b[5] = 1.0;

        for(uint64_t i = 0u; i < n; i++) 
        { testvec_c[i] = 2.0 * log(i + 1); }

        std::vector<signed_logarithm> slvec_a(n), slvec_b(n), slvec_c(n);
        for(uint64_t i = 0u; i < n; i++)
        {
            signed_logarithm term;

            term.first = log(std::abs(testvec_a[i]));
            term.second = (double) (testvec_a[i] > 0) - (double) (testvec_a[i] < 0);
            slvec_a[i] = term;

            term.first = log(std::abs(testvec_b[i]));
            term.second = (double) (testvec_b[i] > 0) - (double) (testvec_b[i] < 0);
            slvec_b[i] = term;

            term.first = log(std::abs(testvec_c[i]));
            term.second = (double) (testvec_c[i] > 0) - (double) (testvec_c[i] < 0);
            slvec_c[i] = term;
        }

        double testbeta_a = 1.0E-7, testbeta_b = 0.1, term, termA, termB, termAp, termBp;
        betafunc_t result_aa, result_ba, result_ca, result_cb;
        betafunc_t check_aa, check_ba, check_ca, check_cb;

        result_aa = aims::utilities::BetaFunctionAndDerivative(testvec_a, slvec_a, testbeta_a, log_constant, true);
        result_ba = aims::utilities::BetaFunctionAndDerivative(testvec_b, slvec_b, testbeta_a, log_constant, true);
        result_ca = aims::utilities::BetaFunctionAndDerivative(testvec_c, slvec_c, testbeta_a, log_constant, true);
        result_cb = aims::utilities::BetaFunctionAndDerivative(testvec_c, slvec_c, testbeta_b, log_constant, true);

        // For the 'uniform' case, small beta
        term = 1.0 / (double) n - exp(log_constant);
        check_aa.first.first = log(std::abs(term));
        check_aa.first.second = (double) (term > 0.0) - (double) (term < 0.0);
        check_aa.second.first = -std::numeric_limits<double>::infinity();
        check_aa.second.second = 0.0; // The sign is ambiguous, so don't compare

        // For the 'unique case', small beta
        termA = 0.0; termB = 0.0; termAp = 0.0; termBp = 0.0;
/*        termA = utilities::LogSumExpScale(2.0 * x, testvec_b);
        termB = utilities::LogSumExpScale(x, testvec_b);
        for(uint64_t i = 0; i < n; i++) 
        { tempvec_b[i] = 2.0 * testvec_b[i] * exp(x * testvec_b[i]); tempvec_a[i] = 2.0 * testvec_b[i] * exp(2.0 * x * testvec_b[i]);}
        termAp = utilities::LogSumExp(tempvec_a);
        termBp = utilities::LogSumExp(tempvec_b);
        termAp = termAp - 2.0 * termB;
        termBp = termBp + termA - 3.0 * termB;
        termA = std::max(termAp, termBp);*/

        std::for_each(testvec_b.begin(), testvec_b.end(), [&] (double x) { termA += exp(2.0 * x * testbeta_a);});
        std::for_each(testvec_b.begin(), testvec_b.end(), [&] (double x) { termB += exp(x * testbeta_a);});
        std::for_each(testvec_b.begin(), testvec_b.end(), [&] (double x) { termAp += 2.0 * x * exp(2.0 * x * testbeta_a);});
        std::for_each(testvec_b.begin(), testvec_b.end(), [&] (double x) { termBp += 2.0 * x * exp(x * testbeta_a);});
        termB = termB * termB;
        term = termA / termB - exp(log_constant);
        check_ba.first.first = log(std::abs(term));
        check_ba.first.second = (double) (term > 0.0) - (double) (term < 0.0);
        term = termAp / termB - termBp * termA / pow(termB, 1.5);
        check_ba.second.first = log(std::abs(term));
        check_ba.second.second = (double) (term > 0.0) - (double) (term < 0.0);; // The sign is ambiguous, so don't compare

        // For the 'square' case, small beta
        termA = 0.0; termB = 0.0; termAp = 0.0; termBp = 0.0;
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termA += exp(2.0 * x * testbeta_a);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termB += exp(x * testbeta_a);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termAp += 2.0 * x * exp(2.0 * x * testbeta_a);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termBp += 2.0 * x * exp(x * testbeta_a);});
        termB = termB * termB;
        term = termA / termB - exp(log_constant);
        check_ca.first.first = log(std::abs(term));
        check_ca.first.second = (double) (term > 0.0) - (double) (term < 0.0);
        term = termAp / termB - termBp * termA / pow(termB, 1.5);
        check_ca.second.first = log(std::abs(term));
        check_ca.second.second = (double) (term > 0.0) - (double) (term < 0.0);

        // For the 'square', large beta
        termA = 0.0; termB = 0.0; termAp = 0.0; termBp = 0.0;
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termA += exp(2.0 * x * testbeta_b);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termB += exp(x * testbeta_b);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termAp += 2.0 * x * exp(2.0 * x * testbeta_b);});
        std::for_each(testvec_c.begin(), testvec_c.end(), [&] (double x) { termBp += 2.0 * x * exp(x * testbeta_b);});
        termB = termB * termB;
        term = termA / termB - exp(log_constant);
        check_cb.first.first = log(std::abs(term));
        check_cb.first.second = (double) (term > 0.0) - (double) (term < 0.0);
        term = termAp / termB - termBp * termA / pow(termB, 1.5);
        check_cb.second.first = log(std::abs(term));
        check_cb.second.second = (double) (term > 0.0) - (double) (term < 0.0);

        CompareValues("constant_likelihood_function_value_smallbeta",   check_aa.first.first,   result_aa.first.first,   epstol);
        CompareValues("unique_likelihood_function_value_smallbeta",     check_ba.first.first,   result_ba.first.first,   epstol);
        CompareValues("square_likelihood_function_value_smallbeta",     check_ca.first.first,   result_ca.first.first,   epstol);
        CompareValues("constant_likelihood_function_sign_smallbeta",    check_aa.first.second,  result_aa.first.second,  epstol);
        CompareValues("unique_likelihood_function_sign_smallbeta",      check_ba.first.second,  result_ba.first.second,  epstol);
        CompareValues("square_likelihood_function_sign_smallbeta",      check_ca.first.second,  result_ca.first.second,  epstol);
        CompareValues("constant_likelihood_derivative_value_smallbeta", check_aa.second.first,  result_aa.second.first,  epstol);
        CompareValues("unique_likelihood_derivative_value_smallbeta",   check_ba.second.first,  result_ba.second.first,  epstol);
        CompareValues("square_likelihood_derivative_value_smallbeta",   check_ca.second.first,  result_ca.second.first,  epstol);

        CompareValues("square_likelihood_derivative_sign_smallbeta",    check_ca.second.second, result_ca.second.second, epstol);

        CompareValues("square_likelihood_function_value_largebeta",     check_cb.first.first,   result_cb.first.first,   epstol);
        CompareValues("square_likelihood_function_sign_largebeta",      check_cb.first.second,  result_cb.first.second,  epstol);
        CompareValues("square_likelihood_derivative_value_largebeta",   check_cb.second.first,  result_cb.second.first,  epstol);
        CompareValues("square_likelihood_derivative_sign_largebeta",    check_cb.second.second, result_cb.second.second, epstol);
    }
}
