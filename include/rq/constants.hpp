#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>

namespace rq {

enum class ConstantKind {

};

struct Constant {
  using Self = rq::Constant;

  rq::ConstantKind _kind;

  explicit RQ_ALWAYS_INLINE Constant(rq::ConstantKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;

  void *_symbol_constant_or_next_free_ptr;
  unsigned _ref_count{0};
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

  void *_symbol_constant_or_next_free_ptr;
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

  bool _is_platform_specific : 1;
  bool _data : 1;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getData() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;

  bool _is_platform_specific : 1;
  llvm::StringRef _data;

  explicit RQ_ALWAYS_INLINE StringConstant(llvm::StringRef data,
                                           bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getData() const;

  [[nodiscard]] static inline bool classof(const rq::Constant *constant_ptr);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  llvm::ArrayRef<rq::Constant> _data;

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