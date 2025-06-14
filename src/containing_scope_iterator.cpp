// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/scope.hpp>
#include <requite/containing_scope_iterator.hpp>

namespace requite {

ContainingScopeIterator::ContainingScopeIterator(requite::Scope &scope)
    : _scope_ptr(&scope) {}

requite::ContainingScopeIterator &ContainingScopeIterator::operator++() {
  requite::Scope &scope = requite::getRef(this->_scope_ptr);
  this->_scope_ptr = scope.getContainingPtr();
  return *this;
}

requite::ContainingScopeIterator ContainingScopeIterator::operator++(int) {
  REQUITE_ASSERT(this->_scope_ptr != nullptr);
  requite::ContainingScopeIterator temp = *this;
  ++(*this);
  return temp;
}

bool ContainingScopeIterator::operator==(
    const requite::ContainingScopeIterator &it) const {
  return this->_scope_ptr == it._scope_ptr;
}

bool ContainingScopeIterator::operator!=(
    const requite::ContainingScopeIterator &it) const {
  return this->_scope_ptr != it._scope_ptr;
}

requite::Scope &ContainingScopeIterator::operator*() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &ContainingScopeIterator::operator*() const {
  return requite::getRef(this->_scope_ptr);
}

requite::Scope *ContainingScopeIterator::operator->() {
  return this->_scope_ptr;
}

const requite::Scope *ContainingScopeIterator::operator->() const {
  return this->_scope_ptr;
}

} // namespace requite