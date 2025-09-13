#include <cstring>
#include <iostream>
#include <string>

#include "../cmd/cmd_reader.h"
#include "../tables/tables.h"

#define TEST_ASSERT(expr)                                                         \
    do                                                                            \
    {                                                                             \
        if (!(expr))                                                              \
        {                                                                         \
            fprintf(stderr, "Assertion error: %s in line:%d\n", #expr, __LINE__); \
            return false;                                                         \
        }                                                                         \
    } while (0);

namespace tests
{
    static int allTestsCount = 0;
    static const char *testsDir = "tests_files";

    static lab0::CmdReader *get_cmd_reader(int argc, const char *argv[])
    {
        lab0::CmdReader *reader;
        try
        {
            reader = new lab0::CmdReader(argc, argv);
        }
        catch (lab0::InvalidArguments &exc)
        {
            fprintf(stderr, "Error: %s in line: %d\n", exc.what(), __LINE__);
            return nullptr;
        }
        return reader;
    }

    static bool cmd_reader_test()
    {
        const char *argv[] = {"lab", "input.txt", "-p", "-q", "-h=100", "-d=;", "-dest=test.csv", "-val"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        lab0::CmdReader *reader = get_cmd_reader(argc, argv);
        TEST_ASSERT(reader != nullptr);

        TEST_ASSERT(reader->check_option("p") == true);
        TEST_ASSERT(reader->check_option("q") == true);
        TEST_ASSERT(reader->check_option("val") == true);
        TEST_ASSERT(reader->check_option("x") == false);

        TEST_ASSERT(strcmp(reader->get_option_value("dest"), "test.csv") == 0);
        TEST_ASSERT(reader->get_option_value("val") == nullptr);
        TEST_ASSERT(reader->get_option_value("zzz") == nullptr);

        TEST_ASSERT(reader->get_option_value("lab") == nullptr);
        TEST_ASSERT(strcmp(reader->get_arg_value(0), "input.txt") == 0);
        TEST_ASSERT(reader->get_arg_value(1) == NULL);

        delete reader;
        return true;
    }

    static bool settings_test()
    {
        const char *argv[] = {"lab", "input.txt", "-p", "-q", "-h=100", "-d=;", "-dest=test.csv", "-val"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        lab0::CmdReader *reader = get_cmd_reader(argc, argv);
        TEST_ASSERT(reader != nullptr);

        lab0::TProgramSettings settings = reader->get_settings();
        TEST_ASSERT(!settings.isError);

        TEST_ASSERT(strcmp(settings.filePath, "input.txt") == 0);
        TEST_ASSERT(strcmp(settings.destPath, "test.csv") == 0);

        TEST_ASSERT(settings.maxColumns == 100);
        TEST_ASSERT(settings.delim == ';');

        delete reader;
        return true;
    }

    static bool csv_data_test()
    {
        std::string testPath = std::string(testsDir) + "/input.txt";
        std::string destPath("dest.csv");
        char delim = ';';
        int maxColumns = 100;

        lab0::TXTWordsFile wordsFile(testPath.c_str());
        lab0::TableStats stats(delim);
        lab0::TTableStatsResp resp = stats.SaveStatsToCsv(wordsFile, destPath.c_str(), maxColumns);
        TEST_ASSERT(!resp.isError);

        std::fstream destFile(destPath);
        TEST_ASSERT(destFile.is_open());

        std::string names, freqs;
        std::getline(destFile, names);
        std::getline(destFile, freqs);

        TEST_ASSERT(names == "aa;cc;a;ff;aaa;b;fff;jfjfj;ll;ssss;");
        TEST_ASSERT(freqs == "4;3;2;2;1;1;1;1;1;1;");

        std::remove(destPath.c_str());
        return true;
    }

    static int run_test(bool (*test_func)(), const char *testName)
    {
        allTestsCount++;

        bool res = test_func();
        if (!res)
        {
            fprintf(stderr, "FAILED: %s\n", testName);
        }
        return (int)res;
    }

} // namespace tests

using namespace tests;

int main()
{
    int passedCount = 0;
    passedCount += run_test(cmd_reader_test, "Cmd reader");
    passedCount += run_test(settings_test, "Settings");
    passedCount += run_test(csv_data_test, "Csv result test");

    if (passedCount != allTestsCount)
        return EXIT_FAILURE;
    printf("Tests finish\n");

    return EXIT_SUCCESS;
}
