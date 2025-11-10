#pragma once

#include <variant>
#include <vector>
#include <string>

namespace cmd_parser
{
    using Arg = std::variant<float, std::string>;
    using Args = std::vector<Arg>;
    struct Command
    {
        int commandId;
        Args args;
    };

} // namespace cmd_parser
