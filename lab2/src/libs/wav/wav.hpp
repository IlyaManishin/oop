#pragma once

#include "internal/wav_internal.hpp"
#include "internal/wav_exceptions.hpp"

#include <cstdint>
#include <fstream>
#include <string>

namespace wav_lib
{
    class WavInterval;

    class WavFile
    {
    public:
        WavFile(const std::string &wavPath);
        ~WavFile();

        void PrintInfo();
        void PlayWav();
        void WriteInterval(const WavInterval &interval);
        void GetInterval(float startSec, float endSec);
        void Close();

    private:
        std::string path;
        std::fstream file;
        
        std::streampos dataStart;
        std::streampos dataEnd;
        
        _detail::TWavHeader header;

        void _extract_file_data();
        void _save_header();
    };

    class WavInterval
    {
    public:
        WavInterval(WavFile *file, float startSec, float endSec);
        void ChangeSpeed(float speed);

    private:
        WavFile *file;
        float startSec;
        float endSec;
        float speed;
    };

    class WavReader
    {
    public:
        WavReader(const std::string &wavDir = "") : wavDir(wavDir) {};
        WavFile *ReadWav(const std::string &path) const;
        WavFile *CreateWav(const std::string &destPath) const;
        bool IsExistsWav(const std::string &path) const;

    private:
        std::string wavDir;
        bool _isWavFile(const std::string &path) const;
    };
} // namespace wav_lib
