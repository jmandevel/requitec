// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/finalizer.hpp>

namespace requite {

bool Context::finalizeLocals(requite::Module &module) {
  requite::Finalizer finalizer(*this, module);
  finalizer.finalizeLocals();
  return finalizer.getIsOk();
}

void Finalizer::finalizeLocals() {
  // TODO
}

} // namespace requite