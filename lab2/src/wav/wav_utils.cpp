#include <cinttypes>
#include <fstream>

#include "wav_exceptions.hpp"
#include "wav_utils.hpp"

namespace wav_lib
{
    uint16_t read_uint16(std::fstream &f)
    {
        uint16_t value;
        if (!f.read((char *)&value, sizeof(value)))
            throw InvalidWavFileExc("Unexpected EOF while reading wav field");
        return value;
    }

    uint32_t read_uint32(std::fstream &f)
    {
        uint32_t value;
        if (!f.read((char *)&value, sizeof(value)))
            throw InvalidWavFileExc("Unexpected EOF while reading wav field");
        return value;
    }
} // namespace wav_lib
