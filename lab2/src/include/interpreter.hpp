#pragma once

#include "file_parser/parser.hpp"
#include <iostream>

namespace interpreter
{
    class Interpreter
    {
    public:
        static void ExecuteTree(const file_parser::FileUPtr &tree, std::ostream &out);
    };
} // namespace interpreter
