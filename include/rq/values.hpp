#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>

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

  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getInt() const;

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

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt &getInt();

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

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat &getFloat();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DetachedArray : public rq::Value {
  using Self = rq::DetachedArray;

  RQ_ALWAYS_INLINE DetachedArray();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MutableDetachedArray final : public rq::DetachedArray {
  using Self = rq::MutableDetachedArray;

  std::vector<rq::Entity *> _data;

  RQ_ALWAYS_INLINE MutableDetachedArray();

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Entity *>
  getDataArray() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Entity *> &getDataArray();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstantDetachedArrayElement final : public llvm::FoldingSetNode {
  using Self = rq::ConstantDetachedArrayElement;

  rq::Entity *_element_ptr;
  rq::ConstantDetachedArrayElement *_next_ptr;

  RQ_ALWAYS_INLINE
  ConstantDetachedArrayElement(rq::Entity &entity,
                               rq::ConstantDetachedArrayElement *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getElement() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getElement();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArrayElement &
  getNext() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArrayElement &getNext();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id);
};

inline void
profileConstantDetachedArrayElement(llvm::FoldingSetNodeID &id,
                                    const rq::Entity &entity,
                                    const rq::ConstantDetachedArray *next_ptr);

struct ConstantDetachedArray final : public rq::DetachedArray,
                                     public llvm::FoldingSetNode {
  using Self = rq::ConstantDetachedArray;

  rq::ConstantDetachedArrayElement *_first_ptr;

  RQ_ALWAYS_INLINE ConstantDetachedArray(rq::ConstantDetachedArray *first_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::ConstantDetachedArrayElement>
      getElementSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE const
      rq::NextSubrange<rq::ConstantDetachedArrayElement>
      getElementSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const
      rq::ConstNextSubrange<rq::ConstantDetachedArrayElement>
      getConstElementSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id);
};

inline void
profileConstantDetachedArray(llvm::FoldingSetNodeID &id,
                             const rq::ConstantDetachedArrayElement *first_ptr);

} // namespace rq

#include <rq/detail/values.hpp>