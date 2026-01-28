#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

#include <optional>

namespace rq {

void Tabulator::tabulateModule() {
  RQ_ASSERT(!this->getIsStarted(), "tabulator can tabulate only once");
  this->setIsStarted();
  rq::Expression &trunk = this->getModule().getExpression();
  if (!trunk.getHasBranch()) {
    return;
  }
  this->tabulateForest(trunk.getBranch(), this->getContext().getTopScope());
}

void Tabulator::tabulateEntry(rq::EntrySymbol &entry) {
  RQ_ASSERT(!this->getIsStarted(), "tabulator can tabulate only once");
  this->setIsStarted();
  this->setIsBuildingInstructions();
  this->setResultKeyword(rq::Keyword::EXIT_CODE);
  this->setHighestSymbolTable(entry);
  rq::Expression &trunk = entry.getExpression();
  if (!trunk.getHasBranch()) {
    return;
  }
  this->tabulateForest(trunk.getBranch(), entry);
}

void Tabulator::tabulateForest(rq::Expression &first,
                               rq::SymbolTableSymbol &scope) {
  for (rq::Expression &branch : first.getInclusiveNextSubrange()) {
    const bool ascribed = branch.getKeyword() == rq::Keyword::ASCRIBE_STATEMENT;
    rq::Expression &statement = ascribed ? branch.getBranch() : branch;
    rq::ExpressionAttributeFlagsFactory flags_factory;
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
    case rq::Keyword::ASSIGN: {
      rq::Expression &lvalue = statement.getBranch();
      if (lvalue.getKeyword() != rq::Keyword::BINDING) {
        RQ_TODO_IMPLEMENTATION();
      }
      rq::Expression &path = lvalue.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::DynamicVariableSymbol &variable =
          this->getContext().allocateValue<rq::DynamicVariableSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, variable);
    } break;
    case rq::Keyword::TABLE: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::TableSymbol &table =
          this->getContext().allocateValue<rq::TableSymbol>(name);
      scope.tabulateNamedSymbol(this->getContext(), name, table);
    } break;
    case rq::Keyword::CLASS: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::ClassSymbol &class_ =
          this->getContext().allocateValue<rq::ClassSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, class_);
    } break;
    case rq::Keyword::ENUMERATION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::EnumerationSymbol &enumeration =
          this->getContext().allocateValue<rq::EnumerationSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, enumeration);
    } break;
    case rq::Keyword::ENTRY: {
      rq::EntrySymbol &entry =
          this->getContext().allocateValue<rq::EntrySymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), entry);
    } break;
    case rq::Keyword::FUNCTION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::FunctionSymbol &function =
          this->getContext().allocateValue<rq::FunctionSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, function);
    } break;
    case rq::Keyword::METHOD: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::MethodSymbol &method =
          this->getContext().allocateValue<rq::MethodSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, method);
    } break;
    case rq::Keyword::EXTENSION_FUNCTION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::ExtensionFunctionSymbol &extension_function =
          this->getContext().allocateValue<rq::ExtensionFunctionSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, extension_function);
    } break;
    case rq::Keyword::EXTENSION_METHOD: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path).value();
      rq::ExtensionMethodSymbol &extension_method =
          this->getContext().allocateValue<rq::ExtensionMethodSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, extension_method);
    } break;
    case rq::Keyword::CONSTRUCTOR: {
      rq::ConstructorSymbol &constructor =
          this->getContext().allocateValue<rq::ConstructorSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), constructor);
    } break;
    case rq::Keyword::DESTRUCTOR: {
      rq::DestructorSymbol &destructor =
          this->getContext().allocateValue<rq::DestructorSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), destructor);
    } break;
    case rq::Keyword::RANGER: {
      rq::RangerSymbol &ranger =
          this->getContext().allocateValue<rq::RangerSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), ranger);
    } break;
    default:
      RQ_TODO_IMPLEMENTATION();
    }
  }
}

rq::FullDeductionResult
Tabulator::deduceFullTypeOfValue(rq::Expression &expression) {
  std::ignore = expression;
  RQ_TODO_IMPLEMENTATION();
}

rq::PartialDeductionResult
Tabulator::deducePartialTypeOfValue(rq::Entity &partial_type,
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
