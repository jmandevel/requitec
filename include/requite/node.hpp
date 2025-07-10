// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/node_type.hpp>
#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringRef.h>

namespace requite {

struct Scope;
struct Expression;
struct RootSymbol;

struct Node final {
  using Self = requite::Node;

  requite::NodeType _type = requite::NodeType::NONE;
  requite::Expression *_expression_ptr = nullptr;
  requite::AttributeFlags _attributes = {};
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_TREE;
  requite::Node* _next_ptr = nullptr;
  bool _has_generated_name_or_value = false;

  // node.cpp
  Node() = default;
  Node(const Self &that) = default;
  Node(Self &&that) = default;
  Self &operator=(Self &&that) = default;
  Self &operator=(const Self &that) = default;
  ~Node() = default;
  void setType(requite::NodeType type);
  [[nodiscard]] requite::NodeType getType() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void setNext(requite::Node& node);
  [[nodiscard]] requite::Node& getNext();
  [[nodiscard]] const requite::Node& getNext() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasNext() const;
  [[nodiscard]] bool getHasGeneratedNameOrValue() const;
  void setHasGeneratedNameOrValue();
};

} // namespace requite