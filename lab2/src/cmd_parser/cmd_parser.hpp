#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cmd
{
    using Arg = std::variant<int, double, std::string>;
    
    struct Command
    {
        int commandId;
        std::vector<Arg> args;
    };
    
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


