#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/grouping_parser.hpp>

namespace requite {

void GroupingParser::startGroup(requite::Expression &existing_expression) {
  if (existing_expression.getHasBranch()) {
    requite::Expression& branch = existing_expression.getBranch();
    REQUITE_ASSERT(!branch.getHasNext());
    this->_last_ptr = &branch;
  }
  this->setOperation(existing_expression);
}

void GroupingParser::appendBranch(requite::Expression &branch) {
  if (this->_last_ptr == nullptr) {
    requite::Expression &operation = this->getOperation();
    operation.setBranch(branch);
    operation.extendSourceOver(branch);
    this->_last_ptr = &branch;
    return;
  }
  requite::Expression &last = requite::getRef(this->_last_ptr);
  last.setNext(branch);
  this->_last_ptr = &branch;
  requite::Expression& operation = this->getOperation();
  operation.extendSourceOver(branch);
}

void
GroupingParser::finishOperation(const requite::Token &last_token) {
  requite::Expression &operation = this->getOperation();
  operation.extendSourceOver(last_token);
}

} // namespace requite