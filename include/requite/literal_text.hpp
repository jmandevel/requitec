// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/text_result.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

namespace requite {

template <unsigned SMALL_SIZE_PARAM>
[[nodiscard]] inline requite::TextResult
getTextValue(llvm::StringRef text,
             llvm::SmallString<SMALL_SIZE_PARAM> &text_out);

}

#include <requite/detail/literal_text.hpp>