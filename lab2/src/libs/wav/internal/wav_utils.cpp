#include "wav_utils.hpp"
#include "config.hpp"
#include "types.hpp"
#include "wav_exceptions.hpp"

#include <cinttypes>
#include <fstream>
#include <iostream>

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

    byteVector *read_file_big_vector(std::fstream &file, uint32_t dataLength, std::streampos startPos)
    {
        set_read_pos(file, startPos);

        byteVector *intervalData = new byteVector(dataLength);
        file.read((char *)intervalData->data(), dataLength);
        if (file.gcount() < static_cast<std::streamsize>(dataLength))
        {
            delete intervalData;
            return nullptr;
        }
        return intervalData;
    }

    bool read_file_chunk(std::fstream &file, uint32_t chunkSize, std::streampos startPos,
                              byteVector *dest)
    {
        set_read_pos(file, startPos);
        file.read((char *)dest->data(), chunkSize);

        bool isValid = (file.gcount() == static_cast<std::streamsize>(chunkSize));
        return isValid;
    }

    bool insert_empty_space(std::fstream &file, std::streampos startPos, uint32_t size)
    {
        file.seekg(0, std::ios::end);
        std::streampos fileEnd = file.tellg();
        if (startPos > fileEnd)
            return false;

        byteVector buffer(config::FILE_BUFFER_SIZE, 0);

        std::streampos readPos = fileEnd;
        std::streampos writePos = fileEnd + static_cast<std::streampos>(size);

        while (readPos > startPos)
        {
            size_t chunk = static_cast<size_t>(std::min<std::streampos>(readPos - startPos, config::FILE_BUFFER_SIZE));
            readPos -= chunk;
            writePos -= chunk;

            file.seekg(readPos);
            file.read((char *)buffer.data(), chunk);

            file.seekp(writePos);
            file.write((char *)buffer.data(), chunk);
        }

        file.seekp(startPos);
        byteVector emptyBlock(size, 0);
        file.write((char *)emptyBlock.data(), size);

        file.flush();
        return true;
    }

    bool extend_file_with_zeros(std::fstream &file, uint32_t size)
    {
        if (!file.is_open())
            return false;

        file.seekp(0, std::ios::end);
        byteVector buffer(config::FILE_BUFFER_SIZE, 0);
        uint32_t remaining = size;

        while (remaining > 0)
        {
            size_t chunk = std::min<size_t>(remaining, config::FILE_BUFFER_SIZE);
            file.write((char *)buffer.data(), chunk);
            remaining -= chunk;
        }

        file.flush();
        return true;
    }

    bool write_big_vector_to_file(std::fstream &file, std::streampos pos, byteVector *data, size_t size)
    {
        if (!file.is_open())
            return false;

        file.seekp(pos);
        size_t written = 0;
        size_t bufferSize = config::FILE_BUFFER_SIZE;

        while (written < size)
        {
            size_t chunk = std::min(size - written, bufferSize);
            file.write((char *)(data->data() + written), chunk);
            if (!file)
                return false;
            written += chunk;
        }

        file.flush();
        return true;
    }

    bool write_vector_to_file(std::fstream &file, std::streampos pos,
                              const byteVector *data, size_t dataLength)
    {
        if (!set_write_pos(file, pos))
            return false;

        file.write((char *)(data->data()), dataLength);
        file.flush();
        return !file.bad();
    }

} // namespace wav_lib
