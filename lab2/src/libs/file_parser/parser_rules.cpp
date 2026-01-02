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
                markError("file", this->curTok);
            }
            throw UnexpectedTokenExc(this->errMark.errorToken, this->errMark.ruleName);
        }

        if (statements)
            return std::make_unique<FileTree>(std::move(statements));

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
        TToken startTok = peekNextToken();
        int pos = savePos();

        if (auto ifstmt = parseIfStat())
            return std::make_unique<Statement>(
                Statement{IfStatUPtr(std::move(ifstmt)),
                          (size_t)startTok.lineno});

        rewind(pos);
        markError("compound_statement", startTok);
        return nullptr;
    }

    StatementUPtr AstParser::parseSimpleStmt()
    {
        TToken startTok = peekNextToken();
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

        rewind(pos);
        markError("simple_statement", startTok);
        return nullptr;
    }

    StatementsUPtr AstParser::parseBlock()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        StatementsUPtr stmts;
        if (acceptTok(INDENT) &&
            (stmts = parseStmts()) &&
            acceptTok(DEDENT))
        {
            return stmts;
        }

        rewind(pos);
        markError("block", startTok);
        return nullptr;
    }

    IfStatUPtr AstParser::parseIfStat()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        FuncCallUPtr condition;
        StatementsUPtr ifStmts;
        StatementsUPtr elseStmts;
        if (acceptTok(IF_KW) &&
            (condition = parseFuncCall()) &&
            acceptTok(COLON) && acceptTok(NEWLINE) &&
            (ifStmts = parseBlock()))
        {
            elseStmts = parseElseStat();
            return std::make_unique<IfStat>(std::move(condition),
                                            std::move(ifStmts),
                                            std::move(elseStmts));
        }

        rewind(pos);
        markError("if_statement", startTok);
        return nullptr;
    }

    StatementsUPtr AstParser::parseElseStat()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        StatementsUPtr stmts;
        if (acceptTok(ELSE_KW) &&
            acceptTok(COLON) && acceptTok(NEWLINE) &&
            (stmts = parseBlock()))
        {
            return stmts;
        }

        rewind(pos);
        markError("else_statement", startTok);
        return nullptr;
    }

    AssignUPtr AstParser::parseAssign()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

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

        rewind(pos);
        markError("assign", startTok);
        return nullptr;
    }

    MethodRunUPtr AstParser::parseMethodRun()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        auto ident = identRule();
        FuncCallUPtr fCall;

        if (ident &&
            acceptTok(DOT) &&
            (fCall = parseFuncCall()) &&
            acceptTok(NEWLINE))
        {
            return std::make_unique<MethodRun>(*ident, std::move(fCall));
        }

        rewind(pos);
        markError("method_run", startTok);
        return nullptr;
    }

    FuncRunUPtr AstParser::parseFuncRun()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        FuncCallUPtr fCall;
        if ((fCall = parseFuncCall()) && acceptTok(NEWLINE))
        {
            return std::make_unique<FuncRun>(std::move(fCall));
        }

        rewind(pos);
        markError("func_run", startTok);
        return nullptr;
    }

    FuncCallUPtr AstParser::parseFuncCall()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        auto ident = identRule();
        ArgsUPtr args;
        if (ident &&
            acceptTok(LPAREN) &&
            (args = readArgsRule()) &&
            acceptTok(RPAREN))
        {
            return std::make_unique<FuncCall>(*ident, std::move(args));
        }

        rewind(pos);
        markError("func_call", startTok);
        return nullptr;
    }

    ArgsUPtr AstParser::readArgsRule()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        ArgsUPtr args = std::make_unique<std::vector<ArgUPtr>>();
        bool hasComma = false;
        while (true)
        {
            auto arg = argRule();
            if (!arg)
                break;
            hasComma = false;
            args->push_back(std::move(arg));
            if (!acceptTok(COMMA))
                break;

            hasComma = true;
        }

        if (!hasComma)
            return args;

        rewind(pos);
        markError("arguments", startTok);
        return args;
    }

    ArgUPtr AstParser::argRule()
    {
        int pos = savePos();

        nextToken();
        TToken startTok = this->curTok;
        if (checkTokType(IDENT))
        {
            std::string v(curTok.start, curTok.end);
            return std::make_unique<Arg>(std::move(v), Arg::Type::IDENT);
        }

        if (checkTokType(NUMBER))
        {
            float v = std::stof(std::string(curTok.start, curTok.end));
            return std::make_unique<Arg>(v);
        }

        if (checkTokType(STRING))
        {
            std::string v(curTok.start, curTok.end);
            return std::make_unique<Arg>(std::move(v), Arg::Type::STRING);
        }

        if (checkTokType(TRUE_KW))
            return std::make_unique<Arg>(true);
        if (checkTokType(FALSE_KW))
            return std::make_unique<Arg>(false);

        rewind(pos);
        markError("argument", startTok);
        return nullptr;
    }

    std::optional<std::string> AstParser::identRule()
    {
        int pos = savePos();

        nextToken();
        if (checkTokType(IDENT))
        {
            std::string value(curTok.start, curTok.end);
            return value;
        }
        rewind(pos);
        return {};
    }
} // namespace file_parser
