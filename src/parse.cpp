#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/parse.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

namespace rq {

void ForestBuilder::appendTree(rq::Expression &branch) {
  [[unlikely]] if (!this->getHasOperation()) {
    this->setOperation(branch);
    this->setLast(branch);
    return;
  }
  rq::Expression &last = this->getLast();
  last.setNext(branch);
  this->setLast(branch);
}

void TreeBuilder::startTree(rq::Expression &trunk) {
  RQ_ASSERT(!trunk.getHasNext(), "trunk must not have next");
  [[unlikely]] if (trunk.getHasBranch()) {
    rq::Expression &branch = trunk.getBranch();
    RQ_ASSERT(!branch.getHasNext(), "branch must not have next");
    this->setLast(branch);
  }
  this->setOperation(trunk);
}

void TreeBuilder::appendBranch(rq::Expression &branch) {
  [[unlikely]] if (this->_last_ptr == nullptr) {
    rq::Expression &operation = this->getOperation();
    operation.setBranch(branch);
    this->setLast(branch);
    return;
  }
  rq::Expression &last = rq::dereferencePtr(this->_last_ptr);
  last.setNext(branch);
  this->setLast(branch);
}

void TreeBuilder::finishOperation(const rq::Token &last_token) {
  rq::Expression &operation = this->getOperation();
  operation.extendSourceOver(last_token);
}

void PrecedenceBuilder::parseDoubleUnary(const rq::Token &token,
                                         rq::Keyword keyword) {
  rq::Expression &operation0 = this->getStaticFrame().acquireExpression();
  operation0.setKeyword(keyword);
  operation0.setSource(token);
  this->appendBranch(operation0);
  this->_operation_ptr = &operation0;
  this->_last_ptr = nullptr;
  rq::Expression &operation1 = this->getStaticFrame().acquireExpression();
  operation1.setKeyword(keyword);
  operation1.setSource(token);
  this->appendBranch(operation1);
  this->_operation_ptr = &operation1;
  this->_last_ptr = nullptr;
}

void PrecedenceBuilder::parseUnary(const rq::Token &token,
                                   rq::Keyword keyword) {
  rq::Expression &operation = this->getStaticFrame().acquireExpression();
  operation.setKeyword(keyword);
  operation.setSource(token);
  this->appendBranch(operation);
  this->_operation_ptr = &operation;
  this->_last_ptr = nullptr;
}

void PrecedenceBuilder::parseAscribe(const rq::Token &token,
                                     rq::Keyword keyword) {
  if (this->getHasOperation()) {
    rq::Expression &old_operation = this->getOperation();
    if (old_operation.getKeyword() != keyword) {
      rq::Expression &new_operation =
          this->getStaticFrame().acquireExpression();
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
  rq::Expression &operation = this->getStaticFrame().acquireExpression();
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

void PrecedenceBuilder::parseBinary(const rq::Token &token,
                                    rq::Keyword keyword) {
  rq::Expression &new_operation = this->getStaticFrame().acquireExpression();
  new_operation.setKeyword(keyword);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceBuilder::parseNary(const rq::Token &token, rq::Keyword keyword) {
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
  rq::Expression &new_operation = this->getStaticFrame().acquireExpression();
  new_operation.setKeyword(keyword);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceBuilder::parseNestingNary(const rq::Token &token,
                                         rq::Keyword keyword) {
  rq::Expression &operation = this->getStaticFrame().acquireExpression();
  operation.setKeyword(keyword);
  operation.setSource(this->getOuter(), token);
  operation.setBranch(this->getOuter());
  this->_operation_ptr = &operation;
  this->_last_ptr = this->_outer_ptr;
  this->_outer_ptr = &operation;
}

void PrecedenceBuilder::parseSequenceBranch(const rq::Token &token,
                                            rq::Keyword keyword,
                                            rq::Expression &rvalue) {
  this->parseNary(token, rq::Keyword::S_SEQUENCE);
  rq::Expression &step = this->getStaticFrame().acquireExpression();
  step.setKeyword(keyword);
  step.setSource(token, rvalue);
  step.setBranch(rvalue);
  this->setRecent(step);
}

void PrecedenceBuilder::appendBranch(rq::Expression &branch) {
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

void PrecedenceBuilder::appendUnaryAttribute(const rq::Token &token,
                                             rq::Keyword keyword) {
  rq::Expression &expression = this->getStaticFrame().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->appendBranch(expression);
}

void PrecedenceBuilder::setRecent(rq::Expression &branch) {
  rq::assignSingleValue(this->_recent_ptr, &branch);
}

void PrecedenceBuilder::setOnlyRecent(rq::Expression &branch) {
  this->_outer_ptr = nullptr;
  this->_operation_ptr = nullptr;
  this->_last_ptr = nullptr;
  this->_recent_ptr = &branch;
}

void PrecedenceBuilder::appendRecent() {
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
  rq::ForestBuilder builder;
  while (!this->getRanger().getIsDone()) {
    rq::Expression &next = this->parseExpression();
    builder.appendTree(next);
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
  return builder.getOperationPtr();
}

// STATEMENT ATTRIBUTES
rq::Expression &RequiteParser::parsePrecedence11() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    const rq::TokenKind kind = token.getKind();
    switch (kind) {
    case rq::TokenKind::AT_SIGIL: {
      rq::Expression &attribute = this->parseStatementAttribute();
      precedence_builder.parseAscribe(token, rq::Keyword::S_ASCRIBE_STATEMENT);
      precedence_builder.appendBranch(attribute);
      continue;
    }
    default:
      precedence_builder.appendBranch(this->parsePrecedence10());
      break;
    }
    break;
  }
  return precedence_builder.getOuter();
}

// ASSIGNMENTS
rq::Expression &RequiteParser::parsePrecedence10() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence9());
  while (!this->getRanger().getIsDone()) {
    if (precedence_builder.getRecent().getHasStatementBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_EQUAL_OPERATOR);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::PLUS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_ASSIGN_ADD);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::DASH_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_ASSIGN_SUBTRACT);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::STAR_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_ASSIGN_MULTIPLY);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::SLASH_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_ASSIGN_DIVIDE);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::PERCENT_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_ASSIGN_MODULUS);
      precedence_builder.setRecent(this->parsePrecedence9());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// COLONS
rq::Expression &RequiteParser::parsePrecedence9() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence8());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::COLON_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_COLON_OPERATOR);
      precedence_builder.setRecent(this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOUBLE_COLON_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_BITWISE_CAST);
      precedence_builder.setRecent(this->parsePrecedence8());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// RANGES
