// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/expression.hpp>

namespace requite {

template <requite::Situation SITUATION_PARAM>
void Context::logNotAtLeastBranchCount(requite::Expression &expression,
                                       unsigned count) {
  this->logSourceMessage(
      expression, requite::LogType::ERROR,
      llvm::Twine("expression with opcode \"") +
          requite::getName(expression.getOpcode()) + "\" in situation \"" +
          requite::getName<SITUATION_PARAM>() + "\" must have at least " +
          llvm::Twine(count) + " branches.");
}

template <requite::Situation SITUATION_PARAM>
void Context::logNotExactBranchCount(requite::Expression &expression,
                                     unsigned count) {
  this->logSourceMessage(
      expression, requite::LogType::ERROR,
      llvm::Twine("expression with opcode \"") +
          requite::getName(expression.getOpcode()) + "\" in situation \"" +
          requite::getName<SITUATION_PARAM>() + "\" must have exactly " +
          llvm::Twine(count) + " branches.");
}

template <requite::Situation SITUATION_PARAM>
void Context::logInvalidBranchSituation(requite::Expression &branch,
                                        requite::Opcode outer_opcode,
                                        requite::Opcode branch_opcode,
                                        unsigned branch_i,
                                        llvm::Twine log_context) {
  this->logSourceMessage(
      branch, requite::LogType::ERROR,
      llvm::Twine("operation with opcode \"") + requite::getName(outer_opcode) +
          "\" has branch with opcode \"" + requite::getName(branch_opcode) +
          "\" at index " + llvm::Twine(branch_i) +
          ". this is not valid in expected situation \"" +
          requite::getName<SITUATION_PARAM>() + "\" for " + log_context + ".");
}

void Context::logInvalidOperation(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "invalid operation.");
}

} // namespace requite