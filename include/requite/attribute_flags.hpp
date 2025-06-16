// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_type.hpp>

#include <llvm/ADT/ArrayRef.h>

#include <bitset>
#include <string>

namespace requite {

struct Context;
struct Expression;

struct AttributeFlags final {
  using Self = requite::AttributeFlags;

  std::bitset<requite::ATTRIBUTE_TYPE_COUNT> _flags = {};

  // detail/attribute_flags.hpp
  AttributeFlags() = default;
  inline AttributeFlags(llvm::ArrayRef<requite::AttributeType> attributes);
  void clear();
  [[nodiscard]] bool getHasAnyAttribute() const;
  void addAttribute(requite::AttributeType type);
  void removeAttribute(requite::AttributeType type);
  [[nodiscard]] bool getHasAttribute(requite::AttributeType type) const;
  [[nodiscard]] bool operator==(const Self &other) const;
  [[nodiscard]] bool operator!=(const Self &other) const;
};

} // namespace requite