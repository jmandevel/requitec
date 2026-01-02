#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
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

struct Expression;
struct Scope;

struct Frame final {
  using Self = rq::Frame;

  llvm::BumpPtrAllocator _llvm_arena;
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs;
  rq::Scope *_scope_ptr;

  Frame(rq::Scope &scope) : _scope_ptr(&scope) {};
  Frame(const Self &) = delete;
  Frame(Self &&) = delete;
  ~Frame() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam &
  allocateValue(ArgNParam &&...arg_n) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef saveString(llvm::Twine twine) {
    llvm::StringRef saved_string = this->_llvm_string_saver.save(twine);
    return saved_string;
  }
  [[nodiscard]] rq::Expression &acquireExpression();
  RQ_ALWAYS_INLINE void discardExpression(rq::Expression &expression) {
    this->_unused_expression_ptrs.emplace_back(&expression);
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] rq::Scope &getScope() {
    return rq::dereferencePtr(this->_scope_ptr);
  }
  [[nodiscard]] const rq::Scope &getScope() const {
    return rq::dereferencePtr(this->_scope_ptr);
  }
};

// TODO static values. only care about dynamic runtime for now (types and
// symbols all that are needed).

enum class ValueKind : std::uint_fast8_t {
  VOID,
  NULL_TYPE,
  NO_RETURN,
  VARIADIC_ARGUMENTS_TYPE,
  BOOLEAN,
  WORD,
  SIGNED,
  UNSIGNED,
  UTF8,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  MODULE,
  TOP_SCOPE,
  TABLE,
  SCOPE,
  OBJECT,
  ENUMERATION,
  ENUMERATOR,
  LAYOUT,
  VARIABLE,
  FUNCTION,
  METHOD,
  ENTRY_POINT
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
  case rq::ValueKind::WORD:
    return "word";
  case rq::ValueKind::SIGNED:
    return "signed";
  case rq::ValueKind::UNSIGNED:
    return "unsigned";
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Scope &getScope() {
    return *std::bit_cast<rq::Scope *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Scope &getScope() const {
    return *std::bit_cast<rq::Scope *>(this);
  }
};

struct ScopeNode;
struct ScopeEntry;
struct ScopeEntryIterator;
struct ConstScopeEntryIterator;

struct ScopeEntry final {
  using Self = rq::ScopeEntry;

  llvm::PointerUnion<rq::Value *, rq::ScopeNode *> _ptr_union{nullptr};

