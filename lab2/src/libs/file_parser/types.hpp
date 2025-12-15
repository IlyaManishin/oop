#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace file_parser
{
    struct Arg;
    struct FuncRun;
    struct Assign;
    struct MethodRun;
    struct IfStat;
    struct Statement;
    struct FileTree;

    using ArgUPtr = std::unique_ptr<Arg>;
    using ArgsUPtr = std::unique_ptr<std::vector<ArgUPtr>>;
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

    struct FuncRun
    {
        std::string name;
        ArgsUPtr args;

        FuncRun(std::string name, ArgsUPtr args)
            : name(std::move(name)), args(std::move(args)) {}
    };

    struct Assign
    {
        std::string ident;
        FuncRunUPtr right;

        Assign(std::string ident, FuncRunUPtr right)
            : ident(std::move(ident)), right(std::move(right)) {}
    };

    struct MethodRun
    {
        std::string object;
        FuncRunUPtr call;

        MethodRun(std::string object, FuncRunUPtr call)
            : object(std::move(object)), call(std::move(call)) {}
    };

    struct IfStat
    {
        FuncRunUPtr condition;
        StatementsUPtr statements;

        IfStat(FuncRunUPtr condition, StatementsUPtr statements)
            : condition(std::move(condition)), statements(std::move(statements)) {}
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

        Statement(IfStatUPtr v, size_t line)
            : value(std::move(v)), line(line) {}
    };

    struct FileTree
    {
        StatementsUPtr statements;

        FileTree(StatementsUPtr statements)
            : statements(std::move(statements)) {}
    };

} // namespace file_parser
