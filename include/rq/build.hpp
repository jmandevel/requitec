#pragma once

#include <rq/utility.hpp>

#include <functional>

namespace rq {

struct Context;

struct Builder final {
  using Self = rq::Builder;

  std::reference_wrapper<rq::Context> _context_ref;
  bool _is_ok{true};

  Builder(rq::Context &context) : _context_ref(context) {}
  Builder(const Self &) = delete;
  Builder(Self &&) = delete;
  ~Builder() = default;
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