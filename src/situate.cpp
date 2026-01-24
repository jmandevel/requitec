#include <rq/entity.hpp>
#include <rq/context.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallVector.h>

#include <utility>

namespace rq {

rq::Expression &Situator::makeModuleTrunk(rq::ModuleSymbol &module) {
  rq::Expression &trunk = this->getContext().acquireExpression();
  trunk.setIsInserted();
  trunk.setSource(module);
  trunk.setKeyword(rq::EntityKind::KW_MODULE_TRUNK);
  return trunk;
}

bool Situator::situateModule(rq::ModuleSymbol &module) {
  if (!module.getHasExpression()) {
    rq::Expression &trunk = this->makeModuleTrunk(module);
    module.setExpression(trunk);
    return true;
  }
  rq::Expression &first = module.getExpression();
  if (first.getKeyword() != rq::EntityKind::KW_MODULE_TRUNK) {
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
  using E = rq::EntityKind;

  bool is_ok = true;
  switch (expression.getKeyword()) {
  case E::KW_NONE:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    is_ok = false;
    break;

  // LITERALS
  case E::KW_INTEGER_LITERAL:
    [[fallthrough]];
  case E::KW_FLOAT_LITERAL:
    [[fallthrough]];
  case E::KW_STRING_LITERAL:
    [[fallthrough]];
  case E::KW_LEFT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case E::KW_MIDDLE_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case E::KW_RIGHT_INTERPOLATION_LITERAL:
    [[fallthrough]];
  case E::KW_CODEUNIT_LITERAL:
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    break;
  case E::KW_IDENTIFIER_LITERAL:
    break;

  // ERRORS
  case E::KW_ERROR:
    this->getContext().logErrorExpressionShouldNeverOccur(expression);
    is_ok = false;
    break;

  // SITUATIONAL
  case E::KW_UNSITUATED_PARENTHESIS_GROUP:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   situation);
    if (is_ok) {
      this->getContext().discardExpression(expression.mergeAndPopBranch());
    }
    break;
  case E::KW_UNSITUATED_EQUAL_OPERATOR:
    switch (situation) {
    case S::ARGUMENT: {
      is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                      S::LVALUE, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(E::KW_NAMED_ARGUMENT);
      }
    } break;
    case S::PARAMETER: {
      is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                      S::BINDING, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(E::KW_DEFAULT_VALUE_PARAMETER);
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
      if (!this->situateExpressionBranch(S::LVALUE, lvalue)) {
        is_ok = false;
      }
      if (!lvalue.getHasNext()) {
        this->getContext().logErrorNotExactBranchCount(situation, expression,
                                                       2);
        is_ok = false;
        break;
      }
      rq::Expression &rvalue = lvalue.getNext();
      if (!this->situateExpressionBranch(S::RVALUE, rvalue)) {
        is_ok = false;
      }
      if (!is_ok) {
        break;
      }
      switch (lvalue.getKeyword()) {
      case E::KW_NULL:
        expression.changeKeyword(E::KW_IGNORE);
        std::ignore = lvalue.popNext();
        this->getContext().discardExpression(expression.replaceBranch(rvalue));
        break;
      case E::KW_TUPLE:
        lvalue.changeKeyword(E::KW_STRUCTURED_BINDING);
        this->getContext().discardExpression(expression.mergeAndPopBranch());
        break;
      default:
        expression.changeKeyword(E::KW_ASSIGN);
        break;
      }
      break;
    }
    default:
      break;
    }
    break;
  case E::KW_UNSITUATED_ASCRIBE_STATEMENT:
    [[fallthrough]];
  case E::KW_UNSITUATED_ASCRIBE_TYPE: {
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
        if (!this->situateExpressionBranch(situation, branch)) {
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
      if (!this->situateExpressionBranch(attribute_situation, branch)) {
        is_ok = false;
        previous_ptr = &branch;
        continue;
      }
      bool has_flags = false;
      if (attribute_situation == S::EXPRESSION_ATTRIBUTE) {
        rq::ExpressionAttributeFlags flags =
            rq::getFlags(branch.getExpressionAttribute());
        if (rq::getHasAll(static_cast<rq::ExpressionAttributeFlags>(found_flags),
                          flags)) {
          has_flags = true;
        }
        found_flags = static_cast<std::uint32_t>(
            static_cast<rq::ExpressionAttributeFlags>(found_flags) | flags);
      } else if (attribute_situation == S::TYPE_ATTRIBUTE) {
        rq::TypeAttributeFlags flags = rq::getFlags(branch.getTypeAttribute());
        if (rq::getHasAll(static_cast<rq::TypeAttributeFlags>(found_flags),
                          flags)) {
          has_flags = true;
        }
        found_flags = static_cast<std::uint32_t>(
            static_cast<rq::TypeAttributeFlags>(found_flags) | flags);
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
  case E::KW_LOGICAL_AND:
    [[fallthrough]];
  case E::KW_LOGICAL_OR:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case E::KW_LOGICAL_COMPLEMENT:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // COMPARISON
  case E::KW_GREATER:
    [[fallthrough]];
  case E::KW_GREATER_EQUAL:
    [[fallthrough]];
  case E::KW_LESS:
    [[fallthrough]];
  case E::KW_LESS_EQUAL:
    [[fallthrough]];
  case E::KW_EQUAL:
    [[fallthrough]];
  case E::KW_NOT_EQUAL:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;

  // APPLY
  case E::KW_EXTEND:
    [[fallthrough]];
  case E::KW_EXTENSION:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_BINDING:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::LVALUE, S::RVALUE);
    break;
  case E::KW_ASCRIBE_TYPE:
    is_ok = this->situateNaryDifferentFirstExpressionBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    break;
  case E::KW_ASCRIBE_STATEMENT:
    is_ok = this->situateNaryDifferentFirstExpressionBranches(
        situation, expression, 2, situation, S::EXPRESSION_ATTRIBUTE);
    break;
  case E::KW_ASCRIBE_ROOT_OF_VALUE:
    is_ok = this->situateNaryDifferentFirstExpressionBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    break;
  case E::KW_IDENTIFY:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // ARITHMETIC
  case E::KW_ADD:
    [[fallthrough]];
  case E::KW_SUBTRACT:
    [[fallthrough]];
  case E::KW_MULTIPLY:
    [[fallthrough]];
  case E::KW_DIVIDE:
    [[fallthrough]];
  case E::KW_MODULUS:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case E::KW_NEGATE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // CASTS
  case E::KW_AS:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_LITERAL_AS:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_ELEMENTS_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_DOWN_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_DOWN_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_FLOAT_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_FLOAT_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_PLATFORM_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_PLATFORM_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_STATIC_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_STATIC_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_BITWISE_CAST:
    is_ok = this->situateNullaryOrUnaryExpressionBranches(
        situation, expression, S::RVALUE);
    break;
  case E::KW_BITWISE_CAST_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;

  // BITWISE
  case E::KW_BITWISE_OR:
    [[fallthrough]];
  case E::KW_BITWISE_AND:
    [[fallthrough]];
  case E::KW_BITWISE_XOR:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case E::KW_BITWISE_COMPLEMENT:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_BITWISE_SHIFT_LEFT:
    [[fallthrough]];
  case E::KW_BITWISE_SHIFT_RIGHT:
    [[fallthrough]];
  case E::KW_BITWISE_ROTATE_LEFT:
    [[fallthrough]];
  case E::KW_BITWISE_ROTATE_RIGHT:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;

  // MEMORY
  case E::KW_CONTENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_CONTENT_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_ADDRESS_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_BORROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_BORROW_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   situation);
    break;
  case E::KW_DATA_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_DATA_ADDRESS_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   situation);
    break;

  // ASSIGNMENT
  case E::KW_ASSIGN:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::LVALUE, S::RVALUE);
    break;

