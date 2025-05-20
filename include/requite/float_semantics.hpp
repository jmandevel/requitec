// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/APFloat.h>

namespace requite {

enum class FloatSemantics {
  NONE,
  BRAIN,
  BINARY_HALF,
  BINARY_SINGLE,
  BINARY_DOUBLE,
  BINARY_QUAD
};

[[nodiscard]] inline const llvm::fltSemantics &
getLlvmSemantics(requite::FloatSemantics semantics);

} // namespace requite

#include <requite/detail/float_semantics.hpp>