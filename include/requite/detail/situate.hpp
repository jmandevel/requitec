// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/context.hpp>
#include <requite/expression_walk_result.hpp>
#include <requite/expression_walker.hpp>
#include <requite/source_location.hpp>

#include <llvm/Support/FormatVariadic.h>

namespace requite {

template <requite::Situation SITUATION_PARAM>
void Situator::situateExpression(requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::__NONE:
    REQUITE_UNREACHABLE();
  case requite::Opcode::__INTEGER_LITERAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::__INTEGER_LITERAL)) {
      REQUITE_UNREACHABLE();
    }
    break;
  case requite::Opcode::__REAL_LITERAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::__REAL_LITERAL)) {
      REQUITE_UNREACHABLE();
    }
    break;
  case requite::Opcode::__STRING_LITERAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::__STRING_LITERAL)) {
      REQUITE_UNREACHABLE();
    }
    break;
  case requite::Opcode::__CODEUNIT_LITERAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::__CODEUNIT_LITERAL)) {
      REQUITE_UNREACHABLE();
    }
    break;
  case requite::Opcode::__IDENTIFIER_LITERAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::__IDENTIFIER_LITERAL)) {
      REQUITE_UNREACHABLE();
    }
    break;
  case requite::Opcode::__ERROR:
    REQUITE_UNREACHABLE();
  case requite::Opcode::_CALL_OR_SIGNATURE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_CALL_OR_SIGNATURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_CallOrSignatureExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_BindValueOrDefaultValueExpression<SITUATION_PARAM>(
          expression);
    }
    break;
  case requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_BindSymbolOrDefaultSymbolExpression<SITUATION_PARAM>(
          expression);
    }
    break;
  case requite::Opcode::_TRIP:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TRIP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_TripExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_CONDUIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_CONDUIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_ConduitExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_INFERENCED_TYPE_OR__INDETERMINATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_INFERENCED_TYPE_OR__INDETERMINATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_InferencedTypeOr_INDETERMINATE<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_LOGICAL_AND:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LOGICAL_AND)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_LOGICAL_OR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LOGICAL_OR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_LOGICAL_COMPLEMENT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LOGICAL_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_GREATER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_GREATER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_GREATER_EQUAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_GREATER_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_LESS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LESS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_LESS_EQUAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LESS_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_EQUAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_NOT_EQUAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NOT_EQUAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_REFLECT_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_REFLECT_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_ReflectValueExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_REFLECT_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_REFLECT_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_ReflectSymbolExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_MEMBER_VALUE_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MEMBER_VALUE_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE,
                                    requite::Situation::SYMBOL_NAME>(
          expression);
    }
    break;
  case requite::Opcode::_MEMBER_SYMBOL_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MEMBER_SYMBOL_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE,
                                    requite::Situation::SYMBOL_NAME>(
          expression);
    }
    break;
  case requite::Opcode::_MEMBER_VALUE_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MEMBER_VALUE_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_SYMBOL,
                                    requite::Situation::SYMBOL_NAME>(
          expression);
    }
    break;
  case requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_SYMBOL,
                                    requite::Situation::SYMBOL_NAME>(
          expression);
    }
    break;
  case requite::Opcode::_BIND_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BIND_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::SYMBOL_NAME,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_BIND_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BIND_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::SYMBOL_NAME,
                                    requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_DEFAULT_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DEFAULT_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::SYMBOL_BINDING,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_DEFAULT_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DEFAULT_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::SYMBOL_BINDING,
                                    requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_ASCRIBE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASCRIBE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<
          SITUATION_PARAM, 2, requite::Situation::ATTRIBUTE, SITUATION_PARAM>(
          expression);
    }
    break;
  case requite::Opcode::_CAST:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_CAST)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_SYMBOL,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::STRINGIFY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::STRINGIFY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::CODEUNIFY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CODEUNIFY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_IDENTIFY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_IDENTIFY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situate_IdentifyExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_BAKE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BAKE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM, SITUATION_PARAM>(
          expression);
    }
    break;
  case requite::Opcode::_ADD:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ADD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_SUBTRACT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_SUBTRACT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_MULTIPLY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MULTIPLY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_DIVIDE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DIVIDE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_MODULUS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MODULUS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_NEGATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NEGATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_BITWISE_CAST:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_CAST)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_SYMBOL,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_BITWISE_OR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_OR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_BITWISE_AND:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_AND)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_BITWISE_XOR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_XOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_BITWISE_COMPLEMENT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_COMPLEMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_BITWISE_SHIFT_LEFT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_SHIFT_LEFT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_BITWISE_SHIFT_RIGHT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_SHIFT_RIGHT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_BITWISE_ROTATE_LEFT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_ROTATE_LEFT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_BITWISE_ROTATE_RIGHT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_BITWISE_ROTATE_RIGHT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_CONCATINATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_CONCATINATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::FROM_FRONT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FROM_FRONT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_FROM_FRONT_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_FROM_FRONT_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::FROM_BACK:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FROM_BACK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_FROM_BACK_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_FROM_BACK_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::TRUNCATE_FRONT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TRUNCATE_FRONT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_TRUNCATE_FRONT_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TRUNCATE_FRONT_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::TRUNCATE_BACK:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TRUNCATE_BACK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_TRUNCATE_BACK_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TRUNCATE_BACK_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::AT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::AT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_AT_OFFSET_FROM_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_AT_OFFSET_FROM_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::ADDRESS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ADDRESS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_ADDRESS_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ADDRESS_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_ASSIGN:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<
          SITUATION_PARAM, 2,
          requite::getNextAssignLvalueSitaution<SITUATION_PARAM>(),
          requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_ASSIGN_ADD:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN_ADD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SITUATION_PARAM>(
          expression, requite::Opcode::_ADD);
    }
    break;
  case requite::Opcode::_ASSIGN_SUBTRACT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN_SUBTRACT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SITUATION_PARAM>(
          expression, requite::Opcode::_SUBTRACT);
    }
    break;
  case requite::Opcode::_ASSIGN_MULTIPLY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN_MULTIPLY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SITUATION_PARAM>(
          expression, requite::Opcode::_MULTIPLY);
    }
    break;
  case requite::Opcode::_ASSIGN_DIVIDE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN_DIVIDE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SITUATION_PARAM>(
          expression, requite::Opcode::_DIVIDE);
    }
    break;
  case requite::Opcode::_ASSIGN_MODULUS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ASSIGN_MODULUS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssignArithmeticExpression<SITUATION_PARAM>(
          expression, requite::Opcode::_MODULUS);
    }
    break;
  case requite::Opcode::COPY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::COPY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_COPY_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_COPY_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::MOVE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MOVE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_MOVE_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MOVE_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::SWAP:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::SWAP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE>(
          expression);
    }
    break;
  case requite::Opcode::_FAT_POINTER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_FAT_POINTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::ARRAY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ARRAY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateArrayExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_REFERENCE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_REFERENCE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_STOLEN_REFERENCE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_STOLEN_REFERENCE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_POINTER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_POINTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::MUTABLE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MUTABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::VOLATILE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::VOLATILE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::ATOMIC:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ATOMIC)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::NULL_TERMINATED:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NULL_TERMINATED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_TUPLE_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TUPLE_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_TUPLE_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TUPLE_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::POSITIONAL_FIELD>(
          expression);
    }
    break;
  case requite::Opcode::_ANONYMOUS_OBJECT_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ANONYMOUS_OBJECT_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::VALUE_BINDING>(
          expression);
    }
    break;
  case requite::Opcode::_ANONYMOUS_OBJECT_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ANONYMOUS_OBJECT_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::NAMED_FIELD>(expression);
    }
    break;
  case requite::Opcode::TEMPLATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TEMPLATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::TEMPLATE_PARAMETER>(
          expression);
    }
    break;
  case requite::Opcode::_SPECIALIZATION:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_SPECIALIZATION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::TEMPLATE_PARAMETER>(
          expression);
    }
    break;
  case requite::Opcode::_NO_PARAMETER_SIGNATURE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NO_PARAMETER_SIGNATURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_NAMED_PARAMETER_SIGNATURE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NAMED_PARAMETER_SIGNATURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::NAMED_FIELD>(expression);
    }
    break;
  case requite::Opcode::_POSITIONAL_PARAMETER_SIGNATURE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_POSITIONAL_PARAMETER_SIGNATURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::POSITIONAL_FIELD>(
          expression);
    }
    break;
  case requite::Opcode::_NO_ARGUMENT_CALL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NO_ARGUMENT_CALL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_NAMED_ARGUMENT_CALL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NAMED_ARGUMENT_CALL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::VALUE_BINDING>(
          expression);
    }
    break;
  case requite::Opcode::_POSITIONAL_ARGUMENT_CALL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_POSITIONAL_ARGUMENT_CALL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::DESTROY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::DESTROY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_DESTROY_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DESTROY_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::ENTRY_POINT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ENTRY_POINT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::FUNCTION:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FUNCTION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::METHOD:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::METHOD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::CONSTRUCTOR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CONSTRUCTOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::DESTRUCTOR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::DESTRUCTOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::_ANONYMOUS_FUNCTION:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_ANONYMOUS_FUNCTION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::CAPTURE,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::_CAPTURE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_CAPTURE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    }
    break;
  case requite::Opcode::RETURN:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::RETURN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BREAK:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BREAK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::CONTINUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CONTINUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::FALLTHROUGH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FALLTHROUGH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::EXIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::EXIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::OBJECT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::OBJECT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::SYMBOL_NAME,
                                  requite::Situation::OBJECT_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::TABLE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TABLE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::SYMBOL_PATH,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::ALIAS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ALIAS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_LOCAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LOCAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateVariableExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::GLOBAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::GLOBAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateVariableExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::PROPERTY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::PROPERTY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situatePropertyExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::CONSTANT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CONSTANT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateVariableExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_NULL_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NULL_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_NULL_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NULL_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::TEMP:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TEMP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::TRUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TRUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::FALSE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FALSE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::INDEX:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::INDEX)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::EXCEPTION:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::EXCEPTION)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_IGNORE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_IGNORE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_INDETERMINATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_INDETERMINATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::NO_DEFAULT_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NO_DEFAULT_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::INFERENCED_COUNT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::INFERENCED_COUNT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::THIS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::THIS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::RESULT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::RESULT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::OUTPUT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::OUTPUT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::INPUT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::INPUT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::ADDRESS_SIZE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ADDRESS_SIZE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::ADDRESS_DEPTH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ADDRESS_DEPTH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BITS_PER_BYTE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BITS_PER_BYTE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_INFERENCED_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_INFERENCED_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::VOID:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::VOID)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BOOLEAN:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BOOLEAN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::WORD:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::WORD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateSizedPrimitiveExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::SIGNED_INTEGER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::SIGNED_INTEGER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateSizedPrimitiveExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::UNSIGNED_INTEGER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::UNSIGNED_INTEGER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateSizedPrimitiveExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BINARY_HALF_FLOAT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BINARY_HALF_FLOAT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BINARY_SINGLE_FLOAT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BINARY_SINGLE_FLOAT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BINARY_DOUBLE_FLOAT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BINARY_DOUBLE_FLOAT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::BINARY_QUAD_FLOAT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::BINARY_QUAD_FLOAT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::CHARACTER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CHARACTER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::UTF8:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::UTF8)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::VARIADIC_ARGUMENTS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::VARIADIC_ARGUMENTS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FIRST_VARIADIC_ARGUMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE,
                                    requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NEXT_VARIADIC_ARGUMENT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateBinaryExpression<SITUATION_PARAM,
                                    requite::Situation::MATTE_VALUE,
                                    requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::IF:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::IF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::ELSE_IF:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ELSE_IF)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::ELSE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ELSE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 0,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::SWITCH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::SWITCH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryWithLastExpression<SITUATION_PARAM, 1,
                                          requite::Situation::MATTE_VALUE,
                                          requite::Situation::SWITCH_CASE,
                                          requite::Situation::LAST_SWITCH_CASE>(
          expression);
    }
    break;
  case requite::Opcode::CASE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CASE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::DEFAULT_CASE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::DEFAULT_CASE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 0,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::FOR:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FOR)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 3,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>(),
          requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::WHILE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::WHILE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::DO_WHILE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::DO_WHILE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::FOR_EACH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::FOR_EACH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_VALUE,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::LOOP:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::LOOP)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 0,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::SCOPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::SCOPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 0,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::_VALUE_CONDUIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_VALUE_CONDUIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::_JUNCTION_CONDUIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_JUNCTION_CONDUIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::_DESTINATION_CONDUIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DESTINATION_CONDUIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::MATTE_LOCAL_STATEMENT>(
          expression);
    }
    break;
  case requite::Opcode::LABEL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::LABEL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::SYMBOL_NAME>(expression);
    }
    break;
  case requite::Opcode::GOTO:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::GOTO)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::PRIVATE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::PRIVATE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::PROTECTED:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::PROTECTED)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::EXPORT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::EXPORT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::IMPORT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::IMPORT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::SYMBOL_NAME>(expression);
    }
    break;
  case requite::Opcode::USE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::USE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    }
    break;
  case requite::Opcode::MODULE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MODULE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::SYMBOL_NAME,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::ASSERT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ASSERT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateAssertExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::TRY:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TRY)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 0,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::CATCH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::CATCH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<
          SITUATION_PARAM, 1, requite::Situation::MATTE_SYMBOL,
          requite::getNextScopeStatementSituation<SITUATION_PARAM>()>(
          expression);
    }
    break;
  case requite::Opcode::THROW:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::THROW)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::MAY_THROW:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MAY_THROW)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::THROWS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::THROWS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    }
    break;
  case requite::Opcode::EXTERNAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::EXTERNAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::C:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::C)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::NOT_FINAL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NOT_FINAL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::MAY_DISCARD:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MAY_DISCARD)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::NO_SHORT_CIRCUIT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NO_SHORT_CIRCUIT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::NO_REMAINDER:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NO_REMAINDER)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::INLINE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::INLINE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::MANGLED_NAME:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::MANGLED_NAME)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateMangledNameExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_MANGLED_NAME_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_MANGLED_NAME_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::PACK:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::PACK)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::SIZE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::SIZE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_SIZE_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_SIZE_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_SIZE_OF_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_SIZE_OF_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::DEPTH:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::DEPTH)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_DEPTH_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DEPTH_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_DEPTH_OF_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_DEPTH_OF_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::COUNT:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::COUNT)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_COUNT_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_COUNT_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_COUNT_OF_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_COUNT_OF_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::NAME:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::NAME)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_NAME_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NAME_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_NAME_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_NAME_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::LINE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::LINE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_LINE_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LINE_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_LINE_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_LINE_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::COLUMN:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::COLUMN)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_COLUMN_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_COLUMN_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::_COLUMN_OF_SYMBOL:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_COLUMN_OF_SYMBOL)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::IS:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::IS)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
    break;
  case requite::Opcode::ARE_SAME:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::ARE_SAME)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    }
    break;
  case requite::Opcode::TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_TYPE_OF_VALUE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_TYPE_OF_VALUE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_VALUE>(expression);
    }
    break;
  case requite::Opcode::UNDERLYING:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::UNDERLYING)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
    }
    break;
  case requite::Opcode::_UNDERLYING_OF_TYPE:
    if constexpr (!requite::getCanBeSituation<SITUATION_PARAM>(
                      requite::Opcode::_UNDERLYING_OF_TYPE)) {
      REQUITE_UNREACHABLE();
    } else {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
    }
  case requite::Opcode::__LAST:
    REQUITE_UNREACHABLE();
  }
  if (expression.getIsConverging()) {
    for (requite::Expression &branch : expression.getBranchSubrange()) {
      if (expression.getOpcode() == branch.getOpcode()) {
        branch.flattenBranch();
      }
    }
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateBranch(llvm::Twine log_context,
                             requite::Expression &outer, unsigned branch_i,
                             requite::Expression &branch) {
  const bool is_ok =
      requite::getCanBeSituation<SITUATION_PARAM>(branch.getOpcode());
  if (!is_ok) {
    this->getContext().logInvalidBranchSituation<SITUATION_PARAM>(
        branch, outer.getOpcode(), branch.getOpcode(), branch_i, log_context);
    this->setNotOk();
    return;
  }
  this->situateExpression<SITUATION_PARAM>(branch);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateNullaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if (expression.getHasBranch()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("expression with opcode \"") +
            requite::getName(expression.getOpcode()) + "\" in situation \"" +
            requite::getName<SITUATION_PARAM>() + "\" must not have branches");
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_PARAM>
void Situator::situateUnaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_PARAM>("the first branch",
                                                        expression, 0, branch);
          })
          .getResult();
  if (!result.getWalkedExactly(1)) {
    this->getContext().logNotExactBranchCount<SITUATION_PARAM>(expression, 1);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_PARAM>
void Situator::situateBinaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doSome<2>([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_PARAM>(
                "the first and second branches", expression, branch_i, branch);
          })
          .getResult();
  if (!result.getWalkedExactly(2)) {
    this->getContext().logNotExactBranchCount<SITUATION_PARAM>(expression, 2);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM>
void Situator::situateBinaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_A_PARAM>(
                "the first branch", expression, 0, branch);
          })
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_B_PARAM>(
                "the second branch", expression, 1, branch);
          })
          .getResult();
  if (!result.getWalkedExactly(2)) {
    this->getContext().logNotExactBranchCount<SITUATION_PARAM>(expression, 2);
    this->setNotOk();
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result = expression.walkBranch().doAll(
      [&](unsigned branch_i, requite::Expression &branch) {
        this->situateBranch<BRANCH_SITUATION_N_PARAM>(
            "all branches", expression, branch_i, branch);
      });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotExactBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_A_PARAM>(
                "the first branch", expression, 0, branch);
          })
          .doAll([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_N_PARAM>(
                "the second and subsequent branches", expression, branch_i,
                branch);
          });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_B_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM>
