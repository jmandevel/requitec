#pragma once

#include <rq/utility.hpp>

#include <functional>

namespace rq {

struct Context;
struct ModuleSymbol;

struct Tabulator final {
  using Self = rq::Tabulator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::ModuleSymbol> _module_ref;
  bool _is_ok = true;

  Tabulator(rq::Context &context, rq::ModuleSymbol& module) : _context_ref(context), _module_ref(module) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getModule() { 
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &getModule() const {
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const {
    return this->_is_ok;
  }
  void setNotOk() {
    this->_is_ok = false;
  }
  void tabulateModule();
  void tabulateGlobalForest(const rq::Expression& first, rq::ScopeSymbol& scope);
  void tabulateEntryPoint(const rq::Expression& expression, rq::ScopeSymbol& scope);
};

} // namespace rq
