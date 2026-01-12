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
  this->tabulateForest(root.getBranch(), this->getContext().getTopScope());
}

void Tabulator::tabulateForest(const rq::Expression &first,
                               rq::SymbolTableSymbol &scope) {
  for (const rq::Expression &branch : first.getInclusiveNextSubrange()) {
    const bool ascribed = branch.getKeyword() == rq::Keyword::S_ASCRIBE_SYMBOL;
    const rq::Expression &statement = ascribed ? branch.getBranch() : branch;
    rq::SymbolAttributeFlagsFactory flags_factory;
    std::ignore = scope;
    if (ascribed) {
      flags_factory.addAllAttributres(statement);
    }
    if (flags_factory.getHasStatic()) {
      continue;
      // TODO symbolic execution
    }
    switch (statement.getKeyword()) {
    case rq::Keyword::IMPORT:

      break;
    case rq::Keyword::MUTATION:

      break;
    case rq::Keyword::S_ASSIGN:

      break;
    case rq::Keyword::TABLE:

      break;
    case rq::Keyword::CLASS:

      break;
    case rq::Keyword::ENUMERATION:

      break;
    case rq::Keyword::ENTRY:

      break;
    case rq::Keyword::FUNCTION:

      break;
    case rq::Keyword::METHOD:

      break;
    case rq::Keyword::EXTENSION_FUNCTION:

      break;
    case rq::Keyword::EXTENSION_METHOD:

      break;
    case rq::Keyword::CONSTRUCTOR:

      break;
    case rq::Keyword::DESTRUCTOR:

      break;
    case rq::Keyword::RANGER:

      break;
    default:
      RQ_TODO_IMPLEMENTATION();
    }
  }
}

} // namespace rq
