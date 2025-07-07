#include <requite/assert.hpp>
#include <requite/object_block.hpp>

namespace requite {

requite::Scope &ObjectBlock::getScope() { return this->_scope; }

const requite::Scope &ObjectBlock::getScope() const { return this->_scope; }

bool ObjectBlock::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void ObjectBlock::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &ObjectBlock::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &ObjectBlock::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool ObjectBlock::getHasProperty() const {
  return this->_first_property_ptr != nullptr;
}

void ObjectBlock::addProperty(requite::Property &property) {
  requite::setSingleRef(this->_first_property_ptr, property);
}
requite::Property &ObjectBlock::getFirstProperty() {
  return requite::getRef(this->_first_property_ptr);
}

const requite::Property &ObjectBlock::getFirstProperty() const {
  return requite::getRef(this->_first_property_ptr);
}

void ObjectBlock::addGlobal(requite::Global &global) {
  requite::setSingleRef(this->_first_global_ptr, global);
}
requite::Global &ObjectBlock::getFirstGlobal() {
  return requite::getRef(this->_first_global_ptr);
}

const requite::Global &ObjectBlock::getFirstGlobal() const {
  return requite::getRef(this->_first_global_ptr);
}

requite::AttributeFlags &ObjectBlock::getAttributeFlags() {
  return this->_attribute_flags;
}

} // namespace requite