void Situator::situateNaryExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_A_PARAM>(
                "the first branch", expression, 0, branch);
          })
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_B_PARAM>(
                "the second branch", expression, 1, branch);
          })
          .doAll([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_N_PARAM>(
                "the third and subsequent branches", expression, branch_i,
                branch);
          });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
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
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_A_PARAM>(
                "the first branch", expression, 0, branch);
          })
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_B_PARAM>(
                "the second branch", expression, 1, branch);
          })
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_C_PARAM>(
                "the third branch", expression, 2, branch);
          })
          .doAll([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_N_PARAM>(
                "the fourth and subsequent branches", expression, branch_i,
                branch);
          });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM,
          requite::Situation BRANCH_SITUATION_LAST_PARAM>
void Situator::situateNaryWithLastExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doUntilLast([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_N_PARAM>(
                "from the first to the penultimate branch", expression,
                branch_i, branch);
          })
          .doLast([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_LAST_PARAM>(
                "the last branch", expression, 2, branch);
          });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
}

template <requite::Situation SITUATION_PARAM, unsigned MIN_COUNT_PARAM,
          requite::Situation BRANCH_SITUATION_A_PARAM,
          requite::Situation BRANCH_SITUATION_N_PARAM,
          requite::Situation BRANCH_SITUATION_LAST_PARAM>
