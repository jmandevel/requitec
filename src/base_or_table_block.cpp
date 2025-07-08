#include <requite/assert.hpp>
#include <requite/base_or_table_block.hpp>

namespace requite {

requite::Scope &BaseOrTableBlock::getScope() { return this->_scope; }

const requite::Scope &BaseOrTableBlock::getScope() const {
  return this->_scope;
}

bool BaseOrTableBlock::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void BaseOrTableBlock::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &BaseOrTableBlock::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &BaseOrTableBlock::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool BaseOrTableBlock::getHasGlobal() const {
  return this->_first_global_ptr != nullptr;
}

void BaseOrTableBlock::addGlobal(requite::Global &global) {
  requite::setSingleRef(this->_first_global_ptr, global);
}
requite::Global &BaseOrTableBlock::getFirstGlobal() {
  return requite::getRef(this->_first_global_ptr);
}

const requite::Global &BaseOrTableBlock::getFirstGlobal() const {
  return requite::getRef(this->_first_global_ptr);
}

requite::AttributeFlags &BaseOrTableBlock::getAttributeFlags() {
  return this->_attribute_flags;
}

requite::SymbolStatus BaseOrTableBlock::getStatus() const {
  return this->_status;
}

void BaseOrTableBlock::incrementStatus() {
  REQUITE_ASSERT(this->_status != requite::SymbolStatus::DONE);
  this->_status = static_cast<requite::SymbolStatus>(
      static_cast<unsigned>(this->_status) + 1);
}

} // namespace requtite