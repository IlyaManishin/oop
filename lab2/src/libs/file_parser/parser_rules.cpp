#include "parser.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    AssignUPtr Parser::parseAssign()
    {
        int pos = save();

        auto id = identRule();
        if (!id)
        {
            rewind(pos);
            return nullptr;
        }

        if (!accept(ASSIGN))
        {
            rewind(pos);
            return nullptr;
        }

        auto func = parseFuncRun();
        if (!func)
        {
            rewind(pos);
            return nullptr;
        }

        return file_parser::AssignUPtr(
            new file_parser::Assign(*id, std::move(func))
        );
    }

    FuncRunUPtr Parser::parseFuncRun()
    {
        int pos = save();

        auto id = identRule();
        if (!id)
        {
            rewind(pos);
            return nullptr;
        }

        if (!accept(LPAREN))
        {
            rewind(pos);
            return nullptr;
        }

        auto args = readArgsRule();

        if (!accept(RPAREN))
        {
            rewind(pos);
            return nullptr;
        }

        return file_parser::FuncRunUPtr(
            new file_parser::FuncRun(*id, std::move(args))
        );
    }

    MethodRunUPtr Parser::parseMethodRun()
    {
        int pos = save();

        auto ident = identRule();
        if (!ident)
        {
            rewind(pos);
            return nullptr;
        }

        if (!accept(DOT))
        {
            rewind(pos);
            return nullptr;
        }

        auto func = parseFuncRun();
        if (!func)
        {
            rewind(pos);
            return nullptr;
        }

        return file_parser::MethodRunUPtr(
            new file_parser::MethodRun(*ident, std::move(func))
        );
    }

    std::vector<ArgUPtr> Parser::readArgsRule()
    {
        std::vector<file_parser::ArgUPtr> args;
        while (true)
        {
            auto arg = argRule();
            if (!arg)
                break;
            args.push_back(std::move(arg));
            if (!accept(COMMA))
                break;
        }
        return args;
    }

    ArgUPtr Parser::argRule()
    {
        int pos = save();

        if (checkType(IDENT))
        {
            std::string v(current.start, current.end);
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::IDENT)
            );
        }

        if (checkType(NUMBER))
        {
            float v = std::stof(std::string(current.start, current.end));
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(v)
            );
        }

        if (checkType(STRING))
        {
            std::string v(current.start, current.end);
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::STRING)
            );
        }

        rewind(pos);
        return nullptr;
    }

    std::optional<std::string> Parser::identRule()
    {
        int pos = save();

        if (!checkType(IDENT))
        {
            rewind(pos);
            return {};
        }

        std::string value(current.start, current.end);
        nextToken();
        return value;
    }
} // namespace file_parser
