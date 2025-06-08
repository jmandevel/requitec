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

Parser::Parser(requite::Context &context, requite::Module &module,
               std::vector<requite::Token> &tokens)
    : _context_ref(context), _module_ref(module), _is_ok(true),
      _it(tokens.cbegin()), _end(tokens.cend()) {}

bool Parser::getIsOk() { return this->_is_ok; }

void Parser::setNotOk() { this->_is_ok = false; }

void Parser::logErrorBinaryNoLValue(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("Found binary operator token of type \"") +
          requite::getName(token.getType()) + "\" with no l-value");
}

void Parser::logErrorHornedNoFirstBranch(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("Found horned grouping token of type \"") +
          requite::getName(token.getType()) +
          "\" with no preceding first branch");
}

void Parser::logErrorFoundErrorToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("Found error token of type \"") +
          requite::getName(token.getType()) + "\"");
}

void Parser::logErrorUnexpectedToken(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("Found unexpected token of type \"") +
          requite::getName(token.getType()) + "\"");
}

void Parser::logErrorInvalidOperatorSpacing(const requite::Token &token) {
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("Found operator token of type \"") +
          requite::getName(token.getType()) + "\" with " +
          requite::getDescription(token.getSpacing()) + "");
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

requite::Module &Parser::getModule() { return this->_module_ref.get(); }

const requite::Module &Parser::getModule() const {
  return this->_module_ref.get();
}

requite::Context &Parser::getContext() { return this->_context_ref.get(); }

const requite::Context &Parser::getContext() const {
  return this->_context_ref.get();
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
  requite::Expression *previous_ptr = &this->parseExpression();
  this->getModule().setExpression(requite::getRef(previous_ptr));
  while (!this->getIsDone()) {
    requite::Expression &next = this->parseExpression();
    requite::getRef(previous_ptr).setNext(next);
    previous_ptr = &next;
  }
  return this->_is_ok;
}

requite::Expression &Parser::parseExpression() {
  REQUITE_ASSERT(!this->getIsDone());
  return this->parsePrecedence11();
}

// ASSIGNMENTS
requite::Expression &Parser::parsePrecedence11() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence10());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::WALRUS_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_LOCAL);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::EQUAL_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_ASSIGN);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::PLUS_EQUAL_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_ADD);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::DASH_EQUAL_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_SUBTRACT);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::STAR_EQUAL_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_MULTIPLY);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::SLASH_EQUAL_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_DIVIDE);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::PERCENT_EQUAL_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_ASSIGN_MODULUS);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    case requite::TokenType::SWAP_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::SWAP);
      precedence_parser.appendBranch(this->parsePrecedence10());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// BINDINGS
requite::Expression &Parser::parsePrecedence10() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence9());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::COLON_OPERATOR:
      precedence_parser.parseBinary(
          *this, requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE);
      precedence_parser.appendBranch(this->parsePrecedence9());
      continue;
    case requite::TokenType::DOUBLE_COLON_OPERATOR:
      precedence_parser.parseBinary(
          *this, requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL);
      precedence_parser.appendBranch(this->parsePrecedence9());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// BINARY CAST
requite::Expression &Parser::parsePrecedence9() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence8());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::SEMICOLON_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_CAST);
      precedence_parser.appendBranch(this->parsePrecedence8());
      continue;
    case requite::TokenType::DOUBLE_SEMICOLON_OPERATOR:
      precedence_parser.parseBinary(*this, requite::Opcode::_BITWISE_CAST);
      precedence_parser.appendBranch(this->parsePrecedence8());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY LOGICAL
requite::Expression &Parser::parsePrecedence8() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence7());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_LOGICAL_AND);
      precedence_parser.appendBranch(this->parsePrecedence7());
      continue;
    case requite::TokenType::DOUBLE_PIPE_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_LOGICAL_OR);
      precedence_parser.appendBranch(this->parsePrecedence7());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY COMPARISON
