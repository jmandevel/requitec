// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/anonymous_object.hpp>
#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/global.hpp>
#include <requite/label.hpp>
#include <requite/local.hpp>
#include <requite/module.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/signature.hpp>
#include <requite/symbol.hpp>
#include <requite/table.hpp>
#include <requite/tuple.hpp>

namespace requite {

RootSymbol::RootSymbol(const requite::RootSymbol &that)
    : _type(that._type), _depth(that._depth) {
  switch (const requite::RootSymbolType type = that.getType()) {
  case requite::RootSymbolType::SIGNATURE:
    this->_signature_ptr = new requite::Signature(that.getSignature());
    break;
  case requite::RootSymbolType::TUPLE:
    this->_tuple_ptr = new requite::Tuple(that.getTuple());
    break;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    this->_anonymous_object_ptr =
        new requite::AnonymousObject(that.getAnonymousObject());
    break;
  case requite::RootSymbolType::SCOPE:
    this->_scope_ptr = that._scope_ptr;
    break;
  case requite::RootSymbolType::OBJECT:
    this->_object_ptr = that._object_ptr;
    break;
  case requite::RootSymbolType::TABLE:
    this->_table_ptr = that._table_ptr;
    break;
  case requite::RootSymbolType::ALIAS:
    this->_alias_ptr = that._alias_ptr;
    break;
  case requite::RootSymbolType::LOCAL:
    this->_local_ptr = that._local_ptr;
    break;
  case requite::RootSymbolType::GLOBAL:
    this->_global_ptr = that._global_ptr;
    break;
  case requite::RootSymbolType::PROCEDURE:
    this->_procedure_ptr = that._procedure_ptr;
    break;
  case requite::RootSymbolType::MODULE:
    this->_module_ptr = that._module_ptr;
    break;
  case requite::RootSymbolType::LABEL:
    this->_label_ptr = that._label_ptr;
    break;
  default:
    break;
  }
}

RootSymbol::~RootSymbol() {
  switch (const requite::RootSymbolType type = this->getType()) {
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    delete this->_anonymous_object_ptr;
    break;
  case requite::RootSymbolType::TUPLE:
    delete this->_tuple_ptr;
    break;
  case requite::RootSymbolType::SIGNATURE:
    delete this->_signature_ptr;
    break;
  default:
    break;
  }
}

requite::RootSymbol &RootSymbol::operator=(const requite::RootSymbol &rhs) {
  if (&rhs != this) {
    *this = requite::RootSymbol(rhs);
  }
  return *this;
}

bool RootSymbol::operator==(const requite::RootSymbol &rhs) const {
  const bool non_data_same =
      this->_type == rhs._type && this->_depth == rhs._depth;
  if (!non_data_same) {
    return false;
  }
  switch (const requite::RootSymbolType type = this->getType()) {
  case requite::RootSymbolType::SIGNATURE:
    return this->getSignature() == rhs.getSignature();
  case requite::RootSymbolType::TUPLE:
    return this->getTuple() == rhs.getTuple();
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return this->getAnonymousObject() == rhs.getAnonymousObject();
  case requite::RootSymbolType::SCOPE:
    return this->getScope() == rhs.getScope();
  case requite::RootSymbolType::OBJECT:
    return this->getObject() == rhs.getObject();
  case requite::RootSymbolType::TABLE:
    return this->getTable() == rhs.getTable();
  case requite::RootSymbolType::ALIAS:
    return this->getAlias() == rhs.getAlias();
  case requite::RootSymbolType::LOCAL:
    return this->getLocal() == rhs.getLocal();
  case requite::RootSymbolType::GLOBAL:
    return this->getGlobal() == rhs.getGlobal();
  case requite::RootSymbolType::PROCEDURE:
    return this->getProcedure() == rhs.getProcedure();
  case requite::RootSymbolType::MODULE:
    return this->getModule() == rhs.getModule();
  case requite::RootSymbolType::LABEL:
    return this->getLabel() == rhs.getLabel();
  default:
    REQUITE_UNREACHABLE();
  }
}

bool RootSymbol::operator!=(const requite::RootSymbol &rhs) const {
  return !(*this == rhs);
}

void RootSymbol::setAsUser(requite::Object &object) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::OBJECT;
  this->_object_ptr = &object;
}

void RootSymbol::setAsUser(requite::Table &table) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::TABLE;
  this->_table_ptr = &table;
}

