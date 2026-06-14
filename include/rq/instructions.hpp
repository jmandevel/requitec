#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <rq/utility.hpp>

#include <llvm/Support/Casting.h>

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::Opcode opcode) {
  using O = rq::Opcode;
  switch (opcode) {
  case O::NONE:
    return "NONE";

  case O::CONSTANT:
    return "CONST";

  // ARITHMETIC
  case O::ADD:
    return "ADD";
  case O::SUBTRACT:
    return "SUB";
  case O::MULTIPLY:
    return "MUL";
  case O::MODULUS:
    return "MOD";
  case O::NEGATE:
    return "NEG";

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

  case O::CONSTANT:
    return OF::NONE;
  case O::OPERATION:
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullary(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::NULLARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnary(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::UNARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::BINARY);
}

struct Expression;

struct Instruction final : public rq::Entity {
  using Self = rq::Instruction;

  rq::Entity *_address0_ptr;
  rq::Entity *_address1_ptr;

  explicit RQ_ALWAYS_INLINE
  Instruction(rq::Opcode opcode)
      : Entity(rq::getUnderlying(opcode) + rq::OPCODE_OFFSET) {}

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

  RQ_ALWAYS_INLINE void changeAddress0(rq::Entity &entity) {
    this->_address0_ptr = &entity;
  }

  RQ_ALWAYS_INLINE void changeAddress1(rq::Entity &entity) {
    this->_address1_ptr = &entity;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::OPCODE_OFFSET;
  }
};

} // namespace rq