// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/context.hpp>
#include <requite/numeric.hpp>
#include <requite/source_location.hpp>

#include <llvm/Support/FormatVariadic.h>

namespace requite {

template <requite::Situation SITUATION_PARAM>
void Situator::situateExpression(requite::Expression &expression) {
  using namespace requite;
  using O = Opcode;
  using S = Situation;
  constexpr S SP = SITUATION_PARAM;
  switch (const O opcode = expression.getOpcode()) {
  case O::__NONE:
    REQUITE_UNREACHABLE();

  // HANDLES
  case O::__LOCAL_HANDLE:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;

  // LITERALS
  case O::__INTEGER_LITERAL:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;
  case O::__FLOAT_LITERAL:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;
  case O::__STRING_LITERAL:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;
  case O::__CODEUNIT_LITERAL:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;
  case O::__IDENTIFIER_LITERAL:
    REQUITE_ASSERT(!expression.getHasBranch());
    break;

  // ERRORS
  case O::__ERROR:
    REQUITE_UNREACHABLE();
    break;

  // SITUATIONAL
  case O::_CLOVEN:
    if constexpr (!getCanBeSituation<SP>(O::_CLOVEN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateClovenExpression<SP>(expression);
    }
    break;
  case O::_COLON:
    if constexpr (!getCanBeSituation<SP>(O::_COLON)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateColonExpression<SP>(expression);
    }
    break;
  case O::_INFERENCE:
    if constexpr (!getCanBeSituation<SP>(O::_INFERENCE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // LOGICAL
  case O::_LOGICAL_AND:
    if constexpr (!getCanBeSituation<SP>(O::_LOGICAL_AND)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_LOGICAL_OR:
    if constexpr (getCanBeSituation<SP>(O::_LOGICAL_OR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_LOGICAL_COMPLEMENT:
    if constexpr (getCanBeSituation<SP>(O::_LOGICAL_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // COMPARISON
  case O::_GREATER:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_GREATER_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_LESS:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_LESS_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_NOT_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;

  // APPLY
  case O::BAKE:
    if constexpr (!getCanBeSituation<SP>(O::BAKE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBakeExpression<SP>(expression);
    }
    break;
  case O::_EXTEND:
    if constexpr (!getCanBeSituation<SP>(O::_EXTEND)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE, S::VALUE>(expression);
    }
    break;
  case O::_BINDING:
    if constexpr (!getCanBeSituation<SP>(O::_BINDING)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::NAME, S::VALUE>(expression);
    }
    break;
  case O::_ASCRIBE_TYPE:
    if constexpr (!getCanBeSituation<SP>(O::_ASCRIBE_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAscribeTypeExpression<SITUATION_PARAM>(expression);
    }
    break;
  case O::_ASCRIBE_STATEMENT:
    if constexpr (!getCanBeSituation<SP>(O::_ASCRIBE_STATEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAscribeStatementExpression<SITUATION_PARAM>(expression);
    }
    break;
  case O::_CAST:
    if constexpr (!getCanBeSituation<SP>(O::_CAST)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_IDENTIFY:
    if constexpr (!getCanBeSituation<SP>(O::_IDENTIFY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateIdentifyExpression<SP>(expression);
    }
    break;

  // ARITHMETIC
  case O::_ADD:
    if constexpr (!getCanBeSituation<SP>(O::_ADD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_SUBTRACT:
    if constexpr (!getCanBeSituation<SP>(O::_SUBTRACT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_MULTIPLY:
    if constexpr (!getCanBeSituation<SP>(O::_MULTIPLY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_DIVIDE:
    if constexpr (!getCanBeSituation<SP>(O::_DIVIDE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_MODULUS:
    if constexpr (!getCanBeSituation<SP>(O::_MODULUS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_NEGATE:
    if constexpr (!getCanBeSituation<SP>(O::_NEGATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // BITWISE
  case O::_BITWISE_CAST:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_CAST)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_OR:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_OR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_AND:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_AND)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_XOR:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_XOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_COMPLEMENT:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_SHIFT_LEFT:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_BITWISE_SHIFT_RIGHT:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::BITWISE_ROTATE_LEFT:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::BITWISE_ROTATE_RIGHT:
    if constexpr (!getCanBeSituation<SP>(O::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // MEMORY
  case O::_CONCATINATE:
    if constexpr (!getCanBeSituation<SP>(O::_CONCATINATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateConcatinateExpression<SP>(expression);
    }
    break;
  case O::SINGLETON:
    if constexpr (!getCanBeSituation<SP>(O::SINGLETON)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_SINGLETON_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SINGLETON_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SINGLETON_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_SINGLETON_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
  case O::CONTENT:
    if constexpr (!getCanBeSituation<SP>(O::CONTENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_CONTENT_OF:
    if constexpr (!getCanBeSituation<SP>(O::_CONTENT_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_CONTENT_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_CONTENT_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;
  case O::ADDRESS:
    if constexpr (!getCanBeSituation<SP>(O::ADDRESS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_ADDRESS_OF:
    if constexpr (!getCanBeSituation<SP>(O::_ADDRESS_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_ADDRESS_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_ADDRESS_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;
  case O::REFER:
    if constexpr (!getCanBeSituation<SP>(O::REFER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_REFERENCE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_REFERENCE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_REFERENCE_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_REFERENCE_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;
  case O::VIEW:
    if constexpr (!getCanBeSituation<SP>(O::VIEW)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_VIEW_OF:
    if constexpr (!getCanBeSituation<SP>(O::_VIEW_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_VIEW_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_VIEW_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;
  case O::FIXED_VIEW:
    if constexpr (!getCanBeSituation<SP>(O::FIXED_VIEW)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_FIXED_VIEW_OF:
    if constexpr (!getCanBeSituation<SP>(O::_FIXED_VIEW_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_FIXED_VIEW_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_FIXED_VIEW_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;
  case O::SLICE:
    if constexpr (!getCanBeSituation<SP>(O::SLICE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SLICE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SLICE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SLICE_OF_ASCRIBED:
    if constexpr (!getCanBeSituation<SP>(O::_FIXED_VIEW_OF_ASCRIBED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 3, S::VALUE, S::VALUE, S::TYPE_ATTRIBUTE>(
          expression);
    }
    break;

  // ASSIGNMENT
  case O::_ASSIGN:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignExpression<SP>(expression);
    }
    break;
  case O::_ASSIGN_ADD:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN_ADD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SP>(expression,
                                                  requite::Opcode::_ADD);
    }
    break;
  case O::_ASSIGN_SUBTRACT:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN_SUBTRACT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SP>(expression,
                                                  requite::Opcode::_SUBTRACT);
    }
    break;
  case O::_ASSIGN_MULTIPLY:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN_MULTIPLY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SP>(expression,
                                                  requite::Opcode::_MULTIPLY);
    }
    break;
  case O::_ASSIGN_DIVIDE:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN_DIVIDE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SP>(expression,
                                                  requite::Opcode::_DIVIDE);
    }
    break;
  case O::_ASSIGN_MODULUS:
    if constexpr (!getCanBeSituation<SP>(O::_ASSIGN_MODULUS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SP>(expression,
                                                  requite::Opcode::_MODULUS);
    }
    break;

  // MOVE SEMANTICS
  case O::COPY:
    if constexpr (!getCanBeSituation<SP>(O::COPY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_COPY_OF:
    if constexpr (!getCanBeSituation<SP>(O::_COPY_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::MOVE:
    if constexpr (!getCanBeSituation<SP>(O::MOVE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_MOVE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_MOVE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::SWAP:
    if constexpr (!getCanBeSituation<SP>(O::SWAP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // SUBTYPE
  case O::_ARRAY:
    if constexpr (!getCanBeSituation<SP>(O::_ARRAY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_REFERENCE:
    if constexpr (!getCanBeSituation<SP>(O::_REFERENCE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_POINTER:
    if constexpr (!getCanBeSituation<SP>(O::_POINTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_FAT_POINTER:
    if constexpr (!getCanBeSituation<SP>(O::_FAT_POINTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_ARRAY_POINTER:
    if constexpr (!getCanBeSituation<SP>(O::_ARRAY_POINTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // TYPE MODIFIER
  case O::MUTABLE:
    if constexpr (!getCanBeSituation<SP>(O::MUTABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::CONSTANT:
    if constexpr (!getCanBeSituation<SP>(O::CONSTANT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::VOLATILE:
    if constexpr (!getCanBeSituation<SP>(O::VOLATILE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::ATOMIC:
    if constexpr (!getCanBeSituation<SP>(O::ATOMIC)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::NULL_TERMINATED:
    if constexpr (!getCanBeSituation<SP>(O::NULL_TERMINATED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::OWNING:
    if constexpr (!getCanBeSituation<SP>(O::OWNING)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::MAY_DISCARD:
    if constexpr (!getCanBeSituation<SP>(O::MAY_DISCARD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // PARAMETER RULES
  case O::_POSITIONAL_PARAMETERS_END:
    if constexpr (!getCanBeSituation<SP>(O::_POSITIONAL_PARAMETERS_END)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_NAMED_PARAMETERS_BEGIN:
    if constexpr (!getCanBeSituation<SP>(O::_NAMED_PARAMETERS_BEGIN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // TRIPS
  case O::_TUPLE:
    if constexpr (!getCanBeSituation<SP>(O::_TUPLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::ARGUMENT>(expression);
    }
    break;
  case O::_LAYOUT:
    if constexpr (!getCanBeSituation<SP>(O::_LAYOUT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::PARAMETER>(expression);
    }
    break;
  case O::_NULL:
    if constexpr (!getCanBeSituation<SP>(O::_NULL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_NULL_TYPE:
    if constexpr (!getCanBeSituation<SP>(O::_NULL_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_STRUCTURED_BINDING:
    if constexpr (!getCanBeSituation<SP>(O::_STRUCTURED_BINDING)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<SP, 2, S::BINDING, S::VALUE>(
          expression);
    }
    break;
  case O::_IGNORE:
    if constexpr (!getCanBeSituation<SP>(O::_IGNORE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SPECIALIZATION:
    if constexpr (!getCanBeSituation<SP>(O::_SPECIALIZATION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::ARGUMENT>(expression);
    }
    break;

  // PROCEDURES
  case O::_CALL:
    if constexpr (!getCanBeSituation<SP>(O::_CALL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::ARGUMENT>(expression);
    }
    break;
  case O::_INDEX:
    if constexpr (!getCanBeSituation<SP>(O::_INDEX)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE>(expression);
    }
    break;
  case O::_SIGNATURE:
    if constexpr (!getCanBeSituation<SP>(O::_SIGNATURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::PARAMETER>(expression);
    }
    break;
  case O::DESTROY:
    if constexpr (!getCanBeSituation<SP>(O::DESTROY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_DESTROY_VALUE:
    if constexpr (!getCanBeSituation<SP>(O::_DESTROY_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::DROP:
    if constexpr (!getCanBeSituation<SP>(O::DROP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_DROP_VALUE:
    if constexpr (!getCanBeSituation<SP>(O::_DROP_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::ENTRY_POINT:
    if constexpr (!getCanBeSituation<SP>(O::ENTRY_POINT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::FUNCTION:
    if constexpr (!getCanBeSituation<SP>(O::FUNCTION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::NAME, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::CONSTRUCTOR:
    if constexpr (!getCanBeSituation<SP>(O::CONSTRUCTOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::VALUE, S::VALUE,
                                  S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::DESTRUCTOR:
    if constexpr (!getCanBeSituation<SP>(O::DESTRUCTOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::RANGER:
    if constexpr (!getCanBeSituation<SP>(O::RANGER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::INDEXER:
    if constexpr (!getCanBeSituation<SP>(O::INDEXER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::_ANONYMOUS_FUNCTION:
    if constexpr (!getCanBeSituation<SP>(O::_ANONYMOUS_FUNCTION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::CAPTURE, S::VALUE,
                                  S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::CAPTURE:
    if constexpr (!getCanBeSituation<SP>(O::CAPTURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::VALUE>(expression);
    }
    break;

  // CONTROL FLOW
  case O::RETURN:
    if constexpr (!getCanBeSituation<SP>(O::RETURN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BREAK:
    if constexpr (!getCanBeSituation<SP>(O::BREAK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::NAME>(expression);
    }
    break;
  case O::CONTINUE:
    if constexpr (!getCanBeSituation<SP>(O::CONTINUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::FALLTHROUGH:
    if constexpr (!getCanBeSituation<SP>(O::FALLTHROUGH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::GOTO:
    if constexpr (!getCanBeSituation<SP>(O::GOTO)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::EXIT:
    if constexpr (!getCanBeSituation<SP>(O::EXIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::LOOP_BODY:
    if constexpr (!getCanBeSituation<SP>(O::LOOP_BODY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // SYMBOLS
  case O::OBJECT:
    if constexpr (!getCanBeSituation<SP>(O::OBJECT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::NAME, S::VALUE,
                                  S::OBJECT_STATEMENT>(expression);
    }
    break;
  case O::VARIANT:
    if constexpr (!getCanBeSituation<SP>(O::VARIANT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::NAME, S::ALTERNATIVE>(expression);
    }
    break;
  case O::TABLE:
    if constexpr (!getCanBeSituation<SP>(O::TABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateTableExpression<SP>(expression);
    }
    break;
  case O::USE:
    if constexpr (!getCanBeSituation<SP>(O::USE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // VALUES
  case O::TRUE:
    if constexpr (!getCanBeSituation<SP>(O::TRUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::FALSE:
    if constexpr (!getCanBeSituation<SP>(O::FALSE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::INDETERMINATE:
    if constexpr (!getCanBeSituation<SP>(O::INDETERMINATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::THIS:
    if constexpr (!getCanBeSituation<SP>(O::THIS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::RESULT:
    if constexpr (!getCanBeSituation<SP>(O::RESULT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::ARGUMENTS:
    if constexpr (!getCanBeSituation<SP>(O::ARGUMENTS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::IN:
    if constexpr (!getCanBeSituation<SP>(O::IN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::OUT:
    if constexpr (!getCanBeSituation<SP>(O::OUT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::ADDRESS_SIZE:
    if constexpr (!getCanBeSituation<SP>(O::ADDRESS_SIZE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::ADDRESS_DEPTH:
    if constexpr (!getCanBeSituation<SP>(O::ADDRESS_DEPTH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BITS_PER_BYTE:
    if constexpr (!getCanBeSituation<SP>(O::BITS_PER_BYTE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // BUILTIN TYPES
  case O::VOID:
    if constexpr (!getCanBeSituation<SP>(O::VOID)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BOOLEAN:
    if constexpr (!getCanBeSituation<SP>(O::BOOLEAN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::WORD:
    if constexpr (!getCanBeSituation<SP>(O::WORD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateDepthTypeExpression<SP>(expression);
    }
    break;
  case O::SIGNED:
    if constexpr (!getCanBeSituation<SP>(O::SIGNED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateDepthTypeExpression<SP>(expression);
    }
    break;
  case O::UNSIGNED:
    if constexpr (!getCanBeSituation<SP>(O::UNSIGNED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateDepthTypeExpression<SP>(expression);
    }
    break;
  case O::BFLOAT16:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BINARY16:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BINARY32:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BINARY64:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::BINARY128:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::UTF8:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // VARIADIC ARGUMENTS
  case O::VARIADIC_ARGUMENTS:
    if constexpr (!getCanBeSituation<SP>(O::BFLOAT16)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::FIRST_VARIADIC_ARGUMENT:
    if constexpr (!getCanBeSituation<SP>(O::FIRST_VARIADIC_ARGUMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_FIRST_VARIADIC_ARGUMENT_OF:
    if constexpr (!getCanBeSituation<SP>(O::_FIRST_VARIADIC_ARGUMENT_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::NEXT_VARIADIC_ARGUMENT:
    if constexpr (!getCanBeSituation<SP>(O::NEXT_VARIADIC_ARGUMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_NEXT_VARIADIC_ARGUMENT_OF:
    if constexpr (!getCanBeSituation<SP>(O::_FIRST_VARIADIC_ARGUMENT_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // SCOPES
  case O::IF:
    if constexpr (!getCanBeSituation<SP>(O::IF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::ELSE_IF:
    if constexpr (!getCanBeSituation<SP>(O::ELSE_IF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::ELSE:
    if constexpr (!getCanBeSituation<SP>(O::ELSE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::SWITCH:
    if constexpr (!getCanBeSituation<SP>(O::SWITCH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<SP, 1, S::VALUE, S::CASE,
                                          S::LAST_CASE>(expression);
    }
    break;
  case O::MATCH:
    if constexpr (!getCanBeSituation<SP>(O::MATCH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<SP, 1, S::VALUE, S::CASE,
                                          S::LAST_CASE>(expression);
    }
    break;
  case O::CASE:
    if constexpr (!getCanBeSituation<SP>(O::CASE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::DEFAULT:
    if constexpr (!getCanBeSituation<SP>(O::DEFAULT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::LOOP:
    if constexpr (!getCanBeSituation<SP>(O::LOOP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::VALUE, S::LOCAL_STATEMENT>(
          expression);
    }
    break;
  case O::SCOPE:
    if constexpr (!getCanBeSituation<SP>(O::SCOPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateScopeExpression<SP>(expression);
    }
    break;
  case O::BLOCK:
    if constexpr (!getCanBeSituation<SP>(O::SCOPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBlockExpression<SP>(expression);
    }
    break;
  case O::_INLINE_BLOCK:
    if constexpr (!getCanBeSituation<SP>(O::_INLINE_BLOCK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::_INLINE_SCOPE:
    if constexpr (!getCanBeSituation<SP>(O::_INLINE_SCOPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::LOCAL_STATEMENT>(expression);
    }
    break;

  // RANGES
  case O::RANGE:
    if constexpr (!getCanBeSituation<SP>(O::RANGE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_NOT_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_GREATER:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_GREATER_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_GREATER_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_LESS:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_LESS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LIMIT_RANGE_LESS_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_LIMIT_RANGE_LESS_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_LONG_RANGE:
    if constexpr (!getCanBeSituation<SP>(O::_LONG_RANGE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::LONG_RANGE_STAGE>(expression);
    }
    break;
  case O::FOR:
    if constexpr (!getCanBeSituation<SP>(O::FOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::DO:
    if constexpr (!getCanBeSituation<SP>(O::DO)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::WHILE:
    if constexpr (!getCanBeSituation<SP>(O::WHILE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::UNTIL:
    if constexpr (!getCanBeSituation<SP>(O::UNTIL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::FOREVER:
    if constexpr (!getCanBeSituation<SP>(O::FOREVER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::STEP:
    if constexpr (!getCanBeSituation<SP>(O::STEP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::LOCAL_STATEMENT>(expression);
    }
    break;
  case O::WHEN:
    if constexpr (!getCanBeSituation<SP>(O::WHEN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_RANGE:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_RANGE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 2, S::SHORT_RANGE_STAGE>(expression);
    }
    break;
  case O::_SHORT_STEP_ADD:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_STEP_ADD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_STEP_SUBTRACT:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_STEP_SUBTRACT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_STEP_MULTIPLY:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_STEP_MULTIPLY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_STEP_DIVIDE:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_STEP_DIVIDE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_STEP_MODULUS:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_STEP_MODULUS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_LESS:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_LESS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_GREATER:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_LESS_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_LESS_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_GREATER_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_GREATER_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_SHORT_WHILE_NOT_EQUAL:
    if constexpr (!getCanBeSituation<SP>(O::_SHORT_WHILE_NOT_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  // ACCESS MODIFIERS
  case O::PRIVATE:
    if constexpr (!getCanBeSituation<SP>(O::PRIVATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::PROTECTED:
    if constexpr (!getCanBeSituation<SP>(O::PROTECTED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::EXPORT:
    if constexpr (!getCanBeSituation<SP>(O::EXPORT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // SYMBOL GRAPH
  case O::IMPORT:
    if constexpr (!getCanBeSituation<SP>(O::IMPORT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::STRING_LITERAL>(expression);
    }
    break;
  case O::USE_TABLE:
    if constexpr (!getCanBeSituation<SP>(O::USE_TABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::PATH>(expression);
    }
    break;

  // SOURCES
  case O::_MODULE_ROOT:
    if constexpr (!getCanBeSituation<SP>(O::_MODULE_ROOT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 1, S::STRING_LITERAL, S::TOP_STATEMENT>(
          expression);
    }
    break;

  // ERROR HANDLING AND DEBUGGING
  case O::ASSERT:
    if constexpr (!getCanBeSituation<SP>(O::ASSERT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssertExpression<SP>(expression);
    }
    break;
  case O::UNREACHABLE:
    if constexpr (!getCanBeSituation<SP>(O::UNREACHABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;

  // ATTRIBUTES
  case O::MAY_PARENT:
    if constexpr (!getCanBeSituation<SP>(O::MAY_PARENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::PARENT:
    if constexpr (!getCanBeSituation<SP>(O::PARENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::POSITION:
    if constexpr (!getCanBeSituation<SP>(O::POSITION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::INLINE:
    if constexpr (!getCanBeSituation<SP>(O::INLINE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::MANGLED_NAME:
    if constexpr (!getCanBeSituation<SP>(O::MANGLED_NAME)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateMangledNameExpression<SP>(expression);
    }
    break;
  case O::_MANGLED_NAME_OF:
    if constexpr (!getCanBeSituation<SP>(O::_MANGLED_NAME_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::PACK:
    if constexpr (!getCanBeSituation<SP>(O::PACK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::USER:
    if constexpr (!getCanBeSituation<SP>(O::USER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::LABEL:
    if constexpr (!getCanBeSituation<SP>(O::LABEL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::NAME>(expression);
    }
    break;
  case O::TEMPLATE:
    if constexpr (!getCanBeSituation<SP>(O::TEMPLATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SP, 0, S::PARAMETER>(expression);
    }
    break;

  // REFLECTIONS
  case O::_REFLECT:
    if constexpr (!getCanBeSituation<SP>(O::_REFLECT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateReflectExpression<SP>(expression);
    }
    break;
  case O::_MEMBER_OF:
    if constexpr (!getCanBeSituation<SP>(O::_MEMBER_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE, S::NAME>(expression);
    }
    break;
  case O::SIZE:
    if constexpr (!getCanBeSituation<SP>(O::SIZE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_SIZE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SIZE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::DEPTH:
    if constexpr (!getCanBeSituation<SP>(O::DEPTH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_DEPTH_OF:
    if constexpr (!getCanBeSituation<SP>(O::_DEPTH_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::COUNT:
    if constexpr (!getCanBeSituation<SP>(O::COUNT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_COUNT_OF:
    if constexpr (!getCanBeSituation<SP>(O::_COUNT_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::LENGTH:
    if constexpr (!getCanBeSituation<SP>(O::_LENGTH_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_LENGTH_OF:
    if constexpr (!getCanBeSituation<SP>(O::_LENGTH_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::SPAN_SIZE:
    if constexpr (!getCanBeSituation<SP>(O::SPAN_SIZE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_SPAN_SIZE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SPAN_SIZE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::SPAN_DEPTH:
    if constexpr (!getCanBeSituation<SP>(O::SPAN_DEPTH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_SPAN_DEPTH_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SPAN_DEPTH_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break; 
  case O::NAME:
    if constexpr (!getCanBeSituation<SP>(O::NAME)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_NAME_OF:
    if constexpr (!getCanBeSituation<SP>(O::_NAME_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::LINE:
    if constexpr (!getCanBeSituation<SP>(O::LINE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_LINE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_LINE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::COLUMN:
    if constexpr (!getCanBeSituation<SP>(O::COLUMN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_COLUMN_OF:
    if constexpr (!getCanBeSituation<SP>(O::_COLUMN_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::IS:
    if constexpr (!getCanBeSituation<SP>(O::IS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_VALUE_IS:
    if constexpr (!getCanBeSituation<SP>(O::_VALUE_IS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::GET:
    if constexpr (!getCanBeSituation<SP>(O::GET)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::_GET_VALUE_OF_VARIANT:
    if constexpr (!getCanBeSituation<SP>(O::_GET_VALUE_OF_VARIANT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::TYPE:
    if constexpr (!getCanBeSituation<SP>(O::TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_TYPE_OF:
    if constexpr (!getCanBeSituation<SP>(O::_TYPE_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::SYMBOL:
    if constexpr (!getCanBeSituation<SP>(O::SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_SYMBOL_OF:
    if constexpr (!getCanBeSituation<SP>(O::_SYMBOL_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;
  case O::DISCRIMINANT:
    if constexpr (!getCanBeSituation<SP>(O::DISCRIMINANT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SP>(expression);
    }
    break;
  case O::_DISCRIMINANT_OF:
    if constexpr (!getCanBeSituation<SP>(O::_DISCRIMINANT_OF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SP, S::VALUE>(expression);
    }
    break;

  case O::__LAST:
    REQUITE_UNREACHABLE();
  }
  if (expression.getIsConverging()) {
    for (requite::Expression &branch : expression.getBranchSubrange()) {
      if (expression.getOpcode() == branch.getOpcode()) {
        branch.mergeBranch();
      }
    }
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_PARAM>
void Situator::situateBranch(llvm::Twine log_context,
                             requite::Expression &outer, unsigned branch_i,
                             requite::Expression &branch) {
  const bool is_ok =
      requite::getCanBeSituation<BRANCH_SITUATION_PARAM>(branch.getOpcode());
  if (!is_ok) {
    this->getContext()
        .logErrorInvalidBranchSituation<SITUATION_PARAM,
                                        BRANCH_SITUATION_PARAM>(
            branch, outer.getOpcode(), branch.getOpcode(), branch_i,
            log_context);
    this->setNotOk();
    return;
  }
  this->situateExpression<BRANCH_SITUATION_PARAM>(branch);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateNullaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if (expression.getHasBranch()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine(requite::getDescription(SITUATION_PARAM)) +
            requite::getName(expression.getOpcode()) +
            " must not have branches");
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_PARAM>
void Situator::situateUnaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    1);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_PARAM>(
      "first branch", expression, 0, first);
  if (first.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    1);
    this->setNotOk();
    return;
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM>
void Situator::situateBinaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    2);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
      "first branch", expression, 0, first);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    2);
    this->setNotOk();
    return;
  }
  requite::Expression &second = first.getNext();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
      "second branch", expression, 1, second);
  if (second.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    2);
    this->setNotOk();
    return;
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM,
          requite::Situation BRANCH_SITUATION_C_PARAM>
inline void
Situator::situateTernaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    3);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
      "first branch", expression, 0, first);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    3);
    this->setNotOk();
    return;
  }
  requite::Expression &second = first.getNext();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
      "second branch", expression, 1, second);
  if (!second.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    3);
    this->setNotOk();
    return;
  }
  requite::Expression &third = second.getNext();
  this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_C_PARAM>(
      "third branch", expression, 2, third);
  if (third.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    3);
    this->setNotOk();
    return;
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
        "all branches", expression, branch_i++, branch);
  }
  if (branch_i < MIN_COUNT_PARAM) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, branch_i);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  do {
    if (!expression.getHasBranch()) {
      break;
    }
    requite::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, branch_i++, first);
    for (requite::Expression &branch : first.getNextSubrange()) {
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
          "second and subsequent branches", expression, branch_i++, branch);
    }
  } while (false);
  if (branch_i < MIN_COUNT_PARAM) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, MIN_COUNT_PARAM);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  do {
    if (!expression.getHasBranch()) {
      break;
    }
    requite::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, branch_i++, first);
    if (!first.getHasNext()) {
      break;
    }
    requite::Expression &second = first.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
        "second branch", expression, branch_i++, second);
    for (requite::Expression &branch : second.getNextSubrange()) {
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
          "third and subsequent branches", expression, branch_i++, branch);
    }
  } while (false);
  if (branch_i < MIN_COUNT_PARAM) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, MIN_COUNT_PARAM);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM,
          requite::Situation BRANCH_SITUATION_C_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  do {
    if (!expression.getHasBranch()) {
      break;
    }
    requite::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, branch_i++, first);
    if (!first.getHasNext()) {
      break;
    }
    requite::Expression &second = first.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_B_PARAM>(
        "second branch", expression, branch_i++, second);
    if (!second.getHasNext()) {
      break;
    }
    requite::Expression &third = second.getNext();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_C_PARAM>(
        "third branch", expression, branch_i++, second);
    for (requite::Expression &branch : third.getNextSubrange()) {
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
          "fourth and subsequent branches", expression, branch_i++, branch);
    }
  } while (false);
  if (branch_i < MIN_COUNT_PARAM) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, branch_i);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM,
          requite::Situation BRANCH_SITUATION_LAST_PARAM>
void Situator::situateNaryWithLastExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    if (!branch.getHasNext()) {
      this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_LAST_PARAM>(
          "last branch", expression, branch_i++, branch);
      break;
    }
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_N_PARAM>(
        "first to penultimate branch", expression, branch_i++, branch);
  }
  if (branch_i < MIN_COUNT_PARAM) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, branch_i);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM,
          requite::Situation BRANCH_SITUATION_LAST_PARAM>
void Situator::situateNaryWithLastExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  unsigned branch_i = 0;
  do {
    if (!expression.getHasBranch()) {
      break;
    }
    requite::Expression &first = expression.getBranch();
    this->situateBranch<SITUATION_PARAM, BRANCH_SITUATION_A_PARAM>(
        "first branch", expression, branch_i++, first);
    for (requite::Expression &branch : first.getNextSubrange()) {
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
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, branch_i);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateClovenExpression(requite::Expression &expression) {
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    1);
    this->setNotOk();
    return;
  }
  requite::Expression &branch = expression.getBranch();
  this->situateBranch<SITUATION_PARAM, SITUATION_PARAM>("first branch",
                                                        expression, 0, branch);
  if (branch.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    1);
    this->setNotOk();
    return;
  }
  expression.mergeBranch();
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateColonExpression(requite::Expression &expression) {
  if constexpr (SITUATION_PARAM == requite::Situation::DESTINATION ||
                SITUATION_PARAM == requite::Situation::PARAMETER ||
                SITUATION_PARAM == requite::Situation::BINDING ||
                SITUATION_PARAM == requite::Situation::ALTERNATIVE) {
    this->situateBinaryExpression<SITUATION_PARAM, requite::Situation::NAME,
                                  requite::Situation::VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_BINDING);
  } else if constexpr (SITUATION_PARAM == requite::Situation::VALUE ||
                       SITUATION_PARAM == requite::Situation::ARGUMENT) {
    this->situateBinaryExpression<SITUATION_PARAM, requite::Situation::VALUE,
                                  requite::Situation::VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_CAST);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateBakeExpression(requite::Expression &expression) {
  if constexpr (SITUATION_PARAM == requite::Situation::VALUE ||
                SITUATION_PARAM == requite::Situation::ARGUMENT) {
    this->situateUnaryExpression<SITUATION_PARAM, requite::Situation::VALUE>(
        expression);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STATEMENT_ATTRIBUTE) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateAscribeTypeExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ASCRIBE_TYPE);
  constexpr requite::Situation ASCRIBED_SITUATION =
      (SITUATION_PARAM == requite::Situation::REFLECTION)
          ? requite::Situation::ASCRIBED_REFLECTION
          : SITUATION_PARAM;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, 2);
    this->setNotOk();
    return;
  }
  requite::Expression &branch = expression.getBranch();
  if (requite::getCanBeAttribute(branch.getOpcode())) {
    if (!branch.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, 2);
      this->setNotOk();
      return;
    }
    unsigned branch_i = 0;
    this->situateBranch<SITUATION_PARAM, requite::Situation::TYPE_ATTRIBUTE>(
        "first to penultimate branch", expression, branch_i++, branch);
    requite::Expression *previous_ptr = &branch;
    for (requite::Expression &next : branch.getNextSubrange()) {
      if (!next.getHasNext()) {
        this->situateBranch<SITUATION_PARAM, ASCRIBED_SITUATION>(
            "last branch", expression, branch_i++, next);
        requite::Expression &previous = requite::getRef(previous_ptr);
        next.setNext(expression.replaceBranch(previous.popNext()));
        break;
      }
      this->situateBranch<SITUATION_PARAM, requite::Situation::TYPE_ATTRIBUTE>(
          "first to penultimate branch", expression, branch_i++, next);
      previous_ptr = &next;
    }
  } else {
    this->situateNaryExpression<SITUATION_PARAM, 2, ASCRIBED_SITUATION,
                                requite::Situation::TYPE_ATTRIBUTE>(expression);
  }
  if (!expression.getHasBranch()) {
    return;
  }
  requite::Expression &unascribed = expression.getBranch();
  if (unascribed.getOpcode() == requite::Opcode::_EXTEND) {
    if (!unascribed.getHasBranch()) {
      return;
    }
    requite::Expression &extended = unascribed.getBranch();
    if (extended.getOpcode() == requite::Opcode::_ASCRIBE_TYPE) {
      requite::Expression &last = extended.getLastBranch();
      last.setNext(unascribed.popNext());
      expression.mergeBranch();
    } else {
      requite::Expression &first_attribute = unascribed.popNext();
      requite::Expression &branch = expression.mergeAndPopBranch();
      branch.setOpcode(requite::Opcode::_ASCRIBE_TYPE);
      branch.setBranch(expression.replaceBranch(branch));
      branch.setNext(extended.replaceNext(first_attribute));
    }
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateAscribeStatementExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ASCRIBE_STATEMENT);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, 2);
    this->setNotOk();
    return;
  }
  requite::Expression &branch = expression.getBranch();
  if (requite::getCanBeAttribute(branch.getOpcode())) {
    if (!branch.getHasNext()) {
      this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, 2);
      this->setNotOk();
      return;
    }
    unsigned branch_i = 0;
    this->situateBranch<SITUATION_PARAM,
                        requite::Situation::STATEMENT_ATTRIBUTE>(
        "first to penultimate branch", expression, branch_i++, branch);
    requite::Expression *previous_ptr = &branch;
    for (requite::Expression &next : branch.getNextSubrange()) {
      if (!next.getHasNext()) {
        this->situateBranch<SITUATION_PARAM, SITUATION_PARAM>(
            "last branch", expression, branch_i++, next);
        requite::Expression &previous = requite::getRef(previous_ptr);
        next.setNext(expression.replaceBranch(previous.popNext()));
        break;
      }
      this->situateBranch<SITUATION_PARAM,
                          requite::Situation::STATEMENT_ATTRIBUTE>(
          "first to penultimate branch", expression, branch_i++, next);
      previous_ptr = &next;
    }
  } else {
    this->situateNaryExpression<SITUATION_PARAM, 2, SITUATION_PARAM,
                                requite::Situation::STATEMENT_ATTRIBUTE>(
        expression);
  }
  if (!expression.getHasBranch()) {
    return;
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateIdentifyExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_IDENTIFY);
  this->situateUnaryExpression<SITUATION_PARAM, requite::Situation::VALUE>(
      expression);
  requite::Expression &branch = expression.getBranch();
  if (branch.getOpcode() != requite::Opcode::__STRING_LITERAL) {
    return;
  }
  requite::SavedString text = branch.getDataText();
  expression.mergeBranch();
  expression.changeOpcode(requite::Opcode::__IDENTIFIER_LITERAL);
  expression.setDataText(text);
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateConcatinateExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_CONCATINATE);
  this->situateNaryExpression<SITUATION_PARAM, 2,
                              requite::Situation::STRING_LITERAL>(expression);
  requite::Expression &first_branch = expression.getBranch();
  for (requite::Expression &branch : first_branch.getHorizontalSubrange()) {
    if (branch.getOpcode() == requite::Opcode::__STRING_LITERAL) {
      while (branch.getHasNext() && branch.getNext().getOpcode() ==
                                        requite::Opcode::__STRING_LITERAL) {
        requite::Expression &next = branch.popNext();
        llvm::StringRef cur_text = branch.getDataText().getString();
        llvm::StringRef next_text = next.getDataText().getString();
        std::string concatinated_text =
            llvm::formatv("{}{}", cur_text, next_text);
        requite::SavedString saved_text =
            this->getContext().saveString(concatinated_text);
        branch.changeDataText(saved_text);
        branch.setNextPtr(next.getNextPtr());
        requite::Expression::deleteExpression(next);
      }
    }
  }
  if (!first_branch.getHasNext()) {
    expression.mergeBranch();
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateAssignExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ASSIGN);
  unsigned branch_i = 0;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    2);
    this->setNotOk();
    return;
  }
  requite::Expression &destination = expression.getBranch();
  this->situateBranch<SITUATION_PARAM, requite::Situation::DESTINATION>(
      "first branch", expression, branch_i++, destination);
  if (!destination.getHasNext()) {
    this->getContext().logErrorNotExactBranchCount<SITUATION_PARAM>(expression,
                                                                    2);
    this->setNotOk();
    return;
  }
  requite::Expression &value = destination.getNext();
  this->situateBranch<SITUATION_PARAM, requite::Situation::VALUE>(
      "second branch", expression, branch_i++, value);
  switch (const requite::Opcode opcode = destination.getOpcode()) {
  case requite::Opcode::_NULL:
    destination.changeOpcode(requite::Opcode::_IGNORE);
    expression.mergeBranch();
    break;
  case requite::Opcode::_TUPLE:
    destination.changeOpcode(requite::Opcode::_STRUCTURED_BINDING);
    expression.mergeBranch();
    break;
  default:
    break;
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateAssignArithmeticExpression(requite::Expression &expression,
                                            requite::Opcode arithmetic_opcode) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  this->situateBinaryExpression<SITUATION_PARAM,
                                requite::Situation::DESTINATION,
                                requite::Situation::VALUE>(expression);
  expression.changeOpcode(requite::Opcode::_ASSIGN);
  if (!expression.getHasBranch()) {
    return;
  }
  requite::Expression &destination = expression.getBranch();
  if (!destination.getHasNext()) {
    return;
  }
  requite::Expression &value = destination.popNext();
  requite::Expression &arithmetic_expression =
      requite::Expression::makeOperation(arithmetic_opcode);
  arithmetic_expression.setSource(value);
  requite::Expression &destination_copy =
      requite::Expression::copyExpression(destination);
  destination.setNext(arithmetic_expression);
  arithmetic_expression.setBranch(destination_copy);
  destination_copy.setNext(value);
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateTableExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::TABLE);
  this->situateNaryExpression<SITUATION_PARAM, 1, requite::Situation::PATH,
                              requite::Situation::TABLE_STATEMENT>(expression);
  requite::Expression &path_expression = expression.getBranch();
  if (path_expression.getOpcode() == requite::Opcode::_MEMBER_OF) {
    requite::Expression *table_body_ptr = path_expression.popNextPtr();
    requite::Expression &first_name_expression = path_expression.popBranch();
    std::ignore = expression.replaceBranch(first_name_expression);
    requite::Expression &second_name_expression =
        first_name_expression.replaceNext(path_expression);
    path_expression.changeOpcode(requite::Opcode::TABLE);
    path_expression.setBranch(second_name_expression);
    requite::Expression *previous_name_expression_ptr = &first_name_expression;
    for (requite::Expression &name_expression :
         second_name_expression.getHorizontalSubrange()) {
      requite::Expression &table_expression =
          requite::Expression::makeOperation(requite::Opcode::TABLE);
      table_expression.setSourceInsertedAfter(expression);
      table_expression.setBranch(name_expression);
      std::ignore = requite::getRef(previous_name_expression_ptr)
                        .replaceNext(table_expression);
      if (!name_expression.getHasNext()) {
        name_expression.setNextPtr(table_body_ptr);
        break;
      } else {
        previous_name_expression_ptr = &name_expression;
      }
    }
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateDepthTypeExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SIGNED ||
                 expression.getOpcode() == requite::Opcode::UNSIGNED ||
                 expression.getOpcode() == requite::Opcode::WORD);
  if (!expression.getHasBranch()) {
    requite::Expression &first =
        requite::Expression::makeOperation(requite::Opcode::ADDRESS_DEPTH);
    first.setSourceInsertedAfter(expression);
    expression.setBranch(first);
  }
  this->situateUnaryExpression<SITUATION_PARAM, requite::Situation::VALUE>(
      expression);
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateScopeExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SCOPE);
  this->situateNaryExpression<SITUATION_PARAM, 0,
                              requite::Situation::LOCAL_STATEMENT>(expression);
  if constexpr (SITUATION_PARAM == requite::Situation::VALUE) {
    expression.changeOpcode(requite::Opcode::_INLINE_SCOPE);
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateBlockExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::BLOCK);
  this->situateNaryExpression<SITUATION_PARAM, 0,
                              requite::Situation::LOCAL_STATEMENT>(expression);
  if constexpr (SITUATION_PARAM == requite::Situation::VALUE) {
    expression.changeOpcode(requite::Opcode::_INLINE_BLOCK);
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateMangledNameExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::MANGLED_NAME);
  if constexpr (SITUATION_PARAM == requite::Situation::STATEMENT_ATTRIBUTE) {
    this->situateUnaryExpression<SITUATION_PARAM, requite::Situation::VALUE>(
        expression);
  } else if constexpr (SITUATION_PARAM == requite::Situation::REFLECTION) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situateAssertExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ASSERT);
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, 1);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  if (!first.getHasNext()) {
    requite::SourceLocation location =
        this->getContext().getSourceStartLocation(expression);
    std::string assertion_text = llvm::formatv(
        "assertion failure for expression: \n\n{0}\n\n at {1}:{2}:{3}\"",
        first.getSourceText(), location.file, location.line, location.column);
    requite::SavedString saved_text =
        this->getContext().saveString(assertion_text);
    requite::Expression &next = requite::Expression::makeString(saved_text);
    next.setSourceInsertedAfter(first);
    first.setNext(next);
  }
  this->situateBinaryExpression<SITUATION_PARAM, requite::Situation::VALUE>(
      expression);
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateReflectExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_REFLECT);
  unsigned branch_i = 0;
  if (!expression.getHasBranch()) {
    this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
        expression, 2);
    this->setNotOk();
    return;
  }
  if constexpr (SITUATION_PARAM == requite::Situation::ASCRIBED_REFLECTION) {
    this->situateNaryExpression<SITUATION_PARAM, 2,
                                requite::Situation::REFLECTION>(expression);
  } else {
    requite::Expression &value = expression.popBranch();
    this->situateBranch<SITUATION_PARAM, requite::Situation::VALUE>(
        "first branch", expression, branch_i++, value);
    requite::Expression *inner_ptr = &value;
    requite::Expression *next_ptr = value.popNextPtr();
    if (next_ptr == nullptr) {
      this->getContext().logErrorNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, 2);
      this->setNotOk();
      return;
    }
    while (next_ptr != nullptr) {
      requite::Expression &inner = requite::getRef(inner_ptr);
      requite::Expression &next = requite::getRef(next_ptr);
      this->situateBranch<SITUATION_PARAM, requite::Situation::REFLECTION>(
          "second and subsequent branches", expression, branch_i++, next);
      next_ptr = next.popNextPtr();
      const requite::Opcode opcode = next.getOpcode();
      if (opcode == requite::Opcode::__IDENTIFIER_LITERAL) {
        requite::Expression &member =
            requite::Expression::makeOperation(requite::Opcode::_MEMBER_OF);
        member.setSource(inner, next);
        member.setBranch(inner);
        inner.setNext(next);
        inner_ptr = &member;
        continue;
      } else if (opcode == requite::Opcode::_ASCRIBE_TYPE) {
        if (!next.getHasBranch()) {
          continue;
        }
        requite::Expression &branch = next.getBranch();
        if (branch.getOpcode() == requite::Opcode::_REFLECT) {
          if (!branch.getHasBranch()) {
            continue;
          }
          requite::Expression &branch_branch = branch.getBranch();
          const requite::Opcode universalized =
              requite::getUniversalizedAscribed(branch_branch.getOpcode());
          next.changeOpcode(universalized);
          inner.setNextPtr(branch.popNextPtr());
          delete &next.replaceBranch(inner);
          next_ptr = branch_branch.popNextPtr();
          inner_ptr = &next;
          continue;
        }
        const requite::Opcode universalized =
            requite::getUniversalizedAscribed(branch.getOpcode());
        next.changeOpcode(universalized);

        requite::Expression& next_branch = next.getBranch();
        if (next_branch.getHasBranch()) {
          requite::Expression& branch_branch = next_branch.popBranch();
          inner.setNext(branch_branch);
          branch_branch.setNextPtr(branch.popNextPtr());
        } else {
          inner.setNextPtr(branch.popNextPtr());
        }
        delete &next.replaceBranch(inner);
        inner_ptr = &next;
        continue;
      }
      const requite::Opcode universalized = requite::getUniversalized(opcode);
      next.changeOpcode(universalized);
      inner.setNextPtr(next.replaceBranchPtr(&inner));
      inner_ptr = &next;
    }
    expression.setBranchPtr(inner_ptr);
    expression.mergeBranch();
  }
}

} // namespace requite