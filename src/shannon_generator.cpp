#include "shannon_generator.hpp"
#include <bitset>
#include <cassert>

std::vector<Inequality> generate_shannon_inequalities(const EntropyIndexer& indexer) {
    int n = indexer.num_vars();
    int dim = (1 << n) - 1;

    std::vector<Inequality> result;

    // 1. H(S) ≥ 0 for all non-empty subsets S
    for (int mask = 1; mask <= dim; ++mask) {
        Inequality ineq;
        ineq.coeffs.assign(dim, 0.0);
        ineq.coeffs[mask - 1] = 1.0;
        ineq.rhs = 0.0;
        result.push_back(ineq);
    }

    // 2. I(A;B|C) ≥ 0
    // Iterate over disjoint subsets A, B, C with A,B ≠ empty
    for (int A = 1; A <= dim; ++A) {
        for (int B = 1; B <= dim; ++B) {
            if (A & B) continue;  // not disjoint

            int AB = A | B;
            for (int C = 0; C <= dim; ++C) {
                if ((A & C) || (B & C)) continue;  // not disjoint with C

                int AC = A | C;
                int BC = B | C;
                int ABC = AB | C;

                Inequality ineq;
                ineq.coeffs.assign(dim, 0.0);

                // I(A;B|C) = H(AC) + H(BC) - H(C) - H(ABC)
                ineq.coeffs[AC - 1] += 1.0;
                ineq.coeffs[BC - 1] += 1.0;
                if (C > 0) ineq.coeffs[C - 1] -= 1.0;
                ineq.coeffs[ABC - 1] -= 1.0;

                ineq.rhs = 0.0;
                result.push_back(ineq);
            }
        }
    }

    return result;
}
