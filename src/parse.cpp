#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/parse.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

namespace rq {

void ForestFactory::appendTree(rq::Expression &branch) {
  [[unlikely]] if (!this->getHasExpression()) {
    this->setExpression(branch);
    this->setLast(branch);
    return;
  }
  rq::Expression &last = this->getLast();
  last.setNext(branch);
  this->setLast(branch);
}

void TreeFactory::startTree(rq::Expression &trunk) {
  RQ_ASSERT(!trunk.getHasNext(), "trunk must not have next");
  [[unlikely]] if (trunk.getHasBranch()) {
    rq::Expression &branch = trunk.getBranch();
    RQ_ASSERT(!branch.getHasNext(), "branch must not have next");
    this->setLast(branch);
  }
  this->setExpression(trunk);
}

void TreeFactory::appendBranch(rq::Expression &branch) {
  [[unlikely]] if (this->_last_ptr == nullptr) {
    rq::Expression &expression = this->getExpression();
    expression.setBranch(branch);
    this->setLast(branch);
    return;
  }
  rq::Expression &last = rq::dereferencePtr(this->_last_ptr);
  last.setNext(branch);
  this->setLast(branch);
}

void TreeFactory::finishExpression(const rq::Token &last_token) {
  rq::Expression &expression = this->getExpression();
  expression.extendSourceOver(last_token);
}

void PrecedenceFactory::parseUnary(const rq::Token &token,
                                   rq::Keyword keyword) {
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->appendBranch(expression);
  this->_expression_ptr = &expression;
  this->_last_ptr = nullptr;
}

void PrecedenceFactory::parseAscribe(const rq::Token &token,
                                     rq::Keyword keyword) {
  if (this->getHasExpression()) {
    rq::Expression &old_expression = this->getExpression();
    if (old_expression.getKeyword() != keyword) {
      rq::Expression &new_expression = this->getContext().acquireExpression();
      new_expression.setKeyword(keyword);
      new_expression.setSource(old_expression, token);
      this->appendBranch(new_expression);
      if (!this->getHasOuter()) {
        this->_outer_ptr = &new_expression;
      }
      this->_expression_ptr = &new_expression;
      this->_last_ptr = nullptr;
    }
    return;
  }
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  if (this->getHasLast()) {
    rq::Expression &last = this->getLast();
    expression.setSource(last);
    expression.setBranch(last);
  } else {
    expression.setSource(token);
  }
  if (!this->getHasOuter()) {
    this->_outer_ptr = &expression;
  }
  this->_expression_ptr = &expression;
}

void PrecedenceFactory::parseBinary(const rq::Token &token,
                                    rq::Keyword keyword) {
  rq::Expression &new_expression = this->getContext().acquireExpression();
  new_expression.setKeyword(keyword);
  new_expression.setSource(this->getRecent(), token);
  this->appendBranch(new_expression);
  this->_expression_ptr = &new_expression;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceFactory::parseOuterBinary(const rq::Token &token,
                                         rq::Keyword keyword) {
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(this->getOuter(), token);
  expression.setBranch(this->getOuter());
  this->_expression_ptr = &expression;
  this->_last_ptr = this->_outer_ptr;
  this->_outer_ptr = &expression;
}

void PrecedenceFactory::parseNary(const rq::Token &token, rq::Keyword keyword) {
  if (this->getHasExpression()) {
    rq::Expression &existing_expression = this->getExpression();
    if (existing_expression.getKeyword() == keyword) {
      // the existing expression already has this keyword, so we can keep
      // appending to this one
      this->appendRecent();
      return;
    }
  }
  // need to make a new expression of this keyword because one does not exist
  // yet
  rq::Expression &new_expression = this->getContext().acquireExpression();
  new_expression.setKeyword(keyword);
  new_expression.setSource(this->getRecent(), token);
  this->appendBranch(new_expression);
  this->_expression_ptr = &new_expression;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceFactory::parseSequenceBranch(const rq::Token &token,
                                            rq::Keyword keyword,
                                            rq::Expression &rvalue) {
  this->parseNary(token, rq::Keyword::ARITHMETIC_SEQUENCE);
  rq::Expression &step = this->getContext().acquireExpression();
  step.setKeyword(keyword);
  step.setSource(token, rvalue);
  step.setBranch(rvalue);
  this->setRecent(step);
}

void PrecedenceFactory::appendBranch(rq::Expression &branch) {
  if (!this->getHasOuter()) {
    this->_outer_ptr = &branch;
  }
  if (this->getHasLast()) {
    this->getLast().setNext(branch);
  }
  if (this->getHasExpression()) {
    rq::Expression &expression = this->getExpression();
    if (!this->getHasLast()) {
      expression.setBranch(branch);
    }
    expression.extendSourceOver(branch);
  }
  this->_last_ptr = &branch;
}

void PrecedenceFactory::appendNullaryAttribute(const rq::Token &token,
                                               rq::Keyword keyword) {
  rq::Expression &expression = this->getContext().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->appendBranch(expression);
}

void PrecedenceFactory::setRecent(rq::Expression &branch) {
  rq::assignSingleValue(this->_recent_ptr, &branch);
}

void PrecedenceFactory::setOnlyRecent(rq::Expression &branch) {
  this->_outer_ptr = nullptr;
  this->_expression_ptr = nullptr;
  this->_last_ptr = nullptr;
  this->_recent_ptr = &branch;
}

void PrecedenceFactory::appendRecent() {
  this->appendBranch(this->getRecent());
  this->_recent_ptr = nullptr;
}

bool TokenRanger::getIsToken(rq::TokenKind kind) const {
  if (this->getIsDone()) {
    return false;
  }
  const bool is_token = this->getToken().getKind() == kind;
  return is_token;
}

// NOTE:
//  This is (mostly) a recursive descent parser.

rq::Expression *RequiteParser::parseExpressions() {
  rq::ForestFactory factory;
  while (!this->getRanger().getIsDone()) {
    rq::Expression &next = this->parseExpression();
    factory.appendTree(next);
    if (this->getRanger().getIsDone()) {
      this->getContext().logErrorExpectedSemicolonSeparator(next);
      this->setNotOk();
      break;
    }
    const rq::Token &after_token = this->getRanger().getToken();
    this->getRanger().incrementToken(1);
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getContext().logErrorUnexpectedToken(after_token);
      this->setNotOk();
    } else if (after_token.getKind() != rq::TokenKind::SEMICOLON_SEPARATOR) {
      next.setIsChainLink();
    }
  }
  return factory.getExpressionPtr();
}

// SYMBOL ATTRIBUTES
rq::Expression &RequiteParser::parsePrecedence12() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    const rq::TokenKind kind = token.getKind();
    switch (kind) {
    case rq::TokenKind::AT_SIGIL: {
      rq::Expression &attribute = this->parseAttribute();
      precedence_factory.parseAscribe(
          token, rq::Keyword::UNSITUATED_ASCRIBE_STATEMENT);
      precedence_factory.appendBranch(attribute);
      continue;
    }
    default:
      precedence_factory.appendBranch(this->parsePrecedence11());
      break;
    }
    break;
  }
  return precedence_factory.getOuter();
}

