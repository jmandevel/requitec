// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>

namespace requite {

ExpressionWalkResult::ExpressionWalkResult(unsigned count, bool is_done)
    : _count(count), _is_done(is_done) {}

unsigned ExpressionWalkResult::getWalkedCount() const { return this->_count; }

bool ExpressionWalkResult::getIsDone() const { return this->_is_done; }

bool ExpressionWalkResult::getWalkedAtLeast(unsigned count) {
  REQUITE_ASSERT(count != 0);
  return this->getWalkedCount() >= count;
}

bool ExpressionWalkResult::getWalkedExactly(unsigned count) {
  return this->getWalkedCount() == count && this->getIsDone();
}

} // namespace requite