#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/situate.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallVector.h>

#include <utility>

namespace rq {

rq::Expression &Situator::makeModuleTrunk(rq::Module &module) {
  rq::Expression &trunk = this->getStaticFrame().acquireExpression();
  trunk.setIsInserted();
  trunk.setSource(module);
  return trunk;
}

bool Situator::situateModule(rq::Module &module) {
  if (!module.getHasExpression()) {
    rq::Expression &trunk = this->makeModuleTrunk(module);
    module.setExpression(trunk);
    return true;
  }
  rq::Expression &first = module.getExpression();
  if (first.getKeyword() != rq::Keyword::S_MODULE_TRUNK) {
    rq::Expression &trunk = this->makeModuleTrunk(module);
    trunk.setNext(module.replaceExpression(trunk));
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
  case K::I_NONE:
    this->getContext().logErrorExpressionShouldNeverOccur(rq::System::SITUATOR,
                                                          expression);
    is_ok = false;
    break;

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
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    break;
  case K::I_IDENTIFIER_LITERAL:
    break;

  // ERRORS
  case K::I_ERROR:
    this->getContext().logErrorExpressionShouldNeverOccur(rq::System::SITUATOR,
                                                          expression);
    is_ok = false;
    break;

  // SITUATIONAL
  case K::S_PARENTHESIS_GROUP:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   situation);
    break;
  case K::S_EQUAL_OPERATOR:
    switch (situation) {
    case S::ARGUMENT: {
      is_ok = this->situateBinaryNonStatementBranches(
          situation, expression, S::SYMBOL_PATH, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::S_NAMED_ARGUMENT);
      }
    } break;
    case S::PARAMETER: {
      is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                      S::BINDING, S::RVALUE);
      if (is_ok) {
        expression.changeKeyword(K::S_DEFAULT_VALUE_PARAMETER);
      }
    } break;
    case S::STATEMENT: {
      if (!expression.getHasBranch()) {
        this->getContext().logErrorNotExactBranchCount(
            rq::System::SITUATOR, situation, expression, 2);
        is_ok = false;
        break;
      }
      rq::Expression &lvalue = expression.getBranch();
      if (!this->situateNonStatementBranch(S::LVALUE, lvalue)) {
        is_ok = false;
      }
      if (!lvalue.getHasNext()) {
        this->getContext().logErrorNotExactBranchCount(
            rq::System::SITUATOR, situation, expression, 2);
        is_ok = false;
        break;
      }
      rq::Expression &rvalue = lvalue.getNext();
      if (!this->situateNonStatementBranch(S::RVALUE, rvalue)) {
        is_ok = false;
      }
      if (!is_ok) {
        break;
      }
      switch (lvalue.getKeyword()) {
      case K::S_NULL:
        expression.changeKeyword(K::S_IGNORE);
        this->getStaticFrame().discardExpression(
            expression.replaceBranch(rvalue));
        break;
      case K::S_TUPLE:
        lvalue.changeKeyword(K::S_STRUCTURED_BINDING);
        this->getStaticFrame().discardExpression(
            expression.mergeAndPopBranch());
        break;
        ;
      default:
        expression.changeKeyword(K::S_ASSIGN);
        break;
      }
      break;
    }
    default:
      break;
    }
    break;
  case K::S_COLON_OPERATOR:
    if (!this->situateBinaryNonStatementBranches(situation, expression,
                                                 S::RVALUE, S::RVALUE)) {
      is_ok = false;
      break;
    }
    switch (situation) {
    case S::PARAMETER:
      [[fallthrough]];
    case S::STATEMENT:
      expression.changeKeyword(K::S_BINDING);
      break;
    case S::RVALUE:
      [[fallthrough]];
    case S::ARGUMENT:
      expression.changeKeyword(K::S_CAST);
      break;
    default:
      break;
    }
    break;
  case K::S_INFERENCE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_UNSITUATED_ASCRIBE_STATEMENT:
    RQ_TODO_IMPLEMENTATION();
  case K::S_UNSITUATED_ASCRIBE_TYPE:
    RQ_TODO_IMPLEMENTATION();

  // LOGICAL
  case K::S_LOGICAL_AND:
    [[fallthrough]];
  case K::S_LOGICAL_OR:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case K::S_LOGICAL_COMPLEMENT:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

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
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;

  // APPLY
  case K::S_EXTEND:
    [[fallthrough]];
  case K::S_EXTENSION:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case K::S_BINDING:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::SYMBOL_PATH, S::RVALUE);
    break;
  case K::S_ASCRIBE_TYPE:
    is_ok = this->situateNaryDifferentFirstNonStatementBranches(
        situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    break;
  case K::S_ASCRIBE_STATEMENT:
    is_ok = this->situateNaryDifferentFirstNonStatementBranches(
        situation, expression, 2, situation, S::STATEMENT_ATTRIBUTE);
    break;
  case K::S_CAST:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case K::S_IDENTIFY:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

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
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case K::S_NEGATE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // BITWISE
  case K::S_BITWISE_CAST:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case K::S_BITWISE_OR:
    [[fallthrough]];
  case K::S_BITWISE_AND:
    [[fallthrough]];
  case K::S_BITWISE_XOR:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 2,
                                                  S::RVALUE);
    break;
  case K::S_BITWISE_COMPLEMENT:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::S_BITWISE_SHIFT_LEFT:
    [[fallthrough]];
  case K::S_BITWISE_SHIFT_RIGHT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_LEFT:
    [[fallthrough]];
  case K::BITWISE_ROTATE_RIGHT:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;

  // MEMORY
  case K::SINGLETON:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_SINGLETON_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::CONTENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_CONTENT_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::ADDRESS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_ADDRESS_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::BORROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_BORROW_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // ASSIGNMENT
  case K::S_ASSIGN:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::LVALUE, S::RVALUE);
    break;
  case K::S_ASSIGN_ADD:
    [[fallthrough]];
  case K::S_ASSIGN_SUBTRACT:
    [[fallthrough]];
  case K::S_ASSIGN_MULTIPLY:
    [[fallthrough]];
  case K::S_ASSIGN_DIVIDE:
    [[fallthrough]];
  case K::S_ASSIGN_MODULUS:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;

  // SUBTYPE
  case K::S_ARRAY:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case K::S_REFERENCE:
    [[fallthrough]];
  case K::S_POINTER:
    [[fallthrough]];
  case K::S_FAT_POINTER:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

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
  case K::DYNAMIC_CAPTURE_LAYOUT:
    is_ok = this->situateNaryParameterBranches(situation, expression);
    break;

  // PARAMETER RULES
  case K::S_POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case K::S_NAMED_PARAMETERS_BEGIN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // BRACES
  case K::S_TUPLE:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 1,
                                                  S::RVALUE);
    break;
  case K::S_LAYOUT_TYPE:
    is_ok = this->situateNaryParameterBranches(situation, expression);
    break;
  case K::S_NULL:
    [[fallthrough]];
  case K::S_NULL_TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_STRUCTURED_BINDING:
    is_ok = this->situateNaryDifferentLastNonStatementBranches(
        situation, expression, 2, S::LVALUE, S::RVALUE);
    break;
  case K::S_IGNORE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::S_SPECIALIZATION:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;

  // PROCEDURES
  case K::S_CALL:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;
  case K::CONSTRUCT_FUNCTOR:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_CONSTRUCT_FUNCTOR_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::S_NAMED_ARGUMENT:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::SYMBOL_PATH, S::RVALUE);
    break;
  case K::S_INDEX_INTO:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::RVALUE, S::RVALUE);
    break;
  case K::S_SIGNATURE_TYPE:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::RVALUE);
    break;
    break;
  case K::S_DEFAULT_VALUE_PARAMETER:
    is_ok = this->situateBinaryNonStatementBranches(situation, expression,
                                                    S::BINDING, S::RVALUE);
    break;
  case K::DESTROY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::S_DESTROY_VALUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::DROP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_DROP_VALUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::MOVE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_MOVE_VALUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::THROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_THROW_VALUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::ENTRY_POINT:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::FUNCTION:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::METHOD:
    is_ok = this->situateNamedMemberProcedure(situation, expression);
    break;
  case K::EXTENSION_FUNCTION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::SYMBOL_PATH, branch0)) {
      is_ok = false;
    }
    if (!branch0.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
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
  case K::EXTENSION_METHOD: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::SYMBOL_PATH, branch0)) {
      is_ok = false;
    }
    if (!branch0.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &branch1 = branch0.getNext();
    if (!this->situateHeaderBranch(S::RVALUE, branch1)) {
      is_ok = false;
    }
    for (rq::Expression &branch : branch1.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
    break;
  }
  case K::CONSTRUCTOR: {
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
        this->getContext().logErrorUnexpectedHeaderExpression(
            rq::System::SITUATOR, *current);
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
  case K::LAYOUT_CONSTRUCTOR:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DESTRUCTOR:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::RANGER:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::S_ANONYMOUS_FUNCTION: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateHeaderBranch(S::DYNAMIC_CAPTURE, branch0)) {
      is_ok = false;
    }
    if (!branch0.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &branch1 = branch0.getNext();
    if (!this->situateHeaderBranch(S::RVALUE, branch1)) {
      is_ok = false;
    }
    for (rq::Expression &branch : branch1.getNextSubrange()) {
      if (!this->situateStatementBranch(branch)) {
        is_ok = false;
      }
    }
    break;
  }
  case K::S_DYNAMIC_CAPTURE:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 0,
                                                  S::ARGUMENT);
    break;

  // CONTROL FLOW
  case K::RETURN:
    if (!expression.getHasBranch()) {
      break;
    }
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::BREAK:
    [[fallthrough]];
  case K::CONTINUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::FALLTHROUGH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::GOTO:
    [[fallthrough]];
  case K::RANGE_OVER:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // DECLARED TYPES
  case K::OBJECT: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 1);
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
          this->getContext().logErrorUnexpectedHeaderExpression(
              rq::System::SITUATOR, branch);
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
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 1);
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
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // BUILTIN TYPES
  case K::VOID:
    [[fallthrough]];
  case K::NO_RETURN:
    [[fallthrough]];
  case K::BOOLEAN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::WORD:
    [[fallthrough]];
  case K::SIGNED:
    [[fallthrough]];
  case K::UNSIGNED:
    if (!expression.getHasBranch()) {
      break;
    }
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
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
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::FIRST_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_FIRST_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::NEXT_VARIADIC_ARGUMENT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_NEXT_VARIADIC_ARGUMENT_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // SCOPES
  case K::IF:
    [[fallthrough]];
  case K::ELSE_IF:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::ELSE:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 0,
                                                  situation);
    break;
  case K::TRY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::CATCH:
    [[fallthrough]];
  case K::FINALLY:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::MATCH:
    [[fallthrough]];
  case K::INLINE_MATCH:
    [[fallthrough]];
  case K::SWITCH:
    [[fallthrough]];
  case K::INLINE_SWITCH:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::CASE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::DEFAULT:
    is_ok = this->situateNaryStatementBranches(expression);
    break;
  case K::FOR: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 1);
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
          this->getContext().logErrorUnexpectedHeaderExpression(
              rq::System::SITUATOR, branch);
          is_ok = false;
        } else {
          if (!this->situateHeaderBranch(S::VIGNETTE, branch)) {
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
  case K::WHILE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::RVALUE);
    break;
  case K::SCOPE:
    [[fallthrough]];
  case K::INLINE_SCOPE:
    [[fallthrough]];
  case K::BLOCK:
    [[fallthrough]];
  case K::INLINE_BLOCK:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 0,
                                                  situation);
    break;

  // RANGES
  case K::RANGE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::S_SEQUENCE: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &value = expression.getBranch();
    is_ok = this->situateNonStatementBranch(S::RVALUE, value);
    if (!value.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 2);
      is_ok = false;
      break;
    }
    rq::Expression &stage_one = value.getNext();
    if (!this->situateNonStatementBranch(S::SEQUENCE_STAGE, stage_one)) {
      is_ok = false;
    }
    if (stage_one.getHasNext()) {
      rq::Expression &stage_two = stage_one.getNext();
      if (!this->situateNonStatementBranch(S::SEQUENCE_STAGE, stage_two)) {
        is_ok = false;
      }
      if (stage_two.getHasNext()) {
        this->getContext().logErrorTooManyBranchCount(rq::System::SITUATOR,
                                                      situation, expression, 3);
        is_ok = false;
        break;
      }
    }
    break;
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
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
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
    [[fallthrough]];
  case K::USE_TABLE:
    [[fallthrough]];
  case K::FACADE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::TABLE:
    is_ok = this->situateNaryHeaderFirstStatementBranches(situation, expression,
                                                          S::SYMBOL_PATH);
    break;
  case K::S_MODULE_TRUNK:
    is_ok = this->situateNaryStatementBranches(expression);
    break;

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::DEBUG_TRAP:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;

  // HINTS
  case K::UNREACHABLE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::ASSUME:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // STATEMENT ATTRIBUTES
  case K::OPAQUE:
    [[fallthrough]];
  case K::GLOBAL:
    [[fallthrough]];
  case K::STATIC:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::STATIC_CAPTURE:
    is_ok = this->situateNaryNonStatementBranches(situation, expression, 0,
                                                  S::ARGUMENT);
    break;
  case K::EVALUATE_IMMEDIATELY:
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
  case K::POSITION:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::INLINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MANGLED_NAME:
    switch (situation) {
    case S::STATEMENT_ATTRIBUTE:
      is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                     S::RVALUE);
      break;
    case S::REFLECTION:
      is_ok = this->situateNullaryExpression(situation, expression);
      break;
    default:
      break;
    }
    break;
  case K::S_MANGLED_NAME_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::PACK:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::USER_ATTRIBUTE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::S_USER_ATTRIBUTE_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::LABEL:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;
  case K::TEMPLATE:
    is_ok = this->situateNaryDifferentFirstParamterBranches(
        situation, expression, S::PARAMETER);
    break;
  case K::LIKELY:
    [[fallthrough]];
  case K::UNLIKELY:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::DEPRECIATED:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::MAY_COPY:
    [[fallthrough]];
  case K::MAY_MOVE:
    [[fallthrough]];
  case K::MAY_THROW:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::MUTABILITY_CLASS:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::SYMBOL_PATH);
    break;

  // EXPRESSIONS
  case K::QUOTE:
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(
          rq::System::SITUATOR, situation, expression, 1);
      is_ok = false;
    }
    break;
  case K::EXPRESSION:
    [[fallthrough]];
  case K::EXPAND:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_EXPAND_STATEMENT:
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
  case K::S_EXPAND_SYMBOL_PATH:
    [[fallthrough]];
  case K::S_EXPAND_SEQUENCE_STAGE:
    [[fallthrough]];
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    [[fallthrough]];
  case K::S_EXPAND_VIGNETTE:
    [[fallthrough]];
  case K::S_EXPAND_VIGNETTE_RVALUE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  // REFLECTIONS
  case K::S_REFLECT:
    RQ_TODO_IMPLEMENTATION();
  case K::BYTE_SIZE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_BYTE_SIZE_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::BIT_DEPTH:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_BIT_DEPTH_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::ELEMENT_COUNT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_ELEMENT_COUNT_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::NAME:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_NAME_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::LINE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_LINE_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::COLUMN:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_COLUMN_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::IS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_IS_TYPE:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::HOLDS:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_HOLDS_ENUMERATOR:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::TYPE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_TYPE_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::SYMBOL:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_SYMBOL_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::SIGNATURE:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_SIGNATURE_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;
  case K::LAYOUT:
    is_ok = this->situateNullaryExpression(situation, expression);
    break;
  case K::S_LAYOUT_OF:
    is_ok = this->situateUnaryNonStatementBranches(situation, expression,
                                                   S::RVALUE);
    break;

  case K::I_LAST:
    RQ_UNREACHABLE();
  }
  if (!is_ok) {
    expression.setHasSituatorError();
  }
  if (is_ok && expression.getIsConverging()) {
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (expression.getKeyword() == branch.getKeyword()) {
        this->getStaticFrame().discardExpression(
            expression.mergeAndPopBranch());
      }
    }
  }
  return is_ok;
}

