// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
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

std::string Parser::getText(llvm::StringRef log_message_type_text,
                            const requite::Token &token,
                            llvm::StringRef source_text) {
  llvm::SmallString<32> buffer;
  requite::TextResult result = requite::getTextValue(source_text, buffer);
  if (result != requite::TextResult::OK) {
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("failed to parse ") + log_message_type_text + " because " +
            requite::getDescription(result) + "");
    this->setNotOk();
  }
  const char *text = buffer.c_str();
  return std::string(text);
}

bool Parser::getIsDone() const { return this->_it >= this->_end; }

const requite::Token &Parser::getToken() const {
  REQUITE_ASSERT(this->_it < this->_end);
  return *this->_it;
}

const requite::Token &Parser::getPreviousToken() const {
  REQUITE_ASSERT(this->_it <= this->_end);
  return *(this->_it - 1);
}

const requite::Token &Parser::getNextToken() {
  const requite::Token &next_token = *this->_it;
  this->_it++;
  return next_token;
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
  if (first_token.getType() == requite::TokenType::SEMICOLON_SEPERATOR) {
    this->incrementToken(1);
    this->getContext().logErrorExpectedExpressionBeforeSemicolon(first_token);
    this->setNotOk();
  }
  requite::Expression &first = this->parseExpression();
  this->checkTokenIsTrailingSemicolonOperator(first);
  this->getModule().setExpression(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    requite::Expression &next = this->parseExpression();
    this->checkTokenIsTrailingSemicolonOperator(next);
    requite::getRef(previous_ptr).setNext(next);
    previous_ptr = &next;
  }
  return this->_is_ok;
}

requite::Expression &Parser::parseExpression() {
  REQUITE_ASSERT(!this->getIsDone());
  return this->parsePrecedence12();
}

// ASSIGNMENTS
requite::Expression &Parser::parsePrecedence12() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence11());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::WALRUS_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this,
                                    requite::Opcode::_VARIABLE_DECLARATION);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::PIRATE_WALRUS_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_PROPERTY);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::FOUR_EYED_WALRUS_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ALIAS);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::PLUS_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_ADD);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::DASH_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::STAR_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::SLASH_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::PERCENT_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_MODULUS);
      precedence_parser.setRecent(this->parsePrecedence11());
      continue;
    case requite::TokenType::SWAP_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::SWAP);
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
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::COLON_OPERATOR: {
    std::ignore = this->checkIsNormativeRequiteOk();
    requite::Expression &tacit =
        requite::Expression::makeOperation(requite::Opcode::_TACIT);
    tacit.setSourceInsertedBefore(first_token);
    precedence_parser.setRecent(tacit);
    precedence_parser.parseNary(*this, requite::Opcode::_POSITIONAL_VALUE);
    this->incrementToken(1);
    precedence_parser.appendBranch(this->parsePrecedence10());
  } break;
  case requite::TokenType::DOUBLE_COLON_OPERATOR: {
    std::ignore = this->checkIsNormativeRequiteOk();
    requite::Expression &tacit =
        requite::Expression::makeOperation(requite::Opcode::_TACIT);
    tacit.setSourceInsertedBefore(first_token);
    precedence_parser.setRecent(tacit);
    precedence_parser.parseNary(*this, requite::Opcode::_POSITIONAL_SYMBOL);
    this->incrementToken(1);
    precedence_parser.appendBranch(this->parsePrecedence10());
  } break;
  default:
    precedence_parser.setRecent(this->parsePrecedence10());
  }
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::COLON_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(
          *this, requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE);
      precedence_parser.setRecent(this->parsePrecedence10());
      continue;
    case requite::TokenType::DOUBLE_COLON_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(
          *this, requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL);
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

// BINARY CAST
requite::Expression &Parser::parsePrecedence10() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence9());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::BACKSLASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_CAST);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_BACKSLASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_BITWISE_CAST);
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

