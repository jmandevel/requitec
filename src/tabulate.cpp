#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

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

void Tabulator::tabulateEntry(rq::EntrySymbol& entry) {
  RQ_ASSERT(!this->getIsStarted(), "tabulator can tabulate only once");
  this->setIsStarted();
  this->setIsBuildingInstructions();
  this->setResultKeyword(rq::EntityKind::KW_EXIT_CODE);
  this->setHighestSymbolTable(entry);
  rq::Expression& trunk = entry.getExpression();
  if (!trunk.getHasBranch()) {
    return;
  } 
  this->tabulateForest(trunk.getBranch(), entry);
}

void Tabulator::tabulateForest(rq::Expression &first,
                               rq::SymbolTableSymbol &scope) {
  for (rq::Expression &branch : first.getInclusiveNextSubrange()) {
    const bool ascribed =
        branch.getKeyword() == rq::EntityKind::KW_ASCRIBE_STATEMENT;
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
    case rq::EntityKind::KW_IMPORT: {
      rq::ImportSymbol &import =
          this->getContext().allocateValue<rq::ImportSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), import);
    } break;
    case rq::EntityKind::KW_MUTATION: {
      rq::MutationSymbol &mutation =
          this->getContext().allocateValue<rq::MutationSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), mutation);
    } break;
    case rq::EntityKind::KW_ASSIGN: {
      rq::Expression &lvalue = statement.getBranch();
      if (lvalue.getKeyword() != rq::EntityKind::KW_BINDING) {
        RQ_TODO_IMPLEMENTATION();
      }
      rq::Expression &path = lvalue.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::DynamicVariableSymbol &variable =
          this->getContext().allocateValue<rq::DynamicVariableSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, variable);
    } break;
    case rq::EntityKind::KW_TABLE: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::TableSymbol &table =
          this->getContext().allocateValue<rq::TableSymbol>(name);
      scope.tabulateNamedSymbol(this->getContext(), name, table);
    } break;
    case rq::EntityKind::KW_CLASS: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::ClassSymbol &class_ =
          this->getContext().allocateValue<rq::ClassSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, class_);
    } break;
    case rq::EntityKind::KW_ENUMERATION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::EnumerationSymbol &enumeration =
          this->getContext().allocateValue<rq::EnumerationSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, enumeration);
    } break;
    case rq::EntityKind::KW_ENTRY: {
      rq::EntrySymbol &entry =
          this->getContext().allocateValue<rq::EntrySymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), entry);
    } break;
    case rq::EntityKind::KW_FUNCTION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::FunctionSymbol &function =
          this->getContext().allocateValue<rq::FunctionSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, function);
    } break;
    case rq::EntityKind::KW_METHOD: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::MethodSymbol &method =
          this->getContext().allocateValue<rq::MethodSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, method);
    } break;
    case rq::EntityKind::KW_EXTENSION_FUNCTION: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::ExtensionFunctionSymbol &extension_function =
          this->getContext().allocateValue<rq::ExtensionFunctionSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, extension_function);
    } break;
    case rq::EntityKind::KW_EXTENSION_METHOD: {
      rq::Expression &path = statement.getBranch();
      if (!path.getIsEvaluatableName()) {
        RQ_TODO_IMPLEMENTATION();
      }
      llvm::StringRef name = this->evaluateName(path);
      rq::ExtensionMethodSymbol &extension_method =
          this->getContext().allocateValue<rq::ExtensionMethodSymbol>(
              statement, this->getModule(), scope, name,
              flags_factory.getFlags());
      scope.tabulateNamedSymbol(this->getContext(), name, extension_method);
    } break;
    case rq::EntityKind::KW_CONSTRUCTOR: {
      rq::ConstructorSymbol &constructor =
          this->getContext().allocateValue<rq::ConstructorSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), constructor);
    } break;
    case rq::EntityKind::KW_DESTRUCTOR: {
      rq::DestructorSymbol &destructor =
          this->getContext().allocateValue<rq::DestructorSymbol>(
              statement, this->getModule(), scope, flags_factory.getFlags());
      scope.tabulateUnamedSymbol(this->getContext(), destructor);
    } break;
    case rq::EntityKind::KW_RANGER: {
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

[[nodiscard]] llvm::StringRef
Tabulator::evaluateName(rq::Expression &expression) {
  if (expression.getKeyword() == rq::EntityKind::KW_IDENTIFIER_LITERAL) {
    return expression.getSourceText();
  } else if (expression.getKeyword() == rq::EntityKind::KW_IDENTIFY) {
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

} // namespace rq
