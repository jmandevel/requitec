// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

inline const llvm::fltSemantics &
getLlvmSemantics(requite::FloatSemantics semantics) {
  switch (semantics) {
  case requite::FloatSemantics::NONE:
    REQUITE_UNREACHABLE();
  case requite::FloatSemantics::BRAIN:
    return llvm::APFloat::BFloat();
  case requite::FloatSemantics::BINARY_HALF:
    return llvm::APFloat::IEEEhalf();
  case requite::FloatSemantics::BINARY_SINGLE:
    return llvm::APFloat::IEEEsingle();
  case requite::FloatSemantics::BINARY_DOUBLE:
    return llvm::APFloat::IEEEdouble();
  case requite::FloatSemantics::BINARY_QUAD:
    return llvm::APFloat::IEEEquad();
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite