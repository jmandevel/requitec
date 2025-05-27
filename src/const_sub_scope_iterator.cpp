// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/scope.hpp>
#include <requite/assert.hpp>
#include <requite/const_sub_scope_iterator.hpp>

namespace requite {

ConstSubScopeIterator::ConstSubScopeIterator(const requite::Scope &Scope)
    : _sub_scope_ptr(&Scope) {}

requite::ConstSubScopeIterator &ConstSubScopeIterator::operator++() {
  const requite::Scope &Scope = requite::getRef(this->_sub_scope_ptr);
  this->_sub_scope_ptr = Scope._next_ptr;
  return *this;
}

requite::ConstSubScopeIterator ConstSubScopeIterator::operator++(int) {
  requite::ConstSubScopeIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]]
bool ConstSubScopeIterator::operator==(const requite::ConstSubScopeIterator &that) const {
  return this->_sub_scope_ptr == that._sub_scope_ptr;
}

[[nodiscard]]
bool ConstSubScopeIterator::operator!=(const requite::ConstSubScopeIterator &that) const {
  return this->_sub_scope_ptr != that._sub_scope_ptr;
}

const requite::Scope &ConstSubScopeIterator::operator*() const {
  return requite::getRef(this->_sub_scope_ptr);
}

const requite::Scope *ConstSubScopeIterator::operator->() const {
  REQUITE_ASSERT(this->_sub_scope_ptr != nullptr);
  return this->_sub_scope_ptr;
}

} // namespace requite
