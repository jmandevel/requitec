#pragma once

#include <requite/symbol.hpp>

#include <llvm/IR/Instructions.h>

namespace requite {

struct Temporary final {
  requite::Symbol type{};
  llvm::AllocaInst *llvm_alloca = nullptr;
};

} // namespace requite