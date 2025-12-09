#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace wav_lib
{
    using Byte = char;
    using ByteVector = std::vector<Byte>;
    
    class Sample
    {
    public:
        std::vector<ByteVector> channelsData;

        Sample() {};
        Sample(std::vector<ByteVector> channelsData)
            : channelsData(channelsData) {};
    };

    using SampleBuffer = std::vector<Sample>;
    using SampleBufferSPtr = std::unique_ptr<SampleBuffer>;

} // namespace wav_lib
