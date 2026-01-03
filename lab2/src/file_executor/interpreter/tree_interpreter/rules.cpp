#include "core/exceptions.hpp"
#include "core/utils.hpp"
#include "tree_interpreter.hpp"

#include <variant>
#include <vector>

using namespace file_parser;

namespace tree_executor
{
    void TreeInterpreter::ExecuteTree(const FileUPtr &tree)
    {
        executeStatements(*tree->statements);
    }

    void TreeInterpreter::executeStatements(const file_parser::Statements &stmts)
    {
        for (const auto &stmt : stmts)
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
            executeIfStmt(*std::get<IfStatUPtr>(stmt.value));
    }

    void TreeInterpreter::executeAssign(const Assign &assign)
    {
        ExObjUPtr right = executeExpression(*assign.right);
        this->addVar(assign.ident, std::move(right));
    }

    void TreeInterpreter::executeFuncRun(const FuncRun &run)
    {
        executeFuncCall(*run.fCall);
    }

    void TreeInterpreter::executeMethodRun(const file_parser::MethodRun &method_run)
    {
        executeMethod(method_run.object, *method_run.call);
    }

    void TreeInterpreter::executeIfStmt(const IfStat &ifstat)
    {
        bool res = executePredicate(*ifstat.condition);

        if (res)
        {
            if (ifstat.ifStmts != nullptr)
                executeStatements(*ifstat.ifStmts);
        }
        else
        {
            if (ifstat.elseStmts != nullptr)
                executeStatements(*ifstat.elseStmts);
        }
    }

}
