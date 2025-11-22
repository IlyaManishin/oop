#include "wav/wav.hpp"
#include <string>

namespace executor
{
    static wav_lib::WavFile *try_read_wav_file(const wav_lib::WavReader &reader, const std::string &path) noexcept;

} // namespace executor