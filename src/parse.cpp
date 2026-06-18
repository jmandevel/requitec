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

void TreeFactory::startTree(rq::Expression &top) {
  RQ_ASSERT(!top.getHasNext(), "top must not have next");
  [[unlikely]] if (top.getHasBranch()) {
    rq::Expression &branch = top.getBranch();
    RQ_ASSERT(!branch.getHasNext(), "branch must not have next");
    this->setLast(branch);
  }
  this->setExpression(top);
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

void PrecedenceFactory::parseAscribe(llvm::StringRef initial_source,
                                     rq::Keyword keyword) {
  if (this->getHasExpression()) {
    rq::Expression &old_expression = this->getExpression();
    if (old_expression.getKeyword() != keyword) {
      rq::Expression &new_expression = this->getContext().acquireExpression();
      new_expression.setKeyword(keyword);
      new_expression.setSource(old_expression);
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
    expression.setSource(initial_source);
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
  rq::ParseBranchesResult result = this->parseBranches(rq::TokenKind::NONE);
  return result.getFirstBranchPtr();
}

// EXPRESSION ATTRIBUTES
rq::Expression &RequiteParser::parsePrecedence11() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  while (!this->getRanger().getIsDone()) {
    rq::Expression &expression = this->parsePrecedence10();
    if (this->getRanger().getIsDone()) {
      precedence_factory.appendBranch(expression);
      return precedence_factory.getOuter();
    }
    const rq::Token after_token = this->getRanger().getToken();
    if (after_token.getIsInferenceTerminator() ||
        expression.getCanBeChainLink()) {
      precedence_factory.appendBranch(expression);
      return precedence_factory.getOuter();
    }
    rq::Expression &instantiation = this->getContext().acquireExpression();
    instantiation.setSource(expression);
    instantiation.setKeyword(rq::Keyword::INSTANTIATE_EXPRESSION_ATTRIBUTE);
    instantiation.setBranch(expression);
    if (after_token.getKind() == rq::TokenKind::DOUBLE_COLON_OPERATOR) {
      this->getRanger().incrementToken(1);
      rq::Expression &value = this->parseNonascribableExpression();
      expression.setNext(value);
      instantiation.extendSourceOver(value);
    }
    precedence_factory.parseAscribe(instantiation.getSourceText(),
                                    rq::Keyword::UNSITUATED_ASCRIBE_EXPRESSION);
    precedence_factory.appendBranch(instantiation);
  }
  return precedence_factory.getOuter();
}

// ASSIGNMENTS
rq::Expression &RequiteParser::parsePrecedence10() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence9());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token,
                                     rq::Keyword::UNSITUATED_EQUAL_OPERATOR);
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

// BINDINGS
rq::Expression &RequiteParser::parsePrecedence9() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence8());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::COLON_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BINDING);
      precedence_factory.setRecent(this->parsePrecedence8());
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
rq::Expression &RequiteParser::parsePrecedence8() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence7());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOT_PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_ADD,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_SUBTRACT,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_MULTIPLY,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_DIVIDE,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_STEP_MODULUS,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_LESS,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_GREATER,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseSequenceBranch(
          token, rq::Keyword::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL,
          this->parsePrecedence7());
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
rq::Expression &RequiteParser::parsePrecedence7() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence6());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOWN_ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LOGICAL_AND);
      precedence_factory.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::UP_ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LOGICAL_OR);
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

// NARY COMPARISON
rq::Expression &RequiteParser::parsePrecedence6() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence5());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::GREATER_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      const rq::Token next_token = this->getRanger().getToken(1);
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
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::GREATER_EQUAL);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::LESS_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      const rq::Token next_token = this->getRanger().getToken(1);
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
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::LESS_EQUAL);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::EQUAL);
      precedence_factory.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::NOT_EQUAL);
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

// NARY MULTIPLICATIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence5() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence4());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::MULTIPLY);
      precedence_factory.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::DIVIDE);
      precedence_factory.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::MODULUS);
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

