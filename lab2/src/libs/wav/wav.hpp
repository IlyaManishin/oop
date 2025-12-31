#pragma once

#include "internal/exceptions.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace wav_lib
{
    enum class WavEffects
    {
        NORMAL,
        BASS,
        HACH_LADA,
        RAISE_HIGH,
        DISTORTION,
    };

    typedef struct TWavHeader
    {
        uint32_t chunkSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate; // Freq per second
        uint32_t byteRate;
        uint16_t blockAlign;    // All channels block size
        uint16_t bitsPerSample; // Per one channel
        uint32_t subchunk2Size; // Full wav data size
    } TWavHeader;

    class IWavFile;
    class IWavInterval;

    using WavFileSPtr = std::shared_ptr<IWavFile>;
    using IWavIntervalSPtr = std::shared_ptr<IWavInterval>;

    class IWavInterval
    {
    public:
        virtual ~IWavInterval() = default;

        virtual void SetEffect(WavEffects effect) = 0;
        virtual void SetVolume(float value) = 0;
        virtual bool IsChangedSound() = 0;
        virtual void Print(std::ostream &out = std::cout) const = 0;
    };

    class IWavFile
    {
    public:
        virtual void PrintInfo(std::ostream &out = std::cout) const noexcept = 0;
        virtual TWavHeader GetHeader() const = 0;
        virtual double GetDurationSec() const = 0;

        virtual void Save() = 0;

        virtual IWavIntervalSPtr GetInterval(float startSec, float endSec) = 0;
        virtual void WriteInterval(IWavIntervalSPtr interval,
                                   float destSecPos,
                                   bool isInsert = false) = 0;

        virtual ~IWavFile() = default;
    };

    class WavReader
    {
    public:
        WavReader(const std::string &wavDir = "") noexcept : wavDir(wavDir) {};
        WavFileSPtr OpenWav(const std::string &path) const;
        WavFileSPtr CreateWav(const std::string &destPath,
                              int numChannels = 2,
                              uint32_t sampleRate = 44100,
                              uint32_t bitsPerSample = 16) const;
        bool IsExistsWav(const std::string &path) const noexcept;

    private:
        std::string wavDir;
        bool isWavFile(const std::string &path) const noexcept;
    };
} // namespace wav_lib
