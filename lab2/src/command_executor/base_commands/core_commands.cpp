#include "core_commands.hpp"
#include "wav/wav.hpp"

#include <iostream>

using namespace wav_lib;

namespace executor
{
    bool cmd_info_impl(WavFile *wavFile)
    {
        wavFile->PrintInfo();
        return true;
    }

    bool cmd_mute_impl(WavFile *wavFile, float start, float end)
    {
        std::cout << "Muted interval successfully (stub)\n";
        return true;
    }

    bool cmd_mix_impl(WavFile *outFile, float outStart, float outEnd,
                      WavFile *inFile, float inStart, float inEnd)
    {
        std::cout << "Mixed successfully\n";
        return true;
    }

        bool cmd_change_speed_impl(WavFile *wavFile, float start, float end, float speed)
    {
        std::cout << "Speed changed successfully (stub)\n";
        return true;
    }
    
} // namespace executor