// ASSIGNMENTS
rq::Expression &RequiteParser::parsePrecedence11() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence10());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token,
                                     rq::Keyword::UNSITUATED_EQUAL_OPERATOR);
      precedence_factory.setRecent(this->parsePrecedence10());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// BINDINGS
rq::Expression &RequiteParser::parsePrecedence10() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence9());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::COLON_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BINDING);
      precedence_factory.setRecent(this->parsePrecedence9());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// RANGES
rq::Expression &RequiteParser::parsePrecedence9() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence8());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOT_PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_ADD,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_SUBTRACT,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_MULTIPLY,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_DIVIDE,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_MODULUS,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_LESS,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_GREATER,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_EQUAL,
          this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOT_BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL,
          this->parsePrecedence8());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// NARY LOGICAL
rq::Expression &RequiteParser::parsePrecedence8() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence7());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOWN_ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LOGICAL_AND);
      precedence_factory.setRecent(this->parsePrecedence7());
      continue;
    case rq::TokenKind::UP_ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LOGICAL_OR);
      precedence_factory.setRecent(this->parsePrecedence7());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// NARY COMPARISON
rq::Expression &RequiteParser::parsePrecedence7() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence6());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::GREATER_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_factory.appendRecent();
          return precedence_factory.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::GREATER);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    }
    case rq::TokenKind::GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::GREATER_EQUAL);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::LESS_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_factory.appendRecent();
          return precedence_factory.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LESS);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    }
    case rq::TokenKind::LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LESS_EQUAL);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::EQUAL);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::NOT_EQUAL);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// NARY MULTIPLICATIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence6() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence5());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::MULTIPLY);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::DIVIDE);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::MODULUS);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// NARY ADDITIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence5() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence4());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::ADD);
      precedence_factory.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::SUBTRACT);
      precedence_factory.setRecent(this->parsePrecedence4());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// NARY AND BINARY BITWISE
