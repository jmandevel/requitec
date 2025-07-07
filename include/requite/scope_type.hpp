#pragma once

namespace requite {

enum class ScopeType {
  NONE,
  MODULE,
  TABLE,
  OBJECT,
  PROCEDURE,
  ANONYMOUS_FUNCTION,
  GLOBAL,
  BASE_OR_TABLE_BLOCK,
  OBJECT_BLOCK,
  LOCAL_BLOCK
};

}