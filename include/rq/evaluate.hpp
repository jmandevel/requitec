#pragma once
#include <rq/generational_arena.hpp>
#include <rq/static_value.hpp>
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

struct StaticRvalue final {
  using Self = rq::StaticRvalue;

  rq::Symbol *_type_ptr{nullptr};
  rq::Entity *_entity_ptr{nullptr};
  rq::StaticValue _temp{};

  explicit StaticRvalue() = default;
  explicit StaticRvalue(rq::Symbol &type, rq::Entity &entity)
      : _type_ptr(&type), _entity_ptr(&entity) {}
  explicit StaticRvalue(rq::Symbol &type, rq::StaticValue &&temp)
      : _type_ptr(&type), _temp(std::move(temp)) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const {
    return this->_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getEntity() const {
    return rq::dereferencePtr(this->_entity_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getEntity() {
    return rq::dereferencePtr(this->_entity_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemp() const {
    return this->_entity_ptr == nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &getTemp() const {
    return this->_temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &getTemp() {
    return this->_temp;
  }
};

struct Evaluator final {
  using Self = rq::Evaluator;

  rq::GenerationalArena<rq::StaticValue> _static_values{};
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
  void evaluateGlobalScope(rq::SymbolTable &table, rq::Module &module,
                           rq::Expression &first_ex);
  void evaluateAllModuleSymbols(rq::Module &module);
  void evaluate(rq::Module &module);
  void evaluate(rq::Destructor &destructor);
  void evaluate(rq::Main &main);
  void evaluate(rq::ClassType &class_);
  void evaluate(rq::EnumerationType &enum_);
  void evaluate(rq::Interface &interface);
  void evaluate(rq::Adapter &adapter);
  void evaluate(rq::GlobalDynamicVariable &var);
  void evaluate(rq::GlobalStaticVariable &var);
  void evaluate(rq::ForwardRanger &ranger);
  void evaluate(rq::BackwardRanger &ranger);
  void evaluate(rq::Function &func);
  void evaluate(rq::Method &meth);
  void evaluate(rq::ExtensionMethod &meth);

  [[nodiscard]] rq::StaticRvalue
  evaluateStaticRvalue(rq::SymbolTable &table, rq::Module &module,
                       rq::Expression &rvalue_ex);
  [[nodiscard]] rq::Expression &
  evaluateExpressionAttributes(rq::ExpressionFlagsFactory &out_factory,
                               rq::SymbolTable &table, rq::Module &module,
                               rq::Expression &outer_ex);
};
} // namespace rq
