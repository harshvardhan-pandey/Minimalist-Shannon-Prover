#include "../include/parser.hpp"
#include <cctype>
#include <regex>
#include <sstream>
#include <unordered_map>

bool parse_linear_entropy_expression(const std::string& input,
                                     const EntropyIndexer& indexer,
                                     std::vector<Inequality>& output) {
    std::string s = input;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    // Identify inequality direction and split
    std::string lhs, rhs;
    enum { GE, LE, EQ } direction;
    size_t pos;

    if ((pos = s.find(">=")) != std::string::npos) {
        lhs = s.substr(0, pos);
        rhs = s.substr(pos + 2);
        direction = GE;
    } else if ((pos = s.find("<=")) != std::string::npos) {
        lhs = s.substr(0, pos);
        rhs = s.substr(pos + 2);
        direction = LE;
    } else if ((pos = s.find("=")) != std::string::npos) {
        lhs = s.substr(0, pos);
        rhs = s.substr(pos + 1);
        direction = EQ;
    } else {
        return false;
    }

    // Parse RHS constant
    double rhs_value;
    try {
        rhs_value = std::stod(rhs);
    } catch (...) {
        return false;
    }

    // Tokenize LHS: find terms like [+/-]?[num]?[H/I](...)
    std::regex term_regex(R"(([+-]?)(\d*\.?\d*)?(H|I)\(([^()]+)\))");
    std::sregex_iterator iter(lhs.begin(), lhs.end(), term_regex);
    std::sregex_iterator end;

    int dim = (1 << indexer.num_vars()) - 1;
    std::vector<double> coeffs(dim, 0.0);

    for (; iter != end; ++iter) {
        std::string sign_str = (*iter)[1];
        std::string coeff_str = (*iter)[2];
        std::string func_type = (*iter)[3];
        std::string arg_str   = (*iter)[4];

        double sign = (sign_str == "-") ? -1.0 : 1.0;
        double scalar = coeff_str.empty() ? 1.0 : std::stod(coeff_str);
        double factor = sign * scalar;

        auto split = [](const std::string& s, char delim) {
            std::vector<std::string> tokens;
            std::stringstream ss(s);
            std::string token;
            while (std::getline(ss, token, delim)) {
                if (!token.empty()) tokens.push_back(token);
            }
            return tokens;
        };

        if (func_type == "H") {
            size_t bar = arg_str.find('|');
            std::vector<std::string> A, B;
            if (bar != std::string::npos) {
                A = split(arg_str.substr(0, bar), ',');
                B = split(arg_str.substr(bar + 1), ',');
            } else {
                A = split(arg_str, ',');
            }

            int a_mask = indexer.name_to_bitmask(A);
            if (a_mask > 0) coeffs[a_mask - 1] += factor;

            if (!B.empty()) {
                int b_mask = indexer.name_to_bitmask(B);
                int ab_mask = a_mask | b_mask;
                coeffs[ab_mask - 1] += factor;
                if (b_mask > 0) coeffs[b_mask - 1] -= factor;
            }
        } else if (func_type == "I") {
            size_t semi = arg_str.find(';');
            size_t bar  = arg_str.find('|');

            if (semi == std::string::npos) return false;

            std::vector<std::string> A = split(arg_str.substr(0, semi), ',');
            std::vector<std::string> B, C;

            if (bar != std::string::npos) {
                B = split(arg_str.substr(semi + 1, bar - semi - 1), ',');
                C = split(arg_str.substr(bar + 1), ',');
            } else {
                B = split(arg_str.substr(semi + 1), ',');
            }

            int a_mask = indexer.name_to_bitmask(A);
            int b_mask = indexer.name_to_bitmask(B);
            int c_mask = indexer.name_to_bitmask(C);
            int ab_mask = a_mask | b_mask;
            int ac_mask = a_mask | c_mask;
            int bc_mask = b_mask | c_mask;
            int abc_mask = ab_mask | c_mask;

            coeffs[ac_mask - 1] += factor;
            coeffs[bc_mask - 1] += factor;
            if (c_mask > 0) coeffs[c_mask - 1] -= factor;
            coeffs[abc_mask - 1] -= factor;
        }
    }

    // Always convert to ≥ form
    auto emit = [&](std::vector<double> coeffs, double rhs) {
        output.push_back({std::move(coeffs), rhs});
    };

    if (direction == GE) {
        emit(coeffs, rhs_value);
    } else if (direction == LE) {
        for (auto& c : coeffs) c *= -1;
        emit(coeffs, -rhs_value);
    } else if (direction == EQ) {
        emit(coeffs, rhs_value);
        for (auto& c : coeffs) c *= -1;
        emit(coeffs, -rhs_value);
    }

    return true;
}
