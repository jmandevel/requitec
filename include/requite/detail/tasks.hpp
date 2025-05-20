// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

template <typename TaskPram> void Context::scheduleTask(TaskPram &&task) {
  if (this->_scheduler_ptr.get() == nullptr) {
    task();
    return;
  }
  this->_scheduler_ptr->async(std::move(task));
}

} // namespace requite