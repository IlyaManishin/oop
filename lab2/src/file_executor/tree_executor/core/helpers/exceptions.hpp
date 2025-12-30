#pragma once

#include "../types/types.hpp"

#include <stdexcept>

namespace tree_executor
{
    class InvalidArgsExc : public std::runtime_error
    {
    public:
        explicit InvalidArgsExc(const std::string &msg = "Invalid argument error")
            : std::runtime_error(msg) {}
    };

    class UnexpectedArgExc : public InvalidArgsExc
    {
    public:
        UnexpectedArgExc(const ExObjUPtr &arg, const char *expectedType)
            : InvalidArgsExc(
                  std::string("Unexpected argument type: '") +
                  arg->GetType() + "', expected: '" + expectedType + "'") {}

        explicit UnexpectedArgExc(const ExObjUPtr &arg)
            : InvalidArgsExc(
                  std::string("Unexpected argument type: '") +
                  arg->GetType() + "'") {}
    };

    class InvalidArgsCountExc : public InvalidArgsExc
    {
    public:
        InvalidArgsCountExc(size_t got, size_t expected)
            : InvalidArgsExc(
                  "Invalid arguments count: expected " +
                  std::to_string(expected) +
                  ", got " +
                  std::to_string(got)) {}
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
