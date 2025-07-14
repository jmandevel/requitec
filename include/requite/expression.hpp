// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/const_expression_iterator.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/opcode.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <ranges>
#include <string>
#include <variant>

namespace requite {

struct Context;
struct Token;
struct Scope;
struct Table;
struct Object;
struct Local;
struct Global;
struct Property;
struct Procedure;
struct AnonymousFunction;
struct Alias;
struct Block;
struct Import;
struct Use;

struct Expression final {
  using Self = requite::Expression;

  requite::Opcode _opcode = requite::Opcode::__NONE;
  requite::Expression *_next_ptr = nullptr;
  requite::Expression *_branch_ptr = nullptr;
  const char *_source_text_ptr = nullptr;
  unsigned _source_text_length = 0;
  std::variant<std::monostate, std::string, requite::Scope *, requite::Table*, requite::Object *,
               requite::Procedure *, requite::Alias *,
               requite::AnonymousFunction *, requite::Global *,
               requite::Property *, requite::Local *, requite::Label *,
               llvm::APSInt, requite::Symbol, requite::Block *, requite::Import *, requite::Use *>
      _data = std::monostate{};

  // expression.cpp
  Expression() = default;
  Expression(Self &) = delete;
  Expression(Self &&) = delete;
  requite::Expression &operator=(const Self &) = delete;
  requite::Expression &operator=(Self &&) = delete;
  ~Expression() = default;
  [[nodiscard]] bool operator==(const Self &) const;
  [[nodiscard]] bool operator!=(const Self &) const;

  // expression_make.cpp
  static void deleteExpression(requite::Expression &expression);
  [[nodiscard]] static requite::Expression &
  copyExpression(const requite::Expression &expression);
  [[nodiscard]] static requite::Expression &makeError();
  [[nodiscard]] static requite::Expression &
  makeOperation(requite::Opcode opcode);
  [[nodiscard]] static requite::Expression &makeInteger();
  [[nodiscard]] static requite::Expression &makeFractional();
  [[nodiscard]] static requite::Expression &makeString(llvm::StringRef text);
  [[nodiscard]] static requite::Expression &makeCodeunit(llvm::StringRef text);
  [[nodiscard]] static requite::Expression &
  makeIdentifier(llvm::StringRef text);

  // detail/expression_type.hpp
  [[nodiscard]] inline bool getIsNone() const;
  [[nodiscard]] inline bool getIsOperation() const;
  [[nodiscard]] inline bool getIsInteger() const;
  [[nodiscard]] inline bool getIsFractional() const;
  [[nodiscard]] inline bool getIsNumeric() const;
  [[nodiscard]] inline bool getIsString() const;
  [[nodiscard]] inline bool getIsCodeunit() const;
  [[nodiscard]] inline bool getIsIdentifier() const;
  [[nodiscard]] inline bool getIsConverging() const;
  [[nodiscard]] inline bool getIsExpanding() const;
  [[nodiscard]] inline bool getIsInternalUseOnly() const;
  [[nodiscard]] inline requite::Opcode getOpcode() const;
  inline void setOpcode(requite::Opcode opcode);
  inline void changeOpcode(requite::Opcode opcode);

