#pragma once

#include <string>

class WavFile
{

public:
    WavFile(std::string wavPath);
    void playWav();

    ~WavFile();

private:
    
};

class WavInterval
{
    
};

class WavReader
{
public:
    WavReader(const std::string &waveDir = "");
    WavFile *ReadWav(const std::string &path) const;
    WavFile *CreateWav(const std::string &destPath) const;
};
