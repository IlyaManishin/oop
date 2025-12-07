#pragma once

#include "types.hpp"

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

    bool set_write_pos(std::fstream &file, std::streampos pos);
    bool set_write_pos_off(std::fstream &file, std::streampos dataStart, uint32_t byteOffset);
    bool set_read_pos(std::fstream &file, std::streampos pos);
    bool set_read_pos_off(std::fstream &file, std::streampos dataStart, uint32_t byteOffset);

    byteVector *read_vector_from_file(std::fstream &file, uint32_t dataLength, std::streampos startPos);
    bool insert_empty_space(std::fstream &file, std::streampos startPos, uint32_t size);
    bool extend_file_with_zeros(std::fstream &file, uint32_t size);
    bool write_big_vector_to_file(std::fstream &file, std::streampos pos, byteVector *data);
    bool write_vector_to_file(std::fstream &file, std::streampos pos,
                              const byteVector *data, size_t dataLength);

    uint32_t sec_to_byte_pos(float posSec, uint32_t byteRate, uint32_t blockAlign);
    inline std::streampos get_offset_pos(std::streampos pos, uint32_t offset)
    {
        return pos + static_cast<std::streampos>(offset);
    }
} // namespace wav_lib
