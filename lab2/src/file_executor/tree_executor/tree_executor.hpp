#pragma once

#include "file_parser/parser.hpp"

namespace tree_executor
{
    class Executor
    {
    public:
        static void ExecuteTree(const file_parser::FileUPtr &tree);
    };
}