  // SUBTYPE
  case E::KW_ARRAY:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_REFERENCE:
    [[fallthrough]];
  case E::KW_POINTER:
    [[fallthrough]];
  case E::KW_FAT_POINTER:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // ARITHMETIC INTERVAL
  case E::KW_ARITHMETIC_INTERVAL_LESS:
    [[fallthrough]];
  case E::KW_ARITHMETIC_INTERVAL_LESS_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_INTERVAL_GREATER:
    [[fallthrough]];
  case E::KW_ARITHMETIC_INTERVAL_GREATER_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_INTERVAL_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_INTERVAL_NOT_EQUAL:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // FINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    [[fallthrough]];
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_ADD:
    [[fallthrough]];
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    [[fallthrough]];
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    [[fallthrough]];
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    [[fallthrough]];
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // TYPE MODIFIER
  case E::KW_MUTABLE:
    [[fallthrough]];
  case E::KW_CONSTANT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_PARTIALLY_MUTABLE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_VOLATILE:
    [[fallthrough]];
  case E::KW_ATOMIC:
    [[fallthrough]];
  case E::KW_NULL_TERMINATED:
    [[fallthrough]];
  case E::KW_MAY_DISCARD:
    [[fallthrough]];
  case E::KW_DEBUG_TRAP_ON_PANIC:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // PARAMETER RULES
  case E::KW_POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case E::KW_NAMED_PARAMETERS_BEGIN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // BRACES
  case E::KW_TUPLE:
    if (!expression.getHasBranch()) {
      expression.changeKeyword(E::KW_NULL);
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    }
    is_ok = this->situateNaryExpressionBranches(situation, expression, 0,
                                                  S::ARGUMENT);
    break;
  case E::KW_LAYOUT_TYPE:
    is_ok = this->situateNaryParameterBranches(situation, expression);
    break;
  case E::KW_NULL:
    [[fallthrough]];
  case E::KW_NULL_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_STRUCTURED_BINDING:
    is_ok = this->situateNaryDifferentLastExpressionBranches(
        situation, expression, 2, S::LVALUE, S::RVALUE);
    break;
  case E::KW_IGNORE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_SPECIALIZATION:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;

