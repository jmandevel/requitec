#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <utility>

namespace rq {

rq::Expression &Situator::makeModuleRoot(rq::Module &module) {
  rq::Expression &root = this->getStaticFrame().acquireExpression();
  root.setIsInserted();
  root.setSource(module);
  return root;
}

bool Situator::situateModule(rq::Module &module) {
  if (!module.getHasExpression()) {
    rq::Expression &root = this->makeModuleRoot(module);
    module.setExpression(root);
    return true;
  }
  rq::Expression &first = module.getExpression();
  if (first.getKeyword() != rq::Keyword::S_MODULE_ROOT) {
    rq::Expression &root = this->makeModuleRoot(module);
    root.setNext(module.replaceExpression(root));
  }
  rq::Expression &root = module.getExpression();
  return this->situateTree(rq::Situation::ROOT_STATEMENT, root);
}

#define RQ_ASSERT_VALID_SITUATION(situation, expression)                       \
  RQ_ASSERT(expression.getCanBeSituation(situation),                           \
            "keyword not valid for situation")

#define RQ_ASSERT_NOT_CHAIN_LINK(expression)                                   \
  RQ_ASSERT(!expression.getIsChainLink(), "branch must never be chain link")

#define RQ_ASSERT_NOT_BOLD(expression)                                         \
  RQ_ASSERT(!expression.getIsBold(), "branch must never be bold")

bool Situator::situateTree(rq::Situation situation,
                           rq::Expression &expression) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  using S = rq::Situation;
  using K = rq::Keyword;
  switch (expression.getKeyword()) {
  case K::I_NONE:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    return false;

  // LITERALS
  case K::I_INTEGER_LITERAL:
    [[fallthrough]];
  case K::I_FLOAT_LITERAL:
    [[fallthrough]];
  case K::I_STRING_LITERAL:
    [[fallthrough]];
  case K::I_LEFT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_MIDDLE_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_RIGHT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::I_CODEUNIT_LITERAL:
    [[fallthrough]];
  case K::I_IDENTIFIER_LITERAL:
    RQ_ASSERT(!expression.getHasBranch(), "literal with branch");
    return true;

  // ERRORS
  case K::I_ERROR:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    return false;

  // SITUATIONAL
  case K::S_PARENTHESIS_GROUP: {
    const bool is_ok = this->situateUnaryNonStatementBranches(
        situation, expression, situation);
    if (is_ok) {
      this->getStaticFrame().discardExpression(expression.mergeAndPopBranch());
    }
    return is_ok;
  } break;
  case K::S_EQUAL_OPERATOR:
    switch (situation) {
    case S::ARGUMENT: {
      const bool is_ok = this->situateBinaryNonStatementBranches(
          situation, expression, S::NAME, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::S_NAMED_ARGUMENT);
      }
      return is_ok;
    } break;
    case S::PARAMETER: {
      const bool is_ok = this->situateBinaryNonStatementBranches(
          situation, expression, S::BINDING, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::S_DEFAULT_VALUE_PARAMETER);
      }
      return is_ok;
    } break;
    case S::ENUMERATOR: {
      const bool is_ok = this->situateBinaryNonStatementBranches(
          situation, expression, S::NAME, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::S_DISCRIMINANT_VALUE_ENUMERATOR);
      }
      return is_ok;
    } break;
    case S::LOCAL_STATEMENT:
      [[fallthrough]];
    case S::TOP_STATEMENT:
      [[fallthrough]];
    case S::TABLE_STATEMENT:
      [[fallthrough]];
    case S::OBJECT_STATEMENT: {
      if (!expression.getHasBranch()) {
        this->getContext().logErrorNotExactBranchCount(situation, expression,
                                                       2);
        return false;
      }
      rq::Expression &lvalue = expression.getBranch();
      bool is_ok = this->situateBranch(situation, expression, S::LVALUE, lvalue,
                                       0, "first branch");
      if (!lvalue.getHasNext()) {
        this->getContext().logErrorNotExactBranchCount(situation, expression,
                                                       2);
        return false;
      }
      rq::Expression &rvalue = lvalue.getNext();
      if (!this->situateBranch(situation, expression, S::RVALUE, rvalue, 1,
                               "second branch")) {
        is_ok = false;
      }
      if (!is_ok) {
        return false;
      }
      switch (lvalue.getKeyword()) {
      case K::S_NULL:
        expression.changeKeyword(K::S_IGNORE);
        this->getStaticFrame().discardExpression(
            expression.replaceBranch(rvalue));
        return true;
      case K::S_TUPLE:
        lvalue.changeKeyword(K::S_STRUCTURED_BINDING);
        this->getStaticFrame().discardExpression(
            expression.mergeAndPopBranch());
        return true;
      default:
        expression.changeKeyword(K::S_ASSIGN);
        return true;
      }
      break;
    }
    default:
      break;
    }
    RQ_UNREACHABLE();
  case K::S_COLON_OPERATOR:
    if (!this->situateBinaryNonStatementBranches(situation, expression,
                                                 S::RVALUE, S::RVALUE)) {
      return false;
    }
    switch (situation) {
    case S::PARAMETER:
      [[fallthrough]];
    case S::ENUMERATOR:
      expression.changeKeyword(K::S_BINDING);
      return true;
    case S::RVALUE:
      [[fallthrough]];
    case S::ARGUMENT:
      expression.changeKeyword(K::S_CAST);
      return true;
    default:
      break;
    }
    RQ_UNREACHABLE();
  case K::S_INFERENCE:
    return this->situateNullaryExpression(situation, expression);

  // LOGICAL
  case K::S_LOGICAL_AND:
    [[fallthrough]];
  case K::S_LOGICAL_OR:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE);
  case K::S_LOGICAL_COMPLEMENT:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // COMPARISON
  case K::S_GREATER:
    [[fallthrough]];
  case K::S_GREATER_EQUAL:
    [[fallthrough]];
  case K::S_LESS:
    [[fallthrough]];
  case K::S_LESS_EQUAL:
    [[fallthrough]];
  case K::S_EQUAL:
    [[fallthrough]];
  case K::S_NOT_EQUAL:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE);

  // APPLY
  case K::S_EXTEND:
    [[fallthrough]];
  case K::S_EXTENSION:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_BINDING:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::NAME, S::RVALUE);
  case K::S_ASCRIBE_TYPE:
    [[fallthrough]];
  case K::S_ASCRIBE_STATEMENT: {
    rq::Situation attribute_situation = S::TYPE_ATTRIBUTE;
    if (expression.getKeyword() == K::S_ASCRIBE_STATEMENT) {
      attribute_situation = S::STATEMENT_ATTRIBUTE;
    }
    bool is_ok = true;
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      return false;
    }
    rq::Expression &first = expression.getBranch();
    if (!first.getCanBeTypeAttribute()) {
      is_ok = this->situateNaryNonStatementBranches(
          situation, expression, 2, situation, attribute_situation);
    } else {
      unsigned branch_i = 0;
      is_ok = this->situateBranch(situation, expression, attribute_situation,
                                  first, 0, "first to penultimate branch");
      if (!first.getHasNext()) {
        this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                         2);
        return false;
      }
      rq::Expression *previous_ptr = &first;
      for (rq::Expression &next : first.getNextSubrange()) {
        if (!next.getHasNext()) {
          if (!this->situateBranch(situation, expression, situation, next,
                                   branch_i++, "last branch")) {
            is_ok = false;
          }
          if (is_ok) {
            rq::Expression &previous = rq::dereferencePtr(previous_ptr);
            next.setNext(expression.replaceBranch(previous.popNext()));
          }
          return is_ok;
        }
        if (!this->situateBranch(situation, expression, attribute_situation,
                                 next, branch_i++,
                                 "first to penultimate branch")) {
          is_ok = false;
        }
        previous_ptr = &next;
      }
    }
    if (!is_ok) {
      return false;
    }
  }
    return true;
  case K::S_CAST:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_IDENTIFY:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // ARITHMETIC
  case K::S_ADD:
    [[fallthrough]];
  case K::S_SUBTRACT:
    [[fallthrough]];
  case K::S_MULTIPLY:
    [[fallthrough]];
  case K::S_DIVIDE:
    [[fallthrough]];
  case K::S_MODULUS:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE);
  case K::S_NEGATE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // BITWISE
  case K::S_BITWISE_CAST:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_BITWISE_OR:
    [[fallthrough]];
  case K::S_BITWISE_AND:
    [[fallthrough]];
  case K::S_BITWISE_XOR:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE);
  case K::S_BITWISE_COMPLEMENT:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::S_BITWISE_SHIFT_LEFT:
    [[fallthrough]];
  case K::S_BITWISE_SHIFT_RIGHT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_LEFT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_RIGHT:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);

  // MEMORY
  case K::SINGLETON:
    [[fallthrough]];
  case K::CONTENT:
    [[fallthrough]];
  case K::ADDRESS:
    [[fallthrough]];
  case K::BORROW:
    return this->situateNullaryExpression(situation, expression);

  // ASSIGNMENT
  case K::S_ASSIGN:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::LVALUE, S::RVALUE);
  case K::S_ASSIGN_ADD:
    [[fallthrough]];
  case K::S_ASSIGN_SUBTRACT:
    [[fallthrough]];
  case K::S_ASSIGN_MULTIPLY:
    [[fallthrough]];
  case K::S_ASSIGN_DIVIDE:
    [[fallthrough]];
  case K::S_ASSIGN_MODULUS:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);

  // SUBTYPE
  case K::S_ARRAY:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_REFERENCE:
    [[fallthrough]];
  case K::S_POINTER:
    [[fallthrough]];
  case K::S_FAT_POINTER:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // TYPE MODIFIER
  case K::MUTABLE:
    [[fallthrough]];
  case K::CONSTANT:
    [[fallthrough]];
  case K::PARTIALLY_MUTABLE:
    [[fallthrough]];
  case K::VOLATILE:
    [[fallthrough]];
  case K::ATOMIC:
    [[fallthrough]];
  case K::NULL_TERMINATED:
    [[fallthrough]];
  case K::MAY_DISCARD:
    [[fallthrough]];
  case K::DEBUG_TRAP_ON_PANIC:
    [[fallthrough]];
  case K::LINEAR:
    return this->situateNullaryExpression(situation, expression);
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return this->situateNaryParameterBranches(situation, expression);

  // PARAMETER RULES
  case K::S_POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case K::S_NAMED_PARAMETERS_BEGIN:
    return this->situateNullaryExpression(situation, expression);

  // BRACES
  case K::S_TUPLE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE);
  case K::S_LAYOUT_TYPE:
    return this->situateNaryParameterBranches(situation, expression);
  case K::S_NULL:
    [[fallthrough]];
  case K::S_NULL_TYPE:
    return this->situateNullaryExpression(situation, expression);
  case K::S_STRUCTURED_BINDING:
    return this->situateNaryDifferentLastNonStatementBranches(
        situation, expression, 2, S::LVALUE, S::RVALUE);
  case K::S_IGNORE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::S_SPECIALIZATION:
    return this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);

  // PROCEDURES
  case K::S_CALL:
    return this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
  case K::S_NAMED_ARGUMENT:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::NAME, S::RVALUE);
  case K::S_CONSTRUCT_FUNCTOR:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE, S::ARGUMENT);
  case K::S_INDEX_INTO:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_SIGNATURE_TYPE:
    return this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
  case K::S_DEFAULT_VALUE_PARAMETER:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::BINDING, S::RVALUE);
  case K::DESTROY:
    [[fallthrough]];
  case K::DROP:
    [[fallthrough]];
  case K::MOVE:
    return this->situateNullaryExpression(situation, expression);
  case K::ENTRY_POINT:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::LOCAL_STATEMENT);
  case K::FUNCTION:
    [[fallthrough]];
  case K::METHOD:
    [[fallthrough]];
  case K::EXTENSION_FUNCTION:
    [[fallthrough]];
  case K::EXTENSION_METHOD:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::CONSTRUCTOR:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::LAYOUT_CONSTRUCTOR:
    return this->situateNullaryExpression(situation, expression);
  case K::DESTRUCTOR:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::LOCAL_STATEMENT);
  case K::RANGER:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::S_ANONYMOUS_FUNCTION:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::S_DYNAMIC_CAPTURE:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::ARGUMENT);

  // CONTROL FLOW
  case K::RETURN:
    if (!expression.getHasBranch()) {
      return true;
    }
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::BREAK:
    [[fallthrough]];
  case K::CONTINUE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::FALLTHROUGH:
    return this->situateNullaryExpression(situation, expression);
  case K::GOTO:
    [[fallthrough]];
  case K::RANGE_OVER:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // DECLARED TYPES
  case K::OBJECT:
    [[fallthrough]];
  case K::ENUMERATION:
    if (situation == S::REFLECTION || situation == S::RVALUE) {
      return this->situateNullaryExpression(situation, expression);
    }
    return this->situateNaryNonStatementBranches(
        situation, expression, 2, S::RVALUE, S::OBJECT_STATEMENT);
  case K::S_DISCRIMINANT_VALUE_ENUMERATOR:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::LVALUE, S::RVALUE);

  // VALUES
  case K::TRUE:
    [[fallthrough]];
  case K::FALSE:
    [[fallthrough]];
  case K::INDETERMINATE:
    [[fallthrough]];
  case K::VALUE:
    [[fallthrough]];
  case K::INDEX:
    [[fallthrough]];
  case K::DISCRIMINANT:
    [[fallthrough]];
  case K::OUT:
    [[fallthrough]];
  case K::THIS:
    [[fallthrough]];
  case K::THIS_TYPE:
    [[fallthrough]];
  case K::RESULT:
    [[fallthrough]];
  case K::COMMAND_LINE_ARGUMENTS:
    [[fallthrough]];
  case K::EXIT_CODE:
    [[fallthrough]];
  case K::ADDRESS_BYTE_SIZE:
    [[fallthrough]];
  case K::ADDRESS_BIT_DEPTH:
    [[fallthrough]];
  case K::BITS_PER_BYTE:
    return this->situateNullaryExpression(situation, expression);

  // BUILTIN TYPES
  case K::VOID:
    [[fallthrough]];
  case K::NO_RETURN:
    [[fallthrough]];
  case K::BOOLEAN:
    return this->situateNullaryExpression(situation, expression);
  case K::WORD:
    [[fallthrough]];
  case K::SIGNED:
    [[fallthrough]];
  case K::UNSIGNED:
    if (!expression.getHasBranch()) {
      return true;
    }
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::BFLOAT16:
    [[fallthrough]];
  case K::BINARY16:
    [[fallthrough]];
  case K::BINARY32:
    [[fallthrough]];
  case K::BINARY64:
    [[fallthrough]];
  case K::BINARY128:
    [[fallthrough]];
  case K::UTF8:
    [[fallthrough]];
  case K::C_CHAR:
    [[fallthrough]];
  case K::C_UNSIGNED_CHAR:
    [[fallthrough]];
  case K::C_SIGNED_CHAR:
    [[fallthrough]];
  case K::C_WCHAR_T:
    [[fallthrough]];
  case K::C_SHORT_INT:
    [[fallthrough]];
  case K::C_UNSIGNED_SHORT_INT:
    [[fallthrough]];
  case K::C_INT:
    [[fallthrough]];
  case K::C_UNSIGNED_INT:
    [[fallthrough]];
  case K::C_LONG_INT:
    [[fallthrough]];
  case K::C_UNSIGNED_LONG_INT:
    [[fallthrough]];
  case K::C_LONG_LONG_INT:
    [[fallthrough]];
  case K::C_UNSIGNED_LONG_LONG_INT:
    [[fallthrough]];
  case K::C_FLOAT:
    [[fallthrough]];
  case K::C_DOUBLE:
    [[fallthrough]];
  case K::C_LONG_DOUBLE:
    return this->situateNullaryExpression(situation, expression);

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    [[fallthrough]];
  case K::FIRST_VARIADIC_ARGUMENT:
    [[fallthrough]];
  case K::NEXT_VARIADIC_ARGUMENT:
    return this->situateNullaryExpression(situation, expression);

  // SCOPES
  case K::IF:
    [[fallthrough]];
  case K::ELSE_IF:
    return this->situateSingleBoldRValueStatementsExpression()
  case K::ELSE:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 situation);
  case K::MATCH:
    [[fallthrough]];
  case K::INLINE_MATCH:
    [[fallthrough]];
  case K::SWITCH:
    [[fallthrough]];
  case K::INLINE_SWITCH: {
    bool is_ok = true;
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      return is_ok;
    }
    rq::Expression &condition = expression.getBranch();
    if (!this->situateBranch(situation, expression, S::RVALUE, condition, 0,
                             "condition branch")) {
      is_ok = false;
    }
    if (!condition.getIsBold()) {
      this->getContext().logErrorNotBoldExpression(
          situation, expression, condition, 0, "condition branch");
      is_ok = false;
    }
    if (!condition.getHasNext()) {
      return is_ok;
    }
    bool previous_is_chainlink = false;
    unsigned branch_i = 1;
    for (rq::Expression &arm : condition.getNextSubrange()) {
      switch (arm.getKeyword()) {
      case K::CASE:
        [[fallthrough]];
      case K::DEFAULT:
        if (arm.getIsChainlink()) {
          previous_is_chainlink = true;
        }
        if (!arm.getHasNext() && arm.getIsChainLink()) {
          this->getContext().logErrorChainLinkExpressionWithNothingAfter(
              situation, expression, S::ARM, arm, branch_i);
          is_ok = false;
        } else if (!arm.getIsChainLink()) {
          this->getContext().logErrorNotChainLinkExpression(
              situation, expression, S::ARM, arm, branch_i);
          is_ok = false;
        }
        break;
      default:
        break;
      }
      if (!this->situateBranch(situation, expression, S::ARM, arm, branch_i,
                               "second to last branch")) {
        is_ok = false;
      }
      branch_i++;
    }
    return is_ok;
  } break;
  case K::CASE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::DEFAULT:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::LOCAL_STATEMENT);
  case K::FOR:
    [[fallthrough]];
  case K::WHILE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE, S::LOCAL_STATEMENT);
  case K::SCOPE:
    [[fallthrough]];
  case K::INLINE_SCOPE:
    [[fallthrough]];
  case K::BLOCK:
    [[fallthrough]];
  case K::INLINE_BLOCK:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 situation);

  // RANGES
  case K::RANGE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::S_SEQUENCE: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      return false;
    }
    rq::Expression &value = expression.getBranch();
    bool is_ok = this->situateBranch(situation, expression, S::RVALUE, value, 0,
                                     "first branch");
    if (!value.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      return false;
    }
    rq::Expression &stage_one = value.getNext();
    if (!this->situateBranch(situation, expression, S::SEQUENCE_STAGE,
                             stage_one, 1, "second branch")) {
      is_ok = false;
    }
    if (stage_one.getHasNext()) {
      rq::Expression &stage_two = stage_one.getNext();
      if (!this->situateBranch(situation, expression, S::SEQUENCE_STAGE,
                               stage_two, 2, "third branch")) {
        is_ok = false;
      }
      if (stage_two.getHasNext()) {
        this->getContext().logErrorTooManyBranchCount(situation, expression, 3);
        return false;
      }
    }
    return is_ok;
  }
  case K::S_SEQUENCE_STEP_ADD:
    [[fallthrough]];
  case K::S_SEQUENCE_STEP_SUBTRACT:
    [[fallthrough]];
  case K::S_SEQUENCE_STEP_MULTIPLY:
    [[fallthrough]];
  case K::S_SEQUENCE_STEP_DIVIDE:
    [[fallthrough]];
  case K::S_SEQUENCE_STEP_MODULUS:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_LESS:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_GREATER:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_LESS_EQUAL:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_GREATER_EQUAL:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_EQUAL:
    [[fallthrough]];
  case K::S_SEQUENCE_WHILE_NOT_EQUAL:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // ACCESS MODIFIERS
  case K::PUBLIC:
    [[fallthrough]];
  case K::PROTECTED:
    [[fallthrough]];
  case K::EXPORT:
    return this->situateNullaryExpression(situation, expression);

  // TABLE GRAPH
  case K::IMPORT:
    [[fallthrough]];
  case K::USE:
    [[fallthrough]];
  case K::USE_TABLE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::FACADE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::SYMBOL_PATH, S::TABLE_STATEMENT);
  case K::TABLE:
    [[fallthrough]];
  case K::MODULE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::SYMBOL_PATH, S::TABLE_STATEMENT);
  case K::S_MODULE_ROOT:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::TOP_STATEMENT);

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::DEBUG_TRAP:
    return this->situateNullaryExpression(situation, expression);

  // HINTS
  case K::UNREACHABLE:
    return this->situateNullaryExpression(situation, expression);
  case K::ASSUME:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // STATEMENT ATTRIBUTES
  case K::OPAQUE:
    [[fallthrough]];
  case K::GLOBAL:
    [[fallthrough]];
  case K::STATIC:
    return this->situateNullaryExpression(situation, expression);
  case K::STATIC_CAPTURE:
    return this->situateNaryNonStatementBranches(situation, expression, 0,
                                                 S::ARGUMENT);
  case K::EVALUATE_IMMEDIATELY:
    [[fallthrough]];
  case K::MAY_PARENT:
    [[fallthrough]];
  case K::PARENT:
    [[fallthrough]];
  case K::ABSTRACT:
    [[fallthrough]];
  case K::VIRTUAL:
    return this->situateNullaryExpression(situation, expression);
  case K::OVERRIDE:
    [[fallthrough]];
  case K::POSITION:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::INLINE:
    return this->situateNullaryExpression(situation, expression);
  case K::MANGLED_NAME:
    switch (situation) {
    case S::STATEMENT_ATTRIBUTE:
      return this->situateUnaryNonStatementBranches(situation, expression,
                                                    S::RVALUE);
    case S::REFLECTION:
      return this->situateNullaryExpression(situation, expression);
    default:
      break;
    }
    RQ_UNREACHABLE();
  case K::PACK:
    return this->situateNullaryExpression(situation, expression);
  case K::USER_ATTRIBUTE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::LABEL:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::SYMBOL_PATH);
  case K::TEMPLATE:
    return this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::PARAMETER);
  case K::LIKELY:
    [[fallthrough]];
  case K::UNLIKELY:
    return this->situateNullaryExpression(situation, expression);
  case K::DEPRECIATED:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::MAY_COPY:
    [[fallthrough]];
  case K::MAY_MOVE:
    [[fallthrough]];
  case K::MUTABILITY_CLASS:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::SYMBOL_PATH);

  // EXPRESSIONS
  case K::QUOTE:
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      return false;
    }
    return true;
  case K::EXPRESSION:
    [[fallthrough]];
  case K::EXPAND:
    return this->situateNullaryExpression(situation, expression);
  case K::S_EXPAND_TOP_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_TABLE_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_OBJECT_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_LOCAL_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_ARM_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_ENUMERATOR_STATEMENT:
    [[fallthrough]];
  case K::S_EXPAND_LVALUE:
    [[fallthrough]];
  case K::S_EXPAND_RVALUE:
    [[fallthrough]];
  case K::S_EXPAND_REFLECTION:
    [[fallthrough]];
  case K::S_EXPAND_ARGUMENT:
    [[fallthrough]];
  case K::S_EXPAND_PARAMETER:
    [[fallthrough]];
  case K::S_EXPAND_SEQUENCE_STAGE:
    [[fallthrough]];
  case K::S_EXPAND_VIGNETTE:
    [[fallthrough]];
  case K::S_EXPAND_VIGNETTE_RVALUE:
    [[fallthrough]];
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

  // REFLECTIONS
  case K::S_REFLECT: {
    bool is_ok = true;
    unsigned branch_i = 0;
    if (expression.getHasBranch()) {
      rq::Expression &branch0 = expression.getBranch();
      RQ_ASSERT_NOT_BOLD(branch0);
      RQ_ASSERT_NOT_CHAIN_LINK(branch0);
      if (!this->situateBranch(situation, expression, rq::Situation::RVALUE,
                               branch0, branch_i++, "first branch")) {
        is_ok = false;
      }
      for (rq::Expression &branch : expression.getBranchSubrange()) {
        RQ_ASSERT_NOT_BOLD(branch);
        RQ_ASSERT_NOT_CHAIN_LINK(branch);
        if (!branch.getHasNext()) {
          if (!this->situateBranch(situation, expression, situation, branch,
                                   branch_i++, "last branch")) {
            is_ok = false;
          } else if (expression.getKeyword() == rq::Keyword::EXPAND) {
            expression.changeKeyword(rq::getExpandOfSituation(situation));
          }
          break;
        }
        if (!this->situateBranch(situation, expression,
                                 rq::Situation::REFLECTION, branch, branch_i++,
                                 "first to penultimate branch")) {
          is_ok = false;
        } else if (expression.getKeyword() == rq::Keyword::EXPAND) {
          expression.changeKeyword(rq::Keyword::S_EXPAND_REFLECTION);
        }
      }
    }
    if (branch_i < 2) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
    }
    return is_ok;
  }
  case K::BYTE_SIZE:
    [[fallthrough]];
  case K::BIT_DEPTH:
    [[fallthrough]];
  case K::ELEMENT_COUNT:
    [[fallthrough]];
  case K::NAME:
    [[fallthrough]];
  case K::LINE:
    [[fallthrough]];
  case K::COLUMN:
    [[fallthrough]];
  case K::IS:
    [[fallthrough]];
  case K::HOLDS:
    [[fallthrough]];
  case K::TYPE:
    [[fallthrough]];
  case K::SYMBOL:
    [[fallthrough]];
  case K::SIGNATURE:
    [[fallthrough]];
  case K::LAYOUT:
    [[fallthrough]];
  case K::CONSTRUCT_FUNCTOR:
    return this->situateNullaryExpression(situation, expression);

  case K::I_LAST:
    RQ_UNREACHABLE();
  }
  return true;
}

