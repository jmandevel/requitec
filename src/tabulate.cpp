#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/log_mode.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>

namespace requite {

bool Contextualizer0::contextualizeModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  for (requite::Expression &module_statement : module_name.getNextSubrange()) {
    this->tabulateStatement(module_statement,
                            requite::Situation::MODULE_STATEMENT, false);
  }
  return this->getIsOk();
}

void Contextualizer0::tabulateStatement(requite::Expression &statement,
                                        requite::Situation situation,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateStatement(statement.getBranch(), situation, true);
    break;
  case requite::Opcode::ENTRY_POINT:
    this->tabulateEntryPoint(statement, situation, has_attributes);
    break;
  case requite::Opcode::FUNCTION:
    this->tabulateFunction(statement, situation, has_attributes);
    break;
  case requite::Opcode::METHOD:
    this->tabulateMethod(statement, situation, has_attributes);
    break;
  case requite::Opcode::EXTENSION:
    this->tabulateExtension(statement, situation, has_attributes);
    break;
  case requite::Opcode::CONSTRUCTOR:
    this->tabulateConstructor(statement, situation, has_attributes);
    break;
  case requite::Opcode::DESTRUCTOR:
    this->tabulateDestructor(statement, situation, has_attributes);
    break;
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, situation, has_attributes);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, situation, has_attributes);
    break;
  case requite::Opcode::IMPORT:
    this->tabulateImport(statement, situation, has_attributes);
    break;
  case requite::Opcode::USE:
    this->tabulateUse(statement, situation, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, situation, has_attributes);
    break;
  case requite::Opcode::PROPERTY:
    this->tabulateProperty(statement, situation, has_attributes);
    break;
  case requite::Opcode::_BLOCK:
    this->tabulateBlock(statement, situation, has_attributes);
    break;
  case requite::Opcode::_EXPAND_VALUE:
    this->tabulate_ExpandValue(statement, situation, has_attributes);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Contextualizer0::tabulateEntryPoint(requite::Expression &expression,
                                         requite::Situation situation,
                                         bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateFunction(requite::Expression &expression,
                                       requite::Situation situation,
                                       bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateMethod(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateExtension(requite::Expression &expression,
                                        requite::Situation situation,
                                        bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateConstructor(requite::Expression &expression,
                                          requite::Situation situation,
                                          bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateDestructor(requite::Expression &expression,
                                         requite::Situation situation,
                                         bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateObject(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateAlias(requite::Expression &expression,
                                    requite::Situation situation,
                                    bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateImport(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateUse(requite::Expression &expression,
                                  requite::Situation situation,
                                  bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateGlobal(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateProperty(requite::Expression &expression,
                                       requite::Situation situation,
                                       bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulateBlock(requite::Expression &expression,
                                       requite::Situation situation,
                                       bool has_attributes) {
  // TODO
}

void Contextualizer0::tabulate_ExpandValue(requite::Expression &expression,
                                       requite::Situation situation,
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