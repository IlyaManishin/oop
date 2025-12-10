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
        ByteVector channels;

        Sample(uint32_t size): channels(size){};
        Sample(ByteVector channelsData)
            : channels(channelsData) {};
    };

    using SampleBuffer = std::vector<Sample>;
    using SampleBufferSPtr = std::unique_ptr<SampleBuffer>;

} // namespace wav_lib
