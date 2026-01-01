#pragma once

#include "file_parser/parser.hpp"
#include "types/types.hpp"

#include <unordered_map>
#include <vector>

namespace tree_executor
{
    class ArgsFrame;
    using ArgsFrameUPtr = std::unique_ptr<ArgsFrame>;

    class TreeInterpreter
    {
    public:
        void Execute(const file_parser::FileUPtr &tree);

    private:
        std::unordered_map<std::string, ExObjUPtr> vars;

        void executeStatement(const file_parser::Statement &stmt);
        void executeAssign(const file_parser::Assign &assign);
        void executeFuncRun(const file_parser::FuncRun &func_run);
        void executeMethodRun(const file_parser::MethodRun &method_run);
        void executeIfStat(const file_parser::IfStat &if_stat);

        ArgsFrameUPtr parseArgs(const file_parser::ArgsUPtr &args);
        void parseArg(ArgsFrameUPtr &argsFrameUPtr, const file_parser::ArgUPtr &arg);

        void addVar(const std::string &name, ExObjUPtr value);
        ExObjUPtr executeFuncCall(file_parser::FuncCall &func_call);
    };
}
