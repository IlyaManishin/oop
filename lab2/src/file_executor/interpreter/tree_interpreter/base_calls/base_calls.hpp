#pragma once

#include "../core/types.hpp"

#include <string>

namespace tree_executor
{
    bool predicate_call_impl(const std::string &predicatName, std::vector<ExObjPtr> args);
    ExObjUPtr func_call_impl(const std::string &funcName, std::vector<ExObjPtr> args);
} // namespace tree_executor
