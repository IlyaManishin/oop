#include "file_parser/parser.hpp"

#include <string>

using namespace file_parser;

namespace executor
{
    bool run_from_config_file(const std::string configPath)
    {
        Parser parser(configPath);
        return false;
    }
} // namespace executor
