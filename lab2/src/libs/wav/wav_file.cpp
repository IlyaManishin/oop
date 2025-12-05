#include "internal/config.hpp"
#include "internal/samples.hpp"
#include "internal/wav_utils.hpp"
#include "wav.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int MAX_CHAIN_COUNT = 12;
const int EMPTY_WAV_CHUNK_SIZE = 36;

namespace wav_lib
{
    class WavInterval
    {
    public:
        WavFile *wavFile;
        uint32_t startPos;
        uint32_t samplesCount;

        WavInterval(WavFile *wavFile, uint32_t startPos, uint32_t samplesCount)
            : wavFile(wavFile), startPos(startPos), samplesCount(samplesCount) {};
        ~WavInterval() {};
    };

    // static InvalidWavFileExc get_wav_not_init_error(const std::string wavPath)
    // {
    //     return InvalidWavFileExc("Wav file not init", wavPath);
    // }

    WavFile::WavFile(const std::string &wavPath)
        : file(wavPath, std::ios::in | std::ios::out | std::ios::binary)
    {
        this->path = wavPath;
        this->isChanged = false;

        if (!this->file.is_open())
            throw InvalidWavFileExc("Cannot open file", this->path);
    }

    WavFileSPtr WavFile::Open(const std::string &path)
    {
        WavFileSPtr file(new WavFile(path));
        file->extractFileData();
        return file;
    }

    WavFileSPtr WavFile::Create(const std::string &path, uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample)
    {
        WavFileSPtr file(new WavFile(path));
        file->initNewHeader(channels, sampleRate, bitsPerSample);
        file->Save();
        return file;
    }

