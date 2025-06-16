// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/anonymous_object.hpp>
#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/label.hpp>
#include <requite/module.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/object.hpp>
#include <requite/ordered_variable.hpp>
#include <requite/procedure.hpp>
#include <requite/signature.hpp>
#include <requite/symbol.hpp>
#include <requite/table.hpp>
#include <requite/tuple.hpp>
#include <requite/unordered_variable.hpp>

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
  default:
    this->_data_ptr = that._data_ptr;
  }
}

RootSymbol::~RootSymbol() {
  switch (const requite::RootSymbolType type = this->getType()) {
  case requite::RootSymbolType::SIGNATURE:
    delete this->_signature_ptr;
    break;
  case requite::RootSymbolType::TUPLE:
    delete this->_tuple_ptr;
    break;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    delete this->_anonymous_object_ptr;
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
  default:
    REQUITE_UNREACHABLE();
  }
}

bool RootSymbol::operator!=(const requite::RootSymbol &rhs) const {
  return !(*this == rhs);
}

requite::RootSymbol RootSymbol::makeUser(requite::Object &object) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::OBJECT);
  root.setObject(object);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::Table &table) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::TABLE);
  root.setTable(table);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::Alias &alias) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::ALIAS);
  root.setAlias(alias);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::OrderedVariable &variable) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::ORDERED_VARIABLE);
  root.setOrderedVariable(variable);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::UnorderedVariable &variable) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::UNORDERED_VARIABLE);
  root.setUnorderedVariable(variable);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::Procedure &procedure) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::PROCEDURE);
  root.setProcedure(procedure);
  return requite::RootSymbol(root);
}

requite::RootSymbol
RootSymbol::makeUser(requite::NamedProcedureGroup &procedure_group) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::NAMED_PROCEDURE_GROUP);
  root.setNamedProcedureGroup(procedure_group);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::Module &module) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::MODULE);
  root.setModule(module);
  return requite::RootSymbol(root);
}

requite::RootSymbol RootSymbol::makeUser(requite::Label &label) {
  requite::RootSymbol root;
  root.setType(requite::RootSymbolType::LABEL);
  root.setLabel(label);
  return requite::RootSymbol(root);
}

void RootSymbol::setType(requite::RootSymbolType type) {
  REQUITE_ASSERT(this->getIsNone());
  REQUITE_ASSERT(type != requite::RootSymbolType::NONE);
  this->_type = type;
  switch (type) {
  case requite::RootSymbolType::SIGNATURE:
    this->_signature_ptr = new requite::Signature();
    break;
  case requite::RootSymbolType::TUPLE:
    this->_tuple_ptr = new requite::Tuple();
    break;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    this->_anonymous_object_ptr = new requite::AnonymousObject();
    break;
  default:
    break;
  }
}

requite::RootSymbolType RootSymbol::getType() const { return this->_type; }

bool RootSymbol::getHasDepth() const {
  REQUITE_ASSERT(requite::getHasDepth(this->getType()));
  return this->_depth != 0;
}

void RootSymbol::setDepth(unsigned depth) {
  REQUITE_ASSERT(requite::getHasDepth(this->getType()));
  REQUITE_ASSERT(this->_depth == 0);
  REQUITE_ASSERT(depth != 0);
  this->_depth = depth;
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
  return this->_type == requite::RootSymbolType::SIGNED_INTEGER;
}

bool RootSymbol::getIsUnsignedInteger() const {
  return this->_type == requite::RootSymbolType::UNSIGNED_INTEGER;
}

bool RootSymbol::getIsBinaryHalfFloat() const {
  return this->_type == requite::RootSymbolType::BINARY_HALF_FLOAT;
}

bool RootSymbol::getIsBinarySingleFloat() const {
  return this->_type == requite::RootSymbolType::BINARY_SINGLE_FLOAT;
}

bool RootSymbol::getIsBinaryDoubleFloat() const {
  return this->_type == requite::RootSymbolType::BINARY_DOUBLE_FLOAT;
}

bool RootSymbol::getIsBinaryQuadFloat() const {
  return this->_type == requite::RootSymbolType::BINARY_QUAD_FLOAT;
}

