#include "../types.hpp"

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace wav_lib
{
    void bass_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        size_t samplesCount = buffer.size() / (bytesPerSample * channels);
        std::vector<int64_t> prev(channels, 0);
        std::vector<int64_t> filtered(channels, 0);

        double alpha = 0.1;
        double gain = 1.5;

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


    void ultra_bass_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        size_t samplesCount = buffer.size() / (bytesPerSample * channels);
        std::vector<int64_t> prev(channels, 0);
        std::vector<int64_t> filtered(channels, 0);

        double alpha = 0.05;
        double gain = 2.5;

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

        int64_t clip = (1LL << (bytesPerSample * 8 - 1)) - 1;

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

                sample = std::clamp(sample * 2, -clip, clip);

                int64_t tmp = sample;
                for (uint32_t b = 0; b < bytesPerSample; ++b)
                {
                    buffer[offset + b] = tmp & 0xFF;
                    tmp >>= 8;
                }

                offset += bytesPerSample;
            }
        }
    }
    
    void hach_lada_effect(ByteVector &buffer, uint32_t channels, uint32_t bytesPerSample)
    {
        size_t samplesCount = buffer.size() / (channels * bytesPerSample);

        std::mt19937 rng(42);
        std::uniform_int_distribution<int64_t> noiseDist(-1, 1);

        std::vector<int64_t> prevLow(channels, 0);
        std::vector<int64_t> prevHigh(channels, 0);

        double alphaLow = 0.05;
        double alphaHigh = 0.2;

        size_t offset = 0;
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

                int64_t low = static_cast<int64_t>(prevLow[ch] + alphaLow * (sample - prevLow[ch]));
                int64_t high = sample - prevHigh[ch];
                prevLow[ch] = low;
                prevHigh[ch] = static_cast<int64_t>(prevHigh[ch] + alphaHigh * high);

                int64_t processed = low + high;

                processed += noiseDist(rng);

                int64_t qbits = bytesPerSample * 8;
                int64_t shift = qbits > 12 ? qbits - 12 : 0;
                if (shift)
                    processed = (processed >> shift) << shift;

                processed = std::clamp(processed, -signBit, signBit - 1);

                int64_t tmp = processed;
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
