#include "wav/wav.hpp"

namespace executor
{
    bool cmd_info_impl(wav_lib::WavFileSPtr wavFile);
    bool cmd_mute_impl(wav_lib::WavFileSPtr wavFile, float start, float end);
    bool cmd_mix_impl(wav_lib::WavFileSPtr outFile, float outStarts,
                      wav_lib::WavFileSPtr inFile, float inStart, float inEnd);
    // add create new

} // namespace executor