void RootSymbol::setAsUser(requite::Alias &alias) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::ALIAS;
  this->_alias_ptr = &alias;
}

void RootSymbol::setAsUser(requite::Local &variable) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::LOCAL;
  this->_local_ptr = &variable;
}

void RootSymbol::setAsUser(requite::Global &variable) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::GLOBAL;
  this->_global_ptr = &variable;
}

void RootSymbol::setAsUser(requite::Property &property) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::PROPERTY;
  this->_property_ptr = &property;
}

void RootSymbol::setAsUser(requite::Procedure &procedure) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::PROCEDURE;
  this->_procedure_ptr = &procedure;
}

void RootSymbol::setAsUser(requite::Module &module) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::MODULE;
  this->_module_ptr = &module;
}

void RootSymbol::setAsUser(requite::Label &label) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::LABEL;
  this->_label_ptr = &label;
}

void RootSymbol::setAsUser(requite::Import &import) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::IMPORT;
  this->_import_ptr = &import;
}

void RootSymbol::setAsUser(requite::Use &use) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::USE;
  this->_use_ptr = &use;
}

void RootSymbol::setAsInference() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::INFERENCE;
}

void RootSymbol::setAsVoid() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::VOID;
}

void RootSymbol::setAsVariadicArguments() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::VARIADIC_ARGUMENTS;
}

void RootSymbol::setAsNull_() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::NULL_;
}

void RootSymbol::setAsIntegerLiteral() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::INTEGER_LITERAL;
}

void RootSymbol::setAsFractionalLiteral() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::FRACTIONAL_LITERAL;
}

void RootSymbol::setAsCodeunitLiteral() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::CODEUNIT_LITERAL;
}

void RootSymbol::setAsStringLiteral() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::STRING_LITERAL;
}

void RootSymbol::setAsBoolean() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BOOLEAN;
}

void RootSymbol::setAsWord(unsigned depth) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::WORD;
  this->_depth = depth;
}

void RootSymbol::setAsSigned(unsigned depth) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::SIGNED;
  this->_depth = depth;
}

void RootSymbol::setAsUnsigned(unsigned depth) {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::UNSIGNED;
  this->_depth = depth;
}

void RootSymbol::setAsBFloat16() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BFLOAT16;
}

void RootSymbol::setAsBinary16() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BFLOAT16;
}

void RootSymbol::setAsBinary32() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BINARY32;
}

void RootSymbol::setAsBinary64() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BINARY64;
}

void RootSymbol::setAsBinary128() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::BINARY128;
}

void RootSymbol::setAsUtf8() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::UTF8;
}

void RootSymbol::setAsSignature() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::SIGNATURE;
  this->_signature_ptr = new requite::Signature();
}

void RootSymbol::setAsTuple() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::TUPLE;
  this->_tuple_ptr = new requite::Tuple();
}

void RootSymbol::setAsAnonymousObject() {
  REQUITE_ASSERT(!this->getHasAllocation());
  this->_type = requite::RootSymbolType::ANONYMOUS_OBJECT;
  this->_anonymous_object_ptr = new requite::AnonymousObject();
}

requite::RootSymbolType RootSymbol::getType() const { return this->_type; }

bool RootSymbol::getHasDepth() const {
  REQUITE_ASSERT(requite::getHasDepth(this->getType()));
  return this->_depth != 0;
}

unsigned RootSymbol::getDepth() const {
  REQUITE_ASSERT(requite::getHasDepth(this->getType()));
  REQUITE_ASSERT(this->_depth != 0);
  return this->_depth;
}

bool RootSymbol::getIsNone() const {
  return this->_type == requite::RootSymbolType::NONE;
}

bool RootSymbol::getIsInference() const {
  return this->_type == requite::RootSymbolType::INFERENCE;
}

bool RootSymbol::getIsVoid() const {
  return this->_type == requite::RootSymbolType::VOID;
}

bool RootSymbol::getIsVariadicArguments() const {
  return this->_type == requite::RootSymbolType::VARIADIC_ARGUMENTS;
}

bool RootSymbol::getIsBoolean() const {
  return this->_type == requite::RootSymbolType::BOOLEAN;
}

bool RootSymbol::getIsWord() const {
  return this->_type == requite::RootSymbolType::WORD;
}

bool RootSymbol::getIsSignedInteger() const {
  return this->_type == requite::RootSymbolType::SIGNED;
}

