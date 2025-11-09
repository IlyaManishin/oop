#pragma once

#include "cmd_parser/cmd_args.hpp"

#include <string>
#include <vector>

namespace command_executor
{
    bool cmd_run_from_config_file(const std::vector<cmd::Arg> &args) noexcept;
    bool cmd_help(const std::vector<cmd::Arg> &args) noexcept;
    bool cmd_info(const std::vector<cmd::Arg> &args) noexcept;
    bool cmd_mix(const std::vector<cmd::Arg> &args) noexcept;
    bool cmd_mute(const std::vector<cmd::Arg> &args) noexcept;
    bool cmd_change_speed(const std::vector<cmd::Arg> &args) noexcept;
} // namespace command_executor
