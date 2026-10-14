#pragma once

#include <rq/entity.hpp>

namespace rq {

struct Instruction;

struct CfgBlock final : public rq::Entity {
  using Self = rq::CfgBlock;

  explicit RQ_ALWAYS_INLINE CfgBlock();

  

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id == rq::CFG_BLOCK_ID;
  }
};

} // namespace rq