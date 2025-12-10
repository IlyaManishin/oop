#include "sample_reader.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include "cassert"

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
          samplesCount(0)
    {
        changes.srcBlockAlign = cfg.input.blockAlign;
        changes.destBlockAlign = cfg.output.blockAlign;

        changes.srcDepth = cfg.input.bytesPerSample;
        changes.destDepth = cfg.output.bytesPerSample;

        changes.srcChannels = cfg.input.channelsCount;
        changes.destChannels = cfg.output.channelsCount;

        changes.effect = cfg.effect;
        changes.isNewVolume = cfg.isNewVolume;
        changes.volumeValue = cfg.volumeValue;
    }

    Sample ISampleReader::normalizeSample(const Sample &src)
    {
        assert(this->isSampleChanges);

        const uint32_t srcCh = changes.srcChannels;
        const uint32_t dstCh = changes.destChannels;
        const uint32_t srcDepth = changes.srcDepth;
        const uint32_t dstDepth = changes.destDepth;

        Sample out(dstCh);

        int diff = static_cast<int>(dstDepth) - static_cast<int>(srcDepth);
        int shift = diff > 0 ? diff : -diff;
        bool upShift = diff > 0;

        int64_t dstMax = (1LL << (dstDepth - 1)) - 1;
        int64_t dstMin = -(1LL << (dstDepth - 1));

        for (uint32_t i = 0; i < dstCh; i++)
        {
            uint32_t srcIdx = i < srcCh ? i : (srcCh ? (i % srcCh) : 0);
            int64_t v = src.channels[srcIdx];

            int64_t sign = v < 0 ? -1 : 1;
            uint64_t absv = static_cast<uint64_t>(v < 0 ? -v : v);

            if (upShift)
                absv = absv << shift;
            else
            {
                if (shift > 0)
                    absv = (absv + (1ULL << (shift - 1))) >> shift;
            }

            int64_t vv = static_cast<int64_t>(absv) * sign;

            if (vv > dstMax)
                vv = dstMax;
            if (vv < dstMin)
                vv = dstMin;

            out.channels[i] = static_cast<int32_t>(vv);
        }
        return out;
    }

    size_t VectorSReader::ReadSampleBuffer(SampleBufferSPtr outBuffer)
    {
    }

} // namespace wav_lib