// NARY ADDITIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence4() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence3());
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::ADD);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::SUBTRACT);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::APPEND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::APPEND);
      precedence_factory.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::CONCATENATE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::CONCATENATE);
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

// NARY AND BINARY BITWISE
rq::Expression &RequiteParser::parsePrecedence3() {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  precedence_factory.setRecent(this->parsePrecedence2(false));
  while (!this->getRanger().getIsDone()) {
    if (precedence_factory.getRecent().getCanBeChainLink()) {
      break;
    }
    const rq::Token token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BITWISE_SHIFT_LEFT);
      precedence_factory.setRecent(this->parsePrecedence2(false));
      continue;
    case rq::TokenKind::DOUBLE_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseBinary(token, rq::Keyword::BITWISE_SHIFT_RIGHT);
      precedence_factory.setRecent(this->parsePrecedence2(false));
      continue;
    case rq::TokenKind::PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_OR);
      precedence_factory.setRecent(this->parsePrecedence2(false));
      continue;
    case rq::TokenKind::AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_AND);
      precedence_factory.setRecent(this->parsePrecedence2(false));
      continue;
    case rq::TokenKind::CAROT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_factory.parseNary(token, rq::Keyword::BITWISE_XOR);
      precedence_factory.setRecent(this->parsePrecedence2(false));
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
rq::Expression &RequiteParser::parsePrecedence2(bool is_type_ascribed) {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  while (!this->getRanger().getIsDone()) {
    const rq::Token token = this->getRanger().getToken();
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
    default:
      precedence_factory.appendBranch(this->parsePrecedence1(is_type_ascribed));
      break;
    }
    break;
  }
  return precedence_factory.getOuter();
}

