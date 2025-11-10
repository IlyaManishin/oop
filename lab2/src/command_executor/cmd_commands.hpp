#pragma once

#include "cmd_parser/cmd_args.hpp"

#include <string>
#include <vector>


namespace executor
{
    bool cmd_run_from_config_file(const cmd_parser::Args &args) noexcept;
    bool cmd_help(const cmd_parser::Args &args) noexcept;
    bool cmd_info(const cmd_parser::Args &args) noexcept;
    bool cmd_mix(const cmd_parser::Args &args) noexcept;
    bool cmd_mute(const cmd_parser::Args &args) noexcept;
    bool cmd_change_speed(const cmd_parser::Args &args) noexcept;
} // namespace executor
