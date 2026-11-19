#include "../types.hpp"
#include <cstdint>

namespace wav_lib
{
    void bass_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample);
    void high_boost_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample);
    void distortion_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample);
    void hach_lada_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample);
} // namespace wav_lib
