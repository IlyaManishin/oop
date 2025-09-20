#include <cstring>
#include <iostream>

#include "cmd/cmd_reader.h"
#include "tables/tables.h"

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
        delete cmdReader;
        return EXIT_FAILURE;
    }
    TXTWordsFile wordsFile(settings.filePath);
    TableStats stats(settings.delim);
    TTableStatsResp resp = stats.SaveStatsToCsv(wordsFile, settings.destPath, settings.maxColumns);
    if (resp.isError)
    {
        std::cout << resp.errMsg << std::endl;
        return EXIT_FAILURE;
    }

    delete cmdReader;
    return EXIT_SUCCESS;
}
