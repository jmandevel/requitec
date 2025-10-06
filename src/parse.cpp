#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/parse.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

namespace rq {

void GroupingParser::startGroup(rq::Expression &existing_expression) {
  if (existing_expression.getHasBranch()) {
    rq::Expression &branch = existing_expression.getBranch();
    RQ_ASSERT(!branch.getHasNext(), "branch must not have next");
    this->_last_ptr = &branch;
  }
  this->setOperation(existing_expression);
}

void GroupingParser::appendBranch(rq::Expression &branch) {
  if (this->_last_ptr == nullptr) {
    rq::Expression &operation = this->getOperation();
    operation.setBranch(branch);
    operation.extendSourceOver(branch);
    this->_last_ptr = &branch;
    return;
  }
  rq::Expression &last = rq::dereferencePtr(this->_last_ptr);
  last.setNext(branch);
  this->_last_ptr = &branch;
  rq::Expression &operation = this->getOperation();
  operation.extendSourceOver(branch);
}

void GroupingParser::finishOperation(const rq::Token &last_token) {
  rq::Expression &operation = this->getOperation();
  operation.extendSourceOver(last_token);
}

void PrecedenceParser::parseDoubleUnary(const rq::Token &token,
                                        rq::Keyword keyword) {
  rq::Expression &operation0 = this->getContext().acquireExpression();
  operation0.setKeyword(keyword);
  operation0.setSource(token);
  this->appendBranch(operation0);
  this->_operation_ptr = &operation0;
  this->_last_ptr = nullptr;
  rq::Expression &operation1 = this->getContext().acquireExpression();
  operation1.setKeyword(keyword);
  operation1.setSource(token);
  this->appendBranch(operation1);
  this->_operation_ptr = &operation1;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseUnary(const rq::Token &token, rq::Keyword keyword) {
  rq::Expression &operation = this->getContext().acquireExpression();
  operation.setKeyword(keyword);
  operation.setSource(token);
  this->appendBranch(operation);
  this->_operation_ptr = &operation;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseAscribe(const rq::Token &token,
                                    rq::Keyword keyword) {
  if (this->getHasOperation()) {
    rq::Expression &old_operation = this->getOperation();
    if (old_operation.getKeyword() != keyword) {
      rq::Expression &new_operation = this->getContext().acquireExpression();
      new_operation.setKeyword(keyword);
      new_operation.setSource(old_operation, token);
      this->appendBranch(new_operation);
      if (!this->getHasOuter()) {
        this->_outer_ptr = &new_operation;
      }
      this->_operation_ptr = &new_operation;
      this->_last_ptr = nullptr;
    }
    return;
  }
  rq::Expression &operation = this->getContext().acquireExpression();
  operation.setKeyword(keyword);
  if (this->getHasLast()) {
    rq::Expression &last = this->getLast();
    operation.setSource(last);
    operation.setBranch(last);
  } else {
    operation.setSource(token);
  }
  if (!this->getHasOuter()) {
    this->_outer_ptr = &operation;
  }
  this->_operation_ptr = &operation;
}

void PrecedenceParser::parseBinary(const rq::Token &token,
                                   rq::Keyword keyword) {
  rq::Expression &new_operation = this->getContext().acquireExpression();
  new_operation.setKeyword(keyword);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceParser::parseNary(const rq::Token &token, rq::Keyword keyword) {
  if (this->getHasOperation()) {
    rq::Expression &existing_operation = this->getOperation();
    if (existing_operation.getKeyword() == keyword) {
      // the existing operation already has this keyword, so we can keep
      // appending to this one
      this->appendRecent();
      return;
    }
  }
  // need to make a new operation of this keyword because one does not exist yet
  rq::Expression &new_operation = this->getContext().acquireExpression();
  new_operation.setKeyword(keyword);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceParser::parseNestingNary(const rq::Token &token,
                                        rq::Keyword keyword) {
  rq::Expression &operation = this->getContext().acquireExpression();
  operation.setKeyword(keyword);
  operation.setSource(this->getOuter(), token);
  operation.setBranch(this->getOuter());
  this->_operation_ptr = &operation;
  this->_last_ptr = this->_outer_ptr;
  this->_outer_ptr = &operation;
}

void PrecedenceParser::parseSequenceBranch(const rq::Token &token,
                                           rq::Keyword keyword,
                                           rq::Expression &rvalue) {
  this->parseNary(token, rq::Keyword::_SEQUENCE);
  rq::Expression &step = this->getContext().acquireExpression();
  step.setKeyword(keyword);
  step.setSource(token, rvalue);
  step.setBranch(rvalue);
  this->setRecent(step);
}

void PrecedenceParser::appendBranch(rq::Expression &branch) {
  if (!this->getHasOuter()) {
    this->_outer_ptr = &branch;
  }
  if (this->getHasLast()) {
    this->getLast().setNext(branch);
  }
  if (this->getHasOperation()) {
    rq::Expression &operation = this->getOperation();
    if (!this->getHasLast()) {
      operation.setBranch(branch);
    }
    operation.extendSourceOver(branch);
  }
  this->_last_ptr = &branch;
}

void PrecedenceParser::appendUnaryAttribute(const rq::Token &token,
                                            rq::Keyword keyword) {
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->appendBranch(expression);
}

void PrecedenceParser::setRecent(rq::Expression &branch) {
  rq::assignSingleValue(this->_recent_ptr, &branch);
}

void PrecedenceParser::setOnlyRecent(rq::Expression &branch) {
  this->_outer_ptr = nullptr;
  this->_operation_ptr = nullptr;
  this->_last_ptr = nullptr;
  this->_recent_ptr = &branch;
}

void PrecedenceParser::appendRecent() {
  this->appendBranch(this->getRecent());
  this->_recent_ptr = nullptr;
}

bool NormativeParser::getIsToken(rq::TokenType type) const {
  if (this->getIsDone()) {
    return false;
  }
  const bool is_token = this->getToken().getType() == type;
  return is_token;
}

// NOTE:
//  This is (mostly) a recursive descent parser.

rq::Expression &NormativeParser::parseExpressions() {
  if (this->getIsDone()) {
    rq::Expression &error = this->getContext().acquireExpression();
    error.setKeyword(rq::Keyword::__ERROR);
    return error;
  }
  rq::Expression &first = this->parseExpression();
  this->checkTokenIsTrailingSemicolonOperator(first);
  rq::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    rq::Expression &previous = rq::dereferencePtr(previous_ptr);
    rq::Expression &next = this->parseExpression();
    this->checkTokenIsTrailingSemicolonOperator(next);
    previous.setNext(next);
    previous_ptr = &next;
  }
  return first;
}

// STATEMENT ATTRIBUTES

rq::Expression &NormativeParser::parsePrecedence11() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  while (!this->getIsDone()) {
    const rq::Token &token = this->getToken();
    const rq::TokenType type = token.getType();
    switch (type) {
    case rq::TokenType::AT_SIGIL: {
      rq::Expression &attribute = this->parseStatementAttribute();
      precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_STATEMENT);
      precedence_parser.appendBranch(attribute);
      continue;
    }
    default:
      precedence_parser.appendBranch(this->parsePrecedence10());
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// ASSIGNMENTS
rq::Expression &NormativeParser::parsePrecedence10() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence9());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_EQUAL_OPERATOR);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenType::PLUS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_ADD);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenType::DASH_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenType::STAR_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenType::SLASH_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenType::PERCENT_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_MODULUS);
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

// BINDINGS
rq::Expression &NormativeParser::parsePrecedence9() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence8());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getCanHaveNoSemicolon()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::COLON_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_COLON_OPERATOR);
      precedence_parser.setRecent(this->parsePrecedence8());
      continue;
    case rq::TokenType::DOUBLE_COLON_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_BITWISE_CAST);
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

