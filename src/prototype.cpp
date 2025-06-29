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
  requite::RootSymbol &return_root = return_type.getRoot();
  return_root.setType(requite::RootSymbolType::SIGNED_INTEGER);
  const unsigned pointer_depth = this->getAddressDepth();
  return_root.setDepth(pointer_depth);
  requite::Expression &expression = procedure.getExpression();
  if (!expression.getHasBranch()) {
    return true;
  }
  if (!this->prototypeProcedureBody(procedure, expression.getBranch())) {
    return false;
  }
  return true;
}

bool Context::prototypeLocal(requite::Scope &scope,
                             requite::OrderedVariable &variable) {
  REQUITE_ASSERT(variable.getType() == requite::VariableType::LOCAL);
  requite::Expression &expression = variable.getExpression();
  requite::Expression &name_expression = expression.getBranch();
  requite::Expression &value_expression = name_expression.getNext();
  requite::Symbol &type = variable.getDataType();
  if (!this->inferenceTypeOfValue(type, scope, value_expression)) {
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
      if (!this->prototypeLocal(scope, statement.getOrderedVariable())) {
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