#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::Opcode opcode) {
  using O = rq::Opcode;
  switch (opcode) {
  case O::NONE:
    return "NONE";

  case O::DEBUG_STEP:
    return "DEBUG_STEP";
  case O::SOURCE_RANGE:
    return "SOURCE_RANGE";

  case O::STATEMENT:
    return "STATEMENT";
  case O::ASSIGN:
    return "ASSIGN";

  // ARITHMETIC
  case O::ADD:
    return "ADD";
  case O::SUBTRACT:
    return "SUBTRACT";
  case O::MULTIPLY:
    return "MULTIPLY";
  case O::MODULUS:
    return "MODULUS";
  case O::NEGATE:
    return "NEGATE";

  default:
    break;
  }
  RQ_UNREACHABLE();
}

struct Expression;

struct Instruction final : public rq::Entity {
  using Self = rq::Instruction;

  rq::Entity *_address0_ptr{nullptr};
  rq::Entity *_address1_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode)
      : Entity(rq::getUnderlying(opcode) + rq::OPCODE_OFFSET) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const {
    return static_cast<rq::Opcode>(this->getId() - rq::OPCODE_OFFSET);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress0() const {
    return this->_address0_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress1() const {
    return this->_address1_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0() {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress1() const {
    return rq::dereferencePtr(this->_address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress1() {
    return rq::dereferencePtr(this->_address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getAddress0Ptr() const {
    return this->_address0_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getAddress0Ptr() {
    return this->_address0_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getAddress1Ptr() const {
    return this->_address1_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getAddress1Ptr() {
    return this->_address1_ptr;
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity &address0) {
    rq::assignSingleValue(this->_address0_ptr, &address0);
  }
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity &address1) {
    rq::assignSingleValue(this->_address1_ptr, &address1);
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity *address0_ptr) {
    rq::assignSingleValue(this->_address0_ptr, address0_ptr);
  }
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity *address1_ptr) {
    rq::assignSingleValue(this->_address1_ptr, address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity &address0) {
    return rq::replaceValue(this->_address0_ptr, &address0);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity &address1) {
    return rq::replaceValue(this->_address1_ptr, &address1);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity *address0_ptr) {
    return rq::replaceValue(this->_address0_ptr, address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity *address1_ptr) {
    return rq::replaceValue(this->_address1_ptr, address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *
  replaceAddress0Ptr(rq::Entity *address0_ptr) {
    return rq::replaceValuePtr(this->_address0_ptr, address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *
  replaceAddress1Ptr(rq::Entity *address1_ptr) {
    return rq::replaceValuePtr(this->_address1_ptr, address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress0() {
    return rq::popValue(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress1() {
    return rq::popValue(this->_address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress0Ptr() {
    return rq::popValuePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress1Ptr() {
    return rq::popValuePtr(this->_address1_ptr);
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::OPCODE_OFFSET;
  }
};

RQ_ALWAYS_INLINE
DottedInstructionIterator::DottedInstructionIterator(rq::Entity *entity_ptr,
                                                     rq::Opcode opcode)
    : _entity_ptr(entity_ptr), _opcode(opcode) {}

RQ_ALWAYS_INLINE rq::DottedInstructionIterator &
DottedInstructionIterator::operator++() {
  rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      this->_entity_ptr = inst.getAddress1Ptr();
      return *this;
    }
  }
  this->_entity_ptr = nullptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::DottedInstructionIterator
DottedInstructionIterator::operator++(int) {
  rq::DottedInstructionIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DottedInstructionIterator::operator==(const Self &it) const {
  return this->_entity_ptr == it._entity_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DottedInstructionIterator::operator!=(const Self &it) const {
  return this->_entity_ptr != it._entity_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
DottedInstructionIterator::operator*() {
  rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0();
    }
  }
  return entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
DottedInstructionIterator::operator*() const {
  const rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0();
    }
  }
  return entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *
DottedInstructionIterator::operator->() {
  if (this->_entity_ptr == nullptr) {
    return nullptr;
  }
  rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0Ptr();
    }
  }
  return &entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
DottedInstructionIterator::operator->() const {
  if (this->_entity_ptr == nullptr) {
    return nullptr;
  }
  const rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0Ptr();
    }
  }
  return &entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DottedInstructionIterator::getIsDone() const {
  return this->_entity_ptr == nullptr;
}

RQ_ALWAYS_INLINE ConstDottedInstructionIterator::ConstDottedInstructionIterator(
    const rq::Entity *entity_ptr, rq::Opcode opcode)
    : _entity_ptr(entity_ptr), _opcode(opcode) {}

RQ_ALWAYS_INLINE rq::ConstDottedInstructionIterator &
ConstDottedInstructionIterator::operator++() {
  const rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      this->_entity_ptr = inst.getAddress1Ptr();
      return *this;
    }
  }
  this->_entity_ptr = nullptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstDottedInstructionIterator
ConstDottedInstructionIterator::operator++(int) {
  rq::ConstDottedInstructionIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstDottedInstructionIterator::operator==(const Self &it) const {
  return this->_entity_ptr == it._entity_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstDottedInstructionIterator::operator!=(const Self &it) const {
  return this->_entity_ptr != it._entity_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
ConstDottedInstructionIterator::operator*() const {
  const rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0();
    }
  }
  return entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
ConstDottedInstructionIterator::operator->() const {
  if (this->_entity_ptr == nullptr) {
    return nullptr;
  }
  const rq::Entity &entity = rq::dereferencePtr(this->_entity_ptr);
  if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &inst = llvm::cast<rq::Instruction>(entity);
    if (inst.getOpcode() == this->_opcode) {
      return inst.getAddress0Ptr();
    }
  }
  return &entity;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstDottedInstructionIterator::getIsDone() const {
  return this->_entity_ptr == nullptr;
}

} // namespace rq