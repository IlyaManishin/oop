#include "internal/wav_utils.hpp"
#include "wav.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

const int MAX_CHAIN_COUNT = 12;

namespace wav_lib
{
    static InvalidWavFileExc get_wav_not_init_error(const std::string wavPath)
    {
        return InvalidWavFileExc("Wav file not init", wavPath);
    }

    WavFile::WavFile(const std::string &wavPath, bool isNew)
        : file(wavPath, std::ios::in | std::ios::out | std::ios::binary)
    {
        this->path = wavPath;
        this->isChanged = false;
        this->isInit = false;

        if (!this->file.is_open())
            throw InvalidWavFileExc("Cannot open file: " + wavPath);
        if (!isNew)
        {
            this->extractFileData();
            this->isInit = true;
        }
    }

    void WavFile::extractFileData()
    {
        char chunkId[4];
        char format[4];
        char subchunk1Id[4];

        if (!this->file.read(chunkId, 4))
            throw InvalidWavFileExc("Failed to read chunkId");
        this->header.chunkSize = read_uint32(this->file);
        if (!this->file.read(format, 4))
            throw InvalidWavFileExc("Failed to read format");

        if (std::strncmp(chunkId, "RIFF", 4) != 0 || std::strncmp(format, "WAVE", 4) != 0)
            throw InvalidWavFileExc("Not a RIFF/WAVE file");

        if (!this->file.read(subchunk1Id, 4))
            throw InvalidWavFileExc("Failed to read subchunk1Id");
        if (std::strncmp(subchunk1Id, "fmt ", 4) != 0)
            throw InvalidWavFileExc("Invalid subchunk1Id identifier");

        uint32_t subchunk1Size = read_uint32(this->file);

        this->header.audioFormat = read_uint16(this->file);
        this->header.numChannels = read_uint16(this->file);
        this->header.sampleRate = read_uint32(this->file);
        this->header.byteRate = read_uint32(this->file);
        this->header.blockAlign = read_uint16(this->file);
        this->header.bitsPerSample = read_uint16(this->file);

        char subchunk2Id[4];
        for (int i = 0; i < MAX_CHAIN_COUNT; i++)
        {
            if (!this->file.read(subchunk2Id, 4))
                throw InvalidWavFileExc("Failed to read subchunk2Id");
            this->header.subchunk2Size = read_uint32(this->file);

            if (std::strncmp(subchunk2Id, "LIST", 4) == 0)
            {
                std::streampos cur = this->file.tellg();
                this->file.seekg(this->header.subchunk2Size, std::ios::cur);
            }
            else if (std::strncmp(subchunk2Id, "data", 4) == 0)
            {
                this->dataStart = this->file.tellg();
                this->file.seekg(this->header.subchunk2Size, std::ios::cur);
                this->dataEnd = this->file.tellg();
                break;
            }
            else
                throw InvalidWavFileExc("Invalid subchunk identifiers");
        }
    }

    void WavFile::PrintInfo()
    {
        if (!this->isInit)
            throw get_wav_not_init_error(this->path);

        std::cout << "File: " << path << "\n";
        std::cout << "Channels: " << this->header.numChannels << "\n";
        std::cout << "Sample rate: " << this->header.sampleRate << " Hz\n";
        std::cout << "Bits per sample: " << this->header.bitsPerSample << "\n";
        std::cout << "Byte rate: " << this->header.byteRate << "\n";
        std::cout << "Data size: " << this->dataEnd - this->dataStart << " bytes\n";
    }

    void WavFile::_save_header()
    {
        this->file.seekp(0, std::ios::beg);

        this->file.write("RIFF", 4);
        int newChunkSize = get_uint_file_size(this->file) - 4;
        write_uint32(this->file, newChunkSize);
        this->file.write("WAVE", 4);

        this->file.write("fmt ", 4);
        write_uint32(this->file, 16);
        write_uint16(this->file, this->header.audioFormat);
        write_uint16(this->file, this->header.numChannels);
        write_uint32(this->file, this->header.sampleRate);
        write_uint32(this->file, this->header.byteRate);
        write_uint16(this->file, this->header.blockAlign); //???
        write_uint16(this->file, this->header.bitsPerSample);

        this->file.write("data", 4);
        write_uint32(this->file, this->header.subchunk2Size);

        this->file.flush();
    }

    void WavFile::Close()
    {
        this->_save_header();
    }

    WavFile::~WavFile()
    {
        if (this->file.is_open())
            this->file.close();
    }

} // namespace wav_lib