rq::Expression &RequiteParser::parsePrecedence8() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence7());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOT_PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_STEP_ADD, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_STEP_SUBTRACT,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_STEP_MULTIPLY,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_STEP_DIVIDE, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_STEP_MODULUS,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_LESS, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_GREATER,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_LESS_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_GREATER_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_EQUAL, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseSequenceBranch(
          token, rq::Keyword::S_SEQUENCE_WHILE_NOT_EQUAL,
          this->parsePrecedence7());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// NARY LOGICAL
rq::Expression &RequiteParser::parsePrecedence7() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence6());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_LOGICAL_AND);
      precedence_builder.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::DOUBLE_PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_LOGICAL_OR);
      precedence_builder.setRecent(this->parsePrecedence6());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// NARY COMPARISON
rq::Expression &RequiteParser::parsePrecedence6() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence5());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::GREATER_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_builder.appendRecent();
        return precedence_builder.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_builder.appendRecent();
        return precedence_builder.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_builder.appendRecent();
          return precedence_builder.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_GREATER);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_GREATER_EQUAL);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::LESS_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_builder.appendRecent();
        return precedence_builder.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_builder.appendRecent();
        return precedence_builder.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_builder.appendRecent();
          return precedence_builder.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_LESS);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_LESS_EQUAL);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_EQUAL);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_NOT_EQUAL);
      precedence_builder.setRecent(this->parsePrecedence5());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// NARY MULTIPLICATIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence5() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence4());
  while (!this->getRanger().getIsDone()) {
    if (precedence_builder.getRecent().getHasStatementBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_MULTIPLY);
      precedence_builder.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_DIVIDE);
      precedence_builder.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_MODULUS);
      precedence_builder.setRecent(this->parsePrecedence4());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// NARY ADDITIVE ARITHMETIC
