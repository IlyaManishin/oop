#pragma once

#include <cinttypes>
#include <fstream>

namespace wav_lib
{
    uint16_t read_uint16(std::fstream &f);
    uint32_t read_uint32(std::fstream &f);
} // namespace wav_lib
