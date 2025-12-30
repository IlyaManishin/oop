#include "tree_executor.hpp"
#include "core/tree_interpreter.hpp"

namespace tree_executor
{
    void Executor::ExecuteTree(const file_parser::FileUPtr &tree)
    {
        TreeInterpreter interpreter;
        interpreter.Execute(tree);
    }
}
