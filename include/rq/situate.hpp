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
  [[nodiscard]] bool situateModule(rq::ModuleDetail &detail);
  [[nodiscard]] bool situateTree(rq::Situation situation,
                                 rq::Expression &expression);
  [[nodiscard]] bool situateTagBranch(rq::Situation branch_situation,
                                      rq::Expression &branch);
  [[nodiscard]] bool situateStatementBranch(rq::Situation situation,
                                            rq::Expression &branch);
  [[nodiscard]] bool situateChainBranch(rq::Situation situation,
                                        rq::Expression &branch);
  [[nodiscard]] bool situateVignetteBranch(rq::Expression &branch);
  [[nodiscard]] bool situateNullary(rq::Situation situation,
                                    rq::Expression &expression);
  [[nodiscard]] bool situateNullaryOrUnaryTag(rq::Situation situation,
                                              rq::Expression &expression,
                                              rq::Situation branch0_situation);
  [[nodiscard]] bool situateUnaryTag(rq::Situation situation,
                                     rq::Expression &expression,
                                     rq::Situation branch0_situation);
  [[nodiscard]] bool situateUnaryOrBinaryTag(rq::Situation situation,
                                             rq::Expression &expression,
                                             rq::Situation branch0_situation,
                                             rq::Situation branch1_situation);
  [[nodiscard]] bool situateBinaryTag(rq::Situation situation,
                                      rq::Expression &expression,
                                      rq::Situation branch0_situation,
                                      rq::Situation branch1_situation);
  [[nodiscard]] bool situateBinaryOrTernaryTag(rq::Situation situation,
                                               rq::Expression &expression,
                                               rq::Situation branch0_situation,
                                               rq::Situation branch1_situation,
                                               rq::Situation branch2_situation);
  [[nodiscard]] bool situateTernaryTag(rq::Situation situation,
                                       rq::Expression &expression,
                                       rq::Situation branch0_situation,
                                       rq::Situation branch1_situation,
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
  // function, function, implement_function, class, enum, adapter
  [[nodiscard]] bool
  situateNameStatementTagStatement(rq::Situation situation,
                                   rq::Expression &expression);
  [[nodiscard]] bool situateNameStatementTag(rq::Situation situation,
                                             rq::Expression &expression);
  // switch, match, if, else_if, while, constructor
  [[nodiscard]] bool situateStatementTagStatement(rq::Situation situation,
                                                  rq::Expression &expression);
  // case, fold
  [[nodiscard]] bool
  situateStatementMultiTagStatement(rq::Situation situation,
                                    rq::Expression &expression,
                                    rq::Situation tag_situation);
  // catch
  [[nodiscard]] bool
  situateStatementVignetteStatement(rq::Situation situation,
                                    rq::Expression &expression);
  [[nodiscard]] bool situateArmStatement(rq::Situation situation,
                                         rq::Expression &expression);
  [[nodiscard]] bool situateForStatement(rq::Situation situation,
                                         rq::Expression &expression);
  // interface, namespace
  [[nodiscard]] bool stiuateNameStatement(rq::Situation situation,
                                          rq::Expression &expression,
                                          rq::Situation name_situation);

private:
  [[nodiscard]] bool situateStatementSequenceBranch(rq::Expression &branch,
                                                    rq::ChainKind &chain);
  // The leading statements form their own chain sequence, ending before the
  // tag.
  [[nodiscard]] bool
  situateStatementBranchesBeforeTag(rq::Expression &first,
                                    rq::Expression *&tag_ptr);
  // Each call starts an independent statement sequence.
  [[nodiscard]] bool situateStatementBranches(rq::Expression *first_ptr);
};

} // namespace rq