// BINARY EXTEND
requite::Expression &Parser::parsePrecedence9() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence8());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::ARROW_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_EXTEND);
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

// NARY LOGICAL
requite::Expression &Parser::parsePrecedence8() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence7());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_LOGICAL_AND);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case requite::TokenType::DOUBLE_PIPE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_LOGICAL_OR);
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

// NARY COMPARISON
requite::Expression &Parser::parsePrecedence7() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence6());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::GREATER_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_GREATER);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::GREATER_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_GREATER_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::LESS_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_LESS);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::LESS_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_LESS_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::DOUBLE_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case requite::TokenType::BANG_EQUAL_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_NOT_EQUAL);
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

// NARY MULTIPLICATIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence6() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence5());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::STAR_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case requite::TokenType::SLASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case requite::TokenType::PERCENT_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_MODULUS);
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

// NARY ADDITIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence5() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setRecent(this->parsePrecedence4());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::PLUS_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_ADD);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::DASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case requite::TokenType::CONCATENATE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this,
                                  requite::Opcode::_COMPILE_TIME_CONCATINATE);
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

// BITWISE AND EARLY UNARY OPERATORS
requite::Expression &Parser::parsePrecedence4() {
  requite::PrecedenceParser precedence_parser;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::TILDE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_BITWISE_COMPLEMENT);
      continue;
    case requite::TokenType::PIPE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_BITWISE_OR);
      continue;
    case requite::TokenType::AMPERSAND_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_BITWISE_AND);
      continue;
    case requite::TokenType::CAROT_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseBinary(*this, requite::Opcode::_BITWISE_XOR);
      continue;
    case requite::TokenType::BANG_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_LOGICAL_COMPLEMENT);
      continue;
    case requite::TokenType::DASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_NEGATE);
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
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING: {
    std::ignore = this->checkIsNormativeRequiteOk();
    precedence_parser.parseCallOrSignatureImplicitCallee(*this);
    precedence_parser.appendBranch(this->parsePrecedence2());
  } break;
  default:
    precedence_parser.setRecent(this->parsePrecedence2());
  }
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    const requite::TokenType type = token.getType();
    if (type == requite::TokenType::LEFT_PARENTHESIS_GROUPING) {
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseCallOrSignature(*this);
    } else if (type == requite::TokenType::LEFT_TRIP_GROUPING) {
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseSpecialization(*this);
    } else {
      break;
    }
    // NOTE:
    //   need to do some wierd stuff to handle dots that are directly after
    //   horned operations of this precedence. the problem is that the dot and
    //   double dot operators are a lower precedence because dot expressions in
    //   the callee should be contained by horned expressions. however, the
    //   entire horned expression should be contained as the first branch in any
    //   expression created from binary dots or double dot operators that are
    //   directly after the closing grouping symbol.
    if (this->getIsDone()) {
      return precedence_parser.getOuter();
    }
    const requite::Token &next0_token = this->getToken();
    const requite::TokenType next0_type = next0_token.getType();
    if (next0_type == requite::TokenType::DOT_OPERATOR) {
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNaryAfterHorned(*this,
                                             requite::Opcode::_REFLECT_VALUE);
      precedence_parser.setRecent(this->parsePrecedence0());
    } else if (next0_type == requite::TokenType::DOUBLE_DOT_OPERATOR) {
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNaryAfterHorned(*this,
                                             requite::Opcode::_REFLECT_SYMBOL);
      precedence_parser.setRecent(this->parsePrecedence0());
    } else {
      continue;
    }
    while (!this->getIsDone()) {
      const requite::Token &next1_token = this->getToken();
      const requite::TokenType next1_type = next1_token.getType();
      if (next1_type == requite::TokenType::DOT_OPERATOR) {
        std::ignore = this->checkIsNormativeRequiteOk();
        precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_VALUE);
        precedence_parser.setRecent(this->parsePrecedence0());
        continue;
      } else if (next1_type == requite::TokenType::DOUBLE_DOT_OPERATOR) {
        std::ignore = this->checkIsNormativeRequiteOk();
        precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_VALUE);
        precedence_parser.setRecent(this->parsePrecedence0());
        continue;
      } else {
        precedence_parser.appendRecent();
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
      precedence_parser.parseAttribute(*this);
      previous_attribute = true;
      continue;
    }
    switch (type) {
    case requite::TokenType::HASH_OPERATOR: {
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &operation =
          requite::Expression::makeOperation(requite::Opcode::_TACIT);
      operation.setSourceInsertedBefore(token);
      precedence_parser.setRecent(operation);
      precedence_parser.parseNary(*this, requite::Opcode::_ARRAY);
      previous_attribute = false;
      continue;
    }
    case requite::TokenType::QUESTION_OPERATOR: {
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &operation =
          requite::Expression::makeOperation(requite::Opcode::_NULL_TYPE);
      operation.setSourceInsertedBefore(token);
      precedence_parser.setRecent(operation);
      precedence_parser.parseNary(*this, requite::Opcode::_VARIANT);
      previous_attribute = false;
      continue;
    }
    case requite::TokenType::CAROT_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_FAT_POINTER);
      previous_attribute = false;
      continue;
    case requite::TokenType::AMPERSAND_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_REFERENCE);
      previous_attribute = false;
      continue;
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseDoubleUnary(*this, requite::Opcode::_REFERENCE);
      previous_attribute = false;
      continue;
    case requite::TokenType::STAR_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnary(*this, requite::Opcode::_POINTER);
      previous_attribute = false;
      continue;
    case requite::TokenType::GRAVE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnaryAttribute(*this, requite::Opcode::MUTABLE);
      previous_attribute = false;
      continue;
    case requite::TokenType::DOUBLE_GRAVE_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnaryAttribute(*this, requite::Opcode::CONSTANT);
      previous_attribute = false;
      continue;
    case requite::TokenType::DOLLAR_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseUnaryAttribute(*this, requite::Opcode::OWNING);
      previous_attribute = false;
      continue;
    default:
      break;
    }
    if (type == requite::TokenType::BACKSLASH_OPERATOR) {
      // NOTE:
      //  cast operators are parsed here a second time in order to handle
      //  implicit inferencences. implicit inferencences must be added when
      //  casting with no root type and only ascriptions and/or subtypes. this
      //  happens only when a unary or ascription operator occurs directly
      //  before a cast.
      //
      // for example:
      //
      //  `&\0
      //
      // this parses as:
      //
      //  [_cast         // this is from the \
      //    [_ascribe    // this is created to apply the mutable attribute
      //      [_mutable] // this is from the `
      //      [_reference   // this is from the &
      //        [tacit] // this is implicit!
      //      ]
      //    ]
      //    0
      //  ]
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_TACIT);
      inference.setSource(token);
      precedence_parser.appendBranch(inference);
      precedence_parser.parseBinaryCombination(*this, requite::Opcode::_CAST);
      precedence_parser.appendBranch(this->parsePrecedence9());
      previous_attribute = false;
      break;
    } else if (type == requite::TokenType::DOUBLE_BACKSLASH_OPERATOR) {
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_TACIT);
      inference.setSource(token);
      precedence_parser.appendBranch(inference);
      precedence_parser.parseBinaryCombination(*this,
                                               requite::Opcode::_BITWISE_CAST);
      precedence_parser.appendBranch(this->parsePrecedence9());
      previous_attribute = false;
      break;
    } else if (type == requite::TokenType::ARROW_OPERATOR) {
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_TACIT);
      inference.setSource(token);
      precedence_parser.appendBranch(inference);
      precedence_parser.parseBinaryCombination(*this, requite::Opcode::_EXTEND);
      precedence_parser.appendBranch(this->parsePrecedence8());
      previous_attribute = false;
      break;
    } else if (requite::getIsSeperator(type) ||
               requite::getIsRightGrouping(type)) {
      std::ignore = this->checkIsNormativeRequiteOk();
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_TACIT);
      inference.setSource(token);
      precedence_parser.appendBranch(inference);
      break;
    }
    if (previous_attribute) {
      precedence_parser.setRecent(this->parsePrecedence12());
      previous_attribute = false;
    } else {
      precedence_parser.setRecent(this->parsePrecedence1());
    }
    if (this->getIsDone()) {
      precedence_parser.appendRecent();
      break;
    }
    const requite::Token &post_token = this->getToken();
    switch (const requite::TokenType post_type = post_token.getType()) {
    case requite::TokenType::HASH_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_ARRAY);
      continue;
    case requite::TokenType::QUESTION_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_VARIANT);
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
    std::ignore = this->checkIsNormativeRequiteOk();
    requite::Expression &tacit =
        requite::Expression::makeOperation(requite::Opcode::_TACIT);
    tacit.setSourceInsertedBefore(first_token);
    precedence_parser.setRecent(tacit);
    precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_VALUE);
    this->incrementToken(1);
    precedence_parser.appendBranch(this->parsePrecedence0());
  } break;
  case requite::TokenType::DOUBLE_DOT_OPERATOR: {
    std::ignore = this->checkIsNormativeRequiteOk();
    requite::Expression &tacit =
        requite::Expression::makeOperation(requite::Opcode::_TACIT);
    tacit.setSourceInsertedBefore(first_token);
    precedence_parser.setRecent(tacit);
    precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_SYMBOL);
    this->incrementToken(1);
    precedence_parser.appendBranch(this->parsePrecedence0());
  } break;
  default:
    precedence_parser.setRecent(this->parsePrecedence0());
  }
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOT_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_VALUE);
      precedence_parser.setRecent(this->parsePrecedence0());
      continue;
    case requite::TokenType::DOUBLE_DOT_OPERATOR:
      std::ignore = this->checkIsNormativeRequiteOk();
      precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_SYMBOL);
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
  case requite::TokenType::LEFT_TRIP_GROUPING:
    std::ignore = this->checkIsNormativeRequiteOk();
    return this->parseTrip();
  case requite::TokenType::LEFT_OPEN_CAP_GROUPING:
    std::ignore = this->checkIsNormativeRequiteOk();
    return this->parseOpenInlineScope();
  case requite::TokenType::LEFT_CLOSED_CAP_GROUPING:
    std::ignore = this->checkIsNormativeRequiteOk();
    return this->parseClosedInlineScope();
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
    std::ignore = this->checkIsNormativeRequiteOk();
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

