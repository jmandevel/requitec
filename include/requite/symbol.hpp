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
  LOCAL,
  GLOBAL,
  PROPERTY,
  PROCEDURE,
  MODULE,
  IMPORT,
  USE
};

// detail/symbol.hpp
[[nodiscard]] inline llvm::StringRef getName(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getHasDepth(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getHasUserAttributeFlags(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getIsLiteral(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getIsInteger(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getIsFloat(requite::RootSymbolType type);

[[nodiscard]] constexpr bool getIsCodeunit(requite::RootSymbolType type);

struct Signature;
struct Tuple;
struct AnonymousObject;
struct Scope;
struct Object;
struct Table;
struct Local;
struct Global;
struct Property;
struct Procedure;
struct Module;
struct Import;
struct Use;

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
    requite::Local *_local_ptr;
    requite::Global *_global_ptr;
    requite::Property *_property_ptr;
    requite::Procedure *_procedure_ptr;
    requite::Module *_module_ptr;
    requite::Import *_import_ptr;
    requite::Use *_use_ptr;
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
  void setAsUser(requite::Local &variable);
  void setAsUser(requite::Global &variable);
  void setAsUser(requite::Property &variable);
  void setAsUser(requite::Procedure &procedure);
  void setAsUser(requite::Module &module);
  void setAsUser(requite::Import &import);
  void setAsUser(requite::Use& use);
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
  [[nodiscard]] bool getIsLocal() const;
  [[nodiscard]] bool getIsGlobal() const;
  [[nodiscard]] bool getIsProcedure() const;
  [[nodiscard]] bool getIsModule() const;
  [[nodiscard]] bool getIsHeader() const;
  [[nodiscard]] bool getIsImport() const;
  [[nodiscard]] bool getIsUse() const;
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
  [[nodiscard]] bool getHasLocal() const;
  [[nodiscard]] requite::Local &getLocal();
  [[nodiscard]] const requite::Local &getLocal() const;
  [[nodiscard]] bool getHasGlobal() const;
  [[nodiscard]] requite::Global &getGlobal();
  [[nodiscard]] const requite::Global &getGlobal() const;
  [[nodiscard]] bool getHasProcedure() const;
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] bool getHasModule() const;
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Import &getImport() const;
  [[nodiscard]] requite::Import &getImport();
  [[nodiscard]] const requite::Use &getUse() const;
  [[nodiscard]] requite::Use &getUse();
  [[nodiscard]] requite::AttributeFlags &getUserAttributeFlags();
};

enum class SubSymbolType {
  NONE,
  ARRAY,
  REFERENCE,
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
  bool _has_INFERENCE_count = false;

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
  void setHasTacitCount();
  [[nodiscard]] bool getHasTacitCount() const;
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
  [[nodiscard]] bool getHasOnlyReferenceSubtypes() const;
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