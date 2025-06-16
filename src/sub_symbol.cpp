// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/symbol.hpp>

namespace requite {

bool SubSymbol::operator==(const requite::SubSymbol &rhs) const {
  return this->_type == rhs._type && this->_count == rhs._count &&
         this->_attributes == rhs._attributes;
}

bool SubSymbol::operator!=(const requite::SubSymbol &rhs) const {
  return this->_type != rhs._type || this->_count != rhs._count ||
         this->_attributes != rhs._attributes;
}

void SubSymbol::setType(requite::SubSymbolType type) {
  REQUITE_ASSERT(this->getType() == requite::SubSymbolType::NONE);
  REQUITE_ASSERT(type != requite::SubSymbolType::NONE);
  this->_type = type;
}

requite::SubSymbolType SubSymbol::getType() const { return this->_type; }

requite::AttributeFlags &SubSymbol::getAttributeFlags() { return this->_attributes; }

const requite::AttributeFlags &SubSymbol::getAttributeFlags() const {
  return this->_attributes;
}

void SubSymbol::setCount(unsigned count) {
  REQUITE_ASSERT(requite::getHasCount(this->getType()));
  REQUITE_ASSERT(this->getCount() == 0);
  this->_count = count;
}

unsigned SubSymbol::getCount() const { return this->_count; }

void SubSymbol::setHasInferencedCount() {
  REQUITE_ASSERT(this->getCount() == 0);
  REQUITE_ASSERT(this->_has_inferenced_count = false);
  this->_has_inferenced_count = true;
}

bool SubSymbol::getHasInferencedCount() const {
  return this->_has_inferenced_count;
}

} // namespace requite