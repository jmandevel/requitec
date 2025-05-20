// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/module.hpp>

namespace requite {

requite::Module *Context::getModulePtr(llvm::StringRef name) {
  if (!this->_module_map.contains(name)) {
    return nullptr;
  }
  return this->_module_map.at(name);
}

const requite::Module *Context::getModulePtr(llvm::StringRef name) const {
  if (!this->_module_map.contains(name)) {
    return nullptr;
  }
  return this->_module_map.at(name);
}

} // namespace requite