// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/expression.hpp>

namespace requite {

template <requite::Situation SITUATION_PARAM>
void Context::logErrorNotAtLeastBranchCount(requite::Expression &expression,
                                            unsigned count) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getDescription(SITUATION_PARAM)) +
                             " expression with opcode " +
                             requite::getName(expression.getOpcode()) +
                             " must have at least " + llvm::Twine(count) +
                             " branches.\n");
}

template <requite::Situation SITUATION_PARAM>
void Context::logErrorNotExactBranchCount(requite::Expression &expression,
                                          unsigned count) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getDescription(SITUATION_PARAM)) +
                             " expression with opcode " +
                             requite::getName(expression.getOpcode()) +
                             " must have exactly " + llvm::Twine(count) +
                             " branches.\n");
}

template <requite::Situation SITUATION_PARAM>
void Context::logErrorTooNotLessOrEqualToBranchCount(
    requite::Expression &expression, unsigned count) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getDescription(SITUATION_PARAM)) +
                             " expression with opcode " +
                             requite::getName(expression.getOpcode()) +
                             " must have no more than " + llvm::Twine(count) +
                             " branches.\n");
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_PARAM>
void Context::logErrorInvalidBranchSituation(requite::Expression &branch,
                                             requite::Opcode outer_opcode,
                                             requite::Opcode branch_opcode,
                                             unsigned branch_i,
                                             llvm::Twine log_context) {
  this->logSourceMessage(
      branch, requite::LogType::ERROR,
      llvm::Twine(requite::getDescription(BRANCH_SITUATION_PARAM)) +
          " expression expected for " + log_context + " of " +
          requite::getDescription(SITUATION_PARAM) + " " +
          requite::getName(outer_opcode) + " but found " +
          requite::getName(branch_opcode) + " at index " +
          llvm::Twine(branch_i) + ".\n");
}

void Context::logErrorInvalidOperation(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "invalid operation.\n");
}

} // namespace requite