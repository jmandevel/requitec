// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/opcode.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace requite {

enum class RootSymbolType {
  NONE,

  // SPECIAL
  INFERENCE,
  VOID,
  VARIADIC_ARGUMENTS,
  NULL_,

  // PRIMITIVE
  BOOLEAN,
  WORD,
  SIGNED_INTEGER,
  UNSIGNED_INTEGER,
  BINARY_HALF_FLOAT,
  BINARY_SINGLE_FLOAT,
  BINARY_DOUBLE_FLOAT,
  BINARY_QUAD_FLOAT,
  C_CHAR,
  UTF8,

  // STRUCTURED
  SIGNATURE,
  TUPLE,
  ANONYMOUS_OBJECT,

  // USER
  SCOPE,
  OBJECT,
  TABLE,
  ALIAS,
  ORDERED_VARIABLE,
  UNORDERED_VARIABLE,
  PROCEDURE,
  NAMED_PROCEDURE_GROUP,
  MODULE,
  LABEL
};

// detail/symbol.hpp
[[nodiscard]] constexpr bool getHasDepth(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getHasUserAttributeFlags(requite::RootSymbolType type);

struct Signature;
struct Tuple;
struct AnonymousObject;
struct Scope;
struct Object;
struct Table;
struct Alias;
struct OrderedVariable;
struct UnorderedVariable;
struct Procedure;
struct NamedProcedureGroup;
struct Module;
struct Label;

struct RootSymbol final {
  using Self = requite::RootSymbol;

  requite::RootSymbolType _type = requite::RootSymbolType::NONE;
  unsigned _depth;
  union {
    void *_data_ptr = nullptr;
    requite::Signature *_signature_ptr;
    requite::Tuple *_tuple_ptr;
    requite::AnonymousObject *_anonymous_object_ptr;
    requite::Scope *_scope_ptr;
    requite::Object *_object_ptr;
    requite::Table *_table_ptr;
    requite::Alias *_alias_ptr;
    requite::OrderedVariable *_ordered_variable_ptr;
    requite::UnorderedVariable *_unordered_variable_ptr;
    requite::Procedure *_procedure_ptr;
    requite::NamedProcedureGroup *_named_procedure_group_ptr;
    requite::Module *_module_ptr;
    requite::Label *_label_ptr;
  };

  // root_symbol.cpp
  RootSymbol() = default;
  RootSymbol(const Self &that);
  RootSymbol(Self &&that) = default;
  ~RootSymbol();
  Self &operator=(const Self &rhs);
  Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] static Self makeUser(requite::Scope &scope);
  [[nodiscard]] static Self makeUser(requite::Object &object);
  [[nodiscard]] static Self makeUser(requite::Table &table);
  [[nodiscard]] static Self makeUser(requite::Alias &alias);
  [[nodiscard]] static Self makeUser(requite::OrderedVariable &variable);
  [[nodiscard]] static Self makeUser(requite::UnorderedVariable &variable);
  [[nodiscard]] static Self makeUser(requite::Procedure &procedure);
  [[nodiscard]] static Self
  makeUser(requite::NamedProcedureGroup &procedure_group);
  [[nodiscard]] static Self makeUser(requite::Module &module);
  [[nodiscard]] static Self makeUser(requite::Label &label);
  void setType(requite::RootSymbolType type);
  [[nodiscard]] requite::RootSymbolType getType() const;
  [[nodiscard]] bool getHasDepth() const;
  void setDepth(unsigned depth);
  [[nodiscard]] unsigned getDepth() const;
  [[nodiscard]] bool getIsNone() const;
  [[nodiscard]] bool getIsInference() const;
  [[nodiscard]] bool getIsVoid() const;
  [[nodiscard]] bool getIsVariadicArguments() const;
  [[nodiscard]] bool getIsBoolean() const;
  [[nodiscard]] bool getIsWord() const;
  [[nodiscard]] bool getIsSignedInteger() const;
  [[nodiscard]] bool getIsUnsignedInteger() const;
  [[nodiscard]] bool getIsBinaryHalfFloat() const;
  [[nodiscard]] bool getIsBinarySingleFloat() const;
  [[nodiscard]] bool getIsBinaryDoubleFloat() const;
  [[nodiscard]] bool getIsBinaryQuadFloat() const;
  [[nodiscard]] bool getIsCChar() const;
  [[nodiscard]] bool getIsUTF8() const;
  [[nodiscard]] bool getIsSignature() const;
  [[nodiscard]] bool getIsTuple() const;
  [[nodiscard]] bool getIsAnonymousObject() const;
  [[nodiscard]] bool getIsScope() const;
  [[nodiscard]] bool getIsObject() const;
  [[nodiscard]] bool getIsTable() const;
  [[nodiscard]] bool getIsAlias() const;
  [[nodiscard]] bool getIsOrderedVariable() const;
  [[nodiscard]] bool getIsUnorderedVariable() const;
  [[nodiscard]] bool getIsProcedure() const;
  [[nodiscard]] bool getIsNamedProcedureGroup() const;
  [[nodiscard]] bool getIsModule() const;
  [[nodiscard]] bool getIsHeader() const;
  [[nodiscard]] bool getIsLabel() const;
  [[nodiscard]] const requite::Signature &getSignature() const;
  [[nodiscard]] requite::Signature &getSignature();
  [[nodiscard]] const requite::Tuple &getTuple() const;
  [[nodiscard]] requite::Tuple &getTuple();
  [[nodiscard]] const requite::AnonymousObject &getAnonymousObject() const;
  [[nodiscard]] requite::AnonymousObject &getAnonymousObject();
  [[nodiscard]] bool getHasScope() const;
  void setScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasObject() const;
  void setObject(requite::Object &object);
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] bool getHasTable() const;
  void setTable(requite::Table &table);
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] bool getHasAlias() const;
  void setAlias(requite::Alias &alias);
  [[nodiscard]] const requite::Alias &getAlias() const;
  [[nodiscard]] requite::Alias &getAlias();
  [[nodiscard]] bool getHasOrderedVariable() const;
  void setOrderedVariable(requite::OrderedVariable &variable);
  [[nodiscard]] requite::OrderedVariable &getOrderedVariable();
  [[nodiscard]] const requite::OrderedVariable &getOrderedVariable() const;
  [[nodiscard]] bool getHasUnorderedVariable() const;
  void setUnorderedVariable(requite::UnorderedVariable &variable);
  [[nodiscard]] requite::UnorderedVariable &getUnorderedVariable();
  [[nodiscard]] const requite::UnorderedVariable &getUnorderedVariable() const;
  [[nodiscard]] bool getHasProcedure() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] bool getHasNamedProcedureGroup() const;
  void
  setNamedProcedureGroup(requite::NamedProcedureGroup &named_procedure_group);
  [[nodiscard]] const requite::NamedProcedureGroup &
  getNamedProcedureGroup() const;
  [[nodiscard]] requite::NamedProcedureGroup &getNamedProcedureGroup();
  [[nodiscard]] bool getHasModule() const;
  void setModule(requite::Module &module);
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] bool getHasLabel() const;
  void setLabel(requite::Label &label);
  [[nodiscard]] const requite::Label &getLabel() const;
  [[nodiscard]] requite::Label &getLabel();
  [[nodiscard]] requite::AttributeFlags &getUserAttributeFlags();
};