requite::Expression *Parser::parseBranches(const requite::Token &left_token,
                                           requite::TokenType end) {
  REQUITE_ASSERT(!this->getIsDone());
  if (this->getToken().getType() == end) {
    return nullptr;
  }
  requite::Expression &first = this->parseExpression();
  requite::Expression *previous_ptr = &first;
  while (true) {
    REQUITE_ASSERT(!this->getIsDone());
    const requite::Token &token = this->getToken();
    const requite::TokenType type = token.getType();
    if (type != end) {
      requite::Expression &current = this->parseExpression();
      requite::getRef(previous_ptr).setNext(current);
      previous_ptr = &current;
      continue;
    }
    break;
  }
  return &first;
}

requite::Expression *
Parser::parseOperationBranches(const requite::Token &left_token,
                               const requite::Token &opcode_token) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_BRACKET_GROUPING:
    return nullptr;
  case requite::TokenType::SEMICOLON_SEPERATOR:
    this->incrementToken(1);
    this->getContext().logErrorExpectedExpressionBeforeSemicolon(first_token);
    this->setNotOk();
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_BRACKET_GROUPING:
      return &first;
    case requite::TokenType::SEMICOLON_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        break;
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() == requite::TokenType::RIGHT_BRACKET_GROUPING) {
        return &first;
      }
    } break;
    case requite::TokenType::TRAILER_SEPERATOR: {
      this->incrementToken(1);
      unsigned trailer_depth = 0;
      const requite::Token *front_token_ptr = &opcode_token;
      while (!this->getIsDone()) {
        const requite::Token &trailer_token = this->getToken();
        const requite::Token &front_token = *(front_token_ptr)++;
        switch (const requite::TokenType trailer_type =
                    trailer_token.getType()) {
        case requite::TokenType::LEFT_BRACKET_GROUPING:
          trailer_depth++;
          break;
        case requite::TokenType::RIGHT_BRACKET_GROUPING:
          if (trailer_depth == 0) {
            return &first;
          }
          trailer_depth--;
          break;
        default:
          break;
        }
        if (trailer_token.getSourceText() != front_token.getSourceText()) {
          this->getContext().logSourceMessage(
              trailer_token, requite::LogType::ERROR,
              llvm::Twine("trailer token \"") + trailer_token.getSourceText() +
                  "\" does not match front token \"" +
                  front_token.getSourceText() + "\"");
          this->setNotOk();
        }
        this->incrementToken(1);
      }
    } break;
    default:
      if (previous.getCanHaveNoSemicolon()) {
        break;
      }
      this->getContext().logErrorMissingTrailingSemicolon(previous);
      this->setNotOk();
    }
    requite::Expression &next = this->parseExpression();
    previous.setNext(next);
    previous_ptr = &next;
    continue;
  }
  this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                      "found unterminated operation");
  this->setNotOk();
  return nullptr;
}

