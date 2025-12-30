#include "tree_interpreter.hpp"
#include "helpers/exceptions.hpp"
#include "helpers/utils.hpp"

#include <variant>
#include <vector>

using namespace file_parser;

namespace tree_executor
{
    class ArgsFrame
    {
    public:
        std::vector<ExObjRef> allArgs;
        ExObjs valueArgs;
    };

    void TreeInterpreter::addVar(const std::string &name, ExObjUPtr value)
    {
        
    }

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

    ArgsFrameUPtr TreeInterpreter::parseArgs(const ArgsUPtr &args)
    {
        ArgsFrameUPtr argsFrame = std::make_unique<ArgsFrame>();
        for (size_t i = 0; i < args->size(); i++)
        {
            this->parseArg(argsFrame, (*args)[i]);
        }
        return argsFrame;
    }

    void TreeInterpreter::parseArg(ArgsFrameUPtr &argsFrameUPtr, const file_parser::ArgUPtr &arg)
    {
        if (arg->type == file_parser::Arg::Type::IDENT)
        {
            auto it = vars.find(std::get<std::string>(arg->value));
            if (it != vars.end())
            {
                argsFrameUPtr->allArgs.push_back(it->second.get());
            }
            else
            {
                throw std::runtime_error("Undefined variable: " + std::get<std::string>(arg->value));
            }
        }
        else
        {
            ExObjUPtr res = exObj_from_value_arg(arg);
            if (!res)
                throw std::runtime_error("Unexpected type");
            argsFrameUPtr->allArgs.push_back(res.get());
            argsFrameUPtr->valueArgs.push_back(std::move(res));
        }
    }
}
