#pragma once

namespace rq {

RQ_ALWAYS_INLINE Value::Value(rq::ValueKind kind, rq::QualifiedType &type)
    : Entity(rq::getId(kind)), _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &Value::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &Value::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Value::classof(const rq::Entity *entity_ptr) {
  return rq::getIsValueKind(entity_ptr->getId());
}

RQ_ALWAYS_INLINE Int::Int(rq::ValueKind kind, rq::QualifiedType &type,
                          const llvm::APInt &value)
    : Value(kind, type), _int(value) {}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &Int::getInt() const {
  return this->_int;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Int::classof(const rq::Entity *entity_ptr) {
  return ConstantInt::classof(entity_ptr) || MutableInt::classof(entity_ptr);
}

RQ_ALWAYS_INLINE ConstantInt::ConstantInt(rq::QualifiedType &type,
                                          const llvm::APInt &value)
    : Int(rq::ValueKind::CONSTANT_INT, type, value) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstantInt::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() == rq::getId(rq::ValueKind::CONSTANT_INT);
}

inline void ConstantInt::Profile(llvm::FoldingSetNodeID &id,
                                 const rq::QualifiedType &type,
                                 const llvm::APInt &value) {
  id.AddPointer(&type);
  value.Profile(id);
}

RQ_ALWAYS_INLINE void ConstantInt::Profile(llvm::FoldingSetNodeID &id) const {
  Self::Profile(id, this->getType(), this->getInt());
}

RQ_ALWAYS_INLINE MutableInt::MutableInt(rq::QualifiedType &type,
                                        const llvm::APInt &value)
    : Int(rq::ValueKind::MUTABLE_INT, type, value) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt &MutableInt::getInt() {
  return this->_int;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
MutableInt::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() == rq::getId(rq::ValueKind::MUTABLE_INT);
}

RQ_ALWAYS_INLINE Float::Float(rq::ValueKind kind, rq::QualifiedType &type,
                              const llvm::APFloat &value)
    : Value(kind, type), _float(value) {}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &Float::getFloat() const {
  return this->_float;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Float::classof(const rq::Entity *entity_ptr) {
  return ConstantFloat::classof(entity_ptr) ||
         MutableFloat::classof(entity_ptr);
}

RQ_ALWAYS_INLINE ConstantFloat::ConstantFloat(rq::QualifiedType &type,
                                              const llvm::APFloat &value)
    : Float(rq::ValueKind::CONSTANT_FLOAT, type, value) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstantFloat::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() == rq::getId(rq::ValueKind::CONSTANT_FLOAT);
}

inline void ConstantFloat::Profile(llvm::FoldingSetNodeID &id,
                                   const rq::QualifiedType &type,
                                   const llvm::APFloat &value) {
  id.AddPointer(&type);
  id.AddPointer(&value.getSemantics());
  value.Profile(id);
}

RQ_ALWAYS_INLINE void ConstantFloat::Profile(llvm::FoldingSetNodeID &id) const {
  Self::Profile(id, this->getType(), this->getFloat());
}

RQ_ALWAYS_INLINE MutableFloat::MutableFloat(rq::QualifiedType &type,
                                            const llvm::APFloat &value)
    : Float(rq::ValueKind::MUTABLE_FLOAT, type, value) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat &MutableFloat::getFloat() {
  return this->_float;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
MutableFloat::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() == rq::getId(rq::ValueKind::MUTABLE_FLOAT);
}

RQ_ALWAYS_INLINE DetachedArray::DetachedArray(rq::ValueKind kind,
                                              rq::QualifiedType &type)
    : Value(kind, type) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DetachedArray::classof(const rq::Entity *entity_ptr) {
  return ConstantDetachedArray::classof(entity_ptr) ||
         MutableDetachedArray::classof(entity_ptr);
}

RQ_ALWAYS_INLINE
MutableDetachedArray::MutableDetachedArray(rq::QualifiedType &type,
                                           llvm::ArrayRef<rq::Entity *> data)
    : DetachedArray(rq::ValueKind::MUTABLE_DETACHED_ARRAY, type),
      _data(data.begin(), data.end()) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Entity *>
MutableDetachedArray::getDataArray() const {
  return {this->_data.data(), this->_data.size()};
}

[[nodiscard]] RQ_ALWAYS_INLINE MutableDetachedArray::Data &
MutableDetachedArray::getDataArray() {
  return this->_data;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
MutableDetachedArray::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() ==
         rq::getId(rq::ValueKind::MUTABLE_DETACHED_ARRAY);
}

RQ_ALWAYS_INLINE ConstantDetachedArrayElement::ConstantDetachedArrayElement(
    rq::Entity &entity, rq::ConstantDetachedArrayElement *next_ptr)
    : _element_ptr(&entity), _next_ptr(next_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
ConstantDetachedArrayElement::getElement() const {
  return rq::dereferencePtr(this->_element_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
ConstantDetachedArrayElement::getElement() {
  return rq::dereferencePtr(this->_element_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArrayElement &
ConstantDetachedArrayElement::getNext() const {
  return rq::dereferencePtr(this->_next_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArrayElement &
ConstantDetachedArrayElement::getNext() {
  return rq::dereferencePtr(this->_next_ptr);
}

inline void ConstantDetachedArrayElement::Profile(
    llvm::FoldingSetNodeID &id, const rq::Entity &entity,
    const rq::ConstantDetachedArrayElement *next_ptr) {
  id.AddPointer(&entity);
  id.AddPointer(next_ptr);
}

RQ_ALWAYS_INLINE void
ConstantDetachedArrayElement::Profile(llvm::FoldingSetNodeID &id) const {
  Self::Profile(id, this->getElement(), this->_next_ptr);
}

RQ_ALWAYS_INLINE ConstantDetachedArray::ConstantDetachedArray(
    rq::QualifiedType &type, rq::ConstantDetachedArrayElement *first_ptr)
    : DetachedArray(rq::ValueKind::CONSTANT_DETACHED_ARRAY, type),
      _first_ptr(first_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstantDetachedArray::classof(const rq::Entity *entity_ptr) {
  return entity_ptr->getId() ==
         rq::getId(rq::ValueKind::CONSTANT_DETACHED_ARRAY);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::ConstantDetachedArrayElement>
    ConstantDetachedArray::getElementSubrange() {
  return {rq::NextIterator<rq::ConstantDetachedArrayElement>(this->_first_ptr),
          rq::NextIterator<rq::ConstantDetachedArrayElement>()};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::ConstantDetachedArrayElement>
    ConstantDetachedArray::getElementSubrange() const {
  return {
      rq::ConstNextIterator<rq::ConstantDetachedArrayElement>(this->_first_ptr),
      rq::ConstNextIterator<rq::ConstantDetachedArrayElement>()};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::ConstantDetachedArrayElement>
    ConstantDetachedArray::getConstElementSubrange() const {
  return this->getElementSubrange();
}

inline void ConstantDetachedArray::Profile(
    llvm::FoldingSetNodeID &id, const rq::QualifiedType &type,
    const rq::ConstantDetachedArrayElement *first_ptr) {
  id.AddPointer(&type);
  id.AddPointer(first_ptr);
}

RQ_ALWAYS_INLINE void
ConstantDetachedArray::Profile(llvm::FoldingSetNodeID &id) const {
  Self::Profile(id, this->getType(), this->_first_ptr);
}

} // namespace rq
