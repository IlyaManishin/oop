#include "parser.hpp"
#include "types.hpp"

#include <string>

namespace file_parser
{
    Parser::Parser(std::string filePath)
        : filePath(filePath)
    {
        this->file = fopen(this->filePath.c_str(), "r");
        this->tokenizer = tokenizer_from_file_data(this->file);
        this->nextToken();
    }

    Parser::~Parser()
    {
        if (this->tokenizer)
            delete_tokenizer(this->tokenizer);
        if (this->file)
            fclose(this->file);
    }

    void Parser::nextToken()
    {
        this->current = token_soft_read(this->tokenizer);
    }

    int Parser::save()
    {
        return get_tokenizer_pos(this->tokenizer);
    }

    void Parser::rewind(int pos)
    {
        set_tokenizer_pos(this->tokenizer, pos);
        this->nextToken();
    }

    bool Parser::checkType(TokenTypes type)
    {
        return this->current.type == type;
    }

    bool Parser::accept(TokenTypes type)
    {
        if (!this->checkType(type))
            return false;
        this->nextToken();
        return true;
    }

    Assign *Parser::parseAssign()
    {
        int pos = this->save();

        auto id = this->identRule();
        if (!id)
        {
            this->rewind(pos);
            return nullptr;
        }

        if (!this->accept(ASSIGN))
        {
            this->rewind(pos);
            return nullptr;
        }

        auto func = this->parseFuncRun();
        if (!func)
        {
            this->rewind(pos);
            return nullptr;
        }

        return new Assign{*id, *func};
    }

    FuncRun *Parser::parseFuncRun()
    {
        int pos = this->save();

        auto id = this->identRule();
        if (!id)
        {
            this->rewind(pos);
            return nullptr;
        }

        if (!this->accept(LPAREN))
        {
            this->rewind(pos);
            return nullptr;
        }

        auto args = this->readArgsRule();

        if (!this->accept(RPAREN))
        {
            this->rewind(pos);
            return nullptr;
        }

        return new FuncRun{*id, args};
    }

    MethodRun *Parser::parseMethodRun()
    {
        int pos = this->save();

        auto id = this->identRule();
        if (!id)
        {
            this->rewind(pos);
            return nullptr;
        }

        if (!this->accept(DOT))
        {
            this->rewind(pos);
            return nullptr;
        }

        auto func = this->parseFuncRun();
        if (!func)
        {
            this->rewind(pos);
            return nullptr;
        }

        return new MethodRun{*id, *func};
    }

    std::vector<Arg> Parser::readArgsRule()
    {
        std::vector<Arg> args;
        while (auto arg = this->argRule()){
            args.push_back(arg.value());
        }
            //????
        return args;
    }

    std::optional<Arg> Parser::argRule()
    {
        return std::optional<Arg>();
    }

    std::optional<std::string> Parser::identRule()
    {
        int pos = this->save();
        if (!this->checkType(IDENT))
        {
            this->rewind(pos);
            return {};
        }
        std::string value(this->current.start, this->current.end);
        this->nextToken();
        return value;
    }
}
