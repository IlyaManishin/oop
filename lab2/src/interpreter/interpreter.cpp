#include "interpreter.hpp"
#include "tree_interpreter/tree_interpreter.hpp"

#include <iostream>

namespace interpreter
{
    void Interpreter::ExecuteTree(const file_parser::FileUPtr &tree, std::ostream &out)
    {
        tree_executor::TreeInterpreter interpreter(out);
        interpreter.ExecuteTree(tree);
    }
} // namespace interpreter
