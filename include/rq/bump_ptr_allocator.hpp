#pragma once

#include <rq/utility.hpp>

#include <llvm/Support/Allocator.h>
#include <llvm/Support/StringSaver.h>

#include <cstring>
#include <span>

namespace rq {

struct Top;

template <typename TypeParam> struct is_acquired final : std::false_type {};

template <typename TypeParam>
constexpr bool is_acquired_v = rq::is_acquired<TypeParam>::value;

template <typename TypeParam>
concept acquired = rq::is_acquired<TypeParam>::value;

template <typename TypeParam> struct is_parent_only final : std::false_type {};

template <typename TypeParam>
constexpr bool is_parent_only_v = rq::is_parent_only<TypeParam>::value;

template <typename TypeParam>
concept parent_only = rq::is_parent_only<TypeParam>::value;

struct BumpPtrAllocator {
  using Self = rq::BumpPtrAllocator;

  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};

  BumpPtrAllocator() = default;
  BumpPtrAllocator(const Self &) = delete;
  BumpPtrAllocator(Self &&) = delete;
  ~BumpPtrAllocator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateValue(ArgNParam &&...arg_n) {
    static_assert(!rq::acquired<TypeParam>, "type must be acquired");
    static_assert(!rq::parent_only<TypeParam>, "type can not be allocted");
    static_assert(!std::same_as<TypeParam, rq::Top>, "no allocate top");
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateAcquiredValue(ArgNParam &&...arg_n) {
    static_assert(rq::acquired<TypeParam>, "type must be allocated");
    static_assert(!rq::parent_only<TypeParam>, "type can not be acquired");
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline std::span<TypeParam> allocateZeroedArray(unsigned count) {
    static_assert(!rq::acquired<TypeParam>, "type must be acquired");
    static_assert(!rq::parent_only<TypeParam>, "type can not be allocted");
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(static_cast<void *>(ptr), 0, sizeof(TypeParam) * count);
    return std::span<TypeParam>(ptr, count);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline std::span<TypeParam> allocateAcquiredZeroedArray(unsigned count) {
    static_assert(rq::acquired<TypeParam>, "type must be allocated");
    static_assert(!rq::parent_only<TypeParam>, "type can not be allocted");
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(static_cast<void *>(ptr), 0, sizeof(TypeParam) * count);
    return std::span<TypeParam>(ptr, count);
  }
  inline llvm::StringRef saveString(llvm::Twine twine) {
    return this->_llvm_string_saver.save(twine);
  }
};

} // namespace rq