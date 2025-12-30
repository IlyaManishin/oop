#pragma once

#include "../types/types.hpp"
#include "file_parser/types.hpp"

namespace tree_executor
{
    const std::string& parse_string_from_obj(const ExObj& obj);

    const ExObjUPtr exObj_from_value_arg(const file_parser::ArgUPtr& arg);

} // namespace tree_executor
