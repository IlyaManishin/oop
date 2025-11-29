#pragma once

#include "../wav.hpp"

#include <cinttypes>
#include <iostream>
#include <vector>

namespace wav_lib
{
    typedef char byte;
    typedef std::vector<byte> byteVector;

    class Sample
    {
    public:
        std::vector<byteVector> channelsData;
        uint32_t chDataSize;

        Sample(std::vector<byteVector> channelsData, uint32_t chDataSize)
            : channelsData(channelsData), chDataSize(chDataSize) {};
    };

    struct SampleReaderConfig
    {
        std::fstream *srcFile;
        size_t startPos;
        size_t samplesCount;
        size_t channelsCount;
        size_t bitsPerSample;
    };

    class SampleReader
    {
    private:
        std::fstream &srcFile;
        size_t startPos;
        size_t samplesCount;
        size_t channelsCount;
        size_t bitsPerSample;

    public:
        SampleReader(const SampleReaderConfig &cfg)
            : srcFile(*cfg.srcFile),
              startPos(cfg.startPos),
              samplesCount(cfg.samplesCount),
              channelsCount(cfg.channelsCount),
              bitsPerSample(cfg.bitsPerSample) {};
    };

} // namespace wav_lib