rq::Expression &RequiteParser::parsePrecedence4() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence3());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BITWISE_SHIFT_LEFT);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::DOUBLE_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BITWISE_SHIFT_RIGHT);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_OR);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_AND);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::CAROT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_XOR);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// EARLY UNARY OPERATORS
rq::Expression &RequiteParser::parsePrecedence3() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::BANG_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::LOGICAL_COMPLEMENT);
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::NEGATE);
      continue;
    case rq::TokenKind::TILDE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::BITWISE_COMPLEMENT);
      continue;
    case rq::TokenKind::BACKSLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::IDENTIFY);
      continue;
    case rq::TokenKind::WHAT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::KEWORDIFY);
      continue;
    case rq::TokenKind::DOUBLE_DOT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseUnary(token, rq::Keyword::ASCEND_FRAME);
      continue;
    default:
      precedence_factory.appendBranch(this->parsePrecedence2());
      break;
    }
    break;
  }
  return precedence_factory.getOuter();
}

// ASCEND FRAME AND ARROWS
rq::Expression &RequiteParser::parsePrecedence2() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence1());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_DOT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::ASCEND_FRAME_OF);
      precedence_factory.setRecent(this->parsePrecedence1());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_factory.appendRecent();
  return precedence_factory.getOuter();
}

