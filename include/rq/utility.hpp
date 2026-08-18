#pragma once

#include <llvm/ADT/PointerIntPair.h>
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

struct AssertException final : public std::logic_error {
  AssertException(
      std::string_view snippet, std::string_view reason,
      std::source_location source_location = std::source_location::current())
      : std::logic_error(std::format("{} in \"{}\" at: {}:{}\n\t\"{}\"", reason,
                                     source_location.function_name(),
                                     source_location.file_name(),
                                     source_location.line(), snippet)) {}

  using std::logic_error::what;
};

#if defined(_MSC_VER) && defined(_NDEBUG)
#define RQ_ALWAYS_INLINE inline __forceinline
#elif (defined(__GNUC__) || defined(__clang__)) && defined(_NDEBUG)
#define RQ_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define RQ_ALWAYS_INLINE inline
#endif

#if !defined(_NDEBUG)
#define RQ_ASSERT(condition, reason)                                           \
  if (!(condition)) {                                                          \
    throw rq::AssertException(#condition, reason);                             \
  }
#define RQ_ASSERT_LOCATION(condition, reason, location)                        \
  if (!(condition)) {                                                          \
    throw rq::AssertException(#condition, reason, location);                   \
  }
#else
#define RQ_ASSERT(condition, reason)
#define RQ_ASSERT_LOCATION(condition, reason, location)
#endif

#if !defined(_NDEBUG)
#define RQ_UNREACHABLE()                                                       \
  throw rq::AssertException("RQ_UNREACHABLE()", "unreachable code detected");
#else
#define RQ_UNREACHABLE() std::unreachable()
#endif

#if !defined(_NDEBUG)
#define RQ_UNHANDLED_ERROR(reason)                                             \
  throw rq::AssertException("RQ_UNHANDLED_ERROR(\"" #reason "\")", #reason);
#else
#define RQ_UNHANDLED_ERROR() std::unreachable()
#endif

#if !defined(_NDEBUG)
#define RQ_TODO_IMPLEMENTATION()                                               \
  throw rq::AssertException("RQ_TODO_IMPLEMENTATION()", "not implemented "     \
                                                        "yet");
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

template <typename DestParam, typename SrcParam>
RQ_ALWAYS_INLINE void assignSingleValue(
    DestParam *&dest_ptr, SrcParam *src_ptr,
    std::source_location source_location = std::source_location::current()) {
  RQ_ASSERT_LOCATION(dest_ptr == nullptr, "single value reassignment",
                     source_location);
  dest_ptr = src_ptr;
}

template <typename DestParam, typename SrcParam>
RQ_ALWAYS_INLINE DestParam &replaceValue(
    DestParam *&dest_ptr, SrcParam *src_ptr,
    std::source_location source_location = std::source_location::current()) {
  DestParam &old = rq::dereferencePtr(dest_ptr, source_location);
  dest_ptr = src_ptr;
  return old;
}

template <typename DestParam, typename SrcParam>
RQ_ALWAYS_INLINE DestParam *replaceValuePtr(DestParam *&dest_ptr,
                                            SrcParam *src_ptr) {
  DestParam *old_ptr = dest_ptr;
  dest_ptr = src_ptr;
  return old_ptr;
}

template <typename TypeParam>
RQ_ALWAYS_INLINE TypeParam &popValue(
    TypeParam *&src_ptr,
    std::source_location source_location = std::source_location::current()) {
  TypeParam *temp_ptr = src_ptr;
  src_ptr = nullptr;
  return rq::dereferencePtr(temp_ptr, source_location);
}

template <typename TypeParam>
RQ_ALWAYS_INLINE TypeParam *popValuePtr(TypeParam *&src_ptr) {
  TypeParam *temp_ptr = src_ptr;
  src_ptr = nullptr;
  return temp_ptr;
}

template <typename FlagsParam> struct is_flags final : std::false_type {};

template <typename FlagsParam>
constexpr bool is_flags_v = rq::is_flags<FlagsParam>::value;

template <typename FlagsParam>
concept flags = rq::is_flags<FlagsParam>::value;

#define RQ_DEFINE_FLAGS(FLAGS) template<> struct is_flags<typename FLAGS> final : std::true_type {}

template <typename FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::underlying_type_t<FlagsParam>
getUNDERLYING_VALUE(FlagsParam flags) {
  return static_cast<std::underlying_type_t<FlagsParam>>(flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::uint64_t
getBit(std::uint64_t bit_i) {
  return (static_cast<std::uint64_t>(1) << bit_i);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator|(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) |
                                 rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator|(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) | rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator|(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs | rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator&(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) &
                                 rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator&(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) & rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator&(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs & rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator^(FlagsParam lhs,
                                                              FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) ^
                                 rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator^(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) & rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator^(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs ^ rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
RQ_ALWAYS_INLINE constexpr FlagsParam &operator|=(FlagsParam &lhs,
                                                  FlagsParam rhs) {
  lhs =
      static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) | rq::getUNDERLYING_VALUE(rhs));
  return lhs;
}

template <rq::flags FlagsParam>
RQ_ALWAYS_INLINE constexpr FlagsParam &operator&=(FlagsParam &lhs,
                                                  FlagsParam rhs) {
  lhs =
      static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) & rq::getUNDERLYING_VALUE(rhs));
  return lhs;
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator<<(FlagsParam lhs,
                                                               FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs)
                                 << rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator<<(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) << rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator<<(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs << rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator>>(FlagsParam lhs,
                                                               FlagsParam rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) >>
                                 rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator>>(FlagsParam lhs, std::underlying_type_t<FlagsParam> rhs) {
  return static_cast<FlagsParam>(rq::getUNDERLYING_VALUE(lhs) >> rhs);
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam
operator>>(std::underlying_type_t<FlagsParam> lhs, FlagsParam rhs) {
  return static_cast<FlagsParam>(lhs >> rq::getUNDERLYING_VALUE(rhs));
}

template <rq::flags FlagsParam>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr FlagsParam operator~(FlagsParam rhs) {
  return static_cast<FlagsParam>(~rq::getUNDERLYING_VALUE(rhs));
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
  return rq::getUNDERLYING_VALUE(flags & mask);
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

template <typename TypeParam, unsigned FLAG_BITS_PARAM, typename FlagsParam>
struct PtrWithFlags {
  using Type = TypeParam;
  static constexpr unsigned FLAG_BITS = FLAG_BITS_PARAM;
  using Flags = FlagsParam;
  using Self = rq::PtrWithFlags<Type, FLAG_BITS, Flags>;
#if defined(_NDEBUG)
  llvm::PointerIntPair<Type *, FLAG_BITS, Flags> _ptr_int_pair;
#else
  Type *_ptr{nullptr};
  Flags _flags{};
#endif

  PtrWithFlags() = default;
  [[nodiscard]] RQ_ALWAYS_INLINE Type *const &getPtr() const {
#if defined(_NDEBUG)
    return this->_ptr_int_pair.getPointer();
#else
    return this->_ptr;
#endif
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Type *&getPtr() {
#if defined(_NDEBUG)
    return _ptr_int_pair.getPointer();
#else
    return _ptr;
#endif
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Flags getFlags() const {
#if defined(_NDEBUG)
    return static_cast<Flags>(this->_ptr_int_pair.getInt());
#else
    return this->_flags;
#endif
  }
  RQ_ALWAYS_INLINE void setPtr(Type *ptr) {
#if defined(_NDEBUG)
    this->_ptr_int_pair.setPointer(ptr);
#else
    this->_ptr = ptr;
#endif
  }
  RQ_ALWAYS_INLINE void addFlags(Flags flags) {
#if defined(_NDEBUG)
    this->_ptr_int_pair.getInt() |= rq::getUNDERLYING_VALUE(flags);
#else
    this->_flags |= flags;
#endif
  }
};

} // namespace rq
