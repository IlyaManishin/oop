#pragma once

#include "tokenizer/tokenizer_api.h"
#include "types.hpp"

#include <cstdio>
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

        void parse();

    private:
        Assign *parseAssign();
        FuncRun *parseFuncRun();
        MethodRun *parseMethodRun();
        std::vector<std::variant<Assign, FuncRun, MethodRun>> parseStatements();

        std::vector<Arg> readArgsRule();
        std::optional<Arg> argRule();

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
} // namespace file_parser
