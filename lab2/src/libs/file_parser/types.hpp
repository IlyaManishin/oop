#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace file_parser
{
    struct Arg;
    struct FuncCall;
    struct FuncRun;
    struct Assign;
    struct MethodRun;
    struct IfStat;
    struct Statement;
    struct FileTree;

    using ArgUPtr = std::unique_ptr<Arg>;
    using ArgsUPtr = std::unique_ptr<std::vector<ArgUPtr>>;
    using FuncCallUPtr = std::unique_ptr<FuncCall>;
    using FuncRunUPtr = std::unique_ptr<FuncRun>;
    using AssignUPtr = std::unique_ptr<Assign>;
    using MethodRunUPtr = std::unique_ptr<MethodRun>;
    using IfStatUPtr = std::unique_ptr<IfStat>;
    using StatementUPtr = std::unique_ptr<Statement>;
    using StatementsUPtr = std::unique_ptr<std::vector<StatementUPtr>>;

    using FileUPtr = std::unique_ptr<FileTree>;

    using StatementVars = std::variant<AssignUPtr, FuncRunUPtr, MethodRunUPtr, IfStatUPtr>;

    struct Arg
    {
        std::variant<std::string, float> value;
        enum class Type
        {
            IDENT,
            NUMBER,
            STRING
        } type;

        Arg(std::string v, Type t)
            : value(std::move(v)), type(t) {}

        Arg(float v)
            : value(v), type(Type::NUMBER) {}
    };

    struct FuncCall
    {
        std::string name;
        ArgsUPtr args;

        FuncCall(std::string name, ArgsUPtr args)
            : name(std::move(name)), args(std::move(args)) {}
    };

    struct FuncRun
    {
        FuncCallUPtr fCall;

        FuncRun(FuncCallUPtr fCall)
            : fCall(std::move(fCall)) {}
    };

    struct Assign
    {
        std::string ident;
        FuncCallUPtr right;

        Assign(std::string ident, FuncCallUPtr right)
            : ident(std::move(ident)), right(std::move(right)) {}
    };

    struct MethodRun
    {
        std::string object;
        FuncCallUPtr call;

        MethodRun(std::string object, FuncCallUPtr call)
            : object(std::move(object)), call(std::move(call)) {}
    };

    struct IfStat
    {
        FuncCallUPtr condition;
        StatementsUPtr ifStmts;
        StatementsUPtr elseStmts;

        IfStat(FuncCallUPtr condition, StatementsUPtr ifStatements, StatementsUPtr elseStatements)
            : condition(std::move(condition)),
              ifStmts(std::move(ifStatements)),
              elseStmts(std::move(elseStatements)) {};
    };

    struct Statement
    {
        StatementVars value;
        size_t line;

        Statement(AssignUPtr v, size_t line)
            : value(std::move(v)), line(line) {}

        Statement(FuncRunUPtr v, size_t line)
            : value(std::move(v)), line(line) {}

        Statement(MethodRunUPtr v, size_t line)
            : value(std::move(v)), line(line) {}

        Statement(IfStatUPtr v, 
            size_t line)
            : value(std::move(v)), line(line) {}
    };

    struct FileTree
    {
        StatementsUPtr statements;

        FileTree(StatementsUPtr statements)
            : statements(std::move(statements)) {}
    };

} // namespace file_parser