bool Situator::situateBranch(rq::Situation situation,
                             rq::Expression &expression,
                             rq::Situation branch_situation,
                             rq::Expression &branch, unsigned branch_i,
                             llvm::StringRef log_context) {
  if (branch_situation == rq::Situation::NONE) {
    return true;
  }
  if (!branch.getCanBeSituation(branch_situation)) {
    this->getContext().logErrorInvalidBranchSituation(
        situation, expression, branch_situation, branch, branch_i, log_context);
    return false;
  }
  return this->situateTree(branch_situation, branch);
}

bool Situator::situateNullaryExpression(rq::Situation situation,
                                        rq::Expression &expression) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 0);
    return false;
  }
  return true;
}

bool Situator::situateUnaryExpression(rq::Situation situation,
                                      rq::Expression &expression) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  rq::Expression &branch = expression.getBranch();
  if (branch.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  return true;
}

bool Situator::situateUnaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  RQ_ASSERT_NOT_BOLD(branch0);
  RQ_ASSERT_NOT_CHAIN_LINK(branch0);
  bool is_ok = this->situateBranch(situation, expression, branch0_situation,
                                   branch0, 0, "first branch");
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateBinaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  RQ_ASSERT_NOT_BOLD(branch0);
  RQ_ASSERT_NOT_CHAIN_LINK(branch0);
  bool is_ok = this->situateBranch(situation, expression, branch0_situation,
                                   branch0, 0, "first branch");
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch1 = branch0.getNext();
  RQ_ASSERT_NOT_BOLD(branch1);
  RQ_ASSERT_NOT_CHAIN_LINK(branch1);
  if (!this->situateBranch(situation, expression, branch1_situation, branch1, 1,
                           "second branch")) {
    is_ok = false;
  }
  if (branch1.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  unsigned branch_i = 0;
  bool is_ok = true;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    RQ_ASSERT_NOT_BOLD(branch);
    RQ_ASSERT_NOT_CHAIN_LINK(branch);
    if (!this->situateBranch(situation, expression, branchn_situation, branch,
                             branch_i++, "first to last branch")) {
      is_ok = false;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  unsigned branch_i = 0;
  bool is_ok = true;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    is_ok = this->situateBranch(situation, expression, branch0_situation,
                                branch0, 0, "first branch");
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      RQ_ASSERT_NOT_BOLD(branch);
      RQ_ASSERT_NOT_CHAIN_LINK(branch);
      if (!this->situateBranch(situation, expression, branchn_situation, branch,
                               branch_i++, "second to last branch")) {
        is_ok = false;
      }
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentLastNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation,
    rq::Situation last_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    RQ_ASSERT_NOT_BOLD(branch);
    RQ_ASSERT_NOT_CHAIN_LINK(branch);
    if (!branch.getHasNext()) {
      if (!this->situateBranch(situation, expression, last_situation, branch,
                               branch_i++, "last branch")) {
        is_ok = false;
      }
      break;
    }
    if (!this->situateBranch(situation, expression, branchn_situation, branch,
                             branch_i++, "first to penultimate branch")) {
      is_ok = false;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentLastNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation, rq::Situation last_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    RQ_ASSERT_NOT_BOLD(branch0);
    RQ_ASSERT_NOT_CHAIN_LINK(branch0);
    if (!this->situateBranch(situation, expression, branch0_situation, branch0,
                             branch_i++, "first branch")) {
      is_ok = false;
    }
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      RQ_ASSERT_NOT_BOLD(branch);
      RQ_ASSERT_NOT_CHAIN_LINK(branch);
      if (!branch.getHasNext()) {
        if (!this->situateBranch(situation, expression, last_situation, branch,
                                 branch_i++, "last branch")) {
          is_ok = false;
        }
        break;
      }
      if (!this->situateBranch(situation, expression, branchn_situation, branch,
                               branch_i++, "first to penultimate branch")) {
        is_ok = false;
      }
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstParamterBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateBranch(situation, expression, branch0_situation, branch0, 0,
                           "first branch")) {
    return false;
  }
  if (!branch0.getHasNext()) {
    return false;
  }
  return this->situateNaryFromFirstParameterBranches(
      situation, expression, branch0.getNext(), 1, "second to last branch");
}

bool Situator::situateNaryParameterBranches(rq::Situation situation,
                                            rq::Expression &expression) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  if (!expression.getHasBranch()) {
    return true;
  }
  return this->situateNaryFromFirstParameterBranches(
      situation, expression, expression.getBranch(), 0, "first to last branch");
}

bool Situator::situateNaryFromFirstParameterBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Expression &first_parameter, unsigned first_parameter_i,
    llvm::StringRef log_context) {
  RQ_ASSERT_VALID_SITUATION(situation, expression);
  unsigned branch_i = first_parameter_i;
  bool is_ok = true;
  rq::Expression *first_named_begin_ptr = nullptr;
  rq::Expression *first_positional_end_ptr = nullptr;
  unsigned first_positional_end_i = 0;
  for (rq::Expression &parameter : first_parameter.getInclusiveNextSubrange()) {
    RQ_ASSERT_NOT_BOLD(parameter);
    RQ_ASSERT_NOT_CHAIN_LINK(parameter);
    if (!this->situateBranch(situation, expression, rq::Situation::PARAMETER,
                             parameter, branch_i, log_context)) {
      is_ok = false;
    }
    switch (parameter.getKeyword()) {
    case rq::Keyword::S_NAMED_PARAMETERS_BEGIN:
      if (first_named_begin_ptr != nullptr) {
        rq::Expression &first_named_begin =
            rq::dereferencePtr(first_named_begin_ptr);
        this->getContext().logErrorDuplicateParameterMark(
            situation, expression, parameter, branch_i, first_named_begin,
            first_positional_end_i);
        is_ok = false;
      } else {
        first_named_begin_ptr = &parameter;
      }
      if (!parameter.getHasNext()) {
        is_ok = false;
        this->getContext().logErrorNamedBeginIsLast(situation, expression,
                                                    parameter, branch_i);
      }
      if (first_positional_end_ptr != nullptr) {
        is_ok = false;
        rq::Expression &first_positional_end =
            rq::dereferencePtr(first_positional_end_ptr);
        this->getContext().logErrorNamedBeginAfterPositionalEnd(
            situation, expression, parameter, branch_i, first_positional_end,
            first_positional_end_i);
      }
      break;
    case rq::Keyword::S_POSITIONAL_PARAMETERS_END:
      if (first_positional_end_ptr != nullptr) {
        rq::Expression &first_positional_end =
            rq::dereferencePtr(first_positional_end_ptr);
        this->getContext().logErrorDuplicateParameterMark(
            situation, expression, parameter, branch_i, first_positional_end,
            first_positional_end_i);
        is_ok = false;
      } else {
        first_positional_end_ptr = &parameter;
        first_positional_end_i = branch_i;
      }
      if (branch_i == first_parameter_i) {
        is_ok = false;
        this->getContext().logErrorPositionalEndIsFirst(situation, expression,
                                                        parameter, branch_i);
      }
      break;
    default:
      break;
    }
    branch_i += 1;
  }
  if (first_named_begin_ptr == nullptr && first_positional_end_ptr == nullptr) {
    this->getContext().logErrorMustHaveParameterMark(situation, expression);
    is_ok = false;
  }
  return is_ok;
}

} // namespace rq
