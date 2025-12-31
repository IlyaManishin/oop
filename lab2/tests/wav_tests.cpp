#include "test_paths.hpp"

#include "../src/command_executor/cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"
#include "wav/internal/config.hpp"
#include "wav/internal/types.hpp"
#include "wav/internal/wav_utils.hpp"

#include "wav/wav.hpp"
#include "wav/wav_file.hpp"

#include <filesystem>
#include <gtest/gtest.h>

using namespace wav_lib;

TEST(WavMixTest, BasicMixToCreatedFile)
{
    std::string inPath = WAV_LIB_MEDIA_DIR + "/Smoke.wav"; // background
    std::string outPath = WAV_LIB_MEDIA_DIR + "/Alive.wav";
    std::string destPath = WAV_LIB_TESTS_DIR + "/Mixed.wav"; // interval

    if (std::filesystem::exists(destPath))
        std::filesystem::remove(destPath);

    WavReader reader;
    WavFileSPtr file = reader.OpenWav(inPath);
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

TEST(WavMixTest, UltraBassTest)
{
    auto src = wav_lib::WavFile::Open(WAV_LIB_MEDIA_DIR + "/Smoke.wav");

    auto dst = wav_lib::WavFile::Create(
        WAV_LIB_MEDIA_DIR + "UltraBass.wav",
        src->GetHeader().numChannels,
        src->GetHeader().sampleRate,
        src->GetHeader().bitsPerSample);

    auto interval = src->GetInterval(0.0f, 100.0f);
    interval->SetEffect(wav_lib::WavEffects::HACH_LADA);

    dst->WriteInterval(interval, 0.0f, false);
    dst->Save();
}

TEST(WavMixTest, multyEffects)
{
    auto src = wav_lib::WavFile::Open(WAV_LIB_MEDIA_DIR + "/Smoke.wav");

    auto dst = wav_lib::WavFile::Create(
        WAV_LIB_TESTS_DIR + "/multyEffects.wav",
        src->GetHeader().numChannels,
        src->GetHeader().sampleRate,
        src->GetHeader().bitsPerSample);

    wav_lib::WavEffects effects[] = {
        wav_lib::WavEffects::NORMAL,
        wav_lib::WavEffects::BASS,
        wav_lib::WavEffects::HACH_LADA,
        wav_lib::WavEffects::RAISE_HIGH,
        wav_lib::WavEffects::DISTORTION};

    const float startOut = 55.0f;
    const float len = 10.0f;

    for (size_t i = 0; i < std::size(effects); ++i)
    {
        auto interval = src->GetInterval(startOut + i * len, startOut + (i + 1) * len);
        interval->SetEffect(effects[i]);
        dst->WriteInterval(interval, i * len, false);
        dst->Save();
        dst->PrintInfo();
    }
}

TEST(InsertEmptySpace, BasicTest)
{
    const char *testFile = "test.bin";

    // создаём исходный файл 2 МБ, заполненный 0xAA
    {
        std::ofstream out(testFile, std::ios::binary);
        ByteVector data(2 * 1024 * 1024, 1);
        out.write((char *)data.data(), data.size());
    }

    // открываем для чтения/записи
    std::fstream file(testFile, std::ios::in | std::ios::out | std::ios::binary);
    ASSERT_TRUE(file.is_open());

    // вставляем 512 КБ пустого пространства в середину
    bool res = insert_empty_space(file, 1024 * 1024, 512 * 1024);
    ASSERT_TRUE(res);

    // проверяем новый размер
    file.seekg(0, std::ios::end);
    std::streampos newSize = file.tellg();
    EXPECT_EQ(newSize, 2 * 1024 * 1024 + 512 * 1024);

    // проверяем вставленное пространство заполнено нулями
    file.seekg(1024 * 1024);
    ByteVector check(512 * 1024);
    file.read((char *)check.data(), check.size());
    for (auto b : check)
    {
        EXPECT_EQ(b, 0);
    }

    file.close();
    std::remove(testFile);
}

TEST(WavMixTest, IntervalInsert)
{
    const std::string smokePath = WAV_LIB_MEDIA_DIR + "/Smoke.wav";
    const std::string alivePath = WAV_LIB_MEDIA_DIR + "/Alive.wav";
    const std::string destPath = WAV_LIB_TESTS_DIR + "/SmokeWithInsert.wav";

    if (std::filesystem::exists(destPath))
        std::filesystem::remove(destPath);

    auto smoke = wav_lib::WavFile::Open(smokePath);
    auto alive = wav_lib::WavFile::Open(alivePath);

    ASSERT_TRUE(smoke);
    ASSERT_TRUE(alive);

    auto dst = wav_lib::WavFile::Create(
        destPath,
        smoke->GetHeader().numChannels,
        smoke->GetHeader().sampleRate,
        smoke->GetHeader().bitsPerSample);

    ASSERT_TRUE(dst);

    {
        auto bg = smoke->GetInterval(0.0f, 1000);
        dst->WriteInterval(bg, 0.0f, false);
    }

    wav_lib::WavEffects effects[] = {
        wav_lib::WavEffects::BASS,
        wav_lib::WavEffects::DISTORTION,
        wav_lib::WavEffects::RAISE_HIGH};

    const float intervalLen = 10.0f;

    for (int i = 0; i < 3; ++i)
    {
        float srcStart = i * intervalLen;
        float srcEnd = srcStart + intervalLen;
        float destPos = (i + 1) * intervalLen;

        auto interval = alive->GetInterval(srcStart, srcEnd);
        ASSERT_TRUE(interval);

        interval->SetEffect(effects[i]);

        dst->WriteInterval(interval, destPos, true);
    }

    dst->Save();
}