rq::Expression &RequiteParser::parsePrecedence4() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence3());
  while (!this->getRanger().getIsDone()) {
    if (precedence_builder.getRecent().getHasStatementBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_ADD);
      precedence_builder.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_SUBTRACT);
      precedence_builder.setRecent(this->parsePrecedence3());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// NARY AND BINARY BITWISE
rq::Expression &RequiteParser::parsePrecedence3() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  precedence_builder.setRecent(this->parsePrecedence2());
  while (!this->getRanger().getIsDone()) {
    if (precedence_builder.getRecent().getHasStatementBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_BITWISE_SHIFT_LEFT);
      precedence_builder.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::DOUBLE_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseBinary(token, rq::Keyword::S_BITWISE_SHIFT_RIGHT);
      precedence_builder.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_BITWISE_OR);
      precedence_builder.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_BITWISE_AND);
      precedence_builder.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::CAROT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(token, rq::Keyword::S_BITWISE_XOR);
      precedence_builder.setRecent(this->parsePrecedence2());
      continue;
    default:
      break;
    }
    break;
  }
  precedence_builder.appendRecent();
  return precedence_builder.getOuter();
}

// EARLY UNARY OPERATORS
rq::Expression &RequiteParser::parsePrecedence2() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::BANG_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseUnary(token, rq::Keyword::S_LOGICAL_COMPLEMENT);
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseUnary(token, rq::Keyword::S_NEGATE);
      continue;
    case rq::TokenKind::TILDE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseUnary(token, rq::Keyword::S_BITWISE_COMPLEMENT);
      continue;
    case rq::TokenKind::BACKSLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseUnary(token, rq::Keyword::S_IDENTIFY);
      continue;
    default:
      precedence_builder.appendBranch(this->parsePrecedence1());
      break;
    }
    break;
  }
  return precedence_builder.getOuter();
}