requite::Opcode Parser::parseOpcode() {
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
        llvm::Twine("token of type \"") + requite::getName(type) +
            "\" with text \"" + token.getSourceText() +
            "\" does not represent an opcode");
    return requite::Opcode::__ERROR;
  }
  if (requite::getIsInternalUseOnly(opcode)) {
    this->setNotOk();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("internal use opcode not allowed: \"") +
            token.getSourceText() + "\"");
    return requite::Opcode::__ERROR;
  }
  if (requite::getIsIntermediateOperation(opcode)) {
    if (!this->checkIsIntermediateRequiteOk()) {
      return requite::Opcode::__ERROR;
    }
  }
  return opcode;
}

requite::Expression &Parser::parseBracketExpression() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  const requite::Token &opcode_token = this->getToken();
  if (opcode_token.getType() ==
      requite::TokenType::LEFT_BRACKET_GROUPING) { // its a anonymous_function
                                                   // expression
    this->incrementToken(1);
    requite::Expression *capture_branch_ptr = this->parseBranches(
        opcode_token, requite::TokenType::RIGHT_BRACKET_GROUPING);
    if (this->getIsDone()) {
      return requite::Expression::makeError();
    }
    requite::Expression &anonymous_function =
        requite::Expression::makeOperation(
            requite::Opcode::_ANONYMOUS_FUNCTION);
    anonymous_function.setSource(left_token);
    requite::Expression &capture =
        requite::Expression::makeOperation(requite::Opcode::_CAPTURE);
    capture.setSource(opcode_token);
    anonymous_function.setBranch(capture);
    capture.setBranchPtr(capture_branch_ptr);
    const requite::Token &right_capture = this->getToken();
    capture.setSource(left_token, right_capture);
    this->incrementToken(1);
    requite::Expression *capture_next_ptr = this->parseBranches(
        left_token, requite::TokenType::RIGHT_BRACKET_GROUPING);
    if (this->getIsDone()) {
      return requite::Expression::makeError();
    }
    capture.setNextPtr(capture_next_ptr);
    const requite::Token &right_anonymous_function = this->getToken();
    anonymous_function.setSource(left_token, right_anonymous_function);
    this->incrementToken(1);
    return anonymous_function;
  }
  const requite::Opcode opcode = this->parseOpcode();
  requite::Expression *first_ptr =
      this->parseOperationBranches(left_token, opcode_token);
  const requite::Token &right_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setBranchPtr(first_ptr);
  operation.setSource(left_token, right_token);
  return operation;
}

