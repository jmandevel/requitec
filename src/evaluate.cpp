#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/expressions.hpp>
#include <rq/literals.hpp>
#include <rq/symbols.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/STLExtras.h>
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

[[nodiscard]] rq::Expression &
Evaluator::evaluateLowFuseFlags(rq::Module &module, rq::SymbolTable &table,
                                rq::LowFactory &factory,
                                rq::Expression &asc_ex) {
  RQ_ASSERT(factory.getIsEmpty(), "factory not empty");
  if (asc_ex.getKeyword() != rq::Keyword::ASCRIBE_LOW) {
    return asc_ex;
  }
  rq::Expression &unasc_ex = asc_ex.getBranch();
  RQ_ASSERT(unasc_ex.getHasNext(), "missing attributes");
  for (rq::Expression &attrib_ex : unasc_ex.getNextSubrange()) {
    RQ_ASSERT(attrib_ex.getKeyword() == rq::Keyword::INSTANTIATE_LOW_ATTRIBUTE,
              "invalid attribute keyword");
    rq::Expression &attrib_rv_ex = attrib_ex.getBranch();
    rq::Expression *attrib_attachment_ex_ptr = attrib_rv_ex.getNextPtr();
    rq::StaticRvalue attrib_rv =
        this->evaluateStaticRvalue(module, table, attrib_rv_ex);
    if (!attrib_rv.getType().getIsLowAttributeType()) {
      RQ_UNHANDLED_ERROR("not low attribute");
    }
    rq::LowAttribute attrib = attrib_rv.getValue().getLowAttribute();
    if (!factory.addFlag(attrib, attrib_ex, attrib_attachment_ex_ptr)) {
      RQ_UNHANDLED_ERROR("duplicate attribute of kind");
    }
  }
  return unasc_ex;
}

