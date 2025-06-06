// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/situation.hpp>

#include <llvm/ADT/Twine.h>

#include <functional>

namespace requite {

struct Expression;
struct Context;
struct Module;

struct Situator final {
  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  bool _is_ok;

  // situate_ast.cpp
  Situator(requite::Context &context, requite::Module &module);
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  requite::Module &getModule();
  [[nodiscard]]
  const requite::Module &getModule() const;
  void setNotOk();
  [[nodiscard]]
  bool getIsOk() const;
  void setIsOk();
  [[nodiscard]]
  bool situateAst();
  void insertModuleRoot();

  // detail/situate/situate.hpp
  template <requite::Situation SITUATION_PARAM>
  inline void situateExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateBranch(llvm::Twine log_context, requite::Expression &outer,
                            unsigned branch_i, requite::Expression &branch);
  template <requite::Situation SITUATION_PARAM>
  inline void situateNullaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM,
            requite::Situation BRANCH_SITUATION_PARAM>
  inline void situateUnaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM,
            requite::Situation BRANCH_SITUATION_PARAM>
  inline void situateBinaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM,
            requite::Situation BRANCH_SITUATION_A_PARAM,
            requite::Situation BRANCH_SITUATION_B_PARAM>
  inline void situateBinaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_A_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_A_PARAM,
            requite::Situation BRANCH_SITUATION_B_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_A_PARAM,
            requite::Situation BRANCH_SITUATION_B_PARAM,
            requite::Situation BRANCH_SITUATION_C_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM,
            requite::Situation BRANCH_SITUATION_LAST_PARAM>
  inline void situateNaryWithLastExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            requite::Situation BRANCH_SITUATION_A_PARAM,
            requite::Situation BRANCH_SITUATION_N_PARAM,
            requite::Situation BRANCH_SITUATION_LAST_PARAM>
  inline void situateNaryWithLastExpression(requite::Expression &expression);

  template <requite::Situation SITUATION_PARAM>
  inline void
  situate_BindValueOrDefaultValueExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void
  situate_BindSymbolOrDefaultSymbolExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void
  situate_InferencedTypeOrIndeterminateExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_ReflectValueExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_ReflectSymbolExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void
  situateAssignArithmeticExpression(requite::Expression &expression,
                                    requite::Opcode arithmetic_opcode);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_DefaultValueExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_DefaultSymbolExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateArgumentBranches(requite::Expression &expression,
                                      requite::Expression &first,
                                      unsigned branch_i);
  template <requite::Situation SITUATION_PARAM>
  inline void situateParameterBranches(requite::Expression &expression,
                                       requite::Expression &first,
                                       unsigned branch_i);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_TupleValue(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_TupleType(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_TripExpression(requite::Expression &expression);
    template <requite::Situation SITUATION_PARAM>
  inline void situate_CallExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_SignatureExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void
  situate_CallOrSignatureExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateVariableExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situatePropertyExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateConstantExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateSizedPrimitiveExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateArrayExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateAssertExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_IdentifyExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situate_ConduitExpression(requite::Expression &expression);
  template <requite::Situation SITUATION_PARAM>
  inline void situateMangledNameExpression(requite::Expression &expression);
};

} // namespace requite

#include <requite/detail/situate.hpp>