// RANGES
rq::Expression &NormativeParser::parsePrecedence8() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence7());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getCanHaveNoSemicolon()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::DOT_PLUS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_ADD, this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_SUBTRACT,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_STAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_MULTIPLY,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_SLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_DIVIDE, this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_PERCENT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_MODULUS, this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_LESS, this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_GREATER,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_LESS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_LESS_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_GREATER_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_GREATER_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_DOUBLE_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_EQUAL, this->parsePrecedence7());
      continue;
    case rq::TokenType::DOT_BANG_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_NOT_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenType::DOUBLE_DOT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_INTERVAL);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case rq::TokenType::DOUBLE_DOT_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_INTERVAL_LESS);
      precedence_parser.setRecent(this->parsePrecedence7());
      continue;
    case rq::TokenType::DOUBLE_DOT_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_INTERVAL_GREATER);
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

// NARY LOGICAL
rq::Expression &NormativeParser::parsePrecedence7() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence6());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getCanHaveNoSemicolon()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::DOUBLE_AMPERSAND_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LOGICAL_AND);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenType::DOUBLE_PIPE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LOGICAL_OR);
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

// NARY COMPARISON
rq::Expression &NormativeParser::parsePrecedence6() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence5());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getCanHaveNoSemicolon()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::GREATER_OPERATOR: {
      if (this->getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const rq::Token &next_token = this->getToken(1);
      switch (const rq::TokenType next_type = next_token.getType()) {
      case rq::TokenType::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenType::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_type)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_GREATER);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenType::GREATER_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_GREATER_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenType::LESS_OPERATOR: {
      if (this->getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const rq::Token &next_token = this->getToken(1);
      switch (const rq::TokenType next_type = next_token.getType()) {
      case rq::TokenType::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenType::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_type)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LESS);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenType::LESS_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LESS_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenType::DOUBLE_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenType::BANG_EQUAL_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_NOT_EQUAL);
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

// NARY MULTIPLICATIVE ARITHMETIC
rq::Expression &NormativeParser::parsePrecedence5() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence4());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::STAR_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenType::SLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenType::PERCENT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_MODULUS);
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

