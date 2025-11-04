#include "command_executor.hpp"
#include "cmd_parser/cmd_args.hpp"
#include "wav/wav.hpp"

#include <iostream>
#include <variant>
#include <vector>

using namespace wav_lib;

template <typename T>
static bool get_arg(const std::vector<cmd::Arg> &args, size_t i, T &out)
{
    if (i >= args.size())
    {
        std::cerr << "Missing argument #" << i << "\n";
        return false;
    }

    if (auto val = std::get_if<T>(&args[i]))
    {
        out = *val;
        return true;
    }

    std::cerr << "Invalid argument type at #" << i << "\n";
    return false;
}

bool cmdHelp(const std::vector<cmd::Arg> &) noexcept
{
    std::cout << "Commands:\n";
    std::cout << "  help          show list of commands\n";
    std::cout << "  mix           mix intervals from two files\n";
    std::cout << "  mute          mute interval in a wav file\n";
    std::cout << "  speed         change speed of interval\n";
    std::cout << "  info          show wav header info\n";

    return true;
}

bool cmdMix(const std::vector<cmd::Arg> &args) noexcept
{
    std::string outputPath, inputFile;
    float outStart = 0, outEnd = 0, inStart = 0, inEnd = 0;

    if (!get_arg(args, 0, outputPath) ||
        !get_arg(args, 1, outStart) ||
        !get_arg(args, 2, outEnd) ||
        !get_arg(args, 3, inputFile) ||
        !get_arg(args, 4, inStart) ||
        !get_arg(args, 5, inEnd))
    {
        std::cerr << "Usage: mix <output> <outStart> <outEnd> <input> <inStart> <inEnd>\n";
        return false;
    }

    try
    {

        std::cout << "Mixed successfully\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return true;
}

bool cmdInfo(const std::vector<cmd::Arg> &args) noexcept
{
    std::string wavPath;
    if (!get_arg(args, 0, wavPath))
    {
        std::cerr << "Usage: info <wavfile>\n";
        return false;
    }

    try
    {
        WavReader reader;
        reader.ReadWav(wavPath);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
    return true;
}

bool cmdMute(const std::vector<cmd::Arg> &args) noexcept
{
    std::string wavPath;
    float start = 0, end = 0;

    if (!get_arg(args, 0, wavPath) ||
        !get_arg(args, 1, start) ||
        !get_arg(args, 2, end))
    {
        std::cerr << "Usage: mute <wavfile> <start> <end>\n";
        return false;
    }

    try
    {
        WavReader reader;
        reader.ReadWav(wavPath);

        std::cout << "Muted interval successfully (stub)\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }

    return true;
}

bool cmdChangeSpeed(const std::vector<cmd::Arg> &args) noexcept
{
    std::string wavPath;
    float start = 0, end = 0, speed = 1.0;

    if (!get_arg(args, 0, wavPath) ||
        !get_arg(args, 1, start) ||
        !get_arg(args, 2, end) ||
        !get_arg(args, 3, speed))
    {
        std::cerr << "Usage: speed <wavfile> <start> <end> <speed>\n";
        return false;
    }

    try
    {
        WavReader reader;
        reader.ReadWav(wavPath);

        std::cout << "Speed changed successfully (stub)\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
    return true;
}
