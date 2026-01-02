#include "../src/command_executor/cmd_commands.hpp"

#include "cmd_parser/cmd_parser.hpp"
#include "test_paths.hpp"
#include "wav/wav.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <string>
#include <variant>

using namespace wav_lib;
using namespace cmd_parser;
using namespace executor;

TEST(CmdCreateWavTest, CreateEmptyWavFile)
{
    const std::string path = "test_empty.wav";

    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    Args args;
    args.emplace_back(path);

    bool result = cmd_create_wav(args);
    EXPECT_TRUE(result);
    EXPECT_TRUE(std::filesystem::exists(path));

    WavReader reader;
    WavFileSPtr created = reader.OpenWav(path);

    EXPECT_FLOAT_EQ(created->GetDurationSec(), 0.0f);

    std::filesystem::remove(path);
}

TEST(CmdMixTest, TwoIntervalsTotalDuration30Sec)
{
    const std::string srcPath = WAV_LIB_MEDIA_DIR + "/Alive.wav";
    const std::string dstPath = WAV_LIB_TESTS_DIR + "/Mix30sec.wav";

    if (std::filesystem::exists(dstPath))
        std::filesystem::remove(dstPath);

    WavReader reader;
    auto src = reader.OpenWav(srcPath);
    ASSERT_TRUE(src);

    auto header = src->GetHeader();
    reader.CreateWav(dstPath, header.numChannels, header.sampleRate, header.bitsPerSample);

    Args args;

    args.push_back(dstPath);
    args.push_back(0.0f);
    args.push_back(srcPath);
    args.push_back(0.0f);
    args.push_back(10.0f);

    EXPECT_TRUE(cmd_mix(args));

    args.clear();
    args.push_back(dstPath);
    args.push_back(10.0f);
    args.push_back(srcPath);
    args.push_back(20.0f);
    args.push_back(40.0f);

    EXPECT_TRUE(cmd_mix(args));

    auto result = reader.OpenWav(dstPath);
    ASSERT_TRUE(result);

    EXPECT_FLOAT_EQ(result->GetDurationSec(), 30.0f);

    std::filesystem::remove(dstPath);
}