requite::Expression &Parser::parseTrip() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &trip =
      requite::Expression::makeOperation(requite::Opcode::_TRIP);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(trip, requite::LogType::ERROR,
                                        "found unterminated trip");
    return trip;
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_TRIP_GROUPING: {
    trip.setSource(left_token, first_token);
    return trip;
  }
  case requite::TokenType::COMMA_SEPERATOR: {
    this->getContext().logErrorExpectedExpressionBeforeComma(first_token);
    this->setNotOk();
    return trip;
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  trip.setBranch(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_TRIP_GROUPING: {
      trip.setSource(left_token, token);
      this->incrementToken(1);
      return trip;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() == requite::TokenType::RIGHT_TRIP_GROUPING) {
        this->incrementToken(1);
        this->getContext().logErrorExpectedExpressionAfterComma(token);
        this->setNotOk();
        return trip;
      }
      break;
    }
    default:
      this->getContext().logErrorMissingCommmaSeperator(token);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->getContext().logSourceMessage(trip, requite::LogType::ERROR,
                                      "found unterminated trip");
  return trip;
}

requite::Expression &
Parser::parseCallOrSignature(requite::Expression *callee_ptr) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  if (callee_ptr == nullptr) {
    callee_ptr = &requite::Expression::makeOperation(requite::Opcode::_TACIT);
    requite::getRef(callee_ptr).setSourceInsertedBefore(left_token);
  }
  requite::Expression &callee = requite::getRef(callee_ptr);
  requite::Expression &call =
      requite::Expression::makeOperation(requite::Opcode::_CALL_OR_SIGNATURE);
  call.setBranch(callee);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                        "found unterminated call or signature");
    this->setNotOk();
    call.setSource(callee, left_token);
    return call;
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING: {
    this->incrementToken(1);
    call.setSource(callee, first_token);
    return call;
  }
  case requite::TokenType::COMMA_SEPERATOR: {
    this->incrementToken(1);
    this->getContext().logErrorExpectedExpressionBeforeComma(first_token);
    this->setNotOk();
    return call;
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  callee.setNext(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_PARENTHESIS_GROUPING: {
      call.setSource(callee, token);
      this->incrementToken(1);
      return call;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        this->getContext().logSourceMessage(
            call, requite::LogType::ERROR,
            "found unterminated call or signature");
        call.setSource(callee, token);
        this->setNotOk();
        return call;
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() ==
          requite::TokenType::RIGHT_PARENTHESIS_GROUPING) {
        this->incrementToken(1);
        this->getContext().logErrorExpectedExpressionAfterComma(token);
        this->setNotOk();
        return call;
      }
      break;
    }
    default:
      this->getContext().logErrorMissingCommmaSeperator(token);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->getContext().logSourceMessage(call, requite::LogType::ERROR,
                                      "found unterminated call or signature");
  this->setNotOk();
  return call;
}