// NARY ADDITIVE ARITHMETIC
rq::Expression &NormativeParser::parsePrecedence4() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence3());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::PLUS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_ADD);
      precedence_parser.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenType::DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenType::CONCATENATE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_CONCATINATE);
      precedence_parser.setRecent(this->parsePrecedence3());
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
rq::Expression &NormativeParser::parsePrecedence3() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  precedence_parser.setRecent(this->parsePrecedence2());
  while (!this->getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::DOUBLE_GREATER_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_BITWISE_SHIFT_LEFT);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenType::DOUBLE_LESS_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_BITWISE_SHIFT_RIGHT);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenType::PIPE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_BITWISE_OR);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenType::AMPERSAND_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_BITWISE_AND);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenType::CAROT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_BITWISE_XOR);
      precedence_parser.setRecent(this->parsePrecedence2());
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
rq::Expression &NormativeParser::parsePrecedence2() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  while (!this->getIsDone()) {
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::BANG_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_LOGICAL_COMPLEMENT);
      continue;
    case rq::TokenType::DASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_NEGATE);
      continue;
    case rq::TokenType::TILDE_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_BITWISE_COMPLEMENT);
      continue;
    case rq::TokenType::BACKSLASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_IDENTIFY);
      continue;
    default:
      precedence_parser.appendBranch(this->parsePrecedence1());
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// LATE UNARY OPERATORS (things get wierd here)
rq::Expression &NormativeParser::parsePrecedence1() {
  rq::PrecedenceParser precedence_parser(this->getContext());
  bool previous_call = false;
  while (!this->getIsDone()) {
    if (!previous_call) {
      const rq::Token &token = this->getToken();
      const rq::TokenType type = token.getType();
      switch (type) {
      case rq::TokenType::DOLLAR_SIGIL: {
        rq::Expression &attribute = this->parseTypeAttribute();
        precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_TYPE);
        precedence_parser.appendBranch(attribute);
        continue;
      }
      case rq::TokenType::ARROW_OPERATOR: {
        rq::Expression &operation = this->getContext().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_EXTEND);
        continue;
      }
      case rq::TokenType::HASH_OPERATOR: {
        rq::Expression &operation = this->getContext().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_ARRAY);
        continue;
      }
      case rq::TokenType::DOT_OPERATOR: {
        rq::Expression &operation = this->getContext().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_REFLECT);
        continue;
      }
      case rq::TokenType::CAROT_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_FAT_POINTER);
        continue;
      case rq::TokenType::PERCENT_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_IDENTIFY);
        continue;
      case rq::TokenType::AMPERSAND_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_REFERENCE);
        continue;
      case rq::TokenType::DOUBLE_AMPERSAND_OPERATOR:
        precedence_parser.parseDoubleUnary(token, rq::Keyword::_REFERENCE);
        continue;
      case rq::TokenType::STAR_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_POINTER);
        continue;
      case rq::TokenType::GRAVE_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_TYPE);
        precedence_parser.appendUnaryAttribute(token, rq::Keyword::MUTABLE);
        continue;
      case rq::TokenType::DOUBLE_GRAVE_OPERATOR:
        this->incrementToken(1);
        precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_TYPE);
        precedence_parser.appendUnaryAttribute(token, rq::Keyword::CONSTANT);
        continue;
      default:
        break;
      }
      if (previous_call) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      if (rq::getIsInferenceTerminator(type)) {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::_INFERENCE);
        inference.setSourceInsertedBefore(token);
        precedence_parser.appendBranch(inference);
        break;
      }
      rq::Expression &expression = this->parsePrecedence0();
      precedence_parser.setRecent(expression);
      if (expression.getCanHaveNoSemicolon()) {
        precedence_parser.appendRecent();
        break;
      }
    }
    previous_call = false;
    if (this->getIsDone()) {
      precedence_parser.appendRecent();
      break;
    }
    const rq::Token &post_token = this->getToken();
    switch (post_token.getType()) {
    case rq::TokenType::HASH_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_ARRAY);
      continue;
    case rq::TokenType::ARROW_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_EXTEND);
      continue;
    case rq::TokenType::DOT_OPERATOR:
      this->incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_REFLECT);
      continue;
    case rq::TokenType::LEFT_PARENTHESIS_GROUPING: {
      this->incrementToken(1);
      precedence_parser.appendRecent();
      rq::Expression &callee = precedence_parser.getOuter();
      rq::Expression &call = this->getContext().acquireExpression();
      call.setKeyword(rq::Keyword::_CALL);
      call.setBranch(callee);
      call.setSource(callee, post_token);
      std::ignore = this->parseCommaSeperatedBranches(
          call, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, true);
      precedence_parser.setOnlyRecent(call);
      previous_call = true;
      continue;
    }
    case rq::TokenType::LEFT_BRACE_GROUPING: {
      this->incrementToken(1);
      precedence_parser.appendRecent();
      rq::Expression &target = precedence_parser.getOuter();
      rq::Expression &specialization = this->getContext().acquireExpression();
      specialization.setKeyword(rq::Keyword::_SPECIALIZATION);
      specialization.setBranch(target);
      specialization.setSource(target, post_token);
      std::ignore = this->parseCommaSeperatedBranches(
          specialization, rq::TokenType::RIGHT_BRACE_GROUPING, true);
      precedence_parser.setOnlyRecent(specialization);
      previous_call = true;
      continue;
    }
    default:
      precedence_parser.appendRecent();
      break;
    }
    break;
  }
  return precedence_parser.getOuter();
}

