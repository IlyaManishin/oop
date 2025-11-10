#include "wav.hpp"
#include <fstream>
#include <iostream>

namespace wav_lib
{
    WavFile *WavReader::ReadWav(const std::string &path) const
    {
        WavFile *wavFile = new WavFile(path);
        return wavFile;
    }

    bool WavReader::_isWavFile(const std::string &path) const
    {
        try
        {
            WavFile *file = this->ReadWav(path);
            delete file;
            return true;
        }
        catch (const WavException &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    bool WavReader::IsExistsWav(const std::string &path) const
    {
        std::fstream wavStream(path);
        if (!wavStream.is_open())
            return false;

        wavStream.close();

        return this->_isWavFile(path);
    }

} // namespace wav_lib
