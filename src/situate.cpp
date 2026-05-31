#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallVector.h>

#include <utility>

namespace rq {

bool Situator::situateModule(rq::ModuleFactory &factory) {
  if (factory.getExpressionPtr() == nullptr) {
    rq::Expression &top = this->getContext().acquireExpression();
    top.setIsInserted();
    top.setKeyword(rq::Keyword::TOP);
    factory.setOrChangeExpression(&top);
  }
  rq::Expression &first = rq::dereferencePtr(factory.getExpressionPtr());
  if (first.getKeyword() != rq::Keyword::TOP) {
    rq::Expression &top = this->getContext().acquireExpression();
    top.setIsInserted();
    top.setKeyword(rq::Keyword::TOP);
    top.setBranch(first);
    factory.setOrChangeExpression(&top);
  }
  rq::Expression &top = rq::dereferencePtr(factory.getExpressionPtr());
  if (!this->situateTree(rq::Situation::TOP, top)) {
    return false;
  }
  return true;
}

bool Situator::situateTree(rq::Situation situation,
                           rq::Expression &expression) {
  RQ_ASSERT(expression.getCanBeSituation(situation),
            "keyword not valid for situation");
  using S = rq::Situation;
  using K = rq::Keyword;

  bool is_ok = true;
  switch (expression.getKeyword()) {
  case K::NONE:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    is_ok = false;
    break;

  // LITERALS
  case K::INTEGER_LITERAL:
    [[fallthrough]];
  case K::FLOAT_LITERAL:
    [[fallthrough]];
  case K::STRING_LITERAL:
    [[fallthrough]];
  case K::CODEUNIT_LITERAL:
    [[fallthrough]];
  case K::IDENTIFIER_LITERAL:
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    break;

  // ERRORS
  case K::ERROR:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    is_ok = false;
    break;

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &first = expression.getBranch();
    if (!this->situateValueBranch(S::RVALUE, first)) {
      is_ok = false;
      break;
    }
    if (!first.getHasNext()) {
      this->getContext().discardExpression(expression.mergeAndPopBranch());
      break;
    }
    if (situation != S::RVALUE) {
      this->getContext().logErrorTooManyBranchCount(situation, expression, 1);
      is_ok = false;
      break;
    }
    {
      bool found_error = false;
      for (rq::Expression &next : first.getNextSubrange()) {
        if (!this->situateValueBranch(S::RVALUE, next)) {
          found_error = true;
          break;
        }
      }
      if (found_error) {
        is_ok = false;
        break;
      }
    }
    expression.changeKeyword(K::FORK);
  } break;
  case K::UNSITUATED_EQUAL_OPERATOR:
    switch (situation) {
    case S::ARGUMENT: {
      if (!this->situateBinaryValueBranches(situation, expression, S::LVALUE,
                                            S::RVALUE)) {
        is_ok = false;
        break;
      }
      expression.changeKeyword(K::NAMED_ARGUMENT);
    } break;
    case S::PARAMETER: {
      if (!this->situateBinaryValueBranches(situation, expression, S::BINDING,
                                            S::RVALUE)) {
        is_ok = false;
        break;
      }
      expression.changeKeyword(K::DEFAULT_VALUE_PARAMETER);
    } break;
    case S::STATEMENT: {
      if (!expression.getHasBranch()) {
        this->getContext().logErrorNotExactBranchCount(situation, expression,
                                                       2);
        is_ok = false;
        break;
      }
      rq::Expression &lvalue = expression.getBranch();
      if (!this->situateValueBranch(S::LVALUE, lvalue)) {
        is_ok = false;
      }
      if (!lvalue.getHasNext()) {
        this->getContext().logErrorNotExactBranchCount(situation, expression,
                                                       2);
        is_ok = false;
        break;
      }
      rq::Expression &rvalue = lvalue.getNext();
      if (!this->situateValueBranch(S::RVALUE, rvalue)) {
        is_ok = false;
      }
      if (!is_ok) {
        break;
      }
      expression.changeKeyword(K::ASSIGN);
      break;
    }
    default:
      break;
    }
    break;
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
    is_ok = this->situateUnsituatedAscribeExpression(
        situation, expression, 2, K::ASCRIBE_EXPRESSION,
        S::EXPRESSION_ATTRIBUTE_INSTANTIATION, situation);
    break;
  case K::UNSITUATED_ASCRIBE_TYPE:
    is_ok = this->situateUnsituatedAscribeExpression(
        situation, expression, 2, K::ASCRIBE_TYPE,
        S::TYPE_ATTRIBUTE_INSTANTIATION, situation);
    break;

  // LOGICAL
  case K::LOGICAL_AND:
    [[fallthrough]];
  case K::LOGICAL_OR:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;
  case K::LOGICAL_COMPLEMENT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // COMPARISON
  case K::GREATER:
    [[fallthrough]];
  case K::GREATER_EQUAL:
    [[fallthrough]];
  case K::LESS:
    [[fallthrough]];
  case K::LESS_EQUAL:
    [[fallthrough]];
  case K::EQUAL:
    [[fallthrough]];
  case K::NOT_EQUAL:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;

  // APPLY
  case K::EXTEND:
    [[fallthrough]];
  case K::INITIALIZE_RECIEVER:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::BINDING:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::LVALUE,
                                             S::RVALUE);
    break;
  case K::ASCRIBE_TYPE:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE_INSTANTIATION);
    break;
  case K::ASCRIBE_EXPRESSION:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation,
        S::EXPRESSION_ATTRIBUTE_INSTANTIATION);
    break;
  case K::ASCRIBE_ROOT_OF_VALUE:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE_INSTANTIATION);
    break;
  case K::INSTANTIATE_EXPRESSION_ATTRIBUTE: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch = expression.getBranch();
    if (!this->situateValueBranch(S::RVALUE, branch)) {
      is_ok = false;
      break;
    }
    if (!branch.getHasNext()) {
      break;
    }
    rq::Expression &attribute_value = branch.getNext();
    if (!this->situateValueBranch(S::RVALUE, branch)) {
      is_ok = false;
      break;
    }
    if (attribute_value.getHasNext()) {
      this->getContext().logErrorTooManyBranchCount(situation, attribute_value,
                                                    2);
      is_ok = false;
    }
  } break;
  case K::INSTANTIATE_TYPE_ATTRIBUTE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IDENTIFY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IDENTIFY_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::FORK:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;

  // JUXTAPOSITIONAL
  case K::CONCATENATE:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;
  case K::APPEND:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;

  // ARITHMETIC
  case K::ADD:
    [[fallthrough]];
  case K::SUBTRACT:
    [[fallthrough]];
  case K::MULTIPLY:
    [[fallthrough]];
  case K::DIVIDE:
    [[fallthrough]];
  case K::MODULUS:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;
  case K::NEGATE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // CASTS
  case K::AS:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
    break;
  case K::AS_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::OF_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                     S::RVALUE);
    break;
  case K::CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::BITWISE_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                     S::RVALUE);
    break;
  case K::BITWISE_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::PROCEDURE_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                     S::RVALUE);
    break;
  case K::PROCEDURE_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;

  // BITWISE
  case K::BITWISE_OR:
    [[fallthrough]];
  case K::BITWISE_AND:
    [[fallthrough]];
  case K::BITWISE_XOR:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;
  case K::BITWISE_COMPLEMENT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BITWISE_SHIFT_LEFT:
    [[fallthrough]];
  case K::BITWISE_SHIFT_RIGHT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_LEFT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_RIGHT:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;

  // MEMORY
  case K::ASSIGN:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::LVALUE,
                                             S::RVALUE);
    break;
  case K::CONTENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::CONTENT_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ADDRESS_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SLICE:
    is_ok = this->situateNaryValueBranches(situation, expression, 0, S::RVALUE);
    break;
  case K::SLICE_OF:
    is_ok = this->situateNaryValueBranches(situation, expression, 1, S::RVALUE);
    break;
  case K::PROCEDURE_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::PROCEDURE_ADDRESS_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BORROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BORROW_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::DATA_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DATA_ADDRESS_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::AT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::AT_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::MOVE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MOVE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::TAKE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::TAKE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::CALL:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 1, S::RVALUE, S::ARGUMENT);
    break;
  case K::EMPLACE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::EMPLACE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::INVOKE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::INVOKE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::COMPOSE:
    is_ok = this->situateNaryValueBranches(situation, expression, 1, S::RVALUE);
    break;
  case K::COMPOSE_OF:
    is_ok = this->situateNaryValueBranches(situation, expression, 2, S::RVALUE);
    break;
  case K::DESTROY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DESTROY_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::DROP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DROP_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::INPLACE_DESTROY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::INPLACE_DESTROY_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::INPLACE_INITIALIZE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::INPLACE_INITIALIZE_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;

  // SUBTYPE
  case K::INSTANTIATE_ARRAY:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::INSTANTIATE_REFERENCE:
    [[fallthrough]];
  case K::INSTANTIATE_POINTER:
    [[fallthrough]];
  case K::INSTANTIATE_SLICE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case K::NONPOSITIONAL_PARAMETERS_BEGIN:
    [[fallthrough]];
  case K::LOCKED_PARAMETERS_BEGIN:
    [[fallthrough]];
  case K::NONAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // BRACES
  case K::INSTANTIATE_TUPLE:
    if (situation == S::RVALUE && !expression.getHasBranch()) {
      expression.changeKeyword(K::INSTANTIATE_LAYOUT);
      is_ok = true;
      break;
    }
    is_ok =
        this->situateNaryValueBranches(situation, expression, 0, S::ARGUMENT);
    break;
  case K::INSTANTIATE_LAYOUT:
    is_ok = this->situateNaryParameterBranches(situation, expression);
    break;
  case K::INSTANTIATE_TEMPLATE:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 1, S::RVALUE, S::ARGUMENT);
    break;

  // PROCEDURES
  case K::NAMED_ARGUMENT:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::NAME,
                                             S::RVALUE);
    break;
  case K::INSTANTIATE_SIGNATURE:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;
  case K::PLACEMENT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::COMPOSITION:
    is_ok = this->situateNaryValueBranches(situation, expression, 1, S::RVALUE);
    break;
  case K::DEFAULT_VALUE_PARAMETER:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::BINDING,
                                             S::RVALUE);
    break;
  case K::FORWARD_RANGER:
    [[fallthrough]];
  case K::BACKWARD_RANGER:
    is_ok = this->situateFirstAndSecondHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::RVALUE);
    break;
  case K::DESTRUCTOR:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::MAIN:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::FUNCTION:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::EXTENSION_METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::IMPLEMENT_FUNCTION:
    [[fallthrough]];
  case K::IMPLEMENT_METHOD:
    [[fallthrough]];
  case K::IMPLEMENT_EXTENSION_METHOD:
    is_ok = this->situateFirstAndSecondHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::RVALUE);
    break;
  case K::USE_FUNCTION:
    [[fallthrough]];
  case K::USE_METHOD:
    is_ok = this->situateTernaryValueBranches(situation, expression, S::NAME,
                                              S::RVALUE, S::RVALUE);
    break;

  // CONTROL FLOW
  case K::RETURN:
    if (!expression.getHasBranch()) {
      break;
    }
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BREAK:
    [[fallthrough]];
  case K::CONTINUE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::FALLTHROUGH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::GOTO:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::RANGE_OVER:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                            S::RVALUE);
    break;

  // DECLARED TYPES
  case K::CLASS: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::NAME, branch0)) {
      is_ok = false;
    }
    bool found_layout_header = false;
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (!branch.getIsStatement()) {
        if (found_layout_header) {
          this->getContext().logErrorUnexpectedHeaderExpression(branch);
          is_ok = false;
        } else {
          found_layout_header = true;
          if (!this->situateHeaderBranch(S::RVALUE, branch)) {
            is_ok = false;
          }
        }
      } else {
        if (!this->situateStatementBranch(branch)) {
          is_ok = false;
        }
      }
    }
    break;
  }
  case K::ENUMERATION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::NAME, branch0)) {
      is_ok = false;
    }
    if (!branch0.getHasNext()) {
      break;
    }
    rq::Expression &branch1 = branch0.getNext();
    if (branch1.getIsStatement()) {
      if (!this->situateStatementBranch(branch1)) {
        is_ok = false;
      }
    } else {
      if (!this->situateHeaderBranch(S::RVALUE, branch1)) {
        is_ok = false;
      }
    }
    for (rq::Expression &branch : branch1.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
    break;
  }
  case K::INTERFACE:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::NAME);
    break;
  case K::IMPLEMENT_INTERFACE:
    is_ok = this->situateFirstAndSecondHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::RVALUE);
    break;

  // VALUES
  case K::ARRAY:
    is_ok = this->situateNaryValueBranches(situation, expression, 0, S::RVALUE);
    break;
  case K::NULL_:
    [[fallthrough]];
  case K::TRUE:
    [[fallthrough]];
  case K::FALSE:
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
  case K::RESULT:
    [[fallthrough]];
  case K::COMMAND_LINE_ARGUMENTS:
    [[fallthrough]];
  case K::CALLSITE:
    [[fallthrough]];

  // BUILTIN TYPES
  case K::INFERENCE:
    [[fallthrough]];
  case K::EXPRESSION:
    [[fallthrough]];
  case K::VOID:
    [[fallthrough]];
  case K::NO_RETURN:
    [[fallthrough]];
  case K::BOOLEAN:
    [[fallthrough]];
  case K::HALF:
    [[fallthrough]];
  case K::SINGLE:
    [[fallthrough]];
  case K::DOUBLE:
    [[fallthrough]];
  case K::QUADRUPLE:
    [[fallthrough]];
  case K::BINARY16:
    [[fallthrough]];
  case K::BINARY32:
    [[fallthrough]];
  case K::BINARY64:
    [[fallthrough]];
  case K::BINARY128:
    [[fallthrough]];
  case K::BFLOAT16:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SIGNED_INTEGER:
    [[fallthrough]];
  case K::UNSIGNED_INTEGER:
    if (expression.getHasBranch()) {
      is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
      break;
    }
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FAST_SIGNED_INTEGER:
    [[fallthrough]];
  case K::FAST_UNSIGNED_INTEGER:
    [[fallthrough]];
  case K::LEAST_SIGNED_INTEGER:
    [[fallthrough]];
  case K::LEAST_UNSIGNED_INTEGER:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SIGNED_INDEX:
    [[fallthrough]];
  case K::UNSIGNED_INDEX:
    [[fallthrough]];
  case K::SIGNED_ADDRESS:
    [[fallthrough]];
  case K::UNSIGNED_ADDRESS:
    [[fallthrough]];
  case K::CHAR:
    [[fallthrough]];
  case K::ASCII:
    [[fallthrough]];
  case K::UTF8:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FIRST_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NEXT_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::VARIADIC_ARGUMENTS:
    is_ok = this->situateNaryValueBranches(situation, expression, 1, S::RVALUE);
    break;

  // SCOPES
  case K::IF:
    [[fallthrough]];
  case K::ELSE_IF:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::ELSE:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::MATCH:
    [[fallthrough]];
  case K::SWITCH:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::CASE:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::WITH:
    is_ok = this->situateFirstAndSecondHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case K::DEFAULT:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::FOR:
    is_ok = this->situateNaryDifferentFirstHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case K::WHILE:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::SPIN:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::WEAVE:
    is_ok = this->situateFirstAndSecondHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case K::SCOPE:
    [[fallthrough]];
  case K::BLOCK:
    is_ok = this->situateNaryStatementBranches(expression);
    break;

  // RANGES
  case K::RANGE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ARITHMETIC_SEQUENCE: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &value = expression.getBranch();
    is_ok = this->situateValueBranch(S::RVALUE, value);
    if (!value.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &stage_one = value.getNext();
    if (!this->situateValueBranch(S::ARITHMETIC_SEQUENCE_STAGE, stage_one)) {
      is_ok = false;
    }
    if (stage_one.getHasNext()) {
      rq::Expression &stage_two = stage_one.getNext();
      if (!this->situateValueBranch(S::ARITHMETIC_SEQUENCE_STAGE, stage_two)) {
        is_ok = false;
      }
      if (stage_two.getHasNext()) {
        this->getContext().logErrorTooManyBranchCount(situation, expression, 3);
        is_ok = false;
        break;
      }
    }
    break;
  }
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    [[fallthrough]];
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // TABLE GRAPH
  case K::IMPORT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NAMESPACE: {
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::NAMESPACE);
    if (!is_ok) {
      break;
    }
    rq::Expression &path = expression.getBranch();
    if (path.getKeyword() == K::REFLECT) {
      rq::Expression &branch = path.popBranch();
      rq::Expression *body_ptr = path.popNextPtr();
      this->getContext().discardExpression(expression.replaceBranch(branch));
      rq::Expression &branch_next = branch.popNext();
      rq::Expression &nested_namespace = this->getContext().acquireExpression();
      nested_namespace.setIsInserted();
      nested_namespace.setSource(expression);
      nested_namespace.setKeyword(K::NAMESPACE);
      nested_namespace.setBranch(branch_next);
      rq::Expression *previous_path_ptr = &branch_next;
      while (previous_path_ptr != nullptr) {
        rq::Expression &previous_path = rq::dereferencePtr(previous_path_ptr);
        if (!previous_path.getHasNext()) {
          previous_path.setNext(body_ptr);
          break;
        }
        rq::Expression &path_next = branch.popNext();
        rq::Expression &next_namespace = this->getContext().acquireExpression();
        next_namespace.setIsInserted();
        next_namespace.setSource(expression);
        next_namespace.setKeyword(K::NAMESPACE);
        next_namespace.setBranch(path_next);
        previous_path_ptr = &path_next;
      }
      branch.setNext(nested_namespace);
    }
    break;
  }
  case K::TOP:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::LABEL:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::NAME);
    break;

  // HINTS
  case K::DEBUG_BREAK:
    [[fallthrough]];
  case K::ABORT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ASSERT:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::UNREACHABLE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ASSUME:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // EXPRESSION ATTRIBUTES
  case K::NO_ANCHOR:
    [[fallthrough]];
  case K::ANCHOR:
    [[fallthrough]];
  case K::NO_OPAQUE:
    [[fallthrough]];
  case K::OPAQUE:
    [[fallthrough]];
  case K::NO_FLANK:
    [[fallthrough]];
  case K::FLANK:
    [[fallthrough]];
  case K::NO_GLOBAL:
    [[fallthrough]];
  case K::GLOBAL:
    [[fallthrough]];
  case K::NO_ACCESS:
    [[fallthrough]];
  case K::EXPORT:
    [[fallthrough]];
  case K::PUBLIC:
    [[fallthrough]];
  case K::NO_PARTIAL_MUTATE:
    [[fallthrough]];
  case K::PARTIAL_MUTATE:
    [[fallthrough]];
  case K::NO_STATIC:
    [[fallthrough]];
  case K::STATIC:
    [[fallthrough]];
  case K::NO_CAPTURE:
    [[fallthrough]];
  case K::CAPTURE:
    [[fallthrough]];
  case K::NO_INLINE:
    [[fallthrough]];
  case K::INLINE:
    [[fallthrough]];
  case K::NO_MANGLE:
    [[fallthrough]];
  case K::MANGLE:
    [[fallthrough]];
  case K::NO_PACK:
    [[fallthrough]];
  case K::PACK:
    [[fallthrough]];
  case K::NO_BRANCH_TREND:
    [[fallthrough]];
  case K::LIKELY:
    [[fallthrough]];
  case K::UNLIKELY:
    [[fallthrough]];
  case K::NO_DEPRECIATE:
    [[fallthrough]];
  case K::DEPRECIATE:
    [[fallthrough]];
  case K::EXPERIMENTAL:
    [[fallthrough]];
  case K::NO_STABLE_ADDRESS:
    [[fallthrough]];
  case K::STABLE_ADDRESS:
    [[fallthrough]];
  case K::NO_VARIADIC:
    [[fallthrough]];
  case K::VARIADIC:
    [[fallthrough]];
  case K::NO_LOCATION:
    [[fallthrough]];
  case K::LOCATION:
    [[fallthrough]];
  case K::NO_TEMPLATE:
    [[fallthrough]];
  case K::TEMPLATE:
    [[fallthrough]];
  case K::NO_CONSTRAINT:
    [[fallthrough]];
  case K::CONSTRAINT:
    [[fallthrough]];
  case K::NO_WEIGHT:
    [[fallthrough]];
  case K::WEIGHT:
    [[fallthrough]];
  case K::NO_REQUIRE:
    [[fallthrough]];
  case K::REQUIRE:
    [[fallthrough]];
  case K::NO_ENSURE:
    [[fallthrough]];
  case K::ENSURE:
    [[fallthrough]];

  // TYPE ATTRIBUTES
  case K::NO_VAR:
    [[fallthrough]];
  case K::VAR:
    [[fallthrough]];
  case K::PARTIAL_VAR:
    [[fallthrough]];
  case K::NO_VOLATILE:
    [[fallthrough]];
  case K::VOLATILE:
    [[fallthrough]];
  case K::NO_ATOMIC:
    [[fallthrough]];
  case K::ATOMIC:
    [[fallthrough]];
  case K::NO_NULL_TERMINATE:
    [[fallthrough]];
  case K::NULL_TERMINATE:
    [[fallthrough]];

  // EXPRESSION ATTRIBUTE TYPES
  case K::ANCHOR_TYPE:
    [[fallthrough]];
  case K::OPAQUE_TYPE:
    [[fallthrough]];
  case K::FLANK_TYPE:
    [[fallthrough]];
  case K::GLOBAL_TYPE:
    [[fallthrough]];
  case K::ACCESS_TYPE:
    [[fallthrough]];
  case K::PARTIAL_MUTATE_TYPE:
    [[fallthrough]];
  case K::STATIC_TYPE:
    [[fallthrough]];
  case K::CAPTURE_TYPE:
    [[fallthrough]];
  case K::INLINE_TYPE:
    [[fallthrough]];
  case K::MANGLE_TYPE:
    [[fallthrough]];
  case K::PACK_TYPE:
    [[fallthrough]];
  case K::BRANCH_TREND_TYPE:
    [[fallthrough]];
  case K::DEPRECIATE_TYPE:
    [[fallthrough]];
  case K::STABLE_ADDRESS_TYPE:
    [[fallthrough]];
  case K::VARIADIC_TYPE:
    [[fallthrough]];
  case K::LOCATION_TYPE:
    [[fallthrough]];
  case K::TEMPLATE_TYPE:
    [[fallthrough]];
  case K::CONSTRAINT_TYPE:
    [[fallthrough]];
  case K::WEIGHT_TYPE:
    [[fallthrough]];
  case K::REQUIRE_TYPE:
    [[fallthrough]];
  case K::ENSURE_TYPE:
    [[fallthrough]];

    // TYPE ATTRIBUTE TYPES
  case K::VAR_TYPE:
    [[fallthrough]];
  case K::VOLATILE_TYPE:
    [[fallthrough]];
  case K::ATOMIC_TYPE:
    [[fallthrough]];
  case K::NULL_TERMINATE_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

    // REFLECTIONS
  case K::REFLECT: {
    if (situation == S::NAMESPACE) {
      is_ok = this->situateNaryValueBranches(situation, expression, 2,
                                             S::NAMESPACE);
      break;
    }
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &value = expression.popBranch();
    const rq::Situation first_situation =
        situation == S::REFLECTION ? S::REFLECTION : S::RVALUE;
    if (!this->situateValueBranch(first_situation, value)) {
      is_ok = false;
    }
    rq::Expression *inner_ptr = &value;
    rq::Expression *next_ptr = value.popNextPtr();
    if (next_ptr == nullptr) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    while (next_ptr != nullptr) {
      rq::Expression &inner = rq::dereferencePtr(inner_ptr);
      rq::Expression &next = rq::dereferencePtr(next_ptr);
      next_ptr = next.popNextPtr();
      if (!this->situateValueBranch(S::REFLECTION, next)) {
        is_ok = false;
        continue;
      }
      if (next.getKeyword() == rq::Keyword::IDENTIFIER_LITERAL) {
        rq::Expression &member = this->getContext().acquireExpression();
        member.setIsInserted();
        member.setSource(inner, next);
        member.setKeyword(rq::Keyword::MEMBER_OF);
        member.setBranch(inner);
        inner.setNext(next);
        inner_ptr = &member;
        continue;
      } else if (next.getKeyword() == rq::Keyword::ASCRIBE_TYPE) {
        next.changeKeyword(rq::Keyword::ASCRIBE_ROOT_OF_VALUE);
        rq::Expression &next_branch = next.getBranch();
        if (!next_branch.getIsUniversalizable()) {
          continue;
        }
        const rq::Keyword universalized = next_branch.getUniversalized();
        next_branch.changeKeyword(universalized);
        inner.setNext(next_branch.replaceBranchPtr(inner));
        inner_ptr = &next;
        continue;
      }
      if (!next.getIsUniversalizable()) {
        continue;
      }
      const rq::Keyword universalized = next.getUniversalized();
      next.changeKeyword(universalized);
      inner.setNext(next.replaceBranchPtr(inner));
      inner_ptr = &next;
    }
    expression.setBranch(inner_ptr);
    this->getContext().discardExpression(expression.mergeAndPopBranch());
  } break;
  case K::MEMBER_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::MEMBER_OF_TOP:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IGNORE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IGNORE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BYTE_SIZE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BYTE_SIZE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BIT_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BIT_DEPTH_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ELEMENT_COUNT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ELEMENT_COUNT_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SNIPPET:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SNIPPET_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::NAME_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::LINE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::COLUMN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::COLUMN_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HOLDS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::HOLDS_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SYMBOL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SYMBOL_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS_MEMBER:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::HAS_MEMBER_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::GET:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::GET_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression, S::RVALUE,
                                             S::RVALUE);
    break;
  case K::SIGNATURE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SIGNATURE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SYNONYM:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SYNONYM_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::CAPTURE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::AS_EXTENSION:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::AS_EXTENSION_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::REVERSE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::REVERSE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_RANGE_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_RANGE_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_PLACEMENT_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_PLACEMENT_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_SIGNED_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_SIGNED_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_UNSIGNED_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_UNSIGNED_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_INTEGER_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_INTEGER_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_FLOAT_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_FLOAT_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_BINARY_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_BINARY_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_BFLOAT_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_BFLOAT_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_STRING_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_STRING_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_CODEUNIT_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_CODEUNIT_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_TYPE_ATTRIBUTE_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_TYPE_ATTRIBUTE_TYPE_OF:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  case K::LAST:
    RQ_UNREACHABLE();

    // NOTE: don't add default case so compiler will warn if cases are missing!
  }
  if (!is_ok) {
    expression.setHasSituatorError();
  }
  if (is_ok && expression.getIsConverging()) {
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (expression.getKeyword() == branch.getKeyword()) {
        this->getContext().discardExpression(expression.mergeAndPopBranch());
      }
    }
  }
  return is_ok;
}

