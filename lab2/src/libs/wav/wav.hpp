#pragma once

#include "internal/wav_exceptions.hpp"

#include <fstream>
#include <string>


namespace wav_lib
{
    class WavInterval;

    class WavFile
    {
        class WavHeader;
    public:
        WavFile(const std::string &wavPath);
        void PrintInfo();
        void PlayWav();
        void WriteInterval(const WavInterval &interval);
        void GetInterval(float startSec, float endSec);

        ~WavFile();

    private:
        std::string path;
        std::fstream file;
        
        uint32_t chunkSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        uint32_t subchunk2Size;

        std::streampos dataStart;
        std::streampos dataEnd;

        void _extract_file_data();
    };

    class WavInterval
    {
    public:
        WavInterval(WavFile *file, float startSec, float endSec)
            : file(file), startSec(startSec), endSec(endSec), speed(1) {};
        void ChangeSpeed(float speed) { this->speed = speed; };

    private:
        WavFile *file;
        float startSec;
        float endSec;
        float speed;
    };

    class WavReader
    {
    public:
        WavReader(const std::string &wavDir = "") : wavDir(wavDir) {};
        WavFile *ReadWav(const std::string &path) const;
        WavFile *CreateWav(const std::string &destPath) const;
        bool IsExistsWav(const std::string &path) const;

    private:
        std::string wavDir;
        bool _isWavFile(std::string &path);
    };
} // namespace wav_lib
