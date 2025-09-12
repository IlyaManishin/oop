#include <iostream>

#include "cmd/cmd_reader.h"
#include "cstring"


using namespace lab0;

int main(int argc, const char *argv[])
{
    CmdReader *cmdReader;
    try
    {
        cmdReader = new CmdReader(argc, argv);
    }
    catch (const InvalidArguments &err)
    {
        std::cout << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    TProgramSettings settings = cmdReader->get_settings();
    if (settings.isError)
    {
        std::cout << settings.errMsg << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
