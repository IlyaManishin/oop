#include "cmd_parser.hpp"

#include <cstdlib>

namespace cmd
{
    void CommandParser::registerCommand(const std::string &name, int id)
    {
        commandMap[name] = id;
    }
    
    Command *CommandParser::parse(int argc, char **argv) const
    {
        if (argc < 2)
            throw std::runtime_error("No commands");
    
        auto it = commandMap.find(argv[1]);
        if (it == commandMap.end())
            throw std::runtime_error("Unknown command: " + std::string(argv[1]));
    
        std::vector<Arg> args;
        for (int i = 2; i < argc; ++i)
            args.push_back(parseArg(argv[i]));
    
        return new Command{it->second, args};
    }
    
    Arg CommandParser::parseArg(const std::string &token)
    {
        char *end = nullptr;
    
        long i = std::strtol(token.c_str(), &end, 10);
        if (*end == '\0')
            return static_cast<int>(i);
    
        double d = std::strtod(token.c_str(), &end);
        if (*end == '\0')
            return d;
    
        return token;
    }
} // namespace cmd