bool Situator::situateValueBranch(rq::Situation branch_situation,
                                  rq::Expression &branch) {
  if (branch.getIsStatement()) {
    this->getContext().logErrorExpectedHeaderExpression(branch);
    return false;
  }
  if (branch.getIsChainLink()) {
    this->getContext().logErrorUnexpectedChainLinkExpression(branch);
    return false;
  }
  if (!branch.getCanBeSituation(branch_situation)) {
    this->getContext().logErrorInvalidBranchSituation(branch_situation, branch);
    return false;
  }
  return this->situateTree(branch_situation, branch);
}

bool Situator::situateHeaderBranch(rq::Situation branch_situation,
                                   rq::Expression &branch) {
  if (branch.getIsStatement()) {
    this->getContext().logErrorExpectedHeaderExpression(branch);
    return false;
  }
  if (branch.getIsChainLink()) {
    this->getContext().logErrorUnexpectedChainLinkExpression(branch);
    return false;
  }
  if (!branch.getCanBeSituation(branch_situation)) {
    this->getContext().logErrorInvalidBranchSituation(branch_situation, branch);
    return false;
  }
  return this->situateTree(branch_situation, branch);
}

bool Situator::situateStatementBranch(rq::Expression &branch) {
  if (!branch.getIsChainLink() && !branch.getIsStatement()) {
    this->getContext().logErrorUnexpectedHeaderExpression(branch);
    return false;
  }
  if (!branch.getCanBeStatement()) {
    this->getContext().logErrorInvalidBranchSituation(rq::Situation::STATEMENT,
                                                      branch);
    return false;
  }
  return this->situateTree(rq::Situation::STATEMENT, branch);
}