  // detail/expression_ast.hpp
  inline void clear();
  [[nodiscard]] inline bool getHasBranch() const;
  [[nodiscard]] inline bool getHasNext() const;
  [[nodiscard]] inline bool getHasOneBranch() const;
  [[nodiscard]] inline bool getHasOneNext() const;
  [[nodiscard]] inline unsigned getBranchCount() const;
  [[nodiscard]] inline unsigned getNextCount() const;
  [[nodiscard]] inline requite::Expression &getBranch();
  [[nodiscard]] inline const requite::Expression &getBranch() const;
  [[nodiscard]] inline requite::Expression *getBranchPtr();
  [[nodiscard]] inline const requite::Expression *getBranchPtr() const;
  [[nodiscard]] inline requite::Expression &getBranch(unsigned branch_i);
  [[nodiscard]] inline const requite::Expression &
  getBranch(unsigned branch_i) const;
  [[nodiscard]] inline requite::Expression &getLastBranch();
  [[nodiscard]] inline const requite::Expression &getLastBranch() const;
  [[nodiscard]] inline requite::Expression &getNext();
  [[nodiscard]] inline const requite::Expression &getNext() const;
  [[nodiscard]] inline requite::Expression *getNextPtr();
  [[nodiscard]] inline const requite::Expression *getNextPtr() const;
  [[nodiscard]] inline requite::Expression &getNext(unsigned next_i);
  [[nodiscard]] inline const requite::Expression &
  getNext(unsigned next_i) const;
  [[nodiscard]] inline requite::Expression &getLastNext();
  [[nodiscard]] inline const requite::Expression &getLastNext() const;
  [[nodiscard]] inline requite::Expression &getUnascribed();
  [[nodiscard]] inline const requite::Expression &getUnascribed() const;
  inline void setBranch(requite::Expression &branch);
  inline void setBranchPtr(requite::Expression *branch);
  inline void setNext(requite::Expression &next);
  inline void setNextPtr(requite::Expression *next);
  [[nodiscard]] inline requite::Expression &
  replaceBranch(requite::Expression &replacement);
  [[nodiscard]] inline requite::Expression &
  replaceNext(requite::Expression &replacement);
  [[nodiscard]] inline requite::Expression &popBranch();
  [[nodiscard]] inline requite::Expression *popBranchPtr();
  [[nodiscard]] inline requite::Expression &popNext();
  [[nodiscard]] inline requite::Expression *popNextPtr();
  inline void mergeBranch();
  inline void replaceWithRecursiveCopy(requite::Expression &replacement);

  // detail/expression_source.hpp
  [[nodiscard]] inline bool getHasSourceText() const;
  [[nodiscard]] inline bool getIsInsertedInSource() const;
  [[nodiscard]] inline llvm::StringRef getSourceText() const;
  [[nodiscard]] inline const char *getSourceTextPtr() const;
  [[nodiscard]] inline unsigned getSourceTextLength() const;
  [[nodiscard]] inline llvm::SMLoc getSourceStartLlvmLocation() const;
  [[nodiscard]] inline llvm::SMLoc getSourceEndLlvmLocation() const;
  [[nodiscard]] inline llvm::SMRange getSourceLlvmRange() const;
  inline void setSource(const requite::Token &token);
  inline void setSource(const requite::Token &first,
                        const requite::Token &last);
  inline void setSource(const requite::Expression &expression);
  inline void setSource(const requite::Expression &first,
                        requite::Expression &last);
  inline void setSource(const requite::Expression &first,
                        const requite::Token &last);
  inline void setSource(const requite::Token &first,
                        const requite::Expression &last);
  inline void setSourceInsertedAt(const char *text_ptr);
  inline void extendSourceOver(const requite::Token &token);
  inline void extendSourceOver(const requite::Expression &expression);
  inline void setSourceInsertedBefore(const requite::Token &token);
  inline void setSourceInsertedBefore(const requite::Expression &expression);
  inline void setSourceInsertedAfter(const requite::Token &token);
  inline void setSourceInsertedAfter(const requite::Expression &expression);

