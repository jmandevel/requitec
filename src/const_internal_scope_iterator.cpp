#include <requite/const_internal_scope_iterator.hpp>
#include <requite/scope.hpp>
#include <requite/assert.hpp>

#include <utility>

namespace requite {

ConstInternalScopeIterator::ConstInternalScopeIterator(reference scope)
    : _scope_ptr(&scope) {}

requite::ConstInternalScopeIterator &ConstInternalScopeIterator::operator++() {
  reference scope = requite::getRef(this->_scope_ptr);
  this->_scope_ptr = scope._containing_scope_ptr;
  return *this;
}

requite::ConstInternalScopeIterator
ConstInternalScopeIterator::operator++(int) {
  Self temp = *this;
  std::ignore = ++(*this);
  return temp;
}

bool ConstInternalScopeIterator::operator==(const Self &rhs) const {
  return this->_scope_ptr == rhs._scope_ptr;
}

bool ConstInternalScopeIterator::operator!=(const Self &rhs) const {
  return this->_scope_ptr != rhs._scope_ptr;
}

const requite::Scope &ConstInternalScopeIterator::operator*() const {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope *ConstInternalScopeIterator::operator->() const {
  return this->_scope_ptr;
}

} // namespace requite