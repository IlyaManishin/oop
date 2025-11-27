#pragma once

#include <cinttypes>
#include <fstream>

namespace wav_lib
{
    uint16_t read_uint16(std::fstream &f);
    uint32_t read_uint32(std::fstream &f);
    void write_uint16(std::fstream &f, uint16_t v);
    void write_uint32(std::fstream &f, uint32_t v);
    uint32_t get_uint_file_size(std::fstream &f);

    static inline uint32_t get_chunk_size(std::fstream &wavFile)
    {
        return get_uint_file_size(wavFile) - 4;
    };

    static inline uint16_t calc_block_align(uint16_t channels, uint16_t bitsPerSample)
    {
        return channels * (bitsPerSample / 8);
    }

    static inline uint32_t calc_byte_rate(uint32_t sampleRate, uint16_t blockAlign)
    {
        return sampleRate * blockAlign;
    }

    size_t sec_to_byte_pos(float posSec, uint32_t byteRate, uint32_t blockAlign);

} // namespace wav_lib
