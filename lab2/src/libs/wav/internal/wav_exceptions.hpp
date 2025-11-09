#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class WavException : public std::runtime_error
{
public:
    explicit WavException(const std::string &msg = "") : std::runtime_error(msg) {}
};

class InvalidWavFileExc : public WavException
{
public:
    explicit InvalidWavFileExc(const std::string &msg = "Invalid WAV file") : WavException(msg) {}
};


