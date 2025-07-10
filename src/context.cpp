// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

namespace requite {

Context::Context(std::string &&executable_path)
    : _executable_path(std::move(executable_path)) {
  this->getSourceModule().getScope().setTable(this->getBaseTable());
}

llvm::StringRef Context::getExecutablePath() const {
  REQUITE_ASSERT(!this->_executable_path.empty());
  return this->_executable_path;
}

requite::Table &Context::getBaseTable() { return this->_base_table; }

const requite::Table &Context::getBaseTable() const { return this->_base_table; }

} // namespace requite