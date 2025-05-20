// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

namespace requite {

enum class TextResult { OK, ERROR_INVALID_ESCAPE };

[[nodiscard]] constexpr llvm::StringRef
getDescription(requite::TextResult result);

} // namespace requite

#include <requite/detail/text_result.hpp>