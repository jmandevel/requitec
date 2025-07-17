// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

[[nodiscard]] inline
std::string getIntermediateOutputStringText(llvm::StringRef text);

}

#include <requite/detail/intermediate_string.hpp>