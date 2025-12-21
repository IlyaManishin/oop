#include "executor.hpp"
#include "file_parser/parser.hpp"

#include <variant>

using namespace file_parser;

namespace file_executor
{
    void Executor::ExecuteTree(file_parser::FileUPtr tree)
    {
        Executor exec;
        for (const auto &stmt : tree->statements)
            exec.executeStatement(*stmt);
    }

    void Executor::executeStatement(const file_parser::Statement &stmt)
    {
        std::visit([this](auto &&node)
                   {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<T, file_parser::Assign>)
            executeAssign(node);
        else if constexpr (std::is_same_v<T, file_parser::FuncRun>)
            executeFuncRun(node);
        else if constexpr (std::is_same_v<T, file_parser::FuncCall>)
            executeFuncCall(node);
        else if constexpr (std::is_same_v<T, file_parser::MethodRun>)
            executeMethodRun(node);
        else if constexpr (std::is_same_v<T, file_parser::IfStat>)
            executeIfStat(node); }, stmt.node);
    }

    void Executor::executeAssign(const file_parser::Assign &)
    {
    }

    void Executor::executeFuncRun(const file_parser::FuncRun &)
    {
    }

    void Executor::executeFuncCall(const file_parser::FuncCall &)
    {
    }

    void Executor::executeMethodRun(const file_parser::MethodRun &)
    {
    }

    void Executor::executeIfStat(const file_parser::IfStat &)
    {
    }

} // namespace file_executor
