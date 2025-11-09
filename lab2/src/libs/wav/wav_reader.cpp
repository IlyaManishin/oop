#include "wav.hpp"
#include <fstream>

namespace wav_lib
{
    WavFile *WavReader::ReadWav(const std::string &path) const
    {
        WavFile *wavFile = new WavFile(path);
        return wavFile;
    }

    bool WavReader::IsExistsWav(const std::string &path) const
    {
        std::fstream wavStream(path);
        if (!wavStream.is_open())
            return false;

        wavStream.close();
        return true;
    }

} // namespace wav_lib
