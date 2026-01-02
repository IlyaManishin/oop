#include "base_commands/core_commands.hpp"
#include "utils/utils.hpp"

#include "cmd_parser/cmd_parser.hpp"
#include "command_executor.hpp"
#include "file_executor.hpp"
#include "file_parser/parser.hpp"
#include "wav/wav.hpp"

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
            return false;

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
        std::string configPath, loggingPath;
        std::ostream *out = &std::cout;
        std::ofstream logger;

        if (!get_arg(args, 0, configPath))
        {
            std::cerr << "Usage: file <config_file> <log file>?";
            return false;
        }
        if (get_arg(args, 1, loggingPath))
        {
            logger.open(loggingPath);
            out = &logger;
        }

        try
        {
            file_executor::FileExecutor::run_from_config_file(configPath, *out);
            return true;
        }
        catch (const std::exception &exc)
        {
            std::cerr << exc.what() << '\n';
            return false;
        }
    }

    bool cmd_help(const Args &) noexcept
    {
        std::cout << "Commands:\n";
        std::cout << "  config        run from config file\n";
        std::cout << "  help          show list of commands\n";
        std::cout << "  create        create new wav file\n";
        std::cout << "  mix           mix intervals from two files\n";
        std::cout << "  effect        apply sound effect to wav file\n";
        std::cout << "  info          show wav header info\n";

        return true;
    }

    bool cmd_create_wav(const cmd_parser::Args &args) noexcept
    {
        const std::string usage = "Usage: create <wavfile>\n";

        std::string path;
        if (!get_arg(args, 0, path))
        {
            std::cerr << usage;
            return false;
        }

        WavReader reader;
        try
        {
            reader.CreateWav(path);
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Create wav error: \n"
                      << e.what() << "\n";
            return false;
        }
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
            cmd_wav_mix_impl(outFile, outStart, inFile, inStart, inEnd);
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Mix error: \n"
                      << e.what() << "\n";
            return false;
        }
    }

    bool cmd_sound_effect(const cmd_parser::Args &args) noexcept
    {
        std::string usage = "Usage: effect <effect_name> <wavfile> [start] [end]\n";
        usage += "Available effects:\n";
        usage += "  normal        no effect\n";
        usage += "  bass          boost low frequencies\n";
        usage += "  hach_lada     aggressive bass effect\n";
        usage += "  raise_high    boost high frequencies\n";
        usage += "  distortion    signal distortion\n";

        std::string effect, wavPath;
        float start = 0, end = -1;

        if (!get_arg(args, 0, effect) ||
            !get_arg(args, 1, wavPath))
        {
            std::cerr << usage;
            return false;
        }
        if (args.size() > 2)
        {
            if (args.size() != 4 ||
                (!get_arg(args, 2, start) || !get_arg(args, 3, end)))
            {
                std::cerr << usage;
                return false;
            }
        }

        WavReader reader;
        WavFileSPtr wavFile = try_read_wav(reader, wavPath);
        if (!wavFile)
            return false;

        if (end < 0.0f)
            end = wavFile->GetDurationSec();

        if (start < 0 || start > end || end > wavFile->GetDurationSec())
        {
            std::cerr << "Invalid effect interval\n";
            return false;
        }

        try
        {
            cmd_set_effect_impl(wavFile, effect, start, end);
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Effect error: \n"
                      << e.what() << "\n";
            return false;
        }
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
            cmd_wav_info_impl(wavFile);
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Wav info error: \n"
                      << e.what() << '\n';
            return false;
        }
    }

} // namespace executor
