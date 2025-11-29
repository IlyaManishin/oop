#pragma once

#include "wav_exceptions.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

namespace wav_lib
{
    typedef struct TWavHeader
    {
        uint32_t chunkSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign; // all channels block size
        uint16_t bitsPerSample;
        uint32_t subchunk2Size;
    } TWavHeader;

    class WavInterval;
    class WavFile;
    class WavSample;
    using WavFileSPtr = std::shared_ptr<WavFile>;

    class WavFile
    {
    public:
        static WavFileSPtr Open(const std::string &path);
        static WavFileSPtr Create(const std::string &path,
                                  uint16_t channels,
                                  uint32_t sampleRate,
                                  uint16_t bitsPerSample);

        void PrintInfo();
        TWavHeader GetHeader();

        void PlayWav();
        void Save();

        WavInterval GetInterval(float startSec, float endSec);
        void WriteInterval(WavInterval interval);

        ~WavFile();

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

        TWavHeader header;

        void extractFileData();
        void saveHeader();
        void initNewHeader(uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample);

        // size_t getReadPos();
        // size_t getWritePos();
        
        // void setReadPos(size_t byteOffset);
        // void setWritePos(size_t byteOffset);

        // void rewindFilePos() { this->file.seekg(this->dataStart); };
        // void rewindFilePos() { this->file.seekp(this->dataStart); };
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
