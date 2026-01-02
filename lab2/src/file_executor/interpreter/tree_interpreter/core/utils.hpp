#pragma once

#include "file_parser/types.hpp"

#include "types.hpp"

namespace tree_executor
{
    const std::string &parse_string_from_obj_ptr(const ExObjPtr obj);
    ExObjUPtr exobj_from_value_arg(const file_parser::ArgUPtr &arg);
} // namespace tree_executor
