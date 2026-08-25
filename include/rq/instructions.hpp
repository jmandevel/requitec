#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

#include <cstdint>
#include <tuple>

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::Opcode opcode) {
  std::ignore = opcode;
  RQ_TODO_IMPLEMENTATION();
}

enum class OpcodeInfoFlags : std::uint_fast8_t {
  NONE,

  ZERO_ADDRESS = rq::getBit(0),
  ONE_ADDRESS = rq::getBit(1),
  TWO_ADDRESS = rq::getBit(2)
};

RQ_DEFINE_FLAGS(rq::OpcodeInfoFlags);

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeInfoFlags
getInfoFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OIF = rq::OpcodeInfoFlags;
  switch (opcode) {
  case O::STATEMENT:
    return OIF::TWO_ADDRESS;
  case O::ASSIGN:
    return OIF::TWO_ADDRESS;
  case O::REF:
    return OIF::ONE_ADDRESS;
  case O::CONDITIONAL_JUMP:
    return OIF::TWO_ADDRESS;
  case O::UNLIKELY_CONDITIONAL_JUMP:
    return OIF::TWO_ADDRESS;
  case O::LIKELY_CONDITIONAL_JUMP:
    return OIF::TWO_ADDRESS;
  case O::JUMP:
    return OIF::ONE_ADDRESS;
  case O::LOGICAL_AND:
    return OIF::TWO_ADDRESS;
  case O::LOGICAL_OR:
    return OIF::TWO_ADDRESS;
  case O::LOGICAL_AND_WITH_SHORTCIRCUIT:
    return OIF::TWO_ADDRESS;
  case O::LOGICAL_OR_WITH_SHORTCIRCUIT:
    return OIF::TWO_ADDRESS;
  case O::LOGICAL_COMPLEMENT:
    return OIF::ONE_ADDRESS;
  case O::RVALUE_PAIR:
    return OIF::TWO_ADDRESS;
  case O::RETURN:
    return OIF::ZERO_ADDRESS;
  case O::CALL:
    return OIF::ONE_ADDRESS | OIF::TWO_ADDRESS;
  case O::LESS:
    return OIF::TWO_ADDRESS;
  case O::GREATER:
    return OIF::TWO_ADDRESS;
  case O::LESS_EQUAL:
    return OIF::TWO_ADDRESS;
  case O::GREATER_EQUAL:
    return OIF::TWO_ADDRESS;
  case O::EQUAL:
    return OIF::TWO_ADDRESS;
  case O::NOT_EQUAL:
    return OIF::TWO_ADDRESS;
  case O::ADD:
    return OIF::TWO_ADDRESS;
  case O::SUBTRACT:
    return OIF::TWO_ADDRESS;
  case O::MULTIPLY:
    return OIF::TWO_ADDRESS;
  case O::DIVIDE:
    return OIF::TWO_ADDRESS;
  case O::MODULUS:
    return OIF::TWO_ADDRESS;
  case O::NEGATE:
    return OIF::ONE_ADDRESS;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getSupportsZeroAddress(rq::Opcode opcode) {
  rq::OpcodeInfoFlags flags = rq::getInfoFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeInfoFlags::ZERO_ADDRESS);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getSupportsOneAddress(rq::Opcode opcode) {
  rq::OpcodeInfoFlags flags = rq::getInfoFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeInfoFlags::ONE_ADDRESS);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getSupportsTwoAddress(rq::Opcode opcode) {
  rq::OpcodeInfoFlags flags = rq::getInfoFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeInfoFlags::TWO_ADDRESS);
}

struct Expression;

struct Instruction final : public rq::Entity {
  using Self = rq::Instruction;

  rq::Entity *_address0_ptr{nullptr};
  rq::Entity *_address1_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET) {
    RQ_ASSERT(rq::getSupportsZeroAddress(opcode), "not zero address");
  }

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode, rq::Entity &address0)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET),
        _address0_ptr(&address0) {
    RQ_ASSERT(rq::getSupportsOneAddress(opcode), "not one address");
  }

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode, rq::Entity &address0,
                                        rq::Entity &address1)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET),
        _address0_ptr(&address0), _address1_ptr(&address1) {
    RQ_ASSERT(rq::getSupportsTwoAddress(opcode), "not two address");
  }

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode, rq::Entity &address0,
                                        rq::Entity *address1_ptr)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET),
        _address0_ptr(&address0), _address1_ptr(address1_ptr) {
    RQ_ASSERT(rq::getSupportsOneAddress(opcode), "not one address");
    RQ_ASSERT(rq::getSupportsTwoAddress(opcode), "not two address");
  }

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        rq::Entity *address0_ptr,
                                        rq::Entity *address1_ptr)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET),
        _address0_ptr(address0_ptr), _address1_ptr(address1_ptr) {
    RQ_ASSERT(rq::getSupportsZeroAddress(opcode), "not zero address");
    RQ_ASSERT(rq::getSupportsOneAddress(opcode), "not one address");
    RQ_ASSERT(rq::getSupportsTwoAddress(opcode), "not two address");
  }

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        rq::Entity *address0_ptr)
      : Entity(rq::getUnderlyingValue(opcode) + rq::OPCODE_OFFSET),
        _address0_ptr(address0_ptr) {
    RQ_ASSERT(rq::getSupportsZeroAddress(opcode), "not zero address");
    RQ_ASSERT(rq::getSupportsOneAddress(opcode), "not one address");
  }

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