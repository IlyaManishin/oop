#pragma once

#include <string>
#include <iostream>

namespace file_executor
{
    class FileExecutor
    {
    public:
        static void run_from_config_file(const std::string& configPath, std::ostream &out = std::cout);
    };
} // namespace file_executor
