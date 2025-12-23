#include "executor.hpp"
#include "file_parser/parser.hpp"

#include <variant>

using namespace file_parser;

namespace file_executor
{
    void Executor::ExecuteTree(const FileUPtr& tree)
    {
        Executor exec;
        for (const auto &stmt : *(tree->statements))
            exec.executeStatement(*stmt);
    }

    void Executor::executeStatement(const Statement &stmt)
    {
        if (std::holds_alternative<AssignUPtr>(stmt.value))
            executeAssign(*std::get<AssignUPtr>(stmt.value));
        else if (std::holds_alternative<FuncRunUPtr>(stmt.value))
            executeFuncRun(*std::get<FuncRunUPtr>(stmt.value));
        else if (std::holds_alternative<MethodRunUPtr>(stmt.value))
            executeMethodRun(*std::get<MethodRunUPtr>(stmt.value));
        else if (std::holds_alternative<IfStatUPtr>(stmt.value))
            executeIfStat(*std::get<IfStatUPtr>(stmt.value));
    }

    void Executor::executeAssign(const Assign &assign)
    {
        executeFuncCall(*assign.right);
    }

    void Executor::executeFuncRun(const FuncRun &run)
    {
        executeFuncCall(*run.fCall);
    }

    void Executor::executeFuncCall(const FuncCall &call)
    {

    }

    void Executor::executeMethodRun(const MethodRun &method)
    {
        executeFuncCall(*method.call);
    }

    void Executor::executeIfStat(const IfStat &ifstat)
    {
        executeFuncCall(*ifstat.condition);

        for (const auto &stmt : *ifstat.statements)
            executeStatement(*stmt);
    }

} // namespace file_executor
