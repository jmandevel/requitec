// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

namespace requite {

template <unsigned SMALL_SIZE_PARAM>
[[nodiscard]]
llvm::StringRef getCsvValueText(llvm::SmallString<SMALL_SIZE_PARAM> &buffer,
                                llvm::StringRef text);

}

#include <requite/detail/csv.hpp>