#pragma once

#include "file_parser/parser.hpp"
#include <iostream>

namespace tree_executor
{
    class Interpreter
    {
    public:
        static void ExecuteTree(const file_parser::FileUPtr &tree, std::ostream &out);
    };
}
