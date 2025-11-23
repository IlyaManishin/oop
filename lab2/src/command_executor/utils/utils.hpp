#include "wav/wav.hpp"

#include <string>

namespace executor
{
    wav_lib::WavFileSPtr try_read_wav(const wav_lib::WavReader &reader, const std::string &path) noexcept;

} // namespace executor