#pragma once

#include <requite/yield_state.hpp>
#include <requite/symbol.hpp>

namespace requite {

struct YieldSymbol final {
    using Self = YieldSymbol;
    
    requite::YieldState _state = requite::YieldState::NONE;
    requite::RootSymbol _user_symbol = {};

    // yeilded_symbol.cpp
    YieldSymbol() = default;
    YieldSymbol(requite::YieldState state, requite::RootSymbol user_symbol);
    YieldSymbol(const Self&) = delete;
    YieldSymbol(Self&&) = delete;
    ~YieldSymbol() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] bool operator==(const Self&) const;
    [[nodiscard]] bool operator!=(const Self&) const;
    void changeState(requite::YieldState state);
    [[nodiscard]] requite::YieldState getState() const;
    [[nodiscard]] requite::RootSymbol getUserSymbol() const;
};

}