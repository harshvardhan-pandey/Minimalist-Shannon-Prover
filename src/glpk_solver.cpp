#include "../include/glpk_solver.hpp"
#include <glpk.h>
#include <iostream>

bool is_inequality_valid(const Inequality& target,
                         const std::vector<Inequality>& constraints,
                         int entropy_dim) {
    int m = constraints.size(); // number of constraints
    int n = entropy_dim;      // number of entropy terms (usually 2^n - 1)

    // Create GLPK problem
    glp_prob* lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MIN); // dummy objective

    // Lambda variables: one for each constraint
    glp_add_cols(lp, m);
    for (int i = 1; i <= m; ++i) {
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0); // lambda_i ≥ 0
    }

    // Each row corresponds to a coefficient in the target
    glp_add_rows(lp, n + 1); // +1 for the b·λ ≤ d constraint
    for (int j = 1; j <= n; ++j) {
        glp_set_row_bnds(lp, j, GLP_FX, target.coeffs[j - 1], target.coeffs[j - 1]); // λᵗA = c
    }
    glp_set_row_bnds(lp, n + 1, GLP_UP, 0.0, target.rhs); // λᵗb ≤ d

    // Build constraint matrix
    // Total non-zeros = m * (n + 1)
    std::vector<int> ia(1), ja(1);
    std::vector<double> ar(1); // GLPK uses 1-based indexing

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            ia.push_back(j + 1);       // row: coefficient of j-th entropy term
            ja.push_back(i + 1);       // column: lambda_i
            ar.push_back(constraints[i].coeffs[j]);
        }
    }

    for (int i = 0; i < m; ++i) {
        ia.push_back(n + 1);          // final row: b · lambda ≤ d
        ja.push_back(i + 1);
        ar.push_back(constraints[i].rhs);
    }

    glp_load_matrix(lp, (int)(ia.size()) - 1, ia.data(), ja.data(), ar.data());

    // Solve
    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_ERR;
    int status = glp_simplex(lp, &params);

    bool valid = false;
    if (status == 0) {
        int code = glp_get_status(lp);
        valid = (code == GLP_OPT);
    }

    glp_delete_prob(lp);
    return valid;
}
