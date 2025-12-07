#include "samples.hpp"
#include "wav_exceptions.hpp"

namespace wav_lib
{
    Sample::Sample(uint32_t channels, uint32_t bitsPerSample)
    {
        this->channelsData.resize(channels);
        uint32_t bytesPerSample = bitsPerSample / 8;
        for (auto &ch : this->channelsData)
            ch.resize(bytesPerSample, 0);
    }

    bool SampleReader::ReadSample(Sample &dest)
    {
        if (this->curSampleCount == this->samplesCount)
        {
            dest.channelsData.clear();
            return false;
        }
        std::vector<byteVector> data(this->channelsCount, byteVector(this->bytesPerChannel));
        for (size_t i = 0; i < this->channelsCount; i++)
        {
            byte *channelData = data[i].data();
            this->srcFile.read(channelData, this->bytesPerChannel);
            if (this->srcFile.bad())
            {
                dest.isError = true;
                return false;
            }
        }
        dest.channelsData = data;

        this->curSampleCount++;
        return true;
    }

} // namespace wav_lib
