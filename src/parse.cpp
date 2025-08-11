// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/grouping_parser.hpp>
#include <requite/literal_text.hpp>
#include <requite/numeric.hpp>
#include <requite/options.hpp>
#include <requite/parser.hpp>
#include <requite/precedence_parser.hpp>
#include <requite/strings.hpp>
#include <requite/unreachable.hpp>

namespace requite {

bool Context::parseAst(requite::Module &module,
                       std::vector<requite::Token> &tokens) {
  bool is_ok = true;
  requite::Parser parser(*this, module, tokens);
  is_ok = parser.parseExpressions();
  return is_ok;
}

requite::SavedString Parser::getText(llvm::StringRef log_message_type_text,
                                     const requite::Token &token,
                                     llvm::StringRef source_text) {
  llvm::SmallString<32> buffer;
  requite::TextResult result = requite::getTextValue(source_text, buffer);
  if (result != requite::TextResult::OK) {
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("failed to parse ") + log_message_type_text + " because " +
            requite::getDescription(result) + "\n");
    this->setNotOk();
  }
  return this->getContext().saveString(buffer.str());
}

bool Parser::getIsDone() const { return this->_it >= this->_end; }

bool Parser::getIsDone(unsigned offset) const {
  return this->_it + offset >= this->_end;
}

const requite::Token &Parser::getToken() const {
  REQUITE_ASSERT(this->_it < this->_end);
  return *this->_it;
}

const requite::Token &Parser::getToken(unsigned offset) const {
  REQUITE_ASSERT(this->_it + offset < this->_end);
  return *(this->_it + offset);
}

const requite::Token &Parser::getPreviousToken() const {
  REQUITE_ASSERT(this->_it <= this->_end);
  return *(this->_it - 1);
}

void Parser::incrementToken(std::size_t offset) { this->_it += offset; }

bool Parser::getIsToken(requite::TokenType type) const {
  if (this->getIsDone()) {
    return false;
  }
  const bool is_token = this->getToken().getType() == type;
  return is_token;
}

// NOTE:
//  This is (mostly) a recursive descent parser.

bool Parser::parseExpressions() {
  if (this->getIsDone()) {
    return this->_is_ok;
  }
  const requite::Token &first_token = this->getToken();
  requite::Expression &first = this->parseExpression();
  this->checkTokenIsTrailingSemicolonOperator(first);
  this->getModule().setExpression(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    requite::Expression &previous = requite::getRef(previous_ptr);
    requite::Expression &next = this->parseExpression();
    this->checkTokenIsTrailingSemicolonOperator(next);
    previous.setNext(next);
    previous_ptr = &next;
  }
  return this->_is_ok;
}

requite::Expression &Parser::parseExpression() {
  return this->parsePrecedence12();
}

// ASSIGNMENTS
requite::Expression &Parser::parsePrecedence12() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence11());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::PLUS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN_ADD);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::DASH_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::STAR_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::SLASH_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::PERCENT_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_ASSIGN_MODULUS);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// BINDINGS
requite::Expression &Parser::parsePrecedence11() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence10());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::COLON_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_COLON);
      precedence_parser.setRecent(this->parsePrecedence10());
      continue;
    case requite::TokenType::DOUBLE_COLON_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_BITWISE_CAST);
      precedence_parser.setRecent(this->parsePrecedence10());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// RANGES
requite::Expression &Parser::parsePrecedence10() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence9());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::THICK_ARROW_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_LONG_RANGE);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_PLUS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_STEP_ADD, this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_STEP_SUBTRACT,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_STAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_STEP_MULTIPLY,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_SLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_STEP_DIVIDE, this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_PERCENT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_STEP_MODULUS,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_LESS, this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_GREATER,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_LESS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_LESS_EQUAL,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_GREATER_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_GREATER_EQUAL,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_DOUBLE_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_EQUAL, this->parsePrecedence9());
      continue;
    case requite::TokenType::DOT_BANG_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseShortRangeBranch(
          token, requite::Opcode::_SHORT_WHILE_NOT_EQUAL,
          this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_DOT_DOUBLE_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_LIMIT_RANGE_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence9());

      continue;
    case requite::TokenType::DOUBLE_DOT_BANG_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token,
                                    requite::Opcode::_LIMIT_RANGE_NOT_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_DOT_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, requite::Opcode::_LIMIT_RANGE_LESS);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_DOT_LESS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token,
                                    requite::Opcode::_LIMIT_RANGE_LESS_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_DOT_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token,
                                    requite::Opcode::_LIMIT_RANGE_GREATER);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_DOT_GREATER_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(
          token, requite::Opcode::_LIMIT_RANGE_GREATER_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// NARY LOGICAL
requite::Expression &Parser::parsePrecedence9() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence8());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_LOGICAL_AND);
      precedence_parser.setRecent(this->parsePrecedence8());
      continue;
    case requite::TokenType::DOUBLE_PIPE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_LOGICAL_OR);
      precedence_parser.setRecent(this->parsePrecedence8());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// NARY COMPARISON
