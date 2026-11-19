#include "wav/wav.hpp"

#include <string>

namespace executor
{
    void cmd_wav_info_impl(wav_lib::IWavFileSPtr wavFile) noexcept;
    void cmd_wav_mix_impl(wav_lib::IWavFileSPtr outFile, float outStarts,
                          wav_lib::IWavFileSPtr inFile, float inStart, float inEnd);
    void cmd_set_effect_impl(wav_lib::IWavFileSPtr wavFile, const std::string &effect,
                        float start, float end);

    // add create new file

} // namespace executor
