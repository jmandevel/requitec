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
  case requite::FloatSemantics::BFLOAT16:
    return llvm::APFloat::BFloat();
  case requite::FloatSemantics::BINARY16:
    return llvm::APFloat::IEEEhalf();
  case requite::FloatSemantics::BINARY32:
    return llvm::APFloat::IEEEsingle();
  case requite::FloatSemantics::BINARY64:
    return llvm::APFloat::IEEEdouble();
  case requite::FloatSemantics::BINARY128:
    return llvm::APFloat::IEEEquad();
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite