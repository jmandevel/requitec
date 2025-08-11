#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/grouping_parser.hpp>

namespace requite {

bool GroupingParser::getHasOperation() const {
  return this->_operation_ptr != nullptr;
}

void GroupingParser::setOperation(requite::Expression &operation) {
  requite::setSingleRef(this->_operation_ptr, operation);
}

requite::Expression &GroupingParser::getOperation() {
  return requite::getRef(this->_operation_ptr);
}

const requite::Expression &GroupingParser::getOperation() const {
  return requite::getRef(this->_operation_ptr);
}

requite::Expression &GroupingParser::getLast() {
  return requite::getRef(this->_last_ptr);
}

const requite::Expression &GroupingParser::getLast() const {
  return requite::getRef(this->_last_ptr);
}

} // namespace requite