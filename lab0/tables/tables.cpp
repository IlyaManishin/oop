#include <algorithm>
#include <iomanip>
#include <map>
#include <string>

#include "tables.h"

#define is_word_symbol(ch) \
    (('a' <= (ch) && (ch) <= 'z') || (('A' <= (ch) && (ch) <= 'Z')) || ('0' <= (ch) && (ch) <= '9') || ((unsigned char)ch >= 128))
#define min(a, b) ((a) < (b) ? (a) : (b))

namespace lab0
{
    class FileBuffer
    {
    private:
        std::fstream *file;
        std::string curLine;
        size_t curIndex;
        bool isEnd = true;

    public:
        FileBuffer(std::fstream *file)
        {
            this->file = file;
            this->curIndex = 0;
            this->isEnd = false;
            std::getline(*this->file, this->curLine, '\n');
        };
        int getCh(char &ch)
        {
            if (isEnd)
                return -1;

            if (curIndex == curLine.size())
            {
                if (!std::getline(*file, curLine, '\n'))
                {
                    isEnd = true;
                    return -1;
                }
                curIndex = 0;
                ch = '\n';
                return 0;
            }
            ch = curLine[curIndex++];
            return 0;
        }
    };

    TXTWordsFile::TXTWordsFile(const char *filePath) : IterableFile(filePath)
    {
        FileBuffer buffer(this->file);
        std::vector<char> wordBuffer;
        char ch;
        while (buffer.getCh(ch) == 0)
        {
            if (is_word_symbol(ch))
            {
                wordBuffer.push_back(ch);
            }
            else if (wordBuffer.size() != 0)
            {
                words.push_back(std::string(wordBuffer.begin(), wordBuffer.end()));
                wordBuffer.clear();
            }
        }
        if (wordBuffer.size() != 0)
        {
            words.push_back(std::string(wordBuffer.begin(), wordBuffer.end()));
        }
    }

    TTableStatsResp TableStats::SaveStatsToCsv(const IterableFile &srcFile, const char *destCsvPath, int maxColumns)
    {
        TTableStatsResp resp;
        if (!srcFile.is_open())
        {
            resp.isError = true;
            resp.errMsg = "Invalid input file";
            return resp;
        }

        std::fstream csvFile(destCsvPath, std::ios::out);
        if (!csvFile.is_open())
        {
            resp.isError = true;
            resp.errMsg = "Invalid stats csv file";
            return resp;
        }

        std::map<std::string, int> frequency;
        size_t wordsCount = 0;
        for (const std::string &word : srcFile)
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

        auto pairCmp = [](std::pair<std::string, int> &p1, std::pair<std::string, int> &p2)
        {
            return p1.first != p2.first ? p1.second > p2.second : p1.first > p2.first;
        };
        std::vector<std::pair<std::string, int>> freqPairs(frequency.begin(), frequency.end());
        std::sort(freqPairs.begin(), freqPairs.end(), pairCmp);

        this->save_pairs_to_csv(freqPairs, csvFile, wordsCount, maxColumns);
        resp.isError = false;
        return resp;
    }

    void TableStats::save_pairs_to_csv(const std::vector<std::pair<std::string, int>> &sortedPairs,
                                       std::fstream &destFile, size_t wordsCount, int maxColumns)
    {
        // int maxIter = min(maxColumns, sortedPairs.size());
        // for (int i = 0; i < maxIter; i++)
        // {
        //     destFile << sortedPairs[i].first << this->delim;
        // }
        // destFile << std::endl;

        // for (int i = 0; i < maxIter; i++)
        // {
        //     destFile << sortedPairs[i].second << this->delim;
        // }
        // destFile << std::endl;

        // for (int i = 0; i < maxIter; i++)
        // {
        //     destFile << std::setprecision(3) << (double)sortedPairs[i].second / wordsCount << this->delim;
        // }
        // destFile << std::endl;
        std::vector<std::string> columns = {"Word", "Count", "Frequency"};
        for (int i = 0; i < columns.size(); i++)
        {
            destFile << columns[i] << this->delim;
        }
        destFile << std::endl;

        size_t maxIter = min(maxColumns, sortedPairs.size());

        for (size_t i = 0; i < maxIter; ++i)
        {
            const auto &p = sortedPairs[i];
            double freq = wordsCount ? (double)(p.second) / wordsCount : 0.0;
            destFile << p.first << this->delim
                     << p.second << this->delim
                     << std::setprecision(3) << freq
                     << std::endl;
        }
    }

}