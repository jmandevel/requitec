#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/scope.hpp>

namespace requite {

bool Context::tabulateEntryPoint(requite::Module &module,
                                 requite::Expression &expression) {
  requite::Procedure &procedure = this->makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.setModule(module);
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  module.addEntryPoint(procedure);
  if (!this->tabulateLocalExpressions(procedure.getModule(),
                                      procedure.getScope(),
                                      expression.getBranch())) {
    return false;
  }
  return true;
}

bool Context::tabulate_Initialize(requite::Module &module,
                                  requite::Scope &scope,
                                  requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_INITIALIZE);
  requite::Expression &lhs = expression.getBranch();
  if (requite::getCanBeSymbolNameSituation(lhs.getOpcode())) {
    llvm::StringRef name;
    if (!this->evaluateName(name, scope, lhs)) {
      return false;
    }
    requite::RootSymbol found;
    found = scope.lookupUserSymbol(name);
    if (found.getIsNone()) {
      expression.changeOpcode(requite::Opcode::_LOCAL);
      requite::Local &local = this->makeLocal();
      local.setExpression(expression);
      expression.setLocal(local);
      local.setName(name);
      scope.addInternalSymbol(local);
    }
  }
  return this->tabulateLocalExpressions(module, scope, lhs);
}

bool Context::tabulateLocalExpressions(requite::Module &module,
                                       requite::Scope &scope,
                                       requite::Expression &first) {
  bool is_ok = true;
  for (requite::Expression &expression : first.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = expression.getOpcode()) {
    case requite::Opcode::_INITIALIZE:
      if (!this->tabulate_Initialize(module, scope, expression)) {
        is_ok = false;
      }
      break;
    default:
      if (!expression.getHasBranch()) {
        break;
      }
      if (!this->tabulateLocalExpressions(module, scope,
                                          expression.getBranch())) {
        is_ok = false;
      }
      break;
    }
  }
  return is_ok;
}

} // namespace requite