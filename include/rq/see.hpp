#pragma once

#include <rq/ast.hpp>

#include <llvm/ADT/StringRef.h>

namespace rq {

struct SymbolicExecutionEngine final {
  using Self = rq::SymbolicExecutionEngine;

  std::int64_t _initial_steps_left;
  std::int64_t _steps_left;

  void _resetStepsLeft() {
    this->_steps_left = this->_initial_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::int64_t getStepsLeft() const {
    return this->_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::int64_t getInitialStepsLeft() const {
    return this->_initial_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStepsLeft() const {
    return this->_steps_left > 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool _decrementStep() {
    RQ_ASSERT(this->getHasStepsLeft(), "no steps left");
    this->_steps_left--;
    return this->getHasStepsLeft();
  }
  [[nodiscard]] llvm::StringRef evaluateName(rq::Node &expression);
};

} // namespace rq