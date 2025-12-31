#include "parser.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    FileUPtr AstParser::parseFileRule()
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
            const std::string msg = std::string("Can't parse rule (line ") +
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

    StatementsUPtr AstParser::parseStatements()
    {
        StatementsUPtr stmts = std::make_unique<std::vector<StatementUPtr>>();
        StatementUPtr stmt = this->parseStatement();
        if (!stmt)
            return stmts;
        stmts->push_back(std::move(stmt));

        while (1)
        {
            auto pos = this->save();
            if (this->curTok.type == NEWLINE ||
                this->curTok.type == DEDENT ||
                this->acceptTok(NEWLINE))
            {
                if ((stmt = parseStatement()))
                {
                    stmts->push_back(std::move(stmt));
                    continue;
                }
            }
            this->rewind(pos);
            break;
        }
        return stmts;
    }

    StatementUPtr AstParser::parseStatement()
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

    StatementsUPtr AstParser::parseBlock()
    {
        int pos = this->save();

        FuncCallUPtr condition;
        StatementsUPtr stmts;
        if (acceptTok(INDENT) &&
            (stmts = parseStatements()) &&
            acceptTok(NEWLINE) && acceptTok(DEDENT))
        {
            return stmts;
        }

        this->rewind(pos);
        return nullptr;
    }

    IfStatUPtr AstParser::parseIfStat()
    {
        int pos = this->save();

        FuncCallUPtr condition;
        StatementsUPtr ifStmts;
        StatementsUPtr elseStmts;
        if (acceptTok(IF_KW) &&
            (condition = parseFuncCall()) &&
            acceptTok(COLON) && acceptTok(NEWLINE) &&
            (ifStmts = parseBlock()))
        {
            elseStmts = parseElseStat();
            return std::make_unique<IfStat>(IfStat{std::move(condition),
                                                   std::move(ifStmts), std::move(elseStmts)});
        }
        this->rewind(pos);
        return nullptr;
    }

    StatementsUPtr AstParser::parseElseStat()
    {
        int pos = save();

        StatementsUPtr stmts;
        if (acceptTok(ELSE_KW) && acceptTok(COLON) && acceptTok(NEWLINE) &&
            (stmts = parseBlock()))
        {
            return stmts;
        }
        rewind(pos);
        return nullptr;
    }

    AssignUPtr AstParser::parseAssign()
    {
        int pos = save();

        auto ident = identRule();
        FuncCallUPtr funcCall;

        if (ident &&
            acceptTok(ASSIGN) &&
            (funcCall = parseFuncCall()))
        {
            return file_parser::AssignUPtr(
                new file_parser::Assign(*ident, std::move(funcCall)));
        }
        this->rewind(pos);
        return nullptr;
    }

    MethodRunUPtr AstParser::parseMethodRun()
    {
        int pos = this->save();

        auto ident = this->identRule();
        FuncCallUPtr fCall;

        if (ident &&
            this->acceptTok(DOT) &&
            (fCall = this->parseFuncCall()))
        {
            return std::make_unique<MethodRun>(*ident, std::move(fCall));
        }

        this->rewind(pos);
        return nullptr;
    }

    FuncRunUPtr AstParser::parseFuncRun()
    {
        int pos = this->save();

        FuncCallUPtr fCall;
        if ((fCall = parseFuncCall()))
        {
            FuncRunUPtr res = std::make_unique<FuncRun>(std::move(fCall));
            return res;
        }
        this->rewind(pos);
        return nullptr;
    }

    FuncCallUPtr AstParser::parseFuncCall()
    {
        int pos = this->save();

        auto ident = this->identRule();
        ArgsUPtr args;
        if (ident &&
            this->acceptTok(LPAREN) &&
            (args = this->readArgsRule()) &&
            this->acceptTok(RPAREN))
        {
            return std::make_unique<FuncCall>(*ident, std::move(args));
        }

        this->rewind(pos);
        return nullptr;
    }

    ArgsUPtr AstParser::readArgsRule()
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

    ArgUPtr AstParser::argRule()
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

    std::optional<std::string> AstParser::identRule()
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
