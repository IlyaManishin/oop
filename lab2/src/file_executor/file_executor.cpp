#include "tree_executor/tree_executor.hpp"

#include "file_parser/parser.hpp"

#include <exception>
#include <iostream>
#include <string>

using namespace file_parser;
using namespace tree_executor;

namespace file_executor
{
    bool run_from_config_file(const std::string configPath)
    {
        Parser parser(configPath);

        file_parser::FileUPtr tree = nullptr;
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
            Executor::ExecuteTree(std::move(tree));
        }
        catch (const std::runtime_error &err)
        {
            std::cerr << err.what() << std::endl;
            return false;
        }
        return true;
    }
} // namespace executor
