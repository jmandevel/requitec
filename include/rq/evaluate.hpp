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
struct LowFactory;

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

enum class JumpKind : std::uint_fast8_t {
  NONE,
  DYNAMIC_RETURN,
  DYNAMIC_BREAK,
  DYNAMIC_CONTINUE,
  DYNAMIC_FALLTHROUGH,
  STATIC_BREAK,
  STATIC_CONTINUE,
  STATIC_FALLTHROUGH
};

enum class JumpInfoFlags : std::uint_fast8_t {
  NONE = 0,

  IS_DYNAMIC = rq::getBit(0),
  IS_STATIC = rq::getBit(1) HAS_TARGET = rq::getBit(2),
  LOOP = rq::getBit(3),
  CASE = rq::getBit(4),
  NON_FRAME = rq::getBit(5)
};

template <> struct is_flags<rq::JumpInfoFlags> final : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE rq::JumpInfoFlags
getInfoFlags(rq::JumpKind kind) {
  using J = rq::JumpKind;
  using JF = rq::JumpInfoFlags;
  switch (kind) {
  case J::NONE:
    return JF::NONE;
  case J::DYNAMIC_RETURN:
    return JF::IS_DYNAMIC | JF::LOOP | JF::CASE | JF::NON_FRAME;
  case J::DYNAMIC_BREAK:
    return JF::IS_DYNAMIC | JF::HAS_TARGET | JF::LOOP | JF::CASE |
           JF::NON_FRAME;
  case J::DYNAMIC_CONTINUE:
    return JF::IS_DYNAMIC | JF::HAS_TARGET | JF::LOOP;
  case J::DYNAMIC_FALLTHROUGH:
    return JF::IS_DYNAMIC | JF::CASE;
  case J::STATIC_BREAK:
    return JF::IS_STATIC | JF::HAS_TARGET | JF::LOOP | JF::CASE | JF::NON_FRAME;
  case J::STATIC_CONTINUE:
    return JF::IS_STATIC | JF::HAS_TARGET | JF::LOOP | JF::CASE | JF::NON_FRAME;
  case J::STATIC_FALLTHROUGH:
    return JF::IS_STATIC | JF::HAS_TARGET | JF::LOOP | JF::CASE | JF::NON_FRAME;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTarget(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::HAS_TARGET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamic(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::IS_DYNAMIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsStatic(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::IS_STATIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInLoop(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::LOOP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInCase(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::CASE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeInNonFrame(rq::JumpKind kind) {
  rq::JumpInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::JumpInfoFlags::NON_FRAME);
}

struct Jump final {
  using Self = rq::Jump;

  rq::JumpKind _kind{rq::JumpKind::NONE};
  rq::Entity *_target_ptr{nullptr};
  rq::Expression *_target_rvalue_ex_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Jump() = default;
  explicit RQ_ALWAYS_INLINE Jump(rq::JumpKind kind) : _kind(kind) {
    RQ_ASSERT(!rq::getHasTarget(kind), "must have target");
  }
  explicit RQ_ALWAYS_INLINE Jump(rq::JumpKind kind, rq::Entity &target,
                                 rq::Expression &target_rvalue_ex)
      : _kind(kind), _target_ptr(&target),
        _target_rvalue_ex_ptr(&target_rvalue_ex) {
    RQ_ASSERT(rq::getHasTarget(kind), "break of kind does not have target");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_kind == rq::JumpKind::NONE;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::JumpKind getKind() const {
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

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamic() const {
    return rq::getIsDynamic(this->getKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStatic() const {
    return rq::getIsStatic(this->getKind());
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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getOuterPtr() const {
    return this->_outer_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getOuterPtr() {
    return this->_outer_ptr;
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
  void surveyGlobalScope(rq::Module &module, rq::SymbolTable &host,
                         rq::Expression &first_ex);
  [[nodiscard]] rq::Expression &
  evaluateLowFuseFlags(rq::Module &module, rq::SymbolTable &host,
                       rq::LowFactory &low_factory, rq::Expression &asc_ex);
  void surveyAllSymbols(rq::Module &module);
  void implementAllSymbols(rq::Module &module);
  void surveyFunction(rq::Module &module, rq::SymbolTable &container,
                      rq::SymbolTable &host, rq::LowFactory &low_factory,
                      rq::Expression &ex);
  void declareFunction(rq::Function &function);
  void implementFunction(rq::Function &function);

  void destroyAllLocalVariables(rq::SymbolTable &table,
                                rq::DottedInstructionFactory &dot_factory);

  [[nodiscard]] rq::Jump
  evaluateStaticLocalStatement(rq::Module &module, rq::SymbolTable &host,
                               rq::Expression &unascribed_ex,
                               rq::LowFactory &low_factory,
                               rq::DottedInstructionFactory *dot_factory_ptr);
  [[nodiscard]] rq::Jump evaluateDynamicLocalStatement(
      rq::Module &module, rq::SymbolTable &host, rq::Expression &unascribed_ex,
      rq::LowFactory &low_factory, rq::DottedInstructionFactory &dot_factory);

  [[nodiscard]] rq::DynamicLvalue
  evaluateDynamicLvalue(rq::ConstantSymbol &result_type, rq::SymbolTable &host,
                        rq::Module &module, rq::Expression &lvalue_ex);

  [[nodiscard]] rq::StaticRvalue
  evaluateStaticRvalue(rq::Module &module, rq::SymbolTable &host,
                       rq::Expression &rvalue_ex);
  // etc

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicRvalue(rq::Module &module, rq::SymbolTable &host,
                        rq::Expression &rvalue_ex);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicArithmeticRvalue(rq::SymbolTable &host, rq::Module &module,
                                  rq::Expression &rvalue_ex, rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicLogicalRvalue(rq::SymbolTable &host, rq::Module &module,
                               rq::Expression &rvalue_ex, rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue evaluateDynamicOrderedComparisonRvalue(
      rq::SymbolTable &host, rq::Module &module, rq::Expression &rvalue_ex,
      rq::Opcode opcode);

  [[nodiscard]] rq::DynamicRvalue evaluateDynamicEquivalenceComparisonRvalue(
      rq::SymbolTable &host, rq::Module &module, rq::Expression &rvalue_ex,
      rq::Opcode opcode);

  [[nodiscard]] rq::Entity &foldDynamicRvalue(rq::Entity &rvalue,
                                              rq::Symbol &type);

  [[nodiscard]] rq::DynamicRvalue
  evaluateDynamicIdentifierRvalue(rq::SymbolTable &host, rq::Module &module,
                                  rq::Name name);

  [[nodiscard]] rq::Name evaluateName(rq::SymbolTable &host, rq::Module &module,
                                      rq::Expression &name_ex);

  [[nodiscard]] rq::SymbolTable &evaluateContainer(rq::Module &module,
                                                   rq::SymbolTable &host,
                                                   rq::LowFactory &low_factory);

  [[nodiscard]] rq::Symbol *completeType(rq::Symbol &to_type,
                                         rq::Symbol &from_type);

  [[nodiscard]] rq::Symbol &deliteralizeType(rq::Symbol &type);

  [[nodiscard]] bool validateAttributes(rq::SymbolTable &container,
                                        rq::Expression &unascribed_ex,
                                        rq::LowFactory &low_factory,
                                        rq::LowFuseFlags flags);
};
} // namespace rq
