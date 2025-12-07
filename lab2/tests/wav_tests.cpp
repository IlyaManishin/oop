#include "../src/command_executor/cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"
#include "wav/wav.hpp"

#include <filesystem>
#include <gtest/gtest.h>

const char *WAV_LIB_TESTS_DIR = "./tests_data/wav_lib";

using namespace wav_lib;
TEST(CmdMixTest, BasicMixToCreatedFile)
{
    std::string inPath = std::string(WAV_LIB_TESTS_DIR) + "/test1/Smoke.wav"; // background
    std::string outPath = std::string(WAV_LIB_TESTS_DIR) + "/test1/Alive.wav";
    std::string destPath = std::string(WAV_LIB_TESTS_DIR) + "/test1/Mixed.wav"; // interval

    if (std::filesystem::exists(destPath))
        std::filesystem::remove(destPath);

    WavReader reader;
    WavFileSPtr file = reader.ReadWav(inPath);
    TWavHeader header = file->GetHeader();
    WavFileSPtr dest = reader.CreateWav(destPath, header.numChannels, header.sampleRate, header.bitsPerSample);

    cmd_parser::Args args;
    args.push_back(destPath);
    args.push_back(0.0f);
    args.push_back(inPath);
    args.push_back(0.0f);
    args.push_back(500.0f); // overflow

    bool ok = executor::cmd_mix(args);
    EXPECT_TRUE(ok);

    args.clear();
    args.push_back(destPath);
    args.push_back(0.0f);
    args.push_back(outPath);
    args.push_back(0.0f);



    
    args.push_back(10.0f); // overflow

    ok = executor::cmd_mix(args);
    EXPECT_TRUE(ok);

    args.clear();
    args.push_back(destPath);
    args.push_back(10.0f);
    args.push_back(outPath);
    args.push_back(100.0f);
    args.push_back(110.0f); // overflow

    ok = executor::cmd_mix(args);
    EXPECT_TRUE(ok);
}
