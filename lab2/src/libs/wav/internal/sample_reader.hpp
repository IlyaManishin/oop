#pragma once

#include "../wav.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include <cinttypes>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace wav_lib
{
    struct SReaderSoundParams
    {
        uint32_t startPos;
        uint32_t samplesCount;
        uint32_t channelsCount;
        uint32_t bitsPerSample;
    };

    class ISampleReader
    {
    protected:
        SReaderSoundParams input;
        SReaderSoundParams output;
        uint32_t maxSamples;

    public:
        ISampleReader(SReaderSoundParams input, SReaderSoundParams output, size_t maxSamples)
            : input(input), output(output), maxSamples(maxSamples) {};

        virtual bool IsBad() const = 0;
        SampleBufferSPtr InitBuffer(size_t size)
        {
            return std::make_unique<SampleBuffer>(size);
        }
        virtual size_t ReadSampleBuffer(SampleBufferSPtr buffer) = 0;
    };

    struct FileSReaderConfig
    {
        std::fstream &srcFile;
        std::streampos startPos;
    };

    class FileSReader : public ISampleReader
    {
    private:
        std::fstream &srcFile;
        std::streampos startPos;
        uint32_t channelsCount;
        uint32_t bytesPerChannel;

        uint32_t curSampleCount = 0;

    public:
        FileSReader(FileSReaderConfig cfg,
                    SReaderSoundParams input, SReaderSoundParams output, size_t maxSamples)
            : ISampleReader(input, output, maxSamples),
              srcFile(cfg.srcFile),
              startPos(cfg.startPos) {};

        bool IsBad() const override { return this->srcFile.bad(); }
        size_t ReadSampleBuffer(SampleBufferSPtr buffer) override;
    };

    class VectorSReader : public ISampleReader
    {
    private:
        const std::vector<char> &buffer;
        size_t offsetBytes;
        uint32_t channelsCount;
        uint32_t bytesPerChannel;

        uint32_t curSampleCount = 0;

    public:
        VectorSReader(const std::vector<char> &buffer,
                      size_t offsetBytes,
                      SReaderSoundParams input,
                      SReaderSoundParams output,
                      size_t maxSamples)
            : ISampleReader(input, output, maxSamples),
              buffer(buffer),
              offsetBytes(offsetBytes) {};

        bool IsBad() const override { return false; };
        size_t ReadSampleBuffer(SampleBufferSPtr buffer) override;
    };

} // namespace wav_lib
