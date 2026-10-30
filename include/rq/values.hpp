#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>

namespace rq {

struct QualifiedType;

struct Value : public rq::Entity {
  using Self = rq::Value;

  rq::QualifiedType *_type_ptr;

  RQ_ALWAYS_INLINE Value(rq::ValueKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Int : public rq::Value {
  using Self = rq::Value;

  llvm::APInt _int;

  RQ_ALWAYS_INLINE Int(rq::ValueKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt& getInt() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstantInt final : public rq::Int {
  using Self = rq::ConstantInt;

  RQ_ALWAYS_INLINE ConstantInt();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MutableInt final : public rq::Int {
  using Self = rq::MutableInt;

  RQ_ALWAYS_INLINE MutableInt();

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt& getInt();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Float : public rq::Value {
  using Self = rq::Value;

  llvm::APFloat _float;

  RQ_ALWAYS_INLINE Float(rq::ValueKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getFloat() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstantFloat final : public rq::Float {
  using Self = rq::ConstantFloat;

  RQ_ALWAYS_INLINE ConstantFloat();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MutableFloat final : public rq::Float {
  using Self = rq::MutableFloat;

  RQ_ALWAYS_INLINE MutableFloat();

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat& getFloat();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/values.hpp>