  // PROCEDURES
  case E::KW_CALL:
    is_ok = this->situateNaryDifferentFirstExpressionBranches(
        situation, expression, 1, S::RVALUE, S::ARGUMENT);
    break;
  case E::KW_NAMED_ARGUMENT:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::SYMBOL_PATH, S::RVALUE);
    break;
  case E::KW_INDEX_INTO:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_SIGNATURE_TYPE:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;
    break;
  case E::KW_DEFAULT_VALUE_PARAMETER:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::BINDING, S::RVALUE);
    break;
  case E::KW_DESTROY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case E::KW_DESTROY_VALUE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_DROP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_DROP_VALUE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_MOVE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_MOVE_VALUE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   situation);
    break;
  case E::KW_ENTRY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case E::KW_FUNCTION:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case E::KW_METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case E::KW_EXTENSION_FUNCTION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::SYMBOL_PATH, branch0)) {
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
  case E::KW_EXTENSION_METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case E::KW_CONSTRUCTOR: {
    if (!expression.getHasBranch()) {
      break;
    }
    rq::Expression *current = &expression.getBranch();
    int header_count = 0;
    while (current->getIsHeader() && header_count < 2) {
      if (!this->situateHeaderBranch(S::RVALUE, *current)) {
        is_ok = false;
      }
      header_count++;
      if (!current->getHasNext()) {
        current = nullptr;
        break;
      }
      current = &current->getNext();
    }
    if (current != nullptr) {
      if (current->getIsHeader()) {
        this->getContext().logErrorUnexpectedHeaderExpression(*current);
        is_ok = false;
      } else {
        if (!this->situateStatementBranch(*current)) {
          is_ok = false;
        }
      }
      for (rq::Expression &branch : current->getNextSubrange()) {
        if (!this->situateStatementBranch(branch)) {
          is_ok = false;
        }
      }
    }
    break;
  }
  case E::KW_LAYOUT_CONSTRUCTOR:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_DESTRUCTOR:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case E::KW_RANGER:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;

  // CONTROL FLOW
  case E::KW_RETURN:
    if (!expression.getHasBranch()) {
      break;
    }
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_BREAK:
    [[fallthrough]];
  case E::KW_CONTINUE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_FALLTHROUGH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_GOTO:
    [[fallthrough]];
  case E::KW_RANGE_OVER:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // DECLARED TYPES
  case E::KW_CLASS: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::SYMBOL_PATH, branch0)) {
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
  case E::KW_ENUMERATION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::SYMBOL_PATH, branch0)) {
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
  case E::KW_MUTATION:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;

  // VALUES
  case E::KW_INITIALIZER_LIST:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 0,
                                                  S::RVALUE);
    break;
  case E::KW_TRUE:
    [[fallthrough]];
  case E::KW_FALSE:
    [[fallthrough]];
  case E::KW_INDETERMINATE:
    [[fallthrough]];
  case E::KW_VALUE:
    [[fallthrough]];
  case E::KW_INDEX:
    [[fallthrough]];
  case E::KW_DISCRIMINANT:
    [[fallthrough]];
  case E::KW_OUT:
    [[fallthrough]];
  case E::KW_THIS:
    [[fallthrough]];
  case E::KW_RESULT:
    [[fallthrough]];
  case E::KW_COMMAND_LINE_ARGUMENTS:
    [[fallthrough]];
  case E::KW_EXIT_CODE:
    [[fallthrough]];

  // BUILTIN TYPES
  case E::KW_INFERENCE:
    [[fallthrough]];
  case E::KW_VOID:
    [[fallthrough]];
  case E::KW_NO_RETURN:
    [[fallthrough]];
  case E::KW_BOOLEAN:
    [[fallthrough]];
  case E::KW_FLOAT:
    [[fallthrough]];
  case E::KW_HALF:
    [[fallthrough]];
  case E::KW_SINGLE:
    [[fallthrough]];
  case E::KW_DOUBLE:
    [[fallthrough]];
  case E::KW_QUADRUPLE:
    [[fallthrough]];
  case E::KW_BINARY:
    [[fallthrough]];
  case E::KW_BFLOAT:
    [[fallthrough]];
  case E::KW_BINARY16:
    [[fallthrough]];
  case E::KW_BINARY32:
    [[fallthrough]];
  case E::KW_BINARY64:
    [[fallthrough]];
  case E::KW_BINARY128:
    [[fallthrough]];
  case E::KW_BFLOAT16:
    [[fallthrough]];
  case E::KW_INTEGER:
    [[fallthrough]];
  case E::KW_SIGNED:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_SIGNED_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_UNSIGNED:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_UNSIGNED_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_SIGNED_FAST_BITS:
    [[fallthrough]];
  case E::KW_SIGNED_FAST_BYTES:
    [[fallthrough]];
  case E::KW_SIGNED_LEAST_BITS:
    [[fallthrough]];
  case E::KW_SIGNED_LEAST_BYTES:
    [[fallthrough]];
  case E::KW_SIGNED_EXACT_BITS:
    [[fallthrough]];
  case E::KW_SIGNED_EXACT_BYTES:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_SIGNED_INDEX:
    [[fallthrough]];
  case E::KW_SIGNED_ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_UNSIGNED_FAST_BITS:
    [[fallthrough]];
  case E::KW_UNSIGNED_FAST_BYTES:
    [[fallthrough]];
  case E::KW_UNSIGNED_LEAST_BITS:
    [[fallthrough]];
  case E::KW_UNSIGNED_LEAST_BTYES:
    [[fallthrough]];
  case E::KW_UNSIGNED_EXACT_BITS:
    [[fallthrough]];
  case E::KW_UNSIGNED_EXACT_BYTES:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_UNSIGNED_INDEX:
    [[fallthrough]];
  case E::KW_UNSIGNED_ADDRESS:
    [[fallthrough]];
  case E::KW_ASCII:
    [[fallthrough]];
  case E::KW_UTF8:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // VARIADIC ARGUMENTS
  case E::KW_VARIADIC_ARGUMENTS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_FIRST_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_FIRST_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_NEXT_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_NEXT_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // SCOPES
  case E::KW_IF:
    [[fallthrough]];
  case E::KW_ELSE_IF:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case E::KW_ELSE:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 0,
                                                  situation);
    break;
  case E::KW_MATCH:
    [[fallthrough]];
  case E::KW_INLINE_MATCH:
    [[fallthrough]];
  case E::KW_SWITCH:
    [[fallthrough]];
  case E::KW_INLINE_SWITCH:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case E::KW_CASE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case E::KW_WITH:
    is_ok = this->situateNaryHeaderFirstAndSecondStatementBranches(
        situation, expression, S::RVALUE, S::STATEMENT);
    break;
  case E::KW_DEFAULT:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case E::KW_FOR: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::RVALUE, branch0)) {
      is_ok = false;
    }
    bool found_statement = false;
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (branch.getIsHeader()) {
        if (found_statement) {
          this->getContext().logErrorUnexpectedHeaderExpression(branch);
          is_ok = false;
        } else {
          if (!this->situateHeaderBranch(S::STATEMENT, branch)) {
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
    break;
  }
  case E::KW_WHILE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case E::KW_SCOPE:
    [[fallthrough]];
  case E::KW_INLINE_SCOPE:
    [[fallthrough]];
  case E::KW_BLOCK:
    [[fallthrough]];
  case E::KW_INLINE_BLOCK:
    is_ok = this->situateNaryExpressionBranches(situation, expression, 0,
                                                  situation);
    break;

  // RANGES
  case E::KW_RANGE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_ARITHMETIC_SEQUENCE: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &value = expression.getBranch();
    is_ok = this->situateExpressionBranch(S::RVALUE, value);
    if (!value.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &stage_one = value.getNext();
    if (!this->situateExpressionBranch(S::ARITHMETIC_SEQUENCE_STAGE,
                                         stage_one)) {
      is_ok = false;
    }
    if (stage_one.getHasNext()) {
      rq::Expression &stage_two = stage_one.getNext();
      if (!this->situateExpressionBranch(S::ARITHMETIC_SEQUENCE_STAGE,
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
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_STEP_ADD:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    [[fallthrough]];
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MODULUS:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // ACCESS MODIFIERS
  case E::KW_PUBLIC:
    [[fallthrough]];
  case E::KW_PROTECTED:
    [[fallthrough]];
  case E::KW_EXPORT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // TABLE GRAPH
  case E::KW_IMPORT:
    [[fallthrough]];
  case E::KW_USE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_FACADE:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_TABLE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::SYMBOL_PATH);
    break;
  case E::KW_MODULE_TRUNK:
    is_ok = this->situateNaryStatementBranches(expression);
    break;

  // ERROR HANDLING AND DEBUGGING
  case E::KW_PANIC_TRAP:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_DEBUG_TRAP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // HINTS
  case E::KW_UNREACHABLE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_ASSUME:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // SYMBOL ATTRIBUTES
  case E::KW_OPAQUE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_OUTSIDE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;
  case E::KW_STATIC:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_CAPTURE:
    if (situation == S::REFLECTION || situation == S::RVALUE) {
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    }
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_CAPTURE_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_EAGER:
    [[fallthrough]];
  case E::KW_MAY_PARENT:
    [[fallthrough]];
  case E::KW_PARENT:
    [[fallthrough]];
  case E::KW_ABSTRACT:
    [[fallthrough]];
  case E::KW_VIRTUAL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_OVERRIDE:
    [[fallthrough]];
  case E::KW_POSITION:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_INLINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_MANGLE:
    switch (situation) {
    case S::EXPRESSION_ATTRIBUTE:
      is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                     S::RVALUE);
      break;
    case S::REFLECTION:
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    default:
      break;
    }
    break;
  case E::KW_PACK:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_LABEL:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;
  case E::KW_TEMPLATE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_LIKELY:
    [[fallthrough]];
  case E::KW_UNLIKELY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_DEPRECIATED:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_MAY_COPY:
    [[fallthrough]];
  case E::KW_MAY_MOVE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_MUTATE_WITH:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;

  // NODES
  case E::KW_QUOTE:
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       1);
      is_ok = false;
    }
    break;
  case E::KW_EXPRESSION:
    [[fallthrough]];
  case E::KW_EXPAND:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_EXPAND_STATEMENT:
    [[fallthrough]];
  case E::KW_EXPAND_LVALUE:
    [[fallthrough]];
  case E::KW_EXPAND_RVALUE:
    [[fallthrough]];
  case E::KW_EXPAND_REFLECTION:
    [[fallthrough]];
  case E::KW_EXPAND_ARGUMENT:
    [[fallthrough]];
  case E::KW_EXPAND_PARAMETER:
    [[fallthrough]];
  case E::KW_EXPAND_SYMBOL_PATH:
    [[fallthrough]];
  case E::KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    [[fallthrough]];

  // REFLECTIONS
  case E::KW_REFLECT: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      is_ok = false;
      break;
    }
    rq::Expression &value = expression.popBranch();
    const rq::Situation first_situation =
        situation == S::REFLECTION ? S::REFLECTION : S::RVALUE;
    if (!this->situateExpressionBranch(first_situation, value)) {
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
      if (!this->situateExpressionBranch(S::REFLECTION, next)) {
        is_ok = false;
        continue;
      }
      if (next.getKeyword() == rq::EntityKind::KW_IDENTIFIER_LITERAL) {
        rq::Expression &member = this->getContext().acquireExpression();
        member.setIsInserted();
        member.setSource(inner, next);
        member.setKeyword(rq::EntityKind::KW_MEMBER_OF);
        member.setBranch(inner);
        inner.setNext(next);
        inner_ptr = &member;
        continue;
      } else if (next.getKeyword() == rq::EntityKind::KW_ASCRIBE_TYPE) {
        next.changeKeyword(rq::EntityKind::KW_ASCRIBE_ROOT_OF_VALUE);
        rq::Expression &next_branch = next.getBranch();
        if (!next_branch.getIsUniversalizable()) {
          continue;
        }
        const rq::EntityKind universalized =
            next_branch.getUniversalized(situation);
        next_branch.changeKeyword(universalized);
        inner.setNext(next_branch.replaceBranchPtr(inner));
        inner_ptr = &next;
        continue;
      }
      if (!next.getIsUniversalizable()) {
        continue;
      }
      const rq::EntityKind universalized = next.getUniversalized(situation);
      next.changeKeyword(universalized);
      inner.setNext(next.replaceBranchPtr(inner));
      inner_ptr = &next;
    }
    expression.setBranch(inner_ptr);
    this->getContext().discardExpression(expression.mergeAndPopBranch());
  } break;
  case E::KW_MEMBER_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_MEMBER_OF_TOP:
    [[fallthrough]];
  case E::KW_ASCEND_FRAME:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_ASCEND_FRAME_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_BYTE_SIZE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_BYTE_SIZE_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_BIT_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_BIT_DEPTH_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_ELEMENT_COUNT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_ELEMENT_COUNT_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_NAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_NAME_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_LINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_LINE_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_COLUMN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_COLUMN_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_IS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_IS_TYPE:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_HOLDS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_HOLDS_ENUMERATOR:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_TYPE_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_SYMBOL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_SYMBOL_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_HAS_MEMBER:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_HAS_MEMBER_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_HAS:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_HAS_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_GET:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_GET_OF:
    is_ok = this->situateBinaryExpressionBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case E::KW_SIGNATURE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_SIGNATURE_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_LAYOUT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_LAYOUT_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case E::KW_SYNONYM:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case E::KW_SYNONYM_OF:
    is_ok = this->situateUnaryExpressionBranches(situation, expression,
                                                   S::RVALUE);
    break;
  default:
    RQ_UNREACHABLE();
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

