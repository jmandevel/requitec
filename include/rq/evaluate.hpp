#pragma once
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <optional>

namespace rq {

struct Context;
struct Module;
struct Expression;
struct SymbolTable;
struct Instruction;
struct ExpressionFlagsFactory;

struct Rvalue final {
  using Self = rq::Rvalue;

  rq::Symbol *_type_ptr{nullptr};
  rq::Entity *_value_ptr{nullptr};

  explicit Rvalue() = default;
  explicit Rvalue(rq::Symbol &type, rq::Entity &value)
      : _type_ptr(&type), _value_ptr(&value) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const {
    return this->_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getValue() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getValue() {
    return rq::dereferencePtr(this->_type_ptr);
  }
};

struct Evaluator final {
  using Self = rq::Evaluator;

  rq::Context *_context_ptr;
  bool _is_ok : 1 = true;

  Evaluator(rq::Context &context) : _context_ptr(&context) {}
  Evaluator(const Self &) = delete;
  Evaluator(Self &&) = delete;
  ~Evaluator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  void setNotOk() { this->_is_ok = false; }
  void evaluateSourceModule();
  void evaluateModule(rq::Module &module);
  void evaluateGlobalScope(rq::SymbolTable &table, rq::Module &module,
                    rq::Expression &first_ex);
  void evaluateAllModuleSymbols(rq::Module &module);

  [[nodiscard]] rq::Rvalue evaluateRvalue(rq::SymbolTable &table,
                                          rq::Module &module,
                                          rq::Expression &rvalue_ex);
  [[nodiscard]] rq::Expression &
  evaluateExpressionAttributes(rq::ExpressionFlagsFactory &out_factory,
                               rq::SymbolTable &table, rq::Module &module,
                               rq::Expression &outer_ex);
};
} // namespace rq
