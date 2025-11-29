#pragma once

#include "../wav.hpp"
#include "wav_exceptions.hpp"

#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>

namespace wav_lib
{
    typedef char byte;
    typedef std::vector<byte> byteVector;

    class Sample
    {
    public:
        bool isEOF = false;
        std::vector<byteVector> channelsData;

        Sample() {};
        Sample(std::vector<byteVector> channelsData)
            : channelsData(channelsData) {};
    };

    struct SReaderConfig
    {
        std::string &path;
        std::fstream &srcFile;
        size_t startPos;
        size_t samplesCount;
        size_t channelsCount;
        size_t bitsPerSample;
    };

    class SampleReader
    {
    private:
        std::string &path;
        std::fstream &srcFile;
        size_t startPos;
        size_t samplesCount;
        size_t channelsCount;
        size_t bytesPerChannel;

        size_t curSampleCount = 0;

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

        Sample ReadSample();
    };

} // namespace wav_lib
