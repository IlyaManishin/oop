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
        struct ReaderParams
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

        bool isDiffSampleRate;
        double sampleStep;
        double curSampleAccum;

        bool isSampleChanges;
        ReaderParams params;

        uint32_t maxSamples;
        uint32_t allSamplesCount;

        bool isError;

    public:
        ISampleReader(const SampleReaderConfig &cfg);

        virtual bool IsBad() const = 0;
        size_t ReadToSampleBuffer(ByteVector& dest);

    private:
        virtual size_t extractSampleBuffer(ByteVector &dest, size_t maxSamples) = 0;
        void addBufferEffects(ByteVector& buffer);
    };

    class FileSReader : public ISampleReader
    {
    private:
        std::fstream &srcFile;
        std::streampos startPos;

    public:
        FileSReader(std::fstream &srcFile,
                    std::streampos startPos,
                    const SampleReaderConfig &cfg);

        bool IsBad() const override;

    private:
        size_t extractSampleBuffer(ByteVector &dest, size_t maxSamples) override;
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
                      const SampleReaderConfig &cfg);

        bool IsBad() const override;

    private:
        size_t extractSampleBuffer(ByteVector &dest, size_t maxSamples) override;
    };



} // namespace wav_lib
