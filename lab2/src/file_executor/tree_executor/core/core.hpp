#pragma once

#include "types/types.hpp"

#include <string>

namespace tree_executor
{
    bool check_predicate(const std::string &predicatName, const ExObjs &objs);
    ExObjUPtr func_call(const std::string &funcName, const ExObjs &args);
} // namespace tree_executor
