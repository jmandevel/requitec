// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/codeunits.hpp>
#include <unicode/uchar.h>
#include <unicode/unistr.h>

#include <llvm/ADT/SmallString.h>

namespace requite {

std::optional<std::uint32_t> getUtf32FromName(llvm::StringRef text) {
  if (text.empty()) {
    return std::nullopt;
  }
  llvm::SmallString<64> buffer = text;
  UErrorCode error_code = U_ZERO_ERROR;
  UChar32 utf32_codepoint =
      u_charFromName(U_UNICODE_CHAR_NAME, buffer.c_str(), &error_code);

  if (U_FAILURE(error_code)) {
    return std::nullopt;
  }
  return static_cast<std::uint32_t>(utf32_codepoint);
}

} // namespace requite