  RQ_ALWAYS_INLINE ScopeEntry() = default;
  RQ_ALWAYS_INLINE ScopeEntry(rq::Value &value) : _ptr_union(&value) {}
  RQ_ALWAYS_INLINE ScopeEntry(rq::ScopeNode &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~ScopeEntry() = default;
  RQ_ALWAYS_INLINE ScopeEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ScopeEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticValue() const {
    return llvm::isa<rq::Value *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeNode() const {
    return llvm::isa<rq::ScopeNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value &getStaticValue() {
    return rq::dereferencePtr(llvm::cast<rq::Value *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value &getStaticValue() const {
    return rq::dereferencePtr(llvm::cast<rq::Value *>(this->_ptr_union));
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

  llvm::PointerUnion<const rq::Value *, const rq::ScopeNode *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE ConstScopeEntry() = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::ScopeEntry &rhs)
      : _ptr_union(
            std::bit_cast<
                llvm::PointerUnion<const rq::Value *, const rq::ScopeNode *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstScopeEntry(rq::ScopeEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Value *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::Value &value)
      : _ptr_union(&value) {}
  RQ_ALWAYS_INLINE ConstScopeEntry(const rq::ScopeNode &node)
      : _ptr_union(&node) {}
  ~ConstScopeEntry() = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ConstScopeEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::ScopeEntry &rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Value *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::ScopeEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Value *, const rq::ScopeNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticValue() const {
    return llvm::isa<const rq::Value *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeNode() const {
    return llvm::isa<const rq::ScopeNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value &getStaticValue() const {
    return rq::dereferencePtr(llvm::cast<const rq::Value *>(this->_ptr_union));
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

  rq::Value *_value_ptr{nullptr};
  rq::ScopeEntry _scope_entry{};

  RQ_ALWAYS_INLINE ScopeNode() = default;
  RQ_ALWAYS_INLINE ScopeNode(rq::Value &value_a, rq::Value &value_b)
      : _value_ptr(&value_a), _scope_entry(value_b) {}
  RQ_ALWAYS_INLINE ScopeNode(rq::Value &value, rq::ScopeNode &node)
      : _value_ptr(&value), _scope_entry(node) {}
  RQ_ALWAYS_INLINE ScopeNode(rq::Value &value, const rq::ScopeEntry &entry)
      : _value_ptr(&value), _scope_entry(entry) {}
  ScopeNode(const Self &) = delete;
  ScopeNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~ScopeNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStaticValue() const {
    return this->_value_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeEntry() const {
    return !this->_scope_entry.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value &getStaticValue() {
    return rq::dereferencePtr(this->_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value &getStaticValue() const {
    return rq::dereferencePtr(this->_value_ptr);
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
  using value_type = rq::Value;
  using reference = rq::Value &;
  using pointer = rq::Value *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ScopeEntry _entry;

  RQ_ALWAYS_INLINE ScopeEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit ScopeEntryIterator(rq::ScopeEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsStaticValue()) {
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
    (*this)++;
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value &operator*() {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value &operator*() const {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value *operator->() {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value *operator->() const {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

struct ConstScopeEntryIterator final {
  using Self = rq::ConstScopeEntryIterator;
  using value_type = const rq::Value;
  using reference = const rq::Value &;
  using pointer = rq::Value *;
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
    if (this->_entry.getIsStaticValue()) {
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
    (*this)++;
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value &operator*() const {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value *operator->() const {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
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

struct Scope : rq::Value {
  using Self = rq::Scope;

  llvm::SmallDenseMap<llvm::StringRef, rq::ScopeEntry> _named_values{};
  rq::ScopeEntry _unamed_values{};

  Scope() : rq::Value(rq::ValueKind::SCOPE) {}
  Scope(rq::ValueKind kind) : rq::Value(kind) {}
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
  void inline tabulateNamedSymbol(rq::Frame &frame, llvm::StringRef name,
                                  rq::Value &symbol) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      rq::ScopeEntry &entry = it->second;
      rq::ScopeNode &node = frame.allocateValue<rq::ScopeNode>(symbol, entry);
      entry = rq::ScopeEntry(node);
    } else {
      this->_named_values.insert({name, rq::ScopeEntry(symbol)});
    }
  }
  inline void tabulateUnamedSymbol(rq::Frame &frame, rq::Value &symbol) {
    rq::ScopeEntry &entry = this->_unamed_values;
    rq::ScopeNode &node = frame.allocateValue<rq::ScopeNode>(symbol, entry);
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

  const rq::Expression *_expression_ptr{nullptr};

  Procedure(rq::ValueKind kind) : rq::Scope(kind) {}
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

struct Module final : public rq::Value {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleKind kind, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : rq::Value(rq::ValueKind::MODULE), _module_kind(kind),
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
    const unsigned bit_width = ost_term.getBitWidth();
    llvm::APInt max_base = llvm::APInt(bit_width, rq::MAX_BASE);
    llvm::APInt min_upper_base = llvm::APInt(bit_width, rq::MIN_UPPER_BASE);
    const llvm::APInt unsigned_max = llvm::APInt::getMaxValue(bit_width);
    llvm::APInt base = llvm::APInt(bit_width, 10);
    llvm::APInt max_digit_multiplier = base - 1;
    llvm::APInt max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
    llvm::APInt max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    llvm::APInt unsigned_term = llvm::APInt(bit_width, 0);
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
            llvm::APInt(bit_width, rq::getDigitBaseMultiplier(lower_c));
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

} // namespace rq
