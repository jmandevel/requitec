#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/StringSaver.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

struct Scope;
struct Module;

// TODO static values. only care about dynamic runtime for now (types and
// symbols all that are needed).

enum class ValueKind : std::uint_fast8_t {
  // builtin simple types
  EXPRESSION_TYPE,
  VOID_TYPE,
  NULL_TYPE,
  NO_RETURN_TYPE,
  VARIADIC_ARGUMENTS_TYPE,
  BOOLEAN_TYPE,
  UTF8_TYPE,
  BFLOAT16_TYPE,
  BINARY16_TYPE,
  BINARY32_TYPE,
  BINARY64_TYPE,
  BINARY128_TYPE,

  // builtin depth types
  WORD_TYPE,
  SIGNED_TYPE,
  UNSIGNED_TYPE,

  // simple subtypes
  RANGE_TYPE,
  REFERENCE_TYPE,
  POINTER_TYPE,
  FAT_POINTER_TYPE,
  ARRAY_TYPE,
  TWO_PART_SEQUENCE_TYPE,
  THREE_PART_SEQUENCE_TYPE,

  // data symbols
  MODULE,
  TOP_SCOPE,
  TABLE,
  SCOPE,
  OBJECT,
  ENUMERATION_TYPE,
  ENUMERATOR,
  LAYOUT,
  TUPLE,
  SIGNATURE,
  VARIABLE,
  FUNCTION,
  METHOD,
  ENTRY_POINT,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  CONSTRUCTOR,
  DESTRUCTOR,
  RANGER
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ValueKind kind) {
  switch (kind) {
  case rq::ValueKind::VOID:
    return "void";
  case rq::ValueKind::NULL_TYPE:
    return "null-type";
  case rq::ValueKind::NO_RETURN:
    return "no-return";
  case rq::ValueKind::VARIADIC_ARGUMENTS_TYPE:
    return "variadic-arguments-type";
  case rq::ValueKind::BOOLEAN:
    return "boolean";
  case rq::ValueKind::UTF8:
    return "utf8";
  case rq::ValueKind::BFLOAT16:
    return "bfloat16";
  case rq::ValueKind::BINARY16:
    return "binary16";
  case rq::ValueKind::BINARY32:
    return "binary32";
  case rq::ValueKind::BINARY64:
    return "binary64";
  case rq::ValueKind::BINARY128:
    return "binary128";
  case rq::ValueKind::WORD:
    return "word";
  case rq::ValueKind::SIGNED:
    return "signed";
  case rq::ValueKind::UNSIGNED:
    return "unsigned";
  case rq::ValueKind::MODULE:
    return "module";
  case rq::ValueKind::TOP_SCOPE:
    return "top-scope";
  case rq::ValueKind::TABLE:
    return "table";
  case rq::ValueKind::SCOPE:
    return "scope";
  case rq::ValueKind::OBJECT:
    return "object";
  case rq::ValueKind::ENUMERATION:
    return "enumeration";
  case rq::ValueKind::ENUMERATOR:
    return "enumerator";
  case rq::ValueKind::LAYOUT:
    return "layout";
  case rq::ValueKind::VARIABLE:
    return "variable";
  case rq::ValueKind::FUNCTION:
    return "function";
  case rq::ValueKind::METHOD:
    return "method";
  case rq::ValueKind::ENTRY_POINT:
    return "entry-point";
  }
  RQ_UNREACHABLE();
}

enum class ValueFlags : std::uint_fast8_t {
  NONE = 0,
  TYPE = rq::getBit(1),
  SYMBOL = rq::getBit(2),
  BUILTIN_SIMPLE_TYPE = rq::getBit(3),
  BUILTIN_DEPTHED_TYPE = rq::getBit(4),
  SCOPE = rq::getBit(5),
  PROCEDURE = rq::getBit(6)
};

template <> struct is_flags<rq::ValueFlags> : std::true_type {};

static constexpr unsigned NO_DATA_SYMBOL_COUNT = 11;

struct VoidType;
struct NullType;
struct NoReturnType;
struct VariadicArgumentsType;
struct BooleanType;
struct Utf8Type;
struct Bfloat16Type;
struct Binary16Type;
struct Binary32Type;
struct Binary64Type;
struct Binary128Type;
struct WordType;
struct UnsignedType;
struct SignedType;
struct BuiltinDepthedType;

