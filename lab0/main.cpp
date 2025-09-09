#include <iostream>

#include "cmd_settings.h"

#define DEFAULT_DELIM_CHAR ';'

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
