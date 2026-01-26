#pragma once

#include <rq/exact_vector.hpp>
#include <rq/generational_arena.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/AlignOf.h>

#include <cstddef>
#include <new>
#include <bit>

namespace rq {

enum class SymbolicValueKind : std::uint32_t {
  NONE,
  INDETERMINATE,
  POINTER,
  INTEGER,
  FLOAT,
  SYMBOL,
  BOOL,
  STRING,
  ARRAY
};

struct Symbol;
struct TypeSymbol;
struct Entity;
struct Expression;

struct SymbolicValue {
  using Self = rq::SymbolicValue;

  rq::SymbolicValueKind _kind{rq::SymbolicValueKind::NONE};
  llvm::AlignedCharArrayUnion<llvm::APInt, llvm::APFloat, bool, llvm::StringRef,
                              rq::Gendex<rq::SymbolicValue>,
                              rq::ExactVector<rq::Gendex<rq::SymbolicValue>>,
                              rq::Entity *>
      _data{};
  bool _is_platform_dependent : 1 {false};

  SymbolicValue() = default;
  ~SymbolicValue() { this->release(); }
  SymbolicValue(const Self &rhs) : _kind(rq::SymbolicValueKind::NONE) {
    this->_copyFrom(rhs);
  }
  SymbolicValue(Self &&rhs) noexcept : _kind(rq::SymbolicValueKind::NONE) {
    this->_moveFrom(std::move(rhs));
  }
  Self &operator=(const Self &rhs) {
    if (this != &rhs) {
      this->release();
      this->_copyFrom(rhs);
    }
    return *this;
  }
  Self &operator=(Self &&rhs) noexcept {
    if (this != &rhs) {
      this->release();
      this->_moveFrom(std::move(rhs));
    }
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolicValueKind getKind() const {
    return this->_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformDependent() const {
    return this->_is_platform_dependent;
  }
  void setIsPlatformDependent() { this->_is_platform_dependent = true; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return this->_kind == rq::SymbolicValueKind::INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return this->_kind == rq::SymbolicValueKind::FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBool() const {
    return this->_kind == rq::SymbolicValueKind::BOOL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsString() const {
    return this->_kind == rq::SymbolicValueKind::STRING;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointer() const {
    return this->_kind == rq::SymbolicValueKind::POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArray() const {
    return this->_kind == rq::SymbolicValueKind::ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return this->_kind == rq::SymbolicValueKind::SYMBOL;
  }
  void setInteger(const llvm::APInt &v) {
    this->release();
    new (&this->_data) llvm::APInt(v);
    this->_kind = rq::SymbolicValueKind::INTEGER;
  }
  void setInteger(llvm::APInt &&v) {
    this->release();
    new (&this->_data) llvm::APInt(std::move(v));
    this->_kind = rq::SymbolicValueKind::INTEGER;
  }
  void setFloat(const llvm::APFloat &v) {
    this->release();
    new (&this->_data) llvm::APFloat(v);
    this->_kind = rq::SymbolicValueKind::FLOAT;
  }
  void setFloat(llvm::APFloat &&v) {
    this->release();
    new (&this->_data) llvm::APFloat(std::move(v));
    this->_kind = rq::SymbolicValueKind::FLOAT;
  }
  void setBool(bool b) {
    this->release();
    new (&this->_data) bool(b);
    this->_kind = rq::SymbolicValueKind::BOOL;
  }
  void setString(llvm::StringRef s) {
    this->release();
    new (&this->_data) llvm::StringRef(s);
    this->_kind = rq::SymbolicValueKind::STRING;
  }
  void setPointer(const rq::Gendex<rq::SymbolicValue> &g) {
    this->release();
    new (&this->_data) rq::Gendex<rq::SymbolicValue>(g);
    this->_kind = rq::SymbolicValueKind::POINTER;
  }
  void setArray(rq::Gendex<rq::SymbolicValue> *data_ptr, std::size_t size) {
    this->release();
    using VecTy = rq::ExactVector<rq::Gendex<rq::SymbolicValue>>;
    new (&this->_data) VecTy(std::bit_cast<VecTy::Element *>(data_ptr), size);
    this->_kind = rq::SymbolicValueKind::ARRAY;
  }
  void setSymbol(rq::Entity *e) {
    this->release();
    *std::bit_cast<rq::Entity **>(&this->_data) = e;
    this->_kind = rq::SymbolicValueKind::SYMBOL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt *getInteger() {
    RQ_ASSERT(this->getIsInteger(), "SymbolicValue is not an integer");
    return std::bit_cast<llvm::APInt *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt *getInteger() const {
    RQ_ASSERT(this->getIsInteger(), "SymbolicValue is not an integer");
    return std::bit_cast<const llvm::APInt *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat *getFloat() {
    RQ_ASSERT(this->getIsFloat(), "SymbolicValue is not a float");
    return std::bit_cast<llvm::APFloat *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat *getFloat() const {
    RQ_ASSERT(this->getIsFloat(), "SymbolicValue is not a float");
    return std::bit_cast<const llvm::APFloat *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getBool() const {
    RQ_ASSERT(this->getIsBool(), "SymbolicValue is not a bool");
    return *std::bit_cast<const bool *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getString() const {
    RQ_ASSERT(this->getIsString(), "SymbolicValue is not a string");
    return *std::bit_cast<const llvm::StringRef *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Gendex<rq::SymbolicValue> *getPointer() {
    RQ_ASSERT(this->getIsPointer(), "SymbolicValue is not a pointer");
    return std::bit_cast<rq::Gendex<rq::SymbolicValue> *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Gendex<rq::SymbolicValue> *
  getPointer() const {
    RQ_ASSERT(this->getIsPointer(), "SymbolicValue is not a pointer");
    return std::bit_cast<const rq::Gendex<rq::SymbolicValue> *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ExactVector<rq::Gendex<rq::SymbolicValue>> *
      getArray() {
    RQ_ASSERT(this->getIsArray(), "SymbolicValue is not an array");
    return std::bit_cast<rq::ExactVector<rq::Gendex<rq::SymbolicValue>> *>(
        &this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const
      rq::ExactVector<rq::Gendex<rq::SymbolicValue>> *
      getArray() const {
    RQ_ASSERT(this->getIsArray(), "SymbolicValue is not an array");
    return std::bit_cast<
        const rq::ExactVector<rq::Gendex<rq::SymbolicValue>> *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getSymbol() const {
    RQ_ASSERT(this->getIsSymbol(), "SymbolicValue is not a symbol");
    return *std::bit_cast<rq::Entity *const *>(&this->_data);
  }
  void release() {
    switch (this->_kind) {
    case rq::SymbolicValueKind::INTEGER:
      std::bit_cast<llvm::APInt *>(&this->_data)->~APInt();
      break;
    case rq::SymbolicValueKind::FLOAT:
      std::bit_cast<llvm::APFloat *>(&this->_data)->~APFloat();
      break;
    case rq::SymbolicValueKind::ARRAY:
      std::bit_cast<rq::ExactVector<rq::Gendex<rq::SymbolicValue>> *>(
          &this->_data)
          ->~ExactVector<rq::Gendex<rq::SymbolicValue>>();
      break;
    case rq::SymbolicValueKind::POINTER:
    case rq::SymbolicValueKind::BOOL:
    case rq::SymbolicValueKind::STRING:
    case rq::SymbolicValueKind::SYMBOL:
    case rq::SymbolicValueKind::NONE:
    case rq::SymbolicValueKind::INDETERMINATE:
      break;
    }
    this->_kind = rq::SymbolicValueKind::NONE;
  }
  void _copyFrom(const Self &rhs) {
    switch (rhs._kind) {
    case rq::SymbolicValueKind::INTEGER:
      new (&this->_data) llvm::APInt(*rhs.getInteger());
      this->_kind = rq::SymbolicValueKind::INTEGER;
      break;
    case rq::SymbolicValueKind::FLOAT:
      new (&this->_data) llvm::APFloat(*rhs.getFloat());
      this->_kind = rq::SymbolicValueKind::FLOAT;
      break;
    case rq::SymbolicValueKind::BOOL:
      new (&this->_data) bool(rhs.getBool());
      this->_kind = rq::SymbolicValueKind::BOOL;
      break;
    case rq::SymbolicValueKind::STRING:
      new (&this->_data) llvm::StringRef(rhs.getString());
      this->_kind = rq::SymbolicValueKind::STRING;
      break;
    case rq::SymbolicValueKind::POINTER:
      new (&this->_data) rq::Gendex<rq::SymbolicValue>(*rhs.getPointer());
      this->_kind = rq::SymbolicValueKind::POINTER;
      break;
    case rq::SymbolicValueKind::ARRAY:
      new (&this->_data)
          rq::ExactVector<rq::Gendex<rq::SymbolicValue>>(*rhs.getArray());
      this->_kind = rq::SymbolicValueKind::ARRAY;
      break;
    case rq::SymbolicValueKind::SYMBOL:
      *std::bit_cast<rq::Entity **>(&this->_data) = rhs.getSymbol();
      this->_kind = rq::SymbolicValueKind::SYMBOL;
      break;
    default:
      this->_kind = rq::SymbolicValueKind::NONE;
      break;
    }
  }
  void _moveFrom(Self &&rhs) {
    switch (rhs._kind) {
    case rq::SymbolicValueKind::INTEGER:
      new (&this->_data) llvm::APInt(std::move(*rhs.getInteger()));
      this->_kind = rq::SymbolicValueKind::INTEGER;
      break;
    case rq::SymbolicValueKind::FLOAT:
      new (&this->_data) llvm::APFloat(std::move(*rhs.getFloat()));
      this->_kind = rq::SymbolicValueKind::FLOAT;
      break;
    case rq::SymbolicValueKind::BOOL:
      new (&this->_data) bool(rhs.getBool());
      this->_kind = rq::SymbolicValueKind::BOOL;
      break;
    case rq::SymbolicValueKind::STRING:
      new (&this->_data) llvm::StringRef(rhs.getString());
      this->_kind = rq::SymbolicValueKind::STRING;
      break;
    case rq::SymbolicValueKind::POINTER:
      new (&this->_data)
          rq::Gendex<rq::SymbolicValue>(std::move(*rhs.getPointer()));
      this->_kind = rq::SymbolicValueKind::POINTER;
      break;
    case rq::SymbolicValueKind::ARRAY:
      new (&this->_data) rq::ExactVector<rq::Gendex<rq::SymbolicValue>>(
          std::move(*rhs.getArray()));
      this->_kind = rq::SymbolicValueKind::ARRAY;
      break;
    case rq::SymbolicValueKind::SYMBOL:
      *std::bit_cast<rq::Entity **>(&this->_data) = rhs.getSymbol();
      this->_kind = rq::SymbolicValueKind::SYMBOL;
      break;
    default:
      this->_kind = rq::SymbolicValueKind::NONE;
      break;
    }
    rhs.release();
  }
};

struct SymbolicExecutionEngine final {
  using Self = rq::SymbolicExecutionEngine;

  rq::GenerationalArena<rq::SymbolicValue> _memory{};
  std::int64_t _initial_steps_left;
  std::int64_t _steps_left;

  void _resetStepsLeft() {
    this->_steps_left = this->_initial_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::int64_t getStepsLeft() const {
    return this->_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::int64_t getInitialStepsLeft() const {
    return this->_initial_steps_left;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStepsLeft() const {
    return this->_steps_left > 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool _decrementStep() {
    RQ_ASSERT(this->getHasStepsLeft(), "no steps left");
    this->_steps_left--;
    return this->getHasStepsLeft();
  }
  [[nodiscard]] llvm::StringRef evaluateName(rq::Expression &expression);
};

} // namespace rq