bool RootSymbol::getIsUnsignedInteger() const {
  return this->_type == requite::RootSymbolType::UNSIGNED;
}

bool RootSymbol::getIsBFloat16() const {
  return this->_type == requite::RootSymbolType::BFLOAT16;
}

bool RootSymbol::getIsBinary16() const {
  return this->_type == requite::RootSymbolType::BINARY16;
}

bool RootSymbol::getIsBinary32() const {
  return this->_type == requite::RootSymbolType::BINARY32;
}

bool RootSymbol::getIsBinary64() const {
  return this->_type == requite::RootSymbolType::BINARY64;
}

bool RootSymbol::getIsBinary128() const {
  return this->_type == requite::RootSymbolType::BINARY128;
}

bool RootSymbol::getIsUTF8() const {
  return this->_type == requite::RootSymbolType::UTF8;
}

bool RootSymbol::getIsSignature() const {
  return this->_type == requite::RootSymbolType::SIGNATURE;
}

bool RootSymbol::getIsTuple() const {
  return this->_type == requite::RootSymbolType::TUPLE;
}

bool RootSymbol::getIsAnonymousObject() const {
  return this->_type == requite::RootSymbolType::ANONYMOUS_OBJECT;
}

bool RootSymbol::getIsScope() const {
  return this->_type == requite::RootSymbolType::SCOPE;
}

bool RootSymbol::getIsObject() const {
  return this->_type == requite::RootSymbolType::OBJECT;
}

bool RootSymbol::getIsTable() const {
  return this->_type == requite::RootSymbolType::TABLE;
}

bool RootSymbol::getIsAlias() const {
  return this->_type == requite::RootSymbolType::ALIAS;
}

bool RootSymbol::getIsLocal() const {
  return this->_type == requite::RootSymbolType::LOCAL;
}

bool RootSymbol::getIsGlobal() const {
  return this->_type == requite::RootSymbolType::GLOBAL;
}

bool RootSymbol::getIsProcedure() const {
  return this->_type == requite::RootSymbolType::PROCEDURE;
}

bool RootSymbol::getIsModule() const {
  return this->_type == requite::RootSymbolType::MODULE;
}

bool RootSymbol::getIsLabel() const {
  return this->_type == requite::RootSymbolType::LABEL;
}

bool RootSymbol::getIsImport() const {
  return this->_type == requite::RootSymbolType::IMPORT;
}

bool RootSymbol::getIsUse() const {
  return this->_type == requite::RootSymbolType::USE;
}

bool RootSymbol::getHasAllocation() const {
  return this->getIsAnonymousObject() || this->getIsTuple() ||
         this->getIsSignature();
}

const requite::Signature &RootSymbol::getSignature() const {
  REQUITE_ASSERT(this->getIsSignature());
  return requite::getRef(this->_signature_ptr);
}

requite::Signature &RootSymbol::getSignature() {
  REQUITE_ASSERT(this->getIsSignature());
  return requite::getRef(this->_signature_ptr);
}

const requite::Tuple &RootSymbol::getTuple() const {
  REQUITE_ASSERT(this->getIsTuple());
  return requite::getRef(this->_tuple_ptr);
}

requite::Tuple &RootSymbol::getTuple() {
  REQUITE_ASSERT(this->getIsTuple());
  return requite::getRef(this->_tuple_ptr);
}

const requite::AnonymousObject &RootSymbol::getAnonymousObject() const {
  REQUITE_ASSERT(this->getIsAnonymousObject());
  return requite::getRef(this->_anonymous_object_ptr);
}

requite::AnonymousObject &RootSymbol::getAnonymousObject() {
  REQUITE_ASSERT(this->getIsAnonymousObject());
  return requite::getRef(this->_anonymous_object_ptr);
}

bool RootSymbol::getHasScope() const { return this->_scope_ptr != nullptr; }

