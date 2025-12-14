#include "sample_reader.hpp"
#include "../wav.hpp"
#include "config.hpp"
#include "sound_effects/effects.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include "cassert"
#include "cstring"
#include "unordered_map"

namespace wav_lib
{
    using EffectFunc = void (*)(ByteVector &, uint32_t, uint32_t);

    static const std::unordered_map<int, EffectFunc> effectMap = {
        {(int)WavEffects::BASS, (EffectFunc)bass_effect},
        {(int)WavEffects::ULTRA_BASS, (EffectFunc)ultra_bass_effect},
        {(int)WavEffects::RAISE_HIGH, (EffectFunc)high_boost_effect},
        {(int)WavEffects::DISTORTION, (EffectFunc)distortion_effect},
        {(int)WavEffects::HACH_LADA, (EffectFunc)hach_lada_effect}};

    ISampleReader::ISampleReader(const SampleReaderConfig &cfg)
        : isDiffSampleRate(cfg.input.sampleRate != cfg.output.sampleRate),
          isSoundChanges(
              cfg.input.channelsCount != cfg.output.channelsCount ||
              cfg.input.bytesPerSample != cfg.output.bytesPerSample),
          maxSamples((uint32_t)cfg.maxSamples),
          bufChunkCount(cfg.bufferSize),
          sampleStep(double(cfg.input.sampleRate) / double(cfg.output.sampleRate))
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

        size_t byteSize = cfg.bufferSize * this->params.destBlockAlign;
        this->buffer = new ByteVector(byteSize);
    }

    size_t ISampleReader::ReadToSampleBuffer()
    {
        if (allSamplesCount >= maxSamples)
            return 0;

        size_t remainSamples = maxSamples - allSamplesCount;
        size_t takeSamples = std::min(remainSamples, this->bufChunkCount);
        size_t takeBytes = takeSamples * this->params.destBlockAlign;

        if (this->buffer->size() != takeBytes)
            this->buffer->resize(takeBytes);

        size_t readSamples = this->readDataToBuf(takeSamples);
        if (this->buffer->size() != readSamples * this->params.destBlockAlign)
            this->buffer->resize(readSamples * this->params.destBlockAlign);
        allSamplesCount += readSamples;

        this->addBufferEffects();
        return readSamples;
    }

    ISampleReader::~ISampleReader()
    {
        if (this->buffer)
        {
            delete this->buffer;
            this->buffer = nullptr;
        }
    }

    void ISampleReader::addBufferEffects()
    {
        if (params.effect == WavEffects::NORMAL)
            return;
        if (!effectMap.contains((int)params.effect))
            return;

        auto it = effectMap.find((int)this->params.effect);
        it->second(*this->buffer, params.destChannels, params.destDepth);
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
        return this->srcFile.bad() || this->isError;
    }

    size_t FileSReader::readDataToBuf(size_t maxSamples)
    {
        if (isSoundChanges)
        {
            isError = true;
            return 0;
        }

        size_t size = maxSamples * this->params.srcBlockAlign;
        srcFile.read(this->buffer->data(), size);
        size_t byteCount = srcFile.gcount();

        return byteCount / this->params.srcBlockAlign;
    }

    VectorSReader::VectorSReader(const ByteVector &wavData,
                                 size_t offsetBytes,
                                 const SampleReaderConfig &cfg)
        : ISampleReader(cfg),
          wavData(wavData),
          offsetBytes(offsetBytes),
          curSampleCount(0) {}

    bool VectorSReader::IsBad() const
    {
        return this->isError;
    }

    size_t VectorSReader::readDataToBuf(size_t maxSamples)
    {
        if (isSoundChanges)
        {
            isError = true;
            return 0;
        }

        size_t remainLocal = wavData.size() > offsetBytes ? wavData.size() - offsetBytes : 0;
        size_t samplesCount = std::min(maxSamples, remainLocal / params.srcBlockAlign);
        if (samplesCount == 0)
            return 0;

        size_t byteSize = samplesCount * this->params.srcBlockAlign;
        std::memcpy(this->buffer->data(), this->wavData.data() + offsetBytes, byteSize); // delete memcpy, only ptrs
        this->offsetBytes += byteSize;

        return samplesCount;
    }

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
