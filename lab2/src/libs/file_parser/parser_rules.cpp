#include "parser.hpp"
#include "parser_exceptions.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace file_parser
{
    FileUPtr AstParser::parseFileRule()
    {
        rewind(this->startTokPos);

        auto statements = this->parseStmts();

        this->readPassTokens();
        if (!acceptTok(EOF_TOKEN))
        {
            if (!this->isErrMark)
            {
                this->nextToken();
                throw UnexpectedTokenExc(this->curTok);
            }
        }
        if (statements)
        {
            return std::make_unique<FileTree>(std::move(statements));
        }
        return nullptr;
    }

    StatementsUPtr AstParser::parseStmts()
    {
        StatementsUPtr stmts = std::make_unique<std::vector<StatementUPtr>>();
        StatementUPtr stmt = this->parseStmt();
        if (!stmt)
            return nullptr;
        stmts->push_back(std::move(stmt));

        while (1)
        {
            auto pos = this->savePos();

            if ((stmt = parseStmt()))
            {
                stmts->push_back(std::move(stmt));
                continue;
            }

            this->rewind(pos);
            break;
        }
        return stmts;
    }

    StatementUPtr AstParser::parseStmt()
    {
        int pos = savePos();

        if (auto stmt = parseCompoundStmt())
            return stmt;

        rewind(pos);
        if (auto stmt = parseSimpleStmt())
            return stmt;

        return nullptr;
    }

    StatementUPtr AstParser::parseCompoundStmt()
    {
        TToken startTok = this->peekNextToken();
        int pos = savePos();

        if (auto ifstmt = parseIfStat())
            return std::make_unique<Statement>(
                Statement{IfStatUPtr(std::move(ifstmt)),
                          (size_t)startTok.lineno});

        rewind(pos);
        return nullptr;
    }

    StatementUPtr AstParser::parseSimpleStmt()
    {
        TToken startTok = this->peekNextToken();
        int pos = savePos();

        if (auto assign = parseAssign())
            return std::make_unique<Statement>(
                Statement{AssignUPtr(std::move(assign)),
                          (size_t)startTok.lineno});

        rewind(pos);
        if (auto func = parseFuncRun())
            return std::make_unique<Statement>(
                Statement{FuncRunUPtr(std::move(func)),
                          (size_t)startTok.lineno});

        rewind(pos);
        if (auto method = parseMethodRun())
            return std::make_unique<Statement>(
                Statement{MethodRunUPtr(std::move(method)),
                          (size_t)startTok.lineno});

        return nullptr;
    }

    StatementsUPtr AstParser::parseBlock()
    {
        int pos = this->savePos();

        FuncCallUPtr condition;
        StatementsUPtr stmts;
        if (acceptTok(INDENT) &&
            (stmts = parseStmts()) &&
            acceptTok(DEDENT))
        {
            return stmts;
        }

        this->rewind(pos);
        return nullptr;
    }

    IfStatUPtr AstParser::parseIfStat()
    {
        int pos = this->savePos();

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
        int pos = savePos();

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
        int pos = savePos();

        auto ident = identRule();
        FuncCallUPtr funcCall;

        if (ident &&
            acceptTok(ASSIGN) &&
            (funcCall = parseFuncCall()) &&
            acceptTok(NEWLINE))
        {
            return file_parser::AssignUPtr(
                new file_parser::Assign(*ident, std::move(funcCall)));
        }
        this->rewind(pos);
        return nullptr;
    }

    MethodRunUPtr AstParser::parseMethodRun()
    {
        int pos = this->savePos();

        auto ident = this->identRule();
        FuncCallUPtr fCall;

        if (ident &&
            acceptTok(DOT) &&
            (fCall = parseFuncCall()) &&
            acceptTok(NEWLINE))
        {
            return std::make_unique<MethodRun>(*ident, std::move(fCall));
        }

        this->rewind(pos);
        return nullptr;
    }

    FuncRunUPtr AstParser::parseFuncRun()
    {
        int pos = this->savePos();

        FuncCallUPtr fCall;
        if ((fCall = parseFuncCall()) && acceptTok(NEWLINE))
        {
            FuncRunUPtr res = std::make_unique<FuncRun>(std::move(fCall));
            return res;
        }
        this->rewind(pos);
        return nullptr;
    }

    FuncCallUPtr AstParser::parseFuncCall()
    {
        int pos = this->savePos();

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
        int pos = savePos();

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
        int pos = savePos();

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