// LATE UNARY OPERATORS (things get wierd here)
rq::Expression &RequiteParser::parsePrecedence1() {
  rq::PrecedenceBuilder precedence_builder(
      this->getContext().getTopStaticFrame());
  bool previous_horned = false;
  while (!this->getRanger().getIsDone()) {
    if (!previous_horned) {
      const rq::Token &token = this->getRanger().getToken();
      const rq::TokenKind kind = token.getKind();
      switch (kind) {
      case rq::TokenKind::DOLLAR_SIGIL: {
        rq::Expression &attribute = this->parseTypeAttribute();
        precedence_builder.parseAscribe(token, rq::Keyword::S_ASCRIBE_TYPE);
        precedence_builder.appendBranch(attribute);
        continue;
      }
      case rq::TokenKind::ARROW_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::S_INFERENCE);
        operation.setIsInserted();
        operation.setSourceBefore(token);
        precedence_builder.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_builder.parseNary(token, rq::Keyword::S_EXTEND);
        continue;
      }
      case rq::TokenKind::HASH_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::S_INFERENCE);
        operation.setIsInserted();
        operation.setSourceBefore(token);
        precedence_builder.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_builder.parseNary(token, rq::Keyword::S_ARRAY);
        continue;
      }
      case rq::TokenKind::DOT_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::S_INFERENCE);
        operation.setIsInserted();
        operation.setSourceBefore(token);
        precedence_builder.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_builder.parseNary(token, rq::Keyword::S_REFLECT);
        continue;
      }
      case rq::TokenKind::CAROT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseUnary(token, rq::Keyword::S_FAT_POINTER);
        continue;
      case rq::TokenKind::PERCENT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseUnary(token, rq::Keyword::S_IDENTIFY);
        continue;
      case rq::TokenKind::AMPERSAND_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseUnary(token, rq::Keyword::S_REFERENCE);
        continue;
      case rq::TokenKind::DOUBLE_AMPERSAND_OPERATOR:
        precedence_builder.parseDoubleUnary(token, rq::Keyword::S_REFERENCE);
        continue;
      case rq::TokenKind::STAR_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseUnary(token, rq::Keyword::S_POINTER);
        continue;
      case rq::TokenKind::GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseAscribe(token, rq::Keyword::S_ASCRIBE_TYPE);
        precedence_builder.appendUnaryAttribute(token, rq::Keyword::MUTABLE);
        continue;
      case rq::TokenKind::DOUBLE_GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_builder.parseAscribe(token, rq::Keyword::S_ASCRIBE_TYPE);
        precedence_builder.appendUnaryAttribute(token, rq::Keyword::CONSTANT);
        continue;
      default:
        break;
      }
      if (previous_horned) {
        precedence_builder.appendRecent();
        return precedence_builder.getOuter();
      }
      if (rq::getIsInferenceTerminator(kind)) {
        rq::Expression &inference =
            this->getContext().getTopStaticFrame().acquireExpression();
        inference.setKeyword(rq::Keyword::S_INFERENCE);
        inference.setIsInserted();
        inference.setSourceBefore(token);
        precedence_builder.appendBranch(inference);
        break;
      }
      rq::Expression &expression = this->parsePrecedence0();
      precedence_builder.setRecent(expression);
    }
    previous_horned = false;
    if (this->getRanger().getIsDone()) {
      precedence_builder.appendRecent();
      break;
    }
    const rq::Token &post_token = this->getRanger().getToken();
    switch (post_token.getKind()) {
    case rq::TokenKind::HASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(post_token, rq::Keyword::S_ARRAY);
      continue;
    case rq::TokenKind::ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(post_token, rq::Keyword::S_EXTEND);
      continue;
    case rq::TokenKind::DOT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_builder.parseNary(post_token, rq::Keyword::S_REFLECT);
      continue;
    case rq::TokenKind::LEFT_PARENTHESIS_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_builder.appendRecent();
      rq::Expression &callee = precedence_builder.getOuter();
      rq::Expression &call =
          this->getContext().getTopStaticFrame().acquireExpression();
      call.setKeyword(rq::Keyword::S_CALL);
      call.setBranch(callee);
      call.setSource(callee, post_token);
      this->parseNonStatementBranches(
          call, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
      precedence_builder.setOnlyRecent(call);
      previous_horned = true;
      continue;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_builder.appendRecent();
      rq::Expression &target = precedence_builder.getOuter();
      rq::Expression &specialization =
          this->getContext().getTopStaticFrame().acquireExpression();
      specialization.setKeyword(rq::Keyword::S_SPECIALIZATION);
      specialization.setBranch(target);
      specialization.setSource(target, post_token);
      this->parseNonStatementBranches(specialization,
                                      rq::TokenKind::RIGHT_BRACE_GROUPING);
      precedence_builder.setOnlyRecent(specialization);
      previous_horned = true;
      continue;
    }
    default:
      precedence_builder.appendRecent();
      break;
    }
    break;
  }
  return precedence_builder.getOuter();
}

// BASE EXPRESSIONS
rq::Expression &RequiteParser::parsePrecedence0() {
  const rq::Token &token = this->getRanger().getToken();
  switch (token.getKind()) {
  case rq::TokenKind::LEFT_BRACKET_GROUPING:
    return this->parseEnclosedBracketExpression();
  case rq::TokenKind::LEFT_PARENTHESIS_GROUPING:
    return this->parseEnclosedParenthesisExpression();
  case rq::TokenKind::LEFT_BRACE_GROUPING:
    return this->parseEnclosedBraceExpression();
  case rq::TokenKind::IDENTIFIER_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::I_IDENTIFIER_LITERAL);
  case rq::TokenKind::CODEUNIT_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::I_CODEUNIT_LITERAL);
  case rq::TokenKind::STRING_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::I_STRING_LITERAL);
  case rq::TokenKind::INTEGER_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::I_INTEGER_LITERAL);
  case rq::TokenKind::FLOAT_LITERAL:
    return this->parseLiteralOrMark(rq::Keyword::I_FLOAT_LITERAL);
  case rq::TokenKind::LEFT_INTERPOLATION_LITERAL:
    return this->parseInterpolatedString();
  default:
    break;
  }
  this->getRanger().incrementToken(1);
  this->getContext().logErrorUnexpectedToken(token);
  this->setNotOk();
  rq::Expression &error =
      this->getContext().getTopStaticFrame().acquireExpression();
  error.setKeyword(rq::Keyword::I_ERROR);
  error.setSource(token);
  return error;
}

