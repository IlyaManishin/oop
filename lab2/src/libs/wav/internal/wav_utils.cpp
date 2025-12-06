#include "wav_utils.hpp"
#include "config.hpp"
#include "wav_exceptions.hpp"

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
        
        f.clear();
        return (uint32_t)size;
    }

    uint32_t sec_to_byte_pos(float posSec, uint32_t byteRate, uint32_t blockAlign)
    {
        size_t rawPos = byteRate * posSec;
        if (rawPos >= config::MAX_WAV_DATA_SIZE)
        {
            return config::MAX_WAV_DATA_SIZE;
        }
        uint32_t bytePos = rawPos - (rawPos % blockAlign);
        return bytePos;
    }

    bool set_write_pos(std::fstream &file, std::streampos pos)
    {
        if (!file.is_open())
            return false;

        file.clear();
        file.seekp(pos);
        return !file.fail();
    }

    bool set_write_pos_off(std::fstream &file, std::streampos dataStart, uint32_t byteOffset)
    {
        if (!file.is_open())
            return false;

        file.clear();
        file.seekp(dataStart + std::streamoff(byteOffset));
        return !file.fail();
    }

    bool set_read_pos(std::fstream &file, std::streampos pos)
    {
        if (!file.is_open())
            return false;

        file.clear();
        file.seekg(pos);
        return !file.fail();
    }

    bool set_read_pos_off(std::fstream &file, std::streampos dataStart, uint32_t byteOffset)
    {
        if (!file.is_open())
            return false;

        file.clear();
        file.seekg(dataStart + std::streamoff(byteOffset));
        return !file.fail();
    }

} // namespace wav_lib
