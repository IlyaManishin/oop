#pragma once

#include "internal/wav_exceptions.hpp"

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
        uint32_t sampleRate; // Freq per second
        uint32_t byteRate;
        uint16_t blockAlign;    // All channels block size
        uint16_t bitsPerSample; // Per one channel
        uint32_t subchunk2Size; // full wav data size
    } TWavHeader;

    class WavFile;
    class IWavInterval;
    class WavInterval;
    class ISampleReader;
    class Sample;

    using WavFileSPtr = std::shared_ptr<WavFile>;
    using IWavIntervalSPtr = std::shared_ptr<IWavInterval>;
    using WavIntervalSPtr = std::shared_ptr<WavInterval>;

    enum class WavEffects
    {
        NORMAL,
        BASS,
        ULTRA_BASS,
        RAISE_HIGH,
        RAISE_MIDDLE
    };

    class IWavInterval
    {
    public:
        virtual ~IWavInterval() = default;

        virtual void SetEffect(WavEffects effect) = 0;
        virtual void SetVolume(float value) = 0;
        virtual bool IsChangedSound() = 0;
    };

    class WavFile
    {
    public:
        static WavFileSPtr Open(const std::string &path);
        static WavFileSPtr Create(const std::string &path,
                                  uint16_t channels,
                                  uint32_t sampleRate,
                                  uint16_t bitsPerSample);

        void PrintInfo() const;
        TWavHeader GetHeader() const { return this->header; };

        void PlayWav();
        void Save();

        IWavIntervalSPtr GetInterval(float startSec, float endSec);
        void WriteInterval(IWavIntervalSPtr intervalI, float destSecPos, bool isInsert = false);

        ~WavFile();

    private:
        WavFile(const std::string &wavPath, bool createNew);

        std::string path;
        std::fstream file;

        std::streampos dataStart;
        std::streampos dataEnd;

        bool isChanged;

        TWavHeader header;

        void extractFileData();
        void saveHeader();
        void initNewHeader(uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample);
        void updateSubchunkSize();

        bool allocIntervalSpace(WavIntervalSPtr interval, uint32_t intervalLength, std::streampos destPos);

        void writeIntervalFast(WavIntervalSPtr interval, bool isInsert, std::streampos destPos);
        void writeIntervalFromCurFast(WavIntervalSPtr interval, std::streampos destPos);
        void writeIntervalFromOtherFast(WavIntervalSPtr interval, std::streampos destPos);

        void writeIntervalSlow(WavIntervalSPtr interval, bool isInsert, std::streampos destPos);
        void writeIntervalFromCurSlow(WavIntervalSPtr interval, std::streampos destPos);
        void writeIntervalFromOtherSlow(WavIntervalSPtr interval, std::streampos destPos);

        void writeIntervalWithReader(WavIntervalSPtr interval, std::streampos destPos,
             ISampleReader &reader, uint32_t maxSamples);
        void writeSample(Sample &sample);

        bool cmpVolumeParams(WavFile *other);
        bool operator==(WavFile &file) { return this->path == file.path; };
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
