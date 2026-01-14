#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

namespace rq {

void Tabulator::tabulateModule() {
  RQ_ASSERT(this->getIsOk(), "tabulator can tabulate only once");
  rq::Expression &root = this->getModule().getExpression();
  if (!root.getHasBranch()) {
    return;
  }
  this->tabulateForest(root.getBranch(), this->getContext().getTopScope());
}

void Tabulator::tabulateForest(rq::Expression &first,
                               rq::SymbolTableSymbol &scope) {
  for (rq::Expression &branch : first.getInclusiveNextSubrange()) {
    const bool ascribed = branch.getKeyword() == rq::Keyword::S_ASCRIBE_SYMBOL;
    rq::Expression &statement = ascribed ? branch.getBranch() : branch;
    rq::SymbolAttributeFlagsFactory flags_factory;
    std::ignore = scope;
    if (ascribed) {
      flags_factory.addAllAttributres(statement);
    }
    if (flags_factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
    }
    switch (statement.getKeyword()) {
    case rq::Keyword::IMPORT: {
      rq::ImportSymbol &import =
          this->getContext().allocateValue<rq::ImportSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), import);
    } break;
    case rq::Keyword::MUTATION: {
      rq::MutationSymbol &mutation =
          this->getContext().allocateValue<rq::MutationSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), mutation);
    } break;
    case rq::Keyword::S_ASSIGN: {
      rq::Expression &branch = statement.getBranch();
      if (branch.getKeyword() != rq::Keyword::S_BINDING) {
        RQ_TODO_IMPLEMENTATION();
      }
      rq::Expression &path = branch.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->getSee().evaluateName(path);
      rq::DynamicVariableSymbol &variable =
          this->getContext().allocateValue<rq::DynamicVariableSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, variable);
    } break;
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
