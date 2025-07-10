// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/symbol.hpp>
#include <requite/symbol_status.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Module;
struct Object;
struct Scope;

struct Alias final {
  using Self = Alias;

  std::string _name = {};
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::Expression *_expression_ptr = nullptr;
  requite::AttributeFlags _attributes = {};
  requite::Symbol _symbol = {};
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;
  bool _has_generated_name = false;

  // alias.cpp
  Alias() = default;
  Alias(const Self&) = delete;
  Alias(Self&&) = delete;
  ~Alias() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]] bool operator==(const Self &) const;
  [[nodiscard]] bool operator!=(const Self &) const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasName() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] requite::Symbol &getSymbol();
  [[nodiscard]] const requite::Symbol &getSymbol() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasGeneratedName() const;
  void setHasGeneratedName();
};

} // namespace requite