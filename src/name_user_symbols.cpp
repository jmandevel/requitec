// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>

namespace requite {

bool Context::nameUserSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    for (std::unique_ptr<requite::Table> &table_uptr : module.getTableUptrs()) {
      requite::Table &table = requite::getRef(table_uptr);
    }
  }
  return true;
}

} // namespace requite