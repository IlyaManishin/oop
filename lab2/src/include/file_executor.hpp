#pragma once

#include <string>

namespace file_executor
{
    class FileExecutor
    {
    public:
        static void run_from_config_file(const std::string& configPath);
    };
} // namespace file_executor