bool Situator::situateNonStatementBranch(rq::Situation branch_situation,
                                         rq::Expression &branch) {
  if (branch.getIsHeader()) {
    this->getContext().logErrorUnexpectedHeaderExpression(rq::System::SITUATOR,
                                                          branch);
    return false;
  }
  if (branch.getIsChainLink()) {
    this->getContext().logErrorUnexpectedChainLinkExpression(
        rq::System::SITUATOR, branch);
    return false;
  }
  if (!branch.getCanBeSituation(branch_situation)) {
    this->getContext().logErrorInvalidBranchSituation(rq::System::SITUATOR,
                                                      branch_situation, branch);
    return false;
  }
  return this->situateTree(branch_situation, branch);
}

bool Situator::situateHeaderBranch(rq::Situation branch_situation,
                                   rq::Expression &branch) {
  if (!branch.getIsHeader()) {
    this->getContext().logErrorExpectedHeaderExpression(rq::System::SITUATOR,
                                                        branch);
    return false;
  }
  if (branch.getIsChainLink()) {
    this->getContext().logErrorUnexpectedChainLinkExpression(
        rq::System::SITUATOR, branch);
    return false;
  }
  if (!branch.getCanBeSituation(branch_situation)) {
    this->getContext().logErrorInvalidBranchSituation(rq::System::SITUATOR,
                                                      branch_situation, branch);
    return false;
  }
  return this->situateTree(branch_situation, branch);
}