// BASE EXPRESSIONS
rq::Expression &NormativeParser::parsePrecedence0() {
  const rq::Token &token = this->getToken();
  switch (token.getType()) {
  case rq::TokenType::LEFT_BRACKET_GROUPING:
    return this->parseEnclosedBracketExpression();
  case rq::TokenType::LEFT_PARENTHESIS_GROUPING:
    return this->parseEnclosedParenthesisExpression();
  case rq::TokenType::LEFT_BRACE_GROUPING:
    return this->parseEnclosedBraceExpression();
  case rq::TokenType::IDENTIFIER_LITERAL:
    return this->parseLiteral(rq::Keyword::__IDENTIFIER_LITERAL);
  case rq::TokenType::CODEUNIT_LITERAL:
    return this->parseLiteral(rq::Keyword::__CODEUNIT_LITERAL);
  case rq::TokenType::STRING_LITERAL:
    return this->parseLiteral(rq::Keyword::__STRING_LITERAL);
  case rq::TokenType::INTEGER_LITERAL:
    return this->parseLiteral(rq::Keyword::__INTEGER_LITERAL);
  case rq::TokenType::FLOAT_LITERAL:
    return this->parseLiteral(rq::Keyword::__FLOAT_LITERAL);
  case rq::TokenType::LEFT_INTERPOLATION_LITERAL:
    return this->parseInterpolatedString();
  default:
    break;
  }
  this->incrementToken(1);
  this->getContext().logErrorUnexpectedToken(token);
  this->setNotOk();
  rq::Expression &error = this->getContext().acquireExpression();
  error.setKeyword(rq::Keyword::__ERROR);
  error.setSource(token);
  return error;
}

bool NormativeParser::parseCommaSeperatedBranches(
    rq::Expression &operation, rq::TokenType end,
    bool must_not_have_parameter_marks) {
  RQ_ASSERT(!operation.getHasSemicolonSeparatedBranches(),
            "operation has semicolon seperated branches");
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedExpression(operation);
    this->setNotOk();
    return false;
  }
  bool found_invalid_parameter_mark = false;
  rq::GroupingParser grouping_parser;
  grouping_parser.startGroup(operation);
  bool has_parameter_marks = false;
  while (!this->getIsDone()) {
    const rq::Token &first_token = this->getToken();
    if (first_token.getType() == end) {
      this->incrementToken(1);
      grouping_parser.finishOperation(first_token);
      return has_parameter_marks;
    }
    while (!this->getIsDone()) {
      while (!this->getIsDone()) {
        const rq::Token &before_token = this->getToken();
        const rq::TokenType before_type = before_token.getType();
        if (before_type == end) {
          this->incrementToken(1);
          grouping_parser.finishOperation(before_token);
          return has_parameter_marks;
        } else if (before_type == rq::TokenType::GREATER_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(before_token);
            this->setNotOk();
          }
          rq::Expression &mark = this->getContext().acquireExpression();
          mark.setKeyword(rq::Keyword::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(before_token);
          grouping_parser.appendBranch(mark);
          this->incrementToken(1);
          continue;
        } else if (before_type == rq::TokenType::LESS_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(before_token);
            this->setNotOk();
          }
          rq::Expression &mark = this->getContext().acquireExpression();
          mark.setKeyword(rq::Keyword::_POSITIONAL_PARAMETERS_END);
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
      rq::Expression &branch = this->parseExpression();
      grouping_parser.appendBranch(branch);
      while (!this->getIsDone()) {
        const rq::Token &after_token = this->getToken();
        const rq::TokenType after_type = after_token.getType();
        if (after_type == end) {
          this->incrementToken(1);
          grouping_parser.finishOperation(after_token);
          return has_parameter_marks;
        } else if (after_type == rq::TokenType::GREATER_OPERATOR) {
          this->incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(after_token);
            this->setNotOk();
          }
          rq::Expression &mark = this->getContext().acquireExpression();
          mark.setKeyword(rq::Keyword::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (after_type == rq::TokenType::LESS_OPERATOR) {
          this->incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(after_token);
            this->setNotOk();
          }
          rq::Expression &mark = this->getContext().acquireExpression();
          mark.setKeyword(rq::Keyword::_POSITIONAL_PARAMETERS_END);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (after_type != rq::TokenType::COMMA_SEPERATOR) {
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

rq::Keyword NormativeParser::parseOperationKeyword() {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &token = this->getToken();
  this->incrementToken(1);
  const rq::TokenType type = token.getType();
  rq::Keyword keyword;
  if (type == rq::TokenType::IDENTIFIER_LITERAL) {
    keyword = this->getContext().getKeyword(token.getSourceText());
  } else {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        "expected identifier literal", {token.getLlvmSourceRange()}, {});
    return rq::Keyword::__ERROR;
  }
  if (keyword == rq::Keyword::__NONE) {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(type)) + " does not represent a keyword",
        {token.getLlvmSourceRange()}, {});
    return rq::Keyword::__ERROR;
  }
  if (rq::getIsInternal(keyword)) {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getName(keyword)) + " is for internal use only",
        {token.getLlvmSourceRange()}, {});
    return rq::Keyword::__ERROR;
  }
  if (rq::getIsSymbolic(keyword)) {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getName(keyword)) + " is for symbolic requite only",
        {token.getLlvmSourceRange()}, {});
    return rq::Keyword::__ERROR;
  }
  return keyword;
}

