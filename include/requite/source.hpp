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

struct Source final {
  using Self = requite::Source;

  std::string _path{};
  std::string _relative_path{};
  llvm::MemoryBufferRef _buffer_ref{};
  std::uint_fast32_t _buffer_i = 0;
  requite::Expression *_root_ptr = nullptr;
  requite::Expression *_name_ptr = nullptr;

  // source.cpp
  Source() = default;
  Source(const Self &) = delete;
  Source(Self &&) = default;
  ~Source();
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
  [[nodiscard]]
  bool getHasRoot() const;
  [[nodiscard]]
  const requite::Expression &getRoot() const;
  [[nodiscard]]
  requite::Expression &getRoot();
  [[nodiscard]]
  const requite::Expression &getLastRoot() const;
  [[nodiscard]]
  requite::Expression &getLastRoot();
  void setRoot(requite::Expression &expression);
  [[nodiscard]]
  requite::Expression &popRoot();
  [[nodiscard]]
  requite::Expression &replaceRoot(requite::Expression &expression);
  [[nodiscard]] std::ranges::subrange<requite::ExpressionIterator,
                                      requite::ExpressionIterator,
                                      std::ranges::subrange_kind::unsized>
  getRootSubrange();
  [[nodiscard]] std::ranges::subrange<requite::ConstExpressionIterator,
                                      requite::ConstExpressionIterator,
                                      std::ranges::subrange_kind::unsized>
  getRootSubrange() const;

  // filesystem.cpp
  [[nodiscard]]
  bool makeIntermediateFilePath(llvm::SmallString<256> &out_path,
                                requite::Context &context,
                                llvm::Twine extension) const;

  // ast.cpp
};

} // namespace requite