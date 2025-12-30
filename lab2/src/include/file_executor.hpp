#pragma once

#include <string>

namespace file_executor
{
    class FileExecutor
    {
    public:
        static bool run_from_config_file(const std::string configPath) noexcept;
    };
} // namespace file_executor