requite::Expression &Parser::parsePrecedence8() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence7());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::GREATER_OPERATOR: {
      if (this->getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const requite::Token &next_token = this->getToken(1);
      switch (const requite::TokenType next_type = next_token.getType()) {
      case requite::TokenType::GREATER_OPERATOR:
        [[fallthrough]];
      case requite::TokenType::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (requite::getIsExpressionEnd(next_type)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_GREATER);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    }
    case requite::TokenType::GREATER_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_GREATER_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case requite::TokenType::LESS_OPERATOR: {
      if (this->getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const requite::Token &next_token = this->getToken(1);
      switch (const requite::TokenType next_type = next_token.getType()) {
      case requite::TokenType::GREATER_OPERATOR:
        [[fallthrough]];
      case requite::TokenType::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (requite::getIsExpressionEnd(next_type)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_LESS);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    }
    case requite::TokenType::LESS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_LESS_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case requite::TokenType::DOUBLE_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case requite::TokenType::BANG_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_NOT_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// NARY MULTIPLICATIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence7() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence6());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::STAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::SLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::PERCENT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_MODULUS);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// NARY ADDITIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence6() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence5());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::PLUS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_ADD);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case requite::TokenType::DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case requite::TokenType::CONCATENATE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token,
                                  requite::Opcode::_CONCATINATE);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// NARY AND BINARY BITWISE
requite::Expression &Parser::parsePrecedence5() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence4());
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOUBLE_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token,
                                    requite::Opcode::_BITWISE_SHIFT_LEFT);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::DOUBLE_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token,
                                    requite::Opcode::_BITWISE_SHIFT_RIGHT);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::PIPE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_BITWISE_OR);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::AMPERSAND_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_BITWISE_AND);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::CAROT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_BITWISE_XOR);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// EARLY UNARY OPERATORS
requite::Expression &Parser::parsePrecedence4() {
  requite::PrecedenceParser precedence_parser;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::BANG_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_LOGICAL_COMPLEMENT);
      continue;
    case requite::TokenType::DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_NEGATE);
      continue;
    case requite::TokenType::TILDE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_BITWISE_COMPLEMENT);
      continue;
    default:
      precedence_parser.appendBranch(this->parsePrecedence3());
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// EARLY GROUPINGS (funny tricks here)
requite::Expression &Parser::parsePrecedence3() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.appendBranch(this->parsePrecedence2());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    const requite::TokenType type = token.getType();
    requite::Expression &outer = precedence_parser.getOuter();
    if (requite::getBranchCanHaveNoSemicolon(outer.getOpcode())) {
      break;
    }
    if (type == requite::TokenType::LEFT_PARENTHESIS_GROUPING) {
      precedence_parser.setOuterOperation(
          this->parseHorned(outer, requite::Opcode::_CALL,
                            requite::TokenType::RIGHT_PARENTHESIS_GROUPING));
    } else if (type == requite::TokenType::LEFT_TRIP_GROUPING) {
      precedence_parser.setOuterOperation(
          this->parseHorned(outer, requite::Opcode::_SPECIALIZATION,
                            requite::TokenType::RIGHT_TRIP_GROUPING));
    } else if (type == requite::TokenType::LEFT_BRACKET_GROUPING) {
      precedence_parser.setOuterOperation(
          this->parseHorned(outer, requite::Opcode::_INDEX,
                            requite::TokenType::RIGHT_BRACKET_GROUPING));
    } else {
      break;
    }
    // NOTE:
    //   need to do some wierd stuff to handle dots that are directly after
    //   horned operations of this precedence. the problem is that the dot
    //   operator is a lower precedence because dot expressions in the callee
    //   should be contained by horned expressions. however, the entire horned
    //   expression should be contained as the first branch in any expression
    //   created from binary dots operators that are directly after the closing
    //   grouping symbol.
    while (!this->getIsDone()) {
      const requite::Token &next1_token = this->getToken();
      const requite::TokenType next1_type = next1_token.getType();
      if (next1_type == requite::TokenType::DOT_OPERATOR) {
        this->incrementToken(1);
        precedence_parser.parseNary(next1_token,
                                           requite::Opcode::_REFLECT);
        precedence_parser.appendBranch(this->parsePrecedence0());
        continue;
      } else {
        break;
      }
    }
  }
  return precedence_parser.getOuter();
}

