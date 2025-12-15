#pragma once

#include "tokenizer/tokenizer_api.h"
#include "types.hpp"

#include <cstdio>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace file_parser
{
    class ParserException : std::runtime_error
    {
    public:
        explicit ParserException(const std::string &msg) : runtime_error(msg) {}
    };

    class Parser
    {
    public:
        Parser(std::string filePath);
        ~Parser();

        FileUPtr ParseFileTree();

    private:
        FILE *file = nullptr;
        bool isParserInit = false;

        std::string filePath;
        TTokenizer *tokenizer = nullptr;
        TToken curTok;

        FileUPtr parseFileRule();
        StatementsUPtr parseStatements();
        StatementUPtr parseStatement();
        AssignUPtr parseAssign();
        FuncRunUPtr parseFuncRun();
        MethodRunUPtr parseMethodRun();
        IfStatUPtr parseIfStat();

        ArgUPtr argRule();
        ArgsUPtr readArgsRule();

        std::optional<std::string> identRule();

        int save();
        void rewind(int pos);
        bool checkTokType(TokenTypes type);
        void nextToken();
        bool acceptTok(TokenTypes type);
        void readPassTokens();
        bool isEOF() { return curTok.type == EOF_TOKEN; };
        bool isErr() { return curTok.type == ERROR_TOKEN; };

        std::string tokToStr(TToken token);
    };
} // namespace file_parser
