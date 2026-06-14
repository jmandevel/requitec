#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/expressions.hpp>
#include <rq/literals.hpp>
#include <rq/symbols.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

void BinaryInstructionFactory::append(rq::Entity &entity) {
  if (this->_outer_ptr == nullptr) {
    this->_outer_ptr = &entity;
    return;
  }
  if (this->_last_ptr == nullptr) {
    rq::BinaryInstruction &next = this->getContext().acquireBinaryExpression( )
  }
}
[[nodiscard]] rq::BinaryInstruction &
BinaryInstructionFactory::getOuter() {
  return llvm::cast<rq::BinaryInstruction>(rq::dereferencePtr(this->_outer_ptr));
}

void FloatFolder::fold(const llvm::APFloat &value) {
  if (!this->getIsFolding()) {
    this->_is_folding = true;
    this->_float = llvm::APFloat(this->getLlvmFltSemantics());
  }
  using O = rq::Opcode;
  switch (this->getOpcode()) {
  case O::ADD:
    this->_float.add(value, llvm::RoundingMode::NearestTiesToEven);
    break;
  case O::SUBTRACT:
    this->_float.subtract(value, llvm::RoundingMode::NearestTiesToEven);
    break;
  case O::MULTIPLY:
    this->_float.multiply(value, llvm::RoundingMode::NearestTiesToEven);
    break;
  case O::MODULUS:
    RQ_UNHANDLED_ERROR("mod float");
    break;
  default:
    RQ_UNREACHABLE();
  }
}

llvm::APFloat FloatFolder::extract() {
  RQ_ASSERT(this->_is_folding, "not folding");
  this->_is_folding = false;
  return std::move(this->_float);
}

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

