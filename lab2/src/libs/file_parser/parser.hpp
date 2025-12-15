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

        void nextToken();
        int save();
        void rewind(int pos);
        bool checkType(TokenTypes type);
        bool accept(TokenTypes type);
        bool isEOF() { return curTok.type == EOF_TOKEN; };
        bool isErr() { return curTok.type == ERROR_TOKEN; };

        std::string tokToStr(TToken token);

    private:
        FILE *file = nullptr;
        
        std::string filePath;
        TTokenizer *tokenizer = nullptr;
        TToken curTok;
    };
} // namespace file_parser
