// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

namespace requite {

[[nodiscard]] constexpr llvm::StringRef getUtf8Name(char codeunit);

[[nodiscard]] constexpr bool getIsWhitespace(char codeunit);

[[nodiscard]] constexpr bool getIsVerticalSpace(char codeunit);

[[nodiscard]] constexpr bool getIsDecimalDigit(char codeunit);

[[nodiscard]] constexpr bool getIsDigit(char codeunit);

// for all characters that can possibly exist in numeric literal.
[[nodiscard]] constexpr bool getIsNumericLiteral(char codeunit);

[[nodiscard]] constexpr unsigned char getDigitBaseMultiplier(char codeunit);

[[nodiscard]] constexpr bool getIsUppercase(char codeunit);

// used for converting to lowercase for numeric literals with bases lower
// than 36.
[[nodiscard]] constexpr char getLowercase(char codeunit);

[[nodiscard]] constexpr bool getIsIdentifier(char codeunit);

[[nodiscard]] constexpr bool getIsIdentifierStart(char codeunit);

[[nodiscard]] constexpr bool getIsSymbol(char codeunit);

[[nodiscard]] constexpr bool getIsExtended(char codeunit);

[[nodiscard]] constexpr unsigned char getExtendedStartCount(char codeunit);

[[nodiscard]] constexpr bool getIsValid(char codeunit);

[[nodiscard]] constexpr llvm::StringRef
getIntermediateFileEscapeSequence(char codeunit);

// escape_sequences.cpp
[[nodiscard]]
std::optional<std::uint32_t> getUtf32FromName(llvm::StringRef text);

} // namespace requite

#include <requite/detail/codeunits.hpp>