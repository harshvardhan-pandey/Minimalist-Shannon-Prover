// cli.cpp
#include "include/entropy_utils.hpp"
#include "include/inequality_utils.hpp"
#include "include/shannon_generator.hpp"
#include "include/parser.hpp"
#include "include/glpk_solver.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void usage(const char* prog) {
    std::cerr << "Usage: " << prog << " <target_file> <constraint_file> [num_variables]\n";
    std::cerr << "  <target_file>: file containing the target inequality (e.g., I(X;Y) - I(X;Z) >= 0)\n";
    std::cerr << "  <constraint_file>: file with one constraint inequality per line (e.g., I(X;Z|Y) = 0)\n";
    std::cerr << "  [num_variables]: optional, defaults to 3\n";
    exit(1);
}

std::string get_variable_name(int index) {
    if (index < 26) return std::string(1, 'A' + index);
    return "X" + std::to_string(index);
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) usage(argv[0]);

    std::string target_file = argv[1];
    std::string constraint_file = argv[2];
    int num_vars = (argc == 4) ? std::stoi(argv[3]) : 3;

    // Define variable names: A, B, C, ..., X26, X27, etc.
    std::vector<std::string> vars;
    for (int i = 0; i < num_vars; ++i) {
        vars.push_back(get_variable_name(i));
    }

    EntropyIndexer indexer(vars);
    int dim = (1 << num_vars) - 1;

    // Generate Shannon inequalities
    std::vector<Inequality> all_constraints = generate_shannon_inequalities(indexer);

    // Read user constraints
    std::ifstream user_file(constraint_file);
    std::string line;
    while (std::getline(user_file, line)) {
        if (line.empty()) continue;
        std::vector<Inequality> parsed;
        if (!parse_linear_entropy_expression(line, indexer, parsed)) {
            std::cerr << "Failed to parse user constraint: " << line << "\n";
            return 1;
        }
        all_constraints.insert(all_constraints.end(), parsed.begin(), parsed.end());
    }

    // Read target inequality
    std::ifstream target_input(target_file);
    std::string target_expr;
    std::getline(target_input, target_expr);

    std::vector<Inequality> parsed_target;
    if (!parse_linear_entropy_expression(target_expr, indexer, parsed_target) || parsed_target.size() != 1) {
        std::cerr << "Failed to parse target inequality: " << target_expr << "\n";
        return 1;
    }

    Inequality target = parsed_target[0];
    std::cout << "Target inequality:\n";
    print_inequality(target, indexer);

    bool valid = is_inequality_valid(target, all_constraints, dim);
    std::cout << "\nStatus: " << (valid ? "VALID" : "NOT VALID") << "\n";

    return 0;
}
