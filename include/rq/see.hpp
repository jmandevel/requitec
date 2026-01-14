#pragma once

#include <rq/ast.hpp>

#include <llvm/ADT/StringRef.h>

namespace rq {

struct SymbolicExecutionEngine final {
  using Self = rq::SymbolicExecutionEngine;

  [[nodiscard]] llvm::StringRef evaluateName(rq::Expression &expression);
};

} // namespace rq