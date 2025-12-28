#pragma once

#include "../types/types.hpp"
#include "file_parser/types.hpp"

namespace tree_executor
{
    const std::string& parse_string_from_obj(const ExObj& obj);

    const ExObjUPtr arg_to_ex_obj(const file_parser::ArgUPtr& arg);

} // namespace tree_executor