// LATE UNARY OPERATORS (things get wierd here)
requite::Expression &Parser::parsePrecedence2() {
  requite::PrecedenceParser precedence_parser;
  bool previous_attribute = false;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    const requite::TokenType type = token.getType();
    if (type == requite::TokenType::AT_OPERATOR) {
      requite::Expression &attribute = this->parseAttribute();
      precedence_parser.parseAscribe(token);
      precedence_parser.appendBranch(attribute);
      previous_attribute = true;
      continue;
    }
    switch (type) {
    case requite::TokenType::ARROW_OPERATOR: {
      requite::Expression &operation =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
      operation.setSourceInsertedBefore(token);
      precedence_parser.setRecent(operation);
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_EXTEND);
      previous_attribute = false;
      continue;
    }
    case requite::TokenType::HASH_OPERATOR: {
      requite::Expression &operation =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
      operation.setSourceInsertedBefore(token);
      precedence_parser.setRecent(operation);
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_ARRAY);
      previous_attribute = false;
      continue;
    }
    case requite::TokenType::TILDE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_FAT_POINTER);
      previous_attribute = false;
      continue;
    case requite::TokenType::BACKSLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_IDENTIFY);
      previous_attribute = false;
      continue;
    case requite::TokenType::AMPERSAND_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_REFERENCE);
      previous_attribute = false;
      continue;
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      precedence_parser.parseDoubleUnary(token, requite::Opcode::_REFERENCE);
      previous_attribute = false;
      continue;
    case requite::TokenType::STAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, requite::Opcode::_POINTER);
      previous_attribute = false;
      continue;
    case requite::TokenType::GRAVE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseAscribe(token);
      precedence_parser.appendUnaryAttribute(token, requite::Opcode::MUTABLE);
      previous_attribute = true;
      continue;
    case requite::TokenType::DOUBLE_GRAVE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseAscribe(token);
      precedence_parser.appendUnaryAttribute(token, requite::Opcode::CONSTANT);
      previous_attribute = true;
      continue;
    case requite::TokenType::DOLLAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseAscribe(token);
      precedence_parser.appendUnaryAttribute(token, requite::Opcode::CONSTANT);
      previous_attribute = true;
      continue;
    default:
      break;
    }
    if (requite::getIsTacitTerminator(type)) {
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
      inference.setSource(token);
      precedence_parser.appendBranch(inference);
      break;
    }
    requite::Expression &expression =
        previous_attribute ? this->parseExpression() : this->parsePrecedence1();
    previous_attribute = false;
    precedence_parser.setRecent(expression);
    if (requite::getBranchCanHaveNoSemicolon(expression.getOpcode())) {
      precedence_parser.appendRecent();
      break;
    }
    if (this->getIsDone()) {
      precedence_parser.appendRecent();
      break;
    }
    const requite::Token &post_token = this->getToken();
    switch (const requite::TokenType post_type = post_token.getType()) {
    case requite::TokenType::HASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_ARRAY);
      previous_attribute = false;
      continue;
    case requite::TokenType::ARROW_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_EXTEND);
      previous_attribute = false;
      continue;
    default:
      precedence_parser.appendRecent();
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY REFLECTION
requite::Expression &Parser::parsePrecedence1() {
  requite::PrecedenceParser precedence_parser;
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::DOT_OPERATOR: {
    requite::Expression &tacit =
        requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
    tacit.setSourceInsertedBefore(first_token);
    precedence_parser.setRecent(tacit);
    this->incrementToken(1);
    precedence_parser.parseNary(first_token, requite::Opcode::_REFLECT);
    precedence_parser.setRecent(this->parsePrecedence0());
  } break;
  default:
    precedence_parser.setRecent(this->parsePrecedence0());
    break;
  }
  while (!this->getIsDone()) {
    if (requite::getBranchCanHaveNoSemicolon(
            precedence_parser.getRecent().getOpcode())) {
      break;
    }
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, requite::Opcode::_REFLECT);
      precedence_parser.setRecent(this->parsePrecedence0());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_parser.appendRecent();
  return precedence_parser.getOuter();
}

// BASE EXPRESSIONS
requite::Expression &Parser::parsePrecedence0() {
  const requite::Token &token = this->getToken();
  switch (const requite::TokenType type = token.getType()) {
  case requite::TokenType::LEFT_BRACKET_GROUPING:
    return this->parseBracketExpression();
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
    return this->parseCloven();
  case requite::TokenType::LEFT_TRIP_GROUPING:
    return this->parseTrip();
  case requite::TokenType::IDENTIFIER_LITERAL:
    return this->parseIdentifierLiteral();
  case requite::TokenType::CODEUNIT_LITERAL:
    return this->parseCodeunitLiteral();
  case requite::TokenType::STRING_LITERAL:
    return this->parseStringLiteral();
  case requite::TokenType::INTEGER_LITERAL:
    return this->parseIntegerLiteral();
  case requite::TokenType::FRACTIONAL_LITERAL:
    return this->parseFractionalLiteral();
  case requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
    return this->parseInterpolatedString();
  default:
    break;
  }
  this->incrementToken(1);
  this->logErrorUnexpectedToken(token);
  this->setNotOk();
  requite::Expression &error = requite::Expression::makeError();
  error.setSource(token);
  return error;
}

