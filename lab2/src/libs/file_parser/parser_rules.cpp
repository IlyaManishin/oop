#include "parser.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    FileUPtr Parser::parseFileRule()
    {
        int pos = this->save();
        auto statements = this->parseStatements();
        if (this->isErr())
        {
            throw ParserException("Can't parse file");
        }

        this->readPassTokens();
        if (!this->isEOF())
        {
            this->nextToken();
            const std::string msg = std::string("Can't parse rule(line ") +
                                    std::to_string(this->curTok.lineno) +
                                    std::string(")\n");
            if (token_strlen(this->curTok) != 0)
            {
                const std::string tokenStr = std::string(this->curTok.start, token_strlen(this->curTok));
                const std::string tokenMsg = std::string("invalid token - ") + tokenStr;
                throw ParserException(msg + tokenMsg);
            }
            else
                throw ParserException(msg);
        }
        if (statements)
        {
            return std::make_unique<FileTree>(std::move(statements));
        }

        this->rewind(pos);
        return nullptr;
    }

    StatementsUPtr Parser::parseStatements()
    {
        StatementsUPtr stmts = std::make_unique<std::vector<StatementUPtr>>();
        while (auto stmt = this->parseStatement())
            stmts->push_back(std::move(stmt));
        return stmts;
    }

    StatementUPtr Parser::parseStatement()
    {
        int pos = this->save();

        if (auto ifstmt = this->parseIfStat())
            return std::make_unique<Statement>(Statement{IfStatUPtr(std::move(ifstmt)),
                                                         (size_t)this->curTok.lineno});

        this->rewind(pos);
        if (auto assign = this->parseAssign())
            return std::make_unique<Statement>(Statement{AssignUPtr(std::move(assign)),
                                                         (size_t)this->curTok.lineno});

        this->rewind(pos);
        if (auto func = this->parseFuncRun())
            return std::make_unique<Statement>(Statement{FuncRunUPtr(std::move(func)),
                                                         (size_t)this->curTok.lineno});

        this->rewind(pos);
        if (auto method = this->parseMethodRun())
            return std::make_unique<Statement>(Statement{MethodRunUPtr(std::move(method)),
                                                         (size_t)this->curTok.lineno});
        return nullptr;
    }

    IfStatUPtr Parser::parseIfStat()
    {
        int pos = this->save();

        auto condition = this->parseFuncRun();
        if (!condition)
        {
            this->rewind(pos);
            return nullptr;
        }

        auto body = this->parseStatements();
        return std::make_unique<IfStat>(IfStat{std::move(condition), std::move(body)});
    }

    AssignUPtr Parser::parseAssign()
    {
        int pos = save();

        auto ident = identRule();
        FuncRunUPtr funcRun;

        if (ident &&
            acceptTok(ASSIGN) &&
            (funcRun = parseFuncRun()))
        {
            return file_parser::AssignUPtr(
                new file_parser::Assign(*ident, std::move(funcRun)));
        }
        this->rewind(pos);
        return nullptr;
    }

    FuncRunUPtr Parser::parseFuncRun()
    {
        int pos = this->save();

        auto ident = this->identRule();
        ArgsUPtr args;
        if (ident &&
            this->acceptTok(LPAREN) &&
            (args = this->readArgsRule()) &&
            this->acceptTok(RPAREN))
        {
            return std::make_unique<FuncRun>(*ident, std::move(args));
        }

        this->rewind(pos);
        return nullptr;
    }

    MethodRunUPtr Parser::parseMethodRun()
    {
        int pos = this->save();

        auto ident = this->identRule();
        FuncRunUPtr func;

        if (ident &&
            this->acceptTok(DOT) &&
            (func = this->parseFuncRun()))
        {
            return std::make_unique<MethodRun>(*ident, std::move(func));
        }

        this->rewind(pos);
        return nullptr;
    }

    ArgsUPtr Parser::readArgsRule()
    {
        ArgsUPtr args = std::make_unique<std::vector<ArgUPtr>>();
        while (true)
        {
            auto arg = argRule();
            if (!arg)
                break;
            args->push_back(std::move(arg));
            if (!acceptTok(COMMA))
                break;
        }
        return args;
    }

    ArgUPtr Parser::argRule()
    {
        int pos = save();

        nextToken();
        if (checkTokType(IDENT))
        {
            std::string v(curTok.start, curTok.end);
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::IDENT));
        }

        if (checkTokType(NUMBER))
        {
            float v = std::stof(std::string(curTok.start, curTok.end));
            return file_parser::ArgUPtr(
                new file_parser::Arg(v));
        }

        if (checkTokType(STRING))
        {
            std::string v(curTok.start, curTok.end);
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::STRING));
        }

        rewind(pos);
        return nullptr;
    }

    std::optional<std::string> Parser::identRule()
    {
        int pos = save();

        nextToken();
        if (!checkTokType(IDENT))
        {
            rewind(pos);
            return {};
        }

        std::string value(curTok.start, curTok.end);
        return value;
    }
} // namespace file_parser
