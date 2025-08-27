#pragma once

#include <requite/see/symbol.hpp>

namespace requite {

struct Type final {
    using Self = requite::Type;

    requite::Symbol _symbol = requite::Symbol(requite::SymbolType::TYPE);
    requite::Symbol* _root_ptr;
};

}