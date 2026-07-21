#pragma once
#include <rq/entity.hpp>
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
struct LowFlagsFactory;

struct StaticRvalue final {
  using Self = rq::StaticRvalue;

  bool _is_empty : 1 {true};
  bool _is_literal : 1 {false};
  rq::StaticValue _value{};
  rq::Symbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE StaticRvalue() = default;
  explicit RQ_ALWAYS_INLINE StaticRvalue(rq::Symbol &literal_type)
      : _is_empty(false), _is_literal(true), _value(),
        _type_ptr(&literal_type) {
    RQ_ASSERT(literal_type.getIsNumericType(), "not literal type");
  }
  explicit RQ_ALWAYS_INLINE StaticRvalue(const rq::StaticValue &value,
                                         rq::Symbol &value_type)
      : _is_empty(false), _is_literal(false), _value(value),
        _type_ptr(&value_type) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_is_empty;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return this->_is_literal;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &getValue() const {
    RQ_ASSERT(!this->getIsLiteral(), "is literal");
    return this->_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &getValue() {
    RQ_ASSERT(!this->getIsLiteral(), "is literal");
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

  rq::Entity *_value_ptr{nullptr};
  rq::Symbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE DynamicRvalue() = default;
  explicit RQ_ALWAYS_INLINE DynamicRvalue(rq::Entity &value, rq::Symbol &type)
      : _value_ptr(&value), _type_ptr(&type) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_value_ptr == nullptr;
  }
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

struct DynamicLvalue final {
  using Self = rq::DynamicLvalue;

  rq::Symbol *_symbol_ptr{nullptr};
  rq::ConstantSymbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE DynamicLvalue() = default;
  explicit RQ_ALWAYS_INLINE DynamicLvalue(rq::Symbol &symbol,
                                          rq::ConstantSymbol &type)
      : _symbol_ptr(&symbol), _type_ptr(&type) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_symbol_ptr == nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
};

enum class LocalBreakKind : std::uint_fast8_t {
  NONE,
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO
};

enum class LocalBreakFlags : std::uint_fast8_t {
  NONE = 0,

  HAS_TARGET = rq::getBit(0),
  LOOP = rq::getBit(1),
  CASE = rq::getBit(2),
  NON_FRAME = rq::getBit(3)
};

template <> struct is_flags<rq::LocalBreakFlags> final : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE rq::LocalBreakFlags
getFlags(rq::LocalBreakKind kind) {
  using LBK = rq::LocalBreakKind;
  using LBF = rq::LocalBreakFlags;
  switch (kind) {
  case LBK::NONE:
    return LBF::NONE;
  case LBK::RETURN:
    return LBF::LOOP | LBF::CASE | LBF::NON_FRAME;
  case LBK::BREAK:
    return LBF::HAS_TARGET | LBF::LOOP | LBF::CASE | LBF::NON_FRAME;
  case LBK::CONTINUE:
    return LBF::HAS_TARGET | LBF::LOOP;
  case LBK::FALLTHROUGH:
    return LBF::CASE;
  case LBK::GOTO:
    return LBF::HAS_TARGET | LBF::LOOP | LBF::CASE | LBF::NON_FRAME;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTarget(rq::LocalBreakKind kind) {
  rq::LocalBreakFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::LocalBreakFlags::HAS_TARGET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInLoop(rq::LocalBreakKind kind) {
  rq::LocalBreakFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::LocalBreakFlags::LOOP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInCase(rq::LocalBreakKind kind) {
  rq::LocalBreakFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::LocalBreakFlags::CASE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeInNonFrame(rq::LocalBreakKind kind) {
  rq::LocalBreakFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::LocalBreakFlags::NON_FRAME);
}

struct LocalBreak final {
  using Self = rq::LocalBreak;

  rq::LocalBreakKind _kind{rq::LocalBreakKind::NONE};
  rq::Entity *_target_ptr{nullptr};
  rq::Expression *_target_rvalue_ex_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalBreak() = default;
  explicit RQ_ALWAYS_INLINE LocalBreak(rq::LocalBreakKind kind) : _kind(kind) {
    RQ_ASSERT(!rq::getHasTarget(kind), "must have target");
  }
  explicit RQ_ALWAYS_INLINE LocalBreak(rq::LocalBreakKind kind,
                                       rq::Entity &target,
                                       rq::Expression &target_rvalue_ex)
      : _kind(kind), _target_ptr(&target),
        _target_rvalue_ex_ptr(&target_rvalue_ex) {
    RQ_ASSERT(rq::getHasTarget(kind), "break of kind does not have target");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocalBreakKind getKind() const {
    return this->_kind;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getTarget() const {
    return rq::dereferencePtr(this->_target_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getTarget() {
    return rq::dereferencePtr(this->_target_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getTargetRvalueEx() const {
    return rq::dereferencePtr(this->_target_rvalue_ex_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getTargetRvalueEx() {
    return rq::dereferencePtr(this->_target_rvalue_ex_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTarget() const {
    return this->_target_ptr != nullptr;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInLoop() const {
    return rq::getCanBeInLoop(this->getKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInCase() const {
    return rq::getCanBeInCase(this->getKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInNonFrame() const {
    return rq::getCanBeInNonFrame(this->getKind());
  }
};

struct DottedInstructionFactory final {
  using Self = rq::DottedInstructionFactory;

  rq::Context *_constext_ptr;
  rq::Opcode _opcode;
  rq::Entity *_outer_ptr{nullptr};
  rq::Instruction *_last_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE DottedInstructionFactory(rq::Context &context,
                                                     rq::Opcode opcode);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const {
    return this->_opcode;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_constext_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_constext_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getOuter() const {
    return rq::dereferencePtr(this->_outer_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getOuter() {
    return rq::dereferencePtr(this->_outer_ptr);
  }

  void append(rq::Entity &entity);
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
  [[nodiscard]] rq::Expression &evaluateLowFlags(rq::Module& module, rq::SymbolTable& table, rq::LowFlagsFactory &factory,
                                                 rq::Expression &asc_ex);
  [[nodiscard]] rq::LocalBreak evaluateStaticOrDynamicLocalStatements(
      rq::DottedInstructionFactory &inst_factory, rq::Module &module,
      rq::Function &function, rq::SymbolTable &table,
      rq::Expression &state0_ex);
  [[nodiscard]] rq::LocalBreak evaluateDynamicLocalStatement(
      rq::DottedInstructionFactory &inst_factory,
      rq::LowFlagsFactory &flags_factory, rq::Function &function,
      rq::ConstantSymbol &result_type, rq::SymbolTable &table,
      rq::Module &module, rq::Expression &state_ex);
  void evaluate(rq::Module &module);
  void evaluate(rq::ClassType &class_);
  void evaluate(rq::EnumerationType &enum_);
  void evaluate(rq::Interface &interface);
  void evaluate(rq::Adapter &adapter);
  void evaluate(rq::GlobalDynamicVariable &var);
  void evaluate(rq::GlobalStaticVariable &var);
  void evaluate(rq::Function &func);

  [[nodiscard]] rq::DynamicLvalue
  evaluateDynamicLvalue(rq::ConstantSymbol &result_type, rq::SymbolTable &table,
                        rq::Module &module, rq::Expression &lvalue_ex);

  [[nodiscard]] rq::StaticRvalue
  evaluateStaticRvalue(rq::Module &module, rq::SymbolTable &table,
                       rq::Expression &rvalue_ex);
  // etc

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicRvalue(rq::Module &module, rq::SymbolTable &table,
                        rq::Expression &rvalue_ex);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicArithmeticRvalue(rq::SymbolTable &table, rq::Module &module,
                                  rq::Expression &rvalue_ex, rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicLogicalRvalue(rq::SymbolTable &table, rq::Module &module,
                               rq::Expression &rvalue_ex, rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue evaluateDynamicOrderedComparisonRvalue(
      rq::SymbolTable &table, rq::Module &module, rq::Expression &rvalue_ex,
      rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue evaluateDynamicEquivalenceComparisonRvalue(
      rq::SymbolTable &table, rq::Module &module, rq::Expression &rvalue_ex,
      rq::Opcode opcode);

  [[nodiscard]] rq::Entity &foldDynamicRvalue(rq::Entity &rvalue,
                                              rq::Symbol &type);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicIdentifierRvalue(rq::SymbolTable &table, rq::Module &module,
                                  rq::Name name);

  [[nodiscard]] rq::Name evaluateName(rq::SymbolTable &table,
                                      rq::Module &module,
                                      rq::Expression &name_ex);

  [[nodiscard]] rq::Symbol *completeType(rq::Symbol &to_type,
                                         rq::Symbol &from_type);

  [[nodiscard]] rq::Symbol &deliteralizeType(rq::Symbol &type);
};
} // namespace rq