void Situator::situateNaryWithLastExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  requite::ExpressionWalkResult result =
      expression.walkBranch()
          .doOne([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_A_PARAM>(
                "the first branch", expression, 0, branch);
          })
          .doUntilLast([&](unsigned branch_i, requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_N_PARAM>(
                "the middle branch", expression, branch_i, branch);
          })
          .doLast([&](requite::Expression &branch) {
            this->situateBranch<BRANCH_SITUATION_LAST_PARAM>(
                "the last branch", expression, 2, branch);
          });
  if constexpr (MIN_COUNT_PARAM != 0) {
    if (!result.getWalkedAtLeast(MIN_COUNT_PARAM)) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(
          expression, MIN_COUNT_PARAM);
      this->setNotOk();
    }
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_BindValueOrDefaultValueExpression(
    requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE);
  if constexpr (SITUATION_PARAM == requite::Situation::VALUE_BINDING) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_BIND_VALUE);
  } else if constexpr (SITUATION_PARAM == requite::Situation::NAMED_FIELD) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::SYMBOL_BINDING,
                                  requite::Situation::MATTE_VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_DEFAULT_VALUE);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::POSITIONAL_FIELD) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_DEFAULT_VALUE);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::TEMPLATE_PARAMETER) {
    REQUITE_UNREACHABLE(); // TODO
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_BindSymbolOrDefaultSymbolExpression(
    requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL);
  if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_BINDING) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::SYMBOL_NAME,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    expression.changeOpcode(requite::Opcode::_BIND_SYMBOL);
  } else if constexpr (SITUATION_PARAM == requite::Situation::NAMED_FIELD) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::SYMBOL_NAME,
                                  requite::Situation::MATTE_SYMBOL>(expression);
    expression.changeOpcode(requite::Opcode::_BIND_SYMBOL);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::TEMPLATE_PARAMETER) {
    REQUITE_UNREACHABLE(); // TODO
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situate_ReflectValueExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_REFLECT_VALUE);
  this->situateNaryWithLastExpression<
      SITUATION_PARAM, 2, requite::Situation::MATTE_VALUE,
      requite::Situation::VALUE_REFLECTIVE_VALUE,
      requite::getNextValueReflectiveSituation<SITUATION_PARAM>()>(expression);
  requite::Expression &first = expression.getBranch();
  requite::Expression &second = first.popNext();
  requite::Expression *branch_ptr = second.popNextPtr();
  if (requite::getCanBeSymbolNameSituation(second.getOpcode())) {
    if constexpr (requite::getIsSymbolSituation<SITUATION_PARAM>()) {
      if (branch_ptr == nullptr) {
        expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_VALUE);
        return;
      }
      expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_VALUE);
    } else {
      expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_VALUE);
    }
    first.setNext(second);
  } else {
    if (second.getHasBranch()) {
      first.setNext(second.popBranch());
    }
    expression.changeOpcode(requite::getUniversalizedValue(second.getOpcode()));
    requite::Expression::deleteExpression(second);
  }
  while (branch_ptr != nullptr) {
    requite::Expression &branch = requite::getRef(branch_ptr);
    branch_ptr = branch.popNextPtr();
    requite::Opcode branch_opcode = branch.getOpcode();
    if (requite::getCanBeSymbolNameSituation(branch.getOpcode())) {
      requite::Expression &new_expression =
          requite::Expression::makeOperation(expression.getOpcode());
      new_expression.setSourceInsertedAfter(branch);
      new_expression.setBranch(expression.replaceBranch(new_expression));
      if constexpr (requite::getIsSymbolSituation<SITUATION_PARAM>()) {
        if (branch_ptr == nullptr) {
          expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_VALUE);
          return;
        }
        expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_VALUE);
      } else {
        expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_VALUE);
      }
      new_expression.setNext(branch);
    } else {
      branch.setNextPtr(branch.popBranchPtr());
      branch.setBranch(expression.replaceBranch(branch));
      requite::Opcode universalized =
          requite::getUniversalizedValue(branch.getOpcode());
      branch.changeOpcode(expression.getOpcode());
      expression.changeOpcode(universalized);
    }
  }
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situate_ReflectSymbolExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_REFLECT_SYMBOL);
  this->situateNaryWithLastExpression<
      SITUATION_PARAM, 2, requite::Situation::MATTE_SYMBOL,
      requite::Situation::SYMBOL_REFLECTIVE_SYMBOL,
      requite::getNextSymbolReflectiveSituation<SITUATION_PARAM>()>(expression);
  requite::Expression &first = expression.getBranch();
  requite::Expression &second = first.popNext();
  requite::Expression *branch_ptr = second.popNextPtr();
  if (requite::getCanBeSymbolNameSituation(second.getOpcode())) {
    if constexpr (requite::getIsValueSituation<SITUATION_PARAM>()) {
      if (branch_ptr == nullptr) {
        expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_SYMBOL);
        return;
      }
      expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL);
    } else {
      expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL);
    }
    first.setNext(second);
  } else {
    if (second.getHasBranch()) {
      first.setNext(second.popBranch());
    }
    expression.changeOpcode(
        requite::getUniversalizedSymbol(second.getOpcode()));
    requite::Expression::deleteExpression(second);
  }
  while (branch_ptr != nullptr) {
    requite::Expression &branch = requite::getRef(branch_ptr);
    branch_ptr = branch.popNextPtr();
    requite::Opcode branch_opcode = branch.getOpcode();
    if (requite::getCanBeSymbolNameSituation(branch.getOpcode())) {
      requite::Expression &new_expression =
          requite::Expression::makeOperation(expression.getOpcode());
      new_expression.setSourceInsertedAfter(branch);
      new_expression.setBranch(expression.replaceBranch(new_expression));
      if constexpr (requite::getIsValueSituation<SITUATION_PARAM>()) {
        if (branch_ptr == nullptr) {
          expression.changeOpcode(requite::Opcode::_MEMBER_VALUE_OF_SYMBOL);
          return;
        }
        expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL);
      } else {
        expression.changeOpcode(requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL);
      }
      new_expression.setNext(branch);
    } else {
      branch.setNextPtr(branch.popBranchPtr());
      branch.setBranch(expression.replaceBranch(branch));
      requite::Opcode universalized =
          requite::getUniversalizedSymbol(branch.getOpcode());
      branch.changeOpcode(expression.getOpcode());
      expression.changeOpcode(universalized);
    }
  }
}