requite::Expression &Parser::parsePrecedence7() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence6());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::GREATER_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_GREATER);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    case requite::TokenType::GREATER_EQUAL_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_GREATER_EQUAL);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    case requite::TokenType::LESS_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_LESS);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    case requite::TokenType::LESS_EQUAL_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_LESS_EQUAL);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    case requite::TokenType::DOUBLE_EQUAL_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_EQUAL);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    case requite::TokenType::BANG_EQUAL_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_NOT_EQUAL);
      precedence_parser.appendBranch(this->parsePrecedence6());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY MULTIPLICATIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence6() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence5());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::STAR_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_MULTIPLY);
      precedence_parser.appendBranch(this->parsePrecedence5());
      continue;
    case requite::TokenType::SLASH_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_DIVIDE);
      precedence_parser.appendBranch(this->parsePrecedence5());
      continue;
    case requite::TokenType::PERCENT_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_MODULUS);
      precedence_parser.appendBranch(this->parsePrecedence5());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY ADDITIVE ARITHMETIC
requite::Expression &Parser::parsePrecedence5() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence4());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::PLUS_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_ADD);
      precedence_parser.appendBranch(this->parsePrecedence4());
      continue;
    case requite::TokenType::DASH_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_SUBTRACT);
      precedence_parser.appendBranch(this->parsePrecedence4());
      continue;
    case requite::TokenType::CONCATENATE_OPERATOR:
      precedence_parser.parseNary(*this,
                                  requite::Opcode::_COMPILE_TIME_CONCATINATE);
      precedence_parser.appendBranch(this->parsePrecedence4());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// EARLY UNARY OPERATORS
requite::Expression &Parser::parsePrecedence4() {
  requite::PrecedenceParser precedence_parser;
  requite::Expression *unary_ptr = nullptr;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::BANG_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_LOGICAL_COMPLEMENT, unary_ptr, 1);
      continue;
    case requite::TokenType::DASH_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(*this, requite::Opcode::_NEGATE,
                                                unary_ptr, 1);
      continue;
    case requite::TokenType::AT_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(*this, requite::Opcode::_BAKE,
                                                unary_ptr, 1);
      continue;
    case requite::TokenType::HASH_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(*this, requite::Opcode::_EXPAND,
                                                unary_ptr, 1);
      continue;
    default:
      break;
    }
    if (unary_ptr != nullptr) {
      requite::getRef(unary_ptr).setBranch(this->parsePrecedence3());
      break;
    } else {
      precedence_parser.appendBranch(this->parsePrecedence3());
      break;
    }
  }
  return precedence_parser.getOuter();
}

// EARLY GROUPINGS
requite::Expression &Parser::parsePrecedence3() {
  requite::Expression *outer_ptr = &this->parsePrecedence2();
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
      outer_ptr = &this->parseHorned(
          requite::getRef(outer_ptr), requite::Opcode::_CALL_OR_SIGNATURE,
          requite::TokenType::RIGHT_PARENTHESIS_GROUPING);
      continue;
    case requite::TokenType::LEFT_COMPAS_GROUPING:
      outer_ptr = &this->parseHorned(requite::getRef(outer_ptr),
                                     requite::Opcode::_SPECIALIZATION,
                                     requite::TokenType::RIGHT_COMPAS_GROUPING);
      continue;
    default:
      break;
    }
    break;
  }
  return requite::getRef(outer_ptr);
}