requite::Expression &
Parser::parseSpecialization(requite::Expression &callee) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &call =
      requite::Expression::makeOperation(requite::Opcode::_SPECIALIZATION);
  call.setBranch(callee);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                        "found unterminated specialization");
    this->setNotOk();
    call.setSource(callee, left_token);
    return callee;
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_TRIP_GROUPING: {
    call.setSource(left_token, first_token);
    return call;
  }
  case requite::TokenType::COMMA_SEPERATOR: {
    this->getContext().logErrorExpectedExpressionBeforeComma(first_token);
    this->setNotOk();
    return call;
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  callee.setNext(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_TRIP_GROUPING: {
      call.setSource(callee, token);
      this->incrementToken(1);
      return call;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        this->getContext().logSourceMessage(
            call, requite::LogType::ERROR, "found unterminated specialization");
        call.setSource(callee, token);
        this->setNotOk();
        return call;
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() == requite::TokenType::RIGHT_TRIP_GROUPING) {
        this->incrementToken(1);
        this->getContext().logErrorExpectedExpressionAfterComma(token);
        this->setNotOk();
        return call;
      }
      break;
    }
    default:
      this->getContext().logErrorMissingCommmaSeperator(token);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->getContext().logSourceMessage(call, requite::LogType::ERROR,
                                      "found unterminated specialization");
  this->setNotOk();
  return call;
}

