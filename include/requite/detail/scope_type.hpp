#pragma once

namespace requite {

constexpr bool getCanHaveLocal(requite::ScopeType type) {
    switch (type) {
        case ScopeType::NONE:
            return false;
        case ScopeType::MODULE:
            return false;
        case ScopeType::TABLE:
            return false;
        case ScopeType::OBJECT:
            return false;
        case ScopeType::PROCEDURE:
            return true;
        case ScopeType::ANONYMOUS_FUNCTION:
            return true;
        case ScopeType::BLOCK:
            return true;
    }
    return false;
}

}