// LATE UNARY OPERATORS (things get wierd here)
requite::Expression &Parser::parsePrecedence2() {
  requite::PrecedenceParser precedence_parser;
  requite::Expression *unary_ptr = nullptr;
  bool new_ascribe_needed = true;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    const requite::TokenType type = token.getType();
    switch (type) {
    case requite::TokenType::SLASH_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_FAT_POINTER, unary_ptr, 1);
      new_ascribe_needed = true;
      continue;
    case requite::TokenType::PERCENT_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(*this, requite::Opcode::ARRAY,
                                                unary_ptr, 1);
      new_ascribe_needed = true;
      continue;
    case requite::TokenType::AMBERSAND_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_REFERENCE, unary_ptr, 1);
      new_ascribe_needed = true;
      continue;
    case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_REFERENCE, unary_ptr, 1);
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_REFERENCE, unary_ptr, 1);
      new_ascribe_needed = true;
      continue;
    case requite::TokenType::DOLLAR_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_STOLEN_REFERENCE, unary_ptr, 1);
      new_ascribe_needed = true;
      continue;

    case requite::TokenType::STAR_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      unary_ptr = &precedence_parser.parseUnary(
          *this, requite::Opcode::_POINTER, unary_ptr, 1);
      new_ascribe_needed = true;
      continue;
    case requite::TokenType::GRAVE_OPERATOR:
      if (!token.getHasUnaryOperatorSpacing()) {
        break;
      }
      precedence_parser.parseAttribute(*this, requite::Opcode::MUTABLE,
                                       new_ascribe_needed, unary_ptr);
      new_ascribe_needed = false;
      unary_ptr = nullptr;
      continue;
    default:
      break;
    }
    if (type == requite::TokenType::SEMICOLON_OPERATOR ||
        type == requite::TokenType::DOUBLE_SEMICOLON_OPERATOR) {
      // NOTE:
      //  cast operators are parsed here a second time in order to handle
      //  implicit inferenceences. implicit inferenceences must be added when
      //  casting with no root type and only ascriptions and/or subtypes. this
      //  happens only when a unary or ascription operator occurs directly
      //  before a cast.
      //
      // for example:
      //
      //  `&;0
      //
      // this parses as:
      //
      //  [cast         // this is from the ;
      //    [ascribe    // this is created to apply the mutable modifier
      //      [mutable] // this is from the `
      //      [reference   // this is from the &
      //        [inference] // this inference is implicit!
      //      ]
      //    ]
      //    0
      //  ]
      if (!token.getHasBinaryOperatorSpacing()) {
        break;
      }
      const requite::Opcode opcode =
          type == requite::TokenType::SEMICOLON_OPERATOR
              ? requite::Opcode::_CAST
              : requite::Opcode::_BITWISE_CAST;
      requite::Expression &inference =
          requite::Expression::makeOperation(requite::Opcode::_INFERENCED_TYPE);
      inference.setSource(token);
      if (unary_ptr != nullptr) {
        requite::getRef(unary_ptr).setBranch(inference);
        unary_ptr = nullptr;
      } else {
        precedence_parser.appendBranch(inference);
      }
      precedence_parser.parseBinary(*this, opcode);
      // NOTE:
      //  this is a bit of a clever hack...
      //  we need to go back down to precedence 1 for whatever follows the
      //  cast. this wierdness is required because casts are technically in
      //  precedence 1.
      precedence_parser.appendBranch(this->parsePrecedence7());
    } else if (unary_ptr != nullptr) {
      requite::getRef(unary_ptr).setBranch(this->parsePrecedence1());
      unary_ptr = nullptr;
    } else {
      precedence_parser.appendBranch(this->parsePrecedence1());
    }
    if (this->getIsDone()) {
      break;
    }
    const requite::Token &next_token = this->getToken();
    switch (const requite::TokenType next_type = next_token.getType()) {
    case requite::TokenType::COMMA_OPERATOR:
      if (!next_token.getHasBinaryOperatorSpacing()) {
        break;
      }
      precedence_parser.parseNary(*this, requite::Opcode::_ASCRIBE);
      new_ascribe_needed = false;
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// NARY REFLECTION
requite::Expression &Parser::parsePrecedence1() {
  requite::PrecedenceParser precedence_parser;
  precedence_parser.setInitialOuter(this->parsePrecedence0());
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    if (!token.getHasBinaryOperatorSpacing()) {
      break;
    }
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::DOT_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_VALUE);
      precedence_parser.appendBranch(this->parsePrecedence0());
      continue;
    case requite::TokenType::DOUBLE_DOT_OPERATOR:
      precedence_parser.parseNary(*this, requite::Opcode::_REFLECT_SYMBOL);
      precedence_parser.appendBranch(this->parsePrecedence0());
      continue;
    default:
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// BASE EXPRESSIONS
requite::Expression &Parser::parsePrecedence0() {
  const requite::Token &token = this->getToken();
  switch (const requite::TokenType type = token.getType()) {
  case requite::TokenType::LEFT_BRACKET_GROUPING:
    return this->parseBracketExpression();
  case requite::TokenType::LEFT_TRIP_GROUPING:
    return this->parseCloven(requite::Opcode::_TRIP,
                             requite::TokenType::RIGHT_TRIP_GROUPING);
  case requite::TokenType::LEFT_CAP_GROUPING:
    return this->parseCloven(requite::Opcode::_CONDUIT,
                             requite::TokenType::RIGHT_CAP_GROUPING);
  case requite::TokenType::LEFT_QUOTE_GROUPING:
    return this->parseCloven(requite::Opcode::_QUOTE,
                             requite::TokenType::RIGHT_QUOTE_GROUPING);
  case requite::TokenType::BACKSLASH_OPERATOR:
    return this->parseIdentify();
  case requite::TokenType::QUESTION_OPERATOR:
    return this->parseInferedTypeOrIndeterminate();
  case requite::TokenType::EMPTY_QUOTE_OPERATOR:
      return this->parseEmptyQuote();
  case requite::TokenType::IDENTIFIER_LITERAL:
    return this->parseIdentifierLiteral();
  case requite::TokenType::CODEUNIT_LITERAL:
    return this->parseCodeunitLiteral();
  case requite::TokenType::STRING_LITERAL:
    return this->parseStringLiteral();
  case requite::TokenType::INTEGER_LITERAL:
    return this->parseIntegerLiteral();
  case requite::TokenType::REAL_LITERAL:
    return this->parseRealLiteral();
  case requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
    return this->parseInterpolatedString();
  case requite::TokenType::LEFT_OPERATOR:
    return this->parseLeftOperator();
  case requite::TokenType::RIGHT_OPERATOR:
    return this->parseRightOperator();
  case requite::TokenType::LEFT_RIGHT_OPERATOR:
    return this->parseLeftRightOperator();
  default: {
    this->incrementToken(1);
    this->logErrorUnexpectedToken(token);
    this->setNotOk();
    requite::Expression &error = requite::Expression::makeError();
    error.setSource(token);
    return error;
  }
  }
  REQUITE_UNREACHABLE();
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
  if (this->getToken().getType() ==
      requite::TokenType::RIGHT_BRACKET_GROUPING) {
    return nullptr;
  }
  requite::Expression &first = this->parseExpression();
  requite::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    const requite::Token &token = this->getToken();
    switch (const requite::TokenType type = token.getType()) {
    case requite::TokenType::RIGHT_BRACKET_GROUPING:
      return &first;
    case requite::TokenType::DOUBLE_BACKSLASH_OPERATOR: {
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
    default: {
      requite::Expression &next = this->parseExpression();
      requite::getRef(previous_ptr).setNext(next);
      previous_ptr = &next;
      continue;
    }
    }
  }
  this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                      "Found unterminated operation");
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
  } else if (type == requite::TokenType::INTEGER_LITERAL) {
    std::underlying_type_t<requite::Opcode> underlying;
    requite::NumericResult result =
        requite::getNumericValue(token.getSourceText(), underlying);
    if (result != requite::NumericResult::OK) {
      this->setNotOk();
      this->getContext().logSourceMessage(
          token, requite::LogType::ERROR,
          llvm::Twine("failed to parse integer opcode: \"") +
              requite::getDescription(result) + "\"");
      return requite::Opcode::__ERROR;
    }
    opcode = static_cast<requite::Opcode>(underlying);
    if (!requite::getIsValid(opcode)) {
      this->setNotOk();
      this->getContext().logSourceMessage(
          token, requite::LogType::ERROR,
          llvm::Twine("invalid integer opcode: \"") + token.getSourceText() +
              "\"");
      return requite::Opcode::__ERROR;
    }
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
  if (!requite::options::INTERMEDIATE_FORM.getValue() &&
      requite::getIsIntermediateOperation(opcode)) {
    this->setNotOk();
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("intermediate operation opcode not allowed: \"") +
            token.getSourceText() + "\"");
    this->getContext().logSourceMessage(token, requite::LogType::NOTE,
                                        "can compile operation with opcode if "
                                        "-intermediate-form compiler flag is "
                                        "set");
    return requite::Opcode::__ERROR;
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

requite::Expression &Parser::parseHorned(requite::Expression &first,
                                         requite::Opcode opcode,
                                         requite::TokenType end) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression *second_ptr = this->parseBranches(left_token, end);
  this->incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(first);
  operation.setBranch(first);
  if (!this->getIsDone()) {
    const requite::Token &right_token = this->getToken();
    operation.setSource(first, right_token);
  }
  first.setNextPtr(second_ptr);
  return operation;
}

