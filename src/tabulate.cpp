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
                             this->getContext().getTopStaticFrame().getTable());
}

void Tabulator::tabulateGlobalForest(const rq::Expression &first,
                                     rq::Table &table) {
  for (const rq::Expression &statement : first.getInclusiveNextSubrange()) {
    switch (statement.getKeyword()) {
    case rq::Keyword::ENTRY_POINT:
      this->tabulateEntryPoint(statement, table);
      break;
    default:
      RQ_TODO_IMPLEMENTATION();
    }
  }
}

void Tabulator::tabulateEntryPoint(const rq::Expression &expression,
                                   rq::Table &table) {
  RQ_ASSERT(expression.getKeyword() == rq::Keyword::ENTRY_POINT,
            "wrong keyword");
  rq::Procedure &procedure =
      this->getContext().getTopStaticFrame().allocateValue<rq::Procedure>(
          rq::ValueKind::ENTRY_POINT);
  procedure.setExpression(expression);
  std::ignore = table; // TODO implement rest
  //table.addUnamedValue(procedure);
}

} // namespace rq
