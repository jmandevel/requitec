// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/implementor.hpp>
#include <requite/signature.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <vector>

namespace requite {
  
bool Context::implementAll() {
  // implement them in order to try to decrease recursive implementations
  bool is_ok = true;
  for (std::unique_ptr<requite::Alias> &alias_uptr : this->getAliasUptrs()) {
    requite::Alias &alias = requite::getRef(alias_uptr);
    if (!this->implementAlias(alias)) {
      return false;
    }
  }
  for (std::unique_ptr<requite::Global> &global_uptr : this->getGlobalUptrs()) {
    requite::Global &global = requite::getRef(global_uptr);
    if (!this->implementGlobal(global)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Property> &property_uptr :
       this->getPropertyUptrs()) {
    requite::Property &property = requite::getRef(property_uptr);
    if (!this->implementProperty(property)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Procedure> &procedure_uptr :
       this->getProcedureUptrs()) {
    requite::Procedure &procedure = requite::getRef(procedure_uptr);
    if (!this->implementProcedure(procedure)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Object> &object_uptr : this->getObjectUptrs()) {
    requite::Object &object = requite::getRef(object_uptr);
    if (!this->implementObject(object)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::checkEntryPointCount() {
  requite::Module &source_module = this->getSourceModule();
  if (!source_module.getHasEntryPoint()) {
    return true;
  }
  requite::Procedure &entry_point = source_module.getEntryPoint();
  if (!entry_point.getHasNextProcedure()) {
    return true;
  }
  for (requite::Procedure &overload : entry_point.getOverloadSubrange()) {
    this->logSourceMessage(overload.getExpression(), requite::LogType::ERROR,
                           "multiple entry points in module.");
  }
  return false;
}

bool Context::implementProcedure(requite::Procedure &procedure) {
  switch (const requite::ProcedureType type = procedure.getType()) {
  case requite::ProcedureType::ENTRY_POINT:
    return this->implementEntryPoint(procedure);
  case requite::ProcedureType::FUNCTION:
    return this->implementFunction(procedure);
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
  requite::Implementor implementor(*this, procedure);
  if (!implementor.implementMatteLocalStatement(first_statement)) {
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

bool Implementor::implementMatteLocalStatement(
    requite::Expression &first_statement) {
  bool is_ok = true;
  for (requite::Expression &branch : first_statement.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = first_statement.getOpcode()) {
    case requite::Opcode::_LOCAL:
      if (!this->implement_Local(branch)) {
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
    case requite::Opcode::_ALIAS:
      if (!this->implement_Alias(branch)) {
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

bool Implementor::implement_Local(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_Assign(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementSwap(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_StructuredBinding(
    requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_Ignore(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_Call(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_DestroyValue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implement_DropValue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementReturn(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementBreak(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementContinue(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementFallthrough(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementExit(requite::Expression &statement) {
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

bool Implementor::implement_Alias(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementIf(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementElseIf(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementElse(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementSwitch(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementCase(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementFor(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementWhile(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementDoWhile(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementLoop(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementScope(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementImport(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementUse(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementAssert(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Implementor::implementUnreachable(requite::Expression &statement) {
  REQUITE_UNREACHABLE(); // TODO
}

} // namespace requite