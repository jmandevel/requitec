#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>

namespace requite {

bool Contextualizer0::contextualizeModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  for (requite::Expression &module_statement : module_name.getNextSubrange()) {
    this->tabulateStatement(module_statement, false);
  }
  return this->getIsOk();
}

void Contextualizer0::tabulateStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateStatement(statement.getBranch(), true);
    break;
  case requite::Opcode::ENTRY_POINT:
    this->tabulateEntryPoint(statement, has_attributes);
    break;
  case requite::Opcode::FUNCTION:
    this->tabulateFunction(statement, has_attributes);
    break;
  case requite::Opcode::METHOD:
    this->tabulateMethod(statement, has_attributes);
    break;
  case requite::Opcode::EXTENSION:
    this->tabulateExtension(statement, has_attributes);
    break;
  case requite::Opcode::CONSTRUCTOR:
    this->tabulateConstructor(statement, has_attributes);
    break;
  case requite::Opcode::DESTRUCTOR:
    this->tabulateDestructor(statement, has_attributes);
    break;
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, has_attributes);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, has_attributes);
    break;
  case requite::Opcode::IMPORT:
    this->tabulateImport(statement, has_attributes);
    break;
  case requite::Opcode::USE:
    this->tabulateUse(statement, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, has_attributes);
    break;
  case requite::Opcode::PROPERTY:
    this->tabulateProperty(statement, has_attributes);
    break;
  case requite::Opcode::_BLOCK:
    this->tabulateBlock(statement, has_attributes);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Contextualizer0::tabulateEntryPoint(requite::Expression &expression,
                                         bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ENTRY_POINT);
  requite::Procedure &procedure = this->getContext().makeEntryPoint();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  if (has_attributes) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "entry point must not have attributes");
    this->setNotOk();
  }
}

void Contextualizer0::tabulateFunction(requite::Expression &expression,
                                       bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION);
  requite::Procedure &procedure = this->getContext().makeFunction();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(
      name, this->getScope(), name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (found.getIsNone()) {
    requite::NamedProcedureGroup &group =
        this->getContext().makeNamedProcedureGroup();
    group.setName(name);
    procedure.setNamedProcedureGroup(group);
    this->getScope().addUserSymbol(group);
  } else if (found.getIsNamedProcedureGroup()) {
    requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
    procedure.setNamedProcedureGroup(group);
    this->getScope().addUserSymbol(group);
  } else {
    this->getContext().logErrorAlreadySymbolOfName(expression);
    this->setNotOk();
    return;
  }
}

void Contextualizer0::tabulateMethod(requite::Expression &expression,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateExtension(requite::Expression &expression,
                                        bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateConstructor(requite::Expression &expression,
                                          bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateDestructor(requite::Expression &expression,
                                         bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateObject(requite::Expression &expression,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateAlias(requite::Expression &expression,
                                    bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateImport(requite::Expression &expression,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateUse(requite::Expression &expression,
                                  bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateGlobal(requite::Expression &expression,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateProperty(requite::Expression &expression,
                                       bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateBlock(requite::Expression &expression,
                                    bool has_attributes) {
  // TODO
}

bool Contextualizer1::tabulate_Local(requite::Local *&out_local_ptr,
                                     llvm::StringRef name,
                                     requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_LOCAL);
  requite::Local &local = this->getContext().makeLocal();
  local.setExpression(statement);
  statement.setLocal(local);
  local.setName(name);
  this->getScope().addUserSymbol(local);
  out_local_ptr = &local;
  return true;
}

} // namespace requite