#pragma once

#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>

#include <algorithm>
#include <bit>
#include <concepts>
#include <type_traits>

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

inline void profileSymbolConstant(llvm::FoldingSetNodeID &out_id,
                                  const rq::Symbol &symbol,
                                  rq::TypeFlags flags) {
  out_id.AddPointer(&symbol);
  out_id.AddInteger(rq::getUnderlying(flags));
}

struct SymbolConstant final : public rq::Constant, public llvm::FoldingSetNode {
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

  inline void Profile(llvm::FoldingSetNodeID &out_id) const {
    rq::profileSymbolConstant(out_id, this->getSymbol(), this->getFlags());
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

inline void profileWordConstant(llvm::FoldingSetNodeID &out_id,
                                const llvm::APInt &value) {
  value.Profile(out_id);
}

template <typename From, typename To>
concept BitCastable = requires(From value) { std::bit_cast<To>(value); };

struct WordConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::WordConstant;

  const llvm::APInt _value;

  explicit RQ_ALWAYS_INLINE WordConstant(const llvm::APInt &value)
      : Constant(rq::ConstantKind::WORD), _value(value) {
    RQ_ASSERT(rq::getIsCanonicalized(value), "value not canonicalized");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getValue() const {
    return this->_value;
  }

  template <typename TypeParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam getAs() const {
    using Type = TypeParam;
    static_assert(!std::same_as<Type, float> && !std::same_as<Type, double> &&
                      !std::same_as<Type, long double>,
                  "platform float type");
    RQ_ASSERT(this->_value.getBitWidth() <= sizeof(Type), "too big");
    Type value;
    if constexpr (sizeof(Type) <= sizeof(std::uint64_t) &&
                  std::unsigned_integral<Type>) {
      value = static_cast<Type>(this->_value.getZExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::int64_t) &&
                         std::integral<Type>) {
      value = static_cast<Type>(this->_value.getSExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::uint64_t) &&
                         std::is_enum_v<Type> &&
                         std::unsigned_integral<std::underlying_type_t<Type>>) {
      value = static_cast<Type>(this->_value.getZExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::int64_t) &&
                         std::is_enum_v<Type> &&
                         std::integral<std::underlying_type_t<Type>>) {
      value = static_cast<Type>(this->_value.getSExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::uint64_t) &&
                         rq::BitCastable<std::uint64_t, Type>) {
      value = std::bit_cast<Type>(this->_value.getZExtValue());
    } else {
      std::memset(&value, 0, sizeof(Type));
      std::memcpy(&value, this->_value.getRawData(), sizeof(Type));
    }
    return value;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::WORD);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) const {
    rq::profileWordConstant(out_id, this->_value);
  }
};

inline void profileArrayConstant(llvm::FoldingSetNodeID &out_id,
                                 llvm::ArrayRef<rq::Constant *> data) {
  for (const rq::Constant *constant_ptr : data) {
    out_id.AddPointer(constant_ptr);
  }
}

struct ArrayConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ArrayConstant;

  std::vector<rq::Constant *> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant *> data)
      : Constant(rq::ConstantKind::ARRAY), _data(data) {}

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant *>
  getData() const {
    return this->_data;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::ARRAY);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) const {
    rq::profileArrayConstant(out_id, this->_data);
  }
};

} // namespace rq