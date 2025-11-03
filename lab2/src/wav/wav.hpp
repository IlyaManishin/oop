#pragma once

#include <string>

class WavInterval
{
public:
    WavInterval(WavFile *file, float startSec, float endSec, float speed)
        : startSec(startSec), endSec(endSec), speed(speed) {};

private:    
    WavFile* file;
    float startSec;
    float endSec;
    float speed;
};

class WavFile
{

public:
    WavFile(std::string wavPath);
    void playWav();
    void writeInterval(WavInterval interval);
    void getInterval(float startSec, float endSec);

    ~WavFile();

private:
};

class WavReader
{
public:
    WavReader(const std::string &waveDir = "");
    WavFile *readWav(const std::string &path) const;
    WavFile *createWav(const std::string &destPath) const;

private:
    bool _isWavFile(std::string &path);
};
