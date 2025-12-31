#include "wav/wav.hpp"

#include <string>

namespace executor
{
    void cmd_wav_info_impl(wav_lib::WavFileSPtr wavFile) noexcept;
    void cmd_wav_mix_impl(wav_lib::WavFileSPtr outFile, float outStarts,
                          wav_lib::WavFileSPtr inFile, float inStart, float inEnd);
    void cmd_set_effect_impl(wav_lib::WavFileSPtr wavFile, const std::string &effect,
                        float start, float end);

    // add create new file

} // namespace executor
