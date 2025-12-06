#include "../src/command_executor/cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>

const char *WAV_LIB_TESTS_DIR = "./tests_data/wav_lib";

TEST(CmdMixTest, BasicMix)
{
    std::cout << "Current working directory: "
              << std::filesystem::current_path() << std::endl;
    std::string outPath = std::string(WAV_LIB_TESTS_DIR) + "/test1/Alive.wav";
    std::string inPath = std::string(WAV_LIB_TESTS_DIR) + "/test1/Smoke.wav";

    cmd_parser::Args args;
    args.push_back(outPath);
    args.push_back(0.0f);
    args.push_back(inPath);
    args.push_back(0.0f);
    args.push_back(1.0f);

    bool ok = executor::cmd_mix(args);
    EXPECT_TRUE(ok);
}
