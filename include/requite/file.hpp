// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/const_expression_iterator.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/opcode.hpp>
#include <requite/token.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/StringSaver.h>

#include <cstdint>
#include <ranges>
#include <string>
#include <vector>

namespace requite {

struct Expression;
struct ExpressionIterator;
struct ConstExpressionIterator;
struct Context;

struct File final {
  using Self = requite::File;

  std::string _path{};
  std::string _relative_path{};
  llvm::MemoryBufferRef _buffer_ref{};
  std::uint_fast32_t _buffer_i = 0;

  // file.cpp
  File() = default;
  File(const Self &) = delete;
  File(Self &&) = default;
  ~File() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]]
  llvm::StringRef getPath() const;
  [[nodiscard]]
  llvm::StringRef getIdentifier() const;
  [[nodiscard]]
  llvm::StringRef getText() const;
  [[nodiscard]]
  const char *getTextPtr() const;
  [[nodiscard]]
  std::uint_fast32_t getBufferI() const;
};

} // namespace requite