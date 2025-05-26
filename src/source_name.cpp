// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/Support/raw_ostream.h>

namespace requite {

bool Context::setupModuleNames() {
  const bool determined_ok = this->determineModuleNames();
  const bool mapped_ok = this->mapModules();
  return determined_ok && mapped_ok;
}

bool Context::determineModuleNames() {
  bool is_ok = true;
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    requite::Expression &root = module.getExpression();
    requite::Expression &name_expression = root.getBranch();
    if (!name_expression.getIsIdentifier()) {
      this->logSourceMessage(name_expression, requite::LogType::ERROR,
                             "module name is not instantly determinable");
      is_ok = false;
    }
    llvm::StringRef name = name_expression.getDataText();
    module.setName(name);
  }
  return is_ok;
}

bool Context::mapModules() {
  struct Entry final {
    llvm::SmallVector<requite::Module *, 1> modules = {};

    Entry() = default;
    Entry(requite::Module &module) { this->modules.push_back(&module); }
  };
  llvm::DenseMap<llvm::StringRef, Entry> name_map;
  name_map.reserve(this->getModuleUptrs().size());
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    if (!module.getTable().getHasName()) {
      continue;
    }
    if (name_map.contains(module.getTable().getName())) {
      Entry &entry = name_map[module.getTable().getName()];
      entry.modules.push_back(&module);
      continue;
    }
    name_map.insert({module.getTable().getName(), Entry(module)});
  }
  bool is_ok = true;
  for (llvm::detail::DenseMapPair<llvm::StringRef, Entry> &kvp : name_map) {
    Entry &entry = kvp.second;
    if (entry.modules.size() > 1) {
      is_ok = false;
      for (requite::Module *module_ptr : entry.modules) {
        requite::Module &module = requite::getRef(module_ptr);
        requite::Expression &root = module.getExpression();
        REQUITE_ASSERT(root.getOpcode() == requite::Opcode::MODULE);
        requite::Expression &name_expression = root.getBranch();
        REQUITE_ASSERT(name_expression.getIsIdentifier());
        llvm::StringRef name = name_expression.getDataText();
        this->logSourceMessage(name, requite::LogType::ERROR,
                               llvm::Twine("module name \"") + name +
                                   "\" is not unique");
      }
    }
  }
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->_module_map.insert({module.getName(), &module});
  }
  return is_ok;
}

} // namespace requite