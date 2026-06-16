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
  std::ignore = first_ex;
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

} // namespace rq
