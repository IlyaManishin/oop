#include "wav.hpp"

#include <string>

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

    WavFile *WavReader::CreateWav(const std::string &destPath,
                                  int numChannels,
                                  uint32_t sampleRate,
                                  uint32_t bitsPerSample) const
    {
        
    }
} // namespace wav_lib
