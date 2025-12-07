#pragma once

#include <cstdint>
#include <cstdlib>

namespace wav_lib
{
    namespace config
    {
        constexpr uint32_t MAX_WAV_DATA_SIZE = UINT32_MAX;
        constexpr int EMPTY_WAV_CHUNK_SIZE = 36;
        constexpr size_t FILE_BUFFER_SIZE = 1024 * 1024;
    } // namespace config

} // namespace wav_lib