void Evaluator::evaluateGlobalScope(rq::Module &module,
                                    rq::SymbolTable &hosting_table,
                                    rq::Expression &first_ex) {
  using K = rq::Keyword;
  using LAK = rq::LowAttributeKind;
  using LF = rq::LowFuseFlags;
  for (rq::Expression &branch_ex : first_ex.getInclusiveNextSubrange()) {
    rq::LowFactory factory{};
    rq::Expression &unascribed_ex =
        this->evaluateLowFuseFlags(module, hosting_table, factory, branch_ex);
    rq::SymbolTable &containing_table =
        this->evaluateContainingTable(module, hosting_table, factory);
    switch (unascribed_ex.getKeyword()) {
    case K::FUNCTION: {
      if (!this->validateAttributes(
              containing_table, unascribed_ex, factory,
              LF::OPAQUE | LF::EXPORT | LF::PUBLIC | LF::CAPTURE | LF::INLINE |
                  LF::MANGLE | LF::DEPRECIATED | LF::EXPERIMENTAL |
                  LF::TEMPLATE | LF::CONSTRAINT | LF::WEIGHT | LF::REQUIRE |
                  LF::ENSURE)) {
        continue;
      }
      rq::Expression &name_ex = unascribed_ex.getBranch();
      rq::Expression *body_ex_ptr = name_ex.getNextPtr();
      rq::Name name = this->evaluateName(hosting_table, module, name_ex);
      rq::Function &func = this->getContext().allocateValue<rq::Function>(
          containing_table, name, hosting_table, unascribed_ex, &name_ex,
          factory.getFuseFlags(), module, body_ex_ptr, /*template_ptr=*/nullptr,
          factory.getExpressionPair(LAK::MANGLE_ATTRIBUTE).getAttachmentExPtr(),
          factory.getExpressionPair(LAK::REQUIRE_ATTRIBUTE)
              .getAttachmentExPtr(),
          factory.getExpressionPair(LAK::ENSURE_ATTRIBUTE)
              .getAttachmentExPtr());
      containing_table.addMember(this->getContext(), name, func);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
}

[[nodiscard]] rq::LocalBreak Evaluator::evaluateStaticOrDynamicLocalStatements(
    rq::DottedInstructionFactory &inst_factory, rq::Module &module,
    rq::Function &function, rq::SymbolTable &table, rq::Expression &state0_ex) {
  for (rq::Expression &state_ex : state0_ex.getInclusiveNextSubrange()) {
    rq::LowFactory flags_factory;
  }
}

[[nodiscard]] rq::LocalBreak Evaluator::evaluateDynamicLocalStatement(
    rq::DottedInstructionFactory &inst_factory, rq::LowFactory &flags_factory,
    rq::Function &function, rq::ConstantSymbol &result_type,
    rq::SymbolTable &table, rq::Module &module, rq::Expression &state_ex) {
  using K = rq::Keyword;
  using O = rq::Opcode;
  switch (state_ex.getKeyword()) {
  case K::ASSIGN: {
    rq::Expression &lvalue_ex = state_ex.getBranch();
    rq::Expression &rvalue_ex = lvalue_ex.getNext();
    rq::DynamicRvalue rvalue =
        this->evaluateDynamicRvalue(module, table, rvalue_ex);
    if (rvalue.getIsEmpty()) {
      break;
    }
    rq::DynamicLvalue lvalue =
        this->evaluateDynamicLvalue(result_type, table, module, lvalue_ex);
    if (lvalue.getIsEmpty()) {
      break;
    }
    rq::Symbol *type_ptr =
        this->completeType(lvalue.getType().getSymbol(), rvalue.getType());
    if (type_ptr == nullptr) {
      RQ_UNHANDLED_ERROR("type not determined");
      break;
    }
    rq::Symbol &type = rq::dereferencePtr(type_ptr);
    if (type != lvalue.getType().getSymbol()) {
      rq::LocalDynamicVariable &var =
          llvm::cast<rq::LocalDynamicVariable>(lvalue.getSymbol());
      rq::ConstantSymbol &type_ct = this->getContext().acquireConstantSymbol(
          lvalue.getType().getInfoFlags(), type);
      var.completeType(type_ct);
    }
    rq::Entity &folded_v = this->foldDynamicRvalue(rvalue.getValue(), type);
    rq::Instruction &inst = this->getContext().acquireInstruction(O::ASSIGN);
    inst.setAddress0(lvalue.getSymbol());
    inst.setAddress1(folded_v);
    factory.append(inst);
    break;
  }
  case K::IF:
    [[fallthrough]];
  case K::ELSE_IF: {
    // TODO
  }
  case K::ELSE: {
    // TODO
  }
  case K::SCOPE: {
    if (!state_ex.getHasBranch()) {
      break;
    }
    rq::ScopeStatement &scope =
        this->getContext().allocateValue<rq::ScopeStatement>(
            table, state_ex, rq::LowFuseFlags::NONE, module);
    rq::Expression &branch0_ex = state_ex.getBranch();
    rq::Instruction *next_ptr = this->evaluateLocalScope(
        function, result_type, scope, module, branch0_ex);
    if (next_ptr == nullptr) {
      break;
    }
    rq::Instruction &next = rq::dereferencePtr(next_ptr);
    factory.append(next);
    break;
  }
  case K::BLOCK: {
    if (!state_ex.getHasBranch()) {
      break;
    }
    rq::Expression &branch0_ex = state_ex.getBranch();
    rq::Instruction *next_ptr = this->evaluateLocalScope(
        function, result_type, table, module, branch0_ex);
    if (next_ptr == nullptr) {
      break;
    }
    rq::Instruction &next = rq::dereferencePtr(next_ptr);
    factory.append(next);
    break;
  }
  case K::RETURN: {
    rq::Instruction &inst = this->getContext().acquireInstruction(O::RETURN);
    factory.append(inst);
    return rq::LocalBreak(rq::LocalBreakKind::RETURN);
  }
  default:
    RQ_UNREACHABLE();
  }
  return rq::LocalBreak();
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
  bool scope_done = false;
  // using S = rq::SymbolKind;
  rq::DottedInstructionFactory factory(this->getContext(), O::STATEMENT);
  for (rq::Expression &state_ex : first_ex.getInclusiveNextSubrange()) {
    if (scope_done) {
      RQ_UNHANDLED_ERROR("unreachable dynamic statement");
    }
  }
  if (!scope_done && table != function) {
    rq::Name result_name(K::RESULT);
    auto result_list = table.lookupList(result_name);
    if (!result_list.getIsEmpty()) {
      rq::LocalDynamicVariable &result =
          llvm::cast<rq::LocalDynamicVariable>(result_list.getHead());
      if (result.getContainingTable() == table) {
        RQ_UNHANDLED_ERROR(
            "must return from scope where result is initialized");
      }
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
  rq::SymbolTable &hosting_table = this->getContext().getTop();
  this->evaluateGlobalScope(module, hosting_table, body_ex);
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
  using K = rq::Keyword;
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
  } else if (func.getName().getKeyword() == K::MAIN) {
    llvm::StringRef main_name = this->getContext().saveString("main");
    func.setMangledName(main_name);
  }
  if (!func.getMangledName().empty()) {
    llvm::StringRef mangled_str = func.getMangledName();
    rq::Name mangled_name(mangled_str);
    this->getContext().getC().addMember(this->getContext(), mangled_name, func);
  }
  rq::Expression &statement0 =
      rq::dereferencePtr(func.getFirstBodyExpressionPtr());
  rq::Expression *body_ptr = &statement0;
  if (statement0.getIsUltimate()) {
    // TODO static statements before signature
    RQ_TODO_IMPLEMENTATION();
  }
  rq::Expression &sig_ex = statement0;
  rq::StaticRvalue sig_rv = this->evaluateStaticRvalue(
      func.getModule(), func.getHostingTable(), sig_ex);
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
  using TF = rq::HighFuseFlags;
  switch (lvalue_ex.getKeyword()) {
  case K::IDENTIFIER_LITERAL: {
    rq::Name name = this->evaluateName(table, module, lvalue_ex);
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
      if (rq::getHasNone(var.getType().getInfoFlags(), TF::VAR)) {
        RQ_UNHANDLED_ERROR("assigning to constant variable");
      }
      return rq::DynamicLvalue(var, var.getType());
    }
  }
  case K::BINDING: {
    rq::Expression &var_ex = lvalue_ex.getBranch();
    rq::Expression &type_ex = var_ex.getNext();
    rq::StaticRvalue type_rvalue =
        this->evaluateStaticRvalue(module, table, type_ex);
    rq::Symbol &type_type = type_rvalue.getType();
    if (type_type != this->getContext().acquireSymbolType()) {
      RQ_UNHANDLED_ERROR("not type");
    }
    rq::StaticSymbol static_sy = type_rvalue.getValue().getSymbol();
    if (var_ex.getKeyword() == K::RESULT) {
      static_sy.symbol_ptr = this->completeType(
          rq::dereferencePtr(static_sy.symbol_ptr), result_type.getSymbol());
    }
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
      rq::LocalDynamicVariable &var =
          this->getContext().allocateValue<rq::LocalDynamicVariable>(
              name, table, table, module, rq::LowFuseFlags::NONE, type);
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
Evaluator::evaluateStaticRvalue(rq::Module &module, rq::SymbolTable &table,
                                rq::Expression &rvalue_ex) {
  using K = rq::Keyword;
  using S = rq::SymbolKind;
  rq::HighFactory factory{};
  rq::Expression *unascribed_ex_ptr = nullptr;
  if (rvalue_ex.getKeyword() == K::ASCRIBE_HIGH) {
    rq::Expression &unascribed_ex = rvalue_ex.getBranch();
    unascribed_ex_ptr = &unascribed_ex;
    for (rq::Expression &attrib_ex : unascribed_ex.getNextSubrange()) {
      rq::Expression &attrib_rv_ex = attrib_ex.getBranch();
      rq::Expression *attrib_br_ex_ptr = attrib_rv_ex.getNextPtr();
      rq::StaticRvalue attrib_rv =
          this->evaluateStaticRvalue(module, table, attrib_rv_ex);
      if (!attrib_rv.getType().getIsHighAttributeType()) {
        RQ_UNHANDLED_ERROR("not high attribute");
      }
      rq::HighAttribute attrib = attrib_rv.getValue().getHighAttribute();
      if (!factory.addFlag(attrib, attrib_br_ex_ptr)) {
        RQ_UNHANDLED_ERROR("duplicate attribute of kind");
      }
    }
  } else {
    unascribed_ex_ptr = &rvalue_ex;
  }
  rq::Expression &unascribed_ex = rq::dereferencePtr(unascribed_ex_ptr);
  switch (unascribed_ex.getKeyword()) {
  case K::SIGNED_INTEGER: {
    if (rvalue_ex.getHasBranch()) {
      RQ_TODO_IMPLEMENTATION();
    }
    rq::Symbol &symbol = this->getContext().acquireSignedIntegerType();
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::StaticRvalue(rq::StaticSymbol{factory.getFuseFlags(), &symbol},
                            type);
  }
  case K::INFERENCE: {
    rq::Symbol &symbol = this->getContext().acquireInferenceType();
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::StaticRvalue(rq::StaticSymbol{factory.getFuseFlags(), &symbol},
                            type);
  }
  case K::MANGLE: {
    rq::LowAttribute value = rq::LowAttribute::MANGLE;
    rq::Symbol &type = this->getContext().acquireMangleAttributeType();
    return rq::StaticRvalue(value, type);
  }
  case K::VAR: {
    rq::HighAttribute value = rq::HighAttribute::VAR;
    rq::Symbol &type = this->getContext().acquireVarAttributeType();
    return rq::StaticRvalue(value, type);
  }
  case K::INSTANTIATE_SIGNATURE: {
    rq::Expression &return_ex = unascribed_ex.getBranch();
    if (return_ex.getHasNext()) {
      // TODO parameters
      RQ_TODO_IMPLEMENTATION();
    }
    rq::StaticRvalue return_rv =
        this->evaluateStaticRvalue(module, table, return_ex);
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
        nullptr, 0, 0, 0, rvalue_ex, 0, module, return_ct, nullptr);
    return rq::StaticRvalue(rq::StaticSymbol({}, &sig),
                            this->getContext().acquireSymbolType());
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::evaluateDynamicRvalue(rq::Module &module, rq::SymbolTable &table,
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
  case K::LOGICAL_AND: {
    return this->evaluateDynamicLogicalRvalue(table, module, rvalue_ex,
                                              O::LOGICAL_AND);
  }
  case K::LOGICAL_OR: {
    return this->evaluateDynamicLogicalRvalue(table, module, rvalue_ex,
                                              O::LOGICAL_OR);
  }
  case K::LOGICAL_AND_WITH_SHORTCIRCUIT: {
    return this->evaluateDynamicLogicalRvalue(table, module, rvalue_ex,
                                              O::LOGICAL_AND_WITH_SHORTCIRCUIT);
  }
  case K::LOGICAL_OR_WITH_SHORTCIRCUIT: {
    return this->evaluateDynamicLogicalRvalue(table, module, rvalue_ex,
                                              O::LOGICAL_OR_WITH_SHORTCIRCUIT);
  }
  case K::LOGICAL_COMPLEMENT: {
    rq::Expression &comp_rv_ex = rvalue_ex.getBranch();
    rq::DynamicRvalue comp_rv =
        this->evaluateDynamicRvalue(module, table, comp_rv_ex);
    if (comp_rv.getIsEmpty()) {
      return rq::DynamicRvalue();
    }
    if (comp_rv.getType() != this->getContext().acquireBooleanType()) {
      RQ_UNHANDLED_ERROR("invalid logical complement type");
    }
    rq::Instruction &negate =
        this->getContext().acquireInstruction(O::LOGICAL_COMPLEMENT);
    negate.setAddress0(comp_rv.getValue());
    return rq::DynamicRvalue(negate, comp_rv.getType());
  }
  case K::LESS: {
    return this->evaluateDynamicOrderedComparisonRvalue(table, module,
                                                        rvalue_ex, O::LESS);
  }
  case K::GREATER: {
    return this->evaluateDynamicOrderedComparisonRvalue(table, module,
                                                        rvalue_ex, O::LESS);
  }
  case K::LESS_EQUAL: {
    return this->evaluateDynamicOrderedComparisonRvalue(
        table, module, rvalue_ex, O::LESS_EQUAL);
  }
  case K::GREATER_EQUAL: {
    return this->evaluateDynamicOrderedComparisonRvalue(
        table, module, rvalue_ex, O::GREATER_EQUAL);
  }
  case K::EQUAL: {
    return this->evaluateDynamicEquivalenceComparisonRvalue(
        table, module, rvalue_ex, O::EQUAL);
  }
  case K::NOT_EQUAL: {
    return this->evaluateDynamicEquivalenceComparisonRvalue(
        table, module, rvalue_ex, O::NOT_EQUAL);
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
  case K::NEGATE: {
    rq::Expression &negate_rv_ex = rvalue_ex.getBranch();
    rq::DynamicRvalue negate_rv =
        this->evaluateDynamicRvalue(module, table, negate_rv_ex);
    if (negate_rv.getIsEmpty()) {
      return rq::DynamicRvalue();
    }
    if (!negate_rv.getType().getIsFloatType() &&
        !negate_rv.getType().getIsIntegerType()) {
      RQ_UNHANDLED_ERROR("invalid negate type");
    }
    if (negate_rv.getType().getIsUnsignedType()) {
      RQ_UNHANDLED_ERROR("not signed");
    }
    rq::Instruction &negate = this->getContext().acquireInstruction(O::NEGATE);
    negate.setAddress0(negate_rv.getValue());
    return rq::DynamicRvalue(negate, negate_rv.getType());
  }
  case K::TRUE: {
    llvm::APInt llvm_boolean = llvm::APInt(1, 1u, false);
    rq::Entity &value = this->getContext().acquireConstantWord(llvm_boolean);
    rq::Symbol &type = this->getContext().acquireBooleanType();
    return rq::DynamicRvalue(value, type);
  }
  case K::FALSE: {
    llvm::APInt llvm_boolean = llvm::APInt(1, 0u, false);
    rq::Entity &value = this->getContext().acquireConstantWord(llvm_boolean);
    rq::Symbol &type = this->getContext().acquireBooleanType();
    return rq::DynamicRvalue(value, type);
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
        this->evaluateDynamicRvalue(module, table, branch_ex);
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

[[nodiscard]] rq::DynamicRvalue Evaluator::evaluateDynamicLogicalRvalue(
    rq::SymbolTable &table, rq::Module &module, rq::Expression &rvalue_ex,
    rq::Opcode opcode) {
  rq::DottedInstructionFactory factory(this->getContext(), opcode);
  for (rq::Expression &branch_ex : rvalue_ex.getBranchSubrange()) {
    rq::DynamicRvalue branch_rv =
        this->evaluateDynamicRvalue(module, table, branch_ex);
    factory.append(branch_rv.getValue());
    if (branch_rv.getIsEmpty()) {
      RQ_UNHANDLED_ERROR("empty branch rv");
    }
    if (branch_rv.getType() != this->getContext().acquireBooleanType()) {
      RQ_UNHANDLED_ERROR("branch of logical operator must be boolean");
    }
  }
  rq::Entity &rvalue = factory.getOuter();
  rq::Symbol &type = this->getContext().acquireBooleanType();
  return rq::DynamicRvalue(rvalue, type);
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::evaluateDynamicOrderedComparisonRvalue(rq::SymbolTable &table,
                                                  rq::Module &module,
                                                  rq::Expression &rvalue_ex,
                                                  rq::Opcode opcode) {
  rq::Expression &branch0_ex = rvalue_ex.getBranch();
  rq::Expression &branch1_ex = branch0_ex.getNext();
  rq::DynamicRvalue branch0_rv =
      this->evaluateDynamicRvalue(module, table, branch0_ex);
  if (branch0_rv.getIsEmpty()) {
    return rq::DynamicRvalue();
  }
  rq::DynamicRvalue branch1_rv =
      this->evaluateDynamicRvalue(module, table, branch1_ex);
  if (branch1_rv.getIsEmpty()) {
    return rq::DynamicRvalue();
  }
  rq::Symbol &branch0_ty = branch0_rv.getType();
  rq::Symbol &branch1_ty = branch1_rv.getType();
  rq::Symbol *ty_ptr = nullptr;
  if (branch0_ty == this->getContext().acquireIntegerLiteralType()) {
    if (branch1_ty.getIsLiteralType() || branch1_ty.getIsNumericType()) {
      ty_ptr = &branch1_ty;
    }
  } else if (branch0_ty == this->getContext().acquireFloatLiteralType()) {
    if (branch1_ty == this->getContext().acquireFloatLiteralType() ||
        branch1_ty.getIsNumericType()) {
      ty_ptr = &branch1_ty;
    }
  } else if (branch0_ty.getIsIntegerType()) {
    if (branch1_ty == branch0_ty ||
        branch1_ty == this->getContext().acquireIntegerLiteralType()) {
      ty_ptr = &branch0_ty;
    }
  } else if (branch0_ty.getIsFloatType()) {
    if (branch1_ty == branch0_ty || branch1_ty.getIsLiteralType()) {
      ty_ptr = &branch0_ty;
    }
  }
  if (ty_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("no builtin ordered relation");
  }
  rq::Symbol &ty = rq::dereferencePtr(ty_ptr);
  rq::Symbol &complete_ty = this->deliteralizeType(ty);
  rq::Entity &branch0_v =
      this->foldDynamicRvalue(branch0_rv.getValue(), complete_ty);
  rq::Entity &branch1_v =
      this->foldDynamicRvalue(branch1_rv.getValue(), complete_ty);
  rq::Instruction &pair =
      this->getContext().acquireInstruction(rq::Opcode::RVALUE_PAIR);
  pair.setAddress0(branch0_v);
  pair.setAddress1(branch1_v);
  rq::Instruction &inst = this->getContext().acquireInstruction(opcode);
  inst.setAddress0(complete_ty);
  inst.setAddress1(pair);
  rq::Symbol &boolean_ty = this->getContext().acquireBooleanType();
  return rq::DynamicRvalue(inst, boolean_ty);
}

[[nodiscard]] rq::DynamicRvalue
Evaluator::evaluateDynamicEquivalenceComparisonRvalue(rq::SymbolTable &table,
                                                      rq::Module &module,
                                                      rq::Expression &rvalue_ex,
                                                      rq::Opcode opcode) {
  rq::Expression &branch0_ex = rvalue_ex.getBranch();
  rq::Expression &branch1_ex = branch0_ex.getNext();
  rq::DynamicRvalue branch0_rv =
      this->evaluateDynamicRvalue(module, table, branch0_ex);
  if (branch0_rv.getIsEmpty()) {
    return rq::DynamicRvalue();
  }
  rq::DynamicRvalue branch1_rv =
      this->evaluateDynamicRvalue(module, table, branch1_ex);
  if (branch1_rv.getIsEmpty()) {
    return rq::DynamicRvalue();
  }
  rq::Symbol &branch0_ty = branch0_rv.getType();
  rq::Symbol &branch1_ty = branch1_rv.getType();
  rq::Symbol *ty_ptr = nullptr;
  if (branch0_ty == this->getContext().acquireIntegerLiteralType()) {
    if (branch1_ty.getIsLiteralType() || branch1_ty.getIsNumericType()) {
      ty_ptr = &branch1_ty;
    }
  } else if (branch0_ty == this->getContext().acquireFloatLiteralType()) {
    if (branch1_ty == this->getContext().acquireFloatLiteralType() ||
        branch1_ty.getIsNumericType()) {
      ty_ptr = &branch1_ty;
    }
  } else if (branch0_ty.getIsIntegerType()) {
    if (branch1_ty == branch0_ty ||
        branch1_ty == this->getContext().acquireIntegerLiteralType()) {
      ty_ptr = &branch0_ty;
    }
  } else if (branch0_ty.getIsFloatType()) {
    if (branch1_ty == branch0_ty || branch1_ty.getIsLiteralType()) {
      ty_ptr = &branch0_ty;
    }
  } else if (branch0_ty == this->getContext().acquireBooleanType()) {
    if (branch1_ty == this->getContext().acquireBooleanType()) {
      ty_ptr = &this->getContext().acquireBooleanType();
    }
  }
  if (ty_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("no builtin equivalence relation");
  }
  rq::Symbol &ty = rq::dereferencePtr(ty_ptr);
  rq::Symbol &complete_ty = this->deliteralizeType(ty);
  rq::Entity &branch0_v =
      this->foldDynamicRvalue(branch0_rv.getValue(), complete_ty);
  rq::Entity &branch1_v =
      this->foldDynamicRvalue(branch1_rv.getValue(), complete_ty);
  rq::Instruction &pair =
      this->getContext().acquireInstruction(rq::Opcode::RVALUE_PAIR);
  pair.setAddress0(branch0_v);
  pair.setAddress1(branch1_v);
  rq::Instruction &inst = this->getContext().acquireInstruction(opcode);
  inst.setAddress0(complete_ty);
  inst.setAddress1(pair);
  rq::Symbol &boolean_ty = this->getContext().acquireBooleanType();
  return rq::DynamicRvalue(inst, boolean_ty);
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
    case O::LOGICAL_AND:
      [[fallthrough]];
    case O::LOGICAL_OR:
      [[fallthrough]];
    case O::LOGICAL_AND_WITH_SHORTCIRCUIT:
      [[fallthrough]];
    case O::LOGICAL_OR_WITH_SHORTCIRCUIT:
      [[fallthrough]];
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
    case O::NEGATE:
      [[fallthrough]];
    case O::LOGICAL_COMPLEMENT: {
      rq::Entity &old_address0 = inst.popAddress0();
      rq::Entity &new_address0 = this->foldDynamicRvalue(old_address0, type);
      inst.setAddress0(new_address0);
      return inst;
    }
    case O::LESS:
      [[fallthrough]];
    case O::GREATER:
      [[fallthrough]];
    case O::LESS_EQUAL:
      [[fallthrough]];
    case O::GREATER_EQUAL:
      [[fallthrough]];
    case O::EQUAL:
      [[fallthrough]];
    case O::NOT_EQUAL: {
      rq::Symbol &nest_type = llvm::cast<rq::Symbol>(inst.getAddress0());
      rq::Instruction &pair = llvm::cast<rq::Instruction>(inst.getAddress1());
      rq::Entity &old_address0 = pair.popAddress0();
      rq::Entity &new_address0 =
          this->foldDynamicRvalue(old_address0, nest_type);
      pair.setAddress0(new_address0);
      rq::Entity &old_address1 = pair.popAddress1();
      rq::Entity &new_address1 =
          this->foldDynamicRvalue(old_address1, nest_type);
      pair.setAddress1(new_address1);
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
  case K::THIS:
    [[fallthrough]];
  case K::OUT:
    [[fallthrough]];
  case K::RESULT: {
    rq::Name name{name_ex.getKeyword()};
    return name;
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::SymbolTable &
Evaluator::evaluateContainingTable(rq::Module &module,
                                   rq::SymbolTable &hosting_table,
                                   rq::LowFactory &factory) {
  using LFF = rq::LowFuseFlags;
  using HFF = rq::HighFuseFlags;
  using LAK = rq::LowAttributeKind;
  if (rq::getHasNone(factory.getFuseFlags(), LFF::FLANK)) {
    return hosting_table;
  }
  rq::LowExpressionPair pair = factory.getExpressionPair(LAK::FLANK_ATTRIBUTE);
  if (pair.getAttachmentExPtr() == nullptr) {
    return hosting_table; // this is error, but is handled in validateAttributes
  }
  rq::Expression &attachment_ex = rq::dereferencePtr(pair.getAttachmentExPtr());
  rq::StaticRvalue anchor_rv =
      this->evaluateStaticRvalue(module, hosting_table, attachment_ex);
  if (anchor_rv.getType() != this->getContext().acquireSymbolType()) {
    RQ_UNHANDLED_ERROR("expected symbol");
  }
  if (anchor_rv.getValue().getSymbol().flags != HFF::NONE) {
    RQ_UNHANDLED_ERROR("expected no high flags");
  }
  rq::Symbol &anchor_sy =
      rq::dereferencePtr(anchor_rv.getValue().getSymbol().symbol_ptr);
  if (!llvm::isa<rq::Anchor>(anchor_sy)) {
    RQ_UNHANDLED_ERROR("expected anchor");
  }
  rq::Anchor &anchor = llvm::cast<rq::Anchor>(anchor_sy);
  rq::SymbolTable &containing_table = anchor.getVessel();
  return containing_table;
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
        to_inf_array.getChild().getInfoFlags(), child);
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
        to_subtype.getChild().getInfoFlags(), child);
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
        to_array.getChild().getInfoFlags(), child);
    rq::ArraySubtype &subtype =
        this->getContext().acquireArraySubtype(child_ct, to_array.getCount());
    return &subtype;
  }
  return &this->deliteralizeType(from_type);
}

[[nodiscard]] rq::Symbol &Evaluator::deliteralizeType(rq::Symbol &type) {
  using S = rq::SymbolKind;
  using HFF = rq::HighFuseFlags;
  if (type.getKind() == S::INTEGER_LITERAL_TYPE) {
    return this->getContext().acquireSignedIntegerType();
  }
  if (type.getKind() == S::FLOAT_LITERAL_TYPE) {
    return this->getContext().acquireSingleType();
  }
  if (type.getKind() == S::STRING_LITERAL_TYPE) {
    rq::Symbol &child = this->getContext().acquireCharType();
    rq::ConstantSymbol &child_ct =
        this->getContext().acquireConstantSymbol(HFF::NONE, child);
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
        uc_subtype.getChild().getInfoFlags(), child);
    rq::UncountedSubtype &new_subtype =
        this->getContext().acquireUncountedSubtype(uc_subtype.getKind(),
                                                   child_ct);
    return new_subtype;
  }
  if (llvm::isa<rq::ArraySubtype>(type)) {
    rq::ArraySubtype &array = llvm::cast<rq::ArraySubtype>(type);
    rq::Symbol &child = this->deliteralizeType(array.getChild().getSymbol());
    rq::ConstantSymbol &child_ct = this->getContext().acquireConstantSymbol(
        array.getChild().getInfoFlags(), child);
    rq::ArraySubtype &new_array =
        this->getContext().acquireArraySubtype(child_ct, array.getCount());
    return new_array;
  }
  return type;
}

[[nodiscard]] bool
Evaluator::validateAttributes(rq::SymbolTable &containing_table,
                              rq::Expression &unascribed_ex,
                              rq::LowFactory &factory, rq::LowFuseFlags flags) {
  using LA = rq::LowAttribute;
  using LAK = rq::LowAttributeKind;
  using LFF = rq::LowFuseFlags;
  for (unsigned attribute_i = static_cast<unsigned>(LA::NONE) + 1;
       attribute_i < static_cast<unsigned>(LA::LAST); attribute_i++) {
    LA attribute = static_cast<LA>(attribute_i);
    LFF flag = rq::getFuseFlags(attribute);
    if (!rq::getHasSome(factory.getFuseFlags(), flag)) {
      continue;
    }
    LAK kind = rq::getKind(attribute);
    rq::LowExpressionPair pair = factory.getExpressionPair(kind);
    if (rq::getMustHaveAttachment(attribute)) {
      if (pair.getAttachmentExPtr() == nullptr) {
        RQ_UNHANDLED_ERROR("attribute must have attachment");
      }
    } else if (rq::getMustNotHaveAttachment(attribute)) {
      if (pair.getAttachmentExPtr() != nullptr) {
        RQ_UNHANDLED_ERROR("attribute must not have attachment");
      }
    }
    if (rq::getHasNone(flags, flag)) {
      this->getContext().logErrorInvalidLowAttribute(
          unascribed_ex, pair.getInstantiationEx(), attribute);
    }
  }
  if (rq::getHasNone(factory.getFuseFlags(), LFF::TEMPLATE)) {
    if (rq::getHasAll(factory.getFuseFlags(), LFF::CONSTRAINT)) {
      RQ_UNHANDLED_ERROR("constraint low attribute must be paired with "
                         "template low attribute");
    }
    if (rq::getHasAll(factory.getFuseFlags(), LFF::WEIGHT)) {
      RQ_UNHANDLED_ERROR(
          "weight low attribute must be paired with template low attribute");
    }
  }
  if (!containing_table.getIsObjectScope()) {
    if (rq::getHasAll(factory.getFuseFlags(), LFF::PUBLIC)) {
      RQ_UNHANDLED_ERROR("symbol with public low attribute not in object scope")
    }
  }
}

} // namespace rq