enum class SubSymbolType {
  NONE,
  ARRAY,
  REFERENCE,
  STOLEN_REFERENCE,
  POINTER,
  FAT_POINTER
};

[[nodiscard]] constexpr requite::SubSymbolType
getSubSymbolType(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasCount(requite::SubSymbolType type);

struct SubSymbol final {
  using Self = requite::SubSymbol;

  requite::SubSymbolType _type = requite::SubSymbolType::NONE;
  requite::AttributeFlags _attributes = {};
  unsigned _count = 0;
  bool _has_inferenced_count = false;

  // sub_symbol.cpp
  SubSymbol() = default;
  SubSymbol(const Self &symbol) = default;
  SubSymbol(Self &&symbol) = default;
  ~SubSymbol() = default;
  [[nodiscard]] Self &operator=(const Self &rhs) = default;
  [[nodiscard]] Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &symrhsbol) const;
  void setType(requite::SubSymbolType type);
  [[nodiscard]] requite::SubSymbolType getType() const;
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  void setCount(unsigned count);
  [[nodiscard]] unsigned getCount() const;
  void setHasInferencedCount();
  [[nodiscard]] bool getHasInferencedCount() const;
};

struct Symbol {
  using Self = requite::Symbol;

  requite::RootSymbol _root = {};
  requite::AttributeFlags _root_attributes = {};
  std::vector<requite::SubSymbol> _subs = {};

  // symbol.cpp
  Symbol() = default;
  Symbol(requite::RootSymbol &root);
  Symbol(requite::RootSymbol &root,
                  llvm::ArrayRef<requite::SubSymbol> subs);
  Symbol(const Self &that) = default;
  Symbol(Self &&that) = default;
  ~Symbol() = default;
  Self &operator=(const Self &rhs) = default;
  Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getIsEmpty() const;
  [[nodiscard]] requite::RootSymbol &getRoot();
  [[nodiscard]] const requite::RootSymbol &getRoot() const;
  [[nodiscard]] requite::AttributeFlags &getRootAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getRootAttributeFlags() const;
  [[nodiscard]] std::vector<requite::SubSymbol> &getSubs();
  [[nodiscard]] const std::vector<requite::SubSymbol> &getSubs() const;
  void wrapSymbol(const requite::Symbol &symbol);
  void applyAttributeFlags(const requite::AttributeFlags &attributes);
  [[nodiscard]] requite::SubSymbol &makeSubSymbol();

  [[nodiscard]] bool getIsPointer() const;
  [[nodiscard]] bool getIsInteger() const;
  [[nodiscard]] bool getIsFloat() const;

  // detail/symbol.hpp
  template<unsigned BUFFER_SIZE_PARAM>
  [[nodiscard]] llvm::StringRef getName(llvm::SmallString<BUFFER_SIZE_PARAM>& buffer) const;
};

} // namespace requite

#include <requite/detail/symbol.hpp>