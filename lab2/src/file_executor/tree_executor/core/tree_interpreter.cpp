#include "tree_interpreter.hpp"

#include <variant>

using namespace file_parser;

namespace tree_executor
{
    void TreeInterpreter::Execute(const FileUPtr &tree)
    {
        for (const auto &stmt : *(tree->statements))
            executeStatement(*stmt);
    }

    void TreeInterpreter::executeStatement(const Statement &stmt)
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

    void TreeInterpreter::executeAssign(const Assign &assign)
    {
        executeFuncCall(*assign.right);
    }

    void TreeInterpreter::executeFuncRun(const FuncRun &run)
    {
        executeFuncCall(*run.fCall);
    }

    void TreeInterpreter::executeFuncCall(const FuncCall &call)
    {
    }

    void TreeInterpreter::executeMethodRun(const MethodRun &method)
    {
    }

    void TreeInterpreter::executeIfStat(const IfStat &ifstat)
    {
        executeFuncCall(*ifstat.condition);

        for (const auto &stmt : *ifstat.statements)
            executeStatement(*stmt);
    }
}
