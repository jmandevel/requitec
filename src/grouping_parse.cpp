#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/grouping_parser.hpp>

namespace requite {

void GroupingParser::startGroup(requite::Opcode opcode,
                                const requite::Token &first_token) {
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(first_token);
  this->setOperation(operation);
}

void GroupingParser::startGroup(requite::Opcode opcode,
                                requite::Expression &first_branch) {
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(first_branch);
  this->setOperation(operation);
}

void GroupingParser::appendBranch(requite::Expression &branch) {
    if (this->_last_ptr == nullptr) {
        requite::Expression& operation = this->getOperation();
        operation.setBranch(branch);
        this->_last_ptr = &branch;
        return;
    }
    requite::Expression& last = requite::getRef(this->_last_ptr);
    last.setNext(branch);
    this->_last_ptr = &branch;
}

requite::Expression &
GroupingParser::finishOperation(const requite::Token &last_token) {
    requite::Expression& operation = this->getOperation();
    operation.extendSourceOver(last_token);
    return operation;
}

} // namespace requite