#pragma once
#include <glpk.h>
#include <vector>
#include <iostream>
#include <cassert>
#include "inequality_utils.hpp"

bool is_inequality_valid(const Inequality& target,
                        const std::vector<Inequality>& constraints,
                        int entropy_dim);