#include "parser.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    AstParser::AstParser(std::string filePath)
        : filePath(std::move(filePath))
    {
        this->file = fopen(this->filePath.c_str(), "r");
        if (!this->file)
            return;

        this->tokenizer = tokenizer_from_file_data(file);
    }

    AstParser::~AstParser()
    {
        if (this->tokenizer)
            delete_tokenizer(this->tokenizer);
        if (this->file)
            fclose(this->file);
    }

    FileUPtr AstParser::ParseFileTree()
    {
        if (this->file == NULL)
            throw ParserException("Can't open file");
        if (this->isErr())
            throw ParserException("Invalid file to parse");

        return this->parseFileRule();
    }

    void AstParser::nextToken()
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

    int AstParser::save()
    {
        return get_tokenizer_pos(tokenizer);
    }

    void AstParser::rewind(int pos)
    {
        set_tokenizer_pos(tokenizer, pos);
    }

    bool AstParser::checkTokType(TokenTypes type)
    {
        return curTok.type == type;
    }

    bool AstParser::acceptTok(TokenTypes type)
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

    void AstParser::readPassTokens()
    {
        while (acceptTok(NEWLINE))
        {
            continue;
        }
    }

    std::string AstParser::tokToStr(TToken token)
    {
        if (token.start == NULL)
        {
            return std::string();
        }
        return std::string(token.start, token_strlen(token));
    }
} // namespace file_parser
