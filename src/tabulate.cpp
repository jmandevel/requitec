#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

#include <optional>

namespace rq {

void Tabulator::tabulateModule() {
  RQ_TODO_IMPLEMENTATION();
}

void Tabulator::tabulateEntry(rq::EntrySymbol &entry) {
  RQ_TODO_IMPLEMENTATION();
}

void Tabulator::tabulateForest(rq::Expression &first,
                               rq::SymbolTableSymbol &scope) {
  RQ_TODO_IMPLEMENTATION();
}

rq::DeductionResult
Tabulator::deduceTypeOfValue(rq::Expression &expression) {
  std::ignore = expression;
  RQ_TODO_IMPLEMENTATION();
}

rq::DeductionWithIncompleteResult
Tabulator::deduceTypeOfValueWithIncomplete(rq::Entity &partial_type,
                                    rq::Expression &expression) {
  std::ignore = partial_type;
  std::ignore = expression;
  RQ_TODO_IMPLEMENTATION();
}

rq::SubstitutionResult
Tabulator::substituteTypeOfValue(rq::Entity &type, rq::Expression &expression) {
  std::ignore = type;
  std::ignore = expression;
  RQ_TODO_IMPLEMENTATION();
}

rq::EvaluationResult Tabulator::evaluateValue(rq::Entity &type,
                                              rq::Expression &expression) {
  std::ignore = type;
  std::ignore = expression;
  RQ_TODO_IMPLEMENTATION();
}

std::optional<llvm::StringRef>
Tabulator::evaluateName(rq::Expression &expression) {
  if (expression.getKeyword() == rq::Keyword::IDENTIFIER_LITERAL) {
    return expression.getSourceText();
  } else if (expression.getKeyword() == rq::Keyword::IDENTIFY) {
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

} // namespace rq
