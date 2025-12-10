#include "sample_reader.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

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
        : isSameSampleRate(cfg.input.sampleRate == cfg.output.sampleRate),
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

    Sample ISampleReader::normalizeSample(Sample sample)
    {
    }

    size_t VectorSReader::ReadSampleBuffer(SampleBufferSPtr outBuffer)
    {
    }

} // namespace wav_lib
