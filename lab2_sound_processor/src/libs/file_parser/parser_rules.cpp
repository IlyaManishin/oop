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

        rewind(pos);
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

        auto ident = parseIdent();
        ExpressionUPtr expr;

        if (ident &&
            acceptTok(ASSIGN) &&
            (expr = parseExpression()) &&
            acceptTok(NEWLINE))
        {
            return file_parser::AssignUPtr(
                new file_parser::Assign(*ident, std::move(expr)));
        }

        rewind(pos);
        markError("assign", startTok);
        return nullptr;
    }

    ExpressionUPtr AstParser::parseExpression()
    {
        TToken startTok = peekNextToken();
        int pos = savePos();

        if (auto fCall = parseFuncCall())
            return std::make_unique<Expression>(std::move(fCall));

        rewind(pos);
        if (auto methodCall = parseMethodCall())
            return std::make_unique<Expression>(std::move(methodCall));

        rewind(pos);
        if (auto arg = parseArg())
            return std::make_unique<Expression>(std::move(arg));

        rewind(pos);
        markError("expression", startTok);
        return nullptr;
    }

    MethodRunUPtr AstParser::parseMethodRun()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        MethodCallUPtr methodCall;
        if ((methodCall = parseMethodCall()) &&
            acceptTok(NEWLINE))
        {
            return std::make_unique<MethodRun>(std::move(methodCall));
        }

        rewind(pos);
        markError("method_run", startTok);
        return nullptr;
    }

    MethodCallUPtr AstParser::parseMethodCall()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        auto ident = parseIdent();
        FuncCallUPtr fCall;

        if (ident &&
            acceptTok(DOT) &&
            (fCall = parseFuncCall()))
        {
            return std::make_unique<MethodCall>(*ident, std::move(fCall));
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

        auto ident = parseIdent();
        ArgsUPtr args;
        if (ident &&
            acceptTok(LPAREN) &&
            (args = parseArgs()) &&
            acceptTok(RPAREN))
        {
            return std::make_unique<FuncCall>(*ident, std::move(args));
        }

        rewind(pos);
        markError("func_call", startTok);
        return nullptr;
    }

    ArgsUPtr AstParser::parseArgs()
    {
        int pos = savePos();
        TToken startTok = peekNextToken();

        ArgsUPtr args = std::make_unique<std::vector<ArgUPtr>>();
        bool hasComma = false;
        while (true)
        {
            auto arg = parseArg();
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

    ArgUPtr AstParser::parseArg()
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
            return parseStringArg(this->curTok);
        }
        if (checkTokType(TRUE_KW) || checkTokType(FALSE_KW))
        {
            return parseBoolArg(this->curTok);
        }

        rewind(pos);
        markError("argument", startTok);
        return nullptr;
    }

    ArgUPtr AstParser::parseStringArg(TToken &token)
    {
        if (token.type != STRING)
            return nullptr;

        std::string v(token.start, token.end);
        size_t pos = 0;
        while ((pos = v.find("\\n", pos)) != std::string::npos)
        {
            v.replace(pos, 2, "\n");
            pos += 1;
        }
        return std::make_unique<Arg>(std::move(v), Arg::Type::STRING);
    }

    ArgUPtr AstParser::parseBoolArg(TToken &token)
    {
        if (token.type == TRUE_KW)
            return std::make_unique<Arg>(true);
        if (token.type == FALSE_KW)
            return std::make_unique<Arg>(false);
        return nullptr;
    }

    std::optional<std::string> AstParser::parseIdent()
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
