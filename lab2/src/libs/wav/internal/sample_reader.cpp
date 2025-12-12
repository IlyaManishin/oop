#include "sample_reader.hpp"
#include "config.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include "cassert"
#include "cstring"

namespace wav_lib
{

    // bool FileSReader::ReadSample(Sample &dest)
    // {
    //     if (this->curSampleCount == this->samplesCount)
    //     {
    //         dest.channelsData.clear();
    //         return false;
    //     }
    //     std::vector<byteVector> data(this->channelsCount, byteVector(this->bytesPerChannel));
    //     for (size_t i = 0; i < this->channelsCount; i++)
    //     {
    //         byte *channelData = data[i].data();
    //         this->srcFile.read(channelData, this->bytesPerChannel);
    //         if (this->srcFile.bad())
    //         {
    //             dest.isError = true;
    //             return false;
    //         }
    //     }
    //     dest.channelsData = data;

    //     this->curSampleCount++;
    //     return true;
    // }

    ISampleReader::ISampleReader(const SampleReaderConfig &cfg)
        : isDiffSampleRate(cfg.input.sampleRate != cfg.output.sampleRate),
          sampleStep(double(cfg.input.sampleRate) / double(cfg.output.sampleRate)),
          curSampleAccum(0.0),
          isSampleChanges(
              cfg.input.channelsCount != cfg.output.channelsCount ||
              cfg.input.bytesPerSample != cfg.output.bytesPerSample),
          maxSamples((uint32_t)cfg.maxSamples),
          allSamplesCount(0)
    {
        params.srcBlockAlign = cfg.input.blockAlign;
        params.destBlockAlign = cfg.output.blockAlign;

        params.srcDepth = cfg.input.bytesPerSample;
        params.destDepth = cfg.output.bytesPerSample;

        params.srcChannels = cfg.input.channelsCount;
        params.destChannels = cfg.output.channelsCount;

        params.effect = cfg.effect;
        params.isNewVolume = cfg.isNewVolume;
        params.volumeValue = cfg.volumeValue;
    }

    size_t ISampleReader::ReadToSampleBuffer(ByteVector &dest)
    {
        if (allSamplesCount >= maxSamples)
            return 0;

        size_t remainSamples = maxSamples - allSamplesCount;
        size_t chunkSamples = std::min(remainSamples, config::SAMPLE_BUFFER_SIZE);
        size_t needBytes = chunkSamples * params.destBlockAlign;

        if (dest.size() < needBytes)
            dest.resize(needBytes);

        size_t readSamples = this->extractSampleBuffer(dest, chunkSamples);
        allSamplesCount += readSamples;

        this->addBufferEffects(dest);
        return readSamples;
    }

    void ISampleReader::addBufferEffects(ByteVector &buffer)
    {
    }

    FileSReader::FileSReader(std::fstream &srcFile,
                             std::streampos startPos,
                             const SampleReaderConfig &cfg)
        : ISampleReader(cfg),
          srcFile(srcFile),
          startPos(startPos)
    {
        this->srcFile.clear();
        this->srcFile.seekg(this->startPos);
    }

    bool FileSReader::IsBad() const
    {
        return this->srcFile.bad() | this->isError;
    }

    size_t FileSReader::extractSampleBuffer(ByteVector &dest, size_t maxSamples)
    {
        if (isSampleChanges)
        {
            isError = true;
            return 0;
        }

        size_t toReadBytes = maxSamples * params.srcBlockAlign;
        srcFile.read(dest.data(), toReadBytes);
        size_t byteCount = srcFile.gcount();

        return byteCount / params.srcBlockAlign;
    }

    VectorSReader::VectorSReader(const ByteVector &buffer,
                                 size_t offsetBytes,
                                 const SampleReaderConfig &cfg)
        : ISampleReader(cfg),
          data(buffer),
          offsetBytes(offsetBytes),
          curSampleCount(0)
    {
    }

    bool VectorSReader::IsBad() const
    {
        return this->isError;
    }

    size_t VectorSReader::extractSampleBuffer(ByteVector &dest, size_t maxSamples)
    {
        if (isSampleChanges)
        {
            isError = true;
            return 0;
        }

        size_t remainLocal = data.size() > offsetBytes ? data.size() - offsetBytes : 0;
        size_t samplesCount = std::min(maxSamples, remainLocal / params.srcBlockAlign);
        if (samplesCount == 0)
            return 0;

        size_t takeBytes = samplesCount * params.srcBlockAlign;
        if (dest.size() < takeBytes)
            dest.resize(takeBytes);

        std::memcpy(dest.data(), data.data() + offsetBytes, takeBytes);
        offsetBytes += takeBytes;

        return samplesCount;
    }

    // Sample ISampleReader::normalizeSample(const Sample &src)
    // {
    //     assert(this->isSampleChanges);

    //     const uint32_t srcCh = params.srcChannels;
    //     const uint32_t dstCh = params.destChannels;
    //     const uint32_t srcDepth = params.srcDepth;
    //     const uint32_t dstDepth = params.destDepth;

    //     Sample out(dstCh);

    //     int diff = int(dstDepth) - int(srcDepth);
    //     int shift = diff > 0 ? diff : -diff;
    //     bool upShift = diff > 0;

    //     int64_t dstMax = (1LL << (dstDepth - 1)) - 1;
    //     int64_t dstMin = -(1LL << (dstDepth - 1));

    //     for (uint32_t i = 0; i < dstCh; i++)
    //     {
    //         uint32_t srcIdx = i < srcCh ? i : (srcCh ? (i % srcCh) : 0);

    //         auto &srcVec = src.channels[srcIdx];
    //         int64_t v = 0;

    //         for (uint32_t b = 0; b < srcDepth; b++)
    //             v = (v << 8) | uint8_t(srcVec[b]);

    //         if (srcVec.back() & 0x80)
    //             for (uint32_t b = srcDepth; b < 8; b++)
    //                 v |= (0xFFLL << (b * 8));

    //         uint64_t absv = v < 0 ? uint64_t(-v) : uint64_t(v);

    //         if (upShift)
    //             absv <<= shift;
    //         else if (shift)
    //             absv = (absv + (1ULL << (shift - 1))) >> shift;

    //         int64_t vv = (v < 0) ? -int64_t(absv) : int64_t(absv);

    //         if (vv > dstMax)
    //             vv = dstMax;
    //         if (vv < dstMin)
    //             vv = dstMin;

    //         out.channels[i].resize(dstDepth);
    //         for (int b = dstDepth - 1; b >= 0; b--)
    //         {
    //             out.channels[i][b] = Byte(vv & 0xFF);
    //             vv >>= 8;
    //         }
    //     }

    //     return out;
    // }

} // namespace wav_lib
