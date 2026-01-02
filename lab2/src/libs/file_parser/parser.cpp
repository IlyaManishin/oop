#include "parser.hpp"
#include "parser_exceptions.hpp"
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
        this->startTokPos = this->savePos();
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
            throw InvalidFileExc(this->filePath, "Can't open file to parse");
        if (this->isErr())
            throw InvalidFileExc(this->filePath, "Invalid file to parse");

        return this->parseFileRule();
    }

    void AstParser::nextToken()
    {
        this->curTok = token_soft_read(this->tokenizer);
        if (is_tokenizer_error(this->tokenizer))
        {
            TTokenizerError error = get_tokenizer_error(this->tokenizer);
            throw TokenizerExc(error);
        }
    }

    TToken AstParser::peekNextToken()
    {
        auto pos = this->savePos();
        this->nextToken();
        TToken token = this->curTok;

        this->rewind(pos);
        return token;
    }

    bool AstParser::acceptTok(TokenTypes type)
    {
        int pos = this->savePos();
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

    void AstParser::markError(const std::string &ruleName, TToken errToken)
    {
        if (errToken.lineno < 0)
            return;
        if (this->isErrMark)
        {
            if (this->errMark.lineno > errToken.lineno)
                return;
            if (this->errMark.lineno == errToken.lineno &&
                this->errMark.col > errToken.col)
                return;
        }
        this->isErrMark = true;
        this->errMark = SyntaxError(errToken, ruleName, errToken.lineno, errToken.col);
    }
} // namespace file_parser
