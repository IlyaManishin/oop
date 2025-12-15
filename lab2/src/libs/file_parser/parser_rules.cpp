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
        if (!this->isEOF())
        {
            const std::string msg = std::string("Can't parse rule(line ") +
                                    std::to_string(this->curTok.lineno) +
                                    std::string(")\n");
            const std::string tokenStr = std::string(this->curTok.start, token_strlen(this->curTok));
            const std::string errMsg = std::string("invalid token - ") + tokenStr;
            throw ParserException(errMsg);
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
            accept(ASSIGN) &&
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
            this->accept(LPAREN) &&
            (args = this->readArgsRule()) &&
            this->accept(RPAREN))
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
            this->accept(DOT) &&
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
            std::string v(curTok.start, curTok.end);
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::IDENT));
        }

        if (checkType(NUMBER))
        {
            float v = std::stof(std::string(curTok.start, curTok.end));
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(v));
        }

        if (checkType(STRING))
        {
            std::string v(curTok.start, curTok.end);
            nextToken();
            return file_parser::ArgUPtr(
                new file_parser::Arg(std::move(v), file_parser::Arg::Type::STRING));
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

        std::string value(curTok.start, curTok.end);
        nextToken();
        return value;
    }
} // namespace file_parser