bool Parser::parseCommaSeperatedBranches(requite::Expression &operation,
                                         requite::TokenType end,
                                         bool must_not_have_parameter_marks) {
  REQUITE_ASSERT(
      !requite::getHasSemicolonSeperatedBranches(operation.getOpcode()));
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedExpression(operation);
    this->setNotOk();
    return false;
  }
  requite::GroupingParser grouping_parser;
  grouping_parser.startGroup(operation);
  bool has_parameter_marks = false;
  while (!this->getIsDone()) {
    const requite::Token &first_token = this->getToken();
    if (first_token.getType() == end) {
      this->incrementToken(1);
      grouping_parser.finishOperation(first_token);
      return has_parameter_marks;
    }
    while (!this->getIsDone()) {
      while (!this->getIsDone()) {
        const requite::Token &before_token = this->getToken();
        const requite::TokenType before_type = before_token.getType();
        if (before_type == end) {
          grouping_parser.finishOperation(before_token);
          return has_parameter_marks;
        } else if (before_type == requite::TokenType::GREATER_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            this->getContext().logErrorMustNotHaveParameterMark(operation,
                                                                before_token);
            this->setNotOk();
          }
          requite::Expression &mark = requite::Expression::makeOperation(
              requite::Opcode::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(before_token);
          grouping_parser.appendBranch(mark);
          this->incrementToken(1);
          continue;
        } else if (before_type == requite::TokenType::LESS_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            this->getContext().logErrorMustNotHaveParameterMark(operation,
                                                                before_token);
            this->setNotOk();
          }
          requite::Expression &mark = requite::Expression::makeOperation(
              requite::Opcode::_POSITIONAL_PARAMETERS_END);
          mark.setSource(before_token);
          grouping_parser.appendBranch(mark);
          this->incrementToken(1);
          continue;
        }
        break;
      }
      if (this->getIsDone()) {
        break;
      }
      requite::Expression &branch = this->parseExpression();
      grouping_parser.appendBranch(branch);
      while (!this->getIsDone()) {
        const requite::Token &after_token = this->getToken();
        const requite::TokenType after_type = after_token.getType();
        if (after_type == end) {
          this->incrementToken(1);
          grouping_parser.finishOperation(after_token);
          return has_parameter_marks;
        } else if (after_type == requite::TokenType::GREATER_OPERATOR) {
          this->incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            this->getContext().logErrorMustNotHaveParameterMark(operation,
                                                                after_token);
            this->setNotOk();
          }
          requite::Expression &mark = requite::Expression::makeOperation(
              requite::Opcode::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (after_type == requite::TokenType::LESS_OPERATOR) {
          this->incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            this->getContext().logErrorMustNotHaveParameterMark(operation,
                                                                after_token);
            this->setNotOk();
          }
          requite::Expression &mark = requite::Expression::makeOperation(
              requite::Opcode::_POSITIONAL_PARAMETERS_END);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (after_type != requite::TokenType::COMMA_SEPERATOR) {
          this->getContext().logErrorExpectedCommaSeperator(after_token);
          this->setNotOk();
          break;
        }
        this->incrementToken(1);
        break;
      }
    }
  }
  grouping_parser.finishOperation(this->getPreviousToken());
  this->getContext().logErrorUnterminatedExpression(operation);
  this->setNotOk();
  return has_parameter_marks;
}

requite::Opcode Parser::parseOperationOpcode() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  this->incrementToken(1);
  const requite::TokenType type = token.getType();
  requite::Opcode opcode;
  if (type == requite::TokenType::IDENTIFIER_LITERAL) {
    opcode = this->getContext().getOpcode(token.getSourceText());
  } else {
    this->setNotOk();
    this->getContext().logSourceMessage(token, requite::LogType::ERROR,
                                        "opcode token not identifier literal");
    return requite::Opcode::__ERROR;
  }
  if (opcode == requite::Opcode::__NONE) {
    this->setNotOk();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine(requite::getDescription(type)) + "\" with text \"" +
            token.getSourceText() + "\" does not represent an opcode");
    return requite::Opcode::__ERROR;
  }
  if (requite::getIsInternal(opcode)) {
    this->setNotOk();
    this->getContext().logSourceMessage(token, requite::LogType::ERROR,
                                        llvm::Twine(requite::getName(opcode)) +
                                            " is for internal use only.");
    return requite::Opcode::__ERROR;
  }
  if (requite::getIsIntermediate(opcode)) {
    this->setNotOk();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine(requite::getName(opcode)) +
            " is for intermediate representation only.");
    return requite::Opcode::__ERROR;
  }
  return opcode;
}

requite::Opcode Parser::parseAttributeOpcode() {
  const requite::Token &token = this->getToken();
  requite::Opcode opcode = this->parseOperationOpcode();
  if (opcode != requite::Opcode::__ERROR) {
    if (!requite::getCanBeAttribute(opcode)) {
      this->setNotOk();
      this->getContext().logSourceMessage(
          token, requite::LogType::ERROR,
          llvm::Twine("opcode is not valid for attribute: \"") +
              token.getSourceText() + "\"");
      return requite::Opcode::__ERROR;
    }
    return opcode;
  }
  return requite::Opcode::__ERROR;
}