// LATE UNARY OPERATORS (things get wierd here)
rq::Expression &RequiteParser::parsePrecedence1() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  bool previous_horned = false;
  while (!this->getRanger().getIsDone()) {
    if (!previous_horned) {
      const rq::Token &token = this->getRanger().getToken();
      const rq::TokenKind kind = token.getKind();
      switch (kind) {
      case rq::TokenKind::DOLLAR_SIGIL: {
        rq::Expression &attribute = this->parseAttribute();
        precedence_factory.parseAscribe(token,
                                        rq::Keyword::UNSITUATED_ASCRIBE_TYPE);
        precedence_factory.appendBranch(attribute);
        continue;
      }
      case rq::TokenKind::ARROW_OPERATOR: {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_factory.setRecent(inference);
        this->getRanger().incrementToken(1);
        precedence_factory.parseNary(token, rq::Keyword::EXTEND);
        continue;
      }
      case rq::TokenKind::THICK_ARROW_OPERATOR: {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_factory.setRecent(inference);
        this->getRanger().incrementToken(1);
        precedence_factory.parseNary(token, rq::Keyword::EXTENSION);
        continue;
      }
      case rq::TokenKind::HASH_OPERATOR: {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_factory.setRecent(inference);
        this->getRanger().incrementToken(1);
        precedence_factory.parseNary(token, rq::Keyword::ARRAY);
        continue;
      }
      case rq::TokenKind::DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::MEMBER_OF_TOP);
        continue;
      case rq::TokenKind::CAROT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::FAT_POINTER);
        continue;
      case rq::TokenKind::PERCENT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::IDENTIFY);
        continue;
      case rq::TokenKind::AMPERSAND_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::REFERENCE);
        continue;
      case rq::TokenKind::STAR_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::POINTER);
        continue;
      case rq::TokenKind::DOT_LESS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token,
                                      rq::Keyword::ARITHMETIC_INTERVAL_LESS);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::ARITHMETIC_INTERVAL_LESS_EQUAL);
        continue;
      case rq::TokenKind::DOT_GREATER_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token,
                                      rq::Keyword::ARITHMETIC_INTERVAL_GREATER);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::ARITHMETIC_INTERVAL_GREATER_EQUAL);
        continue;
      case rq::TokenKind::DOT_DOUBLE_EQUAL_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token,
                                      rq::Keyword::ARITHMETIC_INTERVAL_EQUAL);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::ARITHMETIC_INTERVAL_NOT_EQUAL);
        continue;
      case rq::TokenKind::DOT_LESS_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_ADD);
        continue;
      case rq::TokenKind::DOT_LESS_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_LESS_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_LESS_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE);
        continue;
      case rq::TokenKind::DOT_LESS_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS);
        continue;
      case rq::TokenKind::DOT_GREATER_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD);
        continue;
      case rq::TokenKind::DOT_GREATER_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_GREATER_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_GREATER_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE);
        continue;
      case rq::TokenKind::DOT_GREATER_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE);
        continue;
      case rq::TokenKind::DOT_LESS_EQUAL_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE);
        continue;
      case rq::TokenKind::DOT_GREATER_EQUAL_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS);
        continue;
      case rq::TokenKind::DOT_EQUAL_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD);
        continue;
      case rq::TokenKind::DOT_EQUAL_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_EQUAL_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_EQUAL_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE);
        continue;
      case rq::TokenKind::DOT_EQUAL_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_PLUS_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE);
        continue;
      case rq::TokenKind::DOT_BANG_EQUAL_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token,
            rq::Keyword::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS);
        continue;
      case rq::TokenKind::DOT_PLUS_DOT_OPERTATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::INFINITE_ARITHMETIC_PROGRESSION_ADD);
        continue;
      case rq::TokenKind::DOT_DASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT);
        continue;
      case rq::TokenKind::DOT_STAR_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY);
        continue;
      case rq::TokenKind::DOT_SLASH_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::INFINITE_ARITHMETIC_PROGRESSION_DIVIDE);
        continue;
      case rq::TokenKind::DOT_PERCENT_DOT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(
            token, rq::Keyword::INFINITE_ARITHMETIC_PROGRESSION_MODULUS);
        continue;
      case rq::TokenKind::GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseAscribe(token,
                                        rq::Keyword::UNSITUATED_ASCRIBE_TYPE);
        precedence_factory.appendNullaryAttribute(token, rq::Keyword::MUTABLE);
        continue;
      case rq::TokenKind::DOUBLE_GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseAscribe(token,
                                        rq::Keyword::UNSITUATED_ASCRIBE_TYPE);
        precedence_factory.appendNullaryAttribute(token, rq::Keyword::CONSTANT);
        continue;
      default:
        break;
      }
      if (previous_horned) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      if (rq::getIsInferenceTerminator(kind)) {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_factory.appendBranch(inference);
        break;
      }
      rq::Expression &expression = this->parsePrecedence0();
      precedence_factory.setRecent(expression);
      if (expression.getCanBeChainLink()) {
        precedence_factory.appendRecent();
        break;
      }
    }
    previous_horned = false;
    if (this->getRanger().getIsDone()) {
      precedence_factory.appendRecent();
      break;
    }
    const rq::Token &post_token = this->getRanger().getToken();
    switch (post_token.getKind()) {
    case rq::TokenKind::HASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token, rq::Keyword::ARRAY);
      continue;
    case rq::TokenKind::ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token, rq::Keyword::EXTEND);
      continue;
    case rq::TokenKind::THICK_ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token, rq::Keyword::EXTENSION);
      continue;
    case rq::TokenKind::DOT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(post_token, rq::Keyword::REFLECT);
      continue;
    case rq::TokenKind::LEFT_PARENTHESIS_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      rq::Expression &callee = precedence_factory.getOuter();
      rq::Expression &call = this->getContext().acquireExpression();
      call.setKeyword(rq::Keyword::CALL);
      call.setBranch(callee);
      call.setSource(callee, post_token);
      std::ignore = this->parseExpressionBranches(
          call, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
      precedence_factory.setOnlyRecent(call);
      previous_horned = true;
      continue;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      rq::Expression &target = precedence_factory.getOuter();
      rq::Expression &specialization = this->getContext().acquireExpression();
      specialization.setKeyword(rq::Keyword::SPECIALIZATION);
      specialization.setBranch(target);
      specialization.setSource(target, post_token);
      std::ignore = this->parseExpressionBranches(
          specialization, rq::TokenKind::RIGHT_BRACE_GROUPING);
      precedence_factory.setOnlyRecent(specialization);
      previous_horned = true;
      continue;
    }
    default:
      precedence_factory.appendRecent();
      break;
    }
    break;
  }
  return precedence_factory.getOuter();
}

