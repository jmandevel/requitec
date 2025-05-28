// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/node_type.hpp>
#include <requite/attributes.hpp>

#include <llvm/ADT/StringRef.h>

namespace requite {

struct Scope;
struct Expression;
struct RootSymbol;

struct Node final {
  using Self = requite::Node;

  requite::NodeType _type = requite::NodeType::NONE;
  requite::Expression *_expression_ptr = nullptr;
  requite::Attributes _attributes = {};

  // node.cpp
  Node() = default;
  Node(const Self& that) = default;
  Node(Self&& that) = default;
  Self& operator=(Self&& that) = default;
  Self& operator=(const Self& that) = default;
  ~Node() = default;
  void setType(requite::NodeType type);
  [[nodiscard]] requite::NodeType getType() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasAttributes() const;
  void setAttributes(requite::Attributes attributes);
  [[nodiscard]] requite::Attributes &getAttributes();
  [[nodiscard]] const requite::Attributes &getAttributes() const;
};

} // namespace requite