#pragma once

#include "../types/types.hpp"

#include <stdexcept>

namespace tree_executor
{
    class UnexpectedArg : public std::runtime_error
    {
    public:
        UnexpectedArg(const ExObjUPtr &arg, const char *expectedType)
            : std::runtime_error(
                  std::string("Unexpected argument type: '") + arg->GetType() +
                  "', expected: '" + expectedType + "'") {};
    };

    class UnexpectedIdent : public std::runtime_error
    {
    public:
        UnexpectedIdent(const std::string &ident)
            : std::runtime_error(
                  std::string("Unexpected ident: '") + ident + std::string("'")) {};
    };

} // namespace tree_executor
