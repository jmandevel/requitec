#pragma once

#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/frame.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/Twine.h>

#include <functional>

namespace rq {

struct Context;
struct Module;

struct Situator final {
  using Self = rq::Situator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::Frame> _frame_ref;
  bool _is_ok = true;

  Situator(rq::Context &context, rq::Frame &frame)
      : _context_ref(context), _frame_ref(frame) {}
  Situator(const Self &) = delete;
  Situator(Self &&) = delete;
  ~Situator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Frame &getFrame() {
    return this->_frame_ref.get();
  }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  template <rq::Situation SITUATION_PARAM>
  void logErrorNotAtLeastBranchCount(rq::Expression &expression,
                                     unsigned count) {
    this->getContext().logMessage(
        expression.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(SITUATION_PARAM)) + " " +
            rq::getName(expression.getKeyword()) + " must have at least " +
            llvm::Twine(count) + " branches.\n",
        {expression.getLlvmSourceRange()}, {});
  }
  template <rq::Situation SITUATION_PARAM>
  void logErrorNotExactBranchCount(rq::Expression &expression, unsigned count) {
    this->getContext().logMessage(
        expression.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(SITUATION_PARAM)) + " " +
            rq::getName(expression.getKeyword()) + " must have exactly " +
            llvm::Twine(count) + " branches.\n",
        {expression.getLlvmSourceRange()}, {});
  }
  template <rq::Situation SITUATION_PARAM>
  void logErrorTooManyBranchCount(rq::Expression &expression,
                                  unsigned max_count) {
    this->getContext().logMessage(
        expression.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(SITUATION_PARAM)) + " " +
            rq::getName(expression.getKeyword()) + " must have no more than " +
            llvm::Twine(max_count) + " branches.\n",
        {expression.getLlvmSourceRange()}, {});
  }
  template <rq::Situation SITUATION_PARAM, rq::Situation BRANCH_SITUATION_PARAM>
  void logErrorInvalidBranchSituation(rq::Expression &branch,
                                      rq::Keyword outer_keyword,
                                      rq::Keyword branch_keyword,
                                      unsigned branch_i,
                                      llvm::Twine log_context) {
    this->getContext().logMessage(
        branch.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(BRANCH_SITUATION_PARAM)) +
            " expression expected for " + log_context + " of " +
            rq::getDescription(SITUATION_PARAM) + " " +
            rq::getName(outer_keyword) + " but found " +
            rq::getName(branch_keyword) + " at index " + llvm::Twine(branch_i) +
            ".\n",
        {branch.getLlvmSourceRange()}, {});
  }
  template <rq::Situation SITUATION_PARAM, rq::Situation BRANCH_SITUATION_PARAM>
  inline void situateBranch(llvm::Twine log_context, rq::Expression &outer,
                            unsigned branch_i, rq::Expression &branch);
  template <rq::Situation SITUATION_PARAM>
  inline void situateNullaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    if (expression.getHasBranch()) {
      this->getContext().logMessage(
          expression.getLlvmSourceStart(), rq::LogType::ERROR,
          llvm::Twine(rq::getDescription(SITUATION_PARAM)) + " " +
              rq::getName(expression.getKeyword()) + " must not have branches",
          {expression.getLlvmSourceRange()}, {});
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, rq::Situation BRANCH_SITUATION_PARAM>
  inline void situateUnaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    if (!expression.getHasBranch()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 1);
      this->setNotOk();
      return;
    }
    rq::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_PARAM>(
        "first branch", expression, 0, first);
    if (first.getHasNext()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 1);
      this->setNotOk();
      return;
    }
  }
  template <rq::Situation SITUATION_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_B_PARAM = BRANCH_SITUATION_A_PARAM>
  inline void situateBinaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    if (!expression.getHasBranch()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, 0, first);
    if (!expression.getHasBranch()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &second = first.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
        "second branch", expression, 1, second);
    if (second.getHasNext()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
  }
  template <rq::Situation SITUATION_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_B_PARAM = BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_C_PARAM = BRANCH_SITUATION_B_PARAM>
  inline void situateTernaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    if (!expression.getHasBranch()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 3);
      this->setNotOk();
      return;
    }
    rq::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, 0, first);
    if (!expression.getHasBranch()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 3);
      this->setNotOk();
      return;
    }
    rq::Expression &second = first.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
        "second branch", expression, 1, second);
    if (!second.getHasNext()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 3);
      this->setNotOk();
      return;
    }
    rq::Expression &third = second.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_C_PARAM>(
        "third branch", expression, 2, third);
    if (third.getHasNext()) {
      this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 3);
      this->setNotOk();
      return;
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
          "all branches", expression, branch_i++, branch);
    }
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           branch_i);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    do {
      if (!expression.getHasBranch()) {
        break;
      }
      rq::Expression &first = expression.getBranch();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
          "first branch", expression, branch_i++, first);
      for (rq::Expression &branch : first.getNextSubrange()) {
        this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
            "second and subsequent branches", expression, branch_i++, branch);
      }
    } while (false);
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_B_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    do {
      if (!expression.getHasBranch()) {
        break;
      }
      rq::Expression &first = expression.getBranch();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
          "first branch", expression, branch_i++, first);
      if (!first.getHasNext()) {
        break;
      }
      rq::Expression &second = first.getNext();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
          "second branch", expression, branch_i++, second);
      for (rq::Expression &branch : second.getNextSubrange()) {
        this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
            "third and subsequent branches", expression, branch_i++, branch);
      }
    } while (false);
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_B_PARAM,
            rq::Situation BRANCH_SITUATION_C_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM>
  inline void situateNaryExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    do {
      if (!expression.getHasBranch()) {
        break;
      }
      rq::Expression &first = expression.getBranch();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
          "first branch", expression, branch_i++, first);
      if (!first.getHasNext()) {
        break;
      }
      rq::Expression &second = first.getNext();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
          "second branch", expression, branch_i++, second);
      if (!second.getHasNext()) {
        break;
      }
      rq::Expression &third = second.getNext();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_C_PARAM>(
          "third branch", expression, branch_i++, second);
      for (rq::Expression &branch : third.getNextSubrange()) {
        this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
            "fourth and subsequent branches", expression, branch_i++, branch);
      }
    } while (false);
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           branch_i);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM,
            rq::Situation BRANCH_SITUATION_LAST_PARAM>
  inline void situateNaryWithLastExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (!branch.getHasNext()) {
        this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_LAST_PARAM>(
            "last branch", expression, branch_i++, branch);
        break;
      }
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
          "first to penultimate branch", expression, branch_i++, branch);
    }
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           branch_i);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
            rq::Situation BRANCH_SITUATION_A_PARAM,
            rq::Situation BRANCH_SITUATION_N_PARAM,
            rq::Situation BRANCH_SITUATION_LAST_PARAM>
  inline void situateNaryWithLastExpression(rq::Expression &expression) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "keyword not valid for situation");
    unsigned branch_i = 0;
    do {
      if (!expression.getHasBranch()) {
        break;
      }
      rq::Expression &first = expression.getBranch();
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
          "first branch", expression, branch_i++, first);
      for (rq::Expression &branch : first.getNextSubrange()) {
        if (!branch.getHasNext()) {
          this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_LAST_PARAM>(
              "last branch", expression, branch_i++, branch);
          break;
        }
        this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
            "second to penultimate branch", expression, branch_i++, branch);
      }
    } while (false);
    if (branch_i < MIN_COUNT_PARAM) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                           branch_i);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateAscribeTypeExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_ASCRIBE_TYPE,
              "wrong keyword");
    if (!expression.getHasBranch()) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &branch = expression.getBranch();
    if (branch.getCanBeTypeAttribute()) {
      if (!branch.getHasNext()) {
        this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
        this->setNotOk();
        return;
      }
      unsigned branch_i = 0;
      this->situateBranch<SITUATION_PARAM, rq::Situation::TYPE_ATTRIBUTE>(
          "first to penultimate branch", expression, branch_i++, branch);
      rq::Expression *previous_ptr = &branch;
      for (rq::Expression &next : branch.getNextSubrange()) {
        if (!next.getHasNext()) {
          this->situateBranch<SITUATION_PARAM, SITUATION_PARAM>(
              "last branch", expression, branch_i++, next);
          rq::Expression &previous = rq::dereferencePtr(previous_ptr);
          next.setNext(expression.replaceBranch(previous.popNext()));
          break;
        }
        this->situateBranch<SITUATION_PARAM, rq::Situation::TYPE_ATTRIBUTE>(
            "first to penultimate branch", expression, branch_i++, next);
        previous_ptr = &next;
      }
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2, SITUATION_PARAM,
                                  rq::Situation::TYPE_ATTRIBUTE>(expression);
    }
    if (!expression.getHasBranch()) {
      return;
    }
    rq::Expression &unascribed = expression.getBranch();
    if (unascribed.getKeyword() == rq::Keyword::_EXTEND) {
      if (!unascribed.getHasBranch()) {
        return;
      }
      rq::Expression &extended = unascribed.getBranch();
      if (extended.getKeyword() == rq::Keyword::_ASCRIBE_TYPE) {
        rq::Expression &last = extended.getLastBranch();
        last.setNext(unascribed.popNext());
        this->getFrame().discardExpression(
            expression.mergeAndPopBranch());
      } else {
        rq::Expression &first_attribute = unascribed.popNext();
        rq::Expression &branch = expression.mergeAndPopBranch();
        branch.setKeyword(rq::Keyword::_ASCRIBE_TYPE);
        branch.setBranch(expression.replaceBranch(branch));
        branch.setNext(extended.replaceNext(first_attribute));
      }
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateAscribeStatementExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_ASCRIBE_STATEMENT,
              "wrong keyword");
    if (!expression.getHasBranch()) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &branch = expression.getBranch();
    if (branch.getCanBeStatementAttribute()) {
      if (!branch.getHasNext()) {
        this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
        this->setNotOk();
        return;
      }
      unsigned branch_i = 0;
      this->situateBranch<SITUATION_PARAM, rq::Situation::STATEMENT_ATTRIBUTE>(
          "first to penultimate branch", expression, branch_i++, branch);
      rq::Expression *previous_ptr = &branch;
      for (rq::Expression &next : branch.getNextSubrange()) {
        if (!next.getHasNext()) {
          this->situateBranch<SITUATION_PARAM, SITUATION_PARAM>(
              "last branch", expression, branch_i++, next);
          rq::Expression &previous = rq::dereferencePtr(previous_ptr);
          next.setNext(expression.replaceBranch(previous.popNext()));
          break;
        }
        this->situateBranch<SITUATION_PARAM,
                            rq::Situation::STATEMENT_ATTRIBUTE>(
            "first to penultimate branch", expression, branch_i++, next);
        previous_ptr = &next;
      }
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2, SITUATION_PARAM,
                                  rq::Situation::STATEMENT_ATTRIBUTE>(
          expression);
    }
    if (!expression.getHasBranch()) {
      return;
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateParenthesisGroupExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_PARENTHESIS_GROUP,
              "wrong keyword");
    this->situateUnaryExpression<SITUATION_PARAM, SITUATION_PARAM>(expression);
    this->getFrame().discardExpression(expression.mergeAndPopBranch());
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateColonOperatorExpression(rq::Expression &expression) {
    if constexpr (SITUATION_PARAM == rq::Situation::LVALUE ||
                  SITUATION_PARAM == rq::Situation::PARAMETER ||
                  SITUATION_PARAM == rq::Situation::ENUMERATOR) {
      this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::RVALUE,
                                    rq::Situation::RVALUE>(expression);
      expression.changeKeyword(rq::Keyword::_BINDING);
    } else if constexpr (SITUATION_PARAM == rq::Situation::RVALUE ||
                         SITUATION_PARAM == rq::Situation::ARGUMENT) {
      this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::RVALUE,
                                    rq::Situation::RVALUE>(expression);
      expression.changeKeyword(rq::Keyword::_CAST);
    } else {
      static_assert(false, "invalid situation");
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateEqualOperatorExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_EQUAL_OPERATOR,
              "wrong keyword");
    if constexpr (SITUATION_PARAM == rq::Situation::ARGUMENT) {
      this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::NAME,
                                    rq::Situation::RVALUE>(expression);
      expression.changeKeyword(rq::Keyword::_NAMED_ARGUMENT);
    } else if constexpr (SITUATION_PARAM == rq::Situation::PARAMETER) {
      this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::LVALUE,
                                    rq::Situation::RVALUE>(expression);
      expression.changeKeyword(rq::Keyword::_DEFAULT_VALUE_PARAMETER);
    } else if constexpr (SITUATION_PARAM == rq::Situation::ENUMERATOR) {
      this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::NAME,
                                    rq::Situation::RVALUE>(expression);
      expression.changeKeyword(rq::Keyword::_ENUMERATOR_WITH_DISCRIMINANT);
    } else if constexpr (SITUATION_PARAM == rq::Situation::LOCAL_STATEMENT ||
                         SITUATION_PARAM == rq::Situation::TOP_STATEMENT ||
                         SITUATION_PARAM == rq::Situation::TABLE_STATEMENT ||
                         SITUATION_PARAM == rq::Situation::OBJECT_STATEMENT) {
      unsigned branch_i = 0;
      if (!expression.getHasBranch()) {
        this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 2);
        this->setNotOk();
        return;
      }
      rq::Expression &destination = expression.getBranch();
      this->situateBranch<SITUATION_PARAM, rq::Situation::LVALUE>(
          "first branch", expression, branch_i++, destination);
      if (!destination.getHasNext()) {
        this->logErrorNotExactBranchCount<SITUATION_PARAM>(expression, 2);
        this->setNotOk();
        return;
      }
      rq::Expression &value = destination.getNext();
      this->situateBranch<SITUATION_PARAM, rq::Situation::RVALUE>(
          "second branch", expression, branch_i++, value);
      switch (destination.getKeyword()) {
      case rq::Keyword::_NULL:
        destination.changeKeyword(rq::Keyword::_IGNORE);
        this->getFrame().discardExpression(
            expression.mergeAndPopBranch());
        break;
      case rq::Keyword::_TUPLE:
        destination.changeKeyword(rq::Keyword::_STRUCTURED_BINDING);
        this->getFrame().discardExpression(
            expression.mergeAndPopBranch());
        break;
      default:
        expression.changeKeyword(rq::Keyword::_ASSIGN);
        break;
      }
    } else {
      static_assert(false, "invalid situation");
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void
  situateAssignArithmeticExpression(rq::Expression &expression,
                                    rq::Keyword arithmetic_keyword) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "wrong keyword");
    this->situateBinaryExpression<SITUATION_PARAM, rq::Situation::LVALUE,
                                  rq::Situation::RVALUE>(expression);
    expression.changeKeyword(rq::Keyword::_ASSIGN);
    if (!expression.getHasBranch()) {
      return;
    }
    rq::Expression &destination = expression.getBranch();
    if (!destination.getHasNext()) {
      return;
    }
    rq::Expression &value = destination.popNext();
    rq::Expression &arithmetic_expression =
        this->getFrame().acquireExpression();
    arithmetic_expression.setKeyword(arithmetic_keyword);
    arithmetic_expression.setSource(value);
    rq::Expression &destination_copy =
        this->getFrame().copyExpression(destination);
    destination.setNext(arithmetic_expression);
    arithmetic_expression.setBranch(destination_copy);
    destination_copy.setNext(value);
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateSequenceExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_SEQUENCE, "wrong keyword");
    if (!expression.getHasBranch()) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &value = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, rq::Situation::RVALUE>(
        "first branch", expression, 0, value);
    if (!value.getHasNext()) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &stage_one = value.getNext();
    this->situateBranch<SITUATION_PARAM, rq::Situation::SEQUENCE_STAGE>(
        "second branch", expression, 1, stage_one);
    if (!stage_one.getHasNext()) {
      return;
    }
    rq::Expression &stage_two = stage_one.getNext();
    this->situateBranch<SITUATION_PARAM, rq::Situation::SEQUENCE_STAGE>(
        "third branch", expression, 2, stage_two);
    if (stage_two.getHasNext()) {
      this->logErrorTooManyBranchCount<SITUATION_PARAM>(expression, 3);
      this->setNotOk();
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateTableExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::TABLE, "wrong keyword");
    this->situateNaryExpression<SITUATION_PARAM, 1, rq::Situation::PATH,
                                rq::Situation::TABLE_STATEMENT>(expression);
    if (!expression.getHasBranch()) {
      return;
    }
    rq::Expression &path_expression = expression.getBranch();
    if (path_expression.getKeyword() == rq::Keyword::_MEMBER_OF) {
      rq::Expression *table_body_ptr = path_expression.popNextPtr();
      rq::Expression &first_name_expression = path_expression.popBranch();
      std::ignore = expression.replaceBranch(first_name_expression);
      rq::Expression &second_name_expression =
          first_name_expression.replaceNext(path_expression);
      path_expression.changeKeyword(rq::Keyword::TABLE);
      path_expression.setBranch(second_name_expression);
      rq::Expression *previous_name_expression_ptr = &first_name_expression;
      for (rq::Expression &name_expression :
           second_name_expression.getHorizontalSubrange()) {
        rq::Expression &table_expression =
            this->getFrame().acquireExpression();
        table_expression.setKeyword(rq::Keyword::TABLE);
        table_expression.setSourceInsertedAfter(expression);
        table_expression.setBranch(name_expression);
        std::ignore = rq::dereferencePtr(previous_name_expression_ptr)
                          .replaceNext(table_expression);
        if (!name_expression.getHasNext()) {
          name_expression.setNext(table_body_ptr);
          break;
        } else {
          previous_name_expression_ptr = &name_expression;
        }
      }
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateDepthTypeExpression(rq::Expression &expression,
                                         rq::Keyword default_depth) {
    RQ_ASSERT(rq::getCanBeSituation<SITUATION_PARAM>(expression.getKeyword()),
              "wrong keyword");
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::SIGNED ||
                  expression.getKeyword() == rq::Keyword::UNSIGNED ||
                  expression.getKeyword() == rq::Keyword::WORD,
              "wrong keyword");
    if (!expression.getHasBranch()) {
      rq::Expression &first = this->getFrame().acquireExpression();
      first.setKeyword(default_depth);
      first.setSourceInsertedAfter(expression);
      expression.setBranch(first);
    }
    this->situateUnaryExpression<SITUATION_PARAM, rq::Situation::RVALUE>(
        expression);
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateReflectExpression(rq::Expression &expression) {
    RQ_ASSERT(expression.getKeyword() == rq::Keyword::_REFLECT,
              "wrong keyword");
    unsigned branch_i = 0;
    if (!expression.getHasBranch()) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    rq::Expression &value = expression.popBranch();
    this->situateBranch<SITUATION_PARAM, rq::Situation::RVALUE>(
        "first branch", expression, branch_i++, value);
    rq::Expression *inner_ptr = &value;
    rq::Expression *next_ptr = value.popNextPtr();
    if (next_ptr == nullptr) {
      this->logErrorNotAtLeastBranchCount<SITUATION_PARAM>(expression, 2);
      this->setNotOk();
      return;
    }
    while (next_ptr != nullptr) {
      rq::Expression &inner = rq::dereferencePtr(inner_ptr);
      rq::Expression &next = rq::dereferencePtr(next_ptr);
      this->situateBranch<SITUATION_PARAM, rq::Situation::REFLECTION>(
          "second and subsequent branches", expression, branch_i++, next);
      next_ptr = next.popNextPtr();
      if (next.getKeyword() == rq::Keyword::__IDENTIFIER_LITERAL) {
        rq::Expression &member = this->getFrame().acquireExpression();
        member.setKeyword(rq::Keyword::_MEMBER_OF);
        member.setSource(inner, next);
        member.setBranch(inner);
        inner.setNext(next);
        inner_ptr = &member;
        continue;
      } else if (next.getKeyword() == rq::Keyword::_ASCRIBE_TYPE) {
        this->situateAscribeTypeExpression<rq::Situation::REFLECTION>(next);
        next.changeKeyword(rq::Keyword::_ASCRIBE_TYPE_OF_ELEMENTS);
        if (!next.getHasBranch()) {
          continue;
        }
        rq::Expression &next_branch = next.getBranch();
        const rq::Keyword universalized =
            next_branch.getUniversalized<SITUATION_PARAM>();
        next_branch.changeKeyword(universalized);
        inner.setNext(next_branch.replaceBranchPtr(inner));
        inner_ptr = &next;
        continue;
      }
      const rq::Keyword universalized =
          next.getUniversalized<SITUATION_PARAM>();
      next.changeKeyword(universalized);
      inner.setNext(next.replaceBranchPtr(inner));
      inner_ptr = &next;
    }
    expression.setBranch(inner_ptr);
    this->getFrame().discardExpression(expression.mergeAndPopBranch());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  inline void addTacitCommasIfNullary(rq::Expression &expression,
                                      unsigned comma_count) {
    if (expression.getHasBranch()) {
      return;
    }
    rq::TreeParser parser;
    parser.setOperation(expression);
    for (unsigned comma_i = 0; comma_i < comma_count; comma_i++) {
      rq::Expression &comma = this->getFrame().acquireExpression();
      comma.setSourceInsertedAtEnd(expression);
      comma.setKeyword(rq::Keyword::_TACIT_COMMA_EXPRESSION);
      parser.appendBranch(comma);
    }
  }
  template <rq::Situation SITUATION_PARAM>
  inline void situateExpression(rq::Expression &expression) {
    using namespace rq;
    using K = Keyword;
    using S = Situation;
    constexpr S SP = SITUATION_PARAM;
    switch (expression.getKeyword()) {
    case K::__NONE:
      RQ_UNREACHABLE();
    case K::__INTEGER_LITERAL:
      [[fallthrough]];
    case K::__FLOAT_LITERAL:
      [[fallthrough]];
    case K::__STRING_LITERAL:
      [[fallthrough]];
    case K::__LEFT_INTERPOLATION_LITERAL:
      [[fallthrough]];
    case K::__MIDDLE_INTERPOLATION_LITERAL:
      [[fallthrough]];
    case K::__RIGHT_INTERPOLATION_LITERAL:
      [[fallthrough]];
    case K::__CODEUNIT_LITERAL:
      [[fallthrough]];
    case K::__IDENTIFIER_LITERAL:
      RQ_ASSERT(!expression.getHasBranch(), "literal must not have branch");
      break;
    case K::__ERROR:
      RQ_UNREACHABLE();
    case K::_PARENTHESIS_GROUP:
      if constexpr (!getCanBeSituation<SP>(K::_PARENTHESIS_GROUP)) {
        RQ_UNREACHABLE();
      } else {
        this->situateParenthesisGroupExpression<SP>(expression);
      }
      break;
    case K::_EQUAL_OPERATOR:
      if constexpr (!getCanBeSituation<SP>(K::_EQUAL_OPERATOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateEqualOperatorExpression<SP>(expression);
      }
      break;
    case K::_COLON_OPERATOR:
      if constexpr (!getCanBeSituation<SP>(K::_COLON_OPERATOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateColonOperatorExpression<SP>(expression);
      }
      break;
    case K::_INFERENCE:
      if constexpr (!getCanBeSituation<SP>(K::_INFERENCE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_TACIT_COMMA_EXPRESSION:
      if constexpr (!getCanBeSituation<SP>(K::_TACIT_COMMA_EXPRESSION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // LOGICAL
    case K::_LOGICAL_AND:
      if constexpr (!getCanBeSituation<SP>(K::_LOGICAL_AND)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_LOGICAL_OR:
      if constexpr (!getCanBeSituation<SP>(K::_LOGICAL_OR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_LOGICAL_COMPLEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_LOGICAL_COMPLEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;

    // COMPARISON
    case K::_GREATER:
      if constexpr (!getCanBeSituation<SP>(K::_GREATER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_GREATER_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_GREATER_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_LESS:
      if constexpr (!getCanBeSituation<SP>(K::_LESS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_LESS_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_LESS_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_NOT_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_NOT_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;

    // APPLY
    case K::_EXTEND:
      if constexpr (!getCanBeSituation<SP>(K::_EXTEND)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_BINDING:
      if constexpr (!getCanBeSituation<SP>(K::_BINDING)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_ASCRIBE_TYPE:
      if constexpr (!getCanBeSituation<SP>(K::_ASCRIBE_TYPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAscribeTypeExpression<SP>(expression);
      }
      break;
    case K::_ASCRIBE_STATEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_ASCRIBE_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAscribeStatementExpression<SP>(expression);
      }
      break;
    case K::_ASCRIBE_TYPE_OF_ELEMENTS:
      if constexpr (!getCanBeSituation<SP>(K::_ASCRIBE_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE, S::TYPE_ATTRIBUTE>(
            expression);
      }
      break;
    case K::_CAST:
      if constexpr (!getCanBeSituation<SP>(K::_CAST)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_IDENTIFY:
      if constexpr (!getCanBeSituation<SP>(K::_IDENTIFY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // ARITHMETIC
    case K::_ADD:
      if constexpr (!getCanBeSituation<SP>(K::_ADD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_SUBTRACT:
      if constexpr (!getCanBeSituation<SP>(K::_SUBTRACT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_MULTIPLY:
      if constexpr (!getCanBeSituation<SP>(K::_MULTIPLY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_DIVIDE:
      if constexpr (!getCanBeSituation<SP>(K::_DIVIDE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_MODULUS:
      if constexpr (!getCanBeSituation<SP>(K::_MODULUS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_NEGATE:
      if constexpr (!getCanBeSituation<SP>(K::_NEGATE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // BITWISE
    case K::_BITWISE_CAST:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_CAST)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_OR:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_OR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_AND:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_AND)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_XOR:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_XOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_COMPLEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_COMPLEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_SHIFT_LEFT:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_SHIFT_LEFT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_BITWISE_SHIFT_RIGHT:
      if constexpr (!getCanBeSituation<SP>(K::_BITWISE_SHIFT_RIGHT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::BITWISE_ROTATE_LEFT:
      if constexpr (!getCanBeSituation<SP>(K::BITWISE_ROTATE_LEFT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::BITWISE_ROTATE_RIGHT:
      if constexpr (!getCanBeSituation<SP>(K::BITWISE_ROTATE_RIGHT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // MEMORY
    case K::SINGLETON:
      if constexpr (!getCanBeSituation<SP>(K::SINGLETON)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SINGLETON_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SINGLETON_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::CONTENT:
      if constexpr (!getCanBeSituation<SP>(K::CONTENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_CONTENT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_CONTENT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::ADDRESS:
      if constexpr (!getCanBeSituation<SP>(K::ADDRESS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_ADDRESS_OF:
      if constexpr (!getCanBeSituation<SP>(K::_ADDRESS_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::BORROW:
      if constexpr (!getCanBeSituation<SP>(K::BORROW)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_BORROW_OF:
      if constexpr (!getCanBeSituation<SP>(K::_BORROW_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::VIEW:
      if constexpr (!getCanBeSituation<SP>(K::VIEW)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_VIEW_OF:
      if constexpr (!getCanBeSituation<SP>(K::_VIEW_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SLICE:
      if constexpr (!getCanBeSituation<SP>(K::SLICE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SLICE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SLICE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::ARRAY_SLICE:
      if constexpr (!getCanBeSituation<SP>(K::ARRAY_SLICE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_ARRAY_SLICE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_ARRAY_SLICE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // ASSIGNMENT
    case K::_ASSIGN:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::ENUMERATOR) {
        this->situateBinaryExpression<SP, S::NAME, S::RVALUE>(expression);
        expression.changeKeyword(rq::Keyword::_ENUMERATOR_WITH_DISCRIMINANT);
      } else {
        this->situateBinaryExpression<SP, S::LVALUE, S::RVALUE>(expression);
      }
      break;
    case K::_ASSIGN_ADD:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN_ADD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAssignArithmeticExpression<SP>(expression,
                                                    rq::Keyword::_ADD);
      }
      break;
    case K::_ASSIGN_SUBTRACT:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN_SUBTRACT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAssignArithmeticExpression<SP>(expression,
                                                    rq::Keyword::_SUBTRACT);
      }
      break;
    case K::_ASSIGN_MULTIPLY:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN_MULTIPLY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAssignArithmeticExpression<SP>(expression,
                                                    rq::Keyword::_MULTIPLY);
      }
      break;
    case K::_ASSIGN_DIVIDE:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN_DIVIDE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAssignArithmeticExpression<SP>(expression,
                                                    rq::Keyword::_DIVIDE);
      }
      break;
    case K::_ASSIGN_MODULUS:
      if constexpr (!getCanBeSituation<SP>(K::_ASSIGN_MODULUS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateAssignArithmeticExpression<SP>(expression,
                                                    rq::Keyword::_MODULUS);
      }
      break;

    // MOVE SEMANTICS
    case K::DEEP_COPY:
      if constexpr (!getCanBeSituation<SP>(K::DEEP_COPY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DEEP_COPY_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DEEP_COPY_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::RETAIN_MOVE:
      if constexpr (!getCanBeSituation<SP>(K::RETAIN_MOVE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_RETAIN_MOVE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_RETAIN_MOVE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DROP_MOVE:
      if constexpr (!getCanBeSituation<SP>(K::DROP_MOVE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DROP_MOVE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DROP_MOVE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LINEAR_ASSIGN:
      if constexpr (!getCanBeSituation<SP>(K::LINEAR_ASSIGN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_LINEAR_ASSIGN_OF:
      if constexpr (!getCanBeSituation<SP>(K::_LINEAR_ASSIGN_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SWAP:
      if constexpr (!getCanBeSituation<SP>(K::SWAP)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // SUBTYPE
    case K::_ARRAY:
      if constexpr (!getCanBeSituation<SP>(K::_ARRAY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE>(expression);
      }
      break;
    case K::_REFERENCE:
      if constexpr (!getCanBeSituation<SP>(K::_REFERENCE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_POINTER:
      if constexpr (!getCanBeSituation<SP>(K::_POINTER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_FAT_POINTER:
      if constexpr (!getCanBeSituation<SP>(K::_FAT_POINTER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // TYPE MODIFIER
    case K::MUTABLE:
      if constexpr (!getCanBeSituation<SP>(K::MUTABLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::CONSTANT:
      if constexpr (!getCanBeSituation<SP>(K::CONSTANT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::VOLATILE:
      if constexpr (!getCanBeSituation<SP>(K::VOLATILE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::ATOMIC:
      if constexpr (!getCanBeSituation<SP>(K::ATOMIC)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::NULL_TERMINATED:
      if constexpr (!getCanBeSituation<SP>(K::NULL_TERMINATED)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::MAY_DISCARD:
      if constexpr (!getCanBeSituation<SP>(K::MAY_DISCARD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::DEBUG_TRAP_ON_PANIC:
      if constexpr (!getCanBeSituation<SP>(K::DEBUG_TRAP_ON_PANIC)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::LINEAR:
      if constexpr (!getCanBeSituation<SP>(K::LINEAR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::DYNAMIC_CAPTURE_LAYOUT:
      if constexpr (!getCanBeSituation<SP>(K::DYNAMIC_CAPTURE_LAYOUT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::PARAMETER>(expression);
      }
      break;
    case K::_POSITIONAL_PARAMETERS_END:
      if constexpr (!getCanBeSituation<SP>(K::_POSITIONAL_PARAMETERS_END)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_NAMED_PARAMETERS_BEGIN:
      if constexpr (!getCanBeSituation<SP>(K::_NAMED_PARAMETERS_BEGIN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // BRACES
    case K::_TUPLE:
      if constexpr (!getCanBeSituation<SP>(K::_TUPLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::ARGUMENT>(expression);
      }
      break;
    case K::_LAYOUT_TYPE:
      if constexpr (!getCanBeSituation<SP>(K::_LAYOUT_TYPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::PARAMETER>(expression);
      }
      break;
    case K::_NULL:
      if constexpr (!getCanBeSituation<SP>(K::_NULL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_NULL_TYPE:
      if constexpr (!getCanBeSituation<SP>(K::_NULL_TYPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_STRUCTURED_BINDING:
      if constexpr (!getCanBeSituation<SP>(K::_STRUCTURED_BINDING)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryWithLastExpression<SP, 2, S::LVALUE, S::RVALUE>(
            expression);
      }
      break;
    case K::_IGNORE:
      if constexpr (!getCanBeSituation<SP>(K::_IGNORE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SPECIALIZATION:
      if constexpr (!getCanBeSituation<SP>(K::_SPECIALIZATION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::ARGUMENT>(expression);
      }
      break;

    // PROCEDURES
    case K::_CALL:
      if constexpr (!getCanBeSituation<SP>(K::_CALL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::ARGUMENT>(expression);
      }
      break;
    case K::_NAMED_ARGUMENT:
      if constexpr (!getCanBeSituation<SP>(K::_NAMED_ARGUMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::NAME, S::RVALUE>(expression);
      }
      break;
    case K::_INDEX:
      if constexpr (!getCanBeSituation<SP>(K::_INDEX)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE>(expression);
      }
      break;
    case K::_SIGNATURE_TYPE:
      if constexpr (!getCanBeSituation<SP>(K::_SIGNATURE_TYPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::PARAMETER>(expression);
      }
      break;
    case K::_DEFAULT_VALUE_PARAMETER:
      if constexpr (!getCanBeSituation<SP>(K::_DEFAULT_VALUE_PARAMETER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::LVALUE, S::RVALUE>(expression);
      }
      break;
    case K::DESTROY:
      if constexpr (!getCanBeSituation<SP>(K::DESTROY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DESTROY_VALUE:
      if constexpr (!getCanBeSituation<SP>(K::_DESTROY_VALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DROP:
      if constexpr (!getCanBeSituation<SP>(K::DROP)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DROP_VALUE:
      if constexpr (!getCanBeSituation<SP>(K::_DROP_VALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::ENTRY_POINT:
      if constexpr (!getCanBeSituation<SP>(K::_DROP_VALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::FUNCTION:
      if constexpr (!getCanBeSituation<SP>(K::FUNCTION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE, S::RVALUE,
                                    S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::METHOD:
      if constexpr (!getCanBeSituation<SP>(K::METHOD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE, S::RVALUE,
                                    S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::CONSTRUCTOR:
      if constexpr (!getCanBeSituation<SP>(K::CONSTRUCTOR)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->addTacitCommasIfNullary(expression, 2);
        this->situateNaryExpression<SP, 2, S::RVALUE, S::RVALUE,
                                    S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_CONSTRUCTOR_OF:
      if constexpr (!getCanBeSituation<SP>(K::_CONSTRUCTOR_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LAYOUT_CONSTRUCTOR:
      if constexpr (!getCanBeSituation<SP>(K::LAYOUT_CONSTRUCTOR)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      } else {
        this->addTacitCommasIfNullary(expression, 1);
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::_LAYOUT_CONSTRUCTOR_OF:
      if constexpr (!getCanBeSituation<SP>(K::_LAYOUT_CONSTRUCTOR_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DESTRUCTOR:
      if constexpr (!getCanBeSituation<SP>(K::DESTRUCTOR)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_DESTRUCTOR_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DESTRUCTOR_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::CALLER:
      if constexpr (!getCanBeSituation<SP>(K::DESTRUCTOR)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_CALLER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DESTRUCTOR_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::RANGER:
      if constexpr (!getCanBeSituation<SP>(K::RANGER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->addTacitCommasIfNullary(expression, 1);
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::_RANGER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_RANGER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DEEP_COPIER:
      if constexpr (!getCanBeSituation<SP>(K::DEEP_COPIER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_DEEP_COPIER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DEEP_COPIER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::RETAIN_MOVER:
      if constexpr (!getCanBeSituation<SP>(K::RETAIN_MOVER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_RETAIN_MOVER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_RETAIN_MOVER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DROP_MOVER:
      if constexpr (!getCanBeSituation<SP>(K::DROP_MOVER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_DROP_MOVER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DROP_MOVER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SWAPER:
      if constexpr (!getCanBeSituation<SP>(K::SWAPER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_SWAPER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SWAPER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::INDEXER:
      if constexpr (!getCanBeSituation<SP>(K::INDEXER)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->addTacitCommasIfNullary(expression, 1);
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::_INDEXER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_INDEXER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_ANONYMOUS_FUNCTION:
      if constexpr (!getCanBeSituation<SP>(K::_ANONYMOUS_FUNCTION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::DYNAMIC_CAPTURE, S::RVALUE,
                                    S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::_DYNAMIC_CAPTURE:
      if constexpr (!getCanBeSituation<SP>(K::_DYNAMIC_CAPTURE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::ARGUMENT>(expression);
      }
      break;

    // CONTROL FLOW
    case K::RETURN:
      if constexpr (!getCanBeSituation<SP>(K::RETURN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BREAK:
      if constexpr (!getCanBeSituation<SP>(K::BREAK)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::CONTINUE:
      if constexpr (!getCanBeSituation<SP>(K::CONTINUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::FALLTHROUGH:
      if constexpr (!getCanBeSituation<SP>(K::FALLTHROUGH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::GOTO:
      if constexpr (!getCanBeSituation<SP>(K::GOTO)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::RANGE_OVER:
      if constexpr (!getCanBeSituation<SP>(K::RANGE_OVER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // SYMBOLS
    case K::OBJECT:
      if constexpr (!getCanBeSituation<SP>(K::OBJECT)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION || SP == S::RVALUE) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE, S::RVALUE,
                                    S::OBJECT_STATEMENT>(expression);
      }
      break;
    case K::_OBJECT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_OBJECT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::ENUMERATION:
      if constexpr (!getCanBeSituation<SP>(K::ENUMERATION)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION || SP == S::RVALUE) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateNaryExpression<SP, 2, S::RVALUE, S::RVALUE,
                                    S::OBJECT_STATEMENT>(expression);
      }
      break;
    case K::_ENUMERATION_OF:
      if constexpr (!getCanBeSituation<SP>(K::_ENUMERATION_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
    case K::_ENUMERATOR_WITH_DISCRIMINANT:
      if constexpr (!getCanBeSituation<SP>(K::_ENUMERATOR_WITH_DISCRIMINANT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::LVALUE, S::RVALUE>(expression);
      }
      break;

    // VALUES
    case K::TRUE:
      if constexpr (!getCanBeSituation<SP>(K::TRUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::FALSE:
      if constexpr (!getCanBeSituation<SP>(K::FALSE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::INDETERMINATE:
      if constexpr (!getCanBeSituation<SP>(K::INDETERMINATE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::THIS:
      if constexpr (!getCanBeSituation<SP>(K::THIS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::RESULT:
      if constexpr (!getCanBeSituation<SP>(K::RESULT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::COMMAND_LINE_ARGUMENTS:
      if constexpr (!getCanBeSituation<SP>(K::COMMAND_LINE_ARGUMENTS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::IN:
      if constexpr (!getCanBeSituation<SP>(K::IN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::OUT:
      if constexpr (!getCanBeSituation<SP>(K::OUT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::ADDRESS_SIZE:
      if constexpr (!getCanBeSituation<SP>(K::ADDRESS_SIZE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::ADDRESS_DEPTH:
      if constexpr (!getCanBeSituation<SP>(K::ADDRESS_DEPTH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BITS_PER_BYTE:
      if constexpr (!getCanBeSituation<SP>(K::BITS_PER_BYTE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // BUILTIN TYPES
    case K::VOID:
      if constexpr (!getCanBeSituation<SP>(K::VOID)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::NO_RETURN:
      if constexpr (!getCanBeSituation<SP>(K::NO_RETURN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BOOLEAN:
      if constexpr (!getCanBeSituation<SP>(K::BOOLEAN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::WORD:
      if constexpr (!getCanBeSituation<SP>(K::WORD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateDepthTypeExpression<SP>(expression, K::BITS_PER_BYTE);
      }
      break;
    case K::SIGNED:
      if constexpr (!getCanBeSituation<SP>(K::SIGNED)) {
        RQ_UNREACHABLE();
      } else {
        this->situateDepthTypeExpression<SP>(expression, K::ADDRESS_DEPTH);
      }
      break;
    case K::UNSIGNED:
      if constexpr (!getCanBeSituation<SP>(K::UNSIGNED)) {
        RQ_UNREACHABLE();
      } else {
        this->situateDepthTypeExpression<SP>(expression, K::ADDRESS_DEPTH);
      }
      break;
    case K::BFLOAT16:
      if constexpr (!getCanBeSituation<SP>(K::BFLOAT16)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BINARY16:
      if constexpr (!getCanBeSituation<SP>(K::BINARY16)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BINARY32:
      if constexpr (!getCanBeSituation<SP>(K::BINARY32)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BINARY64:
      if constexpr (!getCanBeSituation<SP>(K::BINARY64)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::BINARY128:
      if constexpr (!getCanBeSituation<SP>(K::BINARY128)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::UTF8:
      if constexpr (!getCanBeSituation<SP>(K::UTF8)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_CHAR:
      if constexpr (!getCanBeSituation<SP>(K::C_CHAR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_UNSIGNED_CHAR:
      if constexpr (!getCanBeSituation<SP>(K::C_UNSIGNED_CHAR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_SIGNED_CHAR:
      if constexpr (!getCanBeSituation<SP>(K::C_SIGNED_CHAR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_WCHAR_T:
      if constexpr (!getCanBeSituation<SP>(K::C_WCHAR_T)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_SHORT_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_SHORT_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_UNSIGNED_SHORT_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_UNSIGNED_SHORT_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_UNSIGNED_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_UNSIGNED_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_LONG_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_LONG_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_UNSIGNED_LONG_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_UNSIGNED_LONG_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_LONG_LONG_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_LONG_LONG_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_UNSIGNED_LONG_LONG_INT:
      if constexpr (!getCanBeSituation<SP>(K::C_UNSIGNED_LONG_LONG_INT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_FLOAT:
      if constexpr (!getCanBeSituation<SP>(K::C_FLOAT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_DOUBLE:
      if constexpr (!getCanBeSituation<SP>(K::C_DOUBLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::C_LONG_DOUBLE:
      if constexpr (!getCanBeSituation<SP>(K::C_LONG_DOUBLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // VARIADIC ARGUMENTS
    case K::VARIADIC_ARGUMENTS:
      if constexpr (!getCanBeSituation<SP>(K::VARIADIC_ARGUMENTS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::FIRST_VARIADIC_ARGUMENT:
      if constexpr (!getCanBeSituation<SP>(K::FIRST_VARIADIC_ARGUMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_FIRST_VARIADIC_ARGUMENT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_FIRST_VARIADIC_ARGUMENT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::NEXT_VARIADIC_ARGUMENT:
      if constexpr (!getCanBeSituation<SP>(K::NEXT_VARIADIC_ARGUMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_NEXT_VARIADIC_ARGUMENT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_NEXT_VARIADIC_ARGUMENT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // SCOPE
    case K::IF:
      if constexpr (!getCanBeSituation<SP>(K::IF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, SP>(expression);
      }
      break;
    case K::ELSE_IF:
      if constexpr (!getCanBeSituation<SP>(K::ELSE_IF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, SP>(expression);
      }
      break;
    case K::ELSE:
      if constexpr (!getCanBeSituation<SP>(K::ELSE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, SP>(expression);
      }
      break;
    case K::MATCH:
      if constexpr (!getCanBeSituation<SP>(K::MATCH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::ARM>(expression);
      }
      break;
    case K::INLINE_MATCH:
      if constexpr (!getCanBeSituation<SP>(K::INLINE_MATCH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::ARM>(expression);
      }
      break;
    case K::SWITCH:
      if constexpr (!getCanBeSituation<SP>(K::SWITCH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::ARM>(expression);
      }
      break;
    case K::CASE:
      if constexpr (!getCanBeSituation<SP>(K::CASE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::DEFAULT:
      if constexpr (!getCanBeSituation<SP>(K::DEFAULT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::FOR:
      if constexpr (!getCanBeSituation<SP>(K::FOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::FOR_EACH:
      if constexpr (!getCanBeSituation<SP>(K::FOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 2, S::LVALUE, S::RVALUE,
                                    S::LOCAL_STATEMENT>(expression);
      }
      break;
    case K::WHILE:
      if constexpr (!getCanBeSituation<SP>(K::WHILE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::RVALUE, S::LOCAL_STATEMENT>(
            expression);
      }
      break;
    case K::SCOPE:
      if constexpr (!getCanBeSituation<SP>(K::SCOPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, SP>(expression);
      }
      break;
    case K::INLINE_SCOPE:
      if constexpr (!getCanBeSituation<SP>(K::INLINE_SCOPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, SP>(expression);
      }
      break;
    case K::_SCOPE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SCOPE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::BLOCK:
      if constexpr (!getCanBeSituation<SP>(K::BLOCK)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, SP>(expression);
      }
      break;
    case K::INLINE_BLOCK:
      if constexpr (!getCanBeSituation<SP>(K::INLINE_BLOCK)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, SP>(expression);
      }
      break;

    // RANGES
    case K::RANGE:
      if constexpr (!getCanBeSituation<SP>(K::RANGE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::CONTAINS:
      if constexpr (!getCanBeSituation<SP>(K::CONTAINS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_CONTAINS_VALUE:
      if constexpr (!getCanBeSituation<SP>(K::_CONTAINS_VALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateSequenceExpression<SP>(expression);
      }
      break;
    case K::_SEQUENCE_STEP_ADD:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_STEP_ADD)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_STEP_SUBTRACT:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_STEP_SUBTRACT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_STEP_MULTIPLY:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_STEP_MULTIPLY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_STEP_DIVIDE:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_STEP_DIVIDE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_STEP_MODULUS:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_STEP_MODULUS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_LESS:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_LESS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_GREATER:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_GREATER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_LESS_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_LESS_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_GREATER_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_GREATER_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_SEQUENCE_WHILE_NOT_EQUAL:
      if constexpr (!getCanBeSituation<SP>(K::_SEQUENCE_WHILE_NOT_EQUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // ACCESS MODIFIERS
    case K::PRIVATE:
      if constexpr (!getCanBeSituation<SP>(K::PRIVATE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::PROTECTED:
      if constexpr (!getCanBeSituation<SP>(K::PROTECTED)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::EXPORT:
      if constexpr (!getCanBeSituation<SP>(K::EXPORT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // TABLE GRAPH
    case K::IMPORT:
      if constexpr (!getCanBeSituation<SP>(K::IMPORT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::USE:
      if constexpr (!getCanBeSituation<SP>(K::USE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::USE_TABLE:
      if constexpr (!getCanBeSituation<SP>(K::USE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::TABLE:
      if constexpr (!getCanBeSituation<SP>(K::TABLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateTableExpression<SP>(expression);
      }
      break;
    case K::_TABLE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_TABLE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::MODULE:
      if constexpr (!getCanBeSituation<SP>(K::MODULE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateTableExpression<SP>(expression);
      }
      break;
    case K::_MODULE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_MODULE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_MODULE_ROOT:
      if constexpr (!getCanBeSituation<SP>(K::_MODULE_ROOT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::TOP_STATEMENT>(expression);
      }
      break;

    // ERROR HANDLING AND DEBUGGING
    case K::PANIC_TRAP:
      if constexpr (!getCanBeSituation<SP>(K::PANIC_TRAP)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DEBUG_TRAP:
      if constexpr (!getCanBeSituation<SP>(K::DEBUG_TRAP)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // HINTS
    case K::UNREACHABLE:
      if constexpr (!getCanBeSituation<SP>(K::UNREACHABLE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::ASSUME:
      if constexpr (!getCanBeSituation<SP>(K::ASSUME)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // STATEMENT ATTRIBUTES
    case K::OPAQUE:
      if constexpr (!getCanBeSituation<SP>(K::OPAQUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::GLOBAL:
      if constexpr (!getCanBeSituation<SP>(K::GLOBAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::STATIC:
      if constexpr (!getCanBeSituation<SP>(K::STATIC)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::STATIC_CAPTURE:
      if constexpr (!getCanBeSituation<SP>(K::STATIC_CAPTURE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 0, S::ARGUMENT>(expression);
      }
      break;
    case K::EVALUATE_IMMEDIATELY:
      if constexpr (!getCanBeSituation<SP>(K::EVALUATE_IMMEDIATELY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::MAY_PARENT:
      if constexpr (!getCanBeSituation<SP>(K::MAY_PARENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::PARENT:
      if constexpr (!getCanBeSituation<SP>(K::PARENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::ABSTRACT:
      if constexpr (!getCanBeSituation<SP>(K::ABSTRACT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::VIRTUAL:
      if constexpr (!getCanBeSituation<SP>(K::VIRTUAL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::OVERRIDE:
      if constexpr (!getCanBeSituation<SP>(K::OVERRIDE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::POSITION:
      if constexpr (!getCanBeSituation<SP>(K::POSITION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::INLINE:
      if constexpr (!getCanBeSituation<SP>(K::INLINE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::MANGLED_NAME:
      if constexpr (!getCanBeSituation<SP>(K::MANGLED_NAME)) {
        RQ_UNREACHABLE();
      } else if constexpr (SP == S::REFLECTION) {
        this->situateNullaryExpression<SP>(expression);
      } else {
        this->situateUnaryExpression<SP, rq::Situation::RVALUE>(expression);
      }
      break;
    case K::_MANGLED_NAME_OF:
      if constexpr (!getCanBeSituation<SP>(K::_MANGLED_NAME_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::PACK:
      if constexpr (!getCanBeSituation<SP>(K::PACK)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::USER:
      if constexpr (!getCanBeSituation<SP>(K::USER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LABEL:
      if constexpr (!getCanBeSituation<SP>(K::LABEL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::TEMPLATE:
      if constexpr (!getCanBeSituation<SP>(K::TEMPLATE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNaryExpression<SP, 1, S::PARAMETER>(expression);
      }
      break;
    case K::LIKELY:
      if constexpr (!getCanBeSituation<SP>(K::LIKELY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::UNLIKELY:
      if constexpr (!getCanBeSituation<SP>(K::UNLIKELY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::DEPRECIATED:
      if constexpr (!getCanBeSituation<SP>(K::DEPRECIATED)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::NO_SHALLOW_COPY:
      if constexpr (!getCanBeSituation<SP>(K::UNLIKELY)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;

    // EXPRESSIONS
    case K::QUOTE:
      if constexpr (!getCanBeSituation<SP>(K::QUOTE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::NONE>(expression);
      }
      break;
    case K::EXPRESSION:
      if constexpr (!getCanBeSituation<SP>(K::EXPRESSION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::EXPAND:
      if constexpr (!getCanBeSituation<SP>(K::EXPAND)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_EXPAND_TOP_STATEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_TOP_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_TABLE_STATEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_TABLE_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_OBJECT_STATEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_OBJECT_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_LOCAL_STATEMENT:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_LOCAL_STATEMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_LVALUE:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_LVALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_RVALUE:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_RVALUE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_REFLECTION:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_REFLECTION)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_ARGUMENT:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_ARGUMENT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_PARAMETER:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_PARAMETER)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_ENUMERATOR:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_ENUMERATOR)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_PATH:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_PATH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_NAME:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_NAME)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_SEQUENCE_STAGE:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_SEQUENCE_STAGE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_ARM:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_ARM)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::_EXPAND_DYNAMIC_CAPTURE:
      if constexpr (!getCanBeSituation<SP>(K::_EXPAND_DYNAMIC_CAPTURE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    // REFLECTIONS
    case K::_REFLECT:
      if constexpr (!getCanBeSituation<SP>(K::_REFLECT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateReflectExpression<SP>(expression);
      }
      break;
    case K::_MEMBER_OF:
      if constexpr (!getCanBeSituation<SP>(K::_MEMBER_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateBinaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SIZE:
      if constexpr (!getCanBeSituation<SP>(K::SIZE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SIZE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SIZE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DEPTH:
      if constexpr (!getCanBeSituation<SP>(K::DEPTH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DEPTH_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DEPTH_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::COUNT:
      if constexpr (!getCanBeSituation<SP>(K::COUNT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_COUNT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_COUNT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LENGTH:
      if constexpr (!getCanBeSituation<SP>(K::LENGTH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_LENGTH_OF:
      if constexpr (!getCanBeSituation<SP>(K::_LENGTH_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SPAN_SIZE:
      if constexpr (!getCanBeSituation<SP>(K::SPAN_SIZE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SPAN_SIZE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SPAN_SIZE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SPAN_DEPTH:
      if constexpr (!getCanBeSituation<SP>(K::SPAN_DEPTH)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SPAN_DEPTH_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SPAN_DEPTH_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::NAME:
      if constexpr (!getCanBeSituation<SP>(K::NAME)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_NAME_OF:
      if constexpr (!getCanBeSituation<SP>(K::_NAME_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LINE:
      if constexpr (!getCanBeSituation<SP>(K::LINE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_LINE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_LINE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::COLUMN:
      if constexpr (!getCanBeSituation<SP>(K::COLUMN)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_COLUMN_OF:
      if constexpr (!getCanBeSituation<SP>(K::_COLUMN_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::IS:
      if constexpr (!getCanBeSituation<SP>(K::IS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_VALUE_IS:
      if constexpr (!getCanBeSituation<SP>(K::_VALUE_IS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::HOLDS:
      if constexpr (!getCanBeSituation<SP>(K::HOLDS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_VALUE_HOLDS:
      if constexpr (!getCanBeSituation<SP>(K::_VALUE_HOLDS)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::TYPE:
      if constexpr (!getCanBeSituation<SP>(K::TYPE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_TYPE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_TYPE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SYMBOL:
      if constexpr (!getCanBeSituation<SP>(K::SYMBOL)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SYMBOL_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SYMBOL_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::DISCRIMINANT:
      if constexpr (!getCanBeSituation<SP>(K::DISCRIMINANT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_DISCRIMINANT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_DISCRIMINANT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::SIGNATURE:
      if constexpr (!getCanBeSituation<SP>(K::SIGNATURE)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_SIGNATURE_OF:
      if constexpr (!getCanBeSituation<SP>(K::_SIGNATURE_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;
    case K::LAYOUT:
      if constexpr (!getCanBeSituation<SP>(K::LAYOUT)) {
        RQ_UNREACHABLE();
      } else {
        this->situateNullaryExpression<SP>(expression);
      }
      break;
    case K::_LAYOUT_OF:
      if constexpr (!getCanBeSituation<SP>(K::_LAYOUT_OF)) {
        RQ_UNREACHABLE();
      } else {
        this->situateUnaryExpression<SP, S::RVALUE>(expression);
      }
      break;

    case K::__LAST:
      RQ_UNREACHABLE();
    }
    if (expression.getIsConverging()) {
      for (rq::Expression &branch : expression.getBranchSubrange()) {
        if (expression.getKeyword() == branch.getKeyword()) {
          this->getFrame().discardExpression(
              expression.mergeAndPopBranch());
        }
      }
    }
  }
  inline void situateRoot(rq::Module &module) {
    RQ_ASSERT(this->_is_ok, "situator can situate only once");
    if (!module.getHasExpression()) {
      rq::Expression &root = this->getFrame().acquireExpression();
      root.setKeyword(rq::Keyword::_MODULE_ROOT);
      root.setSourceTextInserted(module.getSourceText());
      module.setExpression(root);
    } else {
      rq::Expression &first = module.getExpression();
      if (first.getKeyword() != rq::Keyword::_MODULE_ROOT) {
        rq::Expression &root = this->getFrame().acquireExpression();
        root.setKeyword(rq::Keyword::_MODULE_ROOT);
        root.setSourceTextInserted(module.getSourceText());
        root.setBranch(first);
        module.changeExpression(root);
      }
    }
    this->situateExpression<rq::Situation::ROOT_STATEMENT>(
        module.getExpression());
  }
};

template <rq::Situation SITUATION_PARAM, rq::Situation BRANCH_SITUATION_PARAM>
inline void Situator::situateBranch(llvm::Twine log_context,
                                    rq::Expression &outer, unsigned branch_i,
                                    rq::Expression &branch) {
  if (BRANCH_SITUATION_PARAM == rq::Situation::NONE) {
    return;
  }
  const bool is_ok =
      rq::getCanBeSituation<BRANCH_SITUATION_PARAM>(branch.getKeyword());
  if (!is_ok) {
    this->getContext().logErrorInvalidBranchSituation(
        branch, SITUATION_PARAM, BRANCH_SITUATION_PARAM, outer.getKeyword(),
        branch.getKeyword(), branch_i, log_context);
    this->setNotOk();
    return;
  }
  this->situateExpression<BRANCH_SITUATION_PARAM>(branch);
}

} // namespace rq