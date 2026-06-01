#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>

#include <algorithm>

namespace rq {

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit RQ_ALWAYS_INLINE Constant(rq::ConstantKind kind)
      : Entity(rq::getUnderlying(kind) + rq::CONSTANT_OFFSET) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantKind getConstantKind() const {
    return static_cast<rq::ConstantKind>(this->_id - rq::CONSTANT_OFFSET);
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::CONSTANT_OFFSET && id < rq::OPCODE_OFFSET;
  }
};

struct Symbol;
enum class TypeFlags : std::uint_fast8_t;

struct SymbolConstant final : public rq::Constant {
  using Self = rq::SymbolConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  explicit RQ_ALWAYS_INLINE SymbolConstant(rq::Symbol &symbol,
                                           rq::TypeFlags flags)
      : Constant(rq::ConstantKind::SYMBOL), _symbol_ptr(&symbol),
        _type_flags(flags) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const {
    return this->_type_flags;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::SYMBOL);
  }
};

[[nodiscard]] inline llvm::APInt canonicalize(const llvm::APInt &value) {
  if (value.isZero()) {
    return llvm::APInt(1, 0);
  }
  return value.zextOrTrunc(value.getActiveBits());
}

[[nodiscard]] inline bool getIsCanonicalized(const llvm::APInt &value) {
  const unsigned canonical_width = std::max(1u, value.getActiveBits());
  return value.getBitWidth() == canonical_width;
}

struct WordConstant final : public rq::Constant {
  using Self = rq::WordConstant;

  const llvm::APInt _value;

  explicit RQ_ALWAYS_INLINE WordConstant(const llvm::APInt &value)
      : Constant(rq::ConstantKind::WORD), _value(value) {
    RQ_ASSERT(rq::getIsCanonicalized(value), "value not canonicalized");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getValue() const {
    return this->_value;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::WORD);
  }
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  std::vector<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data)
      : Constant(rq::ConstantKind::ARRAY), _data(data) {}

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant> getData() const {
    return this->_data;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::ARRAY);
  }
};

} // namespace rq