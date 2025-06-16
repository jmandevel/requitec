// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attribute_flags.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>

#include <cstddef>

namespace requite {

AttributeFlags::AttributeFlags(
    llvm::ArrayRef<requite::AttributeType> attributes) {
  for (requite::AttributeType attribute : attributes) {
    REQUITE_ASSERT(!this->getHasAttribute(attribute));
    this->addAttribute(attribute);
  }
}

void AttributeFlags::clear() { this->_flags.reset(); }

bool AttributeFlags::getHasAnyAttribute() const { return this->_flags.any(); }

void AttributeFlags::addAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(type == requite::AttributeType::USER ||
                 !this->getHasAttribute(type));
  this->_flags.set(static_cast<std::size_t>(type));
}

void AttributeFlags::removeAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(this->getHasAttribute(type));
  this->_flags.reset(static_cast<std::size_t>(type));
}

bool AttributeFlags::getHasAttribute(requite::AttributeType type) const {
  return this->_flags.test(static_cast<std::size_t>(type));
}

bool AttributeFlags::operator==(const requite::AttributeFlags &other) const {
  return this->_flags == other._flags;
}

bool AttributeFlags::operator!=(const requite::AttributeFlags &other) const {
  return !(*this == other);
}

} // namespace requite