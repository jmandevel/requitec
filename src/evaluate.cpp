#include <rq/context.hpp>
#include <rq/evaluate.hpp>
#include <rq/utility.hpp>

#include <optional>

namespace rq {

void Evaluator::evaluateSourceModule() {
  rq::Module &module = this->getContext().getSourceModule();
  this->surveyAllSymbols(module);
  if (!this->getIsOk()) {
    return;
  }
  this->implementAllSymbols(module);
}

void Evaluator::surveyAllSymbols(rq::Module &module) {
  rq::Top &top = this->getContext().getTop();
  rq::Expression &first_ex = module.getExpression().getBranch();
  this->surveyGlobalScope(module, top, first_ex);
}

void Evaluator::implementAllSymbols(rq::Module &module) {
  rq::Top &top = this->getContext().getTop();
  rq::Expression &first_ex = module.getExpression().getBranch();
  this->implementGlobalScope(module, top, first_ex);
}

void Evaluator::surveyGlobalScope(rq::Module &module, rq::SymbolTable &host,
                                  rq::Expression &first_ex) {
                                    using K = rq::Keyword;
  for (rq::Expression &statement_ex : first_ex.getInclusiveNextSubrange()) {
    rq::LowFactory low_factory;
    rq::Expression *unascribed_ex_ptr =
        this->evaluateLowFuseFlags(module, host, low_factory, statement_ex);
    if (unascribed_ex_ptr == nullptr) {
        continue;
    }
    rq::Expression& unascribed_ex = rq::dereferencePtr(unascribed_ex_ptr);
    rq::SymbolTable * container_ptr = this->evaluateContainer(module, host, low_factory);
    if (container_ptr == nullptr) {
        continue;
    }
    rq::SymbolTable& container = rq::dereferencePtr(container_ptr);
    switch (statement_ex.getKeyword()) {
        case K::ASSIGN:
            RQ_TODO_IMPLEMENTATION();
        case K::CALL:
            RQ_TODO_IMPLEMENTATION();
        case K::REMOVE_OF:
            RQ_TODO_IMPLEMENTATION();
        case K::INPLACE_DESTROY_OF:
            RQ_TODO_IMPLEMENTATION();
        case K::INPLACE_INITIALIZE_OF:
            RQ_TODO_IMPLEMENTATION();
        case K::FUNCTION:
            RQ_TODO_IMPLEMENTATION();
        case K::IMPLEMENT_FUNCTION();
            RQ_TODO_IMPLEMENTATION();
        case K::USE_FUNCTION:
            RQ_TODO_IMPLEMENTATION();
        case K::RETURN:
            RQ_TODO_IMPLEMENTATION();
        case K::BREAK_OF:
            RQ_TODO_IMPLEMENTATION();
        case K::CONTINUE_OF:
            RQ_TODO_IMPLEMENTATION();
        case K::CLASS:
            RQ_TODO_IMPLEMENTATION();
        case K::ENUM:
            RQ_TODO_IMPLEMENTATION();
        case K::INTERFACE:
            RQ_TODO_IMPLEMENTATION();
        case K::ADAPTER:
            RQ_TODO_IMPLEMENTATION()
        case K::IF_CHAIN:
            RQ_TODO_IMPLEMENTATION()
        case K::SWITCH_CHAIN:
            RQ_TODO_IMPLEMENTATION()
        case K::MATCH_CHAIN:
            RQ_TODO_IMPLEMENTATION()
        case K::SPIN_CHAIN:
            RQ_TODO_IMPLEMENTATION()
        case K::IF:
            RQ_TODO_IMPLEMENTATION()
        case K::FOR:
            RQ_TODO_IMPLEMENTATION()
        case K::WHILE:
            RQ_TODO_IMPLEMENTATION()
        case K::SCOPE:
            RQ_TODO_IMPLEMENTATION()
        case K::FOLD:
            RQ_TODO_IMPLEMENTATION()
        case K::IMPORT:
            RQ_TODO_IMPLEMENTATION()
        case K::NAMESPACE:
            RQ_TODO_IMPLEMENTATION()
        case K::DEBUG_BREAK:
            RQ_TODO_IMPLEMENTATION()
        case K::ABORT:
            RQ_TODO_IMPLEMENTATION()
        case K::ASSERT:
            RQ_TODO_IMPLEMENTATION()
        case K::UNREACHABLE:
            RQ_TODO_IMPLEMENTATION()
        case K::ASSUME:
            RQ_TODO_IMPLEMENTATION()
        case K::IGNORE_OF:
            RQ_TODO_IMPLEMENTATION()
        case K::DESTROY_OF:
            RQ_TODO_IMPLEMENTATION()
        default:
            break;
    }    
    RQ_UNREACHABLE();
  }
}

void Evaluator::implementGlobalScope(rq::Module &module, rq::SymbolTable &host,
                                     rq::Expression &first_ex) {}

} // namespace rq
