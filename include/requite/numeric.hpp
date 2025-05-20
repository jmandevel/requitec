// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/float_semantics.hpp>
#include <requite/numeric_result.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

namespace requite {

static constexpr unsigned MAX_BASE = 64;

static constexpr unsigned MIN_UPPER_BASE = 36;

template <typename NumericParam>
[[nodiscard]] inline requite::NumericResult
getNumericValue(llvm::StringRef text, NumericParam &out_value);

[[nodiscard]] inline requite::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &out_value,
                requite::FloatSemantics semantics);

[[nodiscard]] inline int
getIntegerDepthRequired(llvm::StringRef value_portion_text, unsigned base);

[[nodiscard]] inline requite::NumericResult
cleanRealText(llvm::StringRef text, llvm::SmallString<16> &out_clean);

} // namespace requite

#include <requite/detail/numeric.hpp>