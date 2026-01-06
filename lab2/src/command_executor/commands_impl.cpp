#include "wav/wav.hpp"

#include <exception>
#include <iostream>

using namespace wav_lib;

namespace executor
{
    void cmd_wav_info_impl(IWavFileSPtr wavFile) noexcept
    {
        wavFile->PrintInfo();
    }

    void cmd_wav_mix_impl(IWavFileSPtr outFile, float outStart,
                          IWavFileSPtr inFile, float inStart, float inEnd)
    {
        auto interval = inFile->GetInterval(inStart, inEnd);
        outFile->WriteInterval(interval, outStart);
    }

    void cmd_set_effect_impl(IWavFileSPtr wavFile,
                             const std::string &effect,
                             float start,
                             float end)
    {
        WavEffects eff;

        if (effect == "normal")
            eff = wav_lib::WavEffects::NORMAL;
        else if (effect == "bass")
            eff = wav_lib::WavEffects::BASS;
        else if (effect == "hach_lada")
            eff = wav_lib::WavEffects::HACH_LADA;
        else if (effect == "raise_high")
            eff = wav_lib::WavEffects::RAISE_HIGH;
        else if (effect == "distortion")
            eff = wav_lib::WavEffects::DISTORTION;
        else
        {
            std::string msg = std::string("Unknown effect: ") + effect + "\n";
            throw std::runtime_error(msg);
        }

        auto interval = wavFile->GetInterval(start, end);
        interval->SetEffect(eff);

        wavFile->WriteInterval(interval, start, false);
        wavFile->Save();
    }

} // namespace executor