// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/variable_type.hpp>
#include <requite/symbol.hpp>
#include <requite/scope.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Node;
struct Scope;

struct UnorderedVariable final {
  using Self = requite::UnorderedVariable;

  std::string _name = {};
  requite::Expression* _expression_ptr = nullptr;
  requite::VariableType _type = requite::VariableType::NONE;
  requite::AttributeFlags _attributes = {};
  requite::Scope _scope = {};
  requite::Symbol _data_type = {};

  // unordered_variable.cpp
  UnorderedVariable() = default;
  UnorderedVariable(const Self&) = delete;
  UnorderedVariable(Self&&) = delete;
  ~UnorderedVariable() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  void setType(requite::VariableType type);
  [[nodiscard]] requite::VariableType getType() const;
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
};

} // namespace requite