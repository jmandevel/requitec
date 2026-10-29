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

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct TombstoneValue final : public rq::Value {
  using Self = rq::Value;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct Int : public rq::Value {
  using Self = rq::Value;

  llvm::APInt _int;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct ConstantInt final : public rq::Int {
  using Self = rq::ConstantInt;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct MutableInt final : public rq::Int {
  using Self = rq::MutableInt;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct Float : public rq::Value {
  using Self = rq::Value;

  llvm::APFloat _float;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct ConstantFloat final : public rq::Float {
  using Self = rq::ConstantFloat;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

struct MutableFloat final : public rq::Float {
  using Self = rq::MutableFloat;

  [[nodiscard]] RQ_ALWAYS_INLINE bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq