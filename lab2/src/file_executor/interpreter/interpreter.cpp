#include "interpreter.hpp"
#include "tree_interpreter/tree_interpreter.hpp"

#include <iostream>

namespace tree_executor
{
    void Interpreter::ExecuteTree(const file_parser::FileUPtr &tree, std::ostream &out)
    {
        TreeInterpreter interpreter(out);
        interpreter.ExecuteTree(tree);
    }
} // namespace tree_executor