    void WavFile::extractFileData()
    {
        char chunkId[4];
        char format[4];
        char subchunk1Id[4];

        if (!this->file.read(chunkId, 4))
            throw InvalidWavFileExc("Failed to read chunkId", this->path);
        this->header.chunkSize = read_uint32(this->file);
        if (!this->file.read(format, 4))
            throw InvalidWavFileExc("Failed to read format", this->path);

        if (std::strncmp(chunkId, "RIFF", 4) != 0 || std::strncmp(format, "WAVE", 4) != 0)
            throw InvalidWavFileExc("Not a RIFF/WAVE file", this->path);

        if (!this->file.read(subchunk1Id, 4))
            throw InvalidWavFileExc("Failed to read subchunk1Id", this->path);
        if (std::strncmp(subchunk1Id, "fmt ", 4) != 0)
            throw InvalidWavFileExc("Invalid subchunk1Id identifier", this->path);

        read_uint32(this->file); // subchunk1Size

        this->header.audioFormat = read_uint16(this->file);
        this->header.numChannels = read_uint16(this->file);
        this->header.sampleRate = read_uint32(this->file);
        this->header.byteRate = read_uint32(this->file);
        this->header.blockAlign = read_uint16(this->file);
        this->header.bitsPerSample = read_uint16(this->file);

        char subchunk2Id[4];
        uint32_t subchunk2Size;

        for (int i = 0; i < MAX_CHAIN_COUNT; i++)
        {
            if (!this->file.read(subchunk2Id, 4))
                throw InvalidWavFileExc("Failed to read subchunk2Id", this->path);
            subchunk2Size = read_uint32(this->file);

            if (std::strncmp(subchunk2Id, "LIST", 4) == 0) //
            {
                std::streampos cur = this->file.tellg();
                this->file.seekg(subchunk2Size, std::ios::cur);
            }
            else if (std::strncmp(subchunk2Id, "data", 4) == 0)
            {
                this->header.subchunk2Size = subchunk2Size;

                this->dataStart = this->file.tellg();
                this->file.seekg(this->header.subchunk2Size, std::ios::cur);
                this->dataEnd = this->file.tellg();
                break;
            }
            else
                throw InvalidWavFileExc("Invalid subchunk identifiers", this->path);
        }
    }
    void WavFile::initNewHeader(uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample)
    {
        this->header.audioFormat = 1;
        this->header.numChannels = channels;
        this->header.sampleRate = sampleRate;
        this->header.bitsPerSample = bitsPerSample;

        header.blockAlign = calc_block_align(channels, bitsPerSample);
        header.byteRate = calc_byte_rate(sampleRate, header.blockAlign);

        this->header.subchunk2Size = 0;
        this->header.chunkSize = EMPTY_WAV_CHUNK_SIZE;

        this->isChanged = true;
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

    void WavFile::saveHeader()
    {
        this->file.seekp(0, std::ios::beg);

        this->file.write("RIFF", 4);
        int newChunkSize = get_chunk_size(this->file);
        write_uint32(this->file, newChunkSize);
        this->file.write("WAVE", 4);

        this->file.write("fmt ", 4);
        write_uint32(this->file, 16);
        write_uint16(this->file, this->header.audioFormat);
        write_uint16(this->file, this->header.numChannels);
        write_uint32(this->file, this->header.sampleRate);
        write_uint32(this->file, this->header.byteRate);
        write_uint16(this->file, this->header.blockAlign);
        write_uint16(this->file, this->header.bitsPerSample);

        this->file.write("data", 4);
        write_uint32(this->file, this->header.subchunk2Size);

        this->file.flush();
    }
    void WavFile::updateSubchunkSize()
    {
        this->header.subchunk2Size = static_cast<uint32_t>(this->dataEnd - this->dataStart);
        this->isChanged = true;
    }

    void WavFile::Save()
    {
        if (!this->isChanged)
            return;

        this->saveHeader();
        this->isChanged = false;
    }

    TWavHeader WavFile::GetHeader()
    {
        return this->header;
    }

    WavInterval WavFile::GetInterval(float startSec, float endSec)
    {
        if (startSec < 0 || endSec < 0 || startSec > endSec)
        {
            throw OperationExc(get_invalid_interval_msg(startSec, endSec));
        }

        uint32_t dataStart = static_cast<uint32_t>(this->dataStart);
        uint32_t dataEnd = static_cast<uint32_t>(this->dataEnd);

        uint32_t start = dataStart + sec_to_byte_pos(startSec, this->header.byteRate, this->header.blockAlign);
        uint32_t end = dataStart + sec_to_byte_pos(endSec, this->header.byteRate, this->header.blockAlign);
        if (end > dataEnd)
            end = dataEnd;

        WavInterval interval(this, start, (end - start) / this->header.blockAlign);
        return interval;
    }

    void WavFile::WriteInterval(WavInterval interval, float destPos)
    {
        if (this == interval.wavFile)
        {
            throw OperationExc("Can't write interval to current file");
        }

        WavFile *readingWav = interval.wavFile;
        SReaderConfig sconfig{
            .path = readingWav->path,
            .srcFile = readingWav->file,
            .startPos = interval.startPos,
            .samplesCount = interval.samplesCount,
            .channelsCount = readingWav->header.numChannels,
            .bitsPerSample = readingWav->header.bitsPerSample,
        };

        SampleReader reader(sconfig);
        uint32_t bytePos = sec_to_byte_pos(destPos, this->header.byteRate, this->header.blockAlign);
        if (bytePos == config::MAX_WAV_DATA_SIZE)
        {
            std::string msg = std::string("Invalid write position: ") + std::to_string(destPos) + "sec";
            throw OperationExc(msg);
        }

        if (bytePos > this->header.subchunk2Size)
        {
            set_write_pos(this->file, this->dataEnd);
            uint32_t addSamplesCount = (bytePos - this->header.subchunk2Size) / this->header.blockAlign;

            for (uint32_t i = 0; i < addSamplesCount; i++)
            {
                Sample empty;
                this->writeSample(empty);
            }
        }
        Sample sample;
        while (reader.ReadSample(sample))
        {
            this->writeSample(sample);
        }
        if (sample.IsError())
        {
            throw InvalidWavFileExc(readingWav->path, "Can't read interval from wav file");
        }
        this->dataEnd = this->file.tellp();
        this->updateSubchunkSize();
    }

    void WavFile::writeSample(Sample &sample)
    {
        for (auto &i : sample.channelsData)
        {
            this->file.write(i.data(), i.size() * sizeof(byte));
        }
    }

    WavFile::~WavFile()
    {
        if (this->file.is_open())
        {
            if (this->isChanged)
                this->Save();

            this->file.close();
        }
    }

} // namespace wav_lib
