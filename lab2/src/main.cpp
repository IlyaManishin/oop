#include "cmd_parser/cmd_parser.hpp"
#include "command_executor.hpp"

#include <iostream>
#include <unordered_map>

namespace project
{
    enum class CMD
    {
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
        res[(int)CMD::HELP] = cmdHelp;
        res[(int)CMD::INFO] = cmdInfo;
        res[(int)CMD::MIX] = cmdMix;
        res[(int)CMD::MUTE] = cmdMute;
        res[(int)CMD::CHANGE_SPEED] = cmdChangeSpeed;

        return res;
    }

} // namespace project

using namespace project;

int main(int argc, char **argv)
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
        return EXIT_FAILURE;
    }

    auto commandsMap = get_commands_map();
    auto command_func = commandsMap[command->commandId];
    bool res = command_func(command->args);

    delete parser;
    return res ? EXIT_SUCCESS : EXIT_FAILURE;
}