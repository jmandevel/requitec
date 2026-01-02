#pragma once

#include <rq/context.hpp>
#include <rq/value.hpp>
#include <rq/utility.hpp>

#include <functional>

namespace rq {

struct Tabulator final {
  using Self = rq::Tabulator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::Module> _module_ref;
  bool _is_ok = true;

  Tabulator(rq::Context &context, rq::Module& module) : _context_ref(context), _module_ref(module) {}
  Tabulator(const Self &) = delete;
  Tabulator(Self &&) = delete;
  ~Tabulator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() { 
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule() { 
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const {
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const {
    return this->_is_ok;
  }
  void setNotOk() {
    this->_is_ok = false;
  }
  void tabulateModule();
  void tabulateGlobalForest(const rq::Expression& first, rq::Scope& scope);
  void tabulateEntryPoint(const rq::Expression& expression, rq::Scope& scope);
};

} // namespace rq
