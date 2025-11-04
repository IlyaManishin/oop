#include "wav.hpp"
#include "wav_exceptions.hpp"
#include "wav_utils.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace wav_lib
{

    WavFile::WavFile(const std::string &wavPath)
    {
        this->path = wavPath;
        std::ifstream f(wavPath, std::ios::binary);
        if (!f.is_open())
            throw InvalidWavFile("Cannot open file: " + wavPath);

        char chunkId[4];
        char format[4];
        char subchunk1Id[4];
        char subchunk2Id[4];

        if (!f.read(chunkId, 4))
            throw InvalidWavFile("Failed to read chunkId");
        this->chunkSize = read_uint32(f);
        if (!f.read(format, 4))
            throw InvalidWavFile("Failed to read format");

        if (std::strncmp(chunkId, "RIFF", 4) != 0 || std::strncmp(format, "WAVE", 4) != 0)
            throw InvalidWavFile("Not a RIFF/WAVE file");

        if (!f.read(subchunk1Id, 4))
            throw InvalidWavFile("Failed to read subchunk1Id");

        uint32_t subchunk1Size = read_uint32(f);

        this->audioFormat = read_uint16(f);
        this->numChannels = read_uint16(f);
        this->sampleRate = read_uint32(f);
        this->byteRate = read_uint32(f);
        this->blockAlign = read_uint16(f);
        this->bitsPerSample = read_uint16(f);

        if (!f.read(subchunk2Id, 4))
            throw InvalidWavFile("Failed to read subchunk2Id");
        subchunk2Size = read_uint32(f);

        if (std::strncmp(subchunk1Id, "fmt ", 4) != 0 || std::strncmp(subchunk2Id, "data", 4) != 0)
            throw InvalidWavFile("Invalid subchunk identifiers");

        dataStart = f.tellg();
        f.seekg(0, std::ios::end);
        dataEnd = f.tellg();
    }

    void WavFile::PrintInfo()
    {
        std::cout << "File: " << path << "\n";
        std::cout << "Channels: " << this->numChannels << "\n";
        std::cout << "Sample rate: " << this->sampleRate << " Hz\n";
        std::cout << "Bits per sample: " << this->bitsPerSample << "\n";
        std::cout << "Byte rate: " << this->byteRate << "\n";
        std::cout << "Data size: " << this->dataEnd << " bytes\n";
    }

    WavFile::~WavFile()
    {
        if (this->file.is_open())
            this->file.close();
    };

} // namespace wav
