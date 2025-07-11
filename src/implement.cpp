// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/signature.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <vector>

namespace requite {

bool Context::implementProcedure(requite::Procedure &procedure) {
  switch (const requite::ProcedureType type = procedure.getType()) {
  case requite::ProcedureType::ENTRY_POINT:
    return this->implementEntryPoint(procedure);
  case requite::ProcedureType::FUNCTION:
    return this->implementFunction(procedure);
  case requite::ProcedureType::METHOD:
    return this->implementMethod(procedure);
  case requite::ProcedureType::EXTENSION:
    return this->implementExtension(procedure);
  case requite::ProcedureType::CONSTRUCTOR:
    return this->implementConstructor(procedure);
  case requite::ProcedureType::DESTRUCTOR:
    return this->implementDestructor(procedure);
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

bool Context::implementEntryPoint(requite::Procedure &procedure) {
  REQUITE_ASSERT(procedure.getType() == requite::ProcedureType::ENTRY_POINT);
  procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
  requite::Signature &signature = procedure.getSignature();
  requite::Symbol &return_type = signature.getReturnType();
  return_type.getRoot().setAsSigned(this->getAddressDepth());
  requite::Expression &expression = procedure.getExpression();
  if (!expression.getHasBranch()) {
    return true;
  }
  requite::Expression &first_statement = expression.getBranch();
  requite::Contextualizer1 contextualizer1(*this, procedure);
  if (!contextualizer1.implementLocalScope(first_statement)) {
    return false;
  }
  return true;
}

bool Context::implementFunction(requite::Procedure &procedure) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementMethod(requite::Procedure &procedure) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementExtension(requite::Procedure &procedure) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementConstructor(requite::Procedure &procedure) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementDestructor(requite::Procedure &procedure) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementObject(requite::Object &object) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementAlias(requite::Alias &alias) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementGlobal(requite::Global &global) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Context::implementProperty(requite::Property &property) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementLocalScope(
    requite::Expression &first_statement) {
  bool is_ok = true;
  for (requite::Expression &branch : first_statement.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = first_statement.getOpcode()) {
    case requite::Opcode::_INITIALIZE:
      if (!this->implement_Initialize(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_ASSIGN:
      if (!this->implement_Assign(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::SWAP:
      if (!this->implementSwap(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_STRUCTURED_BINDING:
      if (!this->implement_StructuredBinding(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_IGNORE:
      if (!this->implement_Ignore(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_CALL:
      if (!this->implement_Call(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_DESTROY_VALUE:
      if (!this->implement_DestroyValue(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::_DROP_VALUE:
      if (!this->implement_DropValue(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::RETURN:
      if (!this->implementReturn(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::BREAK:
      if (!this->implementBreak(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::CONTINUE:
      if (!this->implementContinue(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::FALLTHROUGH:
      if (!this->implementFallthrough(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::EXIT:
      if (!this->implementExit(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::GOTO:
      if (!this->implementGoto(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::ALIAS:
      if (!this->implementAlias(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::IF:
      if (!this->implementIf(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::ELSE_IF:
      if (!this->implementElseIf(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::ELSE:
      if (!this->implementElse(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::SWITCH:
      if (!this->implementSwitch(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::CASE:
      if (!this->implementCase(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::FOR:
      if (!this->implementFor(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::WHILE:
      if (!this->implementWhile(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::DO_WHILE:
      if (!this->implementDoWhile(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::FOR_EACH:
      if (!this->implementForEach(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::LOOP:
      if (!this->implementLoop(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::SCOPE:
      if (!this->implementScope(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::IMPORT:
      if (!this->implementImport(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::USE:
      if (!this->implementUse(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::ASSERT:
      if (!this->implementAssert(branch)) {
        is_ok = false;
      }
      break;
    case requite::Opcode::UNREACHABLE:
      if (!this->implementUnreachable(branch)) {
        is_ok = false;
      }
      break;
    default:
      REQUITE_UNREACHABLE();
    }
  }
  return is_ok;
}

bool Contextualizer1::implement_Initialize(requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_INITIALIZE);
  requite::Expression &lhs = statement.getBranch();
  if (!requite::getCanBeSymbolBindingSituation(lhs.getOpcode())) {
    return true;
  }
  llvm::StringRef name;
  switch (const requite::EvaluationResult result =
              this->getContext().evaluateName(
                  name, this->getScope(), lhs,
                  requite::LookupMode::UNFOUND_SYMBOL_IS_ERROR)) {
  case requite::EvaluationResult::LITERAL:
    [[fallthrough]];
  case requite::EvaluationResult::GENERATED:
    break;
  case requite::EvaluationResult::GENERATED_NOT_DONE:
    [[fallthrough]];
  case requite::EvaluationResult::ERROR:
    return false;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (!found.getIsNone()) {
    return true;
  }
  statement.changeOpcode(requite::Opcode::_LOCAL);
  requite::Local *local_ptr;
  if (!this->tabulate_Local(local_ptr, name, statement)) {
    return false;
  }
  requite::Local &local = requite::getRef(local_ptr);
  requite::Expression &rhs = lhs.getNext();
  requite::Symbol &type = local.getDataType();
  if (!this->inferenceTypeOfValue(type, rhs)) {
    return false;
  }
  this->getContext().finalizeIfLiteralType(type);
  return true;
}

bool Contextualizer1::implement_Assign(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementSwap(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implement_StructuredBinding(
    requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implement_Ignore(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implement_Call(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implement_DestroyValue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implement_DropValue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementReturn(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementBreak(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementContinue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementFallthrough(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementExit(requite::Expression &statement) {
  requite::Symbol type;
  requite::Expression &branch = statement.getBranch();
  if (!this->inferenceTypeOfValue(type, branch)) {
    return false;
  }
  if (!type.getIsInteger() ||
      type.getRoot().getDepth() != this->getContext().getAddressDepth()) {
    this->getContext().logSourceMessage(branch, requite::LogType::ERROR,
                                        "invalid exit code type");
    return false;
  }
  return true;
}

bool Contextualizer1::implementGoto(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementAlias(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementIf(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementElseIf(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementElse(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementSwitch(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementCase(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementFor(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementWhile(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementDoWhile(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementForEach(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementLoop(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementScope(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementImport(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementUse(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementAssert(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::implementUnreachable(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

} // namespace requite