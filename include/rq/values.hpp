#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/SmallVector.h>

#include <bit>
#include <cstdint>

namespace rq {

struct Expression;
struct Symbol;
struct UserSymbol;
struct Declaration;
struct Scope;
struct Procedure;
struct Template;
struct Layout;
struct Signature;
struct Object;
struct Enumeration;
struct UnorderedVariable;

enum class SymbolType : std::uint_fast8_t {
  NONE,
  NULL_,
  VOID,
  NO_RETURN,
  BOOLEAN,
  WORD,
  SIGNED,
  UNSIGNED,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  UTF8,
  LAYOUT,
  // object_layout is same as layout, but it has parameters with object
  // parameter statement attributes (e.g. private, parent, etc) so it cannot be
  // a root and can only be used as a layout for an object.
  OBJECT_LAYOUT,
  SIGNATURE,
  SCOPE,
  OBJECT,
  OBJECT_TEMPLATE,
  NAMED_OR_POSITIONAL_PARAMETER,
  POSITIONAL_PARAMETER,
  NAMED_PARAMETER,
  ENUMERATION,
  ENUMERATION_TEMPLATE,
  UNORDERED_VARIABLE,
  UNORDERED_VARIABLE_TEMPLATE,
  ORDERED_VARIABLE,
  ENTRY_POINT,
  FUNCTION,
  FUNCTION_TEMPLATE,
  CONSTRUCTOR,
  CONSTRUCTOR_TEMPLATE,
  DESTRUCTOR,
  RANGER,
  DEEP_COPIER,
  RETAIN_MOVER,
  DROP_MOVER,
  SWAPER,
  INDEXER,
  ANONYMOUS_FUNCTION,
  ANONYMOUS_FUNCTION_TEMPLATE
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::SymbolType type) {
  using namespace rq;
  using ST = SymbolType;
  switch (type) {
  case ST::NONE:
    return "no symbol";
  case ST::NULL_:
    return "null";
  case ST::VOID:
    return "void";
  case ST::NO_RETURN:
    return "no return";
  case ST::BOOLEAN:
    return "boolean";
  case ST::WORD:
    return "word";
  case ST::SIGNED:
    return "signed integer";
  case ST::UNSIGNED:
    return "unsigned integer";
  case ST::BFLOAT16:
    return "16 bit brain floating-point float";
  case ST::BINARY16:
    return "half-precision floating-point float";
  case ST::BINARY32:
    return "single-precision floating-point float";
  case ST::BINARY64:
    return "double-precision floating-point float";
  case ST::BINARY128:
    return "quadruple-precision floating-point float";
  case ST::UTF8:
    return "8 bit Unicode Transformation Format codeunit";
  case ST::LAYOUT:
    return "layout";
  case ST::OBJECT_LAYOUT:
    return "object layout";
  case ST::SIGNATURE:
    return "signature";
  case ST::SCOPE:
    return "scope";
  case ST::OBJECT:
    return "object";
  case ST::OBJECT_TEMPLATE:
    return "object template";
  case ST::NAMED_OR_POSITIONAL_PARAMETER:
    return "named or positional parameter";
  case ST::POSITIONAL_PARAMETER:
    return "positional parameter";
  case ST::NAMED_PARAMETER:
    return "named parameter";
  case ST::ENUMERATION:
    return "enumeration";
  case ST::ENUMERATION_TEMPLATE:
    return "enumeration template";
  case ST::UNORDERED_VARIABLE:
    return "unordered variable";
  case ST::UNORDERED_VARIABLE_TEMPLATE:
    return "unordered variable template";
  case ST::ORDERED_VARIABLE:
    return "ordered variable";
  case ST::ENTRY_POINT:
    return "entry point";
  case ST::FUNCTION:
    return "function";
  case ST::FUNCTION_TEMPLATE:
    return "function template";
  case ST::CONSTRUCTOR:
    return "constructor";
  case ST::CONSTRUCTOR_TEMPLATE:
    return "constructor template";
  case ST::DESTRUCTOR:
    return "destructor";
  case ST::RANGER:
    return "ranger";
  case ST::DEEP_COPIER:
    return "deep copier";
  case ST::RETAIN_MOVER:
    return "retain mover";
  case ST::DROP_MOVER:
    return "drop mover";
  case ST::SWAPER:
    return "swaper";
  case ST::INDEXER:
    return "indexer";
  case ST::ANONYMOUS_FUNCTION:
    return "anonymous function";
  case ST::ANONYMOUS_FUNCTION_TEMPLATE:
    return "anonymous function template";
  }
  RQ_UNREACHABLE();
}

enum class SymbolFlags : std::uint16_t {
  NONE = 0,
  USER = rq::getBit(15),
  DECLARED = rq::getBit(14),
  SCOPE = rq::getBit(13),
  PROCEDURE = rq::getBit(12),
  TEMPLATE = rq::getBit(11),
  ROOT = rq::getBit(10),
  INTEGER = rq::getBit(9),
  FLOAT = rq::getBit(8),
  CODEUNIT = rq::getBit(7),
  VARIED_DEPTH = rq::getBit(6),
  LAYOUT = rq::getBit(5),
  PARAMETER = rq::getBit(4),
  NAME_REQUIRED = rq::getBit(3),
  NAME_OPTIONAL = rq::getBit(2)
};

template <> struct is_flags<rq::SymbolFlags> : std::true_type {};

[[nodiscard]] inline rq::SymbolFlags getFlags(rq::SymbolType type) {
  using namespace rq;
  using ST = SymbolType;
  using SF = SymbolFlags;
  switch (type) {
  case ST::NONE:
    return SF::NONE;
  case ST::NULL_:
    return SF::ROOT;
  case ST::VOID:
    return SF::ROOT;
  case ST::NO_RETURN:
    return SF::ROOT;
  case ST::BOOLEAN:
    return SF::ROOT;
  case ST::WORD:
    return SF::ROOT | SF::VARIED_DEPTH;
  case ST::SIGNED:
    return SF::ROOT | SF::INTEGER | SF::VARIED_DEPTH;
  case ST::UNSIGNED:
    return SF::ROOT | SF::INTEGER | SF::VARIED_DEPTH;
  case ST::BFLOAT16:
    return SF::ROOT | SF::FLOAT;
  case ST::BINARY16:
    return SF::ROOT | SF::FLOAT;
  case ST::BINARY32:
    return SF::ROOT | SF::FLOAT;
  case ST::BINARY64:
    return SF::ROOT | SF::FLOAT;
  case ST::BINARY128:
    return SF::ROOT | SF::FLOAT;
  case ST::UTF8:
    return SF::ROOT | SF::CODEUNIT;
  case ST::LAYOUT:
    return SF::USER | SF::ROOT | SF::LAYOUT;
  case ST::OBJECT_LAYOUT:
    return SF::USER | SF::LAYOUT;
  case ST::SIGNATURE:
    return SF::USER | SF::ROOT;
  case ST::SCOPE:
    return SF::USER | SF::DECLARED | SF::SCOPE;
  case ST::OBJECT:
    return SF::USER | SF::DECLARED | SF::SCOPE | SF::ROOT | SF::NAME_REQUIRED;
  case ST::OBJECT_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE | SF::NAME_REQUIRED;
  case ST::NAMED_OR_POSITIONAL_PARAMETER:
    return SF::USER | SF::DECLARED | SF::NAME_REQUIRED;
  case ST::POSITIONAL_PARAMETER:
    return SF::USER | SF::DECLARED | SF::NAME_OPTIONAL;
  case ST::NAMED_PARAMETER:
    return SF::USER | SF::DECLARED | SF::NAME_REQUIRED;
  case ST::ENUMERATION:
    return SF::USER | SF::DECLARED | SF::ROOT | SF::NAME_REQUIRED;
  case ST::ENUMERATION_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE | SF::NAME_REQUIRED;
  case ST::UNORDERED_VARIABLE:
    return SF::USER | SF::DECLARED | SF::NAME_REQUIRED;
  case ST::UNORDERED_VARIABLE_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE | SF::NAME_REQUIRED;
  case ST::ORDERED_VARIABLE:
    return SF::USER | SF::DECLARED | SF::NAME_REQUIRED;
  case ST::ENTRY_POINT:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::FUNCTION:
    return SF::USER | SF::DECLARED | SF::PROCEDURE | SF::NAME_REQUIRED;
  case ST::FUNCTION_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE | SF::NAME_REQUIRED;
  case ST::CONSTRUCTOR:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::CONSTRUCTOR_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE;
  case ST::DESTRUCTOR:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::RANGER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::DEEP_COPIER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::RETAIN_MOVER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::DROP_MOVER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::SWAPER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::INDEXER:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::ANONYMOUS_FUNCTION:
    return SF::USER | SF::DECLARED | SF::PROCEDURE;
  case ST::ANONYMOUS_FUNCTION_TEMPLATE:
    return SF::USER | SF::DECLARED | SF::TEMPLATE;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUser(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::USER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDeclared(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::DECLARED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::ROOT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::CODEUNIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariedDepth(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::VARIED_DEPTH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameRequired(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::NAME_REQUIRED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameOptional(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::NAME_OPTIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameForbidden(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasNone(flags, rq::SymbolFlags::NAME_REQUIRED |
                                   rq::SymbolFlags::NAME_OPTIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayout(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::LAYOUT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasAll(flags, rq::SymbolFlags::PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasData(rq::SymbolType type) {
  const rq::SymbolFlags flags = rq::getFlags(type);
  return rq::getHasSome(flags,
                        rq::SymbolFlags::VARIED_DEPTH | rq::SymbolFlags::USER);
}

struct UserSymbol final {
  using Self = rq::UserSymbol;

  rq::SymbolType _type;

  UserSymbol(rq::SymbolType type) : _type(type) {}
  UserSymbol(const Self &) = delete;
  UserSymbol(Self &&) = delete;
  ~UserSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] inline bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] inline bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolType getType() const {
    return this->_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope() const {
    return rq::getIsScope(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure() const {
    return rq::getIsProcedure(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate() const {
    return rq::getIsTemplate(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot() const {
    return rq::getIsRoot(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameRequired() const {
    return rq::getIsNameRequired(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameOptional() const {
    return rq::getIsNameOptional(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNameForbidden() const {
    return rq::getIsNameForbidden(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDeclared() const {
    return rq::getIsDeclared(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayout() const {
    return rq::getIsLayout(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter() const {
    return rq::getIsParameter(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasData() const {
    return rq::getHasData(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Declaration &getDeclaration() const {
    RQ_ASSERT(this->getIsDeclared(), "not declared");
    return *std::bit_cast<rq::Declaration *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Declaration &getDeclaration() {
    RQ_ASSERT(this->getIsDeclared(), "not declared");
    return *std::bit_cast<rq::Declaration *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Scope &getScope() const {
    RQ_ASSERT(this->getIsScope(), "not scope");
    return *std::bit_cast<rq::Scope *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Scope &getScope() {
    RQ_ASSERT(this->getIsScope(), "not scope");
    return *std::bit_cast<rq::Scope *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &getProcedure() const {
    RQ_ASSERT(this->getIsProcedure(), "not procedure");
    return *std::bit_cast<rq::Procedure *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &getProcedure() {
    RQ_ASSERT(this->getIsProcedure(), "not procedure");
    return *std::bit_cast<rq::Procedure *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &getTemplate() const {
    RQ_ASSERT(this->getIsTemplate(), "not template");
    return *std::bit_cast<rq::Template *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template &getTemplate() {
    RQ_ASSERT(this->getIsTemplate(), "not template");
    return *std::bit_cast<rq::Template *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getLayout() const {
    RQ_ASSERT(this->getIsLayout(), "not layout");
    return *std::bit_cast<rq::Layout *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getLayout() {
    RQ_ASSERT(this->getIsLayout(), "not layout");
    return *std::bit_cast<rq::Layout *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getSignature() const {
    RQ_ASSERT(this->getType() == rq::SymbolType::SIGNATURE, "not signature");
    return *std::bit_cast<rq::Layout *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getSignature() {
    RQ_ASSERT(this->getType() == rq::SymbolType::SIGNATURE, "not signature");
    return *std::bit_cast<rq::Layout *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Object &getObject() const {
    RQ_ASSERT(this->getType() == rq::SymbolType::OBJECT, "not object");
    return *std::bit_cast<rq::Object *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Object &getObject() {
    RQ_ASSERT(this->getType() == rq::SymbolType::OBJECT, "not object");
    return *std::bit_cast<rq::Object *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Enumeration &getEnumeration() const {
    RQ_ASSERT(this->getType() == rq::SymbolType::ENUMERATION,
              "not enumeration");
    return *std::bit_cast<rq::Enumeration *>(this);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Enumeration &getEnumeration() {
    RQ_ASSERT(this->getType() == rq::SymbolType::ENUMERATION,
              "not enumeration");
    return *std::bit_cast<rq::Enumeration *>(this);
  }
};

struct Table;

struct Declaration final {
  using Self = rq::Declaration;

  rq::UserSymbol _user_symbol;
  const rq::Expression *_expression_ptr;
  rq::Table *_containing_table_ptr;
  rq::StatementAttribute _statement_attributes;
  llvm::StringRef _name;

  Declaration(rq::SymbolType type, const rq::Expression &expression,
              rq::Table &containing_table,
              rq::StatementAttribute statement_attributes, llvm::StringRef name)
      : _user_symbol(type), _expression_ptr(&expression),
        _containing_table_ptr(&containing_table),
        _statement_attributes(statement_attributes), _name(name) {
    RQ_ASSERT(rq::getIsDeclared(type), "not declared");
    RQ_ASSERT(rq::getIsNameOptional(type) || rq::getIsNameRequired(type),
              "should not have name");
  }
  Declaration(rq::SymbolType type, const rq::Expression &expression,
              rq::Table &containing_table,
              rq::StatementAttribute statement_attributes)
      : _user_symbol(type), _expression_ptr(&expression),
        _containing_table_ptr(&containing_table),
        _statement_attributes(statement_attributes) {
    RQ_ASSERT(rq::getIsDeclared(type), "not declared");
    RQ_ASSERT(rq::getIsNameOptional(type) || rq::getIsNameForbidden(type),
              "should have name");
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UserSymbol &getUserSymbol() {
    return this->_user_symbol;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::UserSymbol &getUserSymbol() const {
    return this->_user_symbol;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StatementAttribute
  getStatementAttributes() const {
    return this->_statement_attributes;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getContainingTable() {
    return rq::dereferencePtr(this->_containing_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getContainingTable() const {
    return rq::dereferencePtr(this->_containing_table_ptr);
  }
};

struct Table final {
  using Self = rq::Table;

  llvm::StringRef _name;
  rq::Scope *_containing_scope_ptr;
  rq::Table *_containing_table_ptr;
  // symbols nested inside this table.
  llvm::StringMap<rq::Table *> _nested_tables;
  // unamed symbols and symbols that share name of this table.
  llvm::SmallVector<rq::UserSymbol *, 1> _parallel_symbols;

  Table(llvm::StringRef name, rq::Scope *containing_scope_ptr,
        rq::Table *containing_table_ptr)
      : _name(name), _containing_scope_ptr(containing_scope_ptr),
        _containing_table_ptr(containing_table_ptr) {}
  Table(const Self &) = delete;
  Table(Self &&) = delete;
  ~Table() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNestedTable(llvm::StringRef name) {
    return this->_nested_tables.contains(name);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &
  getNestedTable(rq::Context &context, llvm::StringRef name) {
    rq::Table *&table_ptr = this->_nested_tables[name];
    rq::Table &table = context.allocateValue<rq::Table>(
        name, this->_containing_scope_ptr, this);
    table_ptr = &table;
    return table;
  }
  [[nodiscard]] inline rq::Table *tryGetNestedTable(llvm::StringRef name) {
    llvm::StringMap<rq::Table *>::iterator it = this->_nested_tables.find(name);
    if (it == this->_nested_tables.end()) {
      return nullptr;
    }
    return it->getValue();
  }
  [[nodiscard]] inline const rq::Table *
  tryGetNestedTable(llvm::StringRef name) const {
    llvm::StringMap<rq::Table *>::const_iterator it =
        this->_nested_tables.find(name);
    if (it == this->_nested_tables.end()) {
      return nullptr;
    }
    return it->getValue();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::StringMap<rq::Table *> &
  getNestedTables() const {
    return this->_nested_tables;
  }
  void RQ_ALWAYS_INLINE addParallelSymbol(rq::UserSymbol &user_symbol) {
    this->_parallel_symbols.push_back(&user_symbol);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<const rq::UserSymbol *>
  getParallelSymbols() const {
    return this->_parallel_symbols;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeLevelTable() const {
    return this->_containing_table_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getContainingTable() {
    return rq::dereferencePtr(this->_containing_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getContainingTable() const {
    return rq::dereferencePtr(this->_containing_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Scope &getContainingScope() {
    return rq::dereferencePtr(this->_containing_scope_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Scope &getContainingScope() const {
    return rq::dereferencePtr(this->_containing_scope_ptr);
  }
};

struct Scope final {
  using Self = rq::Scope;

  rq::Declaration _declaration;
  rq::Table _table;

  Scope(rq::SymbolType type, const rq::Expression &expression,
        rq::Table &containing_table,
        rq::StatementAttribute statement_attributes, llvm::StringRef name)
      : _declaration(type, expression, containing_table, statement_attributes,
                     name),
        _table(name, this, nullptr) {
    RQ_ASSERT(rq::getIsScope(type), "not scope");
  }
  Scope(rq::SymbolType type, const rq::Expression &expression,
        rq::Table &containing_table,
        rq::StatementAttribute statement_attributes)
      : _declaration(type, expression, containing_table, statement_attributes),
        _table({}, this, nullptr) {
    RQ_ASSERT(rq::getIsScope(type), "not scope");
  }
  Scope(const Self &) = delete;
  Scope(Self &&) = delete;
  ~Scope() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Declaration &getDeclaration() {
    return this->_declaration;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Declaration &getDeclaration() const {
    return this->_declaration;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getTable() { return this->_table; }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getTable() const {
    return this->_table;
  }
};

struct Template final {
  using Self = rq::Template;

  rq::Declaration _declaration;
  rq::Layout *_template_parameters_layout_ptr;

  Template(rq::SymbolType type, const rq::Expression &expression,
           rq::Table &containing_table,
           rq::StatementAttribute statement_attributes)
      : _declaration(type, expression, containing_table, statement_attributes) {
    RQ_ASSERT(rq::getIsTemplate(type), "not template");
  }
  Template(rq::SymbolType type, const rq::Expression &expression,
           rq::Table &containing_table,
           rq::StatementAttribute statement_attributes, llvm::StringRef name)
      : _declaration(type, expression, containing_table, statement_attributes,
                     name) {
    RQ_ASSERT(rq::getIsTemplate(type), "not template");
  }
  Template(const Self &) = delete;
  Template(Self &&) = delete;
  ~Template() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Declaration &getDeclaration() {
    return this->_declaration;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Declaration &getDeclaration() const {
    return this->_declaration;
  }
};

struct Procedure final {
  using Self = rq::Procedure;

  rq::Scope _scope;
  const rq::Signature *_signature_ptr;

  Procedure(rq::SymbolType type, const rq::Expression &expression,
            rq::Table &containing_table,
            rq::StatementAttribute statement_attributes,
            const rq::Signature &signature)
      : _scope(type, expression, containing_table, statement_attributes) {
    RQ_ASSERT(rq::getIsProcedure(type), "not procedure");
    rq::assignSingleValue(this->_signature_ptr, &signature);
  }
  Procedure(rq::SymbolType type, const rq::Expression &expression,
            rq::Table &containing_table,
            rq::StatementAttribute statement_attributes, llvm::StringRef name,
            const rq::Signature &signature)
      : _scope(type, expression, containing_table, statement_attributes, name) {
    RQ_ASSERT(rq::getIsProcedure(type), "not procedure");
    rq::assignSingleValue(this->_signature_ptr, &signature);
  }
  Procedure(const Self &) = delete;
  Procedure(Self &&) = delete;
  ~Procedure() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Scope &getScope() { return this->_scope; }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Scope &getScope() const {
    return this->_scope;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &getSignature() const {
    return rq::dereferencePtr(this->_signature_ptr);
  }
};

struct Symbol final {
  using Self = rq::Symbol;

  rq::SymbolType _type = rq::SymbolType::NONE;
  union {
    void *_nothing_ptr = nullptr;
    rq::UserSymbol *_user_symbol_ptr;
    unsigned _depth;
  };

  Symbol() = default;
  Symbol(rq::SymbolType type, unsigned depth) : _type(type), _depth(depth) {
    RQ_ASSERT(rq::getIsVariedDepth(type), "symbol does not have varied depth");
  }
  Symbol(rq::SymbolType type) : _type(type) {
    RQ_ASSERT(!rq::getHasData(type), "symbol has data");
  }
  Symbol(rq::UserSymbol &user)
      : _user_symbol_ptr(&user), _type(user.getType()) {}
  Symbol(const Self &) = default;
  Symbol(Self &&) = default;
  ~Symbol() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
};

struct ValueType final {
  using Self = rq::ValueType;

  rq::Symbol _root;
  rq::TypeAttribute _type_attributes;
};

struct Parameter final {
  using Self = rq::Parameter;

  rq::Declaration _declaration;
  rq::ValueType _value_type;

  Parameter(rq::SymbolType type, const rq::Expression &expression,
            rq::Table &containing_table,
            rq::StatementAttribute statement_attributes,
            rq::ValueType value_type)
      : _declaration(type, expression, containing_table, statement_attributes),
        _value_type(value_type) {}
  Parameter(rq::SymbolType type, const rq::Expression &expression,
            rq::Table &containing_table,
            rq::StatementAttribute statement_attributes, llvm::StringRef name,
            rq::ValueType value_type)
      : _declaration(type, expression, containing_table, statement_attributes,
                     name),
        _value_type(value_type) {}
};

struct Layout final {
  using Self = rq::Layout;

  rq::UserSymbol _user_symbol;
  llvm::StringMap<rq::Parameter *> _property_ptr_table;
  llvm::SmallVector<rq::Parameter *, 1> _property_ptrs;

  Layout(rq::SymbolType type) : _user_symbol(type) {
    RQ_ASSERT(rq::getIsLayout(type), "must be layout type");
  }
  Layout(const Self &) = delete;
  Layout(Self &&) = delete;
  ~Layout() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Signature final {
  using Self = rq::Signature;

  rq::UserSymbol _user_symbol;
  rq::ValueType _extension_type;
  rq::ValueType _return_type;
  rq::Layout *_parameters_layout_ptr;
  rq::Layout *_dynamic_capture_layout_ptr;

  Signature() : _user_symbol(rq::SymbolType::SIGNATURE) {}
  Signature(const Self &) = delete;
  Signature(Self &&) = delete;
  ~Signature() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Object final {
  using Self = rq::Object;

  rq::Scope _scope;
  const rq::Layout *_properties_layout_ptr;

  Object(const rq::Expression &expression, rq::Table &containing_table,
         rq::StatementAttribute statement_attributes, llvm::StringRef name)
      : _scope(rq::SymbolType::OBJECT, expression, containing_table,
               statement_attributes, name) {}
  Object(const Self &) = delete;
  Object(Self &&) = delete;
  ~Object() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Enumeration final {
  using Self = rq::Enumeration;

  rq::Declaration _declaration;

  Enumeration(const rq::Expression &expression, rq::Table &containing_table,
              rq::StatementAttribute statement_attributes, llvm::StringRef name)
      : _declaration(rq::SymbolType::ENUMERATION, expression, containing_table,
                     statement_attributes, name) {}
  Enumeration(const Self &) = delete;
  Enumeration(Self &&) = delete;
  ~Enumeration() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct UnorderedVariable final {
  using Self = rq::UnorderedVariable;

  rq::Declaration _declaration;

  UnorderedVariable(const rq::Expression &expression,
                    rq::Table &containing_table,
                    rq::StatementAttribute statement_attributes,
                    llvm::StringRef name)
      : _declaration(rq::SymbolType::ENUMERATION, expression, containing_table,
                     statement_attributes, name) {}
  UnorderedVariable(const Self &) = delete;
  UnorderedVariable(Self &&) = delete;
  ~UnorderedVariable() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::SymbolType type) {
  using namespace rq;
  using ST = SymbolType;
  switch (type) {
  case ST::BFLOAT16:
    return llvm::APFloat::BFloat();
  case ST::BINARY16:
    return llvm::APFloat::IEEEhalf();
  case ST::BINARY32:
    return llvm::APFloat::IEEEsingle();
  case ST::BINARY64:
    return llvm::APFloat::IEEEdouble();
  case ST::BINARY128:
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
  ERROR_VALUE_TOO_BIG,
  ERROR_BASE_TOO_BIG,
  ERROR_MULTIPLE_DECIMAL_POINT,
  ERROR_INTEGER_WITH_DECIMAL_POINT,
  ERROR_FLOAT_WITH_BASE
};

[[nodiscard]] constexpr llvm::StringRef
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
  case NR::ERROR_VALUE_TOO_BIG:
    return "numeric literal value is too big";
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
[[nodiscard]] inline rq::NumericResult
getNumericValue(llvm::StringRef text, NumericParam &ost_value) {
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
    Unsigned unsigned_value = 0;
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_value;
        unsigned_value = 0;
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
        if (unsigned_value > max_before_multiply) {
          return rq::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value *= base;
        if (unsigned_value > max_before_add) {
          return rq::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value += digit_base_multiplier;
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
        if (unsigned_value > signed_max) {
          return rq::NumericResult::ERROR_VALUE_TOO_BIG;
        }
      }
    }
    ost_value = std::bit_cast<Numeric>(unsigned_value);
    return rq::NumericResult::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt>) {
    const unsigned bit_width = ost_value.getBitWidth();
    llvm::APInt max_base = llvm::APInt(bit_width, rq::MAX_BASE);
    llvm::APInt min_upper_base = llvm::APInt(bit_width, rq::MIN_UPPER_BASE);
    const llvm::APInt unsigned_max = llvm::APInt::getMaxValue(bit_width);
    llvm::APInt base = llvm::APInt(bit_width, 10);
    llvm::APInt max_digit_multiplier = base - 1;
    llvm::APInt max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
    llvm::APInt max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    llvm::APInt unsigned_value = llvm::APInt(bit_width, 0);
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_value;
        unsigned_value = 0;
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
        if (unsigned_value.ugt(max_before_multiply)) {
          return rq::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value *= base;
        if (unsigned_value.ugt(max_before_add)) {
          return rq::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResult::ERROR_NO_DIGITS;
    }
    ost_value = unsigned_value;
    return rq::NumericResult::OK;
  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    std::ignore = rq::cleanFloatText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), ost_value, 10);
  } else {
    static_assert(false, "type not supported");
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &ost_value,
                rq::SymbolType symbol_type) {
  RQ_ASSERT(rq::getIsFloat(symbol_type), "not float type");
  llvm::SmallString<16> buffer;
  rq::NumericResult result = rq::cleanFloatText(text, buffer);
  if (result != rq::NumericResult::OK) {
    return result;
  }
  const llvm::fltSemantics &llvm_semantics =
      rq::getLlvmFloatSemantics(symbol_type);
  ost_value = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

} // namespace rq