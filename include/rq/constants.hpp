#pragma once

#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
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

inline void profileConstantWord(llvm::FoldingSetNodeID &out_id,
                                const llvm::APInt &word) {
  word.Profile(out_id);
}

struct ConstantWord final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ConstantWord;

  llvm::APInt _word;

  explicit RQ_ALWAYS_INLINE ConstantWord(llvm::APInt word)
      : Constant(rq::ConstantKind::WORD), _word(word) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getWord() const {
    return this->_word;
  }

  template <typename TypeParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam getAsInt() const {
    using Type = TypeParam;
    static_assert(!std::same_as<Type, float> && !std::same_as<Type, double> &&
                      !std::same_as<Type, long double>,
                  "platform float type");
    RQ_ASSERT(this->_word.getBitWidth() <= sizeof(Type), "too big");
    Type value;
    if constexpr (sizeof(Type) <= sizeof(std::uint64_t) &&
                  std::unsigned_integral<Type>) {
      value = static_cast<Type>(this->_word.getZExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::int64_t) &&
                         std::integral<Type>) {
      value = static_cast<Type>(this->_word.getSExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::uint64_t) &&
                         std::is_enum_v<Type> &&
                         std::unsigned_integral<std::underlying_type_t<Type>>) {
      value = static_cast<Type>(this->_word.getZExtValue());
    } else if constexpr (sizeof(Type) <= sizeof(std::int64_t) &&
                         std::is_enum_v<Type> &&
                         std::integral<std::underlying_type_t<Type>>) {
      value = static_cast<Type>(this->_word.getSExtValue());
    } else {
      static_assert(false, "not supported");
    }
    return value;
  }

  template <typename TypeParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam
  getAsFloat(const llvm::fltSemantics &semantics) {
    llvm::APFloat float_(semantics, this->_word);
    return float_;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::WORD);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) const {
    rq::profileConstantWord(out_id, this->_word);
  }
};

inline void profileConstantArray(llvm::FoldingSetNodeID &out_id,
                                 llvm::ArrayRef<rq::Constant *> array) {
  out_id.AddInteger(array.size());
  for (const rq::Constant *constant_ptr : array) {
    out_id.AddPointer(constant_ptr);
  }
}

struct ConstantArray final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ConstantArray;

  std::vector<rq::Constant *> _array;

  explicit RQ_ALWAYS_INLINE ConstantArray(llvm::ArrayRef<rq::Constant *> array)
      : Constant(rq::ConstantKind::ARRAY), _array(array) {}

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant *>
  getArray() const {
    return this->_array;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::ARRAY);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) {
    rq::profileConstantArray(out_id, this->getArray());
  }
};

inline void profileConstantDataArray(llvm::FoldingSetNodeID &out_id,
                                     llvm::ArrayRef<std::byte> data_array) {
  out_id.AddInteger(data_array.size());
  for (std::byte byte : data_array) {
    out_id.AddInteger(static_cast<std::uint8_t>(byte));
  }
}

struct ConstantDataArray final : public rq::Constant,
                                 public llvm::FoldingSetNode {
  using Self = rq::ConstantDataArray;

  std::vector<std::byte> _data_array;

  explicit RQ_ALWAYS_INLINE
  ConstantDataArray(llvm::ArrayRef<std::byte> data_array)
      : Constant(rq::ConstantKind::DATA_ARRAY), _data_array(data_array) {}

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<std::byte>
  getDataArray() const {
    return this->_data_array;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id == rq::CONSTANT_OFFSET +
                     rq::getUnderlying(rq::ConstantKind::DATA_ARRAY);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) {
    rq::profileConstantDataArray(out_id, this->getDataArray());
  }
};

inline void profileConstantSymbol(llvm::FoldingSetNodeID &out_id,
                                  rq::TypeFlags flags,
                                  const rq::Symbol &symbol) {
  out_id.AddInteger(rq::getUnderlying(flags));
  out_id.AddPointer(&symbol);
}

struct ConstantSymbol final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ConstantSymbol;

  rq::TypeFlags _flags;
  rq::Symbol *_symbol_ptr;

  explicit RQ_ALWAYS_INLINE ConstantSymbol(rq::TypeFlags flags,
                                           rq::Symbol &symbol)
      : Constant(rq::ConstantKind::SYMBOL), _flags(flags),
        _symbol_ptr(&symbol) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const {
    return this->_flags;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id ==
           rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::SYMBOL);
  }

  inline void Profile(llvm::FoldingSetNodeID &out_id) {
    rq::profileConstantSymbol(out_id, this->getFlags(), this->getSymbol());
  }
};

} // namespace rq