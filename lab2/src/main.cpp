#include "cmd_parser.hpp"

#include <iostream>

enum class CMD
{
    HELP,
    MIX,
    MUTE,
    CHANGE_SPEED
};

cmd::CommandParser *init_cmd_parser()
{
    cmd::CommandParser *parser = new cmd::CommandParser();
    parser->registerCommand("help", static_cast<int>(CMD::HELP));
    parser->registerCommand("mix", static_cast<int>(CMD::MIX));
    parser->registerCommand("mute", static_cast<int>(CMD::MUTE));
    parser->registerCommand("change_speed", static_cast<int>(CMD::CHANGE_SPEED));
}

int main(int argc, char **argv)
{
    cmd::CommandParser* parser = init_cmd_parser();
    try
    {
        cmd::Command *command = parser->parse(argc, argv);
        

    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
