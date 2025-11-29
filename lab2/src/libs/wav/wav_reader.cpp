#include "wav.hpp"

#include <string>
#include <iostream>

namespace wav_lib
{
    WavFileSPtr WavReader::ReadWav(const std::string &path) const
    {
        try
        {
            auto file = WavFile::Open(path);
            return file;
        }
        catch (const WavException &e)
        {
            std::cerr << "Error reading WAV file \"" << path << "\": " << e.what() << "\n";
            return nullptr;
        }
    }

    WavFileSPtr WavReader::CreateWav(const std::string &destPath,
                                    int numChannels,
                                    uint32_t sampleRate,
                                    uint32_t bitsPerSample) const
    {
        try
        {
            auto file = WavFile::Create(destPath, numChannels, sampleRate, bitsPerSample);
            return file;
        }
        catch (const WavException &e)
        {
            std::cerr << "Error creating WAV file \"" << destPath << "\": " << e.what() << "\n";
            return nullptr;
        }
    }

    bool WavReader::isWavFile(const std::string &path) const
    {
        try
        {
            WavFileSPtr file = WavFile::Open(path);
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

        return this->isWavFile(path);
    }

} // namespace wav_lib
