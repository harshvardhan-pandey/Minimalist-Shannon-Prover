#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class EntropyIndexer {
public:
    EntropyIndexer(const std::vector<std::string>& var_names);

    int subset_to_index(int bitmask) const;
    int name_to_bitmask(const std::vector<std::string>& subset) const;
    std::string bitmask_to_str(int bitmask) const;
    int num_vars() const;

private:
    std::vector<std::string> var_names_;
    std::unordered_map<std::string, int> var_index_;
};
