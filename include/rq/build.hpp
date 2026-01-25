#pragma once

#include <rq/utility.hpp>

#include <functional>

namespace rq {

struct Context;

struct RequiteIrBuilder final {
  using Self = rq::RequiteIrBuilder;

  std::reference_wrapper<rq::Context> _context_ref;
  bool _is_ok{true};

  RequiteIrBuilder(rq::Context &context) : _context_ref(context) {}
  RequiteIrBuilder(const Self &) = delete;
  RequiteIrBuilder(Self &&) = delete;
  ~RequiteIrBuilder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  void buildRequiteIr();
};

struct LlvmIrBuilder final {
  using Self = rq::LlvmIrBuilder;

  std::reference_wrapper<rq::Context> _context_ref;
  bool _is_ok{true};

  LlvmIrBuilder(rq::Context &context) : _context_ref(context) {}
  LlvmIrBuilder(const Self &) = delete;
  LlvmIrBuilder(Self &&) = delete;
  ~LlvmIrBuilder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  void buildLlvmIr();
};

} // namespace rq