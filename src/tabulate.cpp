#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/ordered_variable.hpp>
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
  if (!expression.getHasBranch()) {
    return true;
  }
  if (!this->tabulateProcedureBody(module, procedure, expression.getBranch())) {
    return false;
  }
  return true;
}

bool Context::tabulateLocal(requite::Module &module, requite::Scope &scope,
                            requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_LOCAL);
  requite::Expression &name_expression = expression.getBranch();
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    return false;
  }
  llvm::StringRef name = name_expression.getDataText();
  if (scope.getHasInternalSymbolOfName(name)) {
    this->logErrorAlreadySymbolOfName(name_expression);
    return false;
  }
  requite::OrderedVariable &local = this->makeOrderedVariable();
  local.setType(requite::VariableType::LOCAL);
  local.setExpression(expression);
  expression.setOrderedVariable(local);
  local.setName(name);
  scope.addInternalSymbol(local);
  return true;
}

bool Context::tabulateProcedureBody(requite::Module& module, requite::Procedure &procedure,
                                      requite::Expression &first_statement) {
  bool is_ok = true;
  requite::Scope& scope = procedure.getScope();
  for (requite::Expression &statement :
       first_statement.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = statement.getOpcode()) {
      case requite::Opcode::_LOCAL:
        if (!this->tabulateLocal(module, scope, statement)){
          is_ok = false;
        }
        break;
      default:
        break;
    }
  }
  return is_ok;
}

} // namespace requite