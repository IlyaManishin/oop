#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace wav_lib
{
    const std::string &get_msg_with_path(const std::string &msg, const std::string &path);

    class WavException : public std::runtime_error
    {
    public:
        explicit WavException(const std::string &msg = "") : std::runtime_error(msg) {};
        explicit WavException(const std::string &msg, const std::string &path)
            : std::runtime_error(get_msg_with_path(msg, path)) {};
    };

    class InvalidWavFileExc : public WavException
    {
        using WavException::WavException;
    public:
        explicit InvalidWavFileExc(const std::string &msg = "Invalid WAV file") : WavException(msg) {};
    };

} // namespace wav_lib
