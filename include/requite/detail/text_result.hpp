// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr llvm::StringRef getDescription(requite::TextResult result) {
  switch (result) {
  case requite::TextResult::OK:
    return "no error occured";
  case requite::TextResult::ERROR_INVALID_ESCAPE:
    return "invalid escape sequence";
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite