#pragma once

#include <rq/name.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>

namespace rq {

struct Context;
struct SymbolTable;
struct Expression;

using LookupCandidates = llvm::SmallVector<rq::Entity *, 4>;

struct RValue final {
  using Self = rq::RValue;
};

struct Generator final {
  using Self = rq::Generator;

  rq::Context *_context_ptr;

  explicit RQ_ALWAYS_INLINE Generator(rq::Context &context)
      : _context_ptr(&context) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_context_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_context_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::RValue
  resolveRvalue(rq::SymbolTable &host, rq::Expression &expression);

  void lookup(rq::SymbolTable &host, llvm::ArrayRef<rq::Name> path,
              llvm::SmallVectorImpl<rq::Entity *> &candidates);

  RQ_ALWAYS_INLINE void
  lookup(rq::SymbolTable &host, rq::Name name,
         llvm::SmallVectorImpl<rq::Entity *> &candidates) {
    this->lookup(host, llvm::ArrayRef<rq::Name>(&name, 1), candidates);
  }

  void lookupMember(rq::SymbolTable &host, rq::Entity &receiver,
                    llvm::ArrayRef<rq::Name> path,
                    llvm::SmallVectorImpl<rq::Entity *> &candidates);

  RQ_ALWAYS_INLINE void
  lookupMember(rq::SymbolTable &host, rq::Entity &receiver, rq::Name name,
               llvm::SmallVectorImpl<rq::Entity *> &candidates) {
    this->lookupMember(host, receiver, llvm::ArrayRef<rq::Name>(&name, 1),
                       candidates);
  }

  [[nodiscard]] rq::Entity *lookupBuiltin(rq::Name name);
};

} // namespace rq
