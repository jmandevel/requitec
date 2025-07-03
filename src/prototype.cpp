// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/signature.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <vector>

namespace requite {

bool Context::prototypeEntryPoint(requite::Procedure &procedure) {
  REQUITE_ASSERT(procedure.getType() == requite::ProcedureType::ENTRY_POINT);
  procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
  requite::Signature &signature = procedure.getSignature();
  requite::Symbol &return_type = signature.getReturnType();
  return_type.getRoot().setAsSigned(this->getAddressDepth());
  requite::Expression &expression = procedure.getExpression();
  if (!expression.getHasBranch()) {
    return true;
  }
  if (!this->prototypeProcedureBody(procedure, expression.getBranch())) {
    return false;
  }
  return true;
}

bool Context::prototypeLocal(requite::Local &local) {
  requite::Expression &expression = local.getExpression();
  requite::Expression &name_expression = expression.getBranch();
  requite::Expression &value_expression = name_expression.getNext();
  requite::Symbol &type = local.getDataType();
  requite::Scope &scope = local.getContaining();
  if (!this->inferenceTypeOfValue(type, scope, value_expression)) {
    return false;
  }
  this->finalizeIfLiteralType(type);
  return true;
}

bool Context::prototypeExit(requite::Procedure &procedure,
                            requite::Scope &scope,
                            requite::Expression &expression) {
  requite::Symbol type;
  requite::Expression& branch = expression.getBranch();
  if (!this->inferenceTypeOfValue(type, scope, branch)) {
    return false;
  }
  if (!type.getIsInteger() ||
      type.getRoot().getDepth() != this->getAddressDepth()) {
    this->logSourceMessage(
      branch,
      requite::LogType::ERROR,
      "invalid exit type"      
    );
    return false;
  }
  return true;
}

bool Context::prototypeProcedureBody(requite::Procedure &procedure,
                                     requite::Expression &first_statement) {
  bool is_ok = true;
  requite::Scope &scope = procedure.getScope();
  for (requite::Expression &statement :
       first_statement.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = statement.getOpcode()) {
    case requite::Opcode::_LOCAL:
      if (!this->prototypeLocal(statement.getLocal())) {
        is_ok = false;
      }
      break;
    case requite::Opcode::EXIT:
      if (!this->prototypeExit(procedure, scope, statement)) {
        is_ok = false;
      }
    default:
      break;
    }
  }
  return is_ok;
}

} // namespace requite