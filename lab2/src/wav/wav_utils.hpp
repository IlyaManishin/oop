#pragma once

#include <cinttypes>
#include <fstream>

namespace wav_lib
{
    uint16_t read_uint16(std::ifstream &f);
    uint32_t read_uint32(std::ifstream &f);
} // namespace wav_lib
