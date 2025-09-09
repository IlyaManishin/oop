#include <iostream>

#include "cmd_settings.h"

#define DEFAULT_DELIM_CHAR ';'

namespace lab0
{
    int main(int argc, const char *argv[])
    {
        try
        {
            CmdSettings settings(argc, argv);
        }
        catch (const InvalidArguments &err)
        {
            std::cout << err.what();
        }
        
        return EXIT_SUCCESS;
    }
}