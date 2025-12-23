#include "parser.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    Parser::Parser(std::string filePath)
        : filePath(std::move(filePath))
    {
        this->file = fopen(this->filePath.c_str(), "r");
        if (!this->file)
            return;

        this->tokenizer = tokenizer_from_file_data(file);
    }

    Parser::~Parser()
    {
        if (this->tokenizer)
            delete_tokenizer(this->tokenizer);
        if (this->file)
            fclose(this->file);
    }

    FileUPtr Parser::ParseFileTree()
    {
        if (this->file == NULL)
            throw ParserException("Can't open file");
        if (this->isErr())
            throw ParserException("Invalid file to parse");

        return this->parseFileRule();
    }

    void Parser::nextToken()
    {
        this->curTok = token_soft_read(this->tokenizer);
        if (is_tokenizer_error(this->tokenizer))
        {
            TTokenizerError error = get_tokenizer_error(this->tokenizer);
            char *errMsg = tokenizer_error_to_str(error);

            auto exc = ParserException(std::string(errMsg));
            free(errMsg);
            throw exc;
        }
    }

    int Parser::save()
    {
        return get_tokenizer_pos(tokenizer);
    }

    void Parser::rewind(int pos)
    {
        set_tokenizer_pos(tokenizer, pos);
    }

    bool Parser::checkTokType(TokenTypes type)
    {
        return curTok.type == type;
    }

    bool Parser::acceptTok(TokenTypes type)
    {
        int pos = this->save();
        this->nextToken();
        if (!this->checkTokType(type))
        {
            this->rewind(pos);
            return false;
        }
        return true;
    }

    void Parser::readPassTokens()
    {
        while (acceptTok(NEWLINE))
        {
            continue;
        }
    }

    std::string Parser::tokToStr(TToken token)
    {
        if (token.start == NULL)
        {
            return std::string();
        }
        return std::string(token.start, token_strlen(token));
    }
} // namespace file_parser
