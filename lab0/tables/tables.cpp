#include <map>
#include <string>

#include "tables.h"

namespace lab0
{

    TXTWordsFile::TXTWordsFile(const char *filePath) : IterableFile(filePath)
    {
        
    }

    TTableStatsResp TableStats::SaveStatsToCsv(const char *csvPath, int maxColumns)
    {
        TTableStatsResp resp;

        FILE *csvFile = fopen64(csvPath, "w");
        if (csvFile == NULL)
        {
            resp.isError = true;
            resp.errMsg = "Invalid stats csv file";
            return resp;
        }

        std::map<std::string, int> frequency;
        size_t wordsCount = 0;
        for (std::string &word : *this->usedFile)
        {
            auto iter = frequency.find(word);
            if (iter == frequency.end())
            {
                frequency[word] = 1;
            }
            else
            {
                iter->second++;
            }
            wordsCount++;
        }

        resp.isError = false;
        return resp;
    }

}