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
  new_expression._next_ptr_flags = expression._next_ptr_flags;
  new_expression._keyword = expression._keyword;
  new_expression._source_ptr_flags = expression._source_ptr_flags;
  new_expression._source_text_length = expression._source_text_length;
  return new_expression;
}

} // namespace rq
