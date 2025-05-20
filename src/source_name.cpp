// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/source.hpp>
#include <requite/assert.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/Support/raw_ostream.h>

namespace requite {

bool Context::mapModules() {
  struct Entry final {
    llvm::SmallVector<requite::Source *, 1> sources = {};

    Entry() = default;
    Entry(requite::Source &source) { this->sources.push_back(&source); }
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
      entry.sources.push_back(&module.getSource());
      continue;
    }
    name_map.insert({module.getTable().getName(), Entry(module.getSource())});
  }
  bool is_ok = true;
  for (llvm::detail::DenseMapPair<llvm::StringRef, Entry> &kvp : name_map) {
    Entry &entry = kvp.second;
    if (entry.sources.size() > 1) {
      is_ok = false;
      for (requite::Source *module_source_ptr : entry.sources) {
        requite::Source &source = *module_source_ptr;
        requite::Expression &base = source.getRoot();
        REQUITE_ASSERT(base.getOpcode() == requite::Opcode::MODULE);
        requite::Expression &name = base.getBranch();
        REQUITE_ASSERT(name.getIsIdentifier());
        this->logSourceMessage(name, requite::LogType::ERROR,
                               "module name is not unique");
      }
    }
  }
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->_module_map.insert({module.getTable().getName(), &module});
  }
  return is_ok;
}

} // namespace requite