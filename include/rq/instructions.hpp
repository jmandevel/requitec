#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <rq/utility.hpp>

namespace rq {

enum class Opcode : std::uint_fast32_t { NONE };

enum class OpcodeFlags : std::uint32_t {
  NONE = 0,

  NULLARY = rq::getBit(0),
  UNARY = rq::getBit(1),
  BINARY = rq::getBit(2)
};

template <> struct is_flags<rq::OpcodeFlags> : std::true_type {};

[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  return rq::OpcodeFlags::NONE;
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

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  rq::Opcode opcode;
  const rq::Expression *_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        const rq::Expression *expression_ptr)
      : Entity(rq::getUnderlying(opcode)), _expression_ptr(expression_ptr) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getExpressionPtr() const {
    return this->_expression_ptr;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::OPCODE_OFFSET;
  }
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE
  NullaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr)
      : Instruction(opcode, expression_ptr) {}

  [[nodiscard]] static inline bool classof(const rq::Instruction *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    if (!llvm::isa<rq::Instruction>(entity)) {
      return false;
    }
    const rq::EntityId id = entity.getId();
    return rq::getIsNullary(
        static_cast<rq::Opcode>(id - rq::OPCODE_OFFSET));
  }
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr;

  explicit RQ_ALWAYS_INLINE
  UnaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr,
                   rq::Entity &address0) :
                   Instruction(opcode, expression_ptr), _address0_ptr(&address0) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const {
    return rq::dereferencePtr(this->_address0_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0() {
    return rq::dereferencePtr(this->_address0_ptr);
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    if (!llvm::isa<rq::Instruction>(entity)) {
      return false;
    }
    const rq::EntityId id = entity.getId();
    return rq::getIsUnary(
        static_cast<rq::Opcode>(id - rq::OPCODE_OFFSET));
  }
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity *_address0_ptr;
  rq::Entity *_address1_ptr;

  explicit RQ_ALWAYS_INLINE
  BinaryInstruction(rq::Opcode opcode, rq::Expression *expression_ptr,
                    rq::Entity &address0, rq::Entity &address1)
      : Instruction(opcode, expression_ptr), _address0_ptr(&address0),
        _address1_ptr(&address1) {}

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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    if (!llvm::isa<rq::Instruction>(entity)) {
      return false;
    }
    const rq::EntityId id = entity.getId();
    return rq::getIsBinary(
        static_cast<rq::Opcode>(id - rq::OPCODE_OFFSET));
  }
};

} // namespace rq