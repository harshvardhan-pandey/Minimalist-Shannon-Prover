#include "inequality_utils.hpp"
#include <vector>
#include <iostream>

void print_inequality(const Inequality& ineq, const EntropyIndexer& indexer) {
    for (size_t i = 0; i < ineq.coeffs.size(); ++i) {
        if (ineq.coeffs[i] == 0) continue;
        std::cout << (ineq.coeffs[i] > 0 ? "+" : "") << ineq.coeffs[i]
                  << "*H(" << indexer.bitmask_to_str(i + 1) << ") ";
    }
    std::cout << ">= " << ineq.rhs << "\n";
}