#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace wav_lib
{
    inline const std::string get_msg_with_path(const std::string &msg, const std::string &path)
    {
        return std::string("WAV file error(") + path + std::string("): ") + msg;
    }

    inline const std::string get_invalid_interval_msg(float start, float end)
    {
        std::string interval = std::to_string(start) + "," + std::to_string(end);
        return std::string("Invalid wav interval: (") + interval + std::string(")");
    }

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
