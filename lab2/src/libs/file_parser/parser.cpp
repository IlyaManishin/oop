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
        file = fopen(this->filePath.c_str(), "r");
        tokenizer = tokenizer_from_file_data(file);
        nextToken();
    }

    Parser::~Parser()
    {
        if (tokenizer)
            delete_tokenizer(tokenizer);
        if (file)
            fclose(file);
    }

    void Parser::nextToken()
    {
        current = token_soft_read(tokenizer);
    }

    int Parser::save()
    {
        return get_tokenizer_pos(tokenizer);
    }

    void Parser::rewind(int pos)
    {
        set_tokenizer_pos(tokenizer, pos);
        nextToken();
    }

    bool Parser::checkType(TokenTypes type)
    {
        return current.type == type;
    }

    bool Parser::accept(TokenTypes type)
    {
        if (!checkType(type))
            return false;
        nextToken();
        return true;
    }
} // namespace file_parser
