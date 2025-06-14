// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/scope.hpp>
#include <requite/scope_iterator.hpp>

namespace requite {

ScopeIterator::ScopeIterator(requite::Scope &scope)
    : _scope_ptr(&scope) {}

requite::ScopeIterator &ScopeIterator::operator++() {
  requite::Scope &scope = requite::getRef(this->_scope_ptr);
  this->_scope_ptr = scope.getNextPtr();
  return *this;
}

requite::ScopeIterator ScopeIterator::operator++(int) {
  REQUITE_ASSERT(this->_scope_ptr != nullptr);
  requite::ScopeIterator temp = *this;
  ++(*this);
  return temp;
}

bool ScopeIterator::operator==(
    const requite::ScopeIterator &it) const {
  return this->_scope_ptr == it._scope_ptr;
}

bool ScopeIterator::operator!=(
    const requite::ScopeIterator &it) const {
  return this->_scope_ptr != it._scope_ptr;
}

requite::Scope &ScopeIterator::operator*() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &ScopeIterator::operator*() const {
  return requite::getRef(this->_scope_ptr);
}

requite::Scope *ScopeIterator::operator->() {
  return this->_scope_ptr;
}

const requite::Scope *ScopeIterator::operator->() const {
  return this->_scope_ptr;
}

} // namespace requite