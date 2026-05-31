#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>

namespace rq {

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  unsigned _ref_count{0};

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

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;

  const llvm::APInt _data;

  explicit RQ_ALWAYS_INLINE IntegerConstant(rq::SymbolConstant &symbol,
                                            llvm::APInt &&value);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getValue() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

inline void profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                   const llvm::APInt &data);

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;

  const llvm::APFloat _data;

  explicit RQ_ALWAYS_INLINE FloatConstant(rq::SymbolConstant &symbol,
                                          llvm::APFloat &&value);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getValue() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct Symbol;
enum class TypeFlags : std::uint_fast8_t;

struct SymbolConstant final : public rq::Constant {
  using Self = rq::SymbolConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  explicit RQ_ALWAYS_INLINE SymbolConstant(rq::Symbol &symbol,
                                           rq::TypeFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _data : 1;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getData() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  std::vector<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data,
                                          bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant> getData() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

enum class ExpressionAttribute : std::uint_fast8_t;

struct ExpressionAttributeConstant final : public rq::Constant {
  using Self = rq::ExpressionAttributeConstant;

  rq::ExpressionAttribute _attribute;

  explicit RQ_ALWAYS_INLINE
  ExpressionAttributeConstant(rq::ExpressionAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

enum class TypeAttribute : std::uint_fast8_t;

struct TypeAttributeConstant final : public rq::Constant {
  using Self = rq::TypeAttributeConstant;

  rq::TypeAttribute _attribute;

  explicit RQ_ALWAYS_INLINE TypeAttributeConstant(rq::TypeAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

} // namespace rq