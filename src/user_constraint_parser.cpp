#include "../include/user_constraint_parser.hpp"
#include <algorithm>
#include <sstream>

bool parse_user_constraint(const std::string& input,
                           const EntropyIndexer& indexer,
                           std::vector<Inequality>& output) {
    std::string s = input;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    auto split = [](const std::string& s) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (!token.empty()) result.push_back(token);
        }
        return result;
    };

    auto make_ineq = [&](std::vector<double> coeffs, double rhs) {
        Inequality ineq;
        ineq.coeffs = std::move(coeffs);
        ineq.rhs = rhs;
        output.push_back(std::move(ineq));
    };

    int dim = (1 << indexer.num_vars()) - 1;

    // First handle I(...) op 0
    if (s.find("I(") == 0) {
        size_t close = s.find(')');
        if (close == std::string::npos) return false;
        std::string inside = s.substr(2, close - 2);
        std::string tail = s.substr(close + 1); // =0 or >=0

        size_t semi = inside.find(';');
        size_t bar = inside.find('|');

        if (semi == std::string::npos) return false;

        std::vector<std::string> A = split(inside.substr(0, semi));
        std::vector<std::string> B, C;
        if (bar != std::string::npos) {
            B = split(inside.substr(semi + 1, bar - semi - 1));
            C = split(inside.substr(bar + 1));
        } else {
            B = split(inside.substr(semi + 1));
        }

        int AC = indexer.name_to_bitmask(A);
        for (const auto& v : C) AC |= (1 << indexer.name_to_bitmask({v}) >> 1);
        int BC = indexer.name_to_bitmask(B);
        for (const auto& v : C) BC |= (1 << indexer.name_to_bitmask({v}) >> 1);
        int Cmask = indexer.name_to_bitmask(C);
        int ABC = AC | BC;

        std::vector<double> coeffs(dim, 0.0);
        coeffs[AC - 1] += 1.0;
        coeffs[BC - 1] += 1.0;
        if (Cmask > 0) coeffs[Cmask - 1] -= 1.0;
        coeffs[ABC - 1] -= 1.0;

        // Now parse =, <=, >=
        if (tail.find("=0") == 0) {
            make_ineq(coeffs, 0.0);
            for (auto& c : coeffs) c *= -1;
            make_ineq(coeffs, 0.0);
        } else if (tail.find("<=0") == 0) {
            for (auto& c : coeffs) c *= -1;
            make_ineq(coeffs, 0.0);
        } else if (tail.find(">=0") == 0) {
            make_ineq(coeffs, 0.0);
        } else if (tail.find("<=") == 0 || tail.find(">=") == 0) {
            bool flip = (tail[0] == '<');
            double rhs = std::stod(tail.substr(2));
            if (flip) {
                for (auto& c : coeffs) c *= -1;
                rhs = -rhs;
            }
            make_ineq(coeffs, rhs);
        } else {
            return false;
        }
        return true;
    }

    // Now handle H(...) op rhs
    if (s.find("H(") == 0) {
        size_t close = s.find(')');
        if (close == std::string::npos) return false;
        std::string inside = s.substr(2, close - 2);
        std::string tail = s.substr(close + 1); // =0, <=1, etc.

        size_t bar = inside.find('|');
        std::vector<std::string> A, B;

        if (bar != std::string::npos) {
            A = split(inside.substr(0, bar));
            B = split(inside.substr(bar + 1));
        } else {
            A = split(inside);
        }

        int A_mask = indexer.name_to_bitmask(A);
        int AB_mask = A_mask;
        for (const auto& v : B) AB_mask |= (1 << indexer.name_to_bitmask({v}) >> 1);
        int B_mask = indexer.name_to_bitmask(B);

        std::vector<double> coeffs(dim, 0.0);
        if (!B.empty()) {
            coeffs[AB_mask - 1] += 1.0;
            if (B_mask > 0) coeffs[B_mask - 1] -= 1.0;
        } else {
            coeffs[A_mask - 1] += 1.0;
        }

        // Now parse =, <=, >=
        if (tail.find("=0") == 0) {
            make_ineq(coeffs, 0.0);
            for (auto& c : coeffs) c *= -1;
            make_ineq(coeffs, 0.0);
        } else if (tail.find("<=0") == 0) {
            for (auto& c : coeffs) c *= -1;
            make_ineq(coeffs, 0.0);
        } else if (tail.find(">=0") == 0) {
            make_ineq(coeffs, 0.0);
        } else if (tail.find("<=") == 0 || tail.find(">=") == 0) {
            bool flip = (tail[0] == '<');
            double rhs = std::stod(tail.substr(2));
            if (flip) {
                for (auto& c : coeffs) c *= -1;
                rhs = -rhs;
            }
            make_ineq(coeffs, rhs);
        } else {
            return false;
        }
        return true;
    }

    return false;
}
