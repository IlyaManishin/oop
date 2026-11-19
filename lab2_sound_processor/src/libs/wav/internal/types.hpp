#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace wav_lib
{
    struct TWavSoundParams
    {
        uint32_t channelsCount;
        uint32_t bytesPerSample;
        uint32_t blockAlign;
        uint32_t sampleRate;
    };

    using Byte = char;
    using ByteVector = std::vector<Byte>;
} // namespace wav_lib
