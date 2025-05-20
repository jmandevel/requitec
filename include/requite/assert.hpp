// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <memory>

namespace requite {

struct AssertException final : public std::exception {
  std::string _message;

  AssertException(
      std::string_view snippet,
      std::source_location source_location = std::source_location::current()) {
    this->_message = std::format(
        "assertion failure in \"{}\" at: {}:{}\t\"{}\"",
        source_location.function_name(), source_location.file_name(),
        source_location.line(), snippet);
  }

  const char *what() const noexcept override { return this->_message.c_str(); }
};

} // namespace requite

#if defined(_NDEBUG)
#define REQUITE_ASSERT(x)
#else
#define REQUITE_ASSERT(x)                                                      \
  if (!(x)) {                                                                  \
    throw requite::AssertException(#x);                                        \
  }
#endif

namespace requite {

struct NullptrDereferenceException final : public std::exception {
  std::string _message;

  NullptrDereferenceException(
      std::source_location source_location = std::source_location::current()) {
    this->_message =
        std::format("dereferenced nullptr \"{}\" at: {}:{}",
                    source_location.function_name(),
                    source_location.file_name(), source_location.line());
  }

  const char *what() const noexcept override { return this->_message.c_str(); }
};

template <typename TypeArg>
[[nodiscard]]
TypeArg &
getRef(TypeArg *ptr,
       std::source_location source_location = std::source_location::current()) {
#if !defined(_NDEBUG)
  if (ptr == nullptr) {
    throw requite::NullptrDereferenceException(source_location);
  }
#endif
  return *ptr;
}

template <typename TypeArg>
[[nodiscard]]
TypeArg &
getRef(std::unique_ptr<TypeArg>& uptr,
       std::source_location source_location = std::source_location::current()) {
#if !defined(_NDEBUG)
  if (uptr.get() == nullptr) {
    throw requite::NullptrDereferenceException(source_location);
  }
#endif
  return *uptr.get();
}

template <typename TypeArg>
[[nodiscard]]
const TypeArg &
getRef(const TypeArg *ptr,
       std::source_location source_location = std::source_location::current()) {
#if !defined(_NDEBUG)
  if (ptr == nullptr) {
    throw requite::NullptrDereferenceException(source_location);
  }
#endif
  return *ptr;
}

template <typename TypeArg>
[[nodiscard]]
const TypeArg &
getRef(const std::unique_ptr<TypeArg>& uptr,
       std::source_location source_location = std::source_location::current()) {
#if !defined(_NDEBUG)
  if (uptr.get() == nullptr) {
    throw requite::NullptrDereferenceException(source_location);
  }
#endif
  return *uptr.get();
}

struct SingleRefReassignment final : public std::exception {
  std::string _message;

  SingleRefReassignment(
      std::source_location source_location = std::source_location::current()) {
    this->_message = std::format(
        "assigned to value that was already assigned \"{}\" at: {}:{}",
        source_location.function_name(), source_location.file_name(),
        source_location.line());
  }

  const char *what() const noexcept override { return this->_message.c_str(); }
};

template <typename TypeArg>
void setSingleRef(
    TypeArg *&ptr_ref, TypeArg &ref,
    std::source_location source_location = std::source_location::current()) {
#if !defined(_NDEBUG)
  if (ptr_ref != nullptr) {
    throw requite::SingleRefReassignment(source_location);
  }
#endif
  ptr_ref = &ref;
}

} // namespace requite