#include "wav/wav.hpp"

namespace executor
{
    bool cmd_info_impl(wav_lib::WavFile *wavFile);
    bool cmd_mute_impl(wav_lib::WavFile *wavFile, float start, float end);
    bool cmd_mix_impl(wav_lib::WavFile *outFile, float outStart, float outEnd,
                      wav_lib::WavFile *inFile, float inStart, float inEnd);
    bool cmd_change_speed_impl(wav_lib::WavFile *wavFile, float start, float end, float speed);

} // namespace executor
