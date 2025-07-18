// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/module.hpp>

namespace requite {

void Context::catalogueImport(requite::Import& import) {

}

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

std::vector<std::unique_ptr<requite::Module>> &Context::getModuleUptrs() {
  return this->_module_uptrs;
}

const std::vector<std::unique_ptr<requite::Module>> &
Context::getModuleUptrs() const {
  return this->_module_uptrs;
}

requite::Module &Context::getSourceModule() {
  return this->_source_module;
}

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