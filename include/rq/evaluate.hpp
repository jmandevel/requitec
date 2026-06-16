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

  bool _is_numeric_literal_tree : 1;
  rq::StaticValue _value;
  rq::Symbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE StaticRvalue(rq::Symbol &literal_type)
      : _is_numeric_literal_tree(true), _value(), _type_ptr(&literal_type) {
    RQ_ASSERT(literal_type.getIsLiteralType() &&
                  literal_type.getIsNumericType(),
              "not numeric literal type");
  }
  explicit RQ_ALWAYS_INLINE StaticRvalue(const rq::StaticValue &value,
                                         rq::Symbol &value_type)
      : _is_numeric_literal_tree(false), _value(value), _type_ptr(&value_type) {
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericLiteralTree() const {
    return this->_is_numeric_literal_tree;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &getValue() const {
    RQ_ASSERT(!this->getIsNumericLiteralTree(), "no value");
    return this->_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &getValue() {
    RQ_ASSERT(!this->getIsNumericLiteralTree(), "no value");
    return this->_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
};

struct DynamicRvalue final {
  using Self = rq::DynamicRvalue;

  rq::Entity *_value_ptr;
  rq::Symbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE DynamicRvalue(rq::Entity &value, rq::Symbol &type)
      : _value_ptr(&value), _type_ptr(&type) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getValue() const {
    return rq::dereferencePtr(this->_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getValue() {
    return rq::dereferencePtr(this->_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
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
  // etc

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicRvalue(rq::SymbolTable &table, rq::Module &module,
                        rq::Expression &rvalue_ex);
  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicIdentifier(rq::SymbolTable &table, rq::Module &module,
                            rq::Expression &rvalue_ex);
  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicArithmeticRvalue(rq::Opcode opcode, rq::SymbolTable &table,
                                  rq::Module &module,
                                  rq::Expression &rvalue_ex);
};
} // namespace rq