requite::Expression &Parser::parseCloven(requite::Opcode opcode,
                                         requite::TokenType end) {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &left_token = this->getToken();
  this->incrementToken(1);
  requite::Expression *branch_ptr = this->parseBranches(left_token, end);
  const requite::Token &right_token = this->getToken();
  this->incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(left_token, right_token);
  operation.setBranchPtr(branch_ptr);
  return operation;
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

requite::Expression &Parser::parseIdentify() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::BACKSLASH_OPERATOR);
  requite::Expression &identify =
      requite::Expression::makeOperation(requite::Opcode::_IDENTIFY);
  identify.setSource(token);
  this->incrementToken(1);
  requite::Expression &first = this->parsePrecedence1();
  identify.setBranch(first);
  return identify;
}

requite::Expression &Parser::parseInferedTypeOrIndeterminate() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::QUESTION_OPERATOR);
  requite::Expression &expression = requite::Expression::makeOperation(
      requite::Opcode::_INFERENCED_TYPE_OR_INDETERMINATE);
  expression.setSource(token);
  this->incrementToken(1);
  return expression;
}

requite::Expression &Parser::parseEmptyQuote() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::EMPTY_QUOTE_OPERATOR);
  requite::Expression &expression = requite::Expression::makeOperation(
      requite::Opcode::_QUOTE);
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

