// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace requite {

static constexpr std::string_view BUG_ERROR_MESSAGE =
    "this error should never occur. please report this as a bug to the "
    "developers of the RequiteCXX compiler";

static constexpr std::string_view ENTRY_POINT_NAME = "__entry_point";

static constexpr std::string_view ENTRY_POINT_MANGLED_NAME = "main";

static constexpr std::string_view PROCEDURE_ENTRY_BLOCK_NAME = "entry";


} // namespace requite