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

  // LITERAL
  INTEGER_LITERAL,
  FRACTIONAL_LITERAL,
  CODEUNIT_LITERAL,
  STRING_LITERAL,

  // PRIMITIVE
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
    void *_nothing_ptr = nullptr;
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
  void setAsUser(requite::Scope &scope);
  void setAsUser(requite::Object &object);
  void setAsUser(requite::Table &table);
  void setAsUser(requite::Alias &alias);
  void setAsUser(requite::OrderedVariable &variable);
  void setAsUser(requite::UnorderedVariable &variable);
  void setAsUser(requite::Procedure &procedure);
  void
  setAsUser(requite::NamedProcedureGroup &procedure_group);
  void setAsUser(requite::Module &module);
  void setAsUser(requite::Label &label);
  void setAsInference();
  void setAsVoid();
  void setAsVariadicArguments();
  void setAsNull_();
  void setAsIntegerLiteral();
  void setAsFractionalLiteral();
  void setAsCodeunitLiteral();
  void setAsStringLiteral();
  void setAsBoolean();
  void setAsWord(unsigned depth);
  void setAsSigned(unsigned depth);
  void setAsUnsigned(unsigned depth);
  void setAsBFloat16();
  void setAsBinary16();
  void setAsBinary32();
  void setAsBinary64();
  void setAsBinary128();
  void setAsUtf8();
  void setAsSignature();
  void setAsTuple();
  void setAsAnonymousObject();
  [[nodiscard]] requite::RootSymbolType getType() const;
  [[nodiscard]] bool getHasDepth() const;
  [[nodiscard]] unsigned getDepth() const;
  [[nodiscard]] bool getIsNone() const;
  [[nodiscard]] bool getIsInference() const;
  [[nodiscard]] bool getIsVoid() const;
  [[nodiscard]] bool getIsVariadicArguments() const;
  [[nodiscard]] bool getIsBoolean() const;
  [[nodiscard]] bool getIsWord() const;
  [[nodiscard]] bool getIsSignedInteger() const;
  [[nodiscard]] bool getIsUnsignedInteger() const;
  [[nodiscard]] bool getIsBFloat16() const;
  [[nodiscard]] bool getIsBinary16() const;
  [[nodiscard]] bool getIsBinary32() const;
  [[nodiscard]] bool getIsBinary64() const;
  [[nodiscard]] bool getIsBinary128() const;
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
  [[nodiscard]] bool getHasAllocation() const;
  [[nodiscard]] const requite::Signature &getSignature() const;
  [[nodiscard]] requite::Signature &getSignature();
  [[nodiscard]] const requite::Tuple &getTuple() const;
  [[nodiscard]] requite::Tuple &getTuple();
  [[nodiscard]] const requite::AnonymousObject &getAnonymousObject() const;
  [[nodiscard]] requite::AnonymousObject &getAnonymousObject();
  [[nodiscard]] bool getHasScope() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasObject() const;
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] bool getHasTable() const;
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] bool getHasAlias() const;
  [[nodiscard]] const requite::Alias &getAlias() const;
  [[nodiscard]] requite::Alias &getAlias();
  [[nodiscard]] bool getHasOrderedVariable() const;
  [[nodiscard]] requite::OrderedVariable &getOrderedVariable();
  [[nodiscard]] const requite::OrderedVariable &getOrderedVariable() const;
  [[nodiscard]] bool getHasUnorderedVariable() const;
  [[nodiscard]] requite::UnorderedVariable &getUnorderedVariable();
  [[nodiscard]] const requite::UnorderedVariable &getUnorderedVariable() const;
  [[nodiscard]] bool getHasProcedure() const;
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] bool getHasNamedProcedureGroup() const;
  [[nodiscard]] const requite::NamedProcedureGroup &
  getNamedProcedureGroup() const;
  [[nodiscard]] requite::NamedProcedureGroup &getNamedProcedureGroup();
  [[nodiscard]] bool getHasModule() const;
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] bool getHasLabel() const;
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
  requite::Alias* _resolved_alias_ptr = nullptr;

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
  void resolveAlias();
  [[nodiscard]] bool getHasResolvedAlias() const;
  [[nodiscard]] requite::Alias& getResolvedAlias();
  [[nodiscard]] const requite::Alias& getResolvedAlias() const;

  [[nodiscard]] bool getIsPointer() const;
  [[nodiscard]] bool getIsInteger() const;
  [[nodiscard]] bool getIsFloat() const;

  // detail/symbol.hpp
  template<unsigned BUFFER_SIZE_PARAM>
  [[nodiscard]] llvm::StringRef getName(llvm::SmallString<BUFFER_SIZE_PARAM>& buffer) const;
};

} // namespace requite

#include <requite/detail/symbol.hpp>