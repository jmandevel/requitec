// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/symbol.hpp>
#include <requite/scope.hpp>
#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Node;
struct Scope;

struct Global final {
  using Self = requite::Global;

  std::string _name = {};
  requite::Expression* _expression_ptr = nullptr;
  requite::AttributeFlags _attributes = {};
  requite::Scope _scope = {};
  requite::Symbol _data_type = {};
  requite::Global *_next_ptr = nullptr;
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;
  bool _has_generated_name = false;

  // global.cpp
  Global() = default;
  Global(const Self&) = delete;
  Global(Self&&) = delete;
  ~Global() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]] bool operator==(const Self&) const;
  [[nodiscard]] bool operator!=(const Self&) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] requite::Symbol &getDataType();
  [[nodiscard]] const requite::Symbol& getDataType() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] bool getHasNext() const;
  void setNext(requite::Global& global);
  [[nodiscard]] requite::Global& getNext();
  [[nodiscard]] const requite::Global& getNext() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasGeneratedName() const;
  void setHasGeneratedName();
};

} // namespace requite