bool Situator::situateExpressionBranch(rq::Situation branch_situation,
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

bool Situator::situateNullaryOrUnaryExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    return true;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateExpressionBranch(branch0_situation, branch0);
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateUnaryExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateExpressionBranch(branch0_situation, branch0);
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateBinaryExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateExpressionBranch(branch0_situation, branch0);
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    return false;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateExpressionBranch(branch1_situation, branch1)) {
    is_ok = false;
  }
  if (branch1.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 2);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!this->situateExpressionBranch(branchn_situation, branch)) {
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

bool Situator::situateNaryDifferentFirstExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateExpressionBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    branch_i++;
    for (rq::Expression &branch : branch0.getNextSubrange()) {
      if (!this->situateExpressionBranch(branchn_situation, branch)) {
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

bool Situator::situateNaryDifferentLastExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation,
    rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!branch.getHasNext()) {
      if (!this->situateExpressionBranch(last_situation, branch)) {
        is_ok = false;
      }
      break;
    }
    if (!this->situateExpressionBranch(branchn_situation, branch)) {
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

bool Situator::situateNaryDifferentFirstAndLastExpressionBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation, rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateExpressionBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (!branch.getHasNext()) {
        if (!this->situateExpressionBranch(last_situation, branch)) {
          is_ok = false;
        }
        break;
      }
      if (!this->situateExpressionBranch(branchn_situation, branch)) {
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
  if (!this->situateExpressionBranch(branch0_situation, branch0)) {
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
    if (!this->situateExpressionBranch(rq::Situation::PARAMETER, parameter)) {
      is_ok = false;
    }
    switch (parameter.getKeyword()) {
    case rq::EntityKind::KW_NAMED_PARAMETERS_BEGIN:
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
    case rq::EntityKind::KW_POSITIONAL_PARAMETERS_END:
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

bool Situator::situateNaryHeaderFirstStatementBranches(
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

bool Situator::situateNaryHeaderFirstAndSecondStatementBranches(
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

bool Situator::situateNamedMemberProcedure(rq::Situation situation,
                                           rq::Expression &expression) {
  bool is_ok = true;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(situation, expression, 1);
    is_ok = false;
    return is_ok;
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateHeaderBranch(rq::Situation::SYMBOL_PATH, branch0)) {
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
