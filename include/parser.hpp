#pragma once
#include "entropy_utils.hpp"
#include "inequality_utils.hpp"
#include <string>
#include <vector>

/// Parses user input like "I(X;Z|Y) = 0" into an entropy inequalities.
bool parse_linear_entropy_expression(const std::string& input,
                                     const EntropyIndexer& indexer,
                                     std::vector<Inequality>& output);

