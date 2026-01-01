#pragma once

#include "tokenizer/tokenizer_api.h" //?????
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
    class ParserException : public std::runtime_error
    {
    public:
        explicit ParserException(const std::string &msg) : runtime_error(msg) {}
    };

    class AstParser
    {
    public:
        AstParser(std::string filePath);
        ~AstParser();

        FileUPtr ParseFileTree();

    private:
        FILE *file = nullptr;

        std::string filePath;
        TTokenizer *tokenizer = nullptr;
        TToken curTok;

        FileUPtr parseFileRule();

        StatementsUPtr parseStmts();
        StatementUPtr parseStmt();
        StatementUPtr parseSimpleStmt();
        StatementUPtr parseCompoundStmt();
        StatementsUPtr parseBlock();
        AssignUPtr parseAssign();
        FuncRunUPtr parseFuncRun();
        FuncCallUPtr parseFuncCall();
        MethodRunUPtr parseMethodRun();
        IfStatUPtr parseIfStat();
        StatementsUPtr parseElseStat();

        ArgUPtr argRule();
        ArgsUPtr readArgsRule();

        std::optional<std::string> identRule();

        void unexpectedNextToken();

        int save() { return get_tokenizer_pos(tokenizer); };
        void rewind(int pos) { set_tokenizer_pos(tokenizer, pos); };
        bool checkTokType(TokenTypes type) { return this->curTok.type == type; };

        void nextToken();
        TToken peekNextToken();
        bool acceptTok(TokenTypes type);
        void readPassTokens();

        bool isEOF() { return acceptTok(EOF_TOKEN); };
        bool isErr() { return curTok.type == ERROR_TOKEN; };

        std::string tokToStr(TToken token);
    };
} // namespace file_parser
