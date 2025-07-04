#include "include/entropy_utils.hpp"
#include "include/inequality_utils.hpp"
#include "include/shannon_generator.hpp"
#include "include/parser.hpp"
#include "include/glpk_solver.hpp"

#include <iostream>
#include <vector>
#include <string>

int main() {
    //Define variable names
    std::vector<std::string> vars = {"X", "Y", "Z"};
    EntropyIndexer indexer(vars);
    int dim = (1 << vars.size()) - 1;

    //Generate Shannon inequalities
    std::vector<Inequality> all_constraints = generate_shannon_inequalities(indexer);

    //Add user-defined constraints
    std::vector<std::string> user_constraints = {
        "I(X;Z|Y) = 0"
    };

    for (const auto& line : user_constraints) {
        std::vector<Inequality> parsed;
        if (!parse_linear_entropy_expression(line, indexer, parsed)) {
            std::cerr << "Failed to parse user constraint: " << line << "\n";
            return 1;
        }
        all_constraints.insert(all_constraints.end(), parsed.begin(), parsed.end());
    }

    //Parse target inequality
    std::string target_expr = "I(X;Z) >= 0";
    std::vector<Inequality> parsed_target;
    if (!parse_linear_entropy_expression(target_expr, indexer, parsed_target) || parsed_target.size() != 1) {
        std::cerr << "Failed to parse target inequality: " << target_expr << "\n";
        return 1;
    }
    Inequality target = parsed_target[0];
    print_inequality(target, indexer);

    bool valid = is_inequality_valid(target, all_constraints, dim);

    std::cout << "Target inequality:\n";
    print_inequality(target, indexer);
    std::cout << "\nStatus: " << (valid ? "VALID" : "NOT VALID") << "\n";

    return 0;
}