bool Situator::situateNullaryExpression(rq::Situation situation,
                                        rq::Expression &expression) {
  if (expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 0);
    return false;
  }
  return true;
}

bool Situator::situateNullaryOrUnaryValueBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    return true;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateValueBranch(branch0_situation, branch0);
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateUnaryValueBranches(rq::Situation situation,
                                         rq::Expression &expression,
                                         rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateValueBranch(branch0_situation, branch0);
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateBinaryValueBranches(rq::Situation situation,
                                          rq::Expression &expression,
                                          rq::Situation branch0_situation,
                                          rq::Situation branch1_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateValueBranch(branch0_situation, branch0);
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateValueBranch(branch1_situation, branch1)) {
    is_ok = false;
  }
  if (branch1.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    is_ok = false;
  }
  return is_ok;
}

[[nodiscard]] bool Situator::situateTernaryValueBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation,
    rq::Situation branch2_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 3);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateValueBranch(branch0_situation, branch0);
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 3);
    return false;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateValueBranch(branch1_situation, branch1)) {
    is_ok = false;
  }
  if (!branch1.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 3);
    return false;
  }
  rq::Expression &branch2 = branch1.getNext();
  if (!this->situateValueBranch(branch2_situation, branch2)) {
    is_ok = false;
  }
  if (branch2.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 3);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryValueBranches(rq::Situation situation,
                                        rq::Expression &expression,
                                        unsigned minimum_branch_count,
                                        rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!this->situateValueBranch(branchn_situation, branch)) {
      is_ok = false;
    }
    branch_i++;
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstValueBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateValueBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    branch_i++;
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (!this->situateValueBranch(branchn_situation, branch)) {
        is_ok = false;
      }
      branch_i++;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentLastValueBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation,
    rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    branch_i++;
    if (!branch.getHasNext()) {
      if (!this->situateValueBranch(last_situation, branch)) {
        is_ok = false;
      }
      break;
    }
    if (!this->situateValueBranch(branchn_situation, branch)) {
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

[[nodiscard]] bool Situator::situateUnsituatedAscribeExpression(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Keyword situated_keyword,
    rq::Situation branchn_situation, rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  rq::Expression *last_ptr = nullptr;
  rq::Expression *previous_last_ptr = nullptr;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    branch_i++;
    if (!branch.getHasNext()) {
      if (!this->situateValueBranch(last_situation, branch)) {
        is_ok = false;
      }
      if (is_ok) {
        previous_last_ptr = last_ptr;
        last_ptr = &branch;
        expression.changeKeyword(situated_keyword);
      }
      break;
    }
    last_ptr = &branch;
    if (!this->situateValueBranch(branchn_situation, branch)) {
      is_ok = false;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                     minimum_branch_count);
    is_ok = false;
  }
  if (is_ok && previous_last_ptr != nullptr) {
    rq::Expression &previous_last = rq::dereferencePtr(previous_last_ptr);
    rq::Expression &last = previous_last.popNext();
    last.setNext(expression.replaceBranch(last));
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstAndLastValueBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation, rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateValueBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (!branch.getHasNext()) {
        if (!this->situateValueBranch(last_situation, branch)) {
          is_ok = false;
        }
        break;
      }
      if (!this->situateValueBranch(branchn_situation, branch)) {
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
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateValueBranch(branch0_situation, branch0)) {
    return false;
  }
  if (!branch0.getHasNext()) {
    return true;
  }
  return this->situateNaryFromFirstParameterBranches(situation, expression,
                                                     branch0.getNext());
}

bool Situator::situateNaryParameterBranches(rq::Situation situation,
                                            rq::Expression &expression) {
  if (!expression.getHasBranch()) {
    return true;
  }
  return this->situateNaryFromFirstParameterBranches(situation, expression,
                                                     expression.getBranch());
}

bool Situator::situateNaryFromFirstParameterBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Expression &first_parameter) {
  bool is_ok = true;
  bool found_nonpositional = false;
  bool found_positional = false;
  bool found_locked = false;
  for (rq::Expression &parameter : first_parameter.getInclusiveNextSubrange()) {
    if (!this->situateValueBranch(rq::Situation::PARAMETER, parameter)) {
      is_ok = false;
    }
    switch (parameter.getKeyword()) {
    case rq::Keyword::NONPOSITIONAL_PARAMETERS_BEGIN:
      if (!parameter.getHasNext()) {
        is_ok = false;
        this->getContext().logErrorIsLast(parameter);
      }
      if (found_nonpositional) {
        this->getContext().logErrorDuplicateParameterMark(parameter);
        is_ok = false;
      } else {
        found_nonpositional = true;
      }
      if (found_positional) {
        is_ok = false;
        this->getContext().logErrorNonpositionalBeginAfterPositionalEnd(
            parameter);
      }
      if (found_locked) {
        is_ok = false;
        this->getContext().logErrorNonpositionalBeginAfterLockedBegin(
            parameter);
      }
      break;
    case rq::Keyword::POSITIONAL_PARAMETERS_END:
      if (parameter == first_parameter) {
        this->getContext().logErrorIsFirst(parameter);
      }
      if (found_positional) {
        is_ok = false;
        this->getContext().logErrorDuplicateParameterMark(parameter);
      } else {
        found_positional = true;
      }
      if (found_locked) {
        is_ok = false;
        this->getContext().logErrorPositionalEndAfterLockedBegin(parameter);
      }
      break;
    case rq::Keyword::LOCKED_PARAMETERS_BEGIN:
      if (!parameter.getHasNext()) {
        is_ok = false;
        this->getContext().logErrorIsLast(parameter);
      }
      if (found_locked) {
        this->getContext().logErrorDuplicateParameterMark(parameter);
        is_ok = false;
      } else {
        found_locked = true;
      }
      break;
    default:
      break;
    }
  }
  if (!found_nonpositional && !found_positional && !found_locked) {
    this->getContext().logErrorMustHaveParameterMark(situation, expression);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryStatementBranches(rq::Expression &expression) {
  bool is_ok = true;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!this->situateStatementBranch(branch)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Situator::situateFirstHeaderNaryStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  bool is_ok = true;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
  } else {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateFirstAndSecondHeaderNaryStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation) {
  bool is_ok = true;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 2);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateHeaderBranch(branch0_situation, branch0)) {
    is_ok = false;
  }
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 2);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateHeaderBranch(branch1_situation, branch1)) {
    is_ok = false;
  }
  for (rq::Expression &branch : branch1.getNextSubrange()) {
    if (!this->situateStatementBranch(branch)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstHeaderNaryStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation header0_situation, rq::Situation headern_situation) {
  bool is_ok = true;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateHeaderBranch(header0_situation, branch0)) {
    is_ok = false;
  }
  bool found_statement = false;
  for (rq::Expression &branch : branch0.getNextSubrange()) {
    if (!branch.getIsStatement()) {
      if (found_statement) {
        this->getContext().logErrorUnexpectedHeaderExpression(branch);
        is_ok = false;
      } else {
        if (!this->situateHeaderBranch(headern_situation, branch)) {
          is_ok = false;
        }
      }
    } else {
      found_statement = true;
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
  }
  return is_ok;
}

bool Situator::situateNamedMemberProcedure(rq::Situation situation,
                                           rq::Expression &expression) {
  bool is_ok = true;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 2);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateHeaderBranch(rq::Situation::NAME, branch0)) {
    is_ok = false;
  }
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 2);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateHeaderBranch(rq::Situation::RVALUE, branch1)) {
    is_ok = false;
  }
  for (rq::Expression &branch : branch1.getNextSubrange()) {
    if (!this->situateStatementBranch(branch)) {
      is_ok = false;
    }
  }
  return is_ok;
}

} // namespace rq
