#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "cmd_args.hpp"

namespace cmd
{
    class CommandParser
    {
    public:
        void registerCommand(const std::string &name, int id);
        Command *parse(int argc, char **argv) const;
    
    private:
        std::unordered_map<std::string, int> commandMap;
        static Arg parseArg(const std::string &token);
    };
} // namespace cmd


