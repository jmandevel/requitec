#include <requite/assert.hpp>
#include <requite/global.hpp>
#include <requite/local.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/property.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

UserSymbol::UserSymbol(requite::Global &global, requite::Module &module)
    : _type(requite::UserSymbolType::GLOBAL), _global_ptr(&global),
      _is_exported(global.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

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
      _is_exported(property.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

UserSymbol::UserSymbol(requite::Procedure &procedure, requite::Module &module)
    : _type(requite::UserSymbolType::PROCEDURE), _procedure_ptr(&procedure),
      _is_exported(procedure.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)),
      _containing_module_ptr(&module) {}

bool UserSymbol::operator==(const requite::Global &rhs) const {
  return this->getGlobal() == rhs;
}

bool UserSymbol::operator!=(const requite::Global &rhs) const {
  return this->getGlobal() != rhs;
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

requite::UserSymbolType UserSymbol::getType() const { return this->_type; }

bool UserSymbol::getIsExported() const { return this->_is_exported; }

bool UserSymbol::getHasContainingModule() const {
  return this->_containing_module_ptr != nullptr;
}

void UserSymbol::setContainingModule(requite::Module &module) {
  requite::setSingleRef(this->_containing_module_ptr, module);
}

requite::Module &UserSymbol::getContainingModule() {
  return requite::getRef(this->_containing_module_ptr);
}

const requite::Module &UserSymbol::getContainingModule() const {
  return requite::getRef(this->_containing_module_ptr);
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