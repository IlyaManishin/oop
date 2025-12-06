#include "wav/wav.hpp"

namespace executor
{
    bool cmd_info_impl(wav_lib::WavFileSPtr wavFile);
    bool cmd_mute_impl(wav_lib::WavFileSPtr wavFile, float start, float end);
    bool cmd_mix_impl(wav_lib::WavFileSPtr outFile, float outStarts,
                      wav_lib::WavFileSPtr inFile, float inStart, float inEnd);
    bool cmd_change_speed_impl(wav_lib::WavFileSPtr wavFile, float start, float end, float speed);
    // add create new

} // namespace executor
