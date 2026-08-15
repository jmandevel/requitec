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
  [[nodiscard]] bool situateModule(rq::ModuleFactory &factory);
  [[nodiscard]] bool situateTree(rq::Situation situation,
                                 rq::Expression &expression);
  [[nodiscard]] bool situateTagBranch(rq::Situation branch_situation,
                                      rq::Expression &branch);
  [[nodiscard]] bool situateStatementBranch(rq::Expression &branch);
  [[nodiscard]] bool situateChainLinkBranch(rq::Expression &branch);
  [[nodiscard]] bool situateVignetteBranch(rq::Expression &branch);
  [[nodiscard]] bool situateVignetteOrRvalueBranch(rq::Expression &branch);
  [[nodiscard]] bool situateNullary(rq::Situation situation,
                                              rq::Expression &expression);
  [[nodiscard]] bool
  situateNullaryOrUnaryTag(rq::Situation situation,
                                     rq::Expression &expression,
                                     rq::Situation branch0_situation);
  [[nodiscard]] bool situateUnaryTag(rq::Situation situation,
                                               rq::Expression &expression,
                                               rq::Situation branch0_situation);
  [[nodiscard]] bool situateUnaryOrBinaryTag(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateBinaryTag(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation);
  [[nodiscard]] bool situateBinaryOrTernaryTag(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation,
      rq::Situation branch2_situation);
  [[nodiscard]] bool situateTernaryTag(
      rq::Situation situation, rq::Expression &expression,
      rq::Situation branch0_situation, rq::Situation branch1_situation,
      rq::Situation branch2_situation);
  [[nodiscard]] bool situateNaryTag(rq::Situation situation,
                                              rq::Expression &expression,
                                              unsigned minimum_branch_count,
                                              rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentFirstTag(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branch0_situation,
      rq::Situation branchn_situation);
  [[nodiscard]] bool situateNaryDifferentLastTag(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Situation branchn_situation,
      rq::Situation last_situation);
  [[nodiscard]] bool situateUnsituatedAscribeExpression(
      rq::Situation situation, rq::Expression &expression,
      unsigned minimum_branch_count, rq::Keyword situated_keyword,
      rq::Situation branchn_situation, rq::Situation last_situation);
  [[nodiscard]] bool situateNaryDifferentFirstAndLastTag(
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
  // scope, block, else, default, main
  [[nodiscard]] bool situateStatement(rq::Expression &expression);
  // function, function, implement_function, use_function, class, enum, adapter
  [[nodiscard]] bool
  situateNameStatementTagStatement(rq::Situation situation,
                                           rq::Expression &expression);
  // switch, match, if, else_if, while, forward, backward
  [[nodiscard]] bool
  situateStatementTagStatement(rq::Situation situation,
                                       rq::Expression &expression);
  // case, fold
  [[nodiscard]] bool
  situateStatementMultiTagStatement(rq::Situation situation,
                                            rq::Situation tag_situation,
                                            rq::Expression &expression);
  // if_chainlink, swtich_chainlink, match_chainlink, spin_chainlink
  [[nodiscard]] bool situateChainBranches(rq::Situation situation,
                                          rq::Expression &expression,
                                          rq::Situation start_chainlink,
                                          rq::Situation continue_chainlink,
                                          rq::Situation finish_chainlink);
  // weave
  [[nodiscard]] bool
  situateStatementVignetteStatement(rq::Situation situation,
                                            rq::Expression &expression);
  // for
  [[nodiscard]] bool
  situateStatementMultiVignetteStatement(rq::Situation situation,
                                                 rq::Expression &expression);
  // interface, namespace
  [[nodiscard]] bool stiuateNameStatement(rq::Situation situation,
                                                  rq::Expression &expression,
                                                  rq::Situation name_situation);
};

} // namespace rq
