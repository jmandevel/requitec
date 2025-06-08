// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline void Expression::clearData() { this->_data.emplace<std::monostate>(); }

inline bool Expression::getHasDataText() const {
  return std::holds_alternative<std::string>(this->_data);
}

inline llvm::StringRef Expression::getDataText() const {
  REQUITE_ASSERT(requite::getHasTextData(this->getOpcode()));
  return llvm::StringRef(std::get<std::string>(this->_data));
}

inline void Expression::setDataText(llvm::StringRef text) {
  REQUITE_ASSERT(requite::getHasTextData(this->getOpcode()));
  this->_data.emplace<std::string>(text.str());
}

inline void Expression::changeDataText(llvm::StringRef text) {
  REQUITE_ASSERT(requite::getHasTextData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasDataText());
  std::get<std::string>(this->_data).assign(text.str());
}

inline bool Expression::getHasScope() const {
  REQUITE_ASSERT(requite::getHasScopeData(this->getOpcode()));
  return std::holds_alternative<requite::Scope *>(this->_data) &&
         std::get<requite::Scope *>(this->_data) != nullptr;
}

inline requite::Scope &Expression::getScope() {
  REQUITE_ASSERT(requite::getHasScopeData(this->getOpcode()));
  return requite::getRef(std::get<requite::Scope *>(this->_data));
}

inline const requite::Scope &Expression::getScope() const {
  REQUITE_ASSERT(requite::getHasScopeData(this->getOpcode()));
  return requite::getRef(std::get<requite::Scope *>(this->_data));
}

inline void Expression::setScope(requite::Scope &scope) {
  REQUITE_ASSERT(requite::getHasScopeData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasScope());
  this->_data.emplace<requite::Scope *>(&scope);
}

inline bool Expression::getHasObject() const {
  REQUITE_ASSERT(requite::getHasObjectData(this->getOpcode()));
  return std::holds_alternative<requite::Object *>(this->_data) &&
         std::get<requite::Object *>(this->_data) != nullptr;
}

inline requite::Object &Expression::getObject() {
  REQUITE_ASSERT(requite::getHasObjectData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasObject());
  return requite::getRef(std::get<requite::Object *>(this->_data));
}

inline const requite::Object &Expression::getObject() const {
  REQUITE_ASSERT(requite::getHasObjectData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasObject());
  return requite::getRef(std::get<requite::Object *>(this->_data));
}

inline void Expression::setObject(requite::Object &object) {
  REQUITE_ASSERT(requite::getHasObjectData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasObject());
  this->_data.emplace<requite::Object *>(&object);
}

inline bool Expression::getHasProcedure() const {
  REQUITE_ASSERT(requite::getHasOverloadData(this->getOpcode()));
  return std::holds_alternative<requite::Procedure *>(this->_data) &&
         std::get<requite::Procedure *>(this->_data) != nullptr;
}

inline requite::Procedure &Expression::getProcedure() {
  REQUITE_ASSERT(requite::getHasOverloadData(this->getOpcode()));
  return requite::getRef(std::get<requite::Procedure *>(this->_data));
}

inline const requite::Procedure &Expression::getProcedure() const {
  REQUITE_ASSERT(requite::getHasOverloadData(this->getOpcode()));
  return requite::getRef(std::get<requite::Procedure *>(this->_data));
}

inline void Expression::setProcedure(requite::Procedure &procedure) {
  REQUITE_ASSERT(requite::getHasOverloadData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasProcedure());
  this->_data.emplace<requite::Procedure *>(&procedure);
}

inline bool Expression::getHasLabel() const {
  REQUITE_ASSERT(requite::getHasLabelData(this->getOpcode()));
  return std::holds_alternative<requite::Label *>(this->_data) &&
         std::get<requite::Label *>(this->_data) != nullptr;
}

inline void Expression::setLabel(requite::Label &label) {
  REQUITE_ASSERT(requite::getHasLabelData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasLabel());
  this->_data.emplace<requite::Label *>(&label);
}

