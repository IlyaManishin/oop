#include "../src/command_executor/cmd_commands.hpp"
#include "cmd_parser/cmd_parser.hpp"
#include "wav/internal/config.hpp"
#include "wav/internal/types.hpp"
#include "wav/internal/wav_utils.hpp"

#include "wav/wav.hpp"

#include <filesystem>
#include <gtest/gtest.h>

const std::string WAV_LIB_TESTS_DIR = "./tests_data/wav_lib";
const std::string WAV_LIB_MEDIA_DIR = WAV_LIB_TESTS_DIR + "/media";

using namespace wav_lib;

TEST(WavMixTest, BasicMixToCreatedFile)
{
    std::string inPath = WAV_LIB_MEDIA_DIR + "/Smoke.wav"; // background
    std::string outPath = WAV_LIB_MEDIA_DIR + "/Alive.wav";
    std::string destPath = WAV_LIB_TESTS_DIR + "/Mixed.wav"; // interval

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

TEST(WavMixTest, UltraBassTest)
{
    auto src = wav_lib::WavFile::Open(WAV_LIB_MEDIA_DIR + "/Smoke.wav");

    auto dst = wav_lib::WavFile::Create(
        WAV_LIB_MEDIA_DIR + "UltraBass.wav",
        src->GetHeader().numChannels,
        src->GetHeader().sampleRate,
        src->GetHeader().bitsPerSample);

    auto interval = src->GetInterval(0.0f, 100.0f);
    interval->SetEffect(wav_lib::WavEffects::DISTORTION);

    dst->WriteInterval(interval, 0.0f, false);
    dst->Save();
}

TEST(WavMixTest, multyEffects)
{
    auto src = wav_lib::WavFile::Open(WAV_LIB_MEDIA_DIR + "/Smoke.wav");

    auto dst = wav_lib::WavFile::Create(
        WAV_LIB_MEDIA_DIR + "/multyEffects.wav",
        src->GetHeader().numChannels,
        src->GetHeader().sampleRate,
        src->GetHeader().bitsPerSample);

    wav_lib::WavEffects effects[] = {
        wav_lib::WavEffects::BASS,
        wav_lib::WavEffects::HACH_LADA,
        wav_lib::WavEffects::RAISE_HIGH,
        wav_lib::WavEffects::DISTORTION};

    const float start = 55.0f;
    const float len = 10.0f;

    for (size_t i = 0; i < std::size(effects); ++i)
    {
        auto interval = src->GetInterval(start + i * len, start + (i + 1) * len);
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
        ByteVector data(2 * 1024 * 1024, 0xAA);
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
