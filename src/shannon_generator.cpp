#include "../include/shannon_generator.hpp"
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

    // 2. I(A;B|C) ≥ 0 using optimized triple loop
    for (int A = 1; A < dim; ++A) {
        int notA = (~A) & dim;  // all bits not in A

        // Iterate over B ⊆ complement of A
        for (int B = notA; B > 0; B = (B - 1) & notA) {
            if (B == 0) break;

            int AB = A | B;
            int notAB = (~AB) & dim;

            // Iterate over C ⊆ complement of (A ∪ B)
            for (int C = notAB; ; C = (C - 1) & notAB) {
                int AC = A | C;
                int BC = B | C;
                int ABC = A | B | C;

                Inequality ineq;
                ineq.coeffs.assign(dim, 0.0);

                ineq.coeffs[AC - 1] += 1.0;
                ineq.coeffs[BC - 1] += 1.0;
                if (C > 0) ineq.coeffs[C - 1] -= 1.0;
                ineq.coeffs[ABC - 1] -= 1.0;
                ineq.rhs = 0.0;

                result.push_back(ineq);

                if (C == 0) break;  // stop at C = 0
            }
        }
    }


    return result;
}
