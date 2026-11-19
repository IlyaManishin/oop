#pragma once

#include "../wav.hpp"
#include "config.hpp"
#include "types.hpp"
#include "exceptions.hpp"
#include "wav_utils.hpp"

#include <cinttypes>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace wav_lib
{
    struct SampleReaderConfig
    {
        size_t maxSamples;
        size_t bufferSize = config::SAMPLE_BUFFER_SIZE;

        TWavSoundParams input;
        TWavSoundParams output;

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
        ReaderParams params;
        bool isDiffSampleRate;
        bool isSoundChanges;

        ByteVector *buffer = nullptr;
        uint32_t maxSamples;
        size_t bufChunkCount;

        double sampleStep;
        uint32_t allSamplesCount = 0;
        double curSampleAccum = 0;

        bool isError = false;

    public:
        ISampleReader(const SampleReaderConfig &cfg);

        virtual bool IsBad() const = 0;
        size_t ReadToSampleBuffer();
        const ByteVector *GetBuffer() { return this->buffer; };
        ~ISampleReader();

    private:
        virtual size_t readDataToBuf(size_t maxSamples) = 0;
        void addBufferEffects();
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
        size_t readDataToBuf(size_t maxSamples) override;
    };

    class VectorSReader : public ISampleReader
    {
    private:
        const ByteVector &wavData;
        size_t offsetBytes;
        uint32_t curSampleCount = 0;

    public:
        VectorSReader(const ByteVector &wavData,
                      size_t offsetBytes,
                      const SampleReaderConfig &cfg);

        bool IsBad() const override;

    private:
        size_t readDataToBuf(size_t maxSamples) override;
    };

} // namespace wav_lib
