#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/expressions.hpp>
#include <rq/literals.hpp>
#include <rq/symbols.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

RQ_ALWAYS_INLINE
InstructionConsFactory::InstructionConsFactory(rq::Context &context,
                                               rq::Opcode opcode)
    : _constext_ptr(&context), _opcode(opcode) {}

void InstructionConsFactory::append(rq::Entity &entity) {
  if (this->_outer_ptr == nullptr) {
    this->_outer_ptr = &entity;
    return;
  }
  if (this->_last_ptr == nullptr) {
    rq::Instruction &cons =
        this->getContext().acquireInstruction(this->getOpcode());
    cons.setAddress0(this->_outer_ptr);
    cons.setAddress1(entity);
    this->_outer_ptr = &cons;
    this->_last_ptr = &cons;
    return;
  }
  rq::Instruction &cons =
      this->getContext().acquireInstruction(this->getOpcode());
  rq::Instruction &last = rq::dereferencePtr(this->_last_ptr);
  cons.setAddress0(last.replaceAddress1(cons));
  cons.setAddress1(entity);
  this->_last_ptr = &cons;
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
  using K = rq::Keyword;
  rq::ExpressionFlagsFactory factory{};
  for (rq::Expression &branch_ex : first_ex.getInclusiveNextSubrange()) {
    rq::Expression *unascribed_ex_ptr = nullptr;
    if (branch_ex.getKeyword() == K::ASCRIBE_EXPRESSION) {
      rq::Expression &unascribed_ex = branch_ex.getBranch();
      unascribed_ex_ptr = &unascribed_ex;
      for (rq::Expression &attrib_ex : unascribed_ex.getNextSubrange()) {
        rq::Expression &attrib_rv_ex = attrib_ex.getBranch();
        rq::Expression *attrib_br_ex_ptr = attrib_rv_ex.getNextPtr();
        rq::StaticRvalue attrib_rv =
            this->evaluateStaticRvalue(table, module, attrib_rv_ex);
        if (!attrib_rv.getType().getIsExpressionAttributeType()) {
          RQ_UNHANDLED_ERROR("not expression attribute");
        }
        rq::ExpressionAttribute attrib =
            attrib_rv.getValue().getExpressionAttribute();
        if (!factory.addFlag(attrib, attrib_br_ex_ptr)) {
          RQ_UNHANDLED_ERROR("duplicate attribute of kind");
        }
      }
    } else {
      unascribed_ex_ptr = &branch_ex;
    }
    rq::Expression &unascribed_ex = rq::dereferencePtr(unascribed_ex_ptr);
    switch (unascribed_ex.getKeyword()) {
    case K::MAIN: {
      rq::Name name(K::MAIN);
      rq::Expression *body_ex_ptr = unascribed_ex.getBranchPtr();
      rq::Function &func = this->getContext().allocateValue<rq::Function>(
          table, name, table, unascribed_ex, nullptr, factory.getFlags(), module,
          body_ex_ptr, nullptr, nullptr, nullptr);
      table.addMember(this->getContext(), name, func);
      break;
    }
    case K::FUNCTION: {
      rq::Expression &name_ex = unascribed_ex.getBranch();
      rq::Expression *body_ex_ptr = name_ex.getNextPtr();
      rq::Name name = this->evaluateName(table, module, name_ex);
      rq::Function &func = this->getContext().allocateValue<rq::Function>(
          table, name, table, unascribed_ex, nullptr, factory.getFlags(), module,
          body_ex_ptr, nullptr, nullptr,
          factory.getExpressionPtr(
              rq::ExpressionAttributeKind::MANGLE_ATTRIBUTE));
      table.addMember(this->getContext(), name, func);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
}

[[nodiscard]] rq::Instruction *Evaluator::evaluateLocalScope(
    rq::Function &function, rq::ConstantSymbol &result_type,
    rq::SymbolTable &table, rq::Module &module, rq::Expression &first_ex) {
  std::ignore = function;
  std::ignore = table;
  std::ignore = module;
  std::ignore = result_type;
  using K = rq::Keyword;
  using O = rq::Opcode;
  // using S = rq::SymbolKind;
  rq::InstructionConsFactory factory(this->getContext(), O::STATEMENT);
  for (rq::Expression &state_ex : first_ex.getInclusiveNextSubrange()) {
    switch (state_ex.getKeyword()) {
    case K::ASSIGN: {
      rq::Expression &lvalue_ex = state_ex.getBranch();
      rq::Expression &rvalue_ex = lvalue_ex.getNext();
      rq::DynamicRvalue rvalue =
          this->evaluateDynamicRvalue(table, module, rvalue_ex);
      if (rvalue.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("error");
      }
      rq::DynamicLvalue lvalue =
          this->evaluateDynamicLvalue(result_type, table, module, lvalue_ex);
      if (lvalue.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("error");
      }
      // TODO type deduction
      if (rvalue.getType().getIsLiteralType()) {
        rvalue = this->foldDynamicRvalue(rvalue, lvalue.getType().getSymbol());
      }
      rq::Instruction &inst =
          this->getContext().acquireInstruction(rq::Opcode::ASSIGN);
      inst.setAddress0(lvalue.getSymbol());
      inst.setAddress1(rvalue.getValue());
      factory.append(inst);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  rq::Entity &outer = factory.getOuter();
  rq::Instruction &outer_ins = llvm::cast<rq::Instruction>(outer);
  return &outer_ins;
}

void Evaluator::evaluateAllModuleSymbols(rq::Module &module) {
  rq::Top &top = this->getContext().getTop();
  for (auto &kvp : top.getSymbolListSubrange()) {
    rq::BumpPtrList<rq::Symbol> &list = kvp.getSecond();
    for (rq::Symbol &symbol : list) {
      if (llvm::isa<rq::FunctionPolymorph>(symbol)) {
        rq::FunctionPolymorph &poly = llvm::cast<rq::FunctionPolymorph>(symbol);
        for (rq::Function &function : poly.getDerivedInstanceSubrange()) {
          if (function.getModule() != module) {
            continue;
          }
          if (!function.getIsEvaluated()) {
            this->evaluate(function);
          }
        }
      }
    }
  }
}

void Evaluator::evaluate(rq::Module &module) {
  rq::Expression &top_ex = module.getExpression();
  if (!top_ex.getHasBranch()) {
    return;
  }
  rq::Expression &body_ex = top_ex.getBranch();
  this->evaluateGlobalScope(this->getContext().getTop(), module, body_ex);
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

void Evaluator::evaluate(rq::Function &func) {
  if (func.getFirstBodyExpressionPtr() == nullptr) {
    return;
  }
  if (func.getMangleExpressionPtr() != nullptr) {
    rq::Expression& mangle_ex = rq::dereferencePtr(func.getMangleExpressionPtr());
    rq::Name mangle = this->evaluateName(func.getHostingTable(), func.getModule(), mangle_ex);
    if (mangle.getText().empty()) {
      RQ_UNHANDLED_ERROR("invalid name");
    }
    func.setMangledName(mangle.getText());
  }
  rq::Expression &statement0 =
      rq::dereferencePtr(func.getFirstBodyExpressionPtr());
  rq::Expression* body_ptr = &statement0;
  if (func.getName().getKeyword() != rq::Keyword::MAIN) {
    if (statement0.getIsStatement()) {
      // TODO static statements before signature
      RQ_TODO_IMPLEMENTATION();
    }
    rq::Expression &sig_ex = statement0;
    rq::StaticRvalue sig_rv = this->evaluateStaticRvalue(
        func.getHostingTable(), func.getModule(), sig_ex);
    if (sig_rv.getIsEmpty()) {
      RQ_UNHANDLED_ERROR("invalid rvalue");
    }
    rq::Symbol& sig_sy = rq::dereferencePtr(sig_rv.getValue().getSymbol().symbol_ptr);
    if (!llvm::isa<rq::Signature>(sig_sy)) {
      RQ_UNHANDLED_ERROR("expected sig");
    }
    rq::Signature &sig = llvm::cast<rq::Signature>(sig_sy);
    func.setSignature(sig);
    body_ptr = sig_ex.getNextPtr();
  }
  if (body_ptr == nullptr) {
    return;
  }
  rq::Expression& body = rq::dereferencePtr(body_ptr);
  rq::ConstantSymbol &result_type = this->getContext().acquireConstantSymbol(
      {}, this->getContext().acquireSignedIntegerType());
  rq::Instruction *inst_ptr = this->evaluateLocalScope(
      func, result_type, func, func.getModule(), body);
  if (inst_ptr == nullptr) {
    return;
  }
  rq::Instruction &inst = rq::dereferencePtr(inst_ptr);
  func.setInstructions(inst);
}

[[nodiscard]] rq::DynamicLvalue
Evaluator::evaluateDynamicLvalue(rq::ConstantSymbol &result_type,
                                 rq::SymbolTable &table, rq::Module &module,
                                 rq::Expression &lvalue_ex) {
  using K = rq::Keyword;
  switch (lvalue_ex.getKeyword()) {
  case K::BINDING: {
    rq::Expression &var_ex = lvalue_ex.getBranch();
    rq::Expression &type_ex = var_ex.getNext();
    rq::StaticRvalue type_rvalue =
        this->evaluateStaticRvalue(table, module, type_ex);
    rq::Symbol &type_type = type_rvalue.getType();
    if (type_type != this->getContext().acquireSymbolType()) {
      RQ_UNHANDLED_ERROR("not type");
    }
    rq::StaticSymbol static_sy = type_rvalue.getValue().getSymbol();
    rq::ConstantSymbol &type = this->getContext().acquireConstantSymbol(
        static_sy.flags, rq::dereferencePtr(static_sy.symbol_ptr));
    switch (var_ex.getKeyword()) {
    case K::RESULT: {
      rq::Name name(K::RESULT);
      auto found = table.lookupList(name);
      if (!found.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("result already initialized");
      }
      if (type.getSymbol() != result_type.getSymbol()) {
        RQ_UNHANDLED_ERROR("invalid result type");
      }
      rq::LocalDynamicVariable &result =
          this->getContext().allocateValue<rq::LocalDynamicVariable>(
              name, table, table, module, rq::ExpressionFlags::NONE, type);
      table.addMember(this->getContext(), name, result);
      return rq::DynamicLvalue(result, type);
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::StaticRvalue
Evaluator::evaluateStaticRvalue(rq::SymbolTable &table, rq::Module &module,
                                rq::Expression &rvalue_ex) {
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  using S = rq::SymbolKind;
  switch (rvalue_ex.getKeyword()) {
  case K::SIGNED_INTEGER: {
    if (rvalue_ex.getHasBranch()) {
      RQ_TODO_IMPLEMENTATION();
    }
    rq::Symbol &symbol = this->getContext().acquireSignedIntegerType();
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::StaticRvalue(rq::StaticSymbol{{}, &symbol}, type);
  }
  case K::MANGLE: {
    rq::ExpressionAttribute value = rq::ExpressionAttribute::MANGLE;
    rq::Symbol &type = this->getContext().acquireMangleAttributeType();
    return rq::StaticRvalue(value, type);
  }
  case K::INSTANTIATE_SIGNATURE: {
    rq::Expression &return_ex = rvalue_ex.getBranch();
    if (return_ex.getHasNext()) {
      // TODO parameters
      RQ_TODO_IMPLEMENTATION();
    }
    rq::StaticRvalue return_rv =
        this->evaluateStaticRvalue(table, module, return_ex);
    if (return_rv.getIsEmpty()) {
      RQ_UNHANDLED_ERROR("invalid rvalue");
    }
    if (return_rv.getType().getKind() != S::SYMBOL_TYPE) {
      RQ_UNHANDLED_ERROR("expected symbol");
    }
    rq::Symbol &return_sy =
        rq::dereferencePtr(return_rv.getValue().getSymbol().symbol_ptr);
    if (!return_sy.getIsType()) {
      RQ_UNHANDLED_ERROR("not type");
    }
    rq::ConstantSymbol &return_ct = this->getContext().acquireConstantSymbol(
        return_rv.getValue().getSymbol().flags, return_sy);
    rq::Signature &sig = this->getContext().allocateValue<rq::Signature>(
        nullptr, 0, 0, 0, rvalue_ex, 0, module, return_ct, nullptr, nullptr,
        nullptr);
    return rq::StaticRvalue(rq::StaticSymbol({}, &sig),
                            this->getContext().acquireSymbolType());
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::evaluateDynamicRvalue(rq::SymbolTable &table, rq::Module &module,
                                 rq::Expression &rvalue_ex) {
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  switch (rvalue_ex.getKeyword()) {
  case K::INTEGER_LITERAL: {
    rq::Entity &value = rvalue_ex;
    rq::Symbol &type = this->getContext().acquireIntegerLiteralType();
    return rq::DynamicRvalue(value, type);
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::foldDynamicRvalue(rq::DynamicRvalue rvalue, rq::Symbol &actual_ty) {
  if (actual_ty.getIsIntegerType()) {
    const unsigned depth = this->getContext().getDepth(actual_ty);
    const bool is_signed = actual_ty.getIsSignedType();
    rq::Entity &value = rvalue.getValue();
    llvm::APSInt folder(depth, is_signed);
    this->foldDynamicInteger(folder, value);
    rq::ConstantWord &word = this->getContext().acquireConstantWord(
        static_cast<llvm::APInt>(folder));
    return rq::DynamicRvalue(word, actual_ty);
  }
  RQ_UNREACHABLE();
}

void Evaluator::foldDynamicInteger(llvm::APSInt &inout_int, rq::Entity &value) {
  using K = rq::Keyword;
  if (llvm::isa<rq::Expression>(value)) {
    rq::Expression &ex = llvm::cast<rq::Expression>(value);
    switch (ex.getKeyword()) {
    case K::INTEGER_LITERAL: {
      llvm::APInt term(inout_int.getBitWidth(), 0);
      rq::NumericResultCode code =
          rq::getNumericValue(ex.getSourceText(), term);
      if (code != rq::NumericResultCode::OK) {
        RQ_UNHANDLED_ERROR("error parsing integer literal");
      }
      inout_int = term;
      return;
    }
    default:
      break;
    }
    RQ_UNREACHABLE();
  }
  RQ_UNREACHABLE();
}

rq::Name Evaluator::evaluateName(rq::SymbolTable &table, rq::Module &module,
                                 rq::Expression &name_ex) {
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  switch (name_ex.getKeyword()) {
  case K::IDENTIFIER_LITERAL: {
    llvm::StringRef text =
        this->getContext().saveString(name_ex.getSourceText());
    rq::Name name(text);
    return name;
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

} // namespace rq
