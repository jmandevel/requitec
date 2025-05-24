#pragma once

namespace requite {

enum class ScopeWalkerResultType
{
    NONE,
    FOUND,
    NOT_FOUND
};

struct ScopeWalkerResult final {
    requite::ScopeWalkerResultType type = requite::ScopeWalkerResultType::NONE;
    bool has_error = false;
};

}