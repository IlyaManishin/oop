#include "cmd_parser/cmd_args.hpp"
#include "executor.hpp"
#include "wav/wav.hpp"

#include <iostream>
#include <variant>
#include <vector>

using namespace wav_lib;

namespace command_executor
{
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

    static WavFile *read_wav_file(const WavReader &reader, const std::string &path)
    {
        try
        {
            WavFile *wavFile = reader.ReadWav(path);
            return wavFile;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return nullptr;
        }
    }

    bool cmd_run_from_config_file(const std::vector<cmd::Arg> &args) noexcept
    {
    }

    bool cmd_help(const std::vector<cmd::Arg> &) noexcept
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

    bool cmd_mix_impl(WavFile *outFile, float outStart, float outEnd,
                      WavFile *inFile, float inStart, float inEnd)
    {
        std::cout << "Mixed successfully\n";
        return true;
    }

    bool cmd_mix(const std::vector<cmd::Arg> &args) noexcept
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
            WavReader reader;
            WavFile *outFile = read_wav_file(reader, outputPath);
            if (!outFile)
                return false;
            WavFile *inFile = read_wav_file(reader, inputFile);
            if (!inFile)
                return false;

            return cmd_mix_impl(outFile, outStart, outEnd, inFile, inStart, inEnd);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }

    bool cmd_info_impl(WavFile *wavFile)
    {
        wavFile->PrintInfo();
        return true;
    }

    bool cmd_info(const std::vector<cmd::Arg> &args) noexcept
    {
        std::string wavPath;
        if (!get_arg(args, 0, wavPath))
        {
            std::cerr << "Usage: info <wavfile>\n";
            return false;
        }
        WavReader reader;
        WavFile *wavFile = read_wav_file(reader, wavPath);
        if (!wavFile)
            return false;

        return cmd_info_impl(wavFile);
    }

    bool cmd_mute_impl(WavFile *wavFile, float start, float end)
    {
        std::cout << "Muted interval successfully (stub)\n";
        return true;
    }

    bool cmd_mute(const std::vector<cmd::Arg> &args) noexcept
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
            WavFile *wavFile = read_wav_file(reader, wavPath);
            if (!wavFile)
                return false;

            return cmd_mute_impl(wavFile, start, end);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }

    bool cmd_change_speed_impl(WavFile *wavFile, float start, float end, float speed)
    {
        std::cout << "Speed changed successfully (stub)\n";
        return true;
    }

    bool cmd_change_speed(const std::vector<cmd::Arg> &args) noexcept
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
            WavFile *wavFile = read_wav_file(reader, wavPath);
            if (!wavFile)
                return false;

            return cmd_change_speed_impl(wavFile, start, end, speed);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
} // namespace command_executor