void Parser::parseAttributeArguments(requite::Expression &attribute) {
  REQUITE_ASSERT(!this->getIsDone());
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(attribute, requite::LogType::ERROR,
                                        "found unterminated attribute");
    this->setNotOk();
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING: {
    attribute.extendSourceOver(first_token);
    return;
  }
  case requite::TokenType::COMMA_SEPERATOR: {
    this->getContext().logErrorExpectedExpressionBeforeComma(first_token);
    this->setNotOk();
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  attribute.setBranch(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_PARENTHESIS_GROUPING: {
      attribute.extendSourceOver(token);
      this->incrementToken(1);
      return;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        this->getContext().logSourceMessage(attribute, requite::LogType::ERROR,
                                            "found unterminated attribute");
        attribute.extendSourceOver(token);
        this->setNotOk();
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() ==
          requite::TokenType::RIGHT_PARENTHESIS_GROUPING) {
        this->incrementToken(1);
        this->getContext().logErrorExpectedExpressionAfterComma(token);
        this->setNotOk();
        return;
      }
      break;
    }
    default:
      this->getContext().logErrorMissingCommmaSeperator(token);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->setNotOk();
  this->getContext().logSourceMessage(attribute, requite::LogType::ERROR,
                                      "found unterminated attribute");
}

requite::Expression &Parser::parseOpenInlineScope() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &scope =
      requite::Expression::makeOperation(requite::Opcode::_OPEN_INLINE_SCOPE);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                        "found unterminated open scope");
    scope.setSource(left_token);
    this->setNotOk();
    return scope;
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_OPEN_CAP_GROUPING: {
    scope.setSource(left_token, first_token);
    return scope;
  }
  case requite::TokenType::SEMICOLON_SEPERATOR: {
    this->getContext().logErrorExpectedExpressionBeforeSemicolon(first_token);
    this->setNotOk();
    return scope;
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  scope.setBranch(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_OPEN_CAP_GROUPING: {
      scope.setSource(left_token, token);
      this->incrementToken(1);
      return scope;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                            "found unterminated open scope");
        scope.setSource(left_token, token);
        this->setNotOk();
        return scope;
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() == requite::TokenType::RIGHT_OPEN_CAP_GROUPING) {
        this->incrementToken(1);
        if (previous.getCanHaveNoSemicolon()) {
          return scope;
        }
        this->getContext().logErrorMissingTrailingSemicolon(previous);
        this->setNotOk();
        return scope;
      }
      break;
    }
    default:
      if (previous.getCanHaveNoSemicolon()) {
        break;
      }
      this->getContext().logErrorMissingTrailingSemicolon(previous);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->setNotOk();
  this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                      "found unterminated open scope");
  return scope;
}