requite::Scope &RootSymbol::getScope() {
  REQUITE_ASSERT(this->getIsScope());
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &RootSymbol::getScope() const {
  REQUITE_ASSERT(this->getIsScope());
  return requite::getRef(this->_scope_ptr);
}

bool RootSymbol::getHasObject() const {
  REQUITE_ASSERT(this->getIsObject());
  return this->_object_ptr != nullptr;
}

const requite::Object &RootSymbol::getObject() const {
  REQUITE_ASSERT(this->getIsObject());
  return requite::getRef(this->_object_ptr);
}

requite::Object &RootSymbol::getObject() {
  REQUITE_ASSERT(this->getIsObject());
  return requite::getRef(this->_object_ptr);
}

bool RootSymbol::getHasTable() const {
  REQUITE_ASSERT(this->getIsTable());
  return this->_table_ptr != nullptr;
}

const requite::Table &RootSymbol::getTable() const {
  REQUITE_ASSERT(this->getIsTable());
  return requite::getRef(this->_table_ptr);
}

requite::Table &RootSymbol::getTable() {
  REQUITE_ASSERT(this->getIsTable());
  return requite::getRef(this->_table_ptr);
}

bool RootSymbol::getHasAlias() const {
  REQUITE_ASSERT(this->getIsAlias());
  return this->_alias_ptr != nullptr;
}

const requite::Alias &RootSymbol::getAlias() const {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

requite::Alias &RootSymbol::getAlias() {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

bool RootSymbol::getHasLocal() const {
  REQUITE_ASSERT(this->getIsLocal());
  return this->_local_ptr != nullptr;
}

requite::Local &RootSymbol::getLocal() {
  REQUITE_ASSERT(this->getIsLocal());
  return requite::getRef(this->_local_ptr);
}

const requite::Local &RootSymbol::getLocal() const {
  REQUITE_ASSERT(this->getIsLocal());
  return requite::getRef(this->_local_ptr);
}

bool RootSymbol::getHasGlobal() const {
  REQUITE_ASSERT(this->getIsGlobal());
  return this->_global_ptr != nullptr;
}

requite::Global &RootSymbol::getGlobal() {
  REQUITE_ASSERT(this->getIsGlobal());
  return requite::getRef(this->_global_ptr);
}

const requite::Global &RootSymbol::getGlobal() const {
  REQUITE_ASSERT(this->getIsGlobal());
  return requite::getRef(this->_global_ptr);
}

bool RootSymbol::getHasProcedure() const {
  REQUITE_ASSERT(this->getIsProcedure());
  return this->_procedure_ptr != nullptr;
}

const requite::Procedure &RootSymbol::getProcedure() const {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

requite::Procedure &RootSymbol::getProcedure() {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

bool RootSymbol::getHasModule() const {
  REQUITE_ASSERT(this->getIsModule());
  return this->_module_ptr != nullptr;
}

const requite::Module &RootSymbol::getModule() const {
  REQUITE_ASSERT(this->getIsModule());
  return requite::getRef(this->_module_ptr);
}

requite::Module &RootSymbol::getModule() {
  REQUITE_ASSERT(this->getIsModule());
  return requite::getRef(this->_module_ptr);
}

bool RootSymbol::getHasLabel() const {
  REQUITE_ASSERT(this->getIsLabel());
  return this->_label_ptr != nullptr;
}

const requite::Label &RootSymbol::getLabel() const {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
}

requite::Label &RootSymbol::getLabel() {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
}

const requite::Import &RootSymbol::getImport() const {
  REQUITE_ASSERT(this->getIsImport());
  return requite::getRef(this->_import_ptr);
}

requite::Import &RootSymbol::getImport() {
  REQUITE_ASSERT(this->getIsImport());
  return requite::getRef(this->_import_ptr);
}

const requite::Use &RootSymbol::getUse() const {
  REQUITE_ASSERT(this->getIsUse());
  return requite::getRef(this->_use_ptr);
}

requite::Use &RootSymbol::getUse() {
  REQUITE_ASSERT(this->getIsUse());
  return requite::getRef(this->_use_ptr);
}

requite::AttributeFlags &RootSymbol::getUserAttributeFlags() {
  REQUITE_ASSERT(requite::getHasUserAttributeFlags(this->getType()));
  switch (this->getType()) {
  case requite::RootSymbolType::OBJECT: {
    requite::Object &object = this->getObject();
    return object.getAttributeFlags();
  } break;
  case requite::RootSymbolType::ALIAS: {
    requite::Alias &alias = this->getAlias();
    return alias.getAttributeFlags();
  } break;
  case requite::RootSymbolType::GLOBAL: {
    requite::Global &variable = this->getGlobal();
    return variable.getAttributeFlags();
  } break;
  case requite::RootSymbolType::PROCEDURE: {
    requite::Procedure &procedure = this->getProcedure();
    return procedure.getAttributeFlags();
  } break;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite