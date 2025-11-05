#include <rq/static.hpp>

namespace rq {

rq::Expression &StaticFrame::acquireExpression() {
  if (this->_unused_expression_ptrs.empty()) {
    rq::Expression &new_expression = this->allocateValue<rq::Expression>();
    return new_expression;
  }
  rq::Expression &unused_expression =
      rq::dereferencePtr(this->_unused_expression_ptrs.back());
  this->_unused_expression_ptrs.pop_back();
  unused_expression.clear();
  return unused_expression;
}

rq::Expression &StaticFrame::copyExpression(rq::Expression &expression) {
  rq::Expression &new_expression = rq::dereferencePtr(new rq::Expression());
  if (expression.getHasBranch()) {
    new_expression.setBranch(this->copyExpression(expression.getBranch()));
  }
  if (expression.getHasNext()) {
    new_expression.setNext(this->copyExpression(expression.getNext()));
  }
  new_expression._keyword = expression._keyword;
  new_expression._source_text_ptr = expression._source_text_ptr;
  new_expression._source_text_length = expression._source_text_length;
  return new_expression;
}

void StaticFrame::replaceWithRecursiveCopy(rq::Expression &initial,
                                           rq::Expression &replacement) {
  if (initial.getHasBranch()) {
    rq::Expression &branch = initial.popBranch();
    this->discardExpression(branch);
  }
  if (initial.getHasNext()) {
    rq::Expression &next = initial.popNext();
    this->discardExpression(next);
  }
  if (initial.getHasBranch()) {
    rq::Expression &branch = replacement.getBranch();
    initial.setBranch(this->copyExpression(branch));
  }
  if (replacement.getHasNext()) {
    rq::Expression &next = replacement.getNext();
    initial.setNext(this->copyExpression(next));
  }
  initial.changeKeyword(replacement.getKeyword());
  replacement.setSource(replacement);
}

} // namespace rq