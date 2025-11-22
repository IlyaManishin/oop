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
    
} // namespace wav_lib