template <requite::Situation SITUATION_PARAM>
inline void Situator::situate_InferencedTypeOr_INDETERMINATE(
    requite::Expression &expression) {
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
                SITUATION_PARAM == requite::Situation::POSITIONAL_FIELD) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
    expression.changeOpcode(requite::Opcode::_INFERENCED_TYPE);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
    expression.changeOpcode(requite::Opcode::_INDETERMINATE);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateAssignArithmeticExpression(
    requite::Expression &expression, requite::Opcode arithmetic_opcode) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  this->situateNaryWithLastExpression<
      SITUATION_PARAM, 2,
      requite::getNextAssignLvalueSitaution<SITUATION_PARAM>(),
      requite::Situation::MATTE_VALUE>(expression);
  if (!this->getIsOk()) {
    return;
  }
  expression.changeOpcode(requite::Opcode::COPY);
  requite::Expression &destination = expression.getBranch();
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
void Situator::situate_DefaultValueExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_DEFAULT_VALUE);
  if constexpr (SITUATION_PARAM == requite::Situation::NAMED_FIELD) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::SYMBOL_BINDING,
                                  requite::Situation::MATTE_VALUE>(expression);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::POSITIONAL_FIELD) {
    this->situateBinaryExpression<SITUATION_PARAM,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::MATTE_VALUE>(expression);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::TEMPLATE_PARAMETER) {
    REQUITE_UNREACHABLE(); // TODO
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_DefaultSymbolExpression(
    requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_DEFAULT_SYMBOL);
  if constexpr (SITUATION_PARAM == requite::Situation::TEMPLATE_PARAMETER) {
    REQUITE_UNREACHABLE(); // TODO
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_TripExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_TRIP);
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
                SITUATION_PARAM == requite::Situation::POSITIONAL_FIELD) {
    if (!expression.getHasBranch()) {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
      expression.changeOpcode(requite::Opcode::_NULL_TYPE);
      return;
    }
    requite::Expression &branch = expression.getBranch();
    if (branch.getOpcode() == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
        branch.getOpcode() == requite::Opcode::_DEFAULT_VALUE) {
      if (!branch.getHasBranch()) {
        this->getContext().logNotExactBranchCount<requite::Situation::NONE>(
            branch, 2);
        this->setNotOk();
        return;
      }
      requite::Expression &sub_branch = branch.getBranch();
      if (sub_branch.getOpcode() ==
              requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
          sub_branch.getOpcode() == requite::Opcode::_BIND_SYMBOL) {
        this->situateNaryExpression<SITUATION_PARAM, 0,
                                    requite::Situation::NAMED_FIELD>(
            expression);
        expression.changeOpcode(requite::Opcode::_ANONYMOUS_OBJECT_TYPE);
        return;
      }
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::POSITIONAL_FIELD>(
          expression);
      return;
    }
    if (branch.getOpcode() == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
        branch.getOpcode() == requite::Opcode::_BIND_SYMBOL) {
      this->situateNaryExpression<SITUATION_PARAM, 1,
                                  requite::Situation::NAMED_FIELD>(expression);
      expression.changeOpcode(requite::Opcode::_ANONYMOUS_OBJECT_TYPE);
      return;
    }
    this->situateNaryExpression<SITUATION_PARAM, 0,
                                requite::Situation::POSITIONAL_FIELD>(
        expression);
    if (expression.getHasOneBranch()) {
      expression.flattenBranch();
      return;
    }
    expression.changeOpcode(requite::Opcode::_TUPLE_TYPE);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    if (!expression.getHasBranch()) {
      this->situateNullaryExpression<SITUATION_PARAM>(expression);
      expression.changeOpcode(requite::Opcode::_NULL_VALUE);
      return;
    }
    requite::Expression &branch = expression.getBranch();
    if (branch.getOpcode() == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
        branch.getOpcode() == requite::Opcode::_BIND_VALUE) {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::VALUE_BINDING>(
          expression);
      expression.changeOpcode(requite::Opcode::_ANONYMOUS_OBJECT_VALUE);
      return;
    }
    this->situateNaryExpression<SITUATION_PARAM, 0,
                                requite::Situation::MATTE_VALUE>(expression);
    if (expression.getHasOneBranch()) {
      expression.flattenBranch();
      return;
    }
    expression.changeOpcode(requite::Opcode::_TUPLE_VALUE);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION) {
    if (!expression.getHasBranch()) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                                   1);
      this->setNotOk();
      return;
    }
    requite::Expression &branch = expression.getBranch();
    if (branch.getOpcode() == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
        branch.getOpcode() == requite::Opcode::_BIND_VALUE) {
      this->situateNaryExpression<SITUATION_PARAM, 0,
                                  requite::Situation::VALUE_BINDING>(
          expression);
      expression.changeOpcode(requite::Opcode::_ANONYMOUS_OBJECT_VALUE);
      return;
    }
    this->situateNaryExpression<SITUATION_PARAM, 0,
                                requite::Situation::MATTE_VALUE>(expression);
    if (expression.getHasOneBranch()) {
      expression.flattenBranch();
      return;
    }
    expression.changeOpcode(requite::Opcode::_TUPLE_VALUE);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_DESTINATION) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
    expression.changeOpcode(requite::Opcode::_IGNORE);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_CallOrSignatureExpression(
    requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_DESTINATION ||
                SITUATION_PARAM == requite::Situation::MATTE_LOCAL_STATEMENT ||
                SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    if (!expression.getHasBranch()) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                                   1);
      this->setNotOk();
      return;
    }
    requite::Expression &branch = expression.getBranch();
    if (!branch.getHasNext()) {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
      expression.changeOpcode(requite::Opcode::_NO_ARGUMENT_CALL);
      return;
    }
    requite::Expression &first_argument = branch.getNext();
    if (first_argument.getOpcode() ==
            requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
        first_argument.getOpcode() == requite::Opcode::_BIND_VALUE) {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::VALUE_BINDING>(
          expression);
      expression.changeOpcode(requite::Opcode::_NAMED_ARGUMENT_CALL);
      return;
    }
    this->situateNaryExpression<SITUATION_PARAM, 2,
                                requite::Situation::MATTE_SYMBOL,
                                requite::Situation::MATTE_VALUE>(expression);
    expression.changeOpcode(requite::Opcode::_POSITIONAL_ARGUMENT_CALL);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
                       SITUATION_PARAM ==
                           requite::Situation::POSITIONAL_FIELD) {
    if (!expression.getHasBranch()) {
      this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression,
                                                                   1);
      this->setNotOk();
      return;
    }
    requite::Expression &branch = expression.getBranch();
    if (!branch.getHasNext()) {
      this->situateUnaryExpression<SITUATION_PARAM,
                                   requite::Situation::MATTE_SYMBOL>(
          expression);
      expression.changeOpcode(requite::Opcode::_NO_PARAMETER_SIGNATURE);
      return;
    }
    requite::Expression &first_parameter = branch.getNext();
    if (first_parameter.getOpcode() ==
            requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
        first_parameter.getOpcode() == requite::Opcode::_BIND_SYMBOL ||
        first_parameter.getOpcode() ==
            requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
        first_parameter.getOpcode() == requite::Opcode::_DEFAULT_VALUE) {
      this->situateNaryExpression<SITUATION_PARAM, 2,
                                  requite::Situation::MATTE_SYMBOL,
                                  requite::Situation::NAMED_FIELD>(expression);
      expression.changeOpcode(requite::Opcode::_NAMED_PARAMETER_SIGNATURE);
      return;
    }
    this->situateNaryExpression<SITUATION_PARAM, 2,
                                requite::Situation::MATTE_SYMBOL,
                                requite::Situation::POSITIONAL_FIELD>(
        expression);
    expression.changeOpcode(requite::Opcode::_POSITIONAL_PARAMETER_SIGNATURE);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateVariableExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::GLOBAL ||
                 expression.getOpcode() == requite::Opcode::_LOCAL ||
                 expression.getOpcode() == requite::Opcode::CONSTANT);
  if (!expression.getHasBranch()) {
    this->getContext().logNotExactBranchCount<SITUATION_PARAM>(expression, 2);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  if (first.getOpcode() != requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL &&
      first.getOpcode() != requite::Opcode::_BIND_SYMBOL) {
    requite::Expression &bind_symbol =
        requite::Expression::makeOperation(requite::Opcode::_BIND_SYMBOL);
    bind_symbol.setSourceInsertedAt(first.getSourceTextPtr());
    requite::Expression &inference =
        requite::Expression::makeOperation(requite::Opcode::_INFERENCED_TYPE);
    inference.setSourceInsertedAt(first.getSourceTextPtr());
    bind_symbol.setBranch(expression.replaceBranch(bind_symbol));
    if (first.getHasNext()) {
      bind_symbol.setNext(first.popNext());
    }
    first.setNext(inference);
  }
  this->situateNaryExpression<SITUATION_PARAM, 2,
                              requite::Situation::SYMBOL_BINDING,
                              requite::Situation::MATTE_VALUE>(expression);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situatePropertyExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  if (!expression.getHasBranch()) {
    this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression, 1);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  if (first.getOpcode() != requite::Opcode::_BIND_SYMBOL) {
    requite::Expression &bind_symbol =
        requite::Expression::makeOperation(requite::Opcode::_BIND_SYMBOL);
    bind_symbol.setSourceInsertedAfter(first);
    requite::Expression &inference =
        requite::Expression::makeOperation(requite::Opcode::_INFERENCED_TYPE);
    inference.setSourceInsertedAfter(first);
    bind_symbol.setBranch(expression.replaceBranch(bind_symbol));
    if (first.getHasNext()) {
      bind_symbol.setNext(first.popNext());
    }
    first.setNext(inference);
  }
  requite::Expression &new_first = expression.getBranch();
  if (!new_first.getHasNext()) {
    requite::Expression &no_default_value =
        requite::Expression::makeOperation(requite::Opcode::NO_DEFAULT_VALUE);
    no_default_value.setSourceInsertedAfter(new_first);
    new_first.setNext(no_default_value);
  }
  this->situateNaryExpression<SITUATION_PARAM, 2,
                              requite::Situation::SYMBOL_BINDING,
                              requite::Situation::MATTE_VALUE>(expression);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateSizedPrimitiveExpression(
    requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SIGNED_INTEGER ||
                 expression.getOpcode() == requite::Opcode::UNSIGNED_INTEGER ||
                 expression.getOpcode() == requite::Opcode::WORD);
  if (!expression.getHasBranch()) {
    requite::Expression &first =
        requite::Expression::makeOperation(requite::Opcode::ADDRESS_DEPTH);
    first.setSourceInsertedAfter(expression);
    expression.setBranch(first);
  }
  this->situateUnaryExpression<SITUATION_PARAM,
                               requite::Situation::MATTE_VALUE>(expression);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateArrayExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ARRAY);
  if (!expression.getHasBranch()) {
    this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression, 1);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  if (!first.getHasNext()) {
    requite::Expression &inferrenced_cardinality_expression =
        requite::Expression::makeOperation(requite::Opcode::INFERENCED_COUNT);
    first.setNext(inferrenced_cardinality_expression);
    ;
  }
  this->situateBinaryExpression<SITUATION_PARAM,
                                requite::Situation::MATTE_SYMBOL,
                                requite::Situation::MATTE_VALUE>(expression);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situateAssertExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ASSERT);
  if (!expression.getHasBranch()) {
    this->getContext().logNotAtLeastBranchCount<SITUATION_PARAM>(expression, 1);
    this->setNotOk();
    return;
  }
  requite::Expression &first = expression.getBranch();
  if (!first.getHasNext()) {

    requite::SourceLocation location =
        this->getContext().getSourceStartLocation(expression);
    std::string assertion_text = llvm::formatv(
        "assertion failure for expression: \n\n{0}\n\n at {1}:{2}:{2}\"",
        first.getSourceText(), location.file, location.line, location.column);

    requite::Expression &next = requite::Expression::makeString(assertion_text);
    next.setSourceInsertedAfter(first);
    first.setNext(next);
  }
  this->situateBinaryExpression<SITUATION_PARAM,
                                requite::Situation::MATTE_VALUE>(expression);
}

