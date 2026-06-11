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
  this->evaluate(source);
  if (!this->getIsOk()) {
    return;
  }
  this->evaluateAllModuleSymbols(source);
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

void Evaluator::evaluate(rq::Module &module) {
  rq::Expression &top_ex = module.getExpression();
  if (!top_ex.getHasBranch()) {
    return;
  }
  rq::Expression &first_ex = top_ex.getBranch();
  this->evaluateGlobalScope(this->getContext().getTop(), module, first_ex);
}

void Evaluator::evaluate(rq::Destructor &destructor) {
  std::ignore = destructor;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::Main &main) {
  std::ignore = main;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::ClassType &class_) {
  std::ignore = class_;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::EnumerationType &enum_) { std::ignore = enum_; }

void Evaluator::evaluate(rq::Interface &interface) {
  std::ignore = interface;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::Adapter &adapter) {
  std::ignore = adapter;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::GlobalDynamicVariable &var) {
  std::ignore = var;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::GlobalStaticVariable &var) {
  std::ignore = var;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::ForwardRanger &ranger) {
  std::ignore = ranger;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::BackwardRanger &ranger) {
  std::ignore = ranger;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::Function &func) {
  std::ignore = func;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::Method &meth) {
  std::ignore = meth;
  RQ_TODO_IMPLEMENTATION();
}

void Evaluator::evaluate(rq::ExtensionMethod &meth) {
  std::ignore = meth;
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] rq::StaticRvalue
Evaluator::evaluateStaticRvalue(rq::SymbolTable &table, rq::Module &module,
                                rq::Expression &rvalue_ex) {
  using K = rq::Keyword;
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
      RQ_UNHANDLED_ERROR("no symbol of name");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (list.getHasTail()) {
      RQ_UNHANDLED_ERROR("name collision");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    rq::Symbol &found_sy = list.getHead();
    rq::Symbol *rvalue_sy_ptr = nullptr;
    if (llvm::isa<rq::Polymorph>(found_sy)) {
      if (found_sy.getIsProcedureRelated()) {
        RQ_UNHANDLED_ERROR("procedure polymorph rvalue");
        this->setNotOk();
        return rq::StaticRvalue();
      }
      rq::Polymorph &poly = llvm::cast<rq::Polymorph>(found_sy);
      if (!poly.getHasSomeInstance()) {
        RQ_UNHANDLED_ERROR("polymorph has no non-template instances");
        this->setNotOk();
        return rq::StaticRvalue();
      }
      if (poly.getHasMultipleInstance()) {
        RQ_UNHANDLED_ERROR("polymorph has multiple non-template instances");
        this->setNotOk();
        return rq::StaticRvalue();
      }
      rvalue_sy_ptr = &poly.getOnlyInstance();
    } else {
      rvalue_sy_ptr = &found_sy;
    }
    rq::Symbol &rvalue_sy = rq::dereferencePtr(rvalue_sy_ptr);
    if (llvm::isa<rq::Label>(rvalue_sy)) {
      rq::Symbol &type = this->getContext().getSymbolType();
      rq::Label &label = llvm::cast<rq::Label>(rvalue_sy);
      return rq::StaticRvalue(type, label);
    }
    if (llvm::isa<rq::Anchor>(rvalue_sy)) {
      rq::Symbol &type = this->getContext().getSymbolType();
      rq::Anchor &anchor = llvm::cast<rq::Anchor>(rvalue_sy);
      return rq::StaticRvalue(type, anchor);
    }
    if (llvm::isa<rq::LocalDynamicVariable>(rvalue_sy)) {
      RQ_UNHANDLED_ERROR("no_static value");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (llvm::isa<rq::LocalStaticVariable>(rvalue_sy)) {
      rq::LocalStaticVariable &var =
          llvm::cast<rq::LocalStaticVariable>(rvalue_sy);
      rq::Symbol &type = var.getType().getSymbol();
      rq::Gendex<rq::StaticValue> gendex = var.getValue();
      if (!gendex.getHasData()) {
        RQ_UNHANDLED_ERROR("invalid memory");
        this->setNotOk();
        return rq::StaticRvalue();
      }
      rq::StaticValue value = gendex.getData();
      return rq::StaticRvalue(type, std::move(value));
    }
    if (llvm::isa<rq::ProcedureArgument>(rvalue_sy)) {
      RQ_UNHANDLED_ERROR("no_static value");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (llvm::isa<rq::TemplateArgument>(rvalue_sy)) {
      rq::TemplateArgument &arg = llvm::cast<rq::TemplateArgument>(rvalue_sy);
      rq::Symbol &type = arg.getType().getSymbol();
      rq::Entity &value = arg.getValue();
      return rq::StaticRvalue(type, value);
    }
    if (llvm::isa<rq::Enumerator>(rvalue_sy)) {
      RQ_UNHANDLED_ERROR("enumerator rvalue");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (llvm::isa<rq::Namespace>(rvalue_sy)) {
      rq::Symbol &type = this->getContext().getSymbolType();
      rq::Namespace &namespace_ = llvm::cast<rq::Namespace>(rvalue_sy);
      return rq::StaticRvalue(type, namespace_);
    }
    if (llvm::isa<rq::ClassType>(rvalue_sy)) {
      rq::ClassType &class_ = llvm::cast<rq::ClassType>(rvalue_sy);
      if (!class_.getIsEvaluated()) {
        this->evaluate(class_);
      }
      rq::Symbol &type = this->getContext().getSymbolType();
      return rq::StaticRvalue(type, class_);
    }
    if (llvm::isa<rq::EnumerationType>(rvalue_sy)) {
      rq::EnumerationType &enum_ = llvm::cast<rq::EnumerationType>(rvalue_sy);
      if (!enum_.getIsEvaluated()) {
        this->evaluate(enum_);
      }
      rq::Symbol &type = this->getContext().getSymbolType();
      return rq::StaticRvalue(type, enum_);
    }
    if (llvm::isa<rq::Interface>(rvalue_sy)) {
      rq::Interface &interface = llvm::cast<rq::Interface>(rvalue_sy);
      if (!interface.getIsEvaluated()) {
        this->evaluate(interface);
      }
      rq::Symbol &type = this->getContext().getSymbolType();
      return rq::StaticRvalue(type, interface);
    }
    if (llvm::isa<rq::Adapter>(rvalue_sy)) {
      rq::Adapter &adapter = llvm::cast<rq::Adapter>(rvalue_sy);
      if (!adapter.getIsEvaluated()) {
        this->evaluate(adapter);
      }
      rq::Symbol &type = this->getContext().getSymbolType();
      return rq::StaticRvalue(type, adapter);
    }
    if (llvm::isa<rq::GlobalDynamicVariable>(rvalue_sy)) {
      RQ_UNHANDLED_ERROR("no_static value");
      this->setNotOk();
      return rq::StaticRvalue();
    }
    if (llvm::isa<rq::GlobalStaticVariable>(rvalue_sy)) {
      rq::GlobalStaticVariable &var =
          llvm::cast<rq::GlobalStaticVariable>(rvalue_sy);
      if (!var.getIsEvaluated()) {
        this->evaluate(var);
      }
      rq::SymbolConstant &type = rq::dereferencePtr(var.getTypePtr());
      rq::Symbol &type_sy = type.getSymbol();
      rq::Entity &value = rq::dereferencePtr(var.getValuePtr());
      return rq::StaticRvalue(type_sy, value);
    }
    RQ_UNREACHABLE();
  }
  case K::ADD: {
  }
  default:
    break;
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
      // rq::StaticValue &value = rvalue.getTemp();
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
