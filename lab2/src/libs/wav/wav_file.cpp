#include "internal/config.hpp"
#include "internal/sample_reader.hpp"
#include "internal/types.hpp"
#include "internal/wav_utils.hpp"
#include "wav.hpp"

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr int MAX_CHAIN_COUNT = 12;

namespace wav_lib
{
    class WavInterval : public IWavInterval
    {
    public:
        WavFile *wavFile;
        uint32_t startPos;
        uint32_t samplesCount;
        double durationSec;

        WavEffects effect = WavEffects::NORMAL;
        bool isNewVolume = false;
        float volumeValue;

        WavInterval(WavFile *wavFile, uint32_t startPos, uint32_t samplesCount, double durationSec)
            : wavFile(wavFile), startPos(startPos),
              samplesCount(samplesCount), durationSec(durationSec) {};

        void SetEffect(WavEffects effect) override { this->effect = effect; }
        void SetVolume(float value) override { this->volumeValue = value; }
        bool IsChangedSound() { return isNewVolume || (this->effect != WavEffects::NORMAL); };

        ~WavInterval() {};
    };

    WavFile::WavFile(const std::string &wavPath, bool createNew)
    {
        this->path = wavPath;
        this->isChanged = false;

        auto mode = std::ios::in | std::ios::out | std::ios::binary;
        if (createNew)
            mode = mode | std::ios::trunc;

        this->file.open(wavPath, mode);
        if (!this->file.is_open())
            throw InvalidWavFileExc("Cannot open file", this->path);
    }

    WavFileSPtr WavFile::Open(const std::string &path)
    {
        WavFileSPtr file(new WavFile(path, false));
        file->extractFileData();
        return file;
    }

    WavFileSPtr WavFile::Create(const std::string &path, uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample)
    {
        WavFileSPtr file(new WavFile(path, true));
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
        this->header.chunkSize = config::EMPTY_WAV_CHUNK_SIZE;

        this->isChanged = true;
    }

    void WavFile::PrintInfo() const
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

    IWavIntervalSPtr WavFile::GetInterval(float startSec, float endSec)
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