// BASE NODES
rq::Expression &RequiteParser::parsePrecedence0() {
  const rq::Token &token = this->getRanger().getToken();
  switch (token.getKind()) {
  case rq::TokenKind::LEFT_BRACKET_GROUPING:
    return this->parseEnclosedBracketExpression();
  case rq::TokenKind::LEFT_DOUBLE_BRACKET_GROUPING:
    return this->parseEnclosedDoubleBracketExpression();
  case rq::TokenKind::LEFT_PARENTHESIS_GROUPING:
    return this->parseEnclosedParenthesisExpression();
  case rq::TokenKind::LEFT_BRACE_GROUPING:
    return this->parseEnclosedBraceExpression();
  case rq::TokenKind::IDENTIFIER_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::IDENTIFIER_LITERAL);
  case rq::TokenKind::CODEUNIT_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::CODEUNIT_LITERAL);
  case rq::TokenKind::STRING_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::STRING_LITERAL);
  case rq::TokenKind::INTEGER_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::INTEGER_LITERAL);
  case rq::TokenKind::FLOAT_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::FLOAT_LITERAL);
  case rq::TokenKind::LEFT_INTERPOLATION_LITERAL:
    return this->parseInterpolatedString();
  default:
    break;
  }
  this->getRanger().incrementToken(1);
  this->getContext().logErrorUnexpectedToken(token);
  this->setNotOk();
  rq::Expression &error = this->getContext().acquireExpression();
  error.setKeyword(rq::Keyword::ERROR);
  error.setSource(token);
  return error;
}