  // detail/expression_data.hpp
  inline void clearData();
  [[nodiscard]] inline bool getHasDataText() const;
  [[nodiscard]] inline llvm::StringRef getDataText() const;
  inline void setDataText(llvm::StringRef text);
  inline void changeDataText(llvm::StringRef text);
  [[nodiscard]] inline bool getHasScope() const;
  [[nodiscard]] inline requite::Scope &getScope();
  [[nodiscard]] inline const requite::Scope &getScope() const;
  void setScope(requite::Scope &scope);
  [[nodiscard]] inline bool getHasTable() const;
  [[nodiscard]] inline requite::Table &getTable();
  [[nodiscard]] inline const requite::Table &getTable() const;
  inline void setTable(requite::Table &table);
  [[nodiscard]] inline bool getHasObject() const;
  [[nodiscard]] inline requite::Object &getObject();
  [[nodiscard]] inline const requite::Object &getObject() const;
  inline void setObject(requite::Object &object);
  [[nodiscard]] inline bool getHasProcedure() const;
  [[nodiscard]] inline requite::Procedure &getProcedure();
  [[nodiscard]] inline const requite::Procedure &getProcedure() const;
  [[nodiscard]] inline bool getHasAnonymousFunction() const;
  inline void setAnonymousFunction(requite::AnonymousFunction &function);
  [[nodiscard]] inline requite::AnonymousFunction &getAnonymousFunction();
  [[nodiscard]] inline const requite::AnonymousFunction &
  getAnonymousFunction() const;
  [[nodiscard]] inline bool getHasAlias() const;
  inline void setAlias(requite::Alias &alias);
  [[nodiscard]] inline requite::Alias &getAlias();
  [[nodiscard]] inline const requite::Alias &getAlias() const;
  inline void setGlobal(requite::Global &variable);
  [[nodiscard]] inline bool getHasGlobal() const;
  [[nodiscard]] inline requite::Global &getGlobal();
  [[nodiscard]] inline const requite::Global &getGlobal() const;
  inline void setProperty(requite::Property &property);
  [[nodiscard]] inline bool getHasProperty() const;
  [[nodiscard]] inline requite::Property &getProperty();
  [[nodiscard]] inline const requite::Property &getProperty() const;
  inline void setLocal(requite::Local &variable);
  [[nodiscard]] inline bool getHasLocal() const;
  [[nodiscard]] inline requite::Local &getLocal();
  [[nodiscard]] inline const requite::Local &getLocal() const;
  inline void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] inline bool getHasLabel() const;
  inline void setLabel(requite::Label &label);
  [[nodiscard]] inline requite::Label &getLabel();
  [[nodiscard]] inline const requite::Label &getLabel() const;
  [[nodiscard]] inline bool getHasInteger() const;
  [[nodiscard]] inline llvm::APSInt &emplaceInteger();
  [[nodiscard]] inline llvm::APSInt &getInteger();
  [[nodiscard]] inline const llvm::APSInt &getInteger() const;
  [[nodiscard]] inline bool getHasBlock() const;
  inline void setBlock(requite::Block &block);
  [[nodiscard]] inline requite::Block &getBlock();
  [[nodiscard]] inline const requite::Block &getBlock() const;
  [[nodiscard]] inline bool getHasImport() const;
  inline void setImport(requite::Import &import);
  [[nodiscard]] inline requite::Import &getImport();
  [[nodiscard]] inline const requite::Import &getImport() const;
  [[nodiscard]] inline bool getHasUse() const;
  inline void setUse(requite::Use &use);
  [[nodiscard]] inline requite::Use &getUse();
  [[nodiscard]] inline const requite::Use &getUse() const;

  // detail/expression_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ExpressionIterator, requite::ExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getHorizontalSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      requite::ConstExpressionIterator, requite::ConstExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getHorizontalSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      requite::ExpressionIterator, requite::ExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getNextSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      requite::ConstExpressionIterator, requite::ConstExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getNextSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      requite::ExpressionIterator, requite::ExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getBranchSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      requite::ConstExpressionIterator, requite::ConstExpressionIterator,
      std::ranges::subrange_kind::unsized>
  getBranchSubrange() const;
};

} // namespace requite

#include <requite/detail/expression_ast.hpp>
#include <requite/detail/expression_data.hpp>
#include <requite/detail/expression_source.hpp>
#include <requite/detail/expression_subrange.hpp>
#include <requite/detail/expression_type.hpp>