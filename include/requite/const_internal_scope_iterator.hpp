#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct ConstInternalScopeIterator final {
    using Self = requite::ConstInternalScopeIterator;
    using value_type = requite::Scope;
    using reference = const requite::Scope&;
    using pointer = const requite::Scope*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    pointer _scope_ptr = nullptr;

    // const_internal_scope_iterator.cpp
    ConstInternalScopeIterator() = default;
    ConstInternalScopeIterator(reference scope);
    [[nodiscard]] Self &operator++();
    [[nodiscard]] Self operator++(int);
    [[nodiscard]] bool operator==(const Self& rhs) const;
    [[nodiscard]] bool operator!=(const Self& rhs) const;
    reference operator*() const;
    pointer operator->() const;
};

}