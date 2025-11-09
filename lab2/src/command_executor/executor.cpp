#include "executor.hpp"
#include "cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"

#include <iostream>
#include <unordered_map>

namespace command_executor
{
    enum class CMD
    {
        FROM_FILE,
        HELP,
        INFO,
        MIX,
        MUTE,
        CHANGE_SPEED
    };

    using command_run_func = bool (*)(const std::vector<cmd::Arg> &);

    cmd::CommandParser *init_cmd_parser()
    {
        cmd::CommandParser *parser = new cmd::CommandParser();
        parser->registerCommand("config", (int)CMD::FROM_FILE);
        parser->registerCommand("help", (int)CMD::HELP);
        parser->registerCommand("info", (int)CMD::INFO);
        parser->registerCommand("mix", (int)CMD::MIX);
        parser->registerCommand("mute", (int)CMD::MUTE);
        parser->registerCommand("speed", (int)CMD::CHANGE_SPEED);

        return parser;
    }

    std::unordered_map<int, command_run_func> get_commands_map()
    {
        std::unordered_map<int, command_run_func> res;
        res[(int)CMD::FROM_FILE] = cmd_run_from_config_file;
        res[(int)CMD::HELP] = cmd_help;
        res[(int)CMD::INFO] = cmd_info;
        res[(int)CMD::MIX] = cmd_mix;
        res[(int)CMD::MUTE] = cmd_mute;
        res[(int)CMD::CHANGE_SPEED] = cmd_change_speed;

        return res;
    }

    bool run_from_cmd_args(int argc, char **argv)
    {
        cmd::CommandParser *parser = init_cmd_parser();
        cmd::Command *command;
        try
        {
            command = parser->parse(argc, argv);
        }
        catch (const std::runtime_error e)
        {
            std::cerr << e.what() << '\n';
            delete parser;
            return 0;
        }

        auto commandsMap = get_commands_map();
        auto command_func = commandsMap[command->commandId];
        bool res = command_func(command->args);

        delete parser;
        return res;
    }
}//namespace command_executor
