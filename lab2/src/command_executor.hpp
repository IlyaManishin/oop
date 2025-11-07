#pragma once

#include "cmd_parser/cmd_args.hpp"

#include <string>
#include <vector>

bool cmdHelp(const std::vector<cmd::Arg> &args) noexcept;
bool cmdInfo(const std::vector<cmd::Arg> &args) noexcept;
bool cmdMix(const std::vector<cmd::Arg> &args) noexcept;
bool cmdMute(const std::vector<cmd::Arg> &args) noexcept;
bool cmdChangeSpeed(const std::vector<cmd::Arg> &args) noexcept;