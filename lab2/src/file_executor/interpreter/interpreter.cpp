#include "interpreter.hpp"
#include "tree_interpreter/tree_interpreter.hpp"

namespace tree_executor
{
    void Interpreter::ExecuteTree(const file_parser::FileUPtr &tree)
    {
        TreeInterpreter interpreter;
        interpreter.ExecuteTree(tree);
    }
} // namespace tree_executor
