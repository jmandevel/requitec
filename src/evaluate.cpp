#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/literals.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

void Evaluator::evaluateSourceModule() {
  rq::Module &source = this->getContext().getSourceModule();
  this->rundown(source);
  if (!this->getIsOk()) {
    return;
  }
  this->infill(source);
}
void Evaluator::rundown(rq::Module &module) {
  const rq::Expression &top_ex = module.getExpression();
  if (!top_ex.getHasBranch()) {
    return;
  }
  const rq::Expression &first_ex = top_ex.getBranch();
  this->rundownScope(this->getContext().getTop(), module, first_ex);
}

void Evaluator::rundownScope(rq::SymbolTable &table, rq::Module &module,
                             const rq::Expression &first_ex) {
  std::ignore = table;
  std::ignore = module;
  for (const rq::Expression &outer_ex : first_ex.getInclusiveNextSubrange()) {
    rq::ExpressionFlagsFactory factory;
    const rq::Expression &statement_ex =
        this->evaluateExpressionAttributes(factory, table, module, outer_ex);
    if (!this->getIsOk()) {
      return;
    }
    std::ignore = statement_ex;
  }
}

void Evaluator::infill(rq::Module &module) { std::ignore = module; }

[[nodiscard]] rq::Rvalue
Evaluator::evaluateRvalue(rq::SymbolTable &table, rq::Module &module,
                          const rq::Expression &rvalue_ex,
                          rq::SymbolConstant *lvalue_sy_ptr) {
  std::ignore = table;
  std::ignore = module;
  std::ignore = rvalue_ex;
  std::ignore = lvalue_sy_ptr;
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] const rq::Expression &Evaluator::evaluateExpressionAttributes(
    rq::ExpressionFlagsFactory &out_factory, rq::SymbolTable &table,
    rq::Module &module, const rq::Expression &outer_ex) {
  if (outer_ex.getKeyword() != rq::Keyword::ASCRIBE_EXPRESSION) {
    return outer_ex;
  }
  for (const rq::Expression &branch_ex : outer_ex.getBranchSubrange()) {
    if (!branch_ex.getHasNext()) {
      return branch_ex;
    }
    const rq::Expression &attribute_ex = branch_ex.getBranch();
    rq::Rvalue rvalue =
        this->evaluateRvalue(table, module, attribute_ex, nullptr);
    if (!rvalue.getIsOk()) {
      continue;
    }
    rq::Symbol &attribute_sy = rq::dereferencePtr(rvalue.getSymbol());
    if (!attribute_sy.getIsExpressionAttributeType()) {
      this->getContext().logErrorUnexpectedRvalueType(attribute_ex);
      this->setNotOk();
      continue;
    }
    rq::WordConstant &attribute_wd =
        llvm::cast<rq::WordConstant>(rvalue.getValue());
    rq::ExpressionAttribute attribute =
        attribute_wd.getAs<rq::ExpressionAttribute>();
    out_factory.addFlag(attribute, &branch_ex);
  }
  RQ_UNREACHABLE();
}

} // namespace rq
