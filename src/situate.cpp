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
    std::ignore = module.popExpression();
    root.setBranch(first);
  }
  rq::Expression &root = module.getExpression();
  return this->situateTree(rq::Situation::ROOT_STATEMENT, root);
}

#define RQ_ASSERT_VALID_SITUATION(situation, expression)                       \
  RQ_ASSERT(expression.getCanBeSituation(situation),                           \
            "keyword not valid for situation")

#define RQ_ASSERT_NOT_CHAIN_LINK(expression)                                   \
  RQ_ASSERT(!branch0.getIsChainLink(), "branch must never be chain link")

#define RQ_ASSERT_NOT_BOLD(expression)                                         \
  RQ_ASSERT(!branch0.getIsChainLink(), "branch must never be bold")

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
  case K::S_BRACE_GROUP:
    return false; // TODO
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
  case K::S_ASCRIBE_TYPE: {
    bool is_ok = true;
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      return false;
    }
    rq::Expression &first = expression.getBranch();
    if (!first.getCanBeTypeAttribute()) {
      is_ok = this->situateNaryNonStatementBranches(
          situation, expression, 2, situation, S::TYPE_ATTRIBUTE);
    } else {
      unsigned branch_i = 0;
      is_ok = this->situateBranch(situation, expression, S::TYPE_ATTRIBUTE,
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
        if (!this->situateBranch(situation, expression, S::TYPE_ATTRIBUTE, next,
                                 branch_i++, "first to penultimate branch")) {
          is_ok = false;
        }
        previous_ptr = &next;
      }
    }
    if (!is_ok) {
      return false;
    }
    rq::Expression &unascribed = expression.getBranch();
    if (unascribed.getKeyword() == K::S_EXTENSION) {
      rq::Expression &extended = unascribed.getBranch();
      if (extended.getKeyword() == K::S_ASCRIBE_TYPE) {
        rq::Expression &extended_last =
            extended.getLastBranch(); // TODO: replace usage of getLastBranch
                                      // to avoid excessive pointer chasing
        extended_last.setNext(unascribed.popNext());
        this->getStaticFrame().discardExpression(
            expression.mergeAndPopBranch());
      } else {
        rq::Expression &first_attribute = unascribed.popNext();
        rq::Expression &branch = expression.mergeAndPopBranch();
        branch.setKeyword(K::S_ASCRIBE_TYPE);
        branch.setBranch(expression.replaceBranch(branch));
        branch.setNext(extended.replaceNext(first_attribute));
      }
    }
    return true;
  }
  case K::S_ASCRIBE_STATEMENT: {
    if (!expression.getHasBranch()) {
      this->getContext().logErrorNotAtLeastBranchCount(situation, expression,
                                                       2);
      return false;
    }
    unsigned branch_i = 0;
    rq::Expression &first = expression.getBranch();
    if (first.getCanBeStatementAttribute()) {
      bool is_ok =
          this->situateBranch(situation, expression, S::STATEMENT_ATTRIBUTE,
                              first, branch_i++, "first to penultimate branch");
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
        if (!this->situateBranch(situation, expression, S::STATEMENT_ATTRIBUTE,
                                 next, branch_i++,
                                 "first to penultimate branch")) {
          is_ok = false;
        }
        previous_ptr = &next;
      }
    } else {
      return this->situateNaryNonStatementBranches(
          situation, expression, 2, situation, S::STATEMENT_ATTRIBUTE);
    }
  }
    return true;
  case K::S_ASCRIBE_TYPE_OF_ELEMENTS:
    return this->situateNaryNonStatementBranches(situation, expression, 2,
                                                 S::RVALUE, S::TYPE_ATTRIBUTE);
    break;
  case K::S_CAST:
    return this->situateBinaryNonStatementBranches(situation, expression,
                                                   S::RVALUE, S::RVALUE);
  case K::S_IDENTIFY:
    return this->situateNullaryExpression(situation, expression);

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
    return this->situateNullaryExpression(situation, expression);
  case K::S_SINGLETON_OF:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::CONTENT:
    return this->situateNullaryExpression(situation, expression);
  case K::S_CONTENT_OF:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::ADDRESS:
    return this->situateNullaryExpression(situation, expression);
  case K::S_ADDRESS_OF:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::BORROW:
    return this->situateNullaryExpression(situation, expression);
  case K::S_BORROW_OF:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);
  case K::VIEW:
    return this->situateNullaryExpression(situation, expression);
  case K::S_VIEW_OF:
    return this->situateUnaryNonStatementBranches(situation, expression,
                                                  S::RVALUE);

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
  case K::DYNAMIC_CAPTURE_LAYOUT:{
    // TODO
    return false;
  }

  // PARAMETER RULES
  case K::S_POSITIONAL_PARAMETERS_END:
    [[fallthrough]];
  case K::S_NAMED_PARAMETERS_BEGIN:
    return this->situateNullaryExpression(situation, expression);

  // BRACES
  case K::S_TUPLE:
    return this->situateNaryNonStatementBranches(situation, expression, 1,
                                                 S::RVALUE);
  case K::S_LAYOUT_TYPE: {
    // TODO
    return false;
  }
  case K::S_NULL:
    [[fallthrough]];
  case K::S_NULL_TYPE:
    return this->situateNullaryExpression(situation, expression);
  case K::S_STRUCTURED_BINDING:
    return this->situateNaryWithLastNonStatementBranches(situation, expression, 2, S::LVALUE, S::RVALUE);
  case K::S_IGNORE:
    return this->situateUnaryNonStatementBranches(situation, expression, S::RVALUE);
  case K::S_SPECIALIZATION:
    return this->situateNaryNonStatementBranches(situation, expression, 1, S::RVALUE, S::ARGUMENT);

  // PROCEDURES
  case K::S_CALL:
    return this->situateNaryNonStatementBranches(situation, expression, 1, S::RVALUE, S::ARGUMENT);
  case K::S_INDEX_INTO:
    return this->situateBinaryNonStatementBranches(situation, expression, S::RVALUE, S::RVALUE);
  case K::S_SIGNATURE_TYPE:
    return false; // TODO
  case K::S_DEFAULT_VALUE_PARAMETER:
    break;
  case K::DESTROY:
    break;
  case K::S_DESTROY_VALUE:
    break;
  case K::DROP:
    break;
  case K::S_DROP_VALUE:
    break;
  case K::EXTRACT:
    break;
  case K::S_EXTRACT_OF:
    break;
  case K::MOVE:
    break;
  case K::S_MOVE_OF:
    break;
  case K::ENTRY_POINT:
    break;
  case K::FUNCTION:
    break;
  case K::METHOD:
    break;
  case K::EXTENSION_FUNCTION:
    break;
  case K::EXTENSION_METHOD:
    break;
  case K::CONSTRUCTOR:
    break;
  case K::LAYOUT_CONSTRUCTOR:
    break;
  case K::DESTRUCTOR:
    break;
  case K::RANGER:
    break;
  case K::S_ANONYMOUS_FUNCTION:
    break;
  case K::S_DYNAMIC_CAPTURE:
    break;

  // CONTROL FLOW
  case K::RETURN:
    break;
  case K::BREAK:
    break;
  case K::CONTINUE:
    break;
  case K::FALLTHROUGH:
    break;
  case K::GOTO:
    break;
  case K::RANGE_OVER:
    break;

  // DECLARED TYPES
  case K::OBJECT:
    break;
  case K::ENUMERATION:
    break;

  // VALUES
  case K::TRUE:
    break;
  case K::FALSE:
    break;
  case K::INDETERMINATE:
    break;
  case K::VALUE:
    break;
  case K::INDEX:
    break;
  case K::DISCRIMINANT:
    break;
  case K::S_DISCRIMINANT_OF:
    break;
  case K::IN:
    break;
  case K::OUT:
    break;
  case K::THIS:
    break;
  case K::RESULT:
    break;
  case K::COMMAND_LINE_ARGUMENTS:
    break;
  case K::EXIT_CODE:
    break;
  case K::ADDRESS_BYTE_SIZE:
    break;
  case K::ADDRESS_BIT_DEPTH:
    break;
  case K::BITS_PER_BYTE:
    break;

  // BUILTIN TYPES
  case K::VOID:
    break;
  case K::NO_RETURN:
    break;
  case K::BOOLEAN:
    break;
  case K::WORD:
    break;
  case K::SIGNED:
    break;
  case K::UNSIGNED:
    break;
  case K::BFLOAT16:
    break;
  case K::BINARY16:
    break;
  case K::BINARY32:
    break;
  case K::BINARY64:
    break;
  case K::BINARY128:
    break;
  case K::UTF8:
    break;
  case K::C_CHAR:
    break;
  case K::C_UNSIGNED_CHAR:
    break;
  case K::C_SIGNED_CHAR:
    break;
  case K::C_WCHAR_T:
    break;
  case K::C_SHORT_INT:
    break;
  case K::C_UNSIGNED_SHORT_INT:
    break;
  case K::C_INT:
    break;
  case K::C_UNSIGNED_INT:
    break;
  case K::C_LONG_INT:
    break;
  case K::C_UNSIGNED_LONG_INT:
    break;
  case K::C_LONG_LONG_INT:
    break;
  case K::C_UNSIGNED_LONG_LONG_INT:
    break;
  case K::C_FLOAT:
    break;
  case K::C_DOUBLE:
    break;
  case K::C_LONG_DOUBLE:
    break;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    break;
  case K::FIRST_VARIADIC_ARGUMENT:
    break;
  case K::S_FIRST_VARIADIC_ARGUMENT_OF:
    break;
  case K::NEXT_VARIADIC_ARGUMENT:
    break;
  case K::S_NEXT_VARIADIC_ARGUMENT_OF:
    break;

  // SCOPES
  case K::IF:
    break;
  case K::ELSE_IF:
    break;
  case K::ELSE:
    break;
  case K::MATCH:
    break;
  case K::INLINE_MATCH:
    break;
  case K::SWITCH:
    break;
  case K::CASE:
    break;
  case K::DEFAULT:
    break;
  case K::FOR:
    break;
  case K::WHILE:
    break;
  case K::SCOPE:
    break;
  case K::INLINE_SCOPE:
    break;
  case K::BLOCK:
    break;
  case K::INLINE_BLOCK:
    break;

  // RANGES
  case K::RANGE:
    break;
  case K::CONTAINS:
    break;
  case K::S_CONTAINS_VALUE:
    break;
  case K::S_SEQUENCE:
    break;
  case K::S_SEQUENCE_STEP_ADD:
    break;
  case K::S_SEQUENCE_STEP_SUBTRACT:
    break;
  case K::S_SEQUENCE_STEP_MULTIPLY:
    break;
  case K::S_SEQUENCE_STEP_DIVIDE:
    break;
  case K::S_SEQUENCE_STEP_MODULUS:
    break;
  case K::S_SEQUENCE_WHILE_LESS:
    break;
  case K::S_SEQUENCE_WHILE_GREATER:
    break;
  case K::S_SEQUENCE_WHILE_LESS_EQUAL:
    break;
  case K::S_SEQUENCE_WHILE_GREATER_EQUAL:
    break;
  case K::S_SEQUENCE_WHILE_EQUAL:
    break;
  case K::S_SEQUENCE_WHILE_NOT_EQUAL:
    break;

  // ACCESS MODIFIERS
  case K::PRIVATE:
    break;
  case K::PROTECTED:
    break;
  case K::EXPORT:
    break;

  // TABLE GRAPH
  case K::IMPORT:
    break;
  case K::USE:
    break;
  case K::USE_TABLE:
    break;
  case K::TABLE:
    break;
  case K::MODULE:
    break;
  case K::S_MODULE_ROOT:
    break;

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    break;
  case K::DEBUG_TRAP:
    break;

  // HINTS
  case K::UNREACHABLE:
    break;
  case K::ASSUME:
    break;

  // STATEMENT ATTRIBUTES
  case K::OPAQUE:
    break;
  case K::GLOBAL:
    break;
  case K::STATIC:
    break;
  case K::STATIC_CAPTURE:
    break;
  case K::EVALUATE_IMMEDIATELY:
    break;
  case K::MAY_PARENT:
    break;
  case K::PARENT:
    break;
  case K::ABSTRACT:
    break;
  case K::VIRTUAL:
    break;
  case K::OVERRIDE:
    break;
  case K::POSITION:
    break;
  case K::INLINE:
    break;
  case K::MANGLED_NAME:
    break;
  case K::S_MANGLED_NAME_OF:
    break;
  case K::PACK:
    break;
  case K::USER:
    break;
  case K::LABEL:
    break;
  case K::TEMPLATE:
    break;
  case K::LIKELY:
    break;
  case K::UNLIKELY:
    break;
  case K::DEPRECIATED:
    break;
  case K::MAY_SHALLOW_COPY:
    break;

  // EXPRESSIONS
  case K::QUOTE:
    break;
  case K::EXPRESSION:
    break;
  case K::EXPAND:
    break;
  case K::S_EXPAND_TOP_STATEMENT:
    break;
  case K::S_EXPAND_TABLE_STATEMENT:
    break;
  case K::S_EXPAND_OBJECT_STATEMENT:
    break;
  case K::S_EXPAND_LOCAL_STATEMENT:
    break;
  case K::S_EXPAND_LVALUE:
    break;
  case K::S_EXPAND_RVALUE:
    break;
  case K::S_EXPAND_REFLECTION:
    break;
  case K::S_EXPAND_ARGUMENT:
    break;
  case K::S_EXPAND_PARAMETER:
    break;
  case K::S_EXPAND_ENUMERATOR:
    break;
  case K::S_EXPAND_PATH:
    break;
  case K::S_EXPAND_NAME:
    break;
  case K::S_EXPAND_SEQUENCE_STAGE:
    break;
  case K::S_EXPAND_VIGNETTE:
    break;
  case K::S_EXPAND_VIGNETTE_RVALUE:
    break;
  case K::S_EXPAND_ARM:
    break;
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    break;

  // REFLECTIONS
  case K::S_REFLECT:
    break;
  case K::S_MEMBER_OF:
    break;
  case K::BYTE_SIZE:
    break;
  case K::S_BYTE_SIZE_OF:
    break;
  case K::BIT_DEPTH:
    break;
  case K::S_BIT_DEPTH_OF:
    break;
  case K::ELEMENT_COUNT:
    break;
  case K::S_ELEMENT_COUNT_OF:
    break;
  case K::NAME:
    break;
  case K::S_NAME_OF:
    break;
  case K::LINE:
    break;
  case K::S_LINE_OF:
    break;
  case K::COLUMN:
    break;
  case K::S_COLUMN_OF:
    break;
  case K::IS:
    break;
  case K::S_VALUE_IS:
    break;
  case K::HOLDS:
    break;
  case K::S_VALUE_HOLDS:
    break;
  case K::TYPE:
    break;
  case K::S_TYPE_OF:
    break;
  case K::SYMBOL:
    break;
  case K::S_SYMBOL_OF:
    break;
  case K::LAYOUT:
    break;
  case K::S_LAYOUT_OF:
    break;
  case K::SIGNATURE:
    break;
  case K::S_SIGNATURE_OF:
    break;

  case K::I_LAST:
    break;
  }
  RQ_UNREACHABLE();
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
  const bool is_ok = this->situateBranch(
      situation, expression, branch0_situation, branch0, 0, "first branch");
  if (branch0.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount(situation, expression, 1);
    return false;
  }
  return is_ok;
}

} // namespace rq
