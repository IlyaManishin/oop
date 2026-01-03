#include "interpreter/interpreter.hpp"

#include "file_executor.hpp"
#include "file_parser/parser.hpp"

#include <exception>
#include <iostream>
#include <string>

using namespace file_parser;
using namespace tree_executor;

namespace file_executor
{
    void FileExecutor::run_from_config_file(const std::string &configPath, std::ostream &out)
    {
        AstParser parser(configPath);

        FileUPtr tree = nullptr;
        try
        {
            tree = parser.ParseFileTree();
        }
        catch (const std::exception &exc)
        {
            std::string msg = std::string("File parser error:\n") + exc.what();
            throw std::runtime_error(msg);
        }
        try
        {
            Interpreter::ExecuteTree(tree, out);
        }
        catch (const std::exception &exc)
        {
            std::string msg = std::string("Interpreter error occurred:\n") + exc.what();
            throw std::runtime_error(msg);
        }
    }
} // namespace file_executor