void RequiteParser::parseNonStatementBranches(rq::Expression &operation,
                                              rq::TokenKind end) {
  RQ_ASSERT(operation.getHasNonStatementBranches(),
            "operation must have non-statement branches");
  rq::TreeBuilder builder;
  builder.startTree(operation);
  const rq::Token &first_token = this->getRanger().getToken();
  if (first_token.getKind() == end) {
    this->getRanger().incrementToken(1);
    builder.finishOperation(first_token);
    return;
  }
  while (true) {
    const rq::Token &next_token = this->getRanger().getToken();
    if (next_token.getCanBeMark()) {
      while (true) {
        const rq::Token &mark_token = this->getRanger().getToken();
        if (mark_token.getKind() == rq::TokenKind::LESS_OPERATOR) {
          rq::Expression &mark = this->parseLiteralOrMark(
              rq::Keyword::S_POSITIONAL_PARAMETERS_END);
          builder.appendBranch(mark);
          continue;
        } else if (mark_token.getKind() == rq::TokenKind::GREATER_OPERATOR) {
          rq::Expression &mark =
              this->parseLiteralOrMark(rq::Keyword::S_NAMED_PARAMETERS_BEGIN);
          builder.appendBranch(mark);
          continue;
        }
        break;
      }
      continue;
    }
    rq::Expression &branch = this->parseExpression();
    builder.appendBranch(branch);
    const rq::Token &after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getRanger().incrementToken(1);
      continue;
    } else if (after_token.getKind() == end) {
      this->getRanger().incrementToken(1);
      builder.finishOperation(after_token);
      return;
    } else if (after_token.getCanBeMark()) {
      while (true) {
        const rq::Token &mark_token = this->getRanger().getToken();
        if (mark_token.getKind() == rq::TokenKind::LESS_OPERATOR) {
          rq::Expression &mark = this->parseLiteralOrMark(
              rq::Keyword::S_POSITIONAL_PARAMETERS_END);
          builder.appendBranch(mark);
          continue;
        } else if (mark_token.getKind() == rq::TokenKind::GREATER_OPERATOR) {
          rq::Expression &mark =
              this->parseLiteralOrMark(rq::Keyword::S_NAMED_PARAMETERS_BEGIN);
          builder.appendBranch(mark);
          continue;
        } else if (mark_token.getKind() == end) {
          this->getRanger().incrementToken(1);
          builder.finishOperation(mark_token);
          return;
        } else if (mark_token.getKind() != rq::TokenKind::COMMA_SEPARATOR) {
          this->getContext().logErrorExpectedCommaSeparator(branch);
          this->setNotOk();
          break;
        } else {
          this->getRanger().incrementToken(1);
          break;
        }
      }
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
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        "expected identifier literal", {token.getLlvmSourceRange()}, {});
    return rq::Keyword::I_ERROR;
  }
  if (keyword == rq::Keyword::I_NONE) {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getDescription(token.getKind())) +
            " does not represent a keyword",
        {token.getLlvmSourceRange()}, {});
    return rq::Keyword::I_ERROR;
  }
  if (rq::getIsInternal(keyword)) {
    this->setNotOk();
    this->getContext().logMessage(
        token.getLlvmSourceStart(), rq::LogType::ERROR,
        llvm::Twine(rq::getName(keyword)) + " is for internal use only",
        {token.getLlvmSourceRange()}, {});
    return rq::Keyword::I_ERROR;
  }
  return keyword;
}

