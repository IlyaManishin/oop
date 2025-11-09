#pragma once

#include <variant>
#include <vector>
#include <string>

namespace cmd
{
    using Arg = std::variant<float, std::string>;

    struct Command
    {
        int commandId;
        std::vector<Arg> args;
    };

} // namespace cmd
