#pragma once

#include "wav_exceptions.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

namespace wav_lib
{
    class WavFile;
    class WavInterval;

    using WavFileSPtr = std::shared_ptr<WavFile>;

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

    class WavFile
    {
    public:
        static WavFileSPtr Open(const std::string &path);
        static WavFileSPtr Create(const std::string &path,
                                  uint16_t channels,
                                  uint32_t sampleRate,
                                  uint16_t bitsPerSample);
        ~WavFile();

        void PrintInfo();
        void PlayWav();
        void WriteInterval(const WavInterval &interval);
        void GetInterval(float startSec, float endSec);
        void Save();

    private:
        enum class Mode
        {
            OpenExisting,
            CreateNew
        };
        WavFile(const std::string &wavPath, Mode mode);

        std::string path;
        std::fstream file;

        std::streampos dataStart;
        std::streampos dataEnd;

        bool isChanged;

        _detail::TWavHeader header;

        void extractFileData();
        void saveHeader();
        void initNewHeader(uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample);
    };

    class WavInterval
    {
    public:
        WavInterval(WavFileSPtr file, float startSec, float endSec);

        void ChangeSpeed(float speed) { this->speed = speed; };

    private:
        WavFileSPtr wavFile;
        float startSec;
        float endSec;
        float speed;
    };

    class WavReader
    {
    public:
        WavReader(const std::string &wavDir = "") : wavDir(wavDir) {};
        WavFileSPtr ReadWav(const std::string &path) const;
        WavFileSPtr CreateWav(const std::string &destPath,
                              int numChannels = 2,
                              uint32_t sampleRate = 44100,
                              uint32_t bitsPerSample = 16) const;
        bool IsExistsWav(const std::string &path) const;

    private:
        std::string wavDir;

        bool isWavFile(const std::string &path) const;
    };
} // namespace wav_lib