requite::Expression &Parser::parseBracketExpression() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  const requite::Token &opcode_token = this->getToken();
  if (opcode_token.getType() ==
      requite::TokenType::LEFT_BRACKET_GROUPING) { // its a
                                                   // anonymous_function
                                                   // expression
    requite::Expression &anonymous_function =
        requite::Expression::makeOperation(
            requite::Opcode::_ANONYMOUS_FUNCTION);
    anonymous_function.setSource(left_token);
    requite::GroupingParser parser;
    parser.startGroup(anonymous_function);
    requite::Expression &capture = this->parseCapture();
    parser.appendBranch(capture);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedExpression(parser.getOperation());
      this->setNotOk();
      return parser.getOperation();
    }
    const requite::Token &first_token = this->getToken();
    switch (const requite::TokenType first_type = first_token.getType()) {
    case requite::TokenType::RIGHT_BRACKET_GROUPING: {
      this->incrementToken(1);
      requite::Expression &tacit =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
      tacit.setSourceInsertedBefore(first_token);
      parser.appendBranch(tacit);
      parser.finishOperation(first_token);
      return parser.getOperation();
    }
    case requite::TokenType::COMMA_SEPERATOR:
      break;
    default:
      break;
    }
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedExpression(parser.getOperation());
      this->setNotOk();
      return parser.getOperation();
    }
    requite::Expression &first = this->parseExpression();
    if (this->getIsDone()) {
      parser.appendBranch(first);
      this->getContext().logErrorUnterminatedExpression(parser.getOperation());
      this->setNotOk();
      return parser.getOperation();
    }
    const requite::Token &next_token = this->getToken();
    switch (const requite::TokenType next_type = next_token.getType()) {
    case requite::TokenType::COMMA_SEPERATOR:
      this->incrementToken(1);
      parser.appendBranch(first);
      break;
    case requite::TokenType::RIGHT_BRACKET_GROUPING:
      this->incrementToken(1);
      parser.appendBranch(first);
      parser.finishOperation(next_token);
      return parser.getOperation();
    case requite::TokenType::SEMICOLON_SEPERATOR: {
      this->incrementToken(1);
      requite::Expression &tacit =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCE);
      tacit.setSourceInsertedBefore(first);
      parser.appendBranch(tacit);
      parser.appendBranch(first);
    } break;
    default:
      this->getContext().logErrorExpectedSeperatorOrRightBracket(next_token);
      this->setNotOk();
      break;
    }
    while (!this->getIsDone()) {
      const requite::Token &before_token = this->getToken();
      switch (const requite::TokenType before_type = before_token.getType()) {
      case requite::TokenType::RIGHT_BRACKET_GROUPING:
        this->incrementToken(1);
        parser.finishOperation(before_token);
        return parser.getOperation();
      default:
        break;
      }
      if (this->getIsDone()) {
        break;
      }
      requite::Expression &branch = this->parseExpression();
      parser.appendBranch(branch);
      if (this->getIsDone()) {
        break;
      }
      const requite::Token &after_token = this->getToken();
      const requite::TokenType after_type = after_token.getType();
      switch (after_type) {
      case requite::TokenType::SEMICOLON_SEPERATOR:
        this->incrementToken(1);
        break;
      case requite::TokenType::COMMA_SEPERATOR:
        this->getContext().logErrorMissingTrailingSemicolon(branch);
        this->setNotOk();
        break;
      default:
        if (requite::getBranchCanHaveNoSemicolon(branch.getOpcode())) {
          break;
        }
        this->getContext().logErrorMissingTrailingSemicolon(branch);
        this->setNotOk();
        break;
      }
      if (after_type == requite::TokenType::COMMA_SEPERATOR) {
        this->incrementToken(1);
      }
    }
    this->getContext().logErrorUnterminatedExpression(parser.getOperation());
    this->setNotOk();
    return parser.getOperation();
  }
  const requite::Opcode opcode = this->parseOperationOpcode();
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(left_token);
  if (requite::getHasSemicolonSeperatedBranches(opcode)) {
    const unsigned comma_count =
        requite::getCommaTerminatingBranchCount(opcode);
    requite::GroupingParser parser;
    parser.startGroup(operation);
    unsigned branch_i = 0;
    if (comma_count > 0) {
      while (!this->getIsDone()) { // commas
        requite::Expression &next = this->parseExpression();
        const requite::Token &after_token = this->getToken();
        const requite::TokenType after_type = after_token.getType();
        if (branch_i == comma_count - 1) { // handle last
          switch (after_type) {
          case requite::TokenType::SEMICOLON_SEPERATOR: {
            this->incrementToken(1);
            if (requite::getLastCommaBranchCanBeTacit(opcode)) {
              requite::Expression &tacit = requite::Expression::makeOperation(
                  requite::Opcode::_INFERENCE);
              tacit.setSourceInsertedBefore(next);
              parser.appendBranch(tacit);
              parser.appendBranch(next);
            } else if (requite::getFirstCommaBranchCanBeTacit(opcode) ||
                       requite::getAllCommaBranchesCanBeTacit(opcode)) {
              requite::Expression &tacit = requite::Expression::makeOperation(
                  requite::Opcode::_INFERENCE);
              if (operation.getHasBranch()) {
                requite::Expression &first = operation.getBranch();
                tacit.setSourceInsertedBefore(first);
                tacit.setNext(operation.replaceBranch(tacit));
              } else {
                tacit.setSourceInsertedBefore(next);
                parser.appendBranch(tacit);
              }
              parser.appendBranch(next);
            } else {
              this->getContext().logErrorExpectedCommaSeperator(after_token);
              this->setNotOk();
            }
            break;
          }
          case requite::TokenType::COMMA_SEPERATOR:
            this->incrementToken(1);
            parser.appendBranch(next);
            break;
          case requite::TokenType::RIGHT_BRACKET_GROUPING:
            this->incrementToken(1);
            parser.finishOperation(after_token);
            return operation;
          default:
            this->getContext().logErrorExpectedSeperator(after_token);
            this->setNotOk();
            break;
          }
          branch_i++;
          break; // this was the last comma. break to do semicolons next.
        }
        switch (after_type) {
        case requite::TokenType::COMMA_SEPERATOR:
          this->incrementToken(1);
          parser.appendBranch(next);
          break;
        case requite::TokenType::SEMICOLON_SEPERATOR:
          this->incrementToken(1);
          if (requite::getAllCommaBranchesCanBeTacit(opcode)) {
            for (; branch_i < comma_count; branch_i++) {
              requite::Expression &tacit = requite::Expression::makeOperation(
                  requite::Opcode::_INFERENCE);
              if (operation.getHasBranch()) {
                requite::Expression &first = operation.getBranch();
                tacit.setSourceInsertedBefore(first);
                tacit.setNext(operation.replaceBranch(tacit));
              } else {
                tacit.setSourceInsertedBefore(next);
                parser.appendBranch(tacit);
              }
            }
            parser.appendBranch(next);
          } else {
            this->getContext().logErrorExpectedCommaSeperator(after_token);
            this->setNotOk();
          }
          break;
        case requite::TokenType::RIGHT_BRACKET_GROUPING:
          this->incrementToken(1);
          parser.finishOperation(after_token);
          return operation;
        default:
          this->getContext().logErrorExpectedCommaSeperator(after_token);
          this->setNotOk();
        }
        branch_i++;
      }
    }
    while (!this->getIsDone()) { // semicolons
      const requite::Token &before_token = this->getToken();
      const requite::TokenType before_type = before_token.getType();
      if (before_type == requite::TokenType::RIGHT_BRACKET_GROUPING) {
        this->incrementToken(1);
        parser.finishOperation(before_token);
        return operation;
      }
      requite::Expression &next = this->parseExpression();
      parser.appendBranch(next);
      const requite::Token &after_token = this->getToken();
      const requite::TokenType after_type = after_token.getType();
      switch (after_type) {
      case requite::TokenType::SEMICOLON_SEPERATOR:
        this->incrementToken(1);
        break;
      case requite::TokenType::COMMA_SEPERATOR:
        this->incrementToken(1);
        this->getContext().logErrorExpectedSemicolonSeperator(after_token);
        this->setNotOk();
        break;
      case requite::TokenType::RIGHT_BRACKET_GROUPING:
        this->incrementToken(1);
        parser.finishOperation(after_token);
        return operation;
      default:
        if (requite::getBranchCanHaveNoSemicolon(opcode)) {
          break;
        }
        this->getContext().logErrorMissingTrailingSemicolon(next);
        this->setNotOk();
        break;
      }
    }
    this->getContext().logErrorUnterminatedExpression(operation);
    this->setNotOk();
    return operation;
  }
  bool has_parameter_marks = this->parseCommaSeperatedBranches(
      operation, requite::TokenType::RIGHT_BRACKET_GROUPING, true);
  return operation;
}

