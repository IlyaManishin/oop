#pragma once

#include <string>
#include <variant>
#include <vector>

namespace file_parser
{
    struct Arg
    {
        std::variant<std::string, float> value;
        enum class Type
        {
            IDENT,
            NUMBER,
            STRING
        } type;
    };

    struct FuncRun
    {
        std::string name;
        std::vector<Arg> args;
    };

    struct Assign
    {
        std::string ident;
        FuncRun rhs;
    };

    struct MethodRun
    {
        std::string object;
        FuncRun call;
    };

    using Statement = std::variant<Assign, FuncRun, MethodRun>;

    struct File
    {
        std::vector<Statement> statements;
    };

} // namespace file_parser