rq::Keyword NormativeParser::parseTypeAttributeKeyword() {
  const rq::Token &token = this->getToken();
  rq::Keyword keyword = this->parseOperationKeyword();
  if (keyword != rq::Keyword::__ERROR) {
    if (!rq::getCanBeTypeAttribute(keyword)) {
      this->setNotOk();
      this->getContext().logMessage(
          token.getLlvmSourceStart(), rq::LogType::ERROR,
          llvm::Twine(token.getSourceText()) + " is not type attribute keyword",
          {token.getLlvmSourceRange()}, {});
      return rq::Keyword::__ERROR;
    }
    return keyword;
  }
  return rq::Keyword::__ERROR;
}

rq::Keyword NormativeParser::parseStatementAttributeKeyword() {
  const rq::Token &token = this->getToken();
  rq::Keyword keyword = this->parseOperationKeyword();
  if (keyword != rq::Keyword::__ERROR) {
    if (!rq::getCanBeStatementAttribute(keyword)) {
      this->setNotOk();
      this->getContext().logMessage(token.getLlvmSourceStart(),
                                    rq::LogType::ERROR,
                                    llvm::Twine(token.getSourceText()) +
                                        " is not statement attribute keyword",
                                    {token.getLlvmSourceRange()}, {});
      return rq::Keyword::__ERROR;
    }
    return keyword;
  }
  return rq::Keyword::__ERROR;
}

rq::Expression &NormativeParser::parseEnclosedBracketExpression() {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &left_token = this->getToken();
  this->incrementToken(1);
  const rq::Token &keyword_token = this->getToken();
  rq::Expression &operation = this->getContext().acquireExpression();
  if (keyword_token.getType() ==
      rq::TokenType::LEFT_PARENTHESIS_GROUPING) { // its an
                                                  // _anonymous_function
                                                  // expression
    operation.setKeyword(rq::Keyword::_ANONYMOUS_FUNCTION);
    operation.setSource(left_token);
    rq::GroupingParser parser;
    parser.startGroup(operation);
    rq::Expression &capture = this->getContext().acquireExpression();
    capture.setKeyword(rq::Keyword::_DYNAMIC_CAPTURE);
    capture.setSource(keyword_token);
    this->incrementToken(1);
    std::ignore = this->parseCommaSeperatedBranches(
        capture, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, true);
    parser.appendBranch(capture);
  } else {
    const rq::Keyword keyword = this->parseOperationKeyword();
    operation.setKeyword(keyword);
    operation.setSource(left_token);
  }
  if (operation.getHasSemicolonSeparatedBranches()) {
    const unsigned comma_count = operation.getCommaBranchCount();
    rq::GroupingParser parser;
    parser.startGroup(operation);
    unsigned branch_i = 0;
    while (branch_i < comma_count) {
      const rq::Token &before_token = this->getToken();
      const rq::TokenType before_type = before_token.getType();
      if (before_type == rq::TokenType::RIGHT_BRACKET_GROUPING) {
        for (unsigned inferrence_i = branch_i; inferrence_i < comma_count;
             inferrence_i++) {
          rq::Expression &inference = this->getContext().acquireExpression();
          inference.setKeyword(rq::Keyword::_TACIT_COMMA_EXPRESSION);
          inference.setSourceInsertedBefore(before_token);
          parser.appendBranch(inference);
        }
        this->incrementToken(1);
        parser.finishOperation(before_token);
        return operation;
      }
      rq::Expression &next = this->parseExpression();
      const rq::Token &after_token = this->getToken();
      const rq::TokenType after_type = after_token.getType();
      if (after_type == rq::TokenType::SEMICOLON_SEPERATOR ||
          next.getCanHaveNoSemicolon()) {
        for (unsigned inferrence_i = branch_i; inferrence_i < comma_count;
             inferrence_i++) {
          rq::Expression &inference = this->getContext().acquireExpression();
          inference.setKeyword(rq::Keyword::_TACIT_COMMA_EXPRESSION);
          inference.setSourceInsertedBefore(after_token);
          parser.appendBranch(inference);
        }
        if (after_type == rq::TokenType::SEMICOLON_SEPERATOR) {
          this->incrementToken(1);
        }
        break;
      }
      branch_i++;
      parser.appendBranch(next);
      switch (after_type) {
      case rq::TokenType::COMMA_SEPERATOR:
        this->incrementToken(1);
        break;
      case rq::TokenType::RIGHT_BRACKET_GROUPING:
        break;
      default:
        this->getContext().logErrorExpectedCommaSeperator(after_token);
        this->setNotOk();
        break;
      }
    }
    while (!this->getIsDone()) { // semicolons
      const rq::Token &before_token = this->getToken();
      const rq::TokenType before_type = before_token.getType();
      if (before_type == rq::TokenType::RIGHT_BRACKET_GROUPING) {
        this->incrementToken(1);
        parser.finishOperation(before_token);
        return operation;
      }
      rq::Expression &next = this->parseExpression();
      parser.appendBranch(next);
      const rq::Token &after_token = this->getToken();
      const rq::TokenType after_type = after_token.getType();
      switch (after_type) {
      case rq::TokenType::SEMICOLON_SEPERATOR:
        this->incrementToken(1);
        break;
      case rq::TokenType::COMMA_SEPERATOR:
        this->incrementToken(1);
        this->getContext().logErrorExpectedSemicolonSeperator(after_token);
        this->setNotOk();
        break;
      case rq::TokenType::RIGHT_BRACKET_GROUPING:
        if (!next.getCanHaveNoSemicolon()) {
          this->getContext().logErrorExpectedSemicolonSeperator(after_token);
          this->setNotOk();
          break;
        }
        break;
      default:
        if (operation.getCanHaveNoSemicolon()) {
          break;
        }
        this->getContext().logErrorExpectedSemicolonSeperator(after_token);
        this->setNotOk();
        break;
      }
    }
    this->getContext().logErrorUnterminatedExpression(operation);
    this->setNotOk();
    return operation;
  }
  std::ignore = this->parseCommaSeperatedBranches(
      operation, rq::TokenType::RIGHT_BRACKET_GROUPING, true);
  return operation;
}

