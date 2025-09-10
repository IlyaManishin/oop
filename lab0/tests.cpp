#include <cstring>
#include <iostream>

#include "cmd_settings.h"

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

    bool cmd_settings_test()
    {
        const char *argv[] = {"lab", "-p", "-q", "-d=test.csv", "-val"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        lab0::CmdSettings *settings;
        try
        {
            settings = new lab0::CmdSettings(argc, argv);
        }
        catch (lab0::InvalidArguments &exc)
        {
            fprintf(stderr, "Error: %s in line: %d\n", exc.what(), __LINE__);
            return false;
        }

        TEST_ASSERT(settings->check_option("p") == true);
        TEST_ASSERT(settings->check_option("q") == true);
        TEST_ASSERT(settings->check_option("val") == true);
        TEST_ASSERT(settings->check_option("x") == false);

        TEST_ASSERT(strcmp(settings->get_value("d"), "test.csv") == 0);
        TEST_ASSERT(settings->get_value("val") == nullptr);
        TEST_ASSERT(settings->get_value("zzz") == nullptr);

        TEST_ASSERT(settings->get_value("lab") == nullptr);

        delete settings;
        return true;
    }

    int run_test(bool (*test_func)(), const char *testName)
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
    passedCount += run_test(cmd_settings_test, "Cmd settings");
    if (passedCount != allTestsCount)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
