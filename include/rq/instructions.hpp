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

enum class OpcodeFlags : std::uint32_t {
  NONE = 0,
};

template <> struct is_flags<rq::OpcodeFlags> : std::true_type {};

[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OF = rq::OpcodeFlags;
  switch (opcode) {
  case O::NONE:
    break;

  case O::DEBUG_STEP:
    return OF::NONE;
  case O::SOURCE_RANGE:
    return OF::NONE;

  case O::STATEMENT:
    return OF::NONE;
  case O::ASSIGN:
    return OF::NONE;

  // ARITHMETIC
  case O::ADD:
    return OF::NONE;
  case O::SUBTRACT:
    return OF::NONE;
  case O::MULTIPLY:
    return OF::NONE;
  case O::DIVIDE:
    return OF::NONE;
  case O::MODULUS:
    return OF::NONE;
  case O::NEGATE:
    return OF::NONE;

  case O::LAST:
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

} // namespace rq