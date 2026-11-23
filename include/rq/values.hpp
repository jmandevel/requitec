#pragma once

#include <rq/entity.hpp>
#include <rq/iterators.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/FoldingSet.h>

#include <vector>

namespace rq {

struct QualifiedType;
struct ConstantDetachedArrayElement;

struct Value : public rq::Entity {
  using Self = rq::Value;
  rq::QualifiedType *_type_ptr;

  explicit RQ_ALWAYS_INLINE Value(rq::ValueKind kind, rq::QualifiedType &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getType();
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Int : public rq::Value {
  using Self = rq::Int;
  llvm::APInt _int;

  explicit RQ_ALWAYS_INLINE Int(rq::ValueKind kind, rq::QualifiedType &type,
                                const llvm::APInt &value);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getInt() const;
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConstantInt final : public rq::Int, public llvm::FoldingSetNode {
  using Self = rq::ConstantInt;

  explicit RQ_ALWAYS_INLINE ConstantInt(rq::QualifiedType &type,
                                        const llvm::APInt &value);
  static inline void Profile(llvm::FoldingSetNodeID &id,
                             const rq::QualifiedType &type,
                             const llvm::APInt &value);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct MutableInt final : public rq::Int {
  using Self = rq::MutableInt;

  explicit RQ_ALWAYS_INLINE MutableInt(rq::QualifiedType &type,
                                       const llvm::APInt &value);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt &getInt();
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Float : public rq::Value {
  using Self = rq::Float;
  llvm::APFloat _float;

  explicit RQ_ALWAYS_INLINE Float(rq::ValueKind kind, rq::QualifiedType &type,
                                  const llvm::APFloat &value);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getFloat() const;
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConstantFloat final : public rq::Float, public llvm::FoldingSetNode {
  using Self = rq::ConstantFloat;

  explicit RQ_ALWAYS_INLINE ConstantFloat(rq::QualifiedType &type,
                                          const llvm::APFloat &value);
  static inline void Profile(llvm::FoldingSetNodeID &id,
                             const rq::QualifiedType &type,
                             const llvm::APFloat &value);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct MutableFloat final : public rq::Float {
  using Self = rq::MutableFloat;

  explicit RQ_ALWAYS_INLINE MutableFloat(rq::QualifiedType &type,
                                         const llvm::APFloat &value);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat &getFloat();
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct String : public rq::Value {
  using Self = rq::String;

  explicit RQ_ALWAYS_INLINE String(rq::QualifiedType &type);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct MutableString final : public rq::String {
  using Self = rq::MutableString;

  std::string _text{};

  explicit RQ_ALWAYS_INLINE MutableString(rq::QualifiedType &type);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::string &getText();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConstantString final : public rq::String, public llvm::FoldingSetNode {
  using Self = rq::ConstantString;

  llvm::StringRef _text;

  explicit RQ_ALWAYS_INLINE ConstantString(rq::QualifiedType &type,
                                           llvm::StringRef value);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getText() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &id,
                             const rq::QualifiedType &type,
                             llvm::StringRef value);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id);
};

struct Bool : public rq::Value {
  using Self = rq::Bool;

  bool _value;

  explicit RQ_ALWAYS_INLINE Bool(rq::QualifiedType &type, bool value);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getBool() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct MutableBool final : public rq::Bool {
  using Self = rq::MutableBool;

  explicit RQ_ALWAYS_INLINE MutableBool(rq::QualifiedType &type, bool value);

  RQ_ALWAYS_INLINE void setBool(bool value);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConstantBool final : public rq::Bool, public llvm::FoldingSetNode {
  using Self = rq::ConstantBool;

  explicit RQ_ALWAYS_INLINE ConstantBool(rq::QualifiedType &type, bool value);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &id, bool value);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct DetachedArray : public rq::Value {
  using Self = rq::DetachedArray;

  explicit RQ_ALWAYS_INLINE DetachedArray(rq::ValueKind kind,
                                          rq::QualifiedType &type);
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct MutableDetachedArray final : public rq::DetachedArray {
  using Self = rq::MutableDetachedArray;

  std::vector<rq::Entity *> _data;

  explicit RQ_ALWAYS_INLINE MutableDetachedArray(rq::QualifiedType &type);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Entity *>
  getDataArray() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Entity *> &getDataArray();
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConstantDetachedArrayElement final : public llvm::FoldingSetNode {
  using Self = rq::ConstantDetachedArrayElement;
  rq::Entity *_element_ptr;
  rq::ConstantDetachedArrayElement *_next_ptr;

  explicit RQ_ALWAYS_INLINE
  ConstantDetachedArrayElement(rq::Entity &entity,
                               rq::ConstantDetachedArrayElement *next_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getElement() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getElement();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArrayElement &
  getNext() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArrayElement &getNext();
  static inline void Profile(llvm::FoldingSetNodeID &id,
                             const rq::Entity &entity,
                             const rq::ConstantDetachedArrayElement *next_ptr);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct ConstantDetachedArray final : public rq::DetachedArray,
                                     public llvm::FoldingSetNode {
  using Self = rq::ConstantDetachedArray;
  rq::ConstantDetachedArrayElement *_first_ptr;

  explicit RQ_ALWAYS_INLINE
  ConstantDetachedArray(rq::QualifiedType &type,
                        rq::ConstantDetachedArrayElement *first_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::ConstantDetachedArrayElement>
      getElementSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::ConstantDetachedArrayElement>
      getElementSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::ConstantDetachedArrayElement>
      getConstElementSubrange() const;
  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
  static inline void Profile(llvm::FoldingSetNodeID &id,
                             const rq::QualifiedType &type,
                             const rq::ConstantDetachedArrayElement *first_ptr);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

} // namespace rq

#include <rq/detail/values.hpp>
