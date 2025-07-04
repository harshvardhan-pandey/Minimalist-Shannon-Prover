#pragma once
#include "entropy_utils.hpp"

//Assumes lhs >= rhs
struct Inequality {
    std::vector<double> coeffs;  // length = 2^n - 1
    double rhs;                  // usually 0
};

void print_inequality(const Inequality& ineq, const EntropyIndexer& indexer);