bool RootSymbol::getIsCChar() const {
  return this->_type == requite::RootSymbolType::C_CHAR;
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

bool RootSymbol::getIsOrderedVariable() const {
  return this->_type == requite::RootSymbolType::ORDERED_VARIABLE;
}

bool RootSymbol::getIsUnorderedVariable() const {
  return this->_type == requite::RootSymbolType::UNORDERED_VARIABLE;
}

bool RootSymbol::getIsProcedure() const {
  return this->_type == requite::RootSymbolType::PROCEDURE;
}

bool RootSymbol::getIsNamedProcedureGroup() const {
  return this->_type == requite::RootSymbolType::NAMED_PROCEDURE_GROUP;
}

bool RootSymbol::getIsModule() const {
  return this->_type == requite::RootSymbolType::MODULE;
}

bool RootSymbol::getIsLabel() const {
  return this->_type == requite::RootSymbolType::LABEL;
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

void RootSymbol::setScope(requite::Scope &scope) {
  requite::setSingleRef(this->_scope_ptr, scope);
}

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

void RootSymbol::setObject(requite::Object &object) {
  REQUITE_ASSERT(this->getIsObject());
  requite::setSingleRef(this->_object_ptr, object);
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

void RootSymbol::setTable(requite::Table &table) {
  REQUITE_ASSERT(this->getIsTable());
  requite::setSingleRef(this->_table_ptr, table);
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

void RootSymbol::setAlias(requite::Alias &alias) {
  REQUITE_ASSERT(this->getIsAlias());
  requite::setSingleRef(this->_alias_ptr, alias);
}

const requite::Alias &RootSymbol::getAlias() const {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

requite::Alias &RootSymbol::getAlias() {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

bool RootSymbol::getHasOrderedVariable() const {
  REQUITE_ASSERT(this->getIsOrderedVariable());
  return this->_ordered_variable_ptr != nullptr;
}

void RootSymbol::setOrderedVariable(requite::OrderedVariable &variable) {
  REQUITE_ASSERT(this->getIsOrderedVariable());
  requite::setSingleRef(this->_ordered_variable_ptr, variable);
}

requite::OrderedVariable &RootSymbol::getOrderedVariable() {
  REQUITE_ASSERT(this->getIsOrderedVariable());
  return requite::getRef(this->_ordered_variable_ptr);
}

const requite::OrderedVariable &RootSymbol::getOrderedVariable() const {
  REQUITE_ASSERT(this->getIsOrderedVariable());
  return requite::getRef(this->_ordered_variable_ptr);
}

bool RootSymbol::getHasUnorderedVariable() const {
  REQUITE_ASSERT(this->getIsUnorderedVariable());
  return this->_unordered_variable_ptr != nullptr;
}

void RootSymbol::setUnorderedVariable(requite::UnorderedVariable &variable) {
  REQUITE_ASSERT(this->getIsUnorderedVariable());
  requite::setSingleRef(this->_unordered_variable_ptr, variable);
}

requite::UnorderedVariable &RootSymbol::getUnorderedVariable() {
  REQUITE_ASSERT(this->getIsUnorderedVariable());
  return requite::getRef(this->_unordered_variable_ptr);
}

const requite::UnorderedVariable &RootSymbol::getUnorderedVariable() const {
  REQUITE_ASSERT(this->getIsUnorderedVariable());
  return requite::getRef(this->_unordered_variable_ptr);
}

bool RootSymbol::getHasProcedure() const {
  REQUITE_ASSERT(this->getIsProcedure());
  return this->_procedure_ptr != nullptr;
}

void RootSymbol::setProcedure(requite::Procedure &procedure) {
  REQUITE_ASSERT(this->getIsProcedure());
  requite::setSingleRef(this->_procedure_ptr, procedure);
}

const requite::Procedure &RootSymbol::getProcedure() const {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

requite::Procedure &RootSymbol::getProcedure() {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

bool RootSymbol::getHasNamedProcedureGroup() const {
  REQUITE_ASSERT(this->getIsNamedProcedureGroup());
  return this->_named_procedure_group_ptr != nullptr;
}

void RootSymbol::setNamedProcedureGroup(
    requite::NamedProcedureGroup &named_procedure_group) {
  REQUITE_ASSERT(this->getIsNamedProcedureGroup());
  requite::setSingleRef(this->_named_procedure_group_ptr,
                        named_procedure_group);
}

const requite::NamedProcedureGroup &RootSymbol::getNamedProcedureGroup() const {
  REQUITE_ASSERT(this->getIsNamedProcedureGroup());
  return requite::getRef(this->_named_procedure_group_ptr);
}

requite::NamedProcedureGroup &RootSymbol::getNamedProcedureGroup() {
  REQUITE_ASSERT(this->getIsNamedProcedureGroup());
  return requite::getRef(this->_named_procedure_group_ptr);
}

bool RootSymbol::getHasModule() const {
  REQUITE_ASSERT(this->getIsModule());
  return this->_module_ptr != nullptr;
}

void RootSymbol::setModule(requite::Module &module) {
  REQUITE_ASSERT(this->getIsModule());
  requite::setSingleRef(this->_module_ptr, module);
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

void RootSymbol::setLabel(requite::Label &label) {
  REQUITE_ASSERT(this->getIsLabel());
  requite::setSingleRef(this->_label_ptr, label);
}

const requite::Label &RootSymbol::getLabel() const {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
}

requite::Label &RootSymbol::getLabel() {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
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
  case requite::RootSymbolType::UNORDERED_VARIABLE: {
    requite::UnorderedVariable &variable = this->getUnorderedVariable();
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