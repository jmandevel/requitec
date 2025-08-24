// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/literal_text.hpp>
#include <requite/module.hpp>
#include <requite/options.hpp>
#include <requite/strings.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>

namespace requite {

bool Context::getHasModule(llvm::StringRef name) const {
  return this->_module_map.contains(name);
}

requite::Module &Context::getModule(llvm::StringRef name) {
  REQUITE_ASSERT(this->_module_map.contains(name));
  requite::Module &module = requite::getRef(this->_module_map[name]);
  return module;
}

const requite::Module &Context::getModule(llvm::StringRef name) const {
  REQUITE_ASSERT(this->_module_map.contains(name));
  requite::Module &module = requite::getRef(this->_module_map.at(name));
  return module;
}

requite::Module &Context::getSourceModule() { return this->_source_module; }

const requite::Module &Context::getSourceModule() const {
  return this->_source_module;
}

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