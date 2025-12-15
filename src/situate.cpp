#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

namespace rq {

void Situator::logErrorNotAtLeastBranchCount(rq::Situation situation,
                                             rq::Expression &expression,
                                             unsigned count) {
  this->getContext().logMessage(
      expression.getLlvmSourceStart(), rq::LogType::ERROR,
      llvm::Twine(rq::getDescription(situation)) + " " +
          rq::getName(expression.getKeyword()) + " must have at least " +
          llvm::Twine(count) + " branches.\n",
      {expression.getLlvmSourceRange()}, {});
}
void Situator::logErrorNotExactBranchCount(rq::Situation situation,
                                           rq::Expression &expression,
                                           unsigned count) {
  this->getContext().logMessage(
      expression.getLlvmSourceStart(), rq::LogType::ERROR,
      llvm::Twine(rq::getDescription(situation)) + " " +
          rq::getName(expression.getKeyword()) + " must have exactly " +
          llvm::Twine(count) + " branches.\n",
      {expression.getLlvmSourceRange()}, {});
}
void Situator::logErrorTooManyBranchCount(rq::Situation situation,
                                          rq::Expression &expression,
                                          unsigned max_count) {
  this->getContext().logMessage(
      expression.getLlvmSourceStart(), rq::LogType::ERROR,
      llvm::Twine(rq::getDescription(situation)) + " " +
          rq::getName(expression.getKeyword()) + " must have no more than " +
          llvm::Twine(max_count) + " branches.\n",
      {expression.getLlvmSourceRange()}, {});
}
void Situator::logErrorInvalidBranchSituation(
    rq::Situation situation, rq::Situation branch_situation,
    rq::Expression &branch, rq::Keyword outer_keyword,
    rq::Keyword branch_keyword, unsigned branch_i, llvm::Twine log_context) {
  this->getContext().logMessage(
      branch.getLlvmSourceStart(), rq::LogType::ERROR,
      llvm::Twine(rq::getDescription(branch_situation)) +
          " expression expected for " + log_context + " of " +
          rq::getDescription(situation) + " " + rq::getName(outer_keyword) +
          " but found " + rq::getName(branch_keyword) + " at index " +
          llvm::Twine(branch_i) + ".\n",
      {branch.getLlvmSourceRange()}, {});
}
void Situator::situateModule(rq::Module &module) {
  // TODO
}
void Situator::situateTree(rq::Situation situation, rq::Expression &expression) {
  // TODO
}

} // namespace rq