requite::Expression &Parser::parseRealLiteral() {
  REQUITE_ASSERT(!this->getIsDone());
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::REAL_LITERAL);
  requite::Expression &real = requite::Expression::makeReal();
  real.setSource(token);
  this->incrementToken(1);
  return real;
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
      next_ptr = &this->parseCloven(requite::Opcode::_TRIP,
                                    requite::TokenType::RIGHT_TRIP_GROUPING);
      requite::getRef(previous_ptr).setNextPtr(next_ptr);
      previous_ptr = next_ptr;
      continue;
    default:
      break;
    }
  }
  this->getContext().logSourceMessage(left_token, requite::LogType::ERROR,
                                      "Found unterminated interpolated string");
  this->setNotOk();
  return requite::Expression::makeError();
}

requite::Expression &Parser::parseLeftOperator() {
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::LEFT_OPERATOR);
  this->incrementToken(1);
  if (!this->getIsDone()) {
    const requite::Token &next_token = this->getToken();
    if (next_token.getType() == requite::TokenType::RIGHT_OPERATOR) {
      this->incrementToken(1);
      requite::Expression &operation = requite::Expression::makeOperation(
          requite::Opcode::_POSITIONAL_FIELDS_END_AND_NAMED_FIELDS_BEGIN);
      operation.setSource(token, next_token);
      return operation;
    }
  }
  requite::Expression &operation = requite::Expression::makeOperation(
      requite::Opcode::_POSITIONAL_FIELDS_END);
  operation.setSource(token);
  return operation;
}

requite::Expression &Parser::parseRightOperator() {
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::RIGHT_OPERATOR);
  this->incrementToken(1);
  requite::Expression &operation =
      requite::Expression::makeOperation(requite::Opcode::_NAMED_FIELDS_BEGIN);
  operation.setSource(token);
  return operation;
}

requite::Expression &Parser::parseLeftRightOperator() {
  const requite::Token &token = this->getToken();
  REQUITE_ASSERT(token.getType() == requite::TokenType::LEFT_RIGHT_OPERATOR);
  this->incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(
      requite::Opcode::_POSITIONAL_FIELDS_END_AND_NAMED_FIELDS_BEGIN);
  operation.setSource(token);
  return operation;
}

} // namespace requite