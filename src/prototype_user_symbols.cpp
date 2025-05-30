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

bool Context::prototypeUserSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->prototypeUserSymbol(module);
  }
  return this->getIsOk();
}

void Context::prototypeUserSymbol(requite::Module &module) {
  for (std::unique_ptr<requite::Object> &object_uptr :
       module.getObjectUptrs()) {
    requite::Object &object = requite::getRef(object_uptr);
    requite::Expression &expression = object.getExpression();
    this->logNotSupportedYet(expression);
    this->setNotOk();
  }
}

void Context::prototypeUserSymbol(requite::Object &object) {
  requite::Expression &expression = object.getExpression();
  this->logNotSupportedYet(expression);
  this->setNotOk();
}

void Context::prototypeUserSymbol(requite::Procedure &procedure) {
  if (procedure.getType() == requite::ProcedureType::ENTRY_POINT) {
    procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
    requite::Signature &signature = procedure.getSignature();
    requite::Symbol &return_type = signature.getReturnType();
    requite::RootSymbol &return_root = return_type.getRoot();
    return_root.setType(requite::RootSymbolType::SIGNED_INTEGER);
    const unsigned pointer_depth = this->getPointerDepth();
    return_root.setDepth(pointer_depth);
    return;
  }
  requite::Expression &expression = procedure.getExpression();
  this->logNotSupportedYet(expression);
  this->setNotOk();
}

void Context::prototypeUserSymbol(requite::Alias &alias) {
  requite::Expression &expression = alias.getExpression();
  this->logNotSupportedYet(expression);
  this->setNotOk();
}

void Context::prototypeUserSymbol(requite::Variable &variable) {
  requite::Expression &expression = variable.getExpression();
  this->logNotSupportedYet(expression);
  this->setNotOk();
}

void Context::prototypeUserSymbol(requite::AnonymousFunction &anonymous_function) {
  requite::Expression &expression = anonymous_function.getExpression();
  this->logNotSupportedYet(expression);
  this->setNotOk();
}

} // namespace requite