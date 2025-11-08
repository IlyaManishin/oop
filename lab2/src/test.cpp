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

static WavFile *read_wav_file(const WavReader &reader, const std::string &path)
{
    try
    {
        return reader.ReadWav(path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return nullptr;
    }
}

static std::pair<WavFile *, WavFile *> read_two_wavs(const WavReader &reader,
                                                     const std::string &path1,
                                                     const std::string &path2)
{
    WavFile *first = read_wav_file(reader, path1);
    WavFile *second = read_wav_file(reader, path2);

    if (!first || !second)
        return {nullptr, nullptr};
    return {first, second};
}

// =====================
// Implementation layer
// =====================

static bool cmd_info_impl(WavFile *wavFile)
{
    if (!wavFile)
        return false;
    wavFile->PrintInfo();
    return true;
}

static bool cmd_mute_impl(WavFile *wavFile, float start, float end)
{
    if (!wavFile)
        return false;
    std::cout << "Muted interval [" << start << ", " << end << "] (stub)\n";
    return true;
}

static bool cmd_change_speed_impl(WavFile *wavFile, float start, float end, float speed)
{
    if (!wavFile)
        return false;
    std::cout << "Changed speed on interval [" << start << ", " << end << "] to "
              << speed << " (stub)\n";
    return true;
}

static bool cmd_mix_impl(WavFile *outFile, float outStart, float outEnd,
                         WavFile *inFile, float inStart, float inEnd)
{
    if (!outFile || !inFile)
        return false;
    std::cout << "Mixed successfully: "
              << "[" << inStart << ", " << inEnd << "] → "
              << "[" << outStart << ", " << outEnd << "]\n";
    return true;
}

// =====================
// Command wrappers
// =====================

bool cmd_from_config_file(const std::vector<cmd::Arg> &) noexcept
{
    return false; // stub
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
    return cmd_info_impl(wavFile);
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

    WavReader reader;
    WavFile *wavFile = read_wav_file(reader, wavPath);
    return cmd_mute_impl(wavFile, start, end);
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

    WavReader reader;
    WavFile *wavFile = read_wav_file(reader, wavPath);
    return cmd_change_speed_impl(wavFile, start, end, speed);
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

    WavReader reader;
    auto [outFile, inFile] = read_two_wavs(reader, outputPath, inputFile);
    return cmd_mix_impl(outFile, outStart, outEnd, inFile, inStart, inEnd);
}
