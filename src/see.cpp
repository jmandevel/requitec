#include <rq/see.hpp>

namespace rq {

[[nodiscard]] llvm::StringRef
SymbolicExecutionEngine::evaluateName(rq::Expression &expression) {
  if (expression.getKeyword() == rq::EntityKind::KW_IDENTIFIER_LITERAL) {
    return expression.getSourceText();
  } else if (expression.getKeyword() == rq::EntityKind::KW_IDENTIFY) {
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

} // namespace rq