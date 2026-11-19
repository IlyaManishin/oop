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
    class AstParser
    {
    public:
        AstParser(std::string filePath);
        ~AstParser();

        FileUPtr ParseFileTree();

    private:
        class SyntaxError
        {
        public:
            TToken errorToken;
            std::string ruleName;
            int lineno;
            int col;
            SyntaxError() {};
            SyntaxError(const TToken &tok, const std::string &rule, int line, int column)
                : errorToken(tok), ruleName(rule), lineno(line), col(column) {};
        };
        bool isErrMark = false;
        SyntaxError errMark;

        FILE *file = nullptr;

        std::string filePath;

        TTokenizer *tokenizer = nullptr;
        int startTokPos;
        TToken curTok;

        FileUPtr parseFileRule();

        StatementsUPtr parseStmts();
        StatementUPtr parseStmt();
        StatementUPtr parseSimpleStmt();
        StatementUPtr parseCompoundStmt();
        ExpressionUPtr parseExpression();
        AssignUPtr parseAssign();
        FuncRunUPtr parseFuncRun();

        MethodCallUPtr parseMethodCall();
        FuncCallUPtr parseFuncCall();
        MethodRunUPtr parseMethodRun();
        IfStatUPtr parseIfStat();
        StatementsUPtr parseElseStat();
        StatementsUPtr parseBlock();

        ArgsUPtr parseArgs();
        ArgUPtr parseArg();
        ArgUPtr parseStringArg(TToken& token);
        ArgUPtr parseBoolArg(TToken& token);

        std::optional<std::string> parseIdent();

        void throwUnexpectedTokenExc(TToken errTok);

        int savePos() { return get_tokenizer_pos(tokenizer); };
        void rewind(int pos) { set_tokenizer_pos(tokenizer, pos); };
        bool checkTokType(TokenTypes type) { return this->curTok.type == type; };

        void nextToken();
        TToken peekNextToken();
        bool acceptTok(TokenTypes type);
        void readPassTokens();
        void markError(const std::string &ruleName, TToken errToken);

        bool isEOF() { return acceptTok(EOF_TOKEN); };
        bool isErr() { return is_tokenizer_error(this->tokenizer); };
    };
} // namespace file_parser
