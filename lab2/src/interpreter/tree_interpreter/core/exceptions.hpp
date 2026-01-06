#pragma once

#include "types.hpp"

#include <stdexcept>

namespace tree_executor
{
    class InvalidArgExc : public std::runtime_error
    {
    public:
        explicit InvalidArgExc(const std::string &msg = "Invalid argument error")
            : std::runtime_error(msg) {}
    };

    class UnexpectedArgExc : public InvalidArgExc
    {
    public:
        UnexpectedArgExc(const char *gotType, const char *expectedType, const std::string &callName = "")
            : InvalidArgExc(buildMessage(gotType, expectedType, callName)) {}

        explicit UnexpectedArgExc(const ExObjUPtr &arg)
            : InvalidArgExc(std::string("Unexpected argument type: '") + arg->GetType() + "'") {}

    private:
        static std::string buildMessage(const char *gotType, const char *expectedType, const std::string &callName)
        {
            std::string msg = "Unexpected argument type: '" + std::string(gotType) +
                              "', expected: '" + expectedType + "'";
            if (!callName.empty())
                msg += " in " + callName;
            return msg;
        }
    };

    class InvalidArgsCountExc : public InvalidArgExc
    {
    public:
        InvalidArgsCountExc(size_t got, size_t expected, const std::string &callName = "")
            : InvalidArgExc(buildMessage(got, expected, callName)) {}

    private:
        static std::string buildMessage(size_t got, size_t expected, const std::string &callName)
        {
            std::string msg = "Invalid arguments count: expected " + std::to_string(expected) +
                              ", got " + std::to_string(got);
            if (!callName.empty())
                msg += " in " + callName;
            return msg;
        }
    };

    class RunTimeExc : public std::runtime_error
    {
    public:
        RunTimeExc(const std::string &error)
            : std::runtime_error(std::string("Runtime error: ") + error) {};
    };

    class UnexpectedIdentExc : public std::runtime_error
    {
    public:
        UnexpectedIdentExc(const std::string &ident)
            : std::runtime_error(
                  std::string("Unexpected ident: '") + ident + std::string("'")) {};
    };

    class UndefinedVarExc : public std::runtime_error
    {
    public:
        UndefinedVarExc(const std::string &varName)
            : std::runtime_error(
                  std::string("Variable used before being initialized: '") + varName + std::string("'")) {};
    };

} // namespace tree_executor
