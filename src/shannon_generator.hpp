#pragma once
#include "entropy_utils.hpp"
#include "inequality_utils.hpp"
#include <vector>


std::vector<Inequality> generate_shannon_inequalities(const EntropyIndexer& indexer);
