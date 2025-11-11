#include <cstdint>

namespace wav_lib
{
    namespace _detail
    {
        typedef struct TWavHeader
        {
            uint32_t chunkSize;
            uint16_t audioFormat;
            uint16_t numChannels;
            uint32_t sampleRate;
            uint32_t byteRate;
            uint16_t blockAlign;
            uint16_t bitsPerSample;
            uint32_t subchunk2Size;
        } TWavHeader;
    } // namespace _detail

} // namespace wav_lib
