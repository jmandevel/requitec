#pragma once

#include <rq/generational_arena.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/AlignOf.h>

namespace rq {

struct StaticValue;

struct StaticSlice final {
  using Self = StaticSlice;

  rq::Gendex<rq::StaticValue> gendex;
  std::size_t offset;
  std::size_t count;
};

using StaticInt = llvm::APInt;

using StaticFloat = llvm::APFloat;

using StaticArray = std::vector<rq::StaticValue>;

enum class StaticValueKind {
  SLICE,
  INT,
  FLOAT,
  ARRAY
};

struct StaticValue final {
  using Self = rq::StaticValue;

  rq::StaticValueKind _kind : 2;
  llvm::AlignedCharArrayUnion<rq::StaticSlice,
                                         rq::StaticInt, rq::StaticFloat,
                                         rq::StaticArray> _data;
};

} // namespace rq