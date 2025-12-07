#pragma once

#include "../wav.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>

namespace wav_lib
{
    class Sample
    {
    public:
        bool isError = false;

        std::vector<byteVector> channelsData;

        Sample() {};
        Sample(uint32_t channels, uint32_t bitsPerSample); // empty
        Sample(std::vector<byteVector> channelsData)
            : channelsData(channelsData) {};

        bool IsError() { return this->isError; };
    };

    struct SReaderConfig
    {
        std::string &path;
        std::fstream &srcFile;
        uint32_t startPos;
        uint32_t samplesCount;
        uint32_t channelsCount;
        uint32_t bitsPerSample;
    };

    class SampleReader
    {
    private:
        std::string &path;
        std::fstream &srcFile;
        uint32_t startPos;
        uint32_t samplesCount;
        uint32_t channelsCount;
        uint32_t bytesPerChannel;

        uint32_t curSampleCount = 0;

    public:
        SampleReader(const SReaderConfig &cfg)
            : path(cfg.path),
              srcFile(cfg.srcFile),
              startPos(cfg.startPos),
              samplesCount(cfg.samplesCount),
              channelsCount(cfg.channelsCount),
              bytesPerChannel(cfg.bitsPerSample / 8)
        {
            srcFile.seekg(static_cast<std::streamoff>(startPos), std::ios::beg);
        };

        bool ReadSample(Sample &dest);
    };

} // namespace wav_lib
