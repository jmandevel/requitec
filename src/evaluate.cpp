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
DottedInstructionFactory::DottedInstructionFactory(rq::Context &context,
                                                   rq::Opcode opcode)
    : _constext_ptr(&context), _opcode(opcode) {}

void DottedInstructionFactory::append(rq::Entity &entity) {
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
          table, name, table, unascribed_ex, nullptr, factory.getFlags(),
          module, body_ex_ptr, nullptr, nullptr, nullptr);
      table.addMember(this->getContext(), name, func);
      break;
    }
    case K::FUNCTION: {
      rq::Expression &name_ex = unascribed_ex.getBranch();
      rq::Expression *body_ex_ptr = name_ex.getNextPtr();
      rq::Name name = this->evaluateName(table, module, name_ex);
      rq::Function &func = this->getContext().allocateValue<rq::Function>(
          table, name, table, unascribed_ex, nullptr, factory.getFlags(),
          module, body_ex_ptr, nullptr, nullptr,
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
  rq::DottedInstructionFactory factory(this->getContext(), O::STATEMENT);
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
      rq::Symbol *type_ptr =
          this->completeType(lvalue.getType().getSymbol(), rvalue.getType());
      if (type_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("type not determined");
      }
      rq::Symbol &type = rq::dereferencePtr(type_ptr);
      if (type != lvalue.getType().getSymbol()) {
        rq::LocalDynamicVariable &var =
            llvm::cast<rq::LocalDynamicVariable>(lvalue.getSymbol());
        rq::ConstantSymbol &type_ct = this->getContext().acquireConstantSymbol(
            lvalue.getType().getFlags(), type);
        var.completeType(type_ct);
      }
      rq::Entity &folded_rv = this->foldDynamicRvalue(rvalue.getValue(), type);
      rq::Instruction &inst =
          this->getContext().acquireInstruction(rq::Opcode::ASSIGN);
      inst.setAddress0(lvalue.getSymbol());
      inst.setAddress1(folded_rv);
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
    rq::Expression &mangle_ex =
        rq::dereferencePtr(func.getMangleExpressionPtr());
    rq::Name mangle =
        this->evaluateName(func.getHostingTable(), func.getModule(), mangle_ex);
    if (mangle.getText().empty()) {
      RQ_UNHANDLED_ERROR("invalid name");
    }
    func.setMangledName(mangle.getText());
  }
  rq::Expression &statement0 =
      rq::dereferencePtr(func.getFirstBodyExpressionPtr());
  rq::Expression *body_ptr = &statement0;
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
    rq::Symbol &sig_sy =
        rq::dereferencePtr(sig_rv.getValue().getSymbol().symbol_ptr);
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
  rq::Expression &body = rq::dereferencePtr(body_ptr);
  rq::ConstantSymbol &result_type = this->getContext().acquireConstantSymbol(
      {}, this->getContext().acquireSignedIntegerType());
  rq::Instruction *inst_ptr =
      this->evaluateLocalScope(func, result_type, func, func.getModule(), body);
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
    static_sy.symbol_ptr = this->completeType(
        rq::dereferencePtr(static_sy.symbol_ptr), result_type.getSymbol());
    rq::ConstantSymbol &type = this->getContext().acquireConstantSymbol(
        static_sy.flags, rq::dereferencePtr(static_sy.symbol_ptr));
    switch (var_ex.getKeyword()) {
    case K::IDENTIFIER_LITERAL:
      [[fallthrough]];
    case K::RESULT: {
      rq::Name name = this->evaluateName(table, module, var_ex);
      auto found = table.lookupList(name);
      if (!found.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("var of name already initialized");
      }
      if (var_ex.getKeyword() == K::RESULT &&
          type.getSymbol() != result_type.getSymbol()) {
        RQ_UNHANDLED_ERROR("invalid result type");
      }
      rq::LocalDynamicVariable &var =
          this->getContext().allocateValue<rq::LocalDynamicVariable>(
              name, table, table, module, rq::ExpressionFlags::NONE, type);
      table.addMember(this->getContext(), name, var);
      return rq::DynamicLvalue(var, type);
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
  case K::INFERENCE: {
    rq::Symbol &symbol = this->getContext().acquireInferenceType();
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
  using O = rq::Opcode;
  switch (rvalue_ex.getKeyword()) {
  case K::INTEGER_LITERAL: {
    rq::Entity &value = rvalue_ex;
    rq::Symbol &type = this->getContext().acquireIntegerLiteralType();
    return rq::DynamicRvalue(value, type);
  }
  case K::IDENTIFIER_LITERAL: {
    rq::Name name{rvalue_ex.getSourceText()};
    rq::DynamicRvalue rvalue =
        this->evaluateDynamicIdentifierRvalue(table, module, name);
    return rvalue;
  }
  case K::ADD: {
    return this->evaluateDynamicArithmeticRvalue(table, module, rvalue_ex,
                                                 O::ADD);
  }
  case K::SUBTRACT: {
    return this->evaluateDynamicArithmeticRvalue(table, module, rvalue_ex,
                                                 O::SUBTRACT);
  }
  case K::MULTIPLY: {
    return this->evaluateDynamicArithmeticRvalue(table, module, rvalue_ex,
                                                 O::MULTIPLY);
  }
  case K::MODULUS: {
    return this->evaluateDynamicArithmeticRvalue(table, module, rvalue_ex,
                                                 O::MODULUS);
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::DynamicRvalue Evaluator::evaluateDynamicArithmeticRvalue(
    rq::SymbolTable &table, rq::Module &module, rq::Expression &rvalue_ex,
    rq::Opcode opcode) {
  using S = rq::SymbolKind;
  rq::Symbol *type_ptr = nullptr;
  rq::DottedInstructionFactory factory(this->getContext(), opcode);
  for (rq::Expression &branch_ex : rvalue_ex.getBranchSubrange()) {
    rq::DynamicRvalue branch_rv =
        this->evaluateDynamicRvalue(table, module, branch_ex);
    factory.append(branch_rv.getValue());
    if (branch_rv.getIsEmpty()) {
      RQ_UNHANDLED_ERROR("empty branch rv");
    }
    if (branch_rv.getType().getIsLiteralType()) {
      if (branch_rv.getType().getKind() == S::INTEGER_LITERAL_TYPE) {
        if (type_ptr == nullptr) {
          type_ptr = &this->getContext().acquireIntegerLiteralType();
        }
      } else if (branch_rv.getType().getKind() == S::FLOAT_LITERAL_TYPE) {
        type_ptr = &this->getContext().acquireFloatLiteralType();
      } else {
        RQ_UNHANDLED_ERROR("not arithmitic literal type");
      }
      continue;
    }
    if (type_ptr != nullptr) {
      rq::Symbol &prev_type = rq::dereferencePtr(type_ptr);
      if (prev_type.getKind() == S::FLOAT_LITERAL_TYPE &&
          branch_rv.getType().getIsIntegerType()) {
        RQ_UNHANDLED_ERROR("float literal implicit cast to integer");
      }
      if (!prev_type.getIsLiteralType() &&
          !branch_rv.getType().getIsLiteralType() &&
          branch_rv.getType() != prev_type) {
        RQ_UNHANDLED_ERROR("no implicit conversion");
      }
    }
    type_ptr = &branch_rv.getType();
  }
  rq::Entity &rvalue = factory.getOuter();
  rq::Symbol &type = rq::dereferencePtr(type_ptr);
  return rq::DynamicRvalue(rvalue, type);
}

[[nodiscard]] rq::Entity &Evaluator::foldDynamicRvalue(rq::Entity &rvalue,
                                                       rq::Symbol &type) {
  using K = rq::Keyword;
  using O = rq::Opcode;
  // TODO actually fold instead of spamming constants
  if (llvm::isa<rq::Expression>(rvalue)) {
    rq::Expression &rvalue_ex = llvm::cast<rq::Expression>(rvalue);
    switch (rvalue_ex.getKeyword()) {
    case K::INTEGER_LITERAL: {
      const unsigned depth = this->getContext().getDepth(type);
      llvm::APInt term(depth, 0);
      rq::NumericResultCode code =
          rq::getNumericValue(rvalue_ex.getSourceText(), term);
      if (code != rq::NumericResultCode::OK) {
        RQ_UNHANDLED_ERROR("error parsing integer literal");
      }
      rq::ConstantWord &word = this->getContext().acquireConstantWord(term);
      return word;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  if (llvm::isa<rq::Instruction>(rvalue)) {
    rq::Instruction &inst = llvm::cast<rq::Instruction>(rvalue);
    switch (inst.getOpcode()) {
    case O::ADD:
      [[fallthrough]];
    case O::SUBTRACT:
      [[fallthrough]];
    case O::MULTIPLY:
      [[fallthrough]];
    case O::DIVIDE:
      [[fallthrough]];
    case O::MODULUS: {
      rq::Entity &old_address0 = inst.popAddress0();
      rq::Entity &new_address0 = this->foldDynamicRvalue(old_address0, type);
      inst.setAddress0(new_address0);
      rq::Entity &old_address1 = inst.popAddress1();
      rq::Entity &new_address1 = this->foldDynamicRvalue(old_address1, type);
      inst.setAddress1(new_address1);
      return inst;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  return rvalue;
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::evaluateDynamicIdentifierRvalue(rq::SymbolTable &table,
                                           rq::Module &module, rq::Name name) {
  auto list = table.lookupList(name);
  if (list.getIsEmpty()) {
    RQ_UNHANDLED_ERROR("no symbol of name");
  }
  if (list.getHasTail()) {
    RQ_UNHANDLED_ERROR("name collision");
  }
  rq::Symbol &symbol = list.getHead();
  if (llvm::isa<rq::LocalDynamicVariable>(symbol)) {
    rq::LocalDynamicVariable &var =
        llvm::cast<rq::LocalDynamicVariable>(symbol);
    rq::Symbol &type = var.getType().getSymbol();
    rq::DynamicRvalue rvalue(var, type);
    return rvalue;
  }
  std::ignore = module;
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::Name Evaluator::evaluateName(rq::SymbolTable &table,
                                               rq::Module &module,
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
  case K::RESULT: {
    rq::Name name{name_ex.getKeyword()};
    return name;
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::Symbol *Evaluator::completeType(rq::Symbol &to_type,
                                                  rq::Symbol &from_type) {
  RQ_ASSERT(from_type.getIsCompleteType(), "from not complete");
  using S = rq::SymbolKind;
  if (to_type == from_type) {
    return &from_type;
  }
  if (to_type.getKind() == S::INFERENCE_TYPE) {
    return &this->deliteralizeType(from_type);
  }
  if (llvm::isa<rq::InferenceCountArraySubtype>(to_type)) {
    if (!llvm::isa<rq::ArraySubtype>(from_type)) {
      RQ_UNHANDLED_ERROR("expected array type");
    }
    rq::InferenceCountArraySubtype &to_inf_array =
        llvm::cast<rq::InferenceCountArraySubtype>(to_type);
    rq::ArraySubtype &from_array = llvm::cast<rq::ArraySubtype>(from_type);
    rq::Symbol *child_ptr = this->completeType(
        to_inf_array.getChild().getSymbol(), from_array.getChild().getSymbol());
    if (child_ptr == nullptr) {
      return nullptr;
    }
    rq::Symbol &child = rq::dereferencePtr(child_ptr);
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        to_inf_array.getChild().getFlags(), child);
    rq::ArraySubtype &array =
        this->getContext().acquireArraySubtype(child_ct, from_array.getCount());
    return &array;
  }
  if (llvm::isa<rq::UncountedSubtype>(to_type)) {
    if (to_type.getKind() != from_type.getKind()) {
      RQ_UNHANDLED_ERROR("subtype mismatch");
    }
    rq::UncountedSubtype &to_subtype =
        llvm::cast<rq::UncountedSubtype>(to_type);
    rq::UncountedSubtype &from_subtype =
        llvm::cast<rq::UncountedSubtype>(from_type);
    rq::Symbol *child_ptr = this->completeType(
        to_subtype.getChild().getSymbol(), from_subtype.getChild().getSymbol());
    if (child_ptr == nullptr) {
      return nullptr;
    }
    rq::Symbol &child = rq::dereferencePtr(child_ptr);
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        to_subtype.getChild().getFlags(), child);
    rq::UncountedSubtype &subtype = this->getContext().acquireUncountedSubtype(
        to_subtype.getKind(), child_ct);
    return &subtype;
  }
  if (llvm::isa<rq::ArraySubtype>(to_type)) {
    if (!llvm::isa<rq::ArraySubtype>(from_type)) {
      RQ_UNHANDLED_ERROR("not array");
    }
    rq::ArraySubtype &to_array = llvm::cast<rq::ArraySubtype>(to_type);
    rq::ArraySubtype &from_array = llvm::cast<rq::ArraySubtype>(from_type);
    if (to_array.getCount() != from_array.getCount()) {
      RQ_UNHANDLED_ERROR("count mismatch");
    }
    rq::Symbol *child_ptr = this->completeType(
        to_array.getChild().getSymbol(), from_array.getChild().getSymbol());
    if (child_ptr == nullptr) {
      return nullptr;
    }
    rq::Symbol &child = rq::dereferencePtr(child_ptr);
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        to_array.getChild().getFlags(), child);
    rq::ArraySubtype &subtype =
        this->getContext().acquireArraySubtype(child_ct, to_array.getCount());
    return &subtype;
  }
  return &this->deliteralizeType(from_type);
}

[[nodiscard]] rq::Symbol &Evaluator::deliteralizeType(rq::Symbol &type) {
  using S = rq::SymbolKind;
  using TF = rq::TypeFlags;
  if (type.getKind() == S::INTEGER_LITERAL_TYPE) {
    return this->getContext().acquireSignedIntegerType();
  }
  if (type.getKind() == S::FLOAT_LITERAL_TYPE) {
    return this->getContext().acquireSingleType();
  }
  if (type.getKind() == S::STRING_LITERAL_TYPE) {
    rq::Symbol &child = this->getContext().acquireCharType();
    rq::ConstantSymbol &child_ct =
        this->getContext().acquireConstantSymbol(TF::NONE, child);
    rq::SliceSubtype &slice = this->getContext().acquireSliceSubtype(child_ct);
    return slice;
  }
  if (type.getKind() == S::CODEUNIT_LITERAL_TYPE) {
    rq::Symbol &type = this->getContext().acquireCharType();
    return type;
  }
  if (llvm::isa<rq::UncountedSubtype>(type)) {
    rq::UncountedSubtype &uc_subtype = llvm::cast<rq::UncountedSubtype>(type);
    rq::Symbol &child =
        this->deliteralizeType(uc_subtype.getChild().getSymbol());
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        uc_subtype.getChild().getFlags(), child);
    rq::UncountedSubtype &new_subtype =
        this->getContext().acquireUncountedSubtype(uc_subtype.getKind(),
                                                   child_ct);
    return new_subtype;
  }
  if (llvm::isa<rq::ArraySubtype>(type)) {
    rq::ArraySubtype &array = llvm::cast<rq::ArraySubtype>(type);
    rq::Symbol &child = this->deliteralizeType(array.getChild().getSymbol());
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        array.getChild().getFlags(), child);
    rq::ArraySubtype &new_array =
        this->getContext().acquireArraySubtype(child_ct, array.getCount());
    return new_array;
  }
  return type;
}

} // namespace rq
