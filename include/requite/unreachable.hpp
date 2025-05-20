// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <source_location>
#include <stdexcept>
#include <utility>

namespace requite {

struct UnreachableException final : public std::exception {
  std::string _message;

  UnreachableException(
      std::source_location source_location = std::source_location::current()) {
    this->_message =
        std::format("unreachable code detected in {} at: {}:{}",
                    source_location.function_name(),
                    source_location.file_name(), source_location.line());
  }

  const char *what() const noexcept override { return this->_message.c_str(); }
};

} // namespace requite

#if defined(_NDEBUG)
#define REQUITE_UNRECHABLE() std::unreachable()
#else
#define REQUITE_UNREACHABLE() throw requite::UnreachableException()
#endif