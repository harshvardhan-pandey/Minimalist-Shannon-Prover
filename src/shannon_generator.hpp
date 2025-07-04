#pragma once
#include "entropy_utils.hpp"
#include <vector>

struct Inequality {
    std::vector<double> coeffs;  // length = 2^n - 1
    double rhs;                  // usually 0
};

std::vector<Inequality> generate_shannon_inequalities(const EntropyIndexer& indexer);
