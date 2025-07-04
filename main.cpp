#include "include/entropy_utils.hpp"
#include "include/inequality_utils.hpp"
#include "include/user_constraint_parser.hpp"

#include <iostream>
#include <vector>
#include <string>

int main() {
    // Define variable names
    std::vector<std::string> vars = {"X", "Y", "Z"};
    EntropyIndexer indexer(vars);

    std::vector<std::string> inputs = {
        "H(X) <= 1",
        "H(X,Y) = 0",
        "H(X|Y) = 0",
        "I(X;Y) >= 1",
        "I(X;Z|Y) = 0"
    };

    for (const auto& in : inputs) {
        std::cout << "Parsing: " << in << "\n";
        std::vector<Inequality> parsed;
        if (parse_user_constraint(in, indexer, parsed)) {
            for (const auto& ineq : parsed) {
                print_inequality(ineq, indexer);
            }
        } else {
            std::cerr << "Failed to parse: " << in << "\n";
        }
        std::cout << "-----------------------\n";
    }

    return 0;
}