requite::Expression &Parser::parseTrip() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  requite::Expression &trip =
      requite::Expression::makeOperation(requite::Opcode::_TUPLE);
  trip.setSource(first_token);
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedExpression(trip);
    this->setNotOk();
    return trip;
  }
  const requite::Token &second_token = this->getToken();
  if (this->getIsDone(1)) {
    this->getContext().logErrorUnterminatedExpression(trip);
    this->setNotOk();
    return trip;
  }
  switch (const requite::TokenType second_type = second_token.getType()) {
  case requite::TokenType::GREATER_OPERATOR: {
    const requite::Token &third_token = this->getToken(1);
    if (this->getIsDone(2)) {
      this->getContext().logErrorUnterminatedExpression(trip);
      this->setNotOk();
      return trip;
    }
    switch (const requite::TokenType third_type = third_token.getType()) {
    case requite::TokenType::LESS_OPERATOR: {
      const requite::Token &fourth_token = this->getToken(2);
      switch (const requite::TokenType fourth_type = fourth_token.getType()) {
      case requite::TokenType::RIGHT_TRIP_GROUPING: {
        trip.changeOpcode(requite::Opcode::_NULL_TYPE);
        trip.extendSourceOver(fourth_token);
        this->incrementToken(3);
        return trip;
      } break;
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case requite::TokenType::LESS_OPERATOR: {
    const requite::Token &third_token = this->getToken(1);
    if (this->getIsDone(2)) {
      this->getContext().logErrorUnterminatedExpression(trip);
      this->setNotOk();
      return trip;
    }
    switch (const requite::TokenType third_type = third_token.getType()) {
    case requite::TokenType::GREATER_OPERATOR: {
      const requite::Token &fourth_token = this->getToken(2);
      switch (const requite::TokenType fourth_type = fourth_token.getType()) {
      case requite::TokenType::RIGHT_TRIP_GROUPING: {
        trip.changeOpcode(requite::Opcode::_NULL_TYPE);
        trip.extendSourceOver(fourth_token);
        this->incrementToken(3);
        return trip;
      } break;
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  default:
    break;
  }
  bool has_parameter_marks = this->parseCommaSeperatedBranches(
      trip, requite::TokenType::RIGHT_TRIP_GROUPING, false);
  if (has_parameter_marks) {
    trip.changeOpcode(requite::Opcode::_LAYOUT);
  } else if (!trip.getHasBranch()) {
    trip.changeOpcode(requite::Opcode::_NULL);
  }
  return trip;
}

requite::Expression &Parser::parseCapture() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  requite::Expression &capture =
      requite::Expression::makeOperation(requite::Opcode::CAPTURE);
  capture.setSource(first_token);
  this->incrementToken(1);
  bool has_parameter_marks = this->parseCommaSeperatedBranches(
      capture, requite::TokenType::RIGHT_BRACKET_GROUPING, true);
  return capture;
}

requite::Expression &Parser::parseAttribute() {
  const requite::Token &at_token = this->getToken();
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedAttribute(at_token);
    this->setNotOk();
    requite::Expression &error =
        requite::Expression::makeOperation(requite::Opcode::__ERROR);
    error.setSource(at_token);
    return error;
  }
  const requite::Token &next_token = this->getToken();
  if (next_token.getType() == requite::TokenType::LEFT_PARENTHESIS_GROUPING) {
    this->incrementToken(1);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedAttribute(at_token);
      this->setNotOk();
      requite::Expression &error =
          requite::Expression::makeOperation(requite::Opcode::__ERROR);
      error.setSource(at_token, next_token);
      return error;
    }
    requite::Opcode opcode = this->parseAttributeOpcode();
    requite::Expression &attribute = requite::Expression::makeOperation(opcode);
    attribute.setSource(at_token);
    bool has_parameter_marks = this->parseCommaSeperatedBranches(
        attribute, requite::TokenType::RIGHT_PARENTHESIS_GROUPING, true);
    return attribute;
  } else if (next_token.getType() == requite::TokenType::LEFT_TRIP_GROUPING) {
    this->incrementToken(1);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedAttribute(at_token);
      this->setNotOk();
      requite::Expression &error =
          requite::Expression::makeOperation(requite::Opcode::__ERROR);
      error.setSource(at_token, next_token);
      return error;
    }
    requite::Expression &attribute =
        requite::Expression::makeOperation(requite::Opcode::TEMPLATE);
    attribute.setSource(at_token);
    this->incrementToken(1);
    bool has_parameter_marks = this->parseCommaSeperatedBranches(
        attribute, requite::TokenType::RIGHT_TRIP_GROUPING, false);
    if (!has_parameter_marks) {
      this->getContext().logErrorMustHaveParameterMarks(attribute);
      this->setNotOk();
      return attribute;
    }
    return attribute;
  }
  const requite::Token &opcode_token = this->getToken();
  requite::Opcode opcode = this->parseAttributeOpcode();
  requite::Expression &attribute = requite::Expression::makeOperation(opcode);
  attribute.setSource(at_token, opcode_token);
  return attribute;
}

requite::Expression &Parser::parseHorned(requite::Expression &horn,
                                         requite::Opcode opcode,
                                         requite::TokenType right_token) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  requite::Expression &horned = requite::Expression::makeOperation(opcode);
  horned.setBranch(horn);
  horned.setSource(horn, first_token);
  this->incrementToken(1);
  bool has_parameter_marks =
      this->parseCommaSeperatedBranches(horned, right_token, true);
  return horned;
}

requite::Expression &Parser::parseCloven() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  requite::Expression &cloven =
      requite::Expression::makeOperation(requite::Opcode::_CLOVEN);
  cloven.setSource(first_token);
  this->incrementToken(1);
  bool has_parameter_marks = this->parseCommaSeperatedBranches(
      cloven, requite::TokenType::RIGHT_PARENTHESIS_GROUPING, false);
  if (has_parameter_marks) {
    cloven.changeOpcode(requite::Opcode::_SIGNATURE);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedExpression(cloven);
      this->setNotOk();
      return cloven;
    }
    this->incrementToken(1);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedExpression(cloven);
      this->setNotOk();
      return cloven;
    }
    requite::Expression &return_type = this->parseExpression();
    cloven.extendSourceOver(return_type);
    if (cloven.getHasBranch()) {
      return_type.setNext(cloven.replaceBranch(return_type));
    } else {
      cloven.setBranch(return_type);
    }
  }
  return cloven;
}

