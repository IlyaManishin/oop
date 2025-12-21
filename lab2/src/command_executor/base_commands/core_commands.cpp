#include "core_commands.hpp"
#include "wav/wav.hpp"

#include <iostream>
#include <exception>

using namespace wav_lib;

namespace executor
{
    bool cmd_info_impl(WavFileSPtr wavFile)
    {
        wavFile->PrintInfo();
        return true;
    }

    bool cmd_mix_impl(WavFileSPtr outFile, float outStart,
                      WavFileSPtr inFile, float inStart, float inEnd)
    {
        try
        {
            auto interval = inFile->GetInterval(inStart, inEnd);
            outFile->WriteInterval(interval, outStart);
        }
        catch (OperationExc &err)
        {
            std::cerr << err.what();
            return false;
        }
        return true;
    }

} // namespace executor