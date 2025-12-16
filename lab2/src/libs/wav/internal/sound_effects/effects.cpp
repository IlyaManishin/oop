#include "../types.hpp"

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace wav_lib
{
    static void bass_effect_impl(ByteVector &buffer,
                                 uint32_t channels,
                                 uint32_t bytesPerSample,
                                 double alpha,
                                 double gain)
    {
        size_t samplesCount = buffer.size() / (bytesPerSample * channels);
        std::vector<int64_t> prev(channels, 0);
        std::vector<int64_t> filtered(channels, 0);

        size_t offset = 0;
        for (size_t i = 0; i < samplesCount; ++i)
        {
            for (uint32_t ch = 0; ch < channels; ++ch)
            {
                int64_t sample = 0;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                    sample |= int64_t(uint8_t(buffer[offset + b])) << (8 * b);

                int64_t signBit = int64_t(1) << (bytesPerSample * 8 - 1);
                if (sample & signBit)
                    sample -= int64_t(1) << (bytesPerSample * 8);

                filtered[ch] = static_cast<int64_t>(prev[ch] + alpha * (sample - prev[ch]));

                int64_t boosted = int64_t(std::clamp(double(filtered[ch]) * gain, double(-signBit), double(signBit - 1)));

                prev[ch] = filtered[ch];

                int64_t tmp = boosted;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                {
                    buffer[offset + b] = tmp & 0xFF;
                    tmp >>= 8;
                }

                offset += bytesPerSample;
            }
        }
    }

    void bass_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        double alpha = 0.2; 
        double gain = 1.5;
        bass_effect_impl(buffer, channels, bytesPerSample, alpha, gain);
    }

    void hach_lada_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        double alpha = 0.05;
        double gain = 2.5;

        bass_effect_impl(buffer, channels, bytesPerSample, alpha, gain);
    }

    void high_boost_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        size_t samplesCount = buffer.size() / (bytesPerSample * channels);
        double gain = 1.8;

        std::vector<int64_t> prev(channels, 0);

        size_t offset = 0;
        for (size_t i = 0; i < samplesCount; ++i)
        {
            for (uint32_t ch = 0; ch < channels; ++ch)
            {
                int64_t sample = 0;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                    sample |= int64_t(uint8_t(buffer[offset + b])) << (8 * b);

                int64_t signBit = int64_t(1) << (bytesPerSample * 8 - 1);
                if (sample & signBit)
                    sample -= int64_t(1) << (bytesPerSample * 8);

                int64_t high = sample - prev[ch];
                int64_t boosted = std::clamp(double(sample + gain * high), double(-signBit), double(signBit - 1));

                prev[ch] = sample;

                int64_t tmp = boosted;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                {
                    buffer[offset + b] = tmp & 0xFF;
                    tmp >>= 8;
                }

                offset += bytesPerSample;
            }
        }
    }

    void distortion_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        size_t samplesCount = buffer.size() / (bytesPerSample * channels);
        size_t offset = 0;

        int64_t maxVal = (1LL << (bytesPerSample * 8 - 1)) - 1;

        double drive = 3.0;
        double clipRatio = 0.7;

        int64_t clip = int64_t(maxVal * clipRatio);

        for (size_t i = 0; i < samplesCount; ++i)
        {
            for (uint32_t ch = 0; ch < channels; ++ch)
            {
                int64_t sample = 0;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                    sample |= int64_t(uint8_t(buffer[offset + b])) << (8 * b);

                int64_t signBit = 1LL << (bytesPerSample * 8 - 1);
                if (sample & signBit)
                    sample -= 1LL << (bytesPerSample * 8);

                double normalized = double(sample) / double(maxVal);
                normalized *= drive;
                normalized = std::tanh(normalized);
                int64_t boosted = int64_t(normalized * clip);

                int64_t tmp = boosted;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                {
                    buffer[offset + b] = tmp & 0xFF;
                    tmp >>= 8;
                }

                offset += bytesPerSample;
            }
        }
    }

} // namespace wav_lib
