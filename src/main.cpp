#include "entropy_utils.hpp"
#include "shannon_generator.hpp"
#include <iostream>

int main() {
    std::vector<std::string> vars = {"X", "Y", "Z"};
    EntropyIndexer indexer(vars);

    auto inequalities = generate_shannon_inequalities(indexer);
    std::cout << "Generated " << inequalities.size() << " Shannon-type inequalities.\n";

    for (size_t i = 0; i < inequalities.size(); ++i) {
        const auto& ineq = inequalities[i];
        std::cout << "Inequality " << i+1 << ": ";
        for (size_t j = 0; j < ineq.coeffs.size(); ++j) {
            if (ineq.coeffs[j] != 0) {
                std::cout << ineq.coeffs[j] << "*H(" << indexer.bitmask_to_str(j+1) << ") ";
            }
        }
        std::cout << "≥ " << ineq.rhs << "\n";
    }

    return 0;
}
