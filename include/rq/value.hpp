#pragma once

#include <rq/utility.hpp>
#include <rq/exact_vector.hpp>
#include <rq/generational_arena.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>
#include <new>
#include <cstddef>
#include <llvm/Support/AlignOf.h>

#include <bit>

namespace rq {

enum class ValueKind : std::uint32_t {
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

struct Value {
  using Self = rq::Value;

  rq::ValueKind _kind{};
  llvm::AlignedCharArrayUnion<llvm::APInt, llvm::APFloat, bool, llvm::StringRef,
                              rq::Gendex<rq::Value>,
                              rq::ExactVector<rq::Gendex<rq::Value>>, rq::Entity *>
      _data{};

  Value() = default;
  explicit Value(rq::ValueKind kind) : _kind(kind) {}
  ~Value() { this->release(); }

  Value(const Self &rhs) : _kind(rq::ValueKind::NONE) { this->_copyFrom(rhs); }
  Value(Self &&rhs) noexcept : _kind(rq::ValueKind::NONE) { this->_moveFrom(std::move(rhs)); }

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

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ValueKind getKind() const { return this->_kind; }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return this->_kind == rq::ValueKind::INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return this->_kind == rq::ValueKind::FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBool() const { return this->_kind == rq::ValueKind::BOOL; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsString() const { return this->_kind == rq::ValueKind::STRING; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointer() const { return this->_kind == rq::ValueKind::POINTER; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArray() const { return this->_kind == rq::ValueKind::ARRAY; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const { return this->_kind == rq::ValueKind::SYMBOL; }

  void setInteger(const llvm::APInt &v) {
    this->release();
    new (&this->_data) llvm::APInt(v);
    this->_kind = rq::ValueKind::INTEGER;
  }
  void setInteger(llvm::APInt &&v) {
    this->release();
    new (&this->_data) llvm::APInt(std::move(v));
    this->_kind = rq::ValueKind::INTEGER;
  }

  void setFloat(const llvm::APFloat &v) {
    this->release();
    new (&this->_data) llvm::APFloat(v);
    this->_kind = rq::ValueKind::FLOAT;
  }
  void setFloat(llvm::APFloat &&v) {
    this->release();
    new (&this->_data) llvm::APFloat(std::move(v));
    this->_kind = rq::ValueKind::FLOAT;
  }

  void setBool(bool b) {
    this->release();
    new (&this->_data) bool(b);
    this->_kind = rq::ValueKind::BOOL;
  }

  void setString(llvm::StringRef s) {
    this->release();
    new (&this->_data) llvm::StringRef(s);
    this->_kind = rq::ValueKind::STRING;
  }

  void setPointer(const rq::Gendex<rq::Value> &g) {
    this->release();
    new (&this->_data) rq::Gendex<rq::Value>(g);
    this->_kind = rq::ValueKind::POINTER;
  }

  void setArray(rq::Gendex<rq::Value> *data_ptr, std::size_t size) {
    this->release();
    using VecTy = rq::ExactVector<rq::Gendex<rq::Value>>;
    new (&this->_data) VecTy(std::bit_cast<VecTy::Element *>(data_ptr), size);
    this->_kind = rq::ValueKind::ARRAY;
  }

  void setSymbol(rq::Entity *e) {
    this->release();
    *std::bit_cast<rq::Entity **>(&this->_data) = e;
    this->_kind = rq::ValueKind::SYMBOL;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt *getInteger() {
    RQ_ASSERT(this->getIsInteger(), "Value is not an integer");
    return std::bit_cast<llvm::APInt *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt *getInteger() const {
    RQ_ASSERT(this->getIsInteger(), "Value is not an integer");
    return std::bit_cast<const llvm::APInt *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat *getFloat() {
    RQ_ASSERT(this->getIsFloat(), "Value is not a float");
    return std::bit_cast<llvm::APFloat *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat *getFloat() const {
    RQ_ASSERT(this->getIsFloat(), "Value is not a float");
    return std::bit_cast<const llvm::APFloat *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getBool() const {
    RQ_ASSERT(this->getIsBool(), "Value is not a bool");
    return *std::bit_cast<const bool *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getString() const {
    RQ_ASSERT(this->getIsString(), "Value is not a string");
    return *std::bit_cast<const llvm::StringRef *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Gendex<rq::Value> *getPointer() {
    RQ_ASSERT(this->getIsPointer(), "Value is not a pointer");
    return std::bit_cast<rq::Gendex<rq::Value> *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Gendex<rq::Value> *getPointer() const {
    RQ_ASSERT(this->getIsPointer(), "Value is not a pointer");
    return std::bit_cast<const rq::Gendex<rq::Value> *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExactVector<rq::Gendex<rq::Value>> *getArray() {
    RQ_ASSERT(this->getIsArray(), "Value is not an array");
    return std::bit_cast<rq::ExactVector<rq::Gendex<rq::Value>> *>(&this->_data);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ExactVector<rq::Gendex<rq::Value>> *getArray() const {
    RQ_ASSERT(this->getIsArray(), "Value is not an array");
    return std::bit_cast<const rq::ExactVector<rq::Gendex<rq::Value>> *>(&this->_data);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getSymbol() const {
    RQ_ASSERT(this->getIsSymbol(), "Value is not a symbol");
    return *std::bit_cast<rq::Entity *const *>(&this->_data);
  }

  void release() {
    switch (this->_kind) {
    case rq::ValueKind::INTEGER:
      std::bit_cast<llvm::APInt *>(&this->_data)->~APInt();
      break;
    case rq::ValueKind::FLOAT:
      std::bit_cast<llvm::APFloat *>(&this->_data)->~APFloat();
      break;
    case rq::ValueKind::ARRAY:
      std::bit_cast<rq::ExactVector<rq::Gendex<rq::Value>> *>(&this->_data)
          ->~ExactVector<rq::Gendex<rq::Value>>();
      break;
    case rq::ValueKind::POINTER:
    case rq::ValueKind::BOOL:
    case rq::ValueKind::STRING:
    case rq::ValueKind::SYMBOL:
    case rq::ValueKind::NONE:
    case rq::ValueKind::INDETERMINATE:
      break;
    }
    this->_kind = rq::ValueKind::NONE;
  }

  void _copyFrom(const Self &rhs) {
    switch (rhs._kind) {
    case rq::ValueKind::INTEGER:
      new (&this->_data) llvm::APInt(*rhs.getInteger());
      this->_kind = rq::ValueKind::INTEGER;
      break;
    case rq::ValueKind::FLOAT:
      new (&this->_data) llvm::APFloat(*rhs.getFloat());
      this->_kind = rq::ValueKind::FLOAT;
      break;
    case rq::ValueKind::BOOL:
      new (&this->_data) bool(rhs.getBool());
      this->_kind = rq::ValueKind::BOOL;
      break;
    case rq::ValueKind::STRING:
      new (&this->_data) llvm::StringRef(rhs.getString());
      this->_kind = rq::ValueKind::STRING;
      break;
    case rq::ValueKind::POINTER:
      new (&this->_data) rq::Gendex<rq::Value>(*rhs.getPointer());
      this->_kind = rq::ValueKind::POINTER;
      break;
    case rq::ValueKind::ARRAY:
      new (&this->_data) rq::ExactVector<rq::Gendex<rq::Value>>(*rhs.getArray());
      this->_kind = rq::ValueKind::ARRAY;
      break;
    case rq::ValueKind::SYMBOL:
      *std::bit_cast<rq::Entity **>(&this->_data) = rhs.getSymbol();
      this->_kind = rq::ValueKind::SYMBOL;
      break;
    default:
      this->_kind = rq::ValueKind::NONE;
      break;
    }
  }

  void _moveFrom(Self &&rhs) {
    switch (rhs._kind) {
    case rq::ValueKind::INTEGER:
      new (&this->_data) llvm::APInt(std::move(*rhs.getInteger()));
      this->_kind = rq::ValueKind::INTEGER;
      break;
    case rq::ValueKind::FLOAT:
      new (&this->_data) llvm::APFloat(std::move(*rhs.getFloat()));
      this->_kind = rq::ValueKind::FLOAT;
      break;
    case rq::ValueKind::BOOL:
      new (&this->_data) bool(rhs.getBool());
      this->_kind = rq::ValueKind::BOOL;
      break;
    case rq::ValueKind::STRING:
      new (&this->_data) llvm::StringRef(rhs.getString());
      this->_kind = rq::ValueKind::STRING;
      break;
    case rq::ValueKind::POINTER:
      new (&this->_data) rq::Gendex<rq::Value>(std::move(*rhs.getPointer()));
      this->_kind = rq::ValueKind::POINTER;
      break;
    case rq::ValueKind::ARRAY:
      new (&this->_data) rq::ExactVector<rq::Gendex<rq::Value>>(std::move(*rhs.getArray()));
      this->_kind = rq::ValueKind::ARRAY;
      break;
    case rq::ValueKind::SYMBOL:
      *std::bit_cast<rq::Entity **>(&this->_data) = rhs.getSymbol();
      this->_kind = rq::ValueKind::SYMBOL;
      break;
    default:
      this->_kind = rq::ValueKind::NONE;
      break;
    }
    rhs.release();
  }
};

} // namespace rq
