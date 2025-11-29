#include "samples.hpp"
#include "wav_exceptions.hpp"

namespace wav_lib
{
    static Sample get_oef_sample()
    {
        Sample sample;
        sample.isEOF = true;
        return sample;
    }
    
    Sample SampleReader::ReadSample()
    {
        if (this->curSampleCount == this->samplesCount)
        {
            return get_oef_sample();
        }
        std::vector<byteVector> data(this->channelsCount, byteVector(this->bytesPerChannel));
        for (size_t i = 0; i < this->channelsCount; i++)
        {
            byte *channelData = data[i].data();
            this->srcFile.read(channelData, this->bytesPerChannel);
            if (this->srcFile.bad())
                throw InvalidWavFileExc(this->path);
        }
        Sample res(data);

        this->curSampleCount++;
        return res;
    }

} // namespace wav_lib