rq::Expression &RequiteParser::parseEnclosedBracketExpression() {
  const rq::Token &left_token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  const rq::Token &keyword_token = this->getRanger().getToken();
  rq::TokenRanger keyword_ranger = this->getRanger();
  rq::Expression &operation =
      this->getContext().getTopStaticFrame().acquireExpression();
  if (keyword_token.getKind() == rq::TokenKind::LEFT_BRACKET_GROUPING) {
    operation.setKeyword(rq::Keyword::S_ANONYMOUS_FUNCTION);
    operation.setSource(left_token);
    rq::TreeBuilder builder;
    builder.startTree(operation);
    rq::Expression &capture =
        this->getContext().getTopStaticFrame().acquireExpression();
    capture.setKeyword(rq::Keyword::S_DYNAMIC_CAPTURE);
    capture.setSource(keyword_token);
    this->getRanger().incrementToken(1);
    this->parseNonStatementBranches(capture,
                                    rq::TokenKind::RIGHT_BRACKET_GROUPING);
    builder.appendBranch(capture);
  } else {
    const rq::Keyword keyword = this->parseKeyword();
    operation.setKeyword(keyword);
    operation.setSource(left_token);
  }
  if (!operation.getHasStatementBranches()) {
    this->parseNonStatementBranches(operation,
                                    rq::TokenKind::RIGHT_BRACKET_GROUPING);
    return operation;
  }
  rq::TreeBuilder builder;
  builder.startTree(operation);
  while (true) {
    const rq::Token &next_token = this->getRanger().getToken();
    if (next_token.getKind() == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
      this->getRanger().incrementToken(1);
      builder.finishOperation(next_token);
      return operation;
    } else if (next_token.getKind() == rq::TokenKind::TRAILER_SEPARATOR) {
      this->parseTrailer(operation, keyword_ranger);
      const rq::Token &last_token = this->getRanger().getToken();
      builder.finishOperation(last_token);
      this->getRanger().incrementToken(1);
      return operation;
    }
    rq::Expression &branch = this->parseExpression();
    builder.appendBranch(branch);
    const rq::Token &after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getRanger().incrementToken(1);
      branch.setIsBold();
    } else if (after_token.getKind() == rq::TokenKind::SEMICOLON_SEPARATOR) {
      this->getRanger().incrementToken(1);
    } else {
      branch.setIsChainLink();
    }
  }
  RQ_UNREACHABLE();
}

rq::Expression &RequiteParser::parseEnclosedBraceExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &brace =
      this->getContext().getTopStaticFrame().acquireExpression();
  brace.setKeyword(rq::Keyword::S_TUPLE);
  brace.setSource(first_token);
  this->getRanger().incrementToken(1);
  this->parseNonStatementBranches(brace, rq::TokenKind::RIGHT_BRACE_GROUPING);
  return brace;
}

void RequiteParser::parseTrailer(rq::Expression &operation,
                                 rq::TokenRanger &keyword_ranger) {
  const rq::Token &first_token = this->getRanger().getToken();
  RQ_ASSERT(first_token.getKind() == rq::TokenKind::TRAILER_SEPARATOR,
            "first token not trailer separator");
  this->getRanger().incrementToken(1);
  if (operation.getKeyword() == rq::Keyword::S_ANONYMOUS_FUNCTION) {
    this->getContext().logErrorUnexpectedToken(first_token);
    this->setNotOk();
    return;
  }
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
      this->getContext().logMessage(
          trailer_token.getLlvmSourceStart(), rq::LogType::ERROR,
          "trailer token does not match token from start of "
          "operation",
          {trailer_token.getLlvmSourceRange()}, {});
      this->getContext().logMessage(operation.getLlvmSourceStart(),
                                    rq::LogType::NOTE, "for operation",
                                    {operation.getLlvmSourceRange()}, {});
      this->getContext().logMessage(front_token.getLlvmSourceStart(),
                                    rq::LogType::NOTE,
                                    "for token from start of operation",
                                    {front_token.getLlvmSourceRange()}, {});
      this->setNotOk();
    }
    this->getRanger().incrementToken(1);
    keyword_ranger.incrementToken(1);
  }
}

rq::Expression &RequiteParser::parseStatementAttribute() {
  const rq::Token &at_token = this->getRanger().getToken();
  RQ_ASSERT(at_token.getKind() == rq::TokenKind::AT_SIGIL, "not at sigil");
  this->getRanger().incrementToken(1);
  const rq::Token &next_token = this->getRanger().getToken();
  if (next_token.getKind() == rq::TokenKind::LEFT_BRACKET_GROUPING) {
    this->getRanger().incrementToken(1);
    rq::Keyword keyword = this->parseKeyword();
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(keyword);
    attribute.setSource(at_token);
    this->parseNonStatementBranches(attribute,
                                    rq::TokenKind::RIGHT_BRACKET_GROUPING);
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_BRACE_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::TEMPLATE);
    attribute.setSource(at_token);
    this->getRanger().incrementToken(1);
    this->parseNonStatementBranches(attribute,
                                    rq::TokenKind::RIGHT_BRACE_GROUPING);
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_PARENTHESIS_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::STATIC_CAPTURE);
    attribute.setSource(at_token);
    this->getRanger().incrementToken(1);
    this->parseNonStatementBranches(attribute,
                                    rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
    return attribute;
  }
  const rq::Token &keyword_token = this->getRanger().getToken();
  rq::Keyword keyword = this->parseKeyword();
  rq::Expression &attribute =
      this->getContext().getTopStaticFrame().acquireExpression();
  attribute.setKeyword(keyword);
  attribute.setSource(at_token, keyword_token);
  return attribute;
}

