// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/scope.hpp>
#include <requite/assert.hpp>
#include <requite/sub_scope_iterator.hpp>

namespace requite {

SubScopeIterator::SubScopeIterator(requite::Scope &Scope)
    : _sub_scope_ptr(&Scope) {}

requite::SubScopeIterator &SubScopeIterator::operator++() {
  const requite::Scope &Scope = requite::getRef(this->_sub_scope_ptr);
  this->_sub_scope_ptr = Scope._next_ptr;
  return *this;
}

requite::SubScopeIterator SubScopeIterator::operator++(int) {
  requite::SubScopeIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]]
bool SubScopeIterator::operator==(const requite::SubScopeIterator &that) const {
  return this->_sub_scope_ptr == that._sub_scope_ptr;
}

[[nodiscard]]
bool SubScopeIterator::operator!=(const requite::SubScopeIterator &that) const {
  return this->_sub_scope_ptr != that._sub_scope_ptr;
}

requite::Scope &SubScopeIterator::operator*() {
  return requite::getRef(this->_sub_scope_ptr);
}

requite::Scope *SubScopeIterator::operator->() {
  REQUITE_ASSERT(this->_sub_scope_ptr != nullptr);
  return this->_sub_scope_ptr;
}

} // namespace requite
