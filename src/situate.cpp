#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <utility>

namespace rq {

rq::Expression &Situator::makeModuleRoot(rq::Module &module) {
  rq::Expression &root = this->getStaticFrame().acquireExpression();
  root.setIsInserted();
  root.setSource(module);
  return root;
}

void Situator::situateModule(rq::Module &module) {
  if (!module.getHasExpression()) {
    rq::Expression &root = this->makeModuleRoot(module);
    module.setExpression(root);
    return;
  }
  rq::Expression &first = module.getExpression();
  if (first.getKeyword() != rq::Keyword::S_MODULE_ROOT) {
    rq::Expression &root = this->makeModuleRoot(module);
    std::ignore = module.popExpression();
    root.setBranch(first);
  }
  rq::Expression &root = module.getExpression();
  this->situateTree(rq::Situation::ROOT_STATEMENT, root);
}

#define RQ_ASSERT_VALID_SITUATION(situation, expression)                       \
  RQ_ASSERT(expression.getCanBeSituation(situation),                           \
            "keyword not valid for situation")

void Situator::situateTree(rq::Situation situation,
                           rq::Expression &expression) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
                          /*
  using S = rq::Situation;
  using K = rq::Keyword;
  switch (expression.getKeyword()) {
  case K::I_NONE:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    this->setNotOk();
    return;

  // LITERALS
  case K::I_INTEGER_LITERAL:
    [[fallthrough]];
  case K::I_FLOAT_LITERAL:
    [[fallthrough]];
  case K::I_STRING_LITERAL:
    [[fallthrough]];
  case K::I_LEFT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_MIDDLE_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_RIGHT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_CODEUNIT_LITERAL:
    [[fallthrough]];
  case K::I_IDENTIFIER_LITERAL:
    return;

  // ERRORS
  case K::I_ERROR:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    this->setNotOk();
    return;

  // SITUATIONAL
  case K::S_PARENTHESIS_GROUP:
    this->situateUnaryExpression(situation, expression, situation);
    this->getStaticFrame().discardExpression(expression.mergeAndPopBranch());
    break;
  }
    */
  RQ_UNREACHABLE();
}

void Situator::situateUnaryExpression(rq::Situation situation,
                                      rq::Expression &expression,
                                      rq::Situation branch0_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    //this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    this->setNotOk();
    return;
  }
  rq::Expression& branch0 = expression.getBranch();
  //this->situateBranch(situation, expression, branch0_situation, branch0, 0, "first branch");
  if (branch0.getHasNext()) {
    //this->logErrorNotExactBranchCount(situation, expression, 1);
    this->setNotOk();
    return;
  }
}

} // namespace rq
