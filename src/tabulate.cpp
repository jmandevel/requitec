#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

namespace rq {

void Tabulator::tabulateModule() {
  RQ_ASSERT(this->getIsOk(), "tabulator can tabulate only once");
  const rq::Expression &root = this->getModule().getExpression();
  if (!root.getHasBranch()) {
    return;
  }
  this->tabulateGlobalForest(root.getBranch(),
                             this->getContext().getTopScope());
}

void Tabulator::tabulateGlobalForest(const rq::Expression &first,
                                     rq::ScopeSymbol &scope) {
  for (const rq::Expression &statement : first.getInclusiveNextSubrange()) {
    switch (statement.getKeyword()) {
    case rq::Keyword::ENTRY_POINT:
      this->tabulateEntryPoint(statement, scope);
      break;
    default:
      RQ_TODO_IMPLEMENTATION();
    }
  }
}

void Tabulator::tabulateEntryPoint(const rq::Expression &expression,
                                   rq::ScopeSymbol &scope) {
  RQ_ASSERT(expression.getKeyword() == rq::Keyword::ENTRY_POINT,
            "wrong keyword");
  rq::EntryPointSymbol &procedure =
      this->getContext().allocateValue<rq::EntryPointSymbol>(this->getModule());
  procedure.setExpression(expression);
  scope.tabulateUnamedSymbol(this->getContext(), procedure);
}

} // namespace rq
