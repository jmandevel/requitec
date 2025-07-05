#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/local.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>

namespace requite {

bool Contextualizer0::tabulateModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  bool is_ok = true;
  for (requite::Expression &base_statement : module_name.getNextSubrange()) {
    if (!this->tabulateStatement(base_statement)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Contextualizer0::tabulateStatement(requite::Expression &statement) {
  if (!this->expandExpression(statement)) {
    return false;
  }
  return this->tabulateStatement(statement, false);
}

bool Contextualizer0::tabulateStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    return this->tabulateStatement(statement.getBranch(), true);
  case requite::Opcode::ENTRY_POINT:
    return this->tabulateEntryPoint(statement, has_attributes);
  case requite::Opcode::FUNCTION:
    return this->tabulateFunction(statement, has_attributes);
  case requite::Opcode::METHOD:
    return this->tabulateMethod(statement, has_attributes);
  case requite::Opcode::EXTENSION:
    return this->tabulateExtension(statement, has_attributes);
  case requite::Opcode::CONSTRUCTOR:
    return this->tabulateConstructor(statement, has_attributes);
  case requite::Opcode::DESTRUCTOR:
    return this->tabulateDestructor(statement, has_attributes);
  case requite::Opcode::OBJECT:
    return this->tabulateObject(statement, has_attributes);
  case requite::Opcode::ALIAS:
    return this->tabulateAlias(statement, has_attributes);
  case requite::Opcode::IMPORT:
    return this->tabulateImport(statement, has_attributes);
  case requite::Opcode::USE:
    return this->tabulateUse(statement, has_attributes);
  case requite::Opcode::GLOBAL:
    return this->tabulateGlobal(statement, has_attributes);
  case requite::Opcode::PROPERTY:
    return this->tabulateProperty(statement, has_attributes);
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

bool Contextualizer0::tabulateEntryPoint(requite::Expression &expression,
                                         bool has_attributes) {
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.setModule(this->getModule());
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  this->getModule().addEntryPoint(procedure);
  bool is_ok = true;
  if (has_attributes) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "entry_point must not have attributes");
    is_ok = false;
  }
  if (!this->expandBranchTree(expression)) {
    is_ok = false;
  }
  return is_ok;
}

bool Contextualizer0::tabulateFunction(requite::Expression &expression,
                                       bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateMethod(requite::Expression &expression,
                                     bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateExtension(requite::Expression &expression,
                                        bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateConstructor(requite::Expression &expression,
                                          bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateDestructor(requite::Expression &expression,
                                         bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateObject(requite::Expression &expression,
                                     bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateAlias(requite::Expression &expression,
                                    bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateImport(requite::Expression &import,
                                     bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateUse(requite::Expression &use,
                                  bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateGlobal(requite::Expression &expression,
                                     bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateProperty(requite::Expression &expression,
                                       bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulate_Local(requite::Local *&out_local_ptr,
                                      llvm::StringRef name,
                                      requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_LOCAL);
  requite::Local &local = this->getContext().makeLocal();
  local.setExpression(statement);
  statement.setLocal(local);
  local.setName(name);
  this->getScope().addInternalSymbol(local);
  out_local_ptr = &local;
  return true;
}

bool Contextualizer1::tabulateAlias(requite::Expression &expression) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulateImport(requite::Expression &import) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulateUse(requite::Expression &use) {
  REQUITE_UNREACHABLE(); // TODO
}

} // namespace requite