template <requite::Situation SITUATION_PARAM>
void Situator::situate_IdentifyExpression(requite::Expression &expression) {
  REQUITE_ASSERT(
      requite::getCanBeSituation<SITUATION_PARAM>(expression.getOpcode()));
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_IDENTIFY);
  this->situateUnaryExpression<SITUATION_PARAM,
                               requite::Situation::MATTE_VALUE>(expression);
  requite::Expression &branch = expression.getBranch();
  if (branch.getOpcode() != requite::Opcode::__STRING_LITERAL) {
    return;
  }
  std::string text = branch.getDataText().str();
  expression.flattenBranch();
  expression.changeOpcode(requite::Opcode::__IDENTIFIER_LITERAL);
  expression.setDataText(text);
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situate_ConduitExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_CONDUIT);
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_DESTINATION) {
    expression.changeOpcode(requite::Opcode::_DESTINATION_CONDUIT);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    expression.changeOpcode(requite::Opcode::_VALUE_CONDUIT);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION) {
    expression.changeOpcode(requite::Opcode::_JUNCTION_CONDUIT);
  } else {
    static_assert(false, "inavlid situation");
  }
  this->situateNaryExpression<SITUATION_PARAM, 0,
                              requite::Situation::MATTE_LOCAL_STATEMENT>(
      expression);
}

template <requite::Situation SITUATION_PARAM>
inline void
Situator::situateMangledNameExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::MANGLED_NAME);
  if constexpr (SITUATION_PARAM == requite::Situation::ATTRIBUTE) {
    this->situateUnaryExpression<SITUATION_PARAM,
                                 requite::Situation::MATTE_VALUE>(expression);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_VALUE) {
    this->situateNullaryExpression<SITUATION_PARAM>(expression);
  } else {
    static_assert(false, "invalid situation");
  }
}

} // namespace requite