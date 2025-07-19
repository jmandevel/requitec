// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

namespace requite {

Context::Context(std::string &&executable_path)
    : _executable_path(std::move(executable_path)) {}

llvm::StringRef Context::getExecutablePath() const {
  REQUITE_ASSERT(!this->_executable_path.empty());
  return this->_executable_path;
}

requite::Scope &Context::getOuterScope() { return this->_outer_scope; }

const requite::Scope &Context::getOuterScope() const {
  return this->_outer_scope;
}

} // namespace requite