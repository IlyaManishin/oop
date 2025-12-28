#pragma once

#include "types/types.hpp"

#include <string>

namespace tree_executor
{
    bool check_predicate(const std::string &predicatName, const ExObjs &objs);
} // namespace tree_executor
