#include "tree_interpreter.hpp"
#include "base_calls/base_calls.hpp"
#include "core/exceptions.hpp"
#include "core/utils.hpp"

#include <variant>
#include <vector>

using namespace file_parser;

namespace tree_executor
{
    TreeInterpreter::TreeInterpreter(std::ostream &out) : out(out) {};

    ExObjPtr TreeInterpreter::getVarPtr(const std::string &varName)
    {
        auto it = vars.find(varName);
        if (it != vars.end())
        {
            return it->second.get();
        }
        else
        {
            throw UndefinedVarExc(varName);
        }
    }

    ExObjUPtr TreeInterpreter::executeExpression(file_parser::Expression &expr)
    {
        ExObjUPtr res = nullptr;
        if (std::holds_alternative<FuncCallUPtr>(expr.value))
        {
            res = executeFuncCall(*std::get<FuncCallUPtr>(expr.value));
        }
        else if (std::holds_alternative<ArgUPtr>(expr.value))
        {
            const ArgUPtr &arg = std::get<ArgUPtr>(expr.value);
            res = exobj_from_value_arg(arg);
            if (!res)
                throw InvalidArgExc("Not value argument: " + std::to_string((int)arg->type));
        }
        return res;
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
            const std::string &varName = std::get<std::string>(arg->value);
            ExObjPtr varPtr = this->getVarPtr(varName);
            argsFrameUPtr->allArgs.push_back(varPtr);
        }
        else
        {
            ExObjUPtr res = exobj_from_value_arg(arg);
            if (!res)
                throw InvalidArgExc("Invalid arg with type index: " + std::to_string((int)arg->type));
            argsFrameUPtr->allArgs.push_back(res.get());
            argsFrameUPtr->valueArgs.push_back(std::move(res));
        }
    }

    ExObjUPtr TreeInterpreter::executeFuncCall(file_parser::FuncCall &funcCall)
    {
        ArgsFrameUPtr argsFrame = this->parseArgs(funcCall.args);
        ExObjUPtr res = func_call_impl(funcCall.name, argsFrame->allArgs, this->out);
        return res;
    }

    bool TreeInterpreter::executePredicate(file_parser::FuncCall &funcCall)
    {
        ArgsFrameUPtr argsFrame = this->parseArgs(funcCall.args);
        bool res = predicate_call_impl(funcCall.name, argsFrame->allArgs);
        return res;
    }

    void TreeInterpreter::executeMethod(const std::string &varName, file_parser::FuncCall &funcCall)
    {
        ExObjPtr var = this->getVarPtr(varName);
        ArgsFrameUPtr argsFrame = this->parseArgs(funcCall.args);
        var->RunMethod(funcCall.name, argsFrame->allArgs);
    }
}
