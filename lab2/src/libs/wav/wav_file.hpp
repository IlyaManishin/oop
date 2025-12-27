#include "wav.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace wav_lib
{
    class ISampleReader;
    struct TWavSoundParams;
    struct SampleReaderConfig;

    class WavInterval;
    using WavIntervalSPtr = std::shared_ptr<WavInterval>;

    class WavFile : public IWavFile
    {
    public:
        static WavFileSPtr Open(const std::string &path);
        static WavFileSPtr Create(const std::string &path,
                                  uint16_t channels,
                                  uint32_t sampleRate,
                                  uint16_t bitsPerSample);

        void PrintInfo(std::ostream &out = std::cout) const override;
        TWavHeader GetHeader() const override { return this->header; };

        void Save() override;

        IWavIntervalSPtr GetInterval(float startSec, float endSec) override;
        void WriteInterval(IWavIntervalSPtr intervalI, float destSecPos, bool isInsert = false) override;

        ~WavFile();

    private:
        WavFile(const std::string &wavPath, bool createNew);

        std::string path;
        std::fstream file;

        std::streampos dataStart;
        std::streampos dataEnd;

        bool isChanged;

        TWavHeader header;

        void extractFileData();
        void saveHeader();
        void initNewHeader(uint16_t channels, uint32_t sampleRate, uint16_t bitsPerSample);
        void updateSubchunkSize();

        bool cmpVolumeParams(WavFile *other);
        bool operator==(WavFile &file) { return this->path == file.path; };

    private:
        bool allocIntervalSpace(WavIntervalSPtr interval, uint32_t intervalLength, std::streampos destPos);

        void writeIntervalFast(WavIntervalSPtr interval, bool isInsert, std::streampos destPos);
        void writeIntervalFromCurFast(WavIntervalSPtr interval, std::streampos destPos);
        void writeIntervalFromOtherFast(WavIntervalSPtr interval, std::streampos destPos);

        void writeIntervalSlow(WavIntervalSPtr interval, bool isInsert, std::streampos destPos);
        uint32_t writeIntervalFromCurSlow(WavIntervalSPtr interval, std::streampos destPos, uint64_t maxSamples);
        uint32_t writeIntervalFromOtherSlow(WavIntervalSPtr interval, std::streampos destPos, uint64_t maxSamples);
        uint32_t writeIntervalWithReader(std::streampos destPos, ISampleReader &reader, uint32_t maxSamples);

        void getSampleReaderConfig(WavIntervalSPtr interval,
                                   SampleReaderConfig &config,
                                   uint64_t maxSamples) const;
        void getReaderSoundParams(TWavSoundParams &params) const;
    };
} // namespace wav_lib
