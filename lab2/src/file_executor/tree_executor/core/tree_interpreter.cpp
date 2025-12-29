#include "tree_interpreter.hpp"
#include "helpers/exceptions.hpp"
#include "helpers/utils.hpp"

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
        // const FuncCallUPtr
    }

    std::unique_ptr<ExObjs> TreeInterpreter::parseArgs(const ArgsUPtr &args)
    {
        auto argsObj = std::make_unique<ExObjs>(args->size());
        for (size_t i = 0; i < args->size(); i++)
        {
            (*argsObj)[i] = this->parseArg((*args)[i]);
        }
        return argsObj;
    }

    ExObjUPtr TreeInterpreter::parseArg(const file_parser::ArgUPtr &arg)
    {
        if (arg->type == file_parser::Arg::Type::IDENT)
        {
            auto it = vars.find(std::get<std::string>(arg->value));
            if (it != vars.end())
            {
                return it->second ? std::make_unique<ExObj>(*it->second) : nullptr;
            }
            else
            {
                throw std::runtime_error("Undefined variable: " + std::get<std::string>(arg->value));
            }
        }
        else
        {
            ExObjUPtr res = value_arg_to_ex_obj(arg);
            if (!res)
                throw std::runtime_error("Unexpected type");
            return res;
        }
    }

    void TreeInterpreter::addVar(const std::string &name, ExObjUPtr value)
    {
    }
}
