#include "wav/wav.hpp"

#include "utils.hpp"
#include <exception>
#include <iostream>
#include <string>

using namespace wav_lib;

namespace executor
{
    WavFileSPtr try_read_wav(const WavReader &reader, const std::string &path) noexcept
    {
        try
        {
            WavFileSPtr wavFile = reader.OpenWav(path);
            return wavFile;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return nullptr;
        }
    }
} // namespace executor
