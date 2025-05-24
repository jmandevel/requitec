#include <requite/internal_scope_iterator.hpp>
#include <requite/scope.hpp>

#include <utility>

namespace requite {

InternalScopeIterator::InternalScopeIterator(reference scope)
    : _scope_ptr(&scope) {}

requite::InternalScopeIterator &InternalScopeIterator::operator++() {
    reference scope = requite::getRef(this->_scope_ptr);
    this->_scope_ptr = scope._containing_scope_ptr;
    return *this;
}

requite::InternalScopeIterator InternalScopeIterator::operator++(int)
{
    Self temp = *this;
    std::ignore = ++(*this);
    return temp;
}

bool InternalScopeIterator::operator==(const Self &rhs) const
{
    return this->_scope_ptr == rhs._scope_ptr;
}

bool InternalScopeIterator::operator!=(const Self &rhs) const
{
    return this->_scope_ptr != rhs._scope_ptr;
}

requite::Scope& InternalScopeIterator::operator*()
{
    return requite::getRef(this->_scope_ptr);
}

requite::Scope* InternalScopeIterator::operator->()
{
    return this->_scope_ptr;
}

} // namespace requite