// LATE UNARY OPERATORS (things get wierd here)
rq::Expression &RequiteParser::parsePrecedence1(bool is_type_ascribed) {
  rq::PrecedenceFactory precedence_factory(this->getContext());
  bool previous_horned = false;
  while (!this->getRanger().getIsDone()) {
    if (!previous_horned) {
      const rq::Token token = this->getRanger().getToken();
      const rq::TokenKind kind = token.getKind();
      switch (kind) {
      case rq::TokenKind::BACKSLASH_OPERATOR: {
        this->getRanger().incrementToken(1);
        rq::Expression &attribute = this->parseTypeAscribedExpression();
        precedence_factory.parseAscribe(token.getSourceText(),
                                        rq::Keyword::UNSITUATED_ASCRIBE_TYPE);
        rq::Expression &instantiation = this->getContext().acquireExpression();
        instantiation.setSource(token, attribute);
        instantiation.setKeyword(rq::Keyword::INSTANTIATE_TYPE_ATTRIBUTE);
        instantiation.setBranch(attribute);
        const rq::Token after_token = this->getRanger().getToken();
        if (after_token.getKind() == rq::TokenKind::DOUBLE_COLON_OPERATOR) {
          this->getRanger().incrementToken(1);
          rq::Expression &value = this->parseNonascribableExpression();
          attribute.setNext(value);
          instantiation.extendSourceOver(value);
        }
        precedence_factory.appendBranch(instantiation);
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
        precedence_factory.parseNary(token, rq::Keyword::INSTANTIATE_EXTENSION);
        continue;
      }
      case rq::TokenKind::HASH_OPERATOR: {
        rq::Expression &inference = this->getContext().acquireExpression();
        inference.setKeyword(rq::Keyword::INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_factory.setRecent(inference);
        this->getRanger().incrementToken(1);
        precedence_factory.parseNary(token, rq::Keyword::INSTANTIATE_ARRAY);
        continue;
      }
      case rq::TokenKind::AT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token,
                                      rq::Keyword::INSTANTIATE_SLICE);
        continue;
      case rq::TokenKind::DOLLAR_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token,
                                      rq::Keyword::INSTANTIATE_REFERENCE);
        continue;
      case rq::TokenKind::WHAT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_factory.parseUnary(token, rq::Keyword::INSTANTIATE_POINTER);
        continue;
      default:
        break;
      }
      if (previous_horned) {
        precedence_factory.appendRecent();
        return precedence_factory.getOuter();
      }
      if (rq::getIsInferenceTerminator(kind)) {
        if (is_type_ascribed) {
          break;
        }
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
    const rq::Token post_token = this->getRanger().getToken();
    switch (post_token.getKind()) {
    case rq::TokenKind::HASH_OPERATOR:
      if (is_type_ascribed) {
        precedence_factory.appendRecent();
        break;
      }
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token,
                                          rq::Keyword::INSTANTIATE_ARRAY);
      continue;
    case rq::TokenKind::ARROW_OPERATOR:
      if (is_type_ascribed) {
        precedence_factory.appendRecent();
        break;
      }
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token, rq::Keyword::EXTEND);
      continue;
    case rq::TokenKind::THICK_ARROW_OPERATOR:
      if (is_type_ascribed) {
        precedence_factory.appendRecent();
        break;
      }
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      precedence_factory.parseOuterBinary(post_token,
                                          rq::Keyword::INSTANTIATE_EXTENSION);
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
      rq::ParseBranchesResult result =
          this->parseBranches(rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
      callee.setNext(result.getFirstBranchPtr());
      call.setSource(callee, result.getLastToken());
      precedence_factory.setOnlyRecent(call);
      previous_horned = true;
      continue;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_factory.appendRecent();
      rq::Expression &target = precedence_factory.getOuter();
      rq::Expression &instantiation = this->getContext().acquireExpression();
      instantiation.setKeyword(rq::Keyword::INSTANTIATE_TEMPLATE);
      instantiation.setBranch(target);
      rq::ParseBranchesResult result =
          this->parseBranches(rq::TokenKind::RIGHT_BRACE_GROUPING);
      target.setNext(result.getFirstBranchPtr());
      instantiation.setSource(target, result.getLastToken());
      precedence_factory.setOnlyRecent(instantiation);
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
  const rq::Token token = this->getRanger().getToken();
  switch (token.getKind()) {
  case rq::TokenKind::LEFT_BRACKET_GROUPING:
    return this->parseEnclosedBracketExpression();
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

[[nodiscard]] rq::ParseBranchesResult
RequiteParser::parseBranches(rq::TokenKind end) {
  rq::ForestFactory factory;
  const rq::Token first_token = this->getRanger().getToken();
  if (first_token.getKind() == end) {
    this->getRanger().incrementToken(1);
    return rq::ParseBranchesResult(nullptr, first_token, false);
  }
  bool parameter_mark_found = false;
  while (true) {
    const rq::Token next_token = this->getRanger().getToken();
    RQ_ASSERT(end == rq::TokenKind::NONE ||
                  next_token.getKind() != rq::TokenKind::NONE,
              "unreachable bracket error");
    if (next_token.getKind() == rq::TokenKind::GREATER_OPERATOR) {
      this->getRanger().incrementToken(1);
      parameter_mark_found = true;
      rq::Expression &mark = this->getContext().acquireExpression();
      mark.setSource(next_token);
      mark.setKeyword(rq::Keyword::NONPOSITIONAL_PARAMETERS_BEGIN);
      factory.appendTree(mark);
    } else if (next_token.getKind() == rq::TokenKind::PIPE_OPERATOR) {
      this->getRanger().incrementToken(1);
      parameter_mark_found = true;
      rq::Expression &mark = this->getContext().acquireExpression();
      mark.setSource(next_token);
      mark.setKeyword(rq::Keyword::LOCKED_PARAMETERS_BEGIN);
      factory.appendTree(mark);
    } else if (next_token.getKind() == end) {
      this->getRanger().incrementToken(1);
      return rq::ParseBranchesResult(factory.getExpressionPtr(), next_token,
                                     parameter_mark_found);
    }
    rq::Expression &branch = this->parseAscribableExpression();
    factory.appendTree(branch);
    const rq::Token after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::LESS_OPERATOR) {
      this->getRanger().incrementToken(1);
      parameter_mark_found = true;
      rq::Expression &mark = this->getContext().acquireExpression();
      mark.setSource(next_token);
      mark.setKeyword(rq::Keyword::POSITIONAL_PARAMETERS_END);
      factory.appendTree(mark);
    }
    const rq::Token next_after = this->getRanger().getToken();
    if (next_after.getIsSeparator()) {
      this->getRanger().incrementToken(1);
    }
    if (next_after.getKind() == end ||
        next_after.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      continue;
    } else if (next_after.getKind() == rq::TokenKind::SEMICOLON_SEPARATOR) {
      branch.setIsStatement();
      continue;
    }
    branch.setIsChainLink();
    continue;
  }
  RQ_UNREACHABLE();
}

rq::Keyword RequiteParser::parseKeyword() {
  const rq::Token token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  rq::Keyword keyword;
  if (token.getKind() == rq::TokenKind::IDENTIFIER_LITERAL) {
    keyword = this->getContext().getKeyword(token.getSourceText());
  } else {
    this->getContext().logErrorExpectedIdentifierLiteral(token);
    this->setNotOk();
    return rq::Keyword::ERROR;
  }
  if (keyword == rq::Keyword::NONE) {
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
  const rq::Token left_token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  rq::Expression &expression = this->getContext().acquireExpression();
  const rq::Keyword keyword = this->parseKeyword();
  expression.setKeyword(keyword);
  rq::ParseBranchesResult result =
      this->parseBranches(rq::TokenKind::RIGHT_BRACKET_GROUPING);
  expression.setSource(left_token, result.getLastToken());
  expression.setBranch(result.getFirstBranchPtr());
  return expression;
}

rq::Expression &RequiteParser::parseEnclosedBraceExpression() {
  const rq::Token first_token = this->getRanger().getToken();
  rq::Expression &brace = this->getContext().acquireExpression();
  brace.setKeyword(rq::Keyword::INSTANTIATE_TUPLE);
  this->getRanger().incrementToken(1);
  rq::ParseBranchesResult result =
      this->parseBranches(rq::TokenKind::RIGHT_BRACE_GROUPING);
  brace.setSource(first_token, result.getLastToken());
  brace.setBranch(result.getFirstBranchPtr());
  if (result.getFoundParameterMark()) {
    brace.changeKeyword(rq::Keyword::INSTANTIATE_LAYOUT);
  }
  return brace;
}

rq::Expression &RequiteParser::parseEnclosedParenthesisExpression() {
  const rq::Token first_token = this->getRanger().getToken();
  rq::Expression &parenthesis = this->getContext().acquireExpression();
  parenthesis.setKeyword(rq::Keyword::UNSITUATED_PARENTHESIS_GROUP);
  this->getRanger().incrementToken(1);
  rq::ParseBranchesResult result =
      this->parseBranches(rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
  parenthesis.setSource(first_token, result.getLastToken());
  parenthesis.setBranch(result.getFirstBranchPtr());
  if (result.getFoundParameterMark() || !parenthesis.getHasBranch()) {
    parenthesis.changeKeyword(rq::Keyword::INSTANTIATE_SIGNATURE);
    rq::Expression &return_type = this->parseAscribableExpression();
    if (parenthesis.getHasBranch()) {
      return_type.setNext(parenthesis.replaceBranch(return_type));
    } else {
      parenthesis.setBranch(return_type);
    }
  }
  return parenthesis;
}

rq::Expression &RequiteParser::parseLiteralOrMark(rq::Keyword keyword) {
  const rq::Token token = this->getRanger().getToken();
  RQ_ASSERT(token.getIsLiteral() || token.getCanBeMark(),
            "token is not literal or mark");
  rq::Expression &identifier = this->getContext().acquireExpression();
  identifier.setKeyword(keyword);
  identifier.setSource(token);
  this->getRanger().incrementToken(1);
  return identifier;
}

} // namespace rq
