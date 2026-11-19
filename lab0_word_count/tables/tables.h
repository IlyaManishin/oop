#include <fstream>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace lab0
{
    class IterableFile
    {
    protected:
        std::fstream *file;
        std::vector<std::string> words;

    public:
        IterableFile(const char *filePath)
        {
            this->file = new std::fstream();
            this->file->open(filePath);
        }

        ~IterableFile()
        {
            delete this->file;
        }

        bool is_open() const { return file->is_open(); }

        auto begin() const { return words.begin(); }
        auto end() const { return words.end(); }
    };

    class TXTWordsFile : public IterableFile
    {
    public:
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
        void save_pairs_to_csv(const std::vector<std::pair<std::string, int>> &sortedPairs,
                               std::fstream &destFile, size_t wordsCount, int maxColumns);

        void NewFunction(int &maxColumns, const std::vector<std::pair<std::string, int>> &sortedPairs, std::fstream &destFile, size_t wordsCount);

    public:
        TableStats(char delim) : delim(delim) {};
        TTableStatsResp SaveStatsToCsv(const IterableFile &usedFile, const char *csvPath, int maxColumns);
    };
}