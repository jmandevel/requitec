#pragma once

#include <rq/name.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallPtrSet.h>

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
              llvm::SmallPtrSet<rq::Entity *, 1> &candidates);

  RQ_ALWAYS_INLINE void
  lookup(rq::SymbolTable &host, rq::Name name,
         llvm::SmallPtrSet<rq::Entity *, 1> &candidates) {
    this->lookup(host, llvm::ArrayRef<rq::Name>(&name, 1), candidates);
  }

  void dfsPath(rq::Module& module, rq::TableMember &receiver,
                    llvm::ArrayRef<rq::Name> path,
                    llvm::SmallPtrSet<rq::Entity *, 1> &candidates);


  [[nodiscard]] bool tryImplement(rq::TableMember& member);

  [[nodiscard]] rq::Entity* tryLookupBuiltin(rq::Name name);
};

} // namespace rq
