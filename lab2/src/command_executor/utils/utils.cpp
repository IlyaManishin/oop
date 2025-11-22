#include "wav/wav.hpp"
#include <iostream>
#include <string>

using namespace wav_lib;

namespace executor
{
    static WavFile *try_read_wav_file(const WavReader &reader, const std::string &path) noexcept
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

} // namespace executor
