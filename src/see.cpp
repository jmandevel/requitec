#include <rq/see.hpp>

namespace rq {

[[nodiscard]] llvm::StringRef
SymbolicExecutionEngine::evaluateName(rq::Node &expression) {
  if (expression.getKeyword() == rq::Keyword::I_IDENTIFIER_LITERAL) {
    return expression.getSourceText();
  } else if (expression.getKeyword() == rq::Keyword::S_IDENTIFY) {
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

} // namespace rq