#pragma once

#include "file_parser/parser.hpp"

namespace tree_executor
{
    class Executor
    {
    public:
        static void ExecuteTree(const file_parser::FileUPtr &tree);

    private:
        void executeStatement(const file_parser::Statement &stmt);
        void executeAssign(const file_parser::Assign &assign);
        void executeFuncRun(const file_parser::FuncRun &func_run);
        void executeFuncCall(const file_parser::FuncCall &func_call);
        void executeMethodRun(const file_parser::MethodRun &method_run);
        void executeIfStat(const file_parser::IfStat &if_stat);
    };
} // namespace tree_executor
