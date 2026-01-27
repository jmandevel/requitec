#pragma once

#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerIntPair.h>

#include <bit>
#include <cstdint>

namespace rq {

enum class Opcode : std::uintptr_t {
  NONE = 0,

  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_COMPLEMENT,

  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  EQUAL,
  NOT_EQUAL,

  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  NEGATE,

  CAST,

  BITWISE_OR,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_COMPLEMENT,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  CONTENT_OF,
  ADDRESS_OF,
  CALL,
  DROP,
  MOVE,
  COPY,

  RETURN,
  GOTO,
  IF,

  LOOP_SEQUENCE,
  LOOP_ELEMENTS,
  LOOP_RANGER,

  PANIC_TRAP,
  DEBUG_TRAP,
  UNREACHABLE,
  ASSUME
};

struct RqirCons;

struct RqirAtom final {
  using Self = rq::RqirAtom;

  using Ptr = llvm::PointerUnion<rq::Entity *, rq::RqirCons *>;
  using PtrIntPair =
      llvm::PointerIntPair<Ptr, 2,
                           int>;
  static constexpr int ATOM_OPCODE = 0;
  static constexpr int ATOM_ENTITY = 1;
  static constexpr int ATOM_CONS = 2;

  PtrIntPair _ptr_int_pair{nullptr, false};

  RqirAtom() = default;
  RqirAtom(rq::Opcode opcode) {
    const std::uintptr_t bits = (static_cast<std::uintptr_t>(opcode) << 2);
    this->_ptr_int_pair.setPointer(std::bit_cast<Ptr>(bits));
    this->_ptr_int_pair.setInt(ATOM_OPCODE);
  }
  RqirAtom(rq::Entity &entity) {
    this->_ptr_int_pair.setPointer(&entity);
    this->_ptr_int_pair.setInt(ATOM_ENTITY);
  }
  RqirAtom(rq::RqirCons &cons) {
    this->_ptr_int_pair.setPointer(&cons);
    this->_ptr_int_pair.setInt(ATOM_CONS);
  }
  ~RqirAtom() = default;
  Self &operator=(const Self &) = delete;
  Self &operator==(Self &&) = delete;
  [[nodiscard]] bool getIsOpcode() const {
    return this->_ptr_int_pair.getInt() == ATOM_OPCODE;
  }
  [[nodiscard]] bool getIsEntity() const {
    return this->_ptr_int_pair.getInt() == ATOM_ENTITY;
  }
  [[nodiscard]] bool getIsCons() const {
    return this->_ptr_int_pair.getInt() == ATOM_CONS;
  }
  [[nodiscard]] rq::Opcode getOpcode() const {
    RQ_ASSERT(this->getIsOpcode(), "not opcode");
    auto bits = std::bit_cast<std::uintptr_t>(_ptr_int_pair);
    return static_cast<rq::Opcode>(bits >> 2);
  }
  [[nodiscard]] rq::Entity &getEntity() {
    RQ_ASSERT(this->getIsEntity(), "not entity");
    return rq::dereferencePtr(
        std::bit_cast<rq::Entity *>(this->_ptr_int_pair.getPointer()));
  }
  [[nodiscard]] const rq::Entity &getEntity() const {
    RQ_ASSERT(this->getIsEntity(), "not entity");
    return rq::dereferencePtr(
        std::bit_cast<rq::Entity *>(this->_ptr_int_pair.getPointer()));
  }
  [[nodiscard]] rq::RqirCons &getCons() {
    RQ_ASSERT(this->getIsCons(), "not cons");
    return rq::dereferencePtr(
        std::bit_cast<rq::RqirCons *>(this->_ptr_int_pair.getPointer()));
  }
  [[nodiscard]] const rq::RqirCons &getCons() const {
    RQ_ASSERT(this->getIsCons(), "not cons");
    return rq::dereferencePtr(
        std::bit_cast<rq::RqirCons *>(this->_ptr_int_pair.getPointer()));
  }
};

struct RqirCons final {
  using Self = RqirCons;

  rq::RqirAtom _car{};
  rq::RqirAtom _cdr{};

  RqirCons() = default;
};

} // namespace rq