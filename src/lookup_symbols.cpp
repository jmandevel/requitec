// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/export_table.hpp>
#include <requite/scope.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

requite::RootSymbol Scope::lookupInternalUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapIterator<requite::RootSymbol> it =
      this->getInternalSymbolMap().find(name);
  if (it != this->getInternalSymbolMap().end()) {
    return requite::RootSymbol(it->second);
  }
  return requite::RootSymbol();
}

requite::RootSymbol Scope::lookupExportUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  requite::ExportTable &export_table = this->getExportTable();
  requite::RootSymbol root = export_table.lookupExportUserSymbol(name);
  return root;
}

requite::RootSymbol Scope::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root = this->lookupInternalUserSymbol(name);
  if (root.getIsNone()) {
    if (this->getHasExportTable()) {
      root = this->lookupExportUserSymbol(name);
    }
  }
  return root;
}

requite::RootSymbol ExportTable::lookupExportUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapIterator<requite::RootSymbol> it =
      this->getSymbolMap().find(name);
  if (it != this->getSymbolMap().end()) {
    return requite::RootSymbol(it->second);
  }
  return requite::RootSymbol();
}

} // namespace requite