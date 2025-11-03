#include "command_executor.hpp"
#include "cmd_parser/cmd_args.hpp"

#include <iostream>
#include <vector>

void cmdHelp(const std::vector<cmd::Arg> &args)
{
    std::cout << "Commands:\n";
    std::cout << "  help          show list of commands\n";
    std::cout << "  mix           Mix intervals\n";
    std::cout << "  mute          Mute interval\n";
    std::cout << "  speed         Change speed of interval\n";
}

void cmdMix(const std::vector<cmd::Arg> &args)
{
    std::string outputPath, inputFile;
    float outStart, outEnd, inStart, inEnd;
    try
    {
        outputPath = std::get<std::string>(args[0]);
        outStart = static_cast<float>(std::get<int>(args[1]));
        outEnd = static_cast<float>(std::get<int>(args[2]));
        inputFile = std::get<std::string>(args[3]);
        inStart = static_cast<float>(std::get<int>(args[4]));
        inEnd = static_cast<float>(std::get<int>(args[5]));
    }
    catch (const std::bad_variant_access &e)
    {
        std::cerr << "Mix: argument type mismatch: " << e.what() << '\n';
        return;
    }


}

void cmdMute(const std::vector<cmd::Arg> &args)
{

}

void cmdChangeSpeed(const std::vector<cmd::Arg> &args)
{

}
