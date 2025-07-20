#include <requite/alias.hpp>
#include <requite/assert.hpp>
#include <requite/global.hpp>
#include <requite/label.hpp>
#include <requite/local.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/property.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

UserSymbol::UserSymbol(requite::Alias &alias, requite::Module &module)
    : _type(requite::UserSymbolType::ALIAS), _alias_ptr(&alias),
      _is_exported(alias.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Global &global, requite::Module &module)
    : _type(requite::UserSymbolType::GLOBAL), _global_ptr(&global),
      _is_exported(global.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Label &label, requite::Module &module)
    : _type(requite::UserSymbolType::LABEL), _label_ptr(&label),
      _is_exported(false), _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Local &local, requite::Module &module)
    : _type(requite::UserSymbolType::LOCAL), _local_ptr(&local),
      _is_exported(false), _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Object &object, requite::Module &module)
    : _type(requite::UserSymbolType::OBJECT), _object_ptr(&object),
      _is_exported(object.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Property &property, requite::Module &module)
    : _type(requite::UserSymbolType::PROPERTY), _property_ptr(&property),
      _is_exported(false), _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Procedure &procedure, requite::Module &module)
    : _type(requite::UserSymbolType::PROCEDURE), _procedure_ptr(&procedure),
      _is_exported(procedure.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

bool UserSymbol::operator==(const requite::Alias &rhs) const {
  return this->getAlias() == rhs;
}

bool UserSymbol::operator!=(const requite::Alias &rhs) const {
  return this->getAlias() != rhs;
}

bool UserSymbol::operator==(const requite::Global &rhs) const {
  return this->getGlobal() == rhs;
}

bool UserSymbol::operator!=(const requite::Global &rhs) const {
  return this->getGlobal() != rhs;
}

bool UserSymbol::operator==(const requite::Label &rhs) const {
  return this->getLabel() == rhs;
}

bool UserSymbol::operator!=(const requite::Label &rhs) const {
  return this->getLabel() != rhs;
}

bool UserSymbol::operator==(const requite::Local &rhs) const {
  return this->getLocal() == rhs;
}

bool UserSymbol::operator!=(const requite::Local &rhs) const {
  return this->getLocal() != rhs;
}

bool UserSymbol::operator==(const requite::Object &rhs) const {
  return this->getObject() == rhs;
}

bool UserSymbol::operator!=(const requite::Object &rhs) const {
  return this->getObject() != rhs;
}

bool UserSymbol::operator==(const requite::Property &rhs) const {
  return this->getProperty() == rhs;
}

bool UserSymbol::operator!=(const requite::Property &rhs) const {
  return this->getProperty() != rhs;
}

bool UserSymbol::operator==(const requite::Procedure &rhs) const {
  return this->getProcedure() == rhs;
}

bool UserSymbol::operator!=(const requite::Procedure &rhs) const {
  return this->getProcedure() != rhs;
}

requite::UserSymbolType UserSymbol::getType() const {
  return this->_type;
}

bool UserSymbol::getIsExported() const { return this->_is_exported; }

requite::Module &UserSymbol::getContainingModule() {
  return requite::getRef(this->_containing_module_ptr);
}

const requite::Module &UserSymbol::getContainingModule() const {
  return requite::getRef(this->_containing_module_ptr);
}

bool UserSymbol::getIsAlias() const {
  return this->_type == requite::UserSymbolType::ALIAS;
}

requite::Alias &UserSymbol::getAlias() {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

const requite::Alias &UserSymbol::getAlias() const {
  REQUITE_ASSERT(this->getIsAlias());
  return requite::getRef(this->_alias_ptr);
}

bool UserSymbol::getIsGlobal() const {
  return this->_type == requite::UserSymbolType::GLOBAL;
}

requite::Global &UserSymbol::getGlobal() {
  REQUITE_ASSERT(this->getIsGlobal());
  return requite::getRef(this->_global_ptr);
}

const requite::Global &UserSymbol::getGlobal() const {
  REQUITE_ASSERT(this->getIsGlobal());
  return requite::getRef(this->_global_ptr);
}

bool UserSymbol::getIsLabel() const {
  return this->_type == requite::UserSymbolType::LABEL;
}

requite::Label &UserSymbol::getLabel() {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
}

const requite::Label &UserSymbol::getLabel() const {
  REQUITE_ASSERT(this->getIsLabel());
  return requite::getRef(this->_label_ptr);
}

bool UserSymbol::getIsLocal() const {
  return this->_type == requite::UserSymbolType::LOCAL;
}

requite::Local &UserSymbol::getLocal() {
  REQUITE_ASSERT(this->getIsLocal());
  return requite::getRef(this->_local_ptr);
}

const requite::Local &UserSymbol::getLocal() const {
  REQUITE_ASSERT(this->getIsLocal());
  return requite::getRef(this->_local_ptr);
}

bool UserSymbol::getIsObject() const {
  return this->_type == requite::UserSymbolType::OBJECT;
}

requite::Object &UserSymbol::getObject() {
  REQUITE_ASSERT(this->getIsObject());
  return requite::getRef(this->_object_ptr);
}

const requite::Object &UserSymbol::getObject() const {
  REQUITE_ASSERT(this->getIsObject());
  return requite::getRef(this->_object_ptr);
}

bool UserSymbol::getIsProperty() const {
  return this->_type == requite::UserSymbolType::PROPERTY;
}

requite::Property &UserSymbol::getProperty() {
  REQUITE_ASSERT(this->getIsProperty());
  return requite::getRef(this->_property_ptr);
}

const requite::Property &UserSymbol::getProperty() const {
  REQUITE_ASSERT(this->getIsProperty());
  return requite::getRef(this->_property_ptr);
}

bool UserSymbol::getIsProcedure() const {
  return this->_type == requite::UserSymbolType::PROCEDURE;
}

requite::Procedure &UserSymbol::getProcedure() {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &UserSymbol::getProcedure() const {
  REQUITE_ASSERT(this->getIsProcedure());
  return requite::getRef(this->_procedure_ptr);
}

} // namespace requite