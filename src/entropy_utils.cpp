#include "entropy_utils.hpp"
#include <sstream>
#include <algorithm>

EntropyIndexer::EntropyIndexer(const std::vector<std::string>& var_names)
    : var_names_(var_names) {
    for (size_t i = 0; i < var_names.size(); ++i) {
        var_index_[var_names[i]] = i;
    }
}

int EntropyIndexer::num_vars() const {
    return var_names_.size();
}

int EntropyIndexer::subset_to_index(int bitmask) const {
    // Index from 0 to 2^n - 2 (excluding empty set)
    return bitmask - 1;
}

int EntropyIndexer::name_to_bitmask(const std::vector<std::string>& subset) const {
    int mask = 0;
    for (const auto& name : subset) {
        mask |= (1 << var_index_.at(name));
    }
    return mask;
}

std::string EntropyIndexer::bitmask_to_str(int bitmask) const {
    std::stringstream ss;
    bool first = true;
    for (size_t i = 0; i < var_names_.size(); ++i) {
        if (bitmask & (1 << i)) {
            if (!first) ss << ",";
            ss << var_names_[i];
            first = false;
        }
    }
    return ss.str();
}
