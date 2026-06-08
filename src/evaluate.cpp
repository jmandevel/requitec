#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/expressions.hpp>
#include <rq/literals.hpp>
#include <rq/symbols.hpp>
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
  rq::Expression &top_ex = module.getExpression();
  if (!top_ex.getHasBranch()) {
    return;
  }
  rq::Expression &first_ex = top_ex.getBranch();
  this->rundownScope(this->getContext().getTop(), module, first_ex);
}

void Evaluator::rundownScope(rq::SymbolTable &table, rq::Module &module,
                             rq::Expression &first_ex) {
  std::ignore = table;
  std::ignore = module;
  for (rq::Expression &outer_ex : first_ex.getInclusiveNextSubrange()) {
    rq::ExpressionFlagsFactory factory;
    rq::Expression &statement_ex =
        this->evaluateExpressionAttributes(factory, table, module, outer_ex);
    if (!this->getIsOk()) {
      return;
    }
    std::ignore = statement_ex;
  }
}

void Evaluator::infill(rq::Module &module) { std::ignore = module; }

[[nodiscard]] rq::Rvalue Evaluator::evaluateRvalue(rq::SymbolTable &table,
                                                   rq::Module &module,
                                                   rq::Expression &rvalue_ex) {
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  switch (rvalue_ex.getKeyword()) {
  case K::INTEGER_LITERAL: {
    rq::Symbol &type = this->getContext().getIntegerLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  } break;
  case K::FLOAT_LITERAL: {
    rq::Symbol &type = this->getContext().getFloatLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  }
  default:
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::Expression &Evaluator::evaluateExpressionAttributes(
    rq::ExpressionFlagsFactory &out_factory, rq::SymbolTable &table,
    rq::Module &module, rq::Expression &outer_ex) {
  if (outer_ex.getKeyword() != rq::Keyword::ASCRIBE_EXPRESSION) {
    return outer_ex;
  }
  for (rq::Expression &branch_ex : outer_ex.getBranchSubrange()) {
    if (!branch_ex.getHasNext()) {
      return branch_ex;
    }
    rq::Expression &attribute_ex = branch_ex.getBranch();
    rq::Rvalue rvalue = this->evaluateRvalue(table, module, attribute_ex);
    if (!rvalue.getIsOk()) {
      continue;
    }
    rq::Symbol &attribute_ty = rvalue.getType();
    if (!attribute_ty.getIsExpressionAttributeType()) {
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
