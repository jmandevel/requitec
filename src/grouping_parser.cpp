#include <requite/assert.hpp>
#include <requite/grouping_parser.hpp>

namespace requite {

GroupingParser::GroupingParser(requite::Parser &parser) : _parser_ref(parser) {}

requite::Parser &GroupingParser::getParser() { return this->_parser_ref.get(); }

const requite::Parser &GroupingParser::getParser() const {
  return this->_parser_ref.get();
}

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

} // namespace requite