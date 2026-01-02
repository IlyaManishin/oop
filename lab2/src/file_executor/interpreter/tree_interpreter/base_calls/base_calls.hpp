#pragma once

#include "../core/types.hpp"

#include <string>

namespace tree_executor
{
    bool predicate_impl(const std::string &predicatName, const ExObjs &objs);
    ExObjUPtr func_call_impl(const std::string &funcName, const ExObjs &args);
} // namespace tree_executor
