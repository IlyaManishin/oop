#include "wav/wav.hpp"

#include "utils.hpp"
#include <iostream>
#include <string>

using namespace wav_lib;

namespace executor
{
    WavFileSPtr try_read_wav(const WavReader &reader, const std::string &path)
    {
        try
        {
            WavFileSPtr wavFile = reader.ReadWav(path);
            return wavFile;
        }
        catch (const WavException &e)
        {
            std::cerr << e.what() << "\n";
            return nullptr;
        }
    }
} // namespace executor
