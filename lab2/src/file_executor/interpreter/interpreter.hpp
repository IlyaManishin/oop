#pragma once

#include "file_parser/parser.hpp"

namespace tree_executor
{
    class Interpreter
    {
    public:
        static void ExecuteTree(const file_parser::FileUPtr &tree);
    };
}
