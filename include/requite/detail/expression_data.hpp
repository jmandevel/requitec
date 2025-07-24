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

inline bool Expression::getHasTable() const {
  REQUITE_ASSERT(requite::getHasTableData(this->getOpcode()));
  return std::holds_alternative<requite::Table *>(this->_data) &&
         std::get<requite::Table *>(this->_data) != nullptr;
}

inline requite::Table &Expression::getTable() {
  REQUITE_ASSERT(requite::getHasTableData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTable());
  return requite::getRef(std::get<requite::Table *>(this->_data));
}

inline const requite::Table &Expression::getTable() const {
  REQUITE_ASSERT(requite::getHasTableData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTable());
  return requite::getRef(std::get<requite::Table *>(this->_data));
}

inline void Expression::setTable(requite::Table &table) {
  REQUITE_ASSERT(requite::getHasTableData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasTable());
  this->_data.emplace<requite::Table *>(&table);
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

inline void Expression::setGlobal(requite::Global &variable) {
  REQUITE_ASSERT(requite::getHasGlobalData(this->getOpcode()));
  this->_data.emplace<requite::Global *>(&variable);
}

inline bool Expression::getHasGlobal() const {
  REQUITE_ASSERT(requite::getHasGlobalData(this->getOpcode()));
  return std::holds_alternative<requite::Global *>(this->_data) &&
         std::get<requite::Global *>(this->_data) != nullptr;
}

inline requite::Global &Expression::getGlobal() {
  REQUITE_ASSERT(requite::getHasGlobalData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasGlobal());
  return requite::getRef(std::get<requite::Global *>(this->_data));
}

inline const requite::Global &Expression::getGlobal() const {
  REQUITE_ASSERT(requite::getHasGlobalData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasGlobal());
  return requite::getRef(std::get<requite::Global *>(this->_data));
}

inline void Expression::setProperty(requite::Property &property) {
  REQUITE_ASSERT(requite::getHasPropertyData(this->getOpcode()));
  REQUITE_ASSERT(!this->getHasProperty());
  this->_data.emplace<requite::Property *>(&property);
}

inline bool Expression::getHasProperty() const {
  REQUITE_ASSERT(requite::getHasPropertyData(this->getOpcode()));
  return std::holds_alternative<requite::Property *>(this->_data) &&
         std::get<requite::Property *>(this->_data) != nullptr;
}

inline requite::Property &Expression::getProperty() {
  REQUITE_ASSERT(requite::getHasPropertyData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasProperty());
  return requite::getRef(std::get<requite::Property *>(this->_data));
}

inline const requite::Property &Expression::getProperty() const {
  REQUITE_ASSERT(requite::getHasPropertyData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasProperty());
  return requite::getRef(std::get<requite::Property *>(this->_data));
}

inline void Expression::setLocal(requite::Local &variable) {
  REQUITE_ASSERT(requite::getHasLocalData(this->getOpcode()));
  this->_data.emplace<requite::Local *>(&variable);
}

inline bool Expression::getHasLocal() const {
  REQUITE_ASSERT(requite::getHasLocalData(this->getOpcode()));
  return std::holds_alternative<requite::Local *>(this->_data) &&
         std::get<requite::Local *>(this->_data) != nullptr;
}

inline requite::Local &Expression::getLocal() {
  REQUITE_ASSERT(requite::getHasLocalData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasLocal());
  return requite::getRef(std::get<requite::Local *>(this->_data));
}

inline const requite::Local &Expression::getLocal() const {
  REQUITE_ASSERT(requite::getHasLocalData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasLocal());
  return requite::getRef(std::get<requite::Local *>(this->_data));
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

inline bool Expression::getHasImport() const {
  REQUITE_ASSERT(requite::getHasImportData(this->getOpcode()));
  return std::holds_alternative<requite::Import *>(this->_data);
}

inline void Expression::setImport(requite::Import &import) {
  REQUITE_ASSERT(requite::getHasImportData(this->getOpcode()));
  this->_data.emplace<requite::Import *>(&import);
}

inline requite::Import &Expression::getImport() {
  REQUITE_ASSERT(requite::getHasImportData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasImport());
  return requite::getRef(std::get<requite::Import *>(this->_data));
}

inline const requite::Import &Expression::getImport() const {
  REQUITE_ASSERT(requite::getHasImportData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasImport());
  return requite::getRef(std::get<requite::Import *>(this->_data));
}

inline bool Expression::getHasUse() const {
  REQUITE_ASSERT(requite::getHasUseData(this->getOpcode()));
  return std::holds_alternative<requite::Use *>(this->_data);
}

inline void Expression::setUse(requite::Use &use) {
  REQUITE_ASSERT(requite::getHasUseData(this->getOpcode()));
  this->_data.emplace<requite::Use *>(&use);
}

inline requite::Use &Expression::getUse() {
  REQUITE_ASSERT(requite::getHasUseData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasUse());
  return requite::getRef(std::get<requite::Use *>(this->_data));
}

inline const requite::Use &Expression::getUse() const {
  REQUITE_ASSERT(requite::getHasUseData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasUse());
  return requite::getRef(std::get<requite::Use *>(this->_data));
}

inline bool Expression::getHasTableAlias() const {
  REQUITE_ASSERT(requite::getHasTableAliasData(this->getOpcode()));
  return std::holds_alternative<requite::TableAlias *>(this->_data);
}

inline void Expression::setTableAlias(requite::TableAlias &alias) {
  REQUITE_ASSERT(requite::getHasTableAliasData(this->getOpcode()));
  this->_data.emplace<requite::TableAlias *>(&alias);
}

inline requite::TableAlias &Expression::getTableAlias() {
  REQUITE_ASSERT(requite::getHasTableAliasData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTableAlias());
  return requite::getRef(std::get<requite::TableAlias *>(this->_data));
}

inline const requite::TableAlias &Expression::getTableAlias() const {
  REQUITE_ASSERT(requite::getHasTableAliasData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTableAlias());
  return requite::getRef(std::get<requite::TableAlias *>(this->_data));
}

inline bool Expression::getHasTableUse() const {
  REQUITE_ASSERT(requite::getHasTableUseData(this->getOpcode()));
  return std::holds_alternative<requite::TableUse *>(this->_data);
}

inline void Expression::setTableUse(requite::TableUse &use) {
  REQUITE_ASSERT(requite::getHasTableUseData(this->getOpcode()));
  this->_data.emplace<requite::TableUse *>(&use);
}

inline requite::TableUse &Expression::getTableUse() {
  REQUITE_ASSERT(requite::getHasTableUseData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTableUse());
  return requite::getRef(std::get<requite::TableUse *>(this->_data));
}

inline const requite::TableUse &Expression::getTableUse() const {
  REQUITE_ASSERT(requite::getHasTableUseData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasTableUse());
  return requite::getRef(std::get<requite::TableUse *>(this->_data));
}

inline bool Expression::getHasBlock() const {
  REQUITE_ASSERT(requite::getHasBlockData(this->getOpcode()));
  return std::holds_alternative<requite::Block *>(this->_data);
}

inline void Expression::setBlock(requite::Block &block) {
  REQUITE_ASSERT(requite::getHasBlockData(this->getOpcode()));
  this->_data.emplace<requite::Block *>(&block);
}

inline requite::Block &Expression::getBlock() {
  REQUITE_ASSERT(requite::getHasBlockData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasBlock());
  return requite::getRef(std::get<requite::Block *>(this->_data));
}

inline const requite::Block &Expression::getBlock() const {
  REQUITE_ASSERT(requite::getHasBlockData(this->getOpcode()));
  REQUITE_ASSERT(this->getHasBlock());
  return requite::getRef(std::get<requite::Block *>(this->_data));
}

} // namespace requite