inline requite::Label &Expression::getLabel() {
  REQUITE_ASSERT(requite::getHasLabelData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasLabel());
  return requite::getRef(std::get<requite::Label *>(this->_data));
}

inline const requite::Label &Expression::getLabel() const {
  REQUITE_ASSERT(requite::getHasLabelData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasLabel());
  return requite::getRef(std::get<requite::Label *>(this->_data));
}

inline bool Expression::getHasAnonymousFunction() const {
  REQUITE_ASSERT(requite::getHasAnonymousFunctionData(this->getOpcode()));
  return std::holds_alternative<requite::AnonymousFunction *>(this->_data) &&
         std::get<requite::AnonymousFunction *>(this->_data) != nullptr;
}

inline void
Expression::setAnonymousFunction(requite::AnonymousFunction &function) {
  REQUITE_ASSERT(requite::getHasAnonymousFunctionData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasAnonymousFunction());
  this->_data.emplace<requite::AnonymousFunction *>(&function);
}

inline requite::AnonymousFunction &Expression::getAnonymousFunction() {
  REQUITE_ASSERT(requite::getHasAnonymousFunctionData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasAnonymousFunction());
  return requite::getRef(std::get<requite::AnonymousFunction *>(this->_data));
}

inline const requite::AnonymousFunction &
Expression::getAnonymousFunction() const {
  REQUITE_ASSERT(requite::getHasAnonymousFunctionData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasAnonymousFunction());
  return requite::getRef(std::get<requite::AnonymousFunction *>(this->_data));
}

inline bool Expression::getHasAlias() const {
  REQUITE_ASSERT(requite::getHasAliasData(this->getOpcode()));
  return std::holds_alternative<requite::Alias *>(this->_data) &&
         std::get<requite::Alias *>(this->_data) != nullptr;
}

inline void Expression::setAlias(requite::Alias &alias) {
  REQUITE_ASSERT(requite::getHasAliasData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasAlias());
  this->_data.emplace<requite::Alias *>(&alias);
}

inline requite::Alias &Expression::getAlias() {
  REQUITE_ASSERT(requite::getHasAliasData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasAlias());
  return requite::getRef(std::get<requite::Alias *>(this->_data));
}

inline const requite::Alias &Expression::getAlias() const {
  REQUITE_ASSERT(requite::getHasAliasData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasAlias());
  return requite::getRef(std::get<requite::Alias *>(this->_data));
}

inline void Expression::setVariable(requite::Variable &variable) {
  REQUITE_ASSERT(requite::getHasVariableData(this->getOpcode()));
  this->_data.emplace<requite::Variable *>(&variable);
}

inline bool Expression::getHasVariable() const {
  REQUITE_ASSERT(requite::getHasVariableData(this->getOpcode()));
  return std::holds_alternative<requite::Variable *>(this->_data) &&
         std::get<requite::Variable *>(this->_data) != nullptr;
}

inline requite::Variable &Expression::getVariable() {
  REQUITE_ASSERT(requite::getHasVariableData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasVariable());
  return requite::getRef(std::get<requite::Variable *>(this->_data));
}

inline const requite::Variable &Expression::getVariable() const {
  REQUITE_ASSERT(requite::getHasVariableData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasVariable());
  return requite::getRef(std::get<requite::Variable *>(this->_data));
}

inline bool Expression::getHasInteger() const {
  REQUITE_ASSERT(requite::getHasIntegerData(this->getOpcode()));
  return std::holds_alternative<llvm::APSInt>(this->_data);
}

inline llvm::APSInt &Expression::emplaceInteger() {
  REQUITE_ASSERT(requite::getHasIntegerData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasInteger());
  return this->_data.emplace<llvm::APSInt>();
}

inline llvm::APSInt &Expression::getInteger() {
  REQUITE_ASSERT(this->getHasInteger());
  return std::get<llvm::APSInt>(this->_data);
}

inline const llvm::APSInt &Expression::getInteger() const {
  REQUITE_ASSERT(this->getHasInteger());
  return std::get<llvm::APSInt>(this->_data);
}

} // namespace requite