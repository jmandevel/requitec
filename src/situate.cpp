#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallVector.h>

#include <utility>

namespace rq {

rq::Expression &Situator::makeModuleTrunk(rq::Module &module) {
  rq::Expression &trunk = this->getContext().acquireExpression();
  trunk.setIsInserted();
  trunk.setSource(module);
  trunk.setKeyword(rq::Keyword::MODULE_TRUNK);
  return trunk;
}

bool Situator::situateModule(rq::Module &module) {
  if (!module.getHasExpression()) {
    rq::Expression &trunk = this->makeModuleTrunk(module);
    module.setExpression(trunk);
    return true;
  }
  rq::Expression &first = module.getExpression();
  if (first.getKeyword() != rq::Keyword::MODULE_TRUNK) {
    rq::Expression &trunk = this->makeModuleTrunk(module);
    trunk.setBranch(module.replaceExpression(trunk));
  }
  rq::Expression &trunk = module.getExpression();
  return this->situateTree(rq::Situation::TRUNK, trunk);
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
  case K::LEFT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::MIDDLE_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case K::RIGHT_INTERPOLATION_LITERAL:
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
  case K::UNSITUATED_PARENTHESIS_GROUP:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, situation);
    if (is_ok) {
      this->getContext().discardExpression(expression.mergeAndPopBranch());
    }
    break;
  case K::UNSITUATED_EQUAL_OPERATOR:
    switch (situation) {
    case S::ARGUMENT: {
      is_ok = this->situateBinaryValueBranches(situation, expression,
                                                    S::LVALUE, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::NAMED_ARGUMENT);
      }
    } break;
    case S::PARAMETER: {
      is_ok = this->situateBinaryValueBranches(situation, expression,
                                                    S::BINDING, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::DEFAULT_VALUE_PARAMETER);
      }
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
      switch (lvalue.getKeyword()) {
      case K::NULL_:
        expression.changeKeyword(K::IGNORE);
        std::ignore = lvalue.popNext();
        this->getContext().discardExpression(expression.replaceBranch(rvalue));
        break;
      case K::TUPLE:
        lvalue.changeKeyword(K::STRUCTURED_BINDING);
        this->getContext().discardExpression(expression.mergeAndPopBranch());
        break;
      default:
        expression.changeKeyword(K::ASSIGN);
        break;
      }
      break;
    }
    default:
      break;
    }
    break;
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
    [[fallthrough]];
  case K::UNSITUATED_ASCRIBE_TYPE: {
    unsigned branch_i = 0;
    const rq::Situation attribute_situation =
        expression.getAttributeSituation();
    rq::Expression *previous_ptr = nullptr;
    rq::Expression *next_ptr = expression.getBranchPtr();
    if (next_ptr == nullptr) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    std::uint32_t found_flags = 0;
    while (next_ptr != nullptr) {
      rq::Expression &branch = rq::dereferencePtr(next_ptr);
      if (!branch.getHasNext()) {
        if (!this->situateValueBranch(situation, branch)) {
          is_ok = false;
          break;
        }
        if (branch_i < 1) {
          this->getContext().logErrorNotAtLeastBranchCount(situation,
                                                           expression, 2);
          is_ok = false;
        }
        break;
      }
      next_ptr = branch.getNextPtr();
      branch_i++;
      if (!this->situateValueBranch(attribute_situation, branch)) {
        is_ok = false;
        previous_ptr = &branch;
        continue;
      }
      bool has_flags = false;
      if (attribute_situation == S::EXPRESSION_ATTRIBUTE) {
        rq::ExpressionFlags flags =
            rq::getFlags(branch.getExpressionAttribute());
        if (rq::getHasAll(static_cast<rq::ExpressionFlags>(found_flags),
                          flags)) {
          has_flags = true;
        }
        found_flags = static_cast<std::uint32_t>(
            static_cast<rq::ExpressionFlags>(found_flags) | flags);
      } else if (attribute_situation == S::TYPE_ATTRIBUTE) {
        rq::TypeFlags flags = rq::getFlags(branch.getTypeAttribute());
        if (rq::getHasAll(static_cast<rq::TypeFlags>(found_flags), flags)) {
          has_flags = true;
        }
        found_flags = static_cast<std::uint32_t>(
            static_cast<rq::TypeFlags>(found_flags) | flags);
      }
      if (has_flags) {
        this->getContext().logErrorDuplicateAttribute(branch);
        is_ok = false;
      }
      previous_ptr = &branch;
    }
    if (!is_ok) {
      break;
    }
    rq::Expression &previous_last = rq::dereferencePtr(previous_ptr);
    rq::Expression &last = previous_last.popNext();
    last.setNext(expression.replaceBranch(last));
    expression.changeKeyword(expression.getSituatedAscribe());
  } break;

  // LOGICAL
  case K::LOGICAL_AND:
    [[fallthrough]];
  case K::LOGICAL_OR:
    is_ok = this->situateNaryValueBranches(situation, expression, 2,
                                                S::RVALUE);
    break;
  case K::LOGICAL_COMPLEMENT:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
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
    is_ok = this->situateNaryValueBranches(situation, expression, 2,
                                                S::RVALUE);
    break;

  // APPLY
  case K::EXTEND:
    [[fallthrough]];
  case K::EXTENSION:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::BINDING:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::LVALUE, S::RVALUE);
    break;
  case K::ASCRIBE_TYPE:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    break;
  case K::ASCRIBE_EXPRESSION:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation, S::EXPRESSION_ATTRIBUTE);
    break;
  case K::ASCRIBE_ROOT_OF_VALUE:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    break;
  case K::IDENTIFY:
    [[fallthrough]];
  case K::KEYWORDIFY:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
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
    is_ok = this->situateNaryValueBranches(situation, expression, 2,
                                                S::RVALUE);
    break;
  case K::NEGATE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // CASTS
  case K::AS:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LITERAL_AS:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ELEMENTS_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::DOWN_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::DOWN_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::FLOAT_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::FLOAT_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::PLATFORM_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::PLATFORM_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::STATIC_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::STATIC_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::BITWISE_CAST:
    is_ok = this->situateNullaryOrUnaryValueBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::BITWISE_CAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;

  // BITWISE
  case K::BITWISE_OR:
    [[fallthrough]];
  case K::BITWISE_AND:
    [[fallthrough]];
  case K::BITWISE_XOR:
    is_ok = this->situateNaryValueBranches(situation, expression, 2,
                                                S::RVALUE);
    break;
  case K::BITWISE_COMPLEMENT:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BITWISE_SHIFT_LEFT:
    [[fallthrough]];
  case K::BITWISE_SHIFT_RIGHT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_LEFT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_RIGHT:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;

  // MEMORY
  case K::CONTENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::CONTENT_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ADDRESS_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BORROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BORROW_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, situation);
    break;
  case K::DATA_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DATA_ADDRESS_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, situation);
    break;

  // ASSIGNMENT
  case K::ASSIGN:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::LVALUE, S::RVALUE);
    break;

  // SUBTYPE
  case K::ARRAY:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::REFERENCE:
    [[fallthrough]];
  case K::POINTER:
    [[fallthrough]];
  case K::FAT_POINTER:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // ARITHMETIC INTERVAL
  case K::ARITHMETIC_INTERVAL_LESS:
    [[fallthrough]];
  case K::ARITHMETIC_INTERVAL_LESS_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_INTERVAL_GREATER:
    [[fallthrough]];
  case K::ARITHMETIC_INTERVAL_GREATER_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_INTERVAL_EQUAL:
    [[fallthrough]];
  case K::ARITHMETIC_INTERVAL_NOT_EQUAL:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // FINITE ARITHMETIC PROGRESSION TYPES
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    [[fallthrough]];
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case K::INFINITE_ARITHMETIC_PROGRESSION_ADD:
    [[fallthrough]];
  case K::INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    [[fallthrough]];
  case K::INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    [[fallthrough]];
  case K::INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    [[fallthrough]];
  case K::INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // TYPE ATTRIBUTES
  case K::MUTABLE:
    [[fallthrough]];
  case K::CONSTANT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::PARTIALLY_MUTABLE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::VOLATILE:
    [[fallthrough]];
  case K::ATOMIC:
    [[fallthrough]];
  case K::NULL_TERMINATED:
    [[fallthrough]];
  case K::MAY_DISCARD:
    [[fallthrough]];
  case K::INDETERMINATE:
    [[fallthrough]];
  case K::RANGING:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case K::NAMED_PARAMETERS_BEGIN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // BRACES
  case K::TUPLE:
    if (!expression.getHasBranch()) {
      expression.changeKeyword(K::NULL_);
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    }
    is_ok = this->situateNaryValueBranches(situation, expression, 0,
                                                S::ARGUMENT);
    break;
  case K::LAYOUT_TYPE:
    is_ok = this->situateNaryParameterBranches(situation, expression);
    break;
  case K::NULL_:
    [[fallthrough]];
  case K::NULL_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::STRUCTURED_BINDING:
    is_ok = this->situateNaryDifferentLastValueBranches(
        situation, expression, 2, S::LVALUE, S::RVALUE);
    break;
  case K::IGNORE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SPECIALIZATION:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;

  // PROCEDURES
  case K::CALL:
    is_ok = this->situateNaryDifferentFirstValueBranches(
        situation, expression, 1, S::RVALUE, S::ARGUMENT);
    break;
  case K::NAMED_ARGUMENT:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::NAME, S::RVALUE);
    break;
  case K::INDEX_INTO:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::SIGNATURE_TYPE:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;
    break;
  case K::DEFAULT_VALUE_PARAMETER:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::BINDING, S::RVALUE);
    break;
  case K::DROP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DROP_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::DROP_EACH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DROP_EACH_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::EACH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::EACH_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::MOVE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MOVE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, situation);
    break;
  case K::ENTRY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::FUNCTION:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::RANGER:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::EXTENSION_FUNCTION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::NAME, branch0)) {
      is_ok = false;
    }
    if (!branch0.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &branch1 = branch0.getNext();
    if (!this->situateHeaderBranch(S::RVALUE, branch1)) {
      is_ok = false;
    }
    if (!branch1.getHasNext()) {
      break;
    }
    rq::Expression &branch2 = branch1.getNext();
    if (branch2.getIsHeader()) {
      if (!this->situateHeaderBranch(S::RVALUE, branch2)) {
        is_ok = false;
      }
    } else {
      if (!this->situateStatementBranch(branch2)) {
        is_ok = false;
      }
    }
    for (rq::Expression &branch : branch2.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
    break;
  }
  case K::EXTENSION_METHOD:
    [[fallthrough]];
  case K::EXTENSION_RANGER:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::IMPLEMENT_FUNCTION:
    [[fallthrough]];
  case K::IMPLEMENT_METHOD:
    [[fallthrough]];
  case K::IMPLEMENT_RANGER:
    [[fallthrough]];
  case K::IMPLEMENT_EXTENSION_FUNCTION:
    [[fallthrough]];
  case K::IMPLEMENT_EXTENSION_METHOD:
    [[fallthrough]];
  case K::IMPLEMENT_EXTENSION_RANGER:
    [[fallthrough]];
  case K::SPECIALIZE_FUNCTION:
    [[fallthrough]];
  case K::SPECIALIZE_METHOD:
    [[fallthrough]];
  case K::SPECIALIZE_RANGER:
    [[fallthrough]];
  case K::SPECIALIZE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case K::SPECIALIZE_EXTENSION_METHOD:
    [[fallthrough]];
  case K::SPECIALIZE_EXTENSION_RANGER:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // CONTROL FLOW
  case K::RETURN:
    if (!expression.getHasBranch()) {
      break;
    }
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BREAK:
    [[fallthrough]];
  case K::CONTINUE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::FALLTHROUGH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::GOTO:
    [[fallthrough]];
  case K::RANGE_OVER:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
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
    bool found_rvalue_header = false;
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (branch.getIsHeader()) {
        if (found_rvalue_header) {
          this->getContext().logErrorUnexpectedHeaderExpression(branch);
          is_ok = false;
        } else {
          found_rvalue_header = true;
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
    if (branch1.getIsHeader()) {
      if (!this->situateHeaderBranch(S::RVALUE, branch1)) {
        is_ok = false;
      }
    } else {
      if (!this->situateStatementBranch(branch1)) {
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
  case K::CODE:
    if (situation == S::STATEMENT) {
      is_ok =
          this->situateUnaryValueBranches(situation, expression, S::NAME);
      break;
    }
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::CATEGORY: {
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
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
    break;
  }

  // VALUES
  case K::INITIALIZER_LIST:
    is_ok = this->situateNaryValueBranches(situation, expression, 0,
                                                S::RVALUE);
    break;
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

  // BUILTIN TYPES
  case K::INFERENCE:
    [[fallthrough]];
  case K::VOID:
    [[fallthrough]];
  case K::NO_RETURN:
    [[fallthrough]];
  case K::BOOLEAN:
    [[fallthrough]];
  case K::FLOAT:
    [[fallthrough]];
  case K::HALF:
    [[fallthrough]];
  case K::SINGLE:
    [[fallthrough]];
  case K::DOUBLE:
    [[fallthrough]];
  case K::QUADRUPLE:
    [[fallthrough]];
  case K::BINARY:
    [[fallthrough]];
  case K::BFLOAT:
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
    [[fallthrough]];
  case K::INTEGER:
    [[fallthrough]];
  case K::SIGNED:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SIGNED_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::UNSIGNED:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::UNSIGNED_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::FAST:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::FAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::LEAST:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LEAST_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::EXACT:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::EXACT_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::INDEX_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::INDEX_DEPTH_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ADDRESS_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ADDRESS_DEPTH_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::STRING:
    [[fallthrough]];
  case K::CODEUNIT:
    [[fallthrough]];
  case K::CHAR:
    [[fallthrough]];
  case K::ASCII:
    [[fallthrough]];
  case K::UTF8:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FIRST_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NEXT_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::VARIADIC_LIST:
    is_ok = this->situateNaryValueBranches(situation, expression, 1,
                                                S::RVALUE);
    break;

  // SCOPES
  case K::IF:
    [[fallthrough]];
  case K::ELSE_IF:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::ELSE:
    is_ok = this->situateNaryValueBranches(situation, expression, 0,
                                                situation);
    break;
  case K::MATCH:
    [[fallthrough]];
  case K::INLINE_MATCH:
    [[fallthrough]];
  case K::SWITCH:
    [[fallthrough]];
  case K::INLINE_SWITCH:
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
  case K::SCOPE:
    [[fallthrough]];
  case K::INLINE_SCOPE:
    [[fallthrough]];
  case K::BLOCK:
    [[fallthrough]];
  case K::INLINE_BLOCK:
    is_ok = this->situateNaryValueBranches(situation, expression, 0,
                                                situation);
    break;
  case K::PASS:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::STATEMENT);
    break;
  case K::PASS_OF:
    is_ok = this->situateNaryDifferentFirstHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case K::FAIL:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::STATEMENT);
    break;
  case K::FAIL_OF:
    is_ok = this->situateNaryDifferentFirstHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case K::HANDLE:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::STATEMENT);
    break;
  case K::HANDLE_OF:
    is_ok = this->situateNaryDifferentFirstHeaderNaryStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;

  // RANGES
  case K::RANGE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
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
    if (!this->situateValueBranch(S::ARITHMETIC_SEQUENCE_STAGE,
                                       stage_one)) {
      is_ok = false;
    }
    if (stage_one.getHasNext()) {
      rq::Expression &stage_two = stage_one.getNext();
      if (!this->situateValueBranch(S::ARITHMETIC_SEQUENCE_STAGE,
                                         stage_two)) {
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
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // ACCESS MODIFIERS
  case K::PUBLIC:
    [[fallthrough]];
  case K::PROTECTED:
    [[fallthrough]];
  case K::EXPORT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // TABLE GRAPH
  case K::IMPORT:
    [[fallthrough]];
  case K::USE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NAMESPACE:
    is_ok = this->situateFirstHeaderNaryStatementBranches(situation, expression,
                                                          S::NAME);
    break;
  case K::C:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MODULE_TRUNK:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::NO_NAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // HINTS
  case K::DEBUG_TRAP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::UNREACHABLE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ASSUME:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // SYMBOL ATTRIBUTES
  case K::OPAQUE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::OUTSIDE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::NAME);
    break;
  case K::PARTIAL_MUTATE:
    [[fallthrough]];
  case K::STATIC:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::CAPTURE:
    if (situation == S::REFLECTION || situation == S::RVALUE) {
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    }
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::CAPTURE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::EAGER:
    [[fallthrough]];
  case K::MAY_PARENT:
    [[fallthrough]];
  case K::PARENT:
    [[fallthrough]];
  case K::ABSTRACT:
    [[fallthrough]];
  case K::VIRTUAL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::OVERRIDE:
    [[fallthrough]];
  case K::LOCATION:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::INLINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MANGLE:
    switch (situation) {
    case S::EXPRESSION_ATTRIBUTE:
      is_ok =
          this->situateUnaryValueBranches(situation, expression, S::NAME);
      break;
    case S::REFLECTION:
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    default:
      break;
    }
    break;
  case K::PACK:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::LABEL:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::NAME);
    break;
  case K::TEMPLATE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LIKELY:
    [[fallthrough]];
  case K::UNLIKELY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DEPRECIATED:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::MAY_COPY:
    [[fallthrough]];
  case K::MAY_MOVE:
    [[fallthrough]];
  case K::AUTO_DROP:
    [[fallthrough]];
  case K::DEFER:
    [[fallthrough]];
  case K::OK:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MESSAGE:
    if (situation == S::REFLECTION) {
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    }
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::MESSAGE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;

  // NODES
  case K::QUOTE:
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
    }
    break;
  case K::EXPRESSION:
    [[fallthrough]];
  case K::EXPAND:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::EXPAND_STATEMENT:
    [[fallthrough]];
  case K::EXPAND_LVALUE:
    [[fallthrough]];
  case K::EXPAND_RVALUE:
    [[fallthrough]];
  case K::EXPAND_REFLECTION:
    [[fallthrough]];
  case K::EXPAND_ARGUMENT:
    [[fallthrough]];
  case K::EXPAND_PARAMETER:
    [[fallthrough]];
  case K::EXPAND_NAME:
    [[fallthrough]];
  case K::EXPAND_NAMESPACE:
    [[fallthrough]];
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    is_ok = this->situateUnaryValueBranches(situation, expression, S::RVALUE);

  // REFLECTIONS
  case K::REFLECT: {
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
        const rq::Keyword universalized =
            next_branch.getUniversalized(situation);
        next_branch.changeKeyword(universalized);
        inner.setNext(next_branch.replaceBranchPtr(inner));
        inner_ptr = &next;
        continue;
      }
      if (!next.getIsUniversalizable()) {
        continue;
      }
      const rq::Keyword universalized = next.getUniversalized(situation);
      next.changeKeyword(universalized);
      inner.setNext(next.replaceBranchPtr(inner));
      inner_ptr = &next;
    }
    expression.setBranch(inner_ptr);
    this->getContext().discardExpression(expression.mergeAndPopBranch());
  } break;
  case K::MEMBER_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::MEMBER_OF_TOP:
    [[fallthrough]];
  case K::ASCEND_FRAME:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ASCEND_FRAME_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::BYTE_SIZE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BYTE_SIZE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::BIT_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::BIT_DEPTH_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::ELEMENT_COUNT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ELEMENT_COUNT_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::NAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::NAME_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::LINE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::COLUMN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::COLUMN_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_TYPE:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HOLDS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::HOLDS_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::TYPE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SYMBOL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SYMBOL_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS_MEMBER:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::HAS_MEMBER_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::HAS_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::GET:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::GET_OF:
    is_ok = this->situateBinaryValueBranches(situation, expression,
                                                  S::RVALUE, S::RVALUE);
    break;
  case K::SIGNATURE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SIGNATURE_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::LAYOUT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::LAYOUT_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::SYNONYM:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::SYNONYM_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
    break;
  case K::IS_OK:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::IS_OK_OF:
    is_ok =
        this->situateUnaryValueBranches(situation, expression, S::RVALUE);
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
  if (branch.getIsHeader()) {
    this->getContext().logErrorUnexpectedHeaderExpression(branch);
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
  if (!branch.getIsHeader()) {
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
  if (branch.getIsHeader()) {
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

bool Situator::situateBinaryValueBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation) {
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
    return false;
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
  bool found_named = false;
  bool found_positional = false;
  for (rq::Expression &parameter : first_parameter.getInclusiveNextSubrange()) {
    if (!this->situateValueBranch(rq::Situation::PARAMETER, parameter)) {
      is_ok = false;
    }
    switch (parameter.getKeyword()) {
    case rq::Keyword::NAMED_PARAMETERS_BEGIN:
      if (!parameter.getHasNext()) {
        is_ok = false;
        this->getContext().logErrorNamedBeginIsLast(expression);
      }
      if (found_named) {
        this->getContext().logErrorDuplicateParameterMark(parameter);
        is_ok = false;
      } else {
        found_named = true;
      }
      if (found_positional) {
        is_ok = false;
        this->getContext().logErrorNamedBeginAfterPositionalEnd(parameter);
      }
      break;
    case rq::Keyword::POSITIONAL_PARAMETERS_END:
      if (parameter == first_parameter) {
        this->getContext().logErrorPositionalEndIsFirst(parameter);
      }
      if (found_positional) {
        this->getContext().logErrorDuplicateParameterMark(parameter);
        is_ok = false;
      } else {
        found_positional = true;
      }
      break;
    default:
      break;
    }
  }
  if (!found_named && !found_positional) {
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
    if (branch.getIsHeader()) {
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
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateHeaderBranch(rq::Situation::NAME, branch0)) {
    is_ok = false;
  }
  if (!branch0.getHasNext()) {
    return is_ok;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (branch1.getIsHeader()) {
    if (!this->situateHeaderBranch(rq::Situation::RVALUE, branch1)) {
      is_ok = false;
    }
  } else {
    if (!this->situateStatementBranch(branch1)) {
      is_ok = false;
    }
  }
  for (rq::Expression &branch : branch1.getNextSubrange()) {
    if (!this->situateStatementBranch(branch)) {
      is_ok = false;
    }
  }
  return is_ok;
}

} // namespace rq
