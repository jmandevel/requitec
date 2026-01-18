#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/Support/AlignOf.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/Support/Allocator.h>

namespace rq {

enum class ValueKind : std::uint32_t {
  NONE,
  INDETERMINATE,
  INT,
  FLOAT,
  SYMBOL
};

enum class ValueFlags : std::uint32_t {
  NONE,
  DEAD = rq::getBit(0),
  PLATFORM_SPECIFIC = rq::getBit(1),
  NEEDS_CLEANUP = rq::getBit(2)
};

template <> struct is_flags<rq::ValueFlags> final : std::true_type {};

// https://clang.llvm.org/doxygen/APValue_8h_source.html

struct Symbol;
struct TypeSymbol;

struct Value {
  using Self = rq::Value;

  rq::ValueKind _kind;
  rq::ValueFlags _flags{};
  rq::TypeSymbol *_type_ptr;
  llvm::AlignedCharArrayUnion<llvm::APSInt, llvm::APFloat, rq::Symbol *> _data;

  Value(rq::ValueKind kind, rq::TypeSymbol &type)
      : _kind(kind), _type_ptr(&type) {}

  [[nodiscard]] rq::ValueKind getKind() const { return this->_kind; }
  [[nodiscard]] rq::TypeSymbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  void RQ_ALWAYS_INLINE kill() { this->_flags |= rq::ValueFlags::DEAD; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAlive() const {
    return rq::getHasNone(this->_flags, rq::ValueFlags::DEAD);
  }
};

} // namespace rq