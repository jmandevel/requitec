#pragma once

#include <rq/generational_arena.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/AlignOf.h>
#include <llvm/Support/Casting.h>

#include <bit>
#include <cstdint>
#include <memory>

namespace rq {

enum class StaticValueKind {
  NONE,
  INT,
  FLOAT,
  SLICE,
  ARRAY,
  DATA_ARRAY,
  SYMBOL,
  LOW_ATTRIBUTE,
  HIGH_ATTRIBUTE
};

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

using StaticDataArray = std::vector<std::byte>;

struct Symbol;
enum class HighFlags : std::uint_fast8_t;

struct StaticSymbol final {
  using Self = rq::StaticSymbol;

  rq::HighFlags flags;
  rq::Symbol *symbol_ptr;
};

struct StaticValue final {
  using Self = rq::StaticValue;
  using Kind = rq::StaticValueKind;

  rq::StaticValueKind _kind = rq::StaticValueKind::NONE;
  llvm::AlignedCharArrayUnion<rq::StaticSlice, rq::StaticInt, rq::StaticFloat,
                              rq::StaticDataArray, rq::StaticArray,
                              rq::LowAttribute, rq::HighAttribute>
      _data = {};

  explicit RQ_ALWAYS_INLINE StaticValue() = default;
  RQ_ALWAYS_INLINE StaticValue(const rq::StaticSymbol &symbol)
      : _kind(rq::StaticValueKind::SYMBOL) {
    this->getSymbol() = symbol;
  }
  RQ_ALWAYS_INLINE StaticValue(const rq::LowAttribute &attribute)
      : _kind(rq::StaticValueKind::LOW_ATTRIBUTE) {
    this->getLowAttribute() = attribute;
  }
  RQ_ALWAYS_INLINE StaticValue(const rq::HighAttribute &attribute)
      : _kind(rq::StaticValueKind::HIGH_ATTRIBUTE) {
    this->getHighAttribute() = attribute;
  }
  ~StaticValue() {
    switch (this->_kind) {
    case Kind::NONE:
      break;
    case Kind::INT: {
      rq::StaticInt &int_ = this->getInt();
      std::destroy_at(&int_);
    } break;
    case Kind::FLOAT: {
      rq::StaticFloat &float_ = this->getFloat();
      std::destroy_at(&float_);
    } break;
    case Kind::SLICE:
      break;
    case Kind::ARRAY: {
      rq::StaticArray &array = this->getArray();
      std::destroy_at(&array);
    } break;
    case Kind::DATA_ARRAY: {
      rq::StaticDataArray &data_array = this->getDataArray();
      std::destroy_at(&data_array);
    }
    default:
      break;
    }
  }

  [[nodiscard]] RQ_ALWAYS_INLINE Kind getKind() const { return this->_kind; }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticSlice &getSlice() {
    RQ_ASSERT(this->_kind == Kind::SLICE, "not slice");
    return rq::dereferencePtr(std::bit_cast<rq::StaticSlice *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticSlice &getSlice() const {
    RQ_ASSERT(this->_kind == Kind::SLICE, "not slice");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticSlice *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticInt &getInt() {
    RQ_ASSERT(this->_kind == Kind::INT, "not int");
    return rq::dereferencePtr(std::bit_cast<rq::StaticInt *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticInt &getInt() const {
    RQ_ASSERT(this->_kind == Kind::INT, "not int");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticInt *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticFloat &getFloat() {
    RQ_ASSERT(this->_kind == Kind::FLOAT, "not float");
    return rq::dereferencePtr(std::bit_cast<rq::StaticFloat *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticFloat &getFloat() const {
    RQ_ASSERT(this->_kind == Kind::FLOAT, "not float");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticFloat *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticArray &getArray() {
    RQ_ASSERT(this->_kind == Kind::ARRAY, "not array");
    return rq::dereferencePtr(std::bit_cast<rq::StaticArray *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticArray &getArray() const {
    RQ_ASSERT(this->_kind == Kind::ARRAY, "not array");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticArray *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticDataArray &getDataArray() {
    RQ_ASSERT(this->_kind == Kind::DATA_ARRAY, "not data array");
    return rq::dereferencePtr(
        std::bit_cast<rq::StaticDataArray *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticDataArray &
  getDataArray() const {
    RQ_ASSERT(this->_kind == Kind::DATA_ARRAY, "not array array");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticDataArray *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticSymbol &getSymbol() {
    RQ_ASSERT(this->_kind == Kind::SYMBOL, "not symbol");
    return rq::dereferencePtr(std::bit_cast<rq::StaticSymbol *>(&this->_data));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticSymbol &getSymbol() const {
    RQ_ASSERT(this->_kind == Kind::SYMBOL, "not symbol");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::StaticSymbol *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LowAttribute &
  getLowAttribute() const {
    RQ_ASSERT(this->_kind == Kind::LOW_ATTRIBUTE,
              "not low attribute");
    return rq::dereferencePtr(std::launder(
        std::bit_cast<const rq::LowAttribute *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowAttribute &
  getLowAttribute() {
    RQ_ASSERT(this->_kind == Kind::LOW_ATTRIBUTE,
              "not low attribute");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<rq::LowAttribute *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::HighAttribute &
  getHighAttribute() const {
    RQ_ASSERT(this->_kind == Kind::HIGH_ATTRIBUTE, "not high attribute");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<const rq::HighAttribute *>(&this->_data)));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::HighAttribute &getHighAttribute() {
    RQ_ASSERT(this->_kind == Kind::HIGH_ATTRIBUTE, "not high attribute");
    return rq::dereferencePtr(
        std::launder(std::bit_cast<rq::HighAttribute *>(&this->_data)));
  }
};

} // namespace rq