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
  this->evaluateModule(source);
  if (!this->getIsOk()) {
    return;
  }
  this->evaluateAllModuleSymbols(source);
}
void Evaluator::evaluateModule(rq::Module &module) {
  rq::Expression &top_ex = module.getExpression();
  if (!top_ex.getHasBranch()) {
    return;
  }
  rq::Expression &first_ex = top_ex.getBranch();
  this->evaluateGlobalScope(this->getContext().getTop(), module, first_ex);
}

void Evaluator::evaluateGlobalScope(rq::SymbolTable &table, rq::Module &module,
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

void Evaluator::evaluateAllModuleSymbols(rq::Module &module) {
  std::ignore = module;
}

[[nodiscard]] rq::StaticRvalue
Evaluator::evaluateStaticRvalue(rq::SymbolTable &table, rq::Module &module,
                                rq::Expression &rvalue_ex) {
  using K = rq::Keyword;
  using S = rq::SymbolKind;
  switch (rvalue_ex.getKeyword()) {
  case K::INTEGER_LITERAL: {
    rq::Symbol &type = this->getContext().getIntegerLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::StaticRvalue(type, value);
  }
  case K::FLOAT_LITERAL: {
    rq::Symbol &type = this->getContext().getFloatLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::StaticRvalue(type, value);
  }
  case K::STRING_LITERAL: {
    rq::Symbol &type = this->getContext().getStringLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::StaticRvalue(type, value);
  }
  case K::CODEUNIT_LITERAL: {
    rq::Symbol &type = this->getContext().getCodeunitLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::StaticRvalue(type, value);
  }
  case K::IDENTIFIER_LITERAL: {
    auto list = table.findNamedList(rvalue_ex.getSourceText());
    if (list.getIsEmpty()) {
      this->getContext().logErrorNotSymbol(rvalue_ex);
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (list.getHasTail()) {
      this->getContext().logErrorNameCollision(rvalue_ex);
      this->setNotOk();
      return rq::StaticRvalue();
    }
    rq::Symbol &found_sy = list.getHead();
    rq::Symbol *rvalue_sy_ptr = nullptr;
    if (llvm::isa<rq::Polymorph>(found_sy)) {
      rq::Polymorph &poly = llvm::cast<rq::Polymorph>(found_sy);
      if (!poly.getHasSomeInstance()) {
        this->getContext().logErrorNotSymbol(rvalue_ex);
        this->setNotOk();
        return rq::StaticRvalue();
      }
      if (poly.getHasCollision()) {
        this->getContext().logErrorNameCollision(rvalue_ex);
        this->setNotOk();
        return rq::StaticRvalue();
      }
      rvalue_sy_ptr = &poly.getOnlyInstance();
    } else {
      rvalue_sy_ptr = &found_sy;
    }
    rq::Symbol &rvalue_sy = rq::dereferencePtr(rvalue_sy_ptr);
    std::ignore = rvalue_sy;
    //switch (rvalue_sy.getKind()) {
    //}
  }
  case K::ADD: {

    }
    default: break;
  }

  std::ignore = table;
  std::ignore = module;
  std::ignore = rvalue_ex;
  RQ_TODO_IMPLEMENTATION();
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
    rq::StaticRvalue rvalue =
        this->evaluateStaticRvalue(table, module, attribute_ex);
    if (!rvalue.getIsOk()) {
      continue;
    }
    rq::Symbol &attribute_ty = rvalue.getType();
    if (!attribute_ty.getIsExpressionAttributeType()) {
      this->getContext().logErrorUnexpectedRvalueType(attribute_ex);
      this->setNotOk();
      continue;
    }
    if (rvalue.getHasTemp()) {
      rq::StaticValue &value = rvalue.getTemp();
      rq::StaticInt =
    }
    rq::WordConstant &attribute_wd =
        llvm::cast<rq::WordConstant>(rvalue.getEntity());
    rq::ExpressionAttribute attribute =
        attribute_wd.getAs<rq::ExpressionAttribute>();
    out_factory.addFlag(attribute, &branch_ex);
  }
  RQ_UNREACHABLE();
}

} // namespace rq
