#pragma once
#include <rq/entity.hpp>
#include <rq/generational_arena.hpp>
#include <rq/static_value.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <optional>

namespace rq {

struct Evaluator final {
  // etc etc

  void getOuterMemberOf(rq::SymbolTable& host, rq::Expression &path, llvm::SmallVector<rq::Symbol*>& inout_candidate_ptrs);
  void getInnerMemberOf(rq::SymbolTable& host, rq::Expression& path, llvm::SmallVector<rq::Symbol*>& inout_candidate_ptrs);
};

  // TODO
  
} // namespace rq