rq::Expression &RequiteParser::parseTypeAttribute() {
  const rq::Token &dollar_token = this->getRanger().getToken();
  RQ_ASSERT(dollar_token.getKind() == rq::TokenKind::DOLLAR_SIGIL,
            "not dollar sigil");
  this->getRanger().incrementToken(1);
  const rq::Token &next_token = this->getRanger().getToken();
  if (next_token.getKind() == rq::TokenKind::LEFT_PARENTHESIS_GROUPING) {
    this->getRanger().incrementToken(1);
    rq::Keyword keyword = this->parseKeyword();
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(keyword);
    attribute.setSource(dollar_token);
    this->parseNonStatementBranches(attribute,
                                    rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_PARENTHESIS_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::DYNAMIC_CAPTURE_LAYOUT);
    attribute.setSource(dollar_token);
    this->getRanger().incrementToken(1);
    this->parseNonStatementBranches(attribute,
                                    rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
    return attribute;
  }
  const rq::Token &keyword_token = this->getRanger().getToken();
  rq::Keyword keyword = this->parseKeyword();
  rq::Expression &attribute =
      this->getContext().getTopStaticFrame().acquireExpression();
  attribute.setKeyword(keyword);
  attribute.setSource(dollar_token, keyword_token);
  return attribute;
}

rq::Expression &RequiteParser::parseEnclosedParenthesisExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &parenthesis =
      this->getContext().getTopStaticFrame().acquireExpression();
  parenthesis.setKeyword(rq::Keyword::S_PARENTHESIS_GROUP);
  parenthesis.setSource(first_token);
  this->getRanger().incrementToken(1);
  this->parseNonStatementBranches(parenthesis,
                                  rq::TokenKind::RIGHT_PARENTHESIS_GROUPING);
  return parenthesis;
}

rq::Expression &RequiteParser::parseLiteralOrMark(rq::Keyword keyword) {
  const rq::Token &token = this->getRanger().getToken();
  RQ_ASSERT(token.getIsLiteral() || token.getCanBeMark(),
            "token is not literal or mark");
  rq::Expression &identifier =
      this->getContext().getTopStaticFrame().acquireExpression();
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
      rq::Expression &string =
          this->getContext().getTopStaticFrame().acquireExpression();
      string.setKeyword(rq::Keyword::I_LEFT_INTERPOLATION_LITERAL);
      string.setSource(token);
      first_ptr = &string;
      previous_ptr = &string;
      this->getRanger().incrementToken(1);
      continue;
    }
    case rq::TokenKind::MIDDLE_INTERPOLATION_LITERAL: {
      rq::Expression &string =
          this->getContext().getTopStaticFrame().acquireExpression();
      string.setKeyword(rq::Keyword::I_MIDDLE_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      this->getRanger().incrementToken(1);
      continue;
    }
    case rq::TokenKind::RIGHT_INTERPOLATION_LITERAL: {
      rq::Expression &string =
          this->getContext().getTopStaticFrame().acquireExpression();
      string.setKeyword(rq::Keyword::I_RIGHT_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      rq::Expression &tuple =
          this->getContext().getTopStaticFrame().acquireExpression();
      tuple.setKeyword(rq::Keyword::S_TUPLE);
      tuple.setSource(left_token, token);
      tuple.setBranch(first_ptr);
      this->getRanger().incrementToken(1);
      return tuple;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
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
  rq::Expression &error =
      this->getContext().getTopStaticFrame().acquireExpression();
  error.setKeyword(rq::Keyword::I_ERROR);
  return error;
}

} // namespace rq