rq::Expression &NormativeParser::parseEnclosedBraceExpression() {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &first_token = this->getToken();
  rq::Expression &brace = this->getContext().acquireExpression();
  brace.setKeyword(rq::Keyword::_TUPLE);
  brace.setSource(first_token);
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedExpression(brace);
    this->setNotOk();
    return brace;
  }
  const rq::Token &second_token = this->getToken();
  if (this->getIsDone(1)) {
    this->getContext().logErrorUnterminatedExpression(brace);
    this->setNotOk();
    return brace;
  }
  switch (second_token.getType()) {
  case rq::TokenType::GREATER_OPERATOR: {
    const rq::Token &third_token = this->getToken(1);
    if (this->getIsDone(2)) {
      this->getContext().logErrorUnterminatedExpression(brace);
      this->setNotOk();
      return brace;
    }
    switch (third_token.getType()) {
    case rq::TokenType::LESS_OPERATOR: {
      const rq::Token &fourth_token = this->getToken(2);
      switch (fourth_token.getType()) {
      case rq::TokenType::RIGHT_BRACE_GROUPING: {
        brace.changeKeyword(rq::Keyword::_NULL_TYPE);
        brace.extendSourceOver(fourth_token);
        this->incrementToken(3);
        return brace;
      } break;
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case rq::TokenType::LESS_OPERATOR: {
    const rq::Token &third_token = this->getToken(1);
    if (this->getIsDone(2)) {
      this->getContext().logErrorUnterminatedExpression(brace);
      this->setNotOk();
      return brace;
    }
    switch (third_token.getType()) {
    case rq::TokenType::GREATER_OPERATOR: {
      const rq::Token &fourth_token = this->getToken(2);
      switch (fourth_token.getType()) {
      case rq::TokenType::RIGHT_BRACE_GROUPING: {
        brace.changeKeyword(rq::Keyword::_NULL_TYPE);
        brace.extendSourceOver(fourth_token);
        this->incrementToken(3);
        return brace;
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
      brace, rq::TokenType::RIGHT_BRACE_GROUPING, false);
  if (has_parameter_marks) {
    brace.changeKeyword(rq::Keyword::_LAYOUT);
  } else if (!brace.getHasBranch()) {
    brace.changeKeyword(rq::Keyword::_NULL);
  }
  return brace;
}

rq::Expression &NormativeParser::parseStatementAttribute() {
  const rq::Token &at_token = this->getToken();
  RQ_ASSERT(at_token.getType() == rq::TokenType::AT_SIGIL, "not at sigil");
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedStatementAttribute(at_token);
    this->setNotOk();
    rq::Expression &error = this->getContext().acquireExpression();
    error.setKeyword(rq::Keyword::__ERROR);
    error.setSource(at_token);
    return error;
  }
  const rq::Token &next_token = this->getToken();
  if (next_token.getType() == rq::TokenType::LEFT_BRACKET_GROUPING) {
    this->incrementToken(1);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedStatementAttribute(at_token);
      this->setNotOk();
      rq::Expression &error = this->getContext().acquireExpression();
      error.setKeyword(rq::Keyword::__ERROR);
      error.setSource(at_token, next_token);
      return error;
    }
    rq::Keyword keyword = this->parseStatementAttributeKeyword();
    rq::Expression &attribute = this->getContext().acquireExpression();
    attribute.setKeyword(keyword);
    attribute.setSource(at_token);
    std::ignore = this->parseCommaSeperatedBranches(
        attribute, rq::TokenType::RIGHT_BRACKET_GROUPING, true);
    return attribute;
  } else if (next_token.getType() == rq::TokenType::LEFT_BRACE_GROUPING) {
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedStatementAttribute(at_token);
      this->setNotOk();
      rq::Expression &error = this->getContext().acquireExpression();
      error.setKeyword(rq::Keyword::__ERROR);
      error.setSource(at_token, next_token);
      return error;
    }
    rq::Expression &attribute = this->getContext().acquireExpression();
    attribute.setKeyword(rq::Keyword::TEMPLATE);
    attribute.setSource(at_token);
    this->incrementToken(1);
    bool has_parameter_marks = this->parseCommaSeperatedBranches(
        attribute, rq::TokenType::RIGHT_BRACE_GROUPING, false);
    if (!has_parameter_marks) {
      this->getContext().logErrorMustHaveParameterMarks(attribute);
      this->setNotOk();
      return attribute;
    }
    return attribute;
  } else if (next_token.getType() == rq::TokenType::LEFT_PARENTHESIS_GROUPING) {
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedStatementAttribute(at_token);
      this->setNotOk();
      rq::Expression &error = this->getContext().acquireExpression();
      error.setKeyword(rq::Keyword::__ERROR);
      error.setSource(at_token, next_token);
      return error;
    }
    rq::Expression &attribute = this->getContext().acquireExpression();
    attribute.setKeyword(rq::Keyword::STATIC_CAPTURE);
    attribute.setSource(at_token);
    this->incrementToken(1);
    std::ignore = this->parseCommaSeperatedBranches(
        attribute, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, true);
    return attribute;
  }
  const rq::Token &keyword_token = this->getToken();
  rq::Keyword keyword = this->parseStatementAttributeKeyword();
  rq::Expression &attribute = this->getContext().acquireExpression();
  attribute.setKeyword(keyword);
  attribute.setSource(at_token, keyword_token);
  return attribute;
}

rq::Expression &NormativeParser::parseTypeAttribute() {
  const rq::Token &dollar_token = this->getToken();
  RQ_ASSERT(dollar_token.getType() == rq::TokenType::DOLLAR_SIGIL,
            "not dollar sigil");
  this->incrementToken(1);
  if (this->getIsDone()) {
    this->getContext().logErrorUnterminatedTypeAttribute(dollar_token);
    this->setNotOk();
    rq::Expression &error = this->getContext().acquireExpression();
    error.setKeyword(rq::Keyword::__ERROR);
    error.setSource(dollar_token);
    return error;
  }
  const rq::Token &next_token = this->getToken();
  if (next_token.getType() == rq::TokenType::LEFT_PARENTHESIS_GROUPING) {
    this->incrementToken(1);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedTypeAttribute(dollar_token);
      this->setNotOk();
      rq::Expression &error = this->getContext().acquireExpression();
      error.setKeyword(rq::Keyword::__ERROR);
      error.setSource(dollar_token, next_token);
      return error;
    }
    rq::Keyword keyword = this->parseStatementAttributeKeyword();
    rq::Expression &attribute = this->getContext().acquireExpression();
    attribute.setKeyword(keyword);
    attribute.setSource(dollar_token);
    std::ignore = this->parseCommaSeperatedBranches(
        attribute, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, true);
    return attribute;
  } else if (next_token.getType() == rq::TokenType::LEFT_PARENTHESIS_GROUPING) {
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedStatementAttribute(dollar_token);
      this->setNotOk();
      rq::Expression &error = this->getContext().acquireExpression();
      error.setKeyword(rq::Keyword::__ERROR);
      error.setSource(dollar_token, next_token);
      return error;
    }
    rq::Expression &attribute = this->getContext().acquireExpression();
    attribute.setKeyword(rq::Keyword::DYNAMIC_CAPTURE_LAYOUT);
    attribute.setSource(dollar_token);
    this->incrementToken(1);
    bool has_parameter_marks = this->parseCommaSeperatedBranches(
        attribute, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, false);
    if (!has_parameter_marks) {
      this->getContext().logErrorMustHaveParameterMarks(attribute);
      this->setNotOk();
      return attribute;
    }
    return attribute;
  }
  const rq::Token &keyword_token = this->getToken();
  rq::Keyword keyword = this->parseStatementAttributeKeyword();
  rq::Expression &attribute = this->getContext().acquireExpression();
  attribute.setKeyword(keyword);
  attribute.setSource(dollar_token, keyword_token);
  return attribute;
}

rq::Expression &NormativeParser::parseEnclosedParenthesisExpression() {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &first_token = this->getToken();
  rq::Expression &parenthesis = this->getContext().acquireExpression();
  parenthesis.setKeyword(rq::Keyword::_PARENTHESIS_GROUP);
  parenthesis.setSource(first_token);
  this->incrementToken(1);
  bool has_parameter_marks = this->parseCommaSeperatedBranches(
      parenthesis, rq::TokenType::RIGHT_PARENTHESIS_GROUPING, false);
  if (has_parameter_marks) {
    parenthesis.changeKeyword(rq::Keyword::_SIGNATURE);
    if (this->getIsDone()) {
      this->getContext().logErrorUnterminatedExpression(parenthesis);
      this->setNotOk();
      return parenthesis;
    }
    rq::Expression &return_type = this->parseExpression();
    parenthesis.extendSourceOver(return_type);
    if (parenthesis.getHasBranch()) {
      return_type.setNext(parenthesis.replaceBranch(return_type));
    } else {
      parenthesis.setBranch(return_type);
    }
  }
  return parenthesis;
}

rq::Expression &NormativeParser::parseLiteral(rq::Keyword keyword) {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &token = this->getToken();
  RQ_ASSERT(token.getIsLiteral(), "token is not literal");
  rq::Expression &identifier = this->getContext().acquireExpression();
  identifier.setKeyword(keyword);
  identifier.setSource(token);
  this->incrementToken(1);
  return identifier;
}

rq::Expression &NormativeParser::parseNullaryOperator(rq::Keyword keyword) {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &token = this->getToken();
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->incrementToken(1);
  return expression;
}

rq::Expression &NormativeParser::parseInterpolatedString() {
  RQ_ASSERT(!this->getIsDone(), "parser is done");
  const rq::Token &left_token = this->getToken();
  rq::Expression *first_ptr = nullptr;
  rq::Expression *previous_ptr = nullptr;
  while (!this->getIsDone()) {
    const rq::Token &token = this->getToken();
    switch (token.getType()) {
    case rq::TokenType::LEFT_INTERPOLATION_LITERAL: {
      RQ_ASSERT(first_ptr == nullptr && previous_ptr == nullptr,
                "left interpolated string literal must be first");
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::__LEFT_INTERPOLATION_LITERAL);
      string.setSource(token);
      first_ptr = &string;
      previous_ptr = &string;
      this->incrementToken(1);
      continue;
    }
    case rq::TokenType::MIDDLE_INTERPOLATION_LITERAL: {
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::__MIDDLE_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      this->incrementToken(1);
      continue;
    }
    case rq::TokenType::RIGHT_INTERPOLATION_LITERAL: {
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::__RIGHT_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      rq::Expression &tuple = this->getContext().acquireExpression();
      tuple.setKeyword(rq::Keyword::_TUPLE);
      tuple.setSource(left_token, token);
      tuple.setBranch(first_ptr);
      this->incrementToken(1);
      return tuple;
    }
    case rq::TokenType::LEFT_BRACE_GROUPING: {
      rq::Expression &interpolation = this->parseEnclosedBraceExpression();
      rq::dereferencePtr(previous_ptr).setNext(interpolation);
      previous_ptr = &interpolation;
      continue;
    }
    default:
      break;
    }
  }
  this->getContext().logMessage(left_token.getLlvmSourceStart(),
                                rq::LogType::ERROR,
                                "found unterminated interpolated string",
                                {left_token.getLlvmSourceRange()}, {});
  this->setNotOk();
  rq::Expression &error = this->getContext().acquireExpression();
  error.setKeyword(rq::Keyword::__ERROR);
  return error;
}

void NormativeParser::checkTokenIsTrailingSemicolonOperator(
    rq::Expression &expression) {
  if (expression.getCanBeAscription()) {
    rq::Expression &unascribed = expression.getLastBranch();
    if (unascribed.getCanHaveNoSemicolon()) {
      return;
    }
  } else if (expression.getCanHaveNoSemicolon()) {
    return;
  }
  if (this->getIsDone()) {
    this->getContext().logErrorExpectedSemicolonSeperatorAtEndOfFile(
        expression);
    this->setNotOk();
    return;
  }
  const rq::Token &token = this->getToken();
  if (token.getType() == rq::TokenType::SEMICOLON_SEPERATOR) {
    this->incrementToken(1);
    return;
  }
  this->getContext().logErrorExpectedSemicolonSeperator(token);
  this->setNotOk();
}

rq::Expression &SymbolicParser::parseExpressions() {
  if (this->getIsDone()) {
    this->setNotOk();
    rq::Expression &error = this->getContext().acquireExpression();
    error.setKeyword(rq::Keyword::__ERROR);
    return error;
  }
  rq::Expression &first = this->parseExpression();
  rq::Expression *previous_ptr = &first;
  while (!this->getIsDone()) {
    rq::Expression &expression = this->parseExpression();
    rq::Expression &previous = rq::dereferencePtr(previous_ptr);
    previous.setNext(expression);
    previous_ptr = &expression;
  }
  return rq::dereferencePtr(previous_ptr);
}

rq::Expression &SymbolicParser::parseExpression() {
  // TODO
  this->setNotOk();
  rq::Expression &error = this->getContext().acquireExpression();
  error.setKeyword(rq::Keyword::__ERROR);
  return error;
}

} // namespace rq