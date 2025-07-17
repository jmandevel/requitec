#pragma once

namespace requite {

enum class ScopeType {
  NONE,
  MODULE,
  TABLE,
  OBJECT,
  PROCEDURE,
  ANONYMOUS_FUNCTION,
  BLOCK
};

[[nodiscard]] constexpr bool getCanHaveLocal(requite::ScopeType type);

}

#include <requite/detail/scope_type.hpp>