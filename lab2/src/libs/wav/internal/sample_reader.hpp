#pragma once

#include "../wav.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"
#include "wav_utils.hpp"

#include <cinttypes>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace wav_lib
{
    struct SReaderSoundParams
    {
        uint32_t channelsCount;
        uint32_t bytesPerSample;
        uint32_t blockAlign;
        uint32_t sampleRate;
    };

    struct SampleReaderConfig
    {
        size_t maxSamples;

        SReaderSoundParams input;
        SReaderSoundParams output;

        WavEffects effect;
        bool isNewVolume = false;
        float volumeValue = 1.0f;
    };

    class ISampleReader
    {
    protected:
        struct SampleChanges
        {
            uint32_t srcBlockAlign;
            uint32_t destBlockAlign;

            uint32_t srcDepth;
            uint32_t destDepth;
            uint32_t srcChannels;
            uint32_t destChannels;

            WavEffects effect;
            bool isNewVolume;
            float volumeValue;
        };

        bool isSameSampleRate;
        double sampleStep;
        double curSampleAccum;

        bool isSampleChanges;
        SampleChanges changes;

        uint32_t maxSamples;
        uint32_t samplesCount;

    public:
        ISampleReader(const SampleReaderConfig &cfg);

        SampleBufferSPtr InitBuffer(size_t size)
        {
            return std::make_unique<SampleBuffer>(size);
        }

        virtual bool IsBad() const = 0;
        virtual size_t ReadSampleBuffer(SampleBufferSPtr buffer) = 0;

    private:
        virtual size_t extractSampleBuffer(SampleBufferSPtr buffer) = 0;
        virtual void addBufferEffects(SampleBufferSPtr buffer) = 0;

        Sample normalizeSample(Sample sample);
    };

    class FileSReader : public ISampleReader
    {
    private:
        std::fstream &srcFile;
        std::streampos startPos;

    public:
        FileSReader(std::fstream &srcFile,
                    std::streampos startPos,
                    const SampleReaderConfig &cfg)
            : ISampleReader(cfg),
              srcFile(srcFile),
              startPos(startPos)
        {
            set_read_pos(this->srcFile, this->startPos);
        }

        bool IsBad() const override
        {
            return srcFile.bad();
        }

        size_t ReadSampleBuffer(SampleBufferSPtr buffer) override;
    };

    class VectorSReader : public ISampleReader
    {
    private:
        const ByteVector &data;
        size_t offsetBytes;
        uint32_t curSampleCount = 0;

    public:
        VectorSReader(const ByteVector &buffer,
                      size_t offsetBytes,
                      const SampleReaderConfig &cfg)
            : ISampleReader(cfg),
              data(buffer),
              offsetBytes(offsetBytes) {}

        bool IsBad() const override
        {
            return false;
        }

        size_t ReadSampleBuffer(SampleBufferSPtr outBuffer) override;
    };

} // namespace wav_lib