[[nodiscard]] bool
RequiteParser::parseExpressionBranches(rq::Expression &expression,
                                       rq::TokenKind end) {
  RQ_ASSERT(expression.getHasExpressionBranches(),
            "expression must have non-statement branches");
  rq::TreeFactory factory;
  factory.startTree(expression);
  const rq::Token &first_token = this->getRanger().getToken();
  if (first_token.getKind() == end) {
    this->getRanger().incrementToken(1);
    factory.finishExpression(first_token);
    return false;
  } else if (first_token.getKind() == rq::TokenKind::GREATER_OPERATOR) {
    const rq::Token &second_token = this->getRanger().getToken(1);
    if (second_token.getKind() == rq::TokenKind::LESS_OPERATOR) {
      const rq::Token &third_token = this->getRanger().getToken(2);
      if (third_token.getKind() == end) {
        rq::Expression &first_mark = this->getContext().acquireExpression();
        first_mark.setSource(first_token);
        first_mark.setKeyword(rq::Keyword::NAMED_PARAMETERS_BEGIN);
        factory.appendBranch(first_mark);
        rq::Expression &second_mark = this->getContext().acquireExpression();
        second_mark.setSource(second_token);
        second_mark.setKeyword(rq::Keyword::POSITIONAL_PARAMETERS_END);
        factory.appendBranch(second_mark);
        factory.finishExpression(third_token);
        this->getRanger().incrementToken(3);
        return true;
      }
    }
  }
  bool parameter_mark_found = false;
  while (true) {
    const rq::Token &next_token = this->getRanger().getToken();
    if (next_token.getKind() == rq::TokenKind::GREATER_OPERATOR) {
      this->getRanger().incrementToken(1);
      parameter_mark_found = true;
      rq::Expression &mark = this->getContext().acquireExpression();
      mark.setSource(next_token);
      mark.setKeyword(rq::Keyword::NAMED_PARAMETERS_BEGIN);
      factory.appendBranch(mark);
    }
    rq::Expression &branch = this->parseExpression();
    factory.appendBranch(branch);
    const rq::Token &after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getRanger().incrementToken(1);
      continue;
    } else if (after_token.getKind() == end) {
      this->getRanger().incrementToken(1);
      factory.finishExpression(after_token);
      return parameter_mark_found;
    } else if (after_token.getKind() == rq::TokenKind::LESS_OPERATOR) {
      this->getRanger().incrementToken(1);
      parameter_mark_found = true;
      rq::Expression &mark = this->getContext().acquireExpression();
      mark.setSource(next_token);
      mark.setKeyword(rq::Keyword::POSITIONAL_PARAMETERS_END);
      factory.appendBranch(mark);
      const rq::Token &next_after = this->getRanger().getToken();
      if (next_after.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
        this->getRanger().incrementToken(1);
        continue;
      } else if (next_after.getKind() == end) {
        this->getRanger().incrementToken(1);
        factory.finishExpression(next_after);
        return parameter_mark_found;
      }
      this->getContext().logErrorExpectedCommaSeparator(branch);
      this->setNotOk();
    } else {
      this->getRanger().incrementToken(1);
      this->getContext().logErrorExpectedCommaSeparator(branch);
      this->setNotOk();
      continue;
    }
  }
  RQ_UNREACHABLE();
}

rq::Keyword RequiteParser::parseKeyword() {
  const rq::Token &token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  rq::Keyword keyword;
  if (token.getKind() == rq::TokenKind::IDENTIFIER_LITERAL) {
    keyword = this->getContext().getKeyword(token.getSourceText());
  } else {
    this->getContext().logErrorExpectedIdentifierLiteral(token);
    this->setNotOk();
    return rq::Keyword::ERROR;
  }
  if (keyword != rq::Keyword::NONE) {
    this->getContext().logErrorNotKeyword(token);
    this->setNotOk();
    return rq::Keyword::ERROR;
  }
  if (rq::getIsInternal(keyword)) {
    this->getContext().logErrorInternalUseOnlyKeyword(token, keyword);
    this->setNotOk();
    return rq::Keyword::ERROR;
  }
  return keyword;
}

rq::Expression &RequiteParser::parseEnclosedBracketExpression() {
  const rq::Token &left_token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  rq::TokenRanger keyword_ranger = this->getRanger();
  rq::Expression &expression = this->getContext().acquireExpression();
  const rq::Keyword keyword = this->parseKeyword();
  expression.setKeyword(keyword);
  expression.setSource(left_token);
  if (!expression.getHasStatementBranches()) {
    std::ignore = this->parseExpressionBranches(
        expression, rq::TokenKind::RIGHT_BRACKET_GROUPING);
    return expression;
  }
  rq::TreeFactory factory;
  factory.startTree(expression);
  while (true) {
    const rq::Token &next_token = this->getRanger().getToken();
    if (next_token.getKind() == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
      this->getRanger().incrementToken(1);
      factory.finishExpression(next_token);
      return expression;
    } else if (next_token.getKind() == rq::TokenKind::TRAILER_SEPARATOR) {
      this->parseTrailer(expression, keyword_ranger);
      const rq::Token &last_token = this->getRanger().getToken();
      factory.finishExpression(last_token);
      this->getRanger().incrementToken(1);
      return expression;
    }
    rq::Expression &branch = this->parseExpression();
    factory.appendBranch(branch);
    const rq::Token &after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getRanger().incrementToken(1);
      branch.setIsHeader();
    } else if (after_token.getKind() == rq::TokenKind::SEMICOLON_SEPARATOR) {
      this->getRanger().incrementToken(1);
    } else {
      branch.setIsChainLink();
    }
  }
  RQ_UNREACHABLE();
}

