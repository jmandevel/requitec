// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>

namespace requite {

bool Context::tabulateUserSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    
  }
  return true;
}

} // namespace requite