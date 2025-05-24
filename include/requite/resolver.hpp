// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>

namespace requite {

struct Context;
struct Module;
struct Alias;
struct Procedure;
struct Object;
struct Property;
struct Variable;
struct Symbol;
struct Expression;
struct Scope;
struct Tuple;
struct AnonymousObject;
struct Signature;

struct Resolver final {
  using Self = requite::Resolver;

  // TODO use llvm::SmallPtrSet<void*, 4> in order to check for circular
  // references
  std::reference_wrapper<requite::Context> _context_ref;
  bool _is_ok = true;

  // resolver.cpp
  Resolver(requite::Context &context);
  Resolver(const Self &) = delete;
  Resolver(Self &&) = delete;
  ~Resolver() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  bool getIsOk() const;
  void setNotOk();

  // resolve_symbols.cpp
  void resolveSymbols();
  void resolveModuleSymbols(requite::Module &module);
  void resolveAlias(requite::Alias &alias);
  void resolveProcedure(requite::Procedure &procedure);
  void resolveVariable(requite::Variable &variable);
  [[nodiscard]] bool
  resolveCompileTimeUnsignedInteger(unsigned &uint,
                                    requite::Expression &expression);

  // resolve_matte_symbol_expressions.cpp
  [[nodiscard]] bool
  resolveMatteSymbolExpression(requite::Symbol &symbol,
                               requite::Scope &containing_scope,
                               requite::Expression &expression);
  [[nodiscard]] bool resolveMatteSymbolIdentifierLiteralExpression(
      requite::Symbol &symbol, requite::Scope &containing_scope,
      requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolReflectValueExpression(requite::Symbol &symbol,
                                           requite::Scope &containing_scope,
                                           requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolReflectSymbolExpression(requite::Symbol &symbol,
                                            requite::Scope &containing_scope,
                                            requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolAscribeExpression(requite::Symbol &symbol,
                                      requite::Scope &containing_scope,
                                      requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolFatPointerExpression(requite::Symbol &symbol,
                                         requite::Scope &containing_scope,
                                         requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolArrayExpression(requite::Symbol &symbol,
                                    requite::Scope &containing_scope,
                                    requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolReferenceExpression(requite::Symbol &symbol,
                                        requite::Scope &containing_scope,
                                        requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolStolenReferenceExpression(requite::Symbol &symbol,
                                              requite::Scope &containing_scope,
                                              requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolPointerExpression(requite::Symbol &symbol,
                                      requite::Scope &containing_scope,
                                      requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolTupleExpression(requite::Symbol &symbol,
                                    requite::Scope &containing_scope,
                                    requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolTupleExpression(requite::Tuple &tuple,
                                    requite::Scope &containing_scope,
                                    requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolAnonymousObjectExpression(requite::Symbol &symbol,
                                              requite::Scope &containing_scope,
                                              requite::Expression &expression);
  [[nodiscard]] bool resolveMatteSymbolAnonymousObjectExpression(
      requite::AnonymousObject &anonymous_object,
      requite::Scope &containing_scope, requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolSignatureExpression(requite::Symbol &symbol,
                                        requite::Scope &containing_scope,
                                        requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolSignatureExpression(requite::Signature &signature,
                                        requite::Scope &containing_scope,
                                        requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolNullExpression(requite::Symbol &symbol,
                                   requite::Scope &containing_scope,
                                   requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolInferenceExpression(requite::Symbol &symbol,
                                        requite::Scope &containing_scope,
                                        requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolVoidExpression(requite::Symbol &symbol,
                                   requite::Scope &containing_scope,
                                   requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolBooleanExpression(requite::Symbol &symbol,
                                      requite::Scope &containing_scope,
                                      requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolWordExpression(requite::Symbol &symbol,
                                   requite::Scope &containing_scope,
                                   requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolSignedIntegerExpression(requite::Symbol &symbol,
                                            requite::Scope &containing_scope,
                                            requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolUnsignedIntegerExpression(requite::Symbol &symbol,
                                              requite::Scope &containing_scope,
                                              requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolBinaryHalfFloatExpression(requite::Symbol &symbol,
                                              requite::Scope &containing_scope,
                                              requite::Expression &expression);
  [[nodiscard]] bool resolveMatteSymbolBinarySingleFloatExpression(
      requite::Symbol &symbol, requite::Scope &containing_scope,
      requite::Expression &expression);
  [[nodiscard]] bool resolveMatteSymbolBinaryDoubleFloatExpression(
      requite::Symbol &symbol, requite::Scope &containing_scope,
      requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolBinaryQuadFloatExpression(requite::Symbol &symbol,
                                              requite::Scope &containing_scope,
                                              requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolCCharExpression(requite::Symbol &symbol,
                                    requite::Scope &containing_scope,
                                    requite::Expression &expression);
  [[nodiscard]] bool
  resolveMatteSymbolUtf8Expression(requite::Symbol &symbol,
                                   requite::Scope &containing_scope,
                                   requite::Expression &expression);
  [[nodiscard]] bool resolveMatteSymbolVariadicArgumentsExpression(
      requite::Symbol &symbol, requite::Scope &containing_scope,
      requite::Expression &expression);

  // resolve_value_reflective_symbol_expressions.cpp
  [[nodiscard]] bool
  resolveValueReflectiveSymbolExpression(requite::Symbol &symbol,
                                         requite::Scope &scope,
                                         requite::Expression &expression);

  // resolve_symbol_reflective_symbol_expressions.cpp
  [[nodiscard]] bool
  resolveSymbolReflectiveSymbolExpression(requite::Symbol &symbol,
                                         requite::Scope &scope,
                                         requite::Expression &expression);
};

} // namespace requite