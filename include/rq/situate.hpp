#pragma once

#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/Twine.h>

#include <functional>

namespace rq {

struct Context;
struct ModuleSymbol;

struct Situator final {
  using Self = rq::Situator;

  std::reference_wrapper<rq::Context> _context_ref;

  Situator(rq::Context &context)
      : _context_ref(context) {}
  Situator(const Self &) = delete;
  Situator(Self &&) = delete;
  ~Situator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] rq::Node &makeModuleTrunk(rq::ModuleSymbol &module);
  [[nodiscard]] bool situateModule(rq::ModuleSymbol &module);
  [[nodiscard]] bool situateTree(rq::Situation situation,
                                 rq::Node &expression);
  [[nodiscard]] bool situateExpressionBranch(rq::Situation branch_situation,
                                               rq::Node &branch);
  [[nodiscard]] bool situateHeaderBranch(rq::Situation branch_situation,
                                         rq::Node &branch);
  [[nodiscard]] bool situateStatementBranch(rq::Node &branch);
  [[nodiscard]] bool situateNullaryExpression(rq::Situation situation,
                                              rq::Node &expression);
  [[nodiscard]] bool
  situateNullaryOrUnaryExpressionBranches(rq::Situation situation,
                                   rq::Node &expression,
                                   rq::Situation branch0_situation);
  [[nodiscard]] bool
  situateUnaryExpressionBranches(rq::Situation situation,
                                   rq::Node &expression,
                                   rq::Situation branch0_situation);
  [[nodiscard]] bool situateBinaryExpressionBranches(
      rq::Situation situation, rq::Node &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNaryExpressionBranches(
      rq::Situation situation, rq::Node &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentFirstExpressionBranches(
      rq::Situation situation, rq::Node &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentLastExpressionBranches(
      rq::Situation situation, rq::Node &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation,
      rq::Situation last_situation);
  [[nodiscard]] bool situateNaryDifferentFirstAndLastExpressionBranches(
      rq::Situation situation, rq::Node &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation, rq::Situation last_situation);
  [[nodiscard]] bool
  situateNaryDifferentFirstParamterBranches(rq::Situation situation,
                                            rq::Node &expression,
                                            rq::Situation branch0_situation);
  [[nodiscard]] bool situateNaryParameterBranches(rq::Situation situation,
                                                  rq::Node &expression);
  [[nodiscard]] bool
  situateNaryFromFirstParameterBranches(rq::Situation situation,
                                        rq::Node &expression,
                                        rq::Node &first_parameter);
  [[nodiscard]] bool situateNaryStatementBranches(rq::Node &expression);
  [[nodiscard]] bool
  situateNaryHeaderFirstStatementBranches(rq::Situation situation,
                                          rq::Node &expression,
                                          rq::Situation branch0_situation);
  [[nodiscard]] bool situateNaryHeaderFirstAndSecondStatementBranches(
      rq::Situation situation, rq::Node &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNamedMemberProcedure(rq::Situation situation,
                                                 rq::Node &expression);
};

} // namespace rq
