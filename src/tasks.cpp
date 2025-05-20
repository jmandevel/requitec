// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/options.hpp>

#include <llvm/Support/ThreadPool.h>
#include <llvm/Support/Threading.h>

#include <thread>

namespace requite {

void Context::startScheduler() {
  REQUITE_ASSERT(this->_scheduler_ptr.get() == nullptr);
  if (requite::options::SINGLE_THREAD_EXECUTION.getValue()) {
    return;
  }
  llvm::ThreadPoolStrategy strategy;
  strategy.Limit = requite::options::LIMIT_THREADS_TO_CORES.getValue();
  if (requite::options::THREAD_COUNT.getNumOccurrences() == 1) {
    strategy.ThreadsRequested = requite::options::THREAD_COUNT.getValue();
  } else {
    strategy.ThreadsRequested = std::thread::hardware_concurrency();
  }
  strategy.UseHyperThreads = requite::options::USE_HYPER_THREADS.getValue();
  this->_scheduler_ptr = std::make_unique<llvm::StdThreadPool>(strategy);
}

void Context::waitForTasks() {
  if (this->_scheduler_ptr.get() == nullptr) {
    return;
  }
  this->_scheduler_ptr->wait();
}

} // namespace requite