bool Situator::situateStatementBranch(rq::Expression &branch) {
  if (branch.getIsHeader()) {
    this->getContext().logErrorUnexpectedHeaderExpression(rq::System::SITUATOR,
                                                          branch);
    return false;
  }
  if (!branch.getCanBeStatement()) {
    this->getContext().logErrorInvalidBranchSituation(
        rq::System::SITUATOR, rq::Situation::STATEMENT, branch);
    return false;
  }
  return this->situateTree(rq::Situation::STATEMENT, branch);
}

bool Situator::situateNullaryExpression(rq::Situation situation,
                                        rq::Expression &expression) {
  if (expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 0);
    return false;
  }
  return true;
}

bool Situator::situateUnaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 1);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateNonStatementBranch(branch0_situation, branch0);
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateBinaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation, rq::Situation branch1_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 2);
    return false;
  }
  rq::Expression &branch0 = expression.getBranch();
  bool is_ok = this->situateNonStatementBranch(branch0_situation, branch0);
  if (!branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 2);
    return false;
  }
  rq::Expression &branch1 = branch0.getNext();
  if (!this->situateNonStatementBranch(branch1_situation, branch1)) {
    is_ok = false;
  }
  if (branch1.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(rq::System::SITUATOR,
                                                   situation, expression, 2);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!this->situateNonStatementBranch(branchn_situation, branch)) {
      is_ok = false;
    }
    branch_i++;
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(
        rq::System::SITUATOR, situation, expression, minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateNonStatementBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    branch_i++;
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (!this->situateNonStatementBranch(branchn_situation, branch)) {
        is_ok = false;
      }
      branch_i++;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(
        rq::System::SITUATOR, situation, expression, minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentLastNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branchn_situation,
    rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  for (rq::Expression &branch : expression.getBranchSubrange()) {
    if (!branch.getHasNext()) {
      if (!this->situateNonStatementBranch(last_situation, branch)) {
        is_ok = false;
      }
      break;
    }
    if (!this->situateNonStatementBranch(branchn_situation, branch)) {
      is_ok = false;
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(
        rq::System::SITUATOR, situation, expression, minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstAndLastNonStatementBranches(
    rq::Situation situation, rq::Expression &expression,
    unsigned minimum_branch_count, rq::Situation branch0_situation,
    rq::Situation branchn_situation, rq::Situation last_situation) {
  bool is_ok = true;
  unsigned branch_i = 0;
  if (expression.getHasBranch()) {
    rq::Expression &branch0 = expression.getBranch();
    if (!this->situateNonStatementBranch(branch0_situation, branch0)) {
      is_ok = false;
    }
    for (rq::Expression &branch : expression.getBranchSubrange()) {
      if (!branch.getHasNext()) {
        if (!this->situateNonStatementBranch(last_situation, branch)) {
          is_ok = false;
        }
        break;
      }
      if (!this->situateNonStatementBranch(branchn_situation, branch)) {
        is_ok = false;
      }
    }
  }
  if (branch_i < minimum_branch_count) {
    this->getContext().logErrorNotAtLeastBranchCount(
        rq::System::SITUATOR, situation, expression, minimum_branch_count);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNaryDifferentFirstParamterBranches(
    rq::Situation situation, rq::Expression &expression,
    rq::Situation branch0_situation) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(rq::System::SITUATOR,
                                                     situation, expression, 1);
  }
  rq::Expression &branch0 = expression.getBranch();
  if (!this->situateNonStatementBranch(branch0_situation, branch0)) {
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
    if (!this->situateNonStatementBranch(rq::Situation::PARAMETER, parameter)) {
      is_ok = false;
    }
    switch (parameter.getKeyword()) {
    case rq::Keyword::S_NAMED_PARAMETERS_BEGIN:
      if (!parameter.getHasNext()) {
        is_ok = false;
        this->getContext().logErrorNamedBeginIsLast(rq::System::SITUATOR,
                                                    expression);
      }
      if (found_named) {
        this->getContext().logErrorDuplicateParameterMark(rq::System::SITUATOR,
                                                          parameter);
        is_ok = false;
      } else {
        found_named = true;
      }
      if (found_positional) {
        is_ok = false;
        this->getContext().logErrorNamedBeginAfterPositionalEnd(
            rq::System::SITUATOR, parameter);
      }
      break;
    case rq::Keyword::S_POSITIONAL_PARAMETERS_END:
      if (parameter == first_parameter) {
        this->getContext().logErrorPositionalEndIsFirst(rq::System::SITUATOR,
                                                        parameter);
      }
      if (found_positional) {
        this->getContext().logErrorDuplicateParameterMark(rq::System::SITUATOR,
                                                          parameter);
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
    this->getContext().logErrorMustHaveParameterMark(rq::System::SITUATOR,
                                                     situation, expression);
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
    this->getContext().logErrorNotAtLeastBranchCount(rq::System::SITUATOR,
                                                     situation, expression, 1);
    is_ok = false;
  }
  return is_ok;
}

bool Situator::situateNamedMemberProcedure(rq::Situation situation,
                                           rq::Expression &expression) {
  bool is_ok = true;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount(rq::System::SITUATOR,
                                                     situation, expression, 1);
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
