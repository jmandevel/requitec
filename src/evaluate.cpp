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
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  for (rq::Expression &branch_ex : first_ex.getInclusiveNextSubrange()) {
    switch (branch_ex.getKeyword()) {
    case K::MAIN: {
      rq::Name name(K::MAIN);
      rq::Function &func = this->getContext().allocateValue<rq::Function>(
          table, name, table, branch_ex, nullptr, rq::ExpressionFlags::NONE,
          module, nullptr, nullptr);
      table.addMember(this->getContext(), name, func);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
}

[[nodiscard]] rq::Instruction *
Evaluator::evaluateLocalScope(rq::Function &function, rq::SymbolTable &table,
                              rq::Module &module, rq::Expression &first_ex) {
  std::ignore = function;
  std::ignore = table;
  std::ignore = module;
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
      rq::Symbol *lvalue_ptr = this->evaluateLvalue(table, module, lvalue_ex);
      if (lvalue_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("lvalue error");
      }
      rq::Symbol &lvalue = rq::dereferencePtr(lvalue_ptr);
      // rq::Symbol &rvalue_ty = rvalue.getType();
      // TODO determine final type
      // TODO fold rvalue
      rq::Instruction &inst =
          this->getContext().acquireInstruction(rq::Opcode::ASSIGN);
      inst.setAddress0(lvalue);
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
  rq::Expression &first_ex = top_ex.getBranch();
  this->evaluateGlobalScope(this->getContext().getTop(), module, first_ex);
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
  rq::Expression &ex = func.getExpression();
  if (!ex.getHasBranch()) {
    return;
  }
  if (func.getName().getKeyword() == rq::Keyword::MAIN) {
    rq::Expression &statement0 = ex.getBranch();
    rq::Instruction *inst_ptr =
        this->evaluateLocalScope(func, func, func.getModule(), statement0);
    if (inst_ptr == nullptr) {
      return;
    }
    rq::Instruction &inst = rq::dereferencePtr(inst_ptr);
    func.setInstructions(inst);
  }
}

[[nodiscard]] rq::Symbol *Evaluator::evaluateLvalue(rq::SymbolTable &table,
                                                    rq::Module &module,
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
    // TODO check type matches procedure type
    // TODO check result not already initialized
    // TODO guarantee not initialize result in lower scope then ascend
    switch (var_ex.getKeyword()) {
    case K::RESULT: {
      rq::Name name(K::RESULT);
      auto found = table.lookupList(name);
      if (!found.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("result already initialized");
      }
      rq::LocalDynamicVariable &result =
          this->getContext().allocateValue<rq::LocalDynamicVariable>(
              name, table, table, module, rq::ExpressionFlags::NONE, type);
      table.addMember(this->getContext(), name, result);
      return &result;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] rq::StaticRvalue
Evaluator::evaluateStaticRvalue(rq::SymbolTable &table, rq::Module &module,
                                rq::Expression &rvalue_ex) {
  std::ignore = table;
  std::ignore = module;
  using K = rq::Keyword;
  switch (rvalue_ex.getKeyword()) {
  case K::SIGNED_INTEGER: {
    if (rvalue_ex.getHasBranch()) {
      RQ_TODO_IMPLEMENTATION();
    }
    rq::Symbol &symbol = this->getContext().acquireSignedIntegerType();
    rq::Symbol &type = this->getContext().acquireSymbolType();
    return rq::StaticRvalue(rq::StaticSymbol{{}, &symbol}, type);
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

} // namespace rq
