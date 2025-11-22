#include "../wav_exceptions.hpp"

#include <string>

namespace wav_lib
{
    const std::string &get_msg_with_path(const std::string &msg, const std::string &path)
    {
        return std::string("WAV file error(") + path + std::string("): ") + msg;
    }

} // namespace wav_lib
