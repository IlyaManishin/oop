#pragma once

#include "tokenizer/tokenizer_api.h"
#include "types.hpp"

#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace file_parser
{
    class Parser
    {
    public:
        Parser(std::string filePath);
        ~Parser();

        FileUPtr parse();

    private:
        AssignUPtr parseAssign();
        FuncRunUPtr parseFuncRun();
        MethodRunUPtr parseMethodRun();
        IfStatUPtr parseIfStat();
        StatementUPtr parseStatement();
        std::vector<StatementUPtr> parseStatements();
        
        ArgUPtr argRule();
        ArgsUPtr readArgsRule();

        std::optional<std::string> identRule();
        std::optional<std::string> numberRule();
        std::optional<std::string> stringRule();

        void nextToken();
        int save();
        void rewind(int pos);
        bool checkType(TokenTypes type);
        bool accept(TokenTypes type);

    private:
        FILE *file = nullptr;
        std::string filePath;
        TTokenizer *tokenizer = nullptr;
        TToken current;
    };
}// namespace file_parser