        double deltaSec = (double)(end - start) / (this->header.byteRate);
        uint32_t samplesCount = (end - start) / this->header.blockAlign;
        IWavIntervalSPtr interval = std::make_shared<WavInterval>(this, start, samplesCount, deltaSec);
        return interval;
    }

    void WavFile::WriteInterval(IWavIntervalSPtr intervalI, float destSecPos, bool isInsert)
    {
        WavIntervalSPtr interval = std::dynamic_pointer_cast<WavInterval>(intervalI);
        if (interval == nullptr)
            throw OperationExc("Invalid interval to write");

        uint32_t destBytePos = sec_to_byte_pos(destSecPos, this->header.byteRate, this->header.blockAlign);
        if (destBytePos == config::MAX_WAV_DATA_SIZE)
        {
            std::string msg = std::string("Invalid write position: ") + std::to_string(destSecPos) + "sec";
            throw OperationExc(msg);
        }

        if (destBytePos > this->header.subchunk2Size) // is after file end - extend
        {
            isInsert = false;

            set_write_pos(this->file, this->dataEnd);
            uint32_t addBytes = destBytePos - this->header.subchunk2Size;
            bool res = extend_file_with_zeros(this->file, addBytes);
            if (!res)
            {
                std::string msg = get_msg_with_path(this->path, "Can't extend file");
                throw OperationExc(msg);
            }
        }
        std::streampos destPos = get_offset_pos(this->dataStart, destBytePos);

        bool isSameSound = (this == interval->wavFile) || (this->cmpVolumeParams(interval->wavFile));
        if (interval->IsChangedSound() || !isSameSound)
        {
            this->writeIntervalSlow(interval, isInsert, destPos);
        }
        else
        {
            this->writeIntervalFast(interval, isInsert, destPos);
        }
        this->updateSubchunkSize();
    }

    void WavFile::writeIntervalFast(WavIntervalSPtr interval, bool isInsert, std::streampos destPos)
    {
        assert(!interval->IsChangedSound());
        assert(*this == *interval->wavFile || this->cmpVolumeParams(interval->wavFile));

        uint32_t intervalLength = interval->samplesCount * this->header.blockAlign;
        if (isInsert)
        {
            this->allocIntervalSpace(interval, intervalLength, destPos);
        }

        if (*this == *interval->wavFile)
            this->writeIntervalFromCurFast(interval, destPos);
        else
            this->writeIntervalFromOtherFast(interval, destPos);
    }

    void WavFile::writeIntervalFromCurFast(WavIntervalSPtr interval, std::streampos destPos)
    {
        assert(*this == *interval->wavFile);
        assert(!interval->IsChangedSound());

        uint32_t intervalLength = interval->samplesCount * this->header.blockAlign;
        ByteVector *data = read_file_big_vector(this->file, intervalLength, interval->startPos);
        if (data == nullptr)
        {
            std::string msg = get_msg_with_path(this->path, "Can't read interval from file");
            throw OperationExc(msg);
        }

        bool res = write_big_vector_to_file(this->file, destPos, data, data->size());
        delete data;
        if (!res)
        {
            std::string msg = get_msg_with_path(this->path, "Can't write interval to file");
            throw OperationExc(msg);
        }

        std::streampos endIntervalPos = get_offset_pos(destPos, intervalLength);
        this->dataEnd = std::max(this->dataEnd, endIntervalPos);
    }

    void WavFile::writeIntervalFromOtherFast(WavIntervalSPtr interval, std::streampos destPos)
    {
        assert(this->cmpVolumeParams(interval->wavFile) && !interval->IsChangedSound());
        assert(this != interval->wavFile);

        uint32_t intervalLength = interval->samplesCount * this->header.blockAlign;
        uint32_t remaining = intervalLength;

        std::fstream &srcFile = interval->wavFile->file;
        std::streampos chunkSrc = interval->startPos;
        std::streampos chunkDest = destPos;

        ByteVector *buffer = new ByteVector(config::FILE_BUFFER_SIZE);

        while (remaining > 0)
        {
            uint32_t chunkSize = std::min<uint32_t>(remaining, buffer->size());
            bool res = read_file_chunk(srcFile, chunkSize, chunkSrc, buffer);
            if (!res)
            {
                std::string msg = get_msg_with_path(this->path, "Can't write interval to file");
                throw OperationExc(msg);
            }

            res = write_vector_to_file(this->file, chunkDest, buffer, chunkSize);
            if (!res)
            {
                std::string msg = get_msg_with_path(this->path, "Can't write interval to file");
                throw OperationExc(msg);
            }

            chunkSrc += chunkSize;
            chunkDest += chunkSize;
            remaining -= chunkSize;
        }
        std::streampos endPos = get_offset_pos(destPos, intervalLength);
        this->dataEnd = std::max(this->dataEnd, endPos);
    }

    void WavFile::writeIntervalSlow(WavIntervalSPtr interval, bool isInsert, std::streampos destPos)
    {
        WavFile *srcWav = interval->wavFile;

        uint32_t srcSampleRate = srcWav->header.sampleRate;
        uint32_t dstSampleRate = this->header.sampleRate;

        uint64_t intervalBytes;
        if (this->header.sampleRate != srcWav->header.sampleRate)
        {
            uint64_t dstSamplesCount = (uint64_t)((double)(interval->samplesCount) * dstSampleRate / srcSampleRate);
            intervalBytes = dstSamplesCount * this->header.blockAlign;
        }
        else
        {
            intervalBytes = interval->samplesCount * this->header.blockAlign;
        }

        if (intervalBytes > config::MAX_WAV_DATA_SIZE)
            throw OperationExc("Interval is so long...");

        if (isInsert)
        {
            this->allocIntervalSpace(interval, static_cast<uint32_t>(intervalBytes), destPos);
        }

        if (*this == *interval->wavFile)
        {
            this->writeIntervalFromCurSlow(interval, destPos);
        }
        else
        {
            this->writeIntervalFromOtherSlow(interval, destPos);
        }
    }

    void WavFile::writeIntervalFromCurSlow(WavIntervalSPtr interval, std::streampos destPos)
    {
        assert(*this == *interval->wavFile);
    }

    void WavFile::writeIntervalFromOtherSlow(WavIntervalSPtr interval, std::streampos destPos)
    {
        assert(*this != *interval->wavFile);
    }

    // void WavFile::writeIntervalSlow(WavIntervalSPtr interval, bool isInsert, std::streampos destPos)
    // {
    //     // WavFile *readingWav = interval->wavFile;
    //     // SReaderConfig sconfig{
    //     //     .path = readingWav->path,
    //     //     .srcFile = readingWav->file,
    //     //     .startPos = interval->startPos,
    //     //     .samplesCount = interval->samplesCount,
    //     //     .channelsCount = readingWav->header.numChannels,
    //     //     .bitsPerSample = readingWav->header.bitsPerSample,
    //     // };

    //     // SampleReader reader(sconfig);

    //     // Sample sample;
    //     // while (reader.ReadSample(sample))
    //     // {
    //     //     this->writeSample(sample);
    //     // }
    //     // if (sample.IsError())
    //     // {
    //     //     throw InvalidWavFileExc(readingWav->path, "Can't read interval from wav file");
    //     // }
    //     // this->dataEnd = std::max(this->dataEnd, this->file.tellp());
    // }

    bool WavFile::allocIntervalSpace(WavIntervalSPtr interval, uint32_t intervalLength, std::streampos destPos)
    {
        bool res = insert_empty_space(this->file, destPos, intervalLength);
        this->dataEnd = get_offset_pos(this->dataEnd, intervalLength);

        if (this == interval->wavFile && interval->startPos > destPos)
            interval->startPos += intervalLength;
        this->updateSubchunkSize();

        return res;
    }

    void WavFile::writeIntervalWithReader(WavIntervalSPtr interval, std::streampos destPos,
                                          ISampleReader &reader, uint32_t maxSamples)
    {
    }

    void WavFile::writeSample(Sample &sample)
    {
        for (auto &i : sample.channelsData)
        {
            this->file.write(i.data(), i.size() * sizeof(Byte));
        }
    }

    bool WavFile::cmpVolumeParams(WavFile *other)
    {
        const TWavHeader &header1 = this->header;
        const TWavHeader &header2 = other->header;

        bool res = header1.numChannels == header2.numChannels &&
                   header1.blockAlign == header2.blockAlign &&
                   header1.sampleRate == header2.sampleRate;
        return res;
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
