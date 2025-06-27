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

bool Context::determineModuleName(requite::Module &module) {
  requite::Expression &root = module.getExpression();
  requite::Expression &name_expression = root.getBranch();
  if (!name_expression.getIsIdentifier()) {
    this->logSourceMessage(name_expression, requite::LogType::ERROR,
                           "module name is not instantly determinable");
    return false;
  }
  llvm::StringRef name = name_expression.getDataText();
  module.setName(name);
  return true;
}

} // namespace requite