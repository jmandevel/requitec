#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/module.hpp>
#include <requite/options.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>
#include <requite/table.hpp>
#include <requite/tabulator.hpp>

namespace requite {

bool Context::tabulateModule(requite::Module &module) {
  requite::Tabulator tabulator(*this, module);
  if (!tabulator.tabulateModule()) {
    return false;
  }
  return true;
}

bool Tabulator::tabulateModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  for (requite::Expression &module_statement : module_name.getNextSubrange()) {
    this->tabulateModuleStatement(module_statement, false);
  }
  return this->getIsOk();
}

void Tabulator::tabulateModuleStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateModuleStatement(statement.getBranch(), true);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Tabulator::tabulateTableStatement(requite::Expression &statement,
                                       bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateTableStatement(statement.getBranch(), true);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Tabulator::tabulateObjectStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateObjectStatement(statement.getBranch(), true);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Tabulator::tabulateMatteLocalStatement(requite::Expression &statement,
                                            bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateMatteLocalStatement(statement.getBranch(), true);
    break;
  default:
    break;
  }
}

void Tabulator::tabulateExpressionForest(requite::Expression &expression) {
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->tabulateExpression(branch);
  }
  for (requite::Expression &next : expression.getNextSubrange()) {
    this->tabulateExpression(next);
  }
}

void Tabulator::tabulateExpression(requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE: {
    requite::Expression &branch = expression.getBranch();
    this->getContext().logErrorMustNotHaveAttributes(branch);
    this->tabulateExpression(branch);
  } break;
  default:
    if (!expression.getHasBranch()) {
      return;
    }
    requite::Expression &branch = expression.getBranch();
    this->tabulateExpressionForest(branch);
  }
}

} // namespace requite