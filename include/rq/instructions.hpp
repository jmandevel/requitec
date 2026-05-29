#pragma once

#include <rq/entity.hpp>

#include <rq/utility.hpp>

namespace rq {

enum class Opcode : std::uint_fast32_t {
    NONE
};

struct Expression;

struct Instruction {
  using Self = rq::Instruction;

  rq::Opcode opcode;
  const rq::Expression *_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        const rq::Expression *expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;

  [[nodiscard]] static inline bool
  classof(const rq::Instruction *instruction_ptr);
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE
  NullaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr);

  [[nodiscard]] static inline bool
  classof(const rq::Instruction *instruction_ptr);
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity _address0;

  explicit RQ_ALWAYS_INLINE
  UnaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr,
                   rq::Entity &&address0);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();

  [[nodiscard]] static inline bool
  classof(const rq::Instruction *instruction_ptr);
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity &_address0;
  rq::Entity &_address1;

  explicit RQ_ALWAYS_INLINE BinaryInstruction(rq::Opcode opcode,
                                              rq::Entity &&address0,
                                              rq::Entity &&address1);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress1() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress1();

  [[nodiscard]] static inline bool
  classof(const rq::Instruction *instruction_ptr);
};

} // namespace rq