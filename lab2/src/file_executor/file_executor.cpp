#include "tree_executor/tree_executor.hpp"

#include "file_executor.hpp"
#include "file_parser/parser.hpp"

#include <exception>
#include <iostream>
#include <string>

using namespace file_parser;
using namespace tree_executor;

namespace file_executor
{
    bool FileExecutor::run_from_config_file(const std::string configPath) noexcept
    {
        AstParser parser(configPath);

        FileUPtr tree = nullptr;
        try
        {
            tree = parser.ParseFileTree();
        }
        catch (const ParserException &exc)
        {
            std::cerr << exc.what() << std::endl;
            return false;
        }
        try
        {
            Executor::ExecuteTree(tree);
        }
        catch (const std::runtime_error &err)
        {
            std::cerr << err.what() << std::endl;
            return false;
        }
        return true;
    }
} // namespace executor
