#pragma once

#include "wav_exceptions.hpp"

#include <cstdint>
#include <fstream>
#include <string>

namespace wav_lib
{
    namespace _detail
    {
        typedef struct TWavHeader
        {
            uint32_t chunkSize;
            uint16_t audioFormat;
            uint16_t numChannels;
            uint32_t sampleRate;
            uint32_t byteRate;
            uint16_t blockAlign;
            uint16_t bitsPerSample;
            uint32_t subchunk2Size;
        } TWavHeader;
    } // namespace _detail

    class WavInterval;

    class WavFile
    {
    public:
        WavFile(const std::string &wavPath, bool isExists = true);
        ~WavFile();

        void PrintInfo();
        void PlayWav();
        void WriteInterval(const WavInterval &interval);
        void GetInterval(float startSec, float endSec);
        void Close();

    private:
        std::string path;
        std::fstream file;

        std::streampos dataStart;
        std::streampos dataEnd;

        bool isChanged;
        bool isInit;

        _detail::TWavHeader header;

        void extractFileData();
        void _save_header();
    };

    class WavInterval
    {
    public:
        WavInterval(WavFile *file, float startSec, float endSec);

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
        WavReader(const std::string &wavDir = "", bool) : wavDir(wavDir) {};
        WavFile *ReadWav(const std::string &path) const;
        WavFile *CreateWav(const std::string &destPath,
                           int numChannels = 2,
                           uint32_t sampleRate = 44100,
                           uint32_t bitsPerSample = 16) const;
        bool IsExistsWav(const std::string &path) const;

    private:
        std::string wavDir;

        bool _isWavFile(const std::string &path) const;
    };
} // namespace wav_lib
