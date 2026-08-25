#pragma once

#include <rq/entity.hpp>

namespace rq {

struct Instruction;

struct CfgBlock final : public rq::Entity {
  using Self = rq::CfgBlock;

  rq::Instruction *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE CfgBlock();

  RQ_ALWAYS_INLINE void setFirstInstruction(rq::Instruction *instruction_ptr) {
    rq::assignSingleValue(this->_first_ptr, instruction_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction *
  getInstructionPtr() const {
    return this->_first_ptr;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction *getInstructionPtr() {
    return this->_first_ptr;
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id == rq::CFG_BLOCK_ID;
  }
};

} // namespace rq