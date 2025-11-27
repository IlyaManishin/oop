#include "wav_utils.hpp"
#include "../wav_exceptions.hpp"

#include <cinttypes>
#include <fstream>

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

    void write_uint16(std::fstream &f, uint16_t num)
    {
        f.write((const char *)(&num), sizeof(num));
    }

    void write_uint32(std::fstream &f, uint32_t num)
    {
        f.write((const char *)(&num), sizeof(num));
    }

    uint32_t get_uint_file_size(std::fstream &f)
    {
        std::streampos cur = f.tellg();
        f.seekg(0, std::ios::end);
        std::streamsize size = f.tellg();
        f.seekg(cur);
        return (uint32_t)size;
    }

    size_t sec_to_byte_pos(float posSec, uint32_t byteRate, uint32_t blockAlign)
    {
        size_t rawPos =  (size_t)(byteRate * posSec) ;
        size_t bytePos = rawPos - (rawPos % blockAlign);
        return bytePos;
    }

} // namespace wav_lib
