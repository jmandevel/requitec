#pragma once

#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/Twine.h>

#include <functional>

namespace rq {

struct Context;
struct Module;

struct Situator final {
  using Self = rq::Situator;

  std::reference_wrapper<rq::Context> _context_ref;

  Situator(rq::Context &context) : _context_ref(context) {}
  Situator(const Self &) = delete;
  Situator(Self &&) = delete;
  ~Situator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] rq::Expression &makeModuleTop(rq::Module &module);
  [[nodiscard]] bool situateModule(rq::Module &module);
  [[nodiscard]] bool situateTree(rq::Situation situation,
                                 rq::Expression &expression);
  [[nodiscard]] bool situateValueBranch(rq::Situation branch_situation,
                                        rq::Expression &branch);
  [[nodiscard]] bool situateHeaderBranch(rq::Situation branch_situation,
                                         rq::Expression &branch);
  [[nodiscard]] bool situateStatementBranch(rq::Expression &branch);
  [[nodiscard]] bool situateNullaryExpression(rq::Situation situation,
                                              rq::Expression &expression);
  [[nodiscard]] bool
  situateNullaryOrUnaryValueBranches(rq::Situation situation,
                                     rq::Expression &expression,
                                     rq::Situation branch0_situation);
  [[nodiscard]] bool situateUnaryValueBranches(rq::Situation situation,
                                               rq::Expression &expression,
                                               rq::Situation branch0_situation);
  [[nodiscard]] bool situateBinaryValueBranches(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNaryValueBranches(rq::Situation situation,
                                              rq::Expression &expression,
                                              unsigned minimum_branch_count,
                                              rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentFirstValueBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentLastValueBranches(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation,
      rq::Situation last_situation);
  [[nodiscard]] bool situateUnsituatedAscribeExpression(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Keyword situated_keyword,
      rq::Situation branchn_situation, rq::Situation last_situation);
  [[nodiscard]] bool situateNaryDifferentFirstAndLastValueBranches(
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
  situateFirstHeaderNaryStatementBranches(rq::Situation situation,
                                          rq::Expression &expression,
                                          rq::Situation branch0_situation);
  [[nodiscard]] bool situateFirstAndSecondHeaderNaryStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateNaryDifferentFirstHeaderNaryStatementBranches(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation header0_situation, rq::Situation headern_situation);
  [[nodiscard]] bool situateNamedMemberProcedure(rq::Situation situation,
                                                 rq::Expression &expression);
};

} // namespace rq