[[nodiscard]] rq::Rvalue Evaluator::evaluateRvalue(bool is_static,
                                                   rq::SymbolTable &table,
                                                   rq::Module &module,
                                                   rq::Expression &rvalue_ex) {
  using K = rq::Keyword;
  using O = rq::Opcode;
  switch (rvalue_ex.getKeyword()) {
  case K::INTEGER_LITERAL: {
    rq::Symbol &type = this->getContext().acquireIntegerLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  }
  case K::FLOAT_LITERAL: {
    rq::Symbol &type = this->getContext().acquireFloatLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  }
  case K::STRING_LITERAL: {
    rq::Symbol &type = this->getContext().acquireStringLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  }
  case K::CODEUNIT_LITERAL: {
    rq::Symbol &type = this->getContext().acquireCodeunitLiteralType();
    rq::Entity &value = rvalue_ex;
    return rq::Rvalue(type, value);
  }
  case K::IDENTIFIER_LITERAL:
    return this->evaluateIdentifierLiteralRvalue(is_static, table, module,
                                                 rvalue_ex);
  case K::ADD:
    return this->evaluateArithmeticRvalue(is_static, O::ADD, table, module,
                                          rvalue_ex);
  case K::SUBTRACT:
    return this->evaluateArithmeticRvalue(is_static, O::SUBTRACT, table, module,
                                          rvalue_ex);
  case K::MULTIPLY:
    return this->evaluateArithmeticRvalue(is_static, O::MULTIPLY, table, module,
                                          rvalue_ex);
  case K::MODULUS:
    return this->evaluateArithmeticRvalue(is_static, O::MODULUS, table, module,
                                          rvalue_ex);
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
    rq::Rvalue rvalue = this->evaluateRvalue(true, table, module, attribute_ex);
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
        attribute_wd.getAsInt<rq::ExpressionAttribute>();
    out_factory.addFlag(attribute, &branch_ex);
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::Rvalue Evaluator::evaluateIdentifierLiteralRvalue(
    bool is_static, rq::SymbolTable &table, rq::Module &module,
    rq::Expression &rvalue_ex) {
  std::ignore = module;
  auto list = table.findNamedList(rvalue_ex.getSourceText());
  if (list.getIsEmpty()) {
    RQ_UNHANDLED_ERROR("no symbol of name");
    this->setNotOk();
    return rq::Rvalue();
  }
  if (list.getHasTail()) {
    RQ_UNHANDLED_ERROR("name collision");
    this->setNotOk();
    return rq::Rvalue();
  }
  rq::Symbol &found_sy = list.getHead();
  rq::Symbol *rvalue_sy_ptr = nullptr;
  if (llvm::isa<rq::Polymorph>(found_sy)) {
    if (found_sy.getIsProcedureRelated()) {
      RQ_UNHANDLED_ERROR("procedure polymorph rvalue");
      this->setNotOk();
      return rq::Rvalue();
    }
    rq::Polymorph &poly = llvm::cast<rq::Polymorph>(found_sy);
    if (!poly.getHasSomeInstance()) {
      RQ_UNHANDLED_ERROR("polymorph has no non-template instances");
      this->setNotOk();
      return rq::Rvalue();
    }
    if (poly.getHasMultipleInstance()) {
      RQ_UNHANDLED_ERROR("polymorph has multiple non-template instances");
      this->setNotOk();
      return rq::Rvalue();
    }
    rvalue_sy_ptr = &poly.getOnlyInstance();
  } else {
    rvalue_sy_ptr = &found_sy;
  }
  rq::Symbol &rvalue_sy = rq::dereferencePtr(rvalue_sy_ptr);
  if (llvm::isa<rq::Label>(rvalue_sy)) {
    rq::Symbol &type = this->getContext().acquireSymbolType();
    rq::Label &label = llvm::cast<rq::Label>(rvalue_sy);
    return rq::Rvalue(type, label);
  }
  if (llvm::isa<rq::Anchor>(rvalue_sy)) {
    rq::Symbol &type = this->getContext().acquireSymbolType();
    rq::Anchor &anchor = llvm::cast<rq::Anchor>(rvalue_sy);
    return rq::Rvalue(type, anchor);
  }
  if (llvm::isa<rq::LocalDynamicVariable>(rvalue_sy)) {
    if (is_static) {
      RQ_UNHANDLED_ERROR("no_static value");
      this->setNotOk();
      return rq::Rvalue();
    }
    rq::LocalDynamicVariable &var =
        llvm::cast<rq::LocalDynamicVariable>(rvalue_sy);
    rq::Symbol &type = var.getType().getSymbol();
    return rq::Rvalue(type, var);
  }
  if (llvm::isa<rq::LocalStaticVariable>(rvalue_sy)) {
    rq::LocalStaticVariable &var =
        llvm::cast<rq::LocalStaticVariable>(rvalue_sy);
    rq::Symbol &type = var.getType().getSymbol();
    rq::Gendex<rq::StaticValue> gendex = var.getValue();
    if (!gendex.getHasData()) {
      RQ_UNHANDLED_ERROR("invalid memory");
      this->setNotOk();
      return rq::Rvalue();
    }
    rq::StaticValue value = gendex.getData();
    return rq::Rvalue(type, std::move(value));
  }
  if (llvm::isa<rq::ProcedureArgument>(rvalue_sy)) {
    if (is_static) {
      RQ_UNHANDLED_ERROR("no_static value");
      this->setNotOk();
      return rq::Rvalue();
    }
    rq::ProcedureArgument &arg = llvm::cast<rq::ProcedureArgument>(rvalue_sy);
    rq::Symbol &type = arg.getType().getSymbol();
    return rq::Rvalue(type, arg);
  }
  if (llvm::isa<rq::TemplateArgument>(rvalue_sy)) {
    rq::TemplateArgument &arg = llvm::cast<rq::TemplateArgument>(rvalue_sy);
    rq::Symbol &type = arg.getType().getSymbol();
    rq::Entity &value = arg.getValue();
    return rq::Rvalue(type, value);
  }
  if (llvm::isa<rq::Enumerator>(rvalue_sy)) {
    RQ_UNHANDLED_ERROR("enumerator rvalue");
    this->setNotOk();
    return rq::Rvalue();
  }
  if (llvm::isa<rq::Namespace>(rvalue_sy)) {
    rq::Symbol &type = this->getContext().acquireSymbolType();
    rq::Namespace &namespace_ = llvm::cast<rq::Namespace>(rvalue_sy);
    return rq::Rvalue(type, namespace_);
  }
  if (llvm::isa<rq::ClassType>(rvalue_sy)) {
    rq::ClassType &class_ = llvm::cast<rq::ClassType>(rvalue_sy);
    if (!class_.getIsEvaluated()) {
      this->evaluate(class_);
    }
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::Rvalue(type, class_);
  }
  if (llvm::isa<rq::EnumerationType>(rvalue_sy)) {
    rq::EnumerationType &enum_ = llvm::cast<rq::EnumerationType>(rvalue_sy);
    if (!enum_.getIsEvaluated()) {
      this->evaluate(enum_);
    }
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::Rvalue(type, enum_);
  }
  if (llvm::isa<rq::Interface>(rvalue_sy)) {
    rq::Interface &interface = llvm::cast<rq::Interface>(rvalue_sy);
    if (!interface.getIsEvaluated()) {
      this->evaluate(interface);
    }
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::Rvalue(type, interface);
  }
  if (llvm::isa<rq::Adapter>(rvalue_sy)) {
    rq::Adapter &adapter = llvm::cast<rq::Adapter>(rvalue_sy);
    if (!adapter.getIsEvaluated()) {
      this->evaluate(adapter);
    }
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::Rvalue(type, adapter);
  }
  if (llvm::isa<rq::GlobalDynamicVariable>(rvalue_sy)) {
    RQ_UNHANDLED_ERROR("no_static value");
    this->setNotOk();
    return rq::Rvalue();
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
    return rq::Rvalue(type_sy, value);
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::Rvalue
Evaluator::evaluateArithmeticRvalue(bool is_static, rq::Opcode opcode,
                                    rq::SymbolTable &table, rq::Module &module,
                                    rq::Expression &rvalue_ex) {
  std::ignore = module;
  using O = rq::Opcode;
  llvm::SmallVector<rq::Rvalue> rvalues;
  rq::Symbol *type_sy_ptr = &this->getContext().acquireIntegerLiteralType();
  for (rq::Expression &branch_ex : rvalue_ex.getBranchSubrange()) {
    rq::Rvalue &branch_rv = rvalues.emplace_back(
        this->evaluateRvalue(is_static, table, module, branch_ex));
    if (!branch_rv.getIsOk()) {
      continue;
    }
    rq::Symbol &branch_sy = branch_rv.getType();
    rq::Symbol &type_sy = rq::dereferencePtr(type_sy_ptr);
    if (llvm::isa<rq::IntegerLiteral>(branch_sy)) {
      continue;
    }
    if (llvm::isa<rq::FloatLiteral>(branch_sy)) {
      if (type_sy.getIsIntegerType()) {
        RQ_UNHANDLED_ERROR("float literal with integer");
        this->setNotOk();
        continue;
      }
      if (llvm::isa<rq::IntegerLiteral>(type_sy)) {
        type_sy_ptr = &this->getContext().acquireFloatLiteralType();
        continue;
      }
    }
    if (branch_sy.getIsIntegerType()) {
      if (!type_sy.getIsLiteralType()) {
        if (type_sy != branch_sy) {
          RQ_UNHANDLED_ERROR("types not implicitly convertable");
          this->setNotOk();
        }
        continue;
      }
      if (llvm::isa<rq::FloatLiteral>(type_sy)) {
        RQ_UNHANDLED_ERROR("float literal with integer");
        this->setNotOk();
        continue;
      }
      type_sy_ptr = &branch_sy;
      continue;
    }
    if (branch_sy.getIsFloatType()) {
      if (!type_sy.getIsLiteralType()) {
        if (type_sy != branch_sy) {
          RQ_UNHANDLED_ERROR("types not implicitly convertable");
          this->setNotOk();
        }
        continue;
      }
      type_sy_ptr = &branch_sy;
      continue;
    }
    if (!branch_sy.getIsNumericType()) {
      RQ_UNHANDLED_ERROR("not numeric");
      this->setNotOk();
    }
  }
  if (!this->getIsOk()) {
    return rq::Rvalue();
  }
  rq::Symbol &final_ty_sy = rq::dereferencePtr(type_sy_ptr);
  rq::BinaryInstructionFactory factory{this->getContext(), opcode};
  if (final_ty_sy.getIsLiteralType()) {
    for (rq::Rvalue &rvalue : rvalues) {
      rq::Entity &entity = rvalue.getEntity();
      factory.append(entity);
    }
  } else if (final_ty_sy.getIsFloatType()) {
    const llvm::fltSemantics &semantics =
        this->getContext().getLlvmFltSemantics(final_ty_sy.getKind());
    rq::FloatFolder folder(opcode, semantics);
    for (rq::Rvalue &rvalue : rvalues) {
      rq::Rvalue folded = this->fold(rvalue, final_ty_sy);
      if (!folded.getIsOk()) {
        continue;
      }
      if (folded.getHasTemp()) {
        llvm::APFloat &temp = folded.getTemp().getFloat();
        folder.fold(temp);
      } else {
        rq::Entity &entity = rvalue.getEntity();
        if (llvm::isa<rq::Expression>(entity)) {
          rq::Expression &expression = llvm::cast<rq::Expression>(entity);
          RQ_ASSERT(expression.getIsNumericLiteral(), "not numeric literal");
          auto result =
              rq::parseFloatLiteral(expression.getSourceText(), semantics);
          if (result.code != rq::NumericResultCode::OK) {
            RQ_UNHANDLED_ERROR("error parsing float");
          }
          folder.fold(result.float_);
          continue;
        }
        if (folder.getIsFolding()) {
          llvm::APFloat float_ = folder.extract();
          rq::WordConstant &constant =
              this->getContext().acquireWordConstant(float_);
          factory.append(constant);
        }
        factory.append(entity);
      }
    }
  }
  rq::BinaryInstruction &instruction =
      rq::dereferencePtr(factory.getOuterPointer());
  return rq::Rvalue(final_ty_sy, instruction);
}

} // namespace rq