requite::Expression &Parser::parseClosedInlineScope() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &scope =
      requite::Expression::makeOperation(requite::Opcode::_CLOSED_INLINE_SCOPE);
  if (this->getIsDone()) {
    this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                        "found unterminated closed scope");
    scope.setSource(left_token);
    this->setNotOk();
    return scope;
  }
  const requite::Token &first_token = this->getToken();
  switch (const requite::TokenType first_type = first_token.getType()) {
  case requite::TokenType::RIGHT_CLOSED_CAP_GROUPING: {
    scope.setSource(left_token, first_token);
    return scope;
  }
  case requite::TokenType::SEMICOLON_SEPERATOR: {
    this->getContext().logErrorExpectedExpressionBeforeSemicolon(first_token);
    this->setNotOk();
    return scope;
  }
  default:
    break;
  }
  requite::Expression &first = this->parseExpression();
  scope.setBranch(first);
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    REQUITE_ASSERT(!this->getIsDone());
    requite::Expression &previous = requite::getRef(previous_ptr);
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_CLOSED_CAP_GROUPING: {
      scope.setSource(left_token, token);
      this->incrementToken(1);
      return scope;
    }
    case requite::TokenType::COMMA_SEPERATOR: {
      this->incrementToken(1);
      if (this->getIsDone()) {
        this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                            "found unterminated closed scope");
        scope.setSource(left_token, token);
        this->setNotOk();
        return scope;
      }
      const requite::Token &next_token = this->getToken();
      if (next_token.getType() ==
          requite::TokenType::RIGHT_CLOSED_CAP_GROUPING) {
        this->incrementToken(1);
        if (previous.getCanHaveNoSemicolon()) {
          return scope;
        }
        this->getContext().logErrorMissingTrailingSemicolon(previous);
        this->setNotOk();
        return scope;
      }
      break;
    }
    default:
      if (previous.getCanHaveNoSemicolon()) {
        break;
      }
      this->getContext().logErrorMissingTrailingSemicolon(previous);
      this->setNotOk();
    }
    requite::Expression &current = this->parseExpression();
    previous.setNext(current);
    previous_ptr = &current;
  }
  this->setNotOk();
  this->getContext().logSourceMessage(scope, requite::LogType::ERROR,
                                      "found unterminated closed scope");
  return scope;
}

requite::Expression &Parser::parsePostUnary(requite::Expression &first,
                                            requite::Opcode opcode) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  this->incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(first, token);
  operation.setBranch(first);
  return operation;
}

requite::Expression &Parser::parseIdentifierLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::IDENTIFIER_LITERAL);
  requite::Expression &identifier =
      requite::Expression::makeIdentifier(token.getSourceText());
  identifier.setSource(token);
  identifier.setDataText(token.getSourceText());
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
  requite::Expression &integer = requite::Expression::makeInteger();
  integer.setSource(token);
  this->incrementToken(1);
  return integer;
}

requite::Expression &Parser::parseFractionalLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::FRACTIONAL_LITERAL);
  requite::Expression &fractional = requite::Expression::makeFractional();
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
  std::string text =
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
  std::string text =
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
          &requite::Expression::makeOperation(requite::Opcode::_TRIP);
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

bool Parser::checkIsNormativeRequiteOk() {
  if (!requite::getIsNormativeRequiteOk()) {
    const requite::Token &token = this->getToken();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        "normative requite form is not enabled.");
    this->getContext().logInputFileMessage(
        requite::LogType::NOTE,
        "normative requite can be enabled by setting the compiler flat "
        "--form=normative or --form=multiplicative.");
    this->setNotOk();
    return false;
  }
  return true;
}

bool Parser::checkIsIntermediateRequiteOk() {
  if (!requite::getIsIntermediateRequiteOk()) {
    const requite::Token &token = this->getToken();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        "intermediate requite form is not enabled.");
    this->getContext().logInputFileMessage(
        requite::LogType::NOTE,
        "intermediate requite can be enabled by setting the compiler flat "
        "--form=intermediate or --form=multiplicative.");
    this->setNotOk();
    return false;
  }
  return true;
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
      llvm::Twine("found binary operator token of type \"") +
          requite::getName(token.getType()) + "\" with no l-value");
}

void Parser::logErrorHornedNoFirstBranch(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found horned grouping token of type \"") +
          requite::getName(token.getType()) +
          "\" with no preceding first branch");
}

void Parser::logErrorFoundErrorToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found error token of type \"") +
          requite::getName(token.getType()) + "\"");
}

void Parser::logErrorUnexpectedToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found unexpected token of type \"") +
          requite::getName(token.getType()) + "\"");
}

void Parser::logErrorInvalidOperatorSpacing(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("found operator token of type \"") +
          requite::getName(token.getType()));
}

} // namespace requite