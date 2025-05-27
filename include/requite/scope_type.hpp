#pragma once

namespace requite {

enum class ScopeType {
    NONE,
    LOCAL_STATEMENT,
    MODULE,
    TABLE,
    OBJECT,
    PROCEDURE,
    ANONYMOUS_FUNCTION,
    CONDUIT
};

}