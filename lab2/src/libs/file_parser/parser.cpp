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
        this->tokenizer = tokenizer_from_file_data(file);
        this->nextToken();
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
        this->nextToken();
    }

    bool Parser::checkType(TokenTypes type)
    {
        return curTok.type == type;
    }

    bool Parser::accept(TokenTypes type)
    {
        if (!checkType(type))
            return false;
        nextToken();
        return true;
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
