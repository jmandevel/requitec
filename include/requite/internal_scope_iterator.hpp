#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct InternalScopeIterator final {
    using Self = requite::InternalScopeIterator;
    using value_type = requite::Scope;
    using reference = requite::Scope&;
    using pointer = requite::Scope*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    pointer _scope_ptr = nullptr;

    // internal_scope_iterator.cpp
    InternalScopeIterator() = default;
    InternalScopeIterator(reference scope);
    [[nodiscard]] Self &operator++();
    [[nodiscard]]Self operator++(int);
    [[nodiscard]] bool operator==(const Self& rhs) const;
    [[nodiscard]] bool operator!=(const Self& rhs) const;
    reference operator*();
    pointer operator->();
};

}