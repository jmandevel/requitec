// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/APFloat.h>

namespace requite {

enum class FloatSemantics {
  NONE,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128
};

[[nodiscard]] inline const llvm::fltSemantics &
getLlvmSemantics(requite::FloatSemantics semantics);

} // namespace requite

#include <requite/detail/float_semantics.hpp>