// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_type.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/ErrorOr.h>

#include <bitset>
#include <optional>
#include <string>

namespace requite {

struct Context;
struct Expression;
struct MakeAttributesResult;

struct Attributes final {
  using Self = requite::Attributes;

  std::bitset<requite::ATTRIBUTE_TYPE_COUNT> _flags = {};

  // detail/attributes.hpp
  Attributes() = default;
  inline Attributes(llvm::ArrayRef<requite::AttributeType> attributes);
  [[nodiscard]] static requite::MakeAttributesResult
  makeAttributes(requite::Context &context,
                 requite::Expression &ascribe_expression);
  static void logErrorDuplicateAttribute(requite::Context &context,
                                         requite::Expression &branch,
                                         requite::AttributeType type);
  void clear();
  [[nodiscard]] bool getHasAnyAttribute() const;
  void addAttribute(requite::AttributeType type);
  void removeAttribute(requite::AttributeType type);
  [[nodiscard]] bool getHasAttribute(requite::AttributeType type) const;
  [[nodiscard]] bool operator==(const Self &other) const;
  [[nodiscard]] bool operator!=(const Self &other) const;
};

struct MakeAttributesResult {
  bool has_error = false;
  requite::Attributes attributes = {};
  requite::Expression *last_expression_ptr = nullptr;
  std::string mangled_name = {};
  std::vector<std::string> label_names = {};
};

} // namespace requite