rq::Expression &RequiteParser::parseEnclosedDoubleBracketExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &brace = this->getContext().acquireExpression();
  brace.setKeyword(rq::Keyword::INITIALIZER_LIST);
  brace.setSource(first_token);
  this->getRanger().incrementToken(1);
  std::ignore = this->parseExpressionBranches(
      brace, rq::TokenKind::RIGHT_DOUBLE_BRACKET_GROUPING);
  return brace;
}

rq::Expression &RequiteParser::parseEnclosedBraceExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &brace = this->getContext().acquireExpression();
  brace.setKeyword(rq::Keyword::TUPLE);
  brace.setSource(first_token);
  this->getRanger().incrementToken(1);
  const bool parameter_mark_found =
      this->parseExpressionBranches(brace, rq::TokenKind::RIGHT_BRACE_GROUPING);
  if (parameter_mark_found) {
    brace.changeKeyword(rq::Keyword::LAYOUT_TYPE);
  }
  return brace;
}

void RequiteParser::parseTrailer(rq::Expression &expression,
                                 rq::TokenRanger &keyword_ranger) {
  const rq::Token &first_token = this->getRanger().getToken();
  RQ_ASSERT(first_token.getKind() == rq::TokenKind::TRAILER_SEPARATOR,
            "first token not trailer separator");
  this->getRanger().incrementToken(1);
  unsigned bracket_depth = 1;
  while (!this->getRanger().getIsDone()) {
    const rq::Token &trailer_token = this->getRanger().getToken();
    if (trailer_token.getKind() == rq::TokenKind::LEFT_BRACKET_GROUPING) {
      bracket_depth++;
    } else if (trailer_token.getKind() ==
               rq::TokenKind::RIGHT_BRACKET_GROUPING) {
      bracket_depth--;
      if (bracket_depth == 0) {
        return;
      }
    }
    const rq::Token &front_token = keyword_ranger.getToken();
    if (trailer_token.getSourceText() != front_token.getSourceText()) {
      this->getContext().logErrorTrailerTokenMismatch(trailer_token,
                                                      front_token, expression);
      this->setNotOk();
    }
    this->getRanger().incrementToken(1);
    keyword_ranger.incrementToken(1);
  }
}

rq::Expression &RequiteParser::parseAttribute() {
  const rq::Token &sigil_token = this->getRanger().getToken();
  RQ_ASSERT(sigil_token.getIsSigil(), "not sigil");
  this->getRanger().incrementToken(1);
  const rq::Token &keyword_token = this->getRanger().getToken();
  rq::Keyword keyword = this->parseKeyword();
  rq::Expression &attribute = this->getContext().acquireExpression();
  attribute.setSource(sigil_token, keyword_token);
  attribute.setKeyword(keyword);
  const rq::Token &after_token = this->getRanger().getToken();
  if (after_token.getKind() == rq::TokenKind::DOUBLE_COLON_OPERATOR) {
    this->getRanger().incrementToken(1);
    rq::Expression &branch = this->parseExpression();
    attribute.setBranch(branch);
    attribute.extendSourceOver(branch);
  }
  return attribute;
}

