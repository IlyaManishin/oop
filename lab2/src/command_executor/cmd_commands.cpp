#include "cmd_parser/cmd_parser.hpp"
#include "wav/wav.hpp"

#include "../file_executor/file_commands.hpp"
#include "base_commands/core_commands.hpp"
#include "executor.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <variant>
#include <vector>

using namespace wav_lib;
using namespace cmd_parser;

namespace executor
{
    template <typename T>
    static bool get_arg(const Args &args, size_t i, T &out) noexcept
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

    bool cmd_run_from_config_file(const Args &args) noexcept
    {
        std::string configPath;
        if (!get_arg(args, 0, configPath))
        {
            std::cerr << "Usage: file <config_file> ";
            return false;
        }
        bool res = run_from_config_file(configPath);
        return res;
    }

    bool cmd_help(const Args &) noexcept
    {
        std::cout << "Commands:\n";
        std::cout << "  file          add commands file\n";
        std::cout << "  help          show list of commands\n";
        std::cout << "  mix           mix intervals from two files\n";
        std::cout << "  mute          mute interval in a wav file\n";
        std::cout << "  speed         change speed of interval\n";
        std::cout << "  info          show wav header info\n";

        return true;
    }

    bool cmd_mix(const Args &args) noexcept
    {
        std::string outputPath, inputFile;
        float outStart = 0, inStart = 0, inEnd = 0;

        if (!get_arg(args, 0, outputPath) ||
            !get_arg(args, 1, outStart) ||
            !get_arg(args, 2, inputFile) ||
            !get_arg(args, 3, inStart) ||
            !get_arg(args, 4, inEnd))
        {
            std::cerr << "Usage: mix <output> <outStart> <input> <inStart> <inEnd>\n";
            return false;
        }

        WavReader reader;
        WavFileSPtr outFile = try_read_wav(reader, outputPath);
        if (!outFile)
        {
            return false;
        }
        WavFileSPtr inFile = try_read_wav(reader, inputFile);
        if (!inFile)
        {
            return false;
        }

        try
        {
            return cmd_mix_impl(outFile, outStart, inFile, inStart, inEnd);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }

    bool cmd_sound_effect(const cmd_parser::Args &args) noexcept
    {
        return false;
    }

    bool cmd_info(const Args &args) noexcept
    {
        std::string wavPath;
        if (!get_arg(args, 0, wavPath))
        {
            std::cerr << "Usage: info <wavfile>\n";
            return false;
        }
        WavReader reader;
        WavFileSPtr wavFile = try_read_wav(reader, wavPath);
        if (!wavFile)
        {
            return false;
        }

        try
        {
            bool res = cmd_info_impl(wavFile);
            return res;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return false;
    }

    bool cmd_mute(const Args &args) noexcept
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
            WavFileSPtr wavFile = try_read_wav(reader, wavPath);
            if (!wavFile)
            {
                return false;
            }

            return cmd_mute_impl(wavFile, start, end);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }

    bool cmd_change_speed(const Args &args) noexcept
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

        WavReader reader;
        WavFileSPtr wavFile = try_read_wav(reader, wavPath);
        if (!wavFile)
        {
            return false;
        }

        try
        {
            return cmd_change_speed_impl(wavFile, start, end, speed);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
} // namespace executor
