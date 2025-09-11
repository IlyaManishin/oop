#include <iostream>

#include "cmd_settings.h"

#define DEFAULT_DELIM_CHAR ';'

namespace lab0
{
    typedef struct TProgramSettings
    {
        const char *filePath;
        const char *destPath;
        int maxColumns;
    } TProgramSettings;
}

using namespace lab0;

int main(int argc, const char *argv[])
{
    CmdSettings *cmdSettings;
    try
    {
        cmdSettings = new CmdSettings(argc, argv);
    }
    catch (const InvalidArguments &err)
    {
        std::cout << err.what();
    }

    return EXIT_SUCCESS;
}
