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
  [[nodiscard]] rq::Expression &makeModuleTrunk(rq::ModuleSymbol &module);
  [[nodiscard]] bool situateModule(rq::ModuleSymbol &module);
  [[nodiscard]] bool situateTree(rq::Situation situation,
                                 rq::Expression &expression);
  [[nodiscard]] bool situateNonStatementBranch(rq::Situation branch_situation,
                                               rq::Expression &branch);
  [[nodiscard]] bool situateHeaderBranch(rq::Situation branch_situation,
                                         rq::Expression &branch);
  [[nodiscard]] bool situateStatementBranch(rq::Expression &branch);
  [[nodiscard]] bool situateNullaryExpression(rq::Situation situation,
                                              rq::Expression &expression);
  [[nodiscard]] bool
  situateNullaryOrUnaryNonStatementBranches(rq::Situation situation,
                                   rq::Expression &expression,
                                   rq::Situation branch0_situation);
  [[nodiscard]] bool
  situateUnaryNonStatementBranches(rq::Situation situation,
                                   rq::Expression &expression,
                                   rq::Situation branch0_situation);
  [[nodiscard]] bool situateBinaryNonStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNaryNonStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentFirstNonStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentLastNonStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation,
      rq::Situation last_situation);
  [[nodiscard]] bool situateNaryDifferentFirstAndLastNonStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation, rq::Situation last_situation);
  [[nodiscard]] bool
  situateNaryDifferentFirstParamterBranches(rq::Situation situation,
                                            rq::Expression &expression,
                                            rq::Situation branch0_situation);
  [[nodiscard]] bool situateNaryParameterBranches(rq::Situation situation,
                                                  rq::Expression &expression);
  [[nodiscard]] bool
  situateNaryFromFirstParameterBranches(rq::Situation situation,
                                        rq::Expression &expression,
                                        rq::Expression &first_parameter);
  [[nodiscard]] bool situateNaryStatementBranches(rq::Expression &expression);
  [[nodiscard]] bool
  situateNaryHeaderFirstStatementBranches(rq::Situation situation,
                                          rq::Expression &expression,
                                          rq::Situation branch0_situation);
  [[nodiscard]] bool situateNaryHeaderFirstAndSecondStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNamedMemberProcedure(rq::Situation situation,
                                                 rq::Expression &expression);
};

} // namespace rq
