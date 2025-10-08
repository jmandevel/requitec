#pragma once

#include <llvm/ADT/StringRef.h>

#include <cstdint>
#include <format>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>

namespace rq {

struct _AssertException final : public std::logic_error {
  _AssertException(
      std::string_view snippet, std::string_view reason,
      std::source_location source_location = std::source_location::current())
      : std::logic_error(std::format("{} in \"{}\" at: {}:{}\n\t\"{}\"", reason,
                                     source_location.function_name(),
                                     source_location.file_name(),
                                     source_location.line(), snippet)) {}

  using std::logic_error::what;
};

#if defined(_MSC_VER)
#define RQ_ALWAYS_INLINE inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define RQ_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define RQ_ALWAYS_INLINE inline
#endif

#if !defined(_NDEBUG)
#define RQ_ASSERT(condition, reason)                                           \
  if (!(condition)) {                                                          \
    throw rq::_AssertException(#condition, reason);                            \
  }
#define RQ_ASSERT_LOCATION(condition, reason, location)                        \
  if (!(condition)) {                                                          \
    throw rq::_AssertException(#condition, reason, location);                  \
  }
#else
#define RQ_ASSERT(condition, reason)
#define RQ_ASSERT_LOCATION(condition, reason, location)
#endif

#if !defined(_NDEBUG)
#define RQ_UNREACHABLE()                                                       \
  throw rq::_AssertException("RQ_UNREACHABLE()", "unreachable code detected");
#else
#define RQ_UNREACHABLE() std::unreachable()
#endif

#if !defined(_NDEBUG)
#define RQ_TODO_IMPLEMENTATION()                                               \
  throw rq::_AssertException("RQ_NOT_IMPLEMENTED()", "not implemented yet");
#else
#define RQ_NOT_IMPLEMENTED() std::unreachable()
#endif

template <typename TypeParam>
[[nodiscard]] RQ_ALWAYS_INLINE TypeParam &dereferencePtr(
    TypeParam *ptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(ptr != nullptr, "nullptr dereference", source_location);
  return *ptr;
}

template <typename TypeParam>
[[nodiscard]] RQ_ALWAYS_INLINE const TypeParam &dereferencePtr(
    const TypeParam *ptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(ptr != nullptr, "nullptr dereference", source_location);
  return *ptr;
}

template <typename TypeParam>
[[nodiscard]] RQ_ALWAYS_INLINE TypeParam &dereferenceUptr(
    std::unique_ptr<TypeParam> &uptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(uptr != nullptr, "nullptr dereference", source_location);
  return *uptr;
}

template <typename TypeParam>
[[nodiscard]] RQ_ALWAYS_INLINE const TypeParam &dereferenceUptr(
    const std::unique_ptr<TypeParam> &uptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(uptr != nullptr, "nullptr dereference", source_location);
  return *uptr;
}

template <typename TypeParam>
RQ_ALWAYS_INLINE void assignSingleValue(
    TypeParam *&dest_ptr, TypeParam *src_ptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(dest_ptr == nullptr, "single value reassignment",
                     source_location);
  dest_ptr = src_ptr;
}

template <typename FlagsParam> struct is_flags final : std::false_type {};

template <typename FlagsParam>
constexpr bool is_flags_v = rq::is_flags<FlagsParam>::value;

template <typename FlagsParam>
concept flags = rq::is_flags<FlagsParam>::value;

template <typename FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::underlying_type_t<FlagsParam>
getUnderlying(FlagsParam flags) {
  return static_cast<std::underlying_type_t<FlagsParam>>(flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::uint64_t
getBit(std::uint64_t bit_i) {
  return (static_cast<std::uint64_t>(1) << bit_i);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator|(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) |
                                 rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator|(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) | rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator|(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs | rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator&(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) &
                                 rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator&(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) & rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator&(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs & rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator^(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) ^
                                 rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator^(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) & rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator^(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs ^ rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator<<(FlagsParam lhs,
                                                               FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs)
                                 << rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator<<(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) << rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator<<(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs << rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator>>(FlagsParam lhs,
                                                               FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) >>
                                 rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator>>(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUnderlying(lhs) >> rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator>>(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs >> rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator~(FlagsParam rhs) {
  return static_cast<FlagsParam>(~rq::getUnderlying(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getHasAll(FlagsParam flags,
                                                        FlagsParam has) {
  return (flags & has) == has;
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getHasSome(FlagsParam flags,
                                                         FlagsParam has) {
  return (flags & has) != static_cast<FlagsParam>(0);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getHasNone(FlagsParam flags,
                                                         FlagsParam has) {
  return (flags & has) == static_cast<FlagsParam>(0);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::underlying_type_t<FlagsParam>
getMaskValue(FlagsParam flags, FlagsParam mask) {
  return rq::getUnderlying(flags & mask);
}

enum class Error : std::uint_fast32_t { OK };

struct ErrorCategory final : public std::error_category {
  [[nodiscard]] RQ_ALWAYS_INLINE const char *name() const noexcept override {
    return "requitec";
  }

  [[nodiscard]] inline std::string message(int ec) const override {
    switch (static_cast<rq::Error>(ec)) {
    case rq::Error::OK:
      return "ok";
    default:
      break;
    }
    return "unrecognized error";
  }
};

[[nodiscard]] inline rq::ErrorCategory &getErrorCategory() {
  static rq::ErrorCategory instance;
  return instance;
}

[[nodiscard]] inline std::error_code getErrorCode(rq::Error error) {
  return std::error_code{static_cast<int>(error), rq::getErrorCategory()};
}

} // namespace rq