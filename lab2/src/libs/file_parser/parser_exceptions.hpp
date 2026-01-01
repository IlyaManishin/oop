#pragma once

#include "tokenizer/tokenizer_api.h"

#include <stdexcept>
#include <string>

namespace file_parser
{
    class ParserException : public std::runtime_error
    {
    public:
        explicit ParserException(const std::string &msg) : runtime_error(msg) {}
    };

    class InvalidFileExc : public ParserException
    {
    public:
        InvalidFileExc(const std::string &path,
                       const std::string msg = "Invalid script file")
            : ParserException(msg + ", script path: " + path) {};
    };

    class UnexpectedTokenExc : public ParserException
    {
    public:
        UnexpectedTokenExc(const TToken &tok, const std::string &ruleName = "rule")
            : ParserException(buildMessage(tok, ruleName)) {};

    private:
        static std::string buildMessage(const TToken &tok, const std::string &ruleName)
        {
            std::string errMsg = "Can't parse " + ruleName;
            if (tok.lineno >= 0)
                errMsg += " (line " + std::to_string(tok.lineno + 1) + ")";
            if (token_strlen(tok) != 0)
            {
                errMsg += "\ninvalid token: ";
                if (tok.type == NEWLINE)
                    errMsg += "new_line";
                else
                    errMsg += std::string(tok.start, token_strlen(tok));
            }
            return errMsg;
        }
    };

    class TokenizerExc : public ParserException
    {
    public:
        explicit TokenizerExc(TTokenizerError error)
            : ParserException(buildMessage(error)) {}

    private:
        static std::string buildMessage(TTokenizerError error)
        {
            char *errMsg = tokenizer_error_to_str(error);
            std::string msg(errMsg);
            free(errMsg);
            return msg;
        }
    };

} // namespace file_parser
