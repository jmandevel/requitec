// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

namespace requite {

bool Context::buildUserSymbols() {
  bool is_ok = true;
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    if (!this->buildUserSymbol(module)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::buildUserSymbol(requite::Module &module) {
  bool is_ok = true;
  for (std::unique_ptr<requite::Object> &object_uptr :
       module.getObjectUptrs()) {
    requite::Object &object = requite::getRef(object_uptr);
    if (!this->buildUserSymbol(object)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Procedure> &procedure_uptr :
       module.getProcedureUptrs()) {
    requite::Procedure &procedure = requite::getRef(procedure_uptr);
    if (!this->buildUserSymbol(procedure)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Alias> &alias_uptr : module.getAliasUptrs()) {
    requite::Alias &alias = requite::getRef(alias_uptr);
    if (!this->buildUserSymbol(alias)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::UnorderedVariable> &unordered_variable_uptr :
       module.getUnorderedVariableUptrs()) {
    requite::UnorderedVariable &variable = requite::getRef(unordered_variable_uptr);
    if (!this->buildUserSymbol(variable)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::AnonymousFunction> &anonymous_function_uptr :
       module.getAnonymousFunctionUptrs()) {
    requite::AnonymousFunction &anonymous_function =
        requite::getRef(anonymous_function_uptr);
    if (!this->buildUserSymbol(anonymous_function)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::buildUserSymbol(requite::Object &object) {
  requite::Expression &expression = object.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

bool Context::buildUserSymbol(requite::Procedure &procedure) {
  switch (const requite::ProcedureType type = procedure.getType()) {
    case requite::ProcedureType::ENTRY_POINT: {
      
    } break;
    default:
      break;
  }
  requite::Expression &expression = procedure.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

bool Context::buildUserSymbol(requite::Alias &alias) {
  requite::Expression &expression = alias.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

bool Context::buildUserSymbol(requite::UnorderedVariable &variable) {
  requite::Expression &expression = variable.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

bool Context::buildUserSymbol(requite::AnonymousFunction &anonymous_function) {
  requite::Expression &expression = anonymous_function.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

} // namespace requite