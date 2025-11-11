#include "wav.hpp"
#include "wav_exceptions.hpp"
#include "internal/wav_utils.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

const int MAX_CHAIN_COUNT = 12;

namespace wav_lib
{
    WavFile *WavReader::ReadWav(const std::string &path) const
    {
        WavFile *wavFile = new WavFile(path);
        return wavFile;
    }

    bool WavReader::_isWavFile(const std::string &path) const
    {
        try
        {
            WavFile *file = this->ReadWav(path);
            delete file;
            return true;
        }
        catch (const WavException &e)
        {
            return false;
        }
    }

    bool WavReader::IsExistsWav(const std::string &path) const
    {
        std::fstream wavStream(path);
        if (!wavStream.is_open())
            return false;

        wavStream.close();

        return this->_isWavFile(path);
    }

    WavFile::WavFile(const std::string &wavPath) : file(wavPath, std::ios::in | std::ios::out | std::ios::binary)
    {
        this->path = wavPath;
        if (!this->file.is_open())
            throw InvalidWavFileExc("Cannot open file: " + wavPath);

        this->_extract_file_data();
    }

    void WavFile::_extract_file_data()
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
        std::cout << "File: " << path << "\n";
        std::cout << "Channels: " << this->header.numChannels << "\n";
        std::cout << "Sample rate: " << this->header.sampleRate << " Hz\n";
        std::cout << "Bits per sample: " << this->header.bitsPerSample << "\n";
        std::cout << "Byte rate: " << this->header.byteRate << "\n";
        std::cout << "Data size: " << this->dataEnd - this->dataStart << " bytes\n";
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
