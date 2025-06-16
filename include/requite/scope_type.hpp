#pragma once

namespace requite {

enum class ScopeType {
  NONE,
  MODULE,
  TABLE,
  OBJECT,
  PROCEDURE,
  ANONYMOUS_FUNCTION,
  LOCAL_STATEMENT,
  UNORDERED_VARIABLE
};

}