requite::Expression &Parser::parseIdentifierLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::IDENTIFIER_LITERAL);
  requite::Expression &identifier = requite::Expression::makeIdentifier(
      this->getContext().saveString(token.getSourceText()));
  identifier.setSource(token);
  this->incrementToken(1);
  return identifier;
}

requite::Expression &Parser::parseNullaryOperator(requite::Opcode opcode) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  requite::Expression &expression = requite::Expression::makeOperation(opcode);
  expression.setSource(token);
  this->incrementToken(1);
  return expression;
}

requite::Expression &Parser::parseIntegerLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::INTEGER_LITERAL);
  requite::Expression &integer =
      requite::Expression::makeOperation(requite::Opcode::__INTEGER_LITERAL);
  integer.setSource(token);
  this->incrementToken(1);
  return integer;
}

requite::Expression &Parser::parseFractionalLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::FRACTIONAL_LITERAL);
  requite::Expression &fractional =
      requite::Expression::makeOperation(requite::Opcode::__FRACTIONAL_LITERAL);
  fractional.setSource(token);
  this->incrementToken(1);
  return fractional;
}

requite::Expression &Parser::parseStringLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::STRING_LITERAL);
  requite::Token token_copy = token;
  token_copy.dropFrontAndBack();
  requite::SavedString text =
      this->getText("string literal", token, token_copy.getSourceText());
  requite::Expression &string = requite::Expression::makeString(text);
  this->incrementToken(1);
  string.setSource(token);
  return string;
}