rq::Expression &RequiteParser::parseEnclosedParenthesisExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &parenthesis = this->getContext().acquireExpression();
  parenthesis.setKeyword(rq::Keyword::UNSITUATED_PARENTHESIS_GROUP);
  parenthesis.setSource(first_token);
  this->getRanger().incrementToken(1);
  const bool has_parameter_marks = this->parseExpressionBranches(
      parenthesis, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
  if (has_parameter_marks) {
    parenthesis.changeKeyword(rq::Keyword::SIGNATURE_TYPE);
    rq::Expression &return_type = this->parseExpression();
    if (parenthesis.getHasBranch()) {
      return_type.setNext(parenthesis.replaceBranch(return_type));
    } else {
      parenthesis.setBranch(return_type);
    }
  } else if (!parenthesis.getHasBranch()) {
    rq::Expression &inference = this->getContext().acquireExpression();
    inference.setKeyword(rq::Keyword::INFERENCE);
    inference.setSource(parenthesis);
    inference.setIsInserted();
    parenthesis.setBranch(inference);
  }
  return parenthesis;
}

rq::Expression &RequiteParser::parseLiteralOrMark(rq::Keyword keyword) {
  const rq::Token &token = this->getRanger().getToken();
  RQ_ASSERT(token.getIsLiteral() || token.getCanBeMark(),
            "token is not literal or mark");
  rq::Expression &identifier = this->getContext().acquireExpression();
  identifier.setKeyword(keyword);
  identifier.setSource(token);
  this->getRanger().incrementToken(1);
  return identifier;
}

rq::Expression &RequiteParser::parseInterpolatedString() {
  const rq::Token &left_token = this->getRanger().getToken();
  rq::Expression *first_ptr = nullptr;
  rq::Expression *previous_ptr = nullptr;
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::LEFT_INTERPOLATION_LITERAL: {
      RQ_ASSERT(first_ptr == nullptr && previous_ptr == nullptr,
                "left interpolated string literal must be first");
      this->getRanger().incrementToken(1);
      if (token.getSourceTextLength() == 0) {
        continue;
      }
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::LEFT_INTERPOLATION_LITERAL);
      string.setSource(token);
      first_ptr = &string;
      previous_ptr = &string;
      continue;
    }
    case rq::TokenKind::MIDDLE_INTERPOLATION_LITERAL: {
      this->getRanger().incrementToken(1);
      if (token.getSourceTextLength() == 0) {
        continue;
      }
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::MIDDLE_INTERPOLATION_LITERAL);
      string.setSource(token);
      if (previous_ptr != nullptr) {
        rq::dereferencePtr(previous_ptr).setNext(string);
      }
      if (first_ptr == nullptr) {
        first_ptr = &string;
      }
      previous_ptr = &string;
      continue;
    }
    case rq::TokenKind::RIGHT_INTERPOLATION_LITERAL: {
      this->getRanger().incrementToken(1);
      if (token.getSourceTextLength() == 0) {
        continue;
      }
      rq::Expression &string = this->getContext().acquireExpression();
      string.setKeyword(rq::Keyword::RIGHT_INTERPOLATION_LITERAL);
      string.setSource(token);
      if (previous_ptr != nullptr) {
        rq::dereferencePtr(previous_ptr).setNext(string);
      }
      if (first_ptr == nullptr) {
        first_ptr = &string;
      }
      previous_ptr = &string;
      rq::Expression &tuple = this->getContext().acquireExpression();
      tuple.setKeyword(rq::Keyword::TUPLE);
      tuple.setSource(left_token, token);
      tuple.setBranch(first_ptr);
      return tuple;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
      rq::Expression &interpolation = this->parseEnclosedBraceExpression();
      rq::dereferencePtr(previous_ptr).setNext(interpolation);
      if (first_ptr == nullptr) {
        first_ptr = &interpolation;
      }
      previous_ptr = &interpolation;
      continue;
    }
    default:
      break;
    }
  }
  this->getContext().logErrorUnterminatedInterpolatedString(left_token);
  this->setNotOk();
  rq::Expression &error = this->getContext().acquireExpression();
  error.setKeyword(rq::Keyword::ERROR);
  return error;
}

} // namespace rq