struct ContextCache {
  using Self = ContextCache;

  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs{};
  rq::VoidType *_void_type{nullptr};
  rq::NullType *_null_type{nullptr};
  rq::NoReturnType *_no_return_type{nullptr};
  rq::VariadicArgumentsType *_variadic_arguments_type{nullptr};
  rq::BooleanType *_boolean_type{nullptr};
  rq::Utf8Type *_utf8_type{nullptr};
  rq::Bfloat16Type *_bfloat16_type{nullptr};
  rq::Binary16Type *_binary16_type{nullptr};
  rq::Binary32Type *_binary32_type{nullptr};
  rq::Binary64Type *_binary64_type{nullptr};
  rq::Binary128Type *_binary128_type{nullptr};
  llvm::FoldingSet<rq::BuiltinDepthedType> _builtin_depthed_types{};

  ContextCache() = default;
  ContextCache(const Self &) = delete;
  ContextCache(Self &&) = delete;
  ~ContextCache() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] TypeParam &allocateValue(ArgNParam &&...arg_n);
  [[nodiscard]] llvm::StringRef saveString(llvm::Twine twine);
  [[nodiscard]] rq::Expression &acquireExpression();
  void discardExpression(rq::Expression &expression);
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] rq::VoidType &getVoidType();
  [[nodiscard]] rq::NullType &getNullType();
  [[nodiscard]] rq::NoReturnType &getNoReturnType();
  [[nodiscard]] rq::VariadicArgumentsType &getVariadicArgumentsType();
  [[nodiscard]] rq::BooleanType &getBooleanType();
  [[nodiscard]] rq::Utf8Type &getUtf8Type();
  [[nodiscard]] rq::Bfloat16Type &getBfloat16Type();
  [[nodiscard]] rq::Binary16Type &getBinary16Type();
  [[nodiscard]] rq::Binary32Type &getBinary32Type();
  [[nodiscard]] rq::Binary64Type &getBinary64Type();
  [[nodiscard]] rq::Binary128Type &getBinary128Type();
  [[nodiscard]] rq::WordType &getWordType(unsigned bit_depth);
  [[nodiscard]] rq::UnsignedType &getUnsignedType(unsigned bit_depth);
  [[nodiscard]] rq::SignedType &getSignedType(unsigned bit_depth);
  [[nodiscard]] rq::BuiltinDepthedType &
  _getOrInsertBuiltinDepthType(rq::ValueKind kind, unsigned depth);
};

struct Value {
  using Self = rq::Value;

  rq::ValueKind _kind;

  Value(rq::ValueKind kind) : _kind(kind) {}
  Value(const Self &) = delete;
  Value(Self &&) = delete;
  virtual ~Value() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ValueKind getKind() const {
    return this->_kind;
  }
};

} // namespace rq
namespace llvm {

template <> struct isa_impl<rq::Symbol, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() != rq::ValueKind::MODULE;
  }
};

template <> struct isa_impl<rq::Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind >= rq::ValueKind::VOID && kind <= rq::ValueKind::UNSIGNED;
  }
};

template <> struct isa_impl<rq::BuiltinType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind >= rq::ValueKind::VOID && kind <= rq::ValueKind::UNSIGNED;
  }
};

template <> struct isa_impl<rq::BuiltinSimpleType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind >= rq::ValueKind::VOID && kind <= rq::ValueKind::BINARY128;
  }
};

template <> struct isa_impl<rq::BuiltinDepthedType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind >= rq::ValueKind::WORD && kind <= rq::ValueKind::UNSIGNED;
  }
};

template <> struct isa_impl<rq::VoidType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::VOID;
  }
};

template <> struct isa_impl<rq::NullType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::NULL_TYPE;
  }
};

template <> struct isa_impl<rq::NoReturnType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::NO_RETURN;
  }
};

template <> struct isa_impl<rq::VariadicArgumentsType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::VARIADIC_ARGUMENTS_TYPE;
  }
};

template <> struct isa_impl<rq::BooleanType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BOOLEAN;
  }
};

template <> struct isa_impl<rq::Utf8Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::UTF8;
  }
};

template <> struct isa_impl<rq::Bfloat16Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BFLOAT16;
  }
};

template <> struct isa_impl<rq::Binary16Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BINARY16;
  }
};

template <> struct isa_impl<rq::Binary32Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BINARY32;
  }
};

template <> struct isa_impl<rq::Binary64Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BINARY64;
  }
};

template <> struct isa_impl<rq::Binary128Type, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::BINARY128;
  }
};

template <> struct isa_impl<rq::WordType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::WORD;
  }
};

template <> struct isa_impl<rq::SignedType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::SIGNED;
  }
};

template <> struct isa_impl<rq::UnsignedType, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::UNSIGNED;
  }
};

template <> struct isa_impl<rq::Scope, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind >= rq::ValueKind::TOP_SCOPE && kind <= rq::ValueKind::LAYOUT;
  }
};

template <> struct isa_impl<rq::Procedure, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    auto kind = val.getKind();
    return kind == rq::ValueKind::FUNCTION || kind == rq::ValueKind::METHOD ||
           kind == rq::ValueKind::ENTRY_POINT;
  }
};

template <> struct isa_impl<rq::Module, rq::Value> {
  static inline bool doit(const rq::Value &val) {
    return val.getKind() == rq::ValueKind::MODULE;
  }
};

} // namespace llvm
namespace rq {

struct Symbol : public rq::Value {
  using Self = rq::Symbol;

  Symbol(rq::ValueKind kind) : rq::Value(kind) {}
  Symbol(const Self &) = delete;
  Symbol(Self &&) = delete;
  virtual ~Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Type : public rq::Symbol {
  using Self = rq::Type;

  Type(rq::ValueKind kind) : rq::Symbol(kind) {}
  Type(const Self &) = delete;
  Type(Self &&) = delete;
  virtual ~Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BuiltinType : public rq::Type {
  using Self = rq::Type;

  BuiltinType(rq::ValueKind kind) : rq::Type(kind) {}
  BuiltinType(const Self &) = delete;
  BuiltinType(Self &&) = delete;
  virtual ~BuiltinType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BuiltinSimpleType : public rq::BuiltinType {
  using Self = rq::BuiltinSimpleType;

  BuiltinSimpleType(rq::ValueKind kind) : rq::BuiltinType(kind) {}
  BuiltinSimpleType(const Self &) = delete;
  BuiltinSimpleType(Self &&) = delete;
  virtual ~BuiltinSimpleType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VoidType : public rq::BuiltinSimpleType {
  using Self = rq::VoidType;

  VoidType() : rq::BuiltinSimpleType(rq::ValueKind::VOID) {}
  VoidType(const Self &) = delete;
  VoidType(Self &&) = delete;
  virtual ~VoidType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NullType : public rq::BuiltinSimpleType {
  using Self = rq::NullType;

  NullType() : rq::BuiltinSimpleType(rq::ValueKind::NULL_TYPE) {}
  NullType(const Self &) = delete;
  NullType(Self &&) = delete;
  virtual ~NullType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NoReturnType : public rq::BuiltinSimpleType {
  using Self = rq::NullType;

  NoReturnType() : rq::BuiltinSimpleType(rq::ValueKind::NO_RETURN) {}
  NoReturnType(const Self &) = delete;
  NoReturnType(Self &&) = delete;
  virtual ~NoReturnType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VariadicArgumentsType : public rq::BuiltinSimpleType {
  using Self = rq::NullType;

  VariadicArgumentsType()
      : rq::BuiltinSimpleType(rq::ValueKind::VARIADIC_ARGUMENTS_TYPE) {}
  VariadicArgumentsType(const Self &) = delete;
  VariadicArgumentsType(Self &&) = delete;
  virtual ~VariadicArgumentsType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BooleanType : public rq::BuiltinSimpleType {
  using Self = rq::NullType;

  BooleanType() : rq::BuiltinSimpleType(rq::ValueKind::BOOLEAN) {}
  BooleanType(const Self &) = delete;
  BooleanType(Self &&) = delete;
  virtual ~BooleanType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Utf8Type : public rq::BuiltinSimpleType {
  using Self = rq::NullType;

  Utf8Type() : rq::BuiltinSimpleType(rq::ValueKind::UTF8) {}
  Utf8Type(const Self &) = delete;
  Utf8Type(Self &&) = delete;
  virtual ~Utf8Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Bfloat16Type : public rq::BuiltinSimpleType {
  using Self = rq::Bfloat16Type;

  Bfloat16Type() : rq::BuiltinSimpleType(rq::ValueKind::BFLOAT16) {}
  Bfloat16Type(const Self &) = delete;
  Bfloat16Type(Self &&) = delete;
  virtual ~Bfloat16Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary16Type : public rq::BuiltinSimpleType {
  using Self = rq::Binary16Type;

  Binary16Type() : rq::BuiltinSimpleType(rq::ValueKind::BINARY16) {}
  Binary16Type(const Self &) = delete;
  Binary16Type(Self &&) = delete;
  virtual ~Binary16Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary32Type : public rq::BuiltinSimpleType {
  using Self = rq::Binary16Type;

  Binary32Type() : rq::BuiltinSimpleType(rq::ValueKind::BINARY32) {}
  Binary32Type(const Self &) = delete;
  Binary32Type(Self &&) = delete;
  virtual ~Binary32Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary64Type : public rq::BuiltinSimpleType {
  using Self = rq::Binary64Type;

  Binary64Type() : rq::BuiltinSimpleType(rq::ValueKind::BINARY64) {}
  Binary64Type(const Self &) = delete;
  Binary64Type(Self &&) = delete;
  virtual ~Binary64Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary128Type : public rq::BuiltinSimpleType {
  using Self = rq::Binary64Type;

  Binary128Type() : rq::BuiltinSimpleType(rq::ValueKind::BINARY128) {}
  Binary128Type(const Self &) = delete;
  Binary128Type(Self &&) = delete;
  virtual ~Binary128Type() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BuiltinDepthedType : public rq::BuiltinType,
                            public llvm::FoldingSetNode {
  using Self = rq::BuiltinDepthedType;

  unsigned _bit_depth;

  BuiltinDepthedType(rq::ValueKind kind, unsigned bit_depth)
      : rq::BuiltinType(kind), _bit_depth(bit_depth) {}
  BuiltinDepthedType(const Self &) = delete;
  BuiltinDepthedType(Self &&) = delete;
  virtual ~BuiltinDepthedType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getBitDepth() const {
    return this->_bit_depth;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
    id.AddInteger(this->_bit_depth);
  }
};

struct WordType : public rq::BuiltinDepthedType {
  using Self = WordType;

  WordType(unsigned bit_depth)
      : rq::BuiltinDepthedType(rq::ValueKind::WORD, bit_depth) {}
  WordType(const Self &) = delete;
  WordType(Self &&) = delete;
  virtual ~WordType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct UnsignedType : public rq::BuiltinDepthedType {
  using Self = WordType;

  UnsignedType(unsigned bit_depth)
      : rq::BuiltinDepthedType(rq::ValueKind::UNSIGNED, bit_depth) {}
  UnsignedType(const Self &) = delete;
  UnsignedType(Self &&) = delete;
  virtual ~UnsignedType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignedType : public rq::BuiltinDepthedType {
  using Self = WordType;

  SignedType(unsigned bit_depth)
      : rq::BuiltinDepthedType(rq::ValueKind::SIGNED, bit_depth) {}
  SignedType(const Self &) = delete;
  SignedType(Self &&) = delete;
  virtual ~SignedType() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScopeNode;
struct ScopeEntry;
struct ScopeEntryIterator;
struct ConstScopeEntryIterator;

struct ScopeEntry final {
  using Self = rq::ScopeEntry;

  llvm::PointerUnion<rq::Symbol *, rq::ScopeNode *> _ptr_union{nullptr};

  RQ_ALWAYS_INLINE ScopeEntry() = default;
  RQ_ALWAYS_INLINE ScopeEntry(rq::Symbol &symbol) : _ptr_union(&symbol) {}
  RQ_ALWAYS_INLINE ScopeEntry(rq::ScopeNode &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~ScopeEntry() = default;
  RQ_ALWAYS_INLINE ScopeEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ScopeEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return llvm::isa<rq::Symbol *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeNode() const {
    return llvm::isa<rq::ScopeNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(llvm::cast<rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(llvm::cast<rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeNode &getScopeNode() {
    return rq::dereferencePtr(llvm::cast<rq::ScopeNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeNode &getScopeNode() const {
    return rq::dereferencePtr(llvm::cast<rq::ScopeNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntryIterator begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntryIterator end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cend() const;
};

struct ConstScopeEntry final {
  using Self = rq::ConstScopeEntry;

  llvm::PointerUnion<const rq::Symbol *, const rq::ScopeNode *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE ConstScopeEntry() = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::ScopeEntry &rhs)
      : _ptr_union(
            std::bit_cast<
                llvm::PointerUnion<const rq::Symbol *, const rq::ScopeNode *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstScopeEntry(rq::ScopeEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::Symbol &symbol)
      : _ptr_union(&symbol) {}
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::ScopeNode &node)
      : _ptr_union(&node) {}
  ~ConstScopeEntry() = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::ScopeEntry &rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::ScopeEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return llvm::isa<const rq::Symbol *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeNode() const {
    return llvm::isa<const rq::ScopeNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(llvm::cast<const rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeNode &getScopeNode() const {
    return rq::dereferencePtr(
        llvm::cast<const rq::ScopeNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cend() const;
};

struct ScopeNode final {
  using Self = rq::ScopeNode;

  rq::Symbol *_symbol_ptr{nullptr};
  rq::ScopeEntry _scope_entry{};

  RQ_ALWAYS_INLINE ScopeNode() = default;
  RQ_ALWAYS_INLINE ScopeNode(rq::Symbol &symbol_a, rq::Symbol &symbol_b)
      : _symbol_ptr(&symbol_a), _scope_entry(symbol_b) {}
  RQ_ALWAYS_INLINE ScopeNode(rq::Symbol &symbol, rq::ScopeNode &node)
      : _symbol_ptr(&symbol), _scope_entry(node) {}
  RQ_ALWAYS_INLINE ScopeNode(rq::Symbol &symbol, const rq::ScopeEntry &entry)
      : _symbol_ptr(&symbol), _scope_entry(entry) {}
  ScopeNode(const Self &) = delete;
  ScopeNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~ScopeNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSymbol() const {
    return this->_symbol_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeEntry() const {
    return !this->_scope_entry.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntry &getScopeEntry() {
    return this->_scope_entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeEntry &getScopeEntry() const {
    return this->_scope_entry;
  }
};

struct ScopeEntryIterator final {
  using Self = rq::ScopeEntryIterator;
  using value_type = rq::Symbol;
  using reference = rq::Symbol &;
  using pointer = rq::Symbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ScopeEntry _entry;

  RQ_ALWAYS_INLINE ScopeEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit ScopeEntryIterator(rq::ScopeEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsSymbol()) {
      this->_entry = rq::ScopeEntry();
    } else if (this->_entry.getIsScopeNode()) {
      this->_entry = rq::ScopeEntry(this->_entry.getScopeNode());
    } else {
      RQ_UNREACHABLE();
    }
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &operator*() {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &operator*() const {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *operator->() {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *operator->() const {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

struct ConstScopeEntryIterator final {
  using Self = rq::ConstScopeEntryIterator;
  using value_type = const rq::Symbol;
  using reference = const rq::Symbol &;
  using pointer = rq::Symbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ConstScopeEntry _entry;

  RQ_ALWAYS_INLINE ConstScopeEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstScopeEntryIterator(const rq::ScopeEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE explicit ConstScopeEntryIterator(
      const rq::ConstScopeEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsSymbol()) {
      this->_entry = rq::ConstScopeEntry();
    } else if (this->_entry.getIsScopeNode()) {
      this->_entry = rq::ConstScopeEntry(this->_entry.getScopeNode());
    } else {
      RQ_UNREACHABLE();
    }
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &operator*() const {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *operator->() const {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

rq::ScopeEntryIterator ScopeEntry::begin() {
  return rq::ScopeEntryIterator(*this);
}

rq::ScopeEntryIterator ScopeEntry::end() { return rq::ScopeEntryIterator(); }

rq::ConstScopeEntryIterator ScopeEntry::begin() const {
  return rq::ConstScopeEntryIterator(*this);
}

rq::ConstScopeEntryIterator ScopeEntry::end() const {
  return rq::ConstScopeEntryIterator();
}

rq::ConstScopeEntryIterator ScopeEntry::cbegin() const {
  return rq::ConstScopeEntryIterator(*this);
}

rq::ConstScopeEntryIterator ScopeEntry::cend() const {
  return rq::ConstScopeEntryIterator();
}

rq::ConstScopeEntryIterator ConstScopeEntry::begin() const {
  return rq::ConstScopeEntryIterator(*this);
}

rq::ConstScopeEntryIterator ConstScopeEntry::end() const {
  return rq::ConstScopeEntryIterator();
}

rq::ConstScopeEntryIterator ConstScopeEntry::cbegin() const {
  return rq::ConstScopeEntryIterator(*this);
}

rq::ConstScopeEntryIterator ConstScopeEntry::cend() const {
  return rq::ConstScopeEntryIterator();
}

struct Scope : rq::Symbol {
  using Self = rq::Scope;

  llvm::SmallDenseMap<llvm::StringRef, rq::ScopeEntry> _named_values{};
  rq::ScopeEntry _unamed_values{};

  Scope() : rq::Symbol(rq::ValueKind::SCOPE) {}
  Scope(rq::ValueKind kind) : rq::Symbol(kind) {}
  Scope(const Self &) = delete;
  Scope(Self &&) = delete;
  ~Scope() override {
    // TODO call destructors of all contained terms
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  void inline tabulateNamedSymbol(rq::ContextCache &cache, llvm::StringRef name,
                                  rq::Symbol &symbol) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      rq::ScopeEntry &entry = it->second;
      rq::ScopeNode &node = cache.allocateValue<rq::ScopeNode>(symbol, entry);
      entry = rq::ScopeEntry(node);
    } else {
      this->_named_values.insert({name, rq::ScopeEntry(symbol)});
    }
  }
  inline void tabulateUnamedSymbol(rq::ContextCache &cache,
                                   rq::Symbol &symbol) {
    rq::ScopeEntry &entry = this->_unamed_values;
    if (entry.getIsEmpty()) {
      entry = symbol;
      return;
    }
    rq::ScopeNode &node = cache.allocateValue<rq::ScopeNode>(symbol, entry);
    entry = rq::ScopeEntry(node);
  }
  [[nodiscard]] inline rq::ScopeEntry getNamedEntry(llvm::StringRef name) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::ScopeEntry();
  }
  [[nodiscard]] inline rq::ConstScopeEntry
  getNamedEntry(llvm::StringRef name) const {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::ConstScopeEntry();
  }
  [[nodiscard]] inline rq::ScopeEntry getUnamedEntry() {
    return this->_unamed_values;
  }
  [[nodiscard]] inline rq::ConstScopeEntry getUnamedEntry() const {
    return this->_unamed_values;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedEntryRange() {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedEntryRange() const {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
};

struct Layout final : public rq::Scope {
  using Self = rq::Layout;

  Layout() : rq::Scope(rq::ValueKind::LAYOUT) {}
};

struct Procedure final : public rq::Scope {
  using Self = rq::Procedure;

  rq::Module *_module_ptr{nullptr};
  const rq::Expression *_expression_ptr{nullptr};

  Procedure(rq::ValueKind kind, rq::Module &module)
      : rq::Scope(kind), _module_ptr(&module) {}
  Procedure(const Self &) = delete;
  Procedure(Self &&) = delete;
  ~Procedure() override = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(Self &&rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule() {
    return rq::dereferencePtr(this->_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const {
    return rq::dereferencePtr(this->_module_ptr);
  }
  void setExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  switch (kind) {
  case rq::ModuleKind::NONE:
    return "none";
  case rq::ModuleKind::SOURCE:
    return "source";
  case rq::ModuleKind::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleKind kind, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : rq::Symbol(rq::ValueKind::MODULE), _module_kind(kind),
        _llvm_buffer_ref(std::move(buffer)), _path(path) {}
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const {
    return this->_module_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_llvm_buffer_ref.getBufferSize() != 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
    return this->_llvm_buffer_ref.getBuffer();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
  RQ_ALWAYS_INLINE void changeExpression(rq::Expression &expression) {
    RQ_ASSERT(this->_expression_ptr != nullptr, "no expression");
    this->_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &popExpression() {
    rq::Expression &expression = rq::dereferencePtr(this->_expression_ptr);
    this->_expression_ptr = nullptr;
    return expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression) {
    rq::Expression &replaced_expression = this->getExpression();
    this->_expression_ptr = &expression;
    return replaced_expression;
  }
};

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::ValueKind kind) {
  using namespace rq;
  using TK = ValueKind;
  switch (kind) {
  case TK::BFLOAT16:
    return llvm::APFloat::BFloat();
  case TK::BINARY16:
    return llvm::APFloat::IEEEhalf();
  case TK::BINARY32:
    return llvm::APFloat::IEEEsingle();
  case TK::BINARY64:
    return llvm::APFloat::IEEEdouble();
  case TK::BINARY128:
    return llvm::APFloat::IEEEquad();
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class NumericResult {
  OK,
  ERROR_EMPTY,
  ERROR_INVALID_DIGIT,
  ERROR_NO_DIGITS,
  ERROR_NO_DIGITS_IN_BASE,
  ERROR_ZERO_BASE,
  ERROR_TERM_TOO_BIG,
  ERROR_BASE_TOO_BIG,
  ERROR_MULTIPLE_DECIMAL_POINT,
  ERROR_INTEGER_WITH_DECIMAL_POINT,
  ERROR_FLOAT_WITH_BASE
};

[[nodiscard]] constexpr inline llvm::StringRef
getDescription(rq::NumericResult result) {
  using namespace rq;
  using NR = NumericResult;
  switch (result) {
  case NR::OK:
    return "no error occured";
  case NR::ERROR_EMPTY:
    return "numeric literal has no codeunits";
  case NR::ERROR_INVALID_DIGIT:
    return "invalid digit in numeric literal";
  case NR::ERROR_NO_DIGITS:
    return "numeric literal has no digits";
  case NR::ERROR_NO_DIGITS_IN_BASE:
    return "numeric literal has base with no digits";
  case NR::ERROR_ZERO_BASE:
    return "numeric literal has a base of 0";
  case NR::ERROR_TERM_TOO_BIG:
    return "numeric literal Value is too big";
  case NR::ERROR_BASE_TOO_BIG:
    return "numeric literal base is too big";
  case NR::ERROR_MULTIPLE_DECIMAL_POINT:
    return "numeric literal has multiple decimal points";
  case NR::ERROR_INTEGER_WITH_DECIMAL_POINT:
    return "integer literal has decimal point";
  case NR::ERROR_FLOAT_WITH_BASE:
    return "floating point numeric literal has base";
  }
  return "unkown error";
}

static constexpr unsigned MAX_BASE = 64;

static constexpr unsigned MIN_UPPER_BASE = 36;

[[nodiscard]] inline rq::NumericResult
cleanFloatText(llvm::StringRef text, llvm::SmallString<16> &ost_clean) {
  bool found_decimal = false;
  for (const char c : text) {
    switch (c) {
    case '.':
      if (found_decimal) {
        return rq::NumericResult::ERROR_MULTIPLE_DECIMAL_POINT;
      }
      found_decimal = true;
      ost_clean += c;
      break;
    case '_':
      break;
    case '0':
      [[fallthrough]];
    case '1':
      [[fallthrough]];
    case '2':
      [[fallthrough]];
    case '3':
      [[fallthrough]];
    case '4':
      [[fallthrough]];
    case '5':
      [[fallthrough]];
    case '6':
      [[fallthrough]];
    case '7':
      [[fallthrough]];
    case '8':
      [[fallthrough]];
    case '9':
      ost_clean += c;
      break;
    default:
      return rq::NumericResult::ERROR_INVALID_DIGIT;
    }
  }
  return rq::NumericResult::OK;
}

template <typename NumericParam>
[[nodiscard]] inline rq::NumericResult getNumericValue(llvm::StringRef text,
                                                       NumericParam &ost_term) {
  using Numeric = NumericParam;
  text = text.trim();
  if (text.empty()) {
    return rq::NumericResult::ERROR_EMPTY;
  }
  if constexpr (std::integral<Numeric>) {
    using Unsigned = std::make_unsigned_t<Numeric>;
    const Unsigned unsigned_max = std::numeric_limits<Unsigned>::max();
    Unsigned base = 10;
    Unsigned max_digit_multiplier = base - 1;
    Unsigned max_before_multiply = unsigned_max / max_digit_multiplier;
    Unsigned max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    Unsigned unsigned_term = 0;
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_term;
        unsigned_term = 0;
        max_digit_multiplier = base - 1;
        max_before_multiply = unsigned_max / max_digit_multiplier;
        max_before_add = unsigned_max - max_digit_multiplier;
        if (base == 0) {
          return rq::NumericResult::ERROR_ZERO_BASE;
        } else if (base > rq::MAX_BASE) {
          return rq::NumericResult::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT;
      } else if (c == '_') {
        continue;
      } else {
        char lower_c = c;
        if (base < rq::MIN_UPPER_BASE) {
          lower_c = rq::getLowercaseLetter(c);
        }
        const Numeric digit_base_multiplier =
            rq::getDigitBaseMultiplier(lower_c);
        if (digit_base_multiplier >= base) {
          return rq::NumericResult::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term > max_before_multiply) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term > max_before_add) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResult::ERROR_NO_DIGITS;
    }
    if constexpr (std::signed_integral<Numeric>) {
      if (!explicit_base) {
        const Unsigned signed_max =
            static_cast<Unsigned>(std::numeric_limits<Numeric>::max());
        if (unsigned_term > signed_max) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
      }
    }
    ost_term = std::bit_cast<Numeric>(unsigned_term);
    return rq::NumericResult::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt>) {
    const unsigned bit_depth = ost_term.getBitWidth();
    llvm::APInt max_base = llvm::APInt(bit_depth, rq::MAX_BASE);
    llvm::APInt min_upper_base = llvm::APInt(bit_depth, rq::MIN_UPPER_BASE);
    const llvm::APInt unsigned_max = llvm::APInt::getMaxValue(bit_depth);
    llvm::APInt base = llvm::APInt(bit_depth, 10);
    llvm::APInt max_digit_multiplier = base - 1;
    llvm::APInt max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
    llvm::APInt max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    llvm::APInt unsigned_term = llvm::APInt(bit_depth, 0);
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_term;
        unsigned_term = 0;
        max_digit_multiplier = base - 1;
        max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
        max_before_add = unsigned_max - max_digit_multiplier;
        if (base == 0) {
          return rq::NumericResult::ERROR_ZERO_BASE;
        } else if (base.ugt(max_base)) {
          return rq::NumericResult::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT;
      } else if (c == '_') {
        continue;
      } else {
        char lower_c = c;
        if (base.ugt(min_upper_base)) {
          lower_c = rq::getLowercaseLetter(c);
        }
        const llvm::APInt digit_base_multiplier =
            llvm::APInt(bit_depth, rq::getDigitBaseMultiplier(lower_c));
        if (digit_base_multiplier.uge(base)) {
          return rq::NumericResult::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term.ugt(max_before_multiply)) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term.ugt(max_before_add)) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResult::ERROR_NO_DIGITS;
    }
    ost_term = unsigned_term;
    return rq::NumericResult::OK;
  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    std::ignore = rq::cleanFloatText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), ost_term, 10);
  } else {
    static_assert(false, "type not supported");
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &ost_term,
                rq::ValueKind semantics) {
  llvm::SmallString<16> buffer;
  rq::NumericResult result = rq::cleanFloatText(text, buffer);
  if (result != rq::NumericResult::OK) {
    return result;
  }
  const llvm::fltSemantics &llvm_semantics =
      rq::getLlvmFloatSemantics(semantics);
  ost_term = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

template <typename TypeParam, typename... ArgNParam>
inline TypeParam &rq::ContextCache::allocateValue(ArgNParam &&...arg_n) {
  TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
  ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
  return rq::dereferencePtr(ptr);
}

inline llvm::StringRef rq::ContextCache::saveString(llvm::Twine twine) {
  return this->_llvm_string_saver.save(twine);
}

inline void rq::ContextCache::discardExpression(rq::Expression &expression) {
  this->_unused_expression_ptrs.emplace_back(&expression);
}

inline rq::VoidType &rq::ContextCache::getVoidType() {
  if (!this->_void_type) {
    this->_void_type = &this->allocateValue<rq::VoidType>();
  }
  return rq::dereferencePtr(this->_void_type);
}

inline rq::NullType &rq::ContextCache::getNullType() {
  if (!this->_null_type) {
    this->_null_type = &this->allocateValue<rq::NullType>();
  }
  return rq::dereferencePtr(this->_null_type);
}

inline rq::NoReturnType &rq::ContextCache::getNoReturnType() {
  if (!this->_no_return_type) {
    this->_no_return_type = &this->allocateValue<rq::NoReturnType>();
  }
  return rq::dereferencePtr(this->_no_return_type);
}

inline rq::VariadicArgumentsType &rq::ContextCache::getVariadicArgumentsType() {
  if (!this->_variadic_arguments_type) {
    this->_variadic_arguments_type =
        &this->allocateValue<rq::VariadicArgumentsType>();
  }
  return rq::dereferencePtr(this->_variadic_arguments_type);
}

inline rq::BooleanType &rq::ContextCache::getBooleanType() {
  if (!this->_boolean_type) {
    this->_boolean_type = &this->allocateValue<rq::BooleanType>();
  }
  return rq::dereferencePtr(this->_boolean_type);
}

inline rq::Utf8Type &rq::ContextCache::getUtf8Type() {
  if (!this->_utf8_type) {
    this->_utf8_type = &this->allocateValue<rq::Utf8Type>();
  }
  return rq::dereferencePtr(this->_utf8_type);
}

inline rq::Bfloat16Type &rq::ContextCache::getBfloat16Type() {
  if (!this->_bfloat16_type) {
    this->_bfloat16_type = &this->allocateValue<rq::Bfloat16Type>();
  }
  return rq::dereferencePtr(this->_bfloat16_type);
}

inline rq::Binary16Type &rq::ContextCache::getBinary16Type() {
  if (!this->_binary16_type) {
    this->_binary16_type = &this->allocateValue<rq::Binary16Type>();
  }
  return rq::dereferencePtr(this->_binary16_type);
}

inline rq::Binary32Type &rq::ContextCache::getBinary32Type() {
  if (!this->_binary32_type) {
    this->_binary32_type = &this->allocateValue<rq::Binary32Type>();
  }
  return rq::dereferencePtr(this->_binary32_type);
}

inline rq::Binary64Type &rq::ContextCache::getBinary64Type() {
  if (!this->_binary64_type) {
    this->_binary64_type = &this->allocateValue<rq::Binary64Type>();
  }
  return rq::dereferencePtr(this->_binary64_type);
}

inline rq::Binary128Type &rq::ContextCache::getBinary128Type() {
  if (!this->_binary128_type) {
    this->_binary128_type = &this->allocateValue<rq::Binary128Type>();
  }
  return rq::dereferencePtr(this->_binary128_type);
}

inline rq::BuiltinDepthedType &
rq::ContextCache::_getOrInsertBuiltinDepthType(rq::ValueKind kind,
                                               unsigned parameter) {
  llvm::FoldingSetNodeID id;
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(parameter);
  void *insert_pos = nullptr;
  if (rq::BuiltinDepthedType *existing =
          this->_builtin_depthed_types.FindNodeOrInsertPos(id, insert_pos)) {
    return rq::dereferencePtr(existing);
  }
  rq::BuiltinDepthedType &new_type =
      this->allocateValue<rq::BuiltinDepthedType>(kind, parameter);
  this->_builtin_depthed_types.InsertNode(&new_type, insert_pos);
  return new_type;
}

inline rq::WordType &rq::ContextCache::getWordType(unsigned bit_depth) {
  return this->_getOrInsertBuiltinDepthType(rq::ValueKind::WORD, bit_depth)
      .getWordType();
}

inline rq::UnsignedType &rq::ContextCache::getUnsignedType(unsigned bit_depth) {
  return this->_getOrInsertBuiltinDepthType(rq::ValueKind::UNSIGNED, bit_depth)
      .getUnsignedType();
}

inline rq::SignedType &rq::ContextCache::getSignedType(unsigned bit_depth) {
  return this->_getOrInsertBuiltinDepthType(rq::ValueKind::SIGNED, bit_depth)
      .getSignedType();
}

} // namespace rq