requite::Expression &Parser::parseCodeunitLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::CODEUNIT_LITERAL);
  requite::Token token_copy = token;
  token_copy.dropFrontAndBack();
  requite::SavedString text =
      this->getText("codeunit literal", token, token_copy.getSourceText());
  requite::Expression &codeunit = requite::Expression::makeCodeunit(text);
  codeunit.setSource(token);
  this->incrementToken(1);
  return codeunit;
}

requite::Expression &Parser::parseInterpolatedString() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  requite::Expression *expression_ptr = nullptr;
  requite::Expression *first_ptr = nullptr;
  requite::Expression *previous_ptr = nullptr;
  requite::Expression *next_ptr = nullptr;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    requite::Token token_copy = token;
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
      REQUITE_ASSERT(first_ptr == nullptr);
      REQUITE_ASSERT(previous_ptr == nullptr);
      REQUITE_ASSERT(next_ptr == nullptr);
      token_copy.dropFront();
      first_ptr = &requite::Expression::makeString(this->getText(
          "left string interpolation", token, token_copy.getSourceText()));
      first_ptr->setSource(token);
      previous_ptr = first_ptr;
      this->incrementToken(1);
      continue;
    case requite::TokenType::MIDDLE_INTERPOLATED_STRING_LITERAL:
      next_ptr = &requite::Expression::makeString(this->getText(
          "middle string interpolation", token, token_copy.getSourceText()));
      next_ptr->setSource(token);
      requite::getRef(previous_ptr).setNextPtr(next_ptr);
      previous_ptr = next_ptr;
      this->incrementToken(1);
      continue;
    case requite::TokenType::RIGHT_INTERPOLATED_STRING_LITERAL:
      token_copy.dropBack();
      next_ptr = &requite::Expression::makeString(this->getText(
          "right string interpolation", token, token_copy.getSourceText()));
      next_ptr->setSource(token);
      requite::getRef(previous_ptr).setNextPtr(next_ptr);
      previous_ptr = next_ptr;
      REQUITE_ASSERT(expression_ptr == nullptr);
      expression_ptr =
          &requite::Expression::makeOperation(requite::Opcode::_TUPLE);
      requite::getRef(expression_ptr).setSource(left_token, token);
      requite::getRef(expression_ptr).setBranchPtr(first_ptr);
      this->incrementToken(1);
      return requite::getRef(expression_ptr);
    case requite::TokenType::LEFT_TRIP_GROUPING:
      next_ptr = &this->parseTrip();
      requite::getRef(previous_ptr).setNextPtr(next_ptr);
      previous_ptr = next_ptr;
      continue;
    default:
      break;
    }
  }
  this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                      "found unterminated interpolated string");
  this->setNotOk();
  return requite::Expression::makeError();
}

void Parser::checkTokenIsTrailingSemicolonOperator(
    requite::Expression &expression) {
  if (this->getIsDone()) {
    if (expression.getCanHaveNoSemicolon()) {
      return;
    }
    this->getContext().logErrorMissingTrailingSemicolon(expression);
    this->setNotOk();
    return;
  }
  const requite::Token &token = this->getToken();
  if (token.getType() == requite::TokenType::SEMICOLON_SEPERATOR) {
    this->incrementToken(1);
    return;
  }
  if (expression.getCanHaveNoSemicolon()) {
    return;
  }
  this->getContext().logErrorMissingTrailingSemicolon(expression);
  this->setNotOk();
}

void Parser::logErrorBinaryNoLValue(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found binary ") + requite::getDescription(token.getType()) +
          " with no l-value");
}

void Parser::logErrorFoundErrorToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found ") + requite::getDescription(token.getType()));
}

void Parser::logErrorUnexpectedToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found unexpected ") +
          requite::getDescription(token.getType()) + " token");
}

} // namespace requite