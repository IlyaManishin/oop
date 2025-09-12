#include <iostream>
#include <string>
#include <vector>

namespace lab0
{
    class IterableFile
    {
    private:
        FILE *file;
        size_t curWordIndex;
        size_t wordsCount;
        std::vector<std::string> words;

    public:
        IterableFile(const char *filePath)
        {
            this->file = fopen64(filePath, "r");
        }

        ~IterableFile()
        {
            fclose(this->file);
        }

        auto begin()  { return words.begin(); }
        auto end()  { return words.end(); }
    };

    class TXTWordsFile : public IterableFile
    {
        TXTWordsFile(const char *filePath);
        ~TXTWordsFile() = default;
    };

    typedef struct TTableStatsResp
    {
        bool isError;
        const char *errMsg;
    } TTableStatsResp;

    class TableStats
    {
    private:
        char delim;
        IterableFile *usedFile;

    public:
        TableStats(IterableFile *usedFile, char delim) : usedFile(usedFile), delim(delim) {};
        TTableStatsResp SaveStatsToCsv(const char *csvPath, int maxColumns);
    };
}