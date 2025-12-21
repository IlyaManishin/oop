#include "cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"
#include "command_executor.hpp"

#include <iostream>
#include <unordered_map>

namespace executor
{
    enum class COMMANDS
    {
        FROM_FILE,
        HELP,
        INFO,
        MIX,
        EFFECT,
    };

    using command_run_func = bool (*)(const std::vector<cmd_parser::Arg> &);

    cmd_parser::CommandParser *init_cmd_parser()
    {
        cmd_parser::CommandParser *parser = new cmd_parser::CommandParser();
        parser->registerCommand("config", (int)COMMANDS::FROM_FILE);
        parser->registerCommand("help", (int)COMMANDS::HELP);
        parser->registerCommand("info", (int)COMMANDS::INFO);
        parser->registerCommand("mix", (int)COMMANDS::MIX);
        parser->registerCommand("effect", (int)COMMANDS::EFFECT);

        return parser;
    }

    std::unordered_map<int, command_run_func> get_commands_map()
    {
        std::unordered_map<int, command_run_func> res;
        res[(int)COMMANDS::FROM_FILE] = cmd_run_from_config_file;
        res[(int)COMMANDS::HELP] = cmd_help;
        res[(int)COMMANDS::INFO] = cmd_info;
        res[(int)COMMANDS::MIX] = cmd_mix;
        res[(int)COMMANDS::EFFECT] = cmd_sound_effect;
        // res[(int)COMMANDS::CHANGE_SPEED] = cmd_change_speed;

        return res;
    }

    bool run_from_cmd_args(int argc, char **argv)
    {
        cmd_parser::CommandParser *parser = init_cmd_parser();
        cmd_parser::Command *command;
        try
        {
            command = parser->parse(argc, argv);
        }
        catch (const std::runtime_error &e)
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
} // namespace executor
