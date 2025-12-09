#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/parse.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

namespace rq {

void ForestParser::appendTree(rq::Expression &branch) {
  [[unlikely]] if (!this->getHasOperation()) {
    this->setOperation(branch);
    this->setLast(branch);
    return;
  }
  rq::Expression &last = this->getLast();
  last.setNext(branch);
  this->setLast(branch);
}

void TreeParser::startTree(rq::Expression &trunk) {
  RQ_ASSERT(!trunk.getHasNext(), "trunk must not have next");
  [[unlikely]] if (trunk.getHasBranch()) {
    rq::Expression &branch = trunk.getBranch();
    RQ_ASSERT(!branch.getHasNext(), "branch must not have next");
    this->setLast(branch);
  }
  this->setOperation(trunk);
}

void TreeParser::appendBranch(rq::Expression &branch) {
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

void TreeParser::finishOperation(const rq::Token &last_token) {
  rq::Expression &operation = this->getOperation();
  operation.extendSourceOver(last_token);
}

void PrecedenceParser::parseDoubleUnary(const rq::Token &token,
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

void PrecedenceParser::parseUnary(const rq::Token &token, rq::Keyword keyword) {
  rq::Expression &operation = this->getStaticFrame().acquireExpression();
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

void PrecedenceParser::parseBinary(const rq::Token &token,
                                   rq::Keyword keyword) {
  rq::Expression &new_operation = this->getStaticFrame().acquireExpression();
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
  rq::Expression &new_operation = this->getStaticFrame().acquireExpression();
  new_operation.setKeyword(keyword);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceParser::parseNestingNary(const rq::Token &token,
                                        rq::Keyword keyword) {
  rq::Expression &operation = this->getStaticFrame().acquireExpression();
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
  rq::Expression &step = this->getStaticFrame().acquireExpression();
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
  rq::Expression &expression = this->getStaticFrame().acquireExpression();
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

bool TokenRanger::getIsToken(rq::TokenKind kind) const {
  if (this->getIsDone()) {
    return false;
  }
  const bool is_token = this->getToken().getKind() == kind;
  return is_token;
}

// NOTE:
//  This is (mostly) a recursive descent parser.

rq::Expression *NormativeParser::parseExpressions() {
  rq::ForestParser parser;
  bool in_if_chunk = false;
  while (!this->getRanger().getIsDone()) {
    rq::Expression &next = this->parseExpression();
    parser.appendTree(next);
    if (in_if_chunk) {
      in_if_chunk = false;
      if (!next.getIsIfChunkNotStart()) {
        this->getContext().logErrorNotSecondOrSubsequentIfChunkExpression(next);
        this->setNotOk();
      }
    }
    if (this->getRanger().getIsDone()) {
      this->getContext().logErrorExpectedSemicolonSeparator(next);
      this->setNotOk();
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    if (next.getCanBeArm()) {
      continue;
    }
    if (token.getKind() == rq::TokenKind::SEMICOLON_SEPARATOR) {
      this->getRanger().incrementToken(1);
      continue;
    }
    if (next.getIsIfChunkNotEnd()) {
      in_if_chunk = true;
      continue;
    }
    this->getContext().logErrorExpectedSemicolonSeparator(next);
    this->setNotOk();
  }
  return parser.getOperationPtr();
}

// STATEMENT ATTRIBUTES
rq::Expression &NormativeParser::parsePrecedence11() {
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    const rq::TokenKind kind = token.getKind();
    switch (kind) {
    case rq::TokenKind::AT_SIGIL: {
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence9());
  while (!this->getRanger().getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_EQUAL_OPERATOR);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::PLUS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_ADD);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::DASH_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_SUBTRACT);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::STAR_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::SLASH_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_ASSIGN_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence9());
      continue;
    case rq::TokenKind::PERCENT_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
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

// COLONS
rq::Expression &NormativeParser::parsePrecedence9() {
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence8());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::COLON_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_COLON_OPERATOR);
      precedence_parser.setRecent(this->parsePrecedence8());
      continue;
    case rq::TokenKind::DOUBLE_COLON_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence7());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOT_PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_ADD, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_SUBTRACT,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_MULTIPLY,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_DIVIDE, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_STEP_MODULUS, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_LESS, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_GREATER,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_LESS_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_GREATER_EQUAL,
          this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_EQUAL, this->parsePrecedence7());
      continue;
    case rq::TokenKind::DOT_BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseSequenceBranch(
          token, rq::Keyword::_SEQUENCE_WHILE_NOT_EQUAL,
          this->parsePrecedence7());
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence6());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LOGICAL_AND);
      precedence_parser.setRecent(this->parsePrecedence6());
      continue;
    case rq::TokenKind::DOUBLE_PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence5());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::GREATER_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_GREATER);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::GREATER_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_GREATER_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::LESS_OPERATOR: {
      if (this->getRanger().getIsDone(1)) {
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      }
      const rq::Token &next_token = this->getRanger().getToken(1);
      switch (const rq::TokenKind next_kind = next_token.getKind()) {
      case rq::TokenKind::GREATER_OPERATOR:
        [[fallthrough]];
      case rq::TokenKind::LESS_OPERATOR:
        precedence_parser.appendRecent();
        return precedence_parser.getOuter();
      default:
        if (rq::getIsExpressionEnd(next_kind)) {
          precedence_parser.appendRecent();
          return precedence_parser.getOuter();
        }
      }
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LESS);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    }
    case rq::TokenKind::LESS_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_LESS_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::DOUBLE_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_EQUAL);
      precedence_parser.setRecent(this->parsePrecedence5());
      continue;
    case rq::TokenKind::BANG_EQUAL_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence4());
  while (!this->getRanger().getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::STAR_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_MULTIPLY);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::SLASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_DIVIDE);
      precedence_parser.setRecent(this->parsePrecedence4());
      continue;
    case rq::TokenKind::PERCENT_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence3());
  while (!this->getRanger().getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::PLUS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_ADD);
      precedence_parser.setRecent(this->parsePrecedence3());
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_SUBTRACT);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  precedence_parser.setRecent(this->parsePrecedence2());
  while (!this->getRanger().getIsDone()) {
    if (precedence_parser.getRecent().getHasSemicolonSeparatedBranches()) {
      break;
    }
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::DOUBLE_GREATER_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_BITWISE_SHIFT_LEFT);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::DOUBLE_LESS_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseBinary(token, rq::Keyword::_BITWISE_SHIFT_RIGHT);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::PIPE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_BITWISE_OR);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::AMPERSAND_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(token, rq::Keyword::_BITWISE_AND);
      precedence_parser.setRecent(this->parsePrecedence2());
      continue;
    case rq::TokenKind::CAROT_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    switch (token.getKind()) {
    case rq::TokenKind::BANG_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_LOGICAL_COMPLEMENT);
      continue;
    case rq::TokenKind::DASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_NEGATE);
      continue;
    case rq::TokenKind::TILDE_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseUnary(token, rq::Keyword::_BITWISE_COMPLEMENT);
      continue;
    case rq::TokenKind::BACKSLASH_OPERATOR:
      this->getRanger().incrementToken(1);
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
  rq::PrecedenceParser precedence_parser(
      this->getContext().getTopStaticFrame());
  bool previous_call = false;
  while (!this->getRanger().getIsDone()) {
    if (!previous_call) {
      const rq::Token &token = this->getRanger().getToken();
      const rq::TokenKind kind = token.getKind();
      switch (kind) {
      case rq::TokenKind::DOLLAR_SIGIL: {
        rq::Expression &attribute = this->parseTypeAttribute();
        precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_TYPE);
        precedence_parser.appendBranch(attribute);
        continue;
      }
      case rq::TokenKind::ARROW_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_EXTEND);
        continue;
      }
      case rq::TokenKind::HASH_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_ARRAY);
        continue;
      }
      case rq::TokenKind::DOT_OPERATOR: {
        rq::Expression &operation =
            this->getContext().getTopStaticFrame().acquireExpression();
        operation.setKeyword(rq::Keyword::_INFERENCE);
        operation.setSourceInsertedBefore(token);
        precedence_parser.setRecent(operation);
        this->getRanger().incrementToken(1);
        precedence_parser.parseNary(token, rq::Keyword::_REFLECT);
        continue;
      }
      case rq::TokenKind::CAROT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_FAT_POINTER);
        continue;
      case rq::TokenKind::PERCENT_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_IDENTIFY);
        continue;
      case rq::TokenKind::AMPERSAND_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_REFERENCE);
        continue;
      case rq::TokenKind::DOUBLE_AMPERSAND_OPERATOR:
        precedence_parser.parseDoubleUnary(token, rq::Keyword::_REFERENCE);
        continue;
      case rq::TokenKind::STAR_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_parser.parseUnary(token, rq::Keyword::_POINTER);
        continue;
      case rq::TokenKind::GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
        precedence_parser.parseAscribe(token, rq::Keyword::_ASCRIBE_TYPE);
        precedence_parser.appendUnaryAttribute(token, rq::Keyword::MUTABLE);
        continue;
      case rq::TokenKind::DOUBLE_GRAVE_OPERATOR:
        this->getRanger().incrementToken(1);
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
      if (rq::getIsInferenceTerminator(kind)) {
        rq::Expression &inference =
            this->getContext().getTopStaticFrame().acquireExpression();
        inference.setKeyword(rq::Keyword::_INFERENCE);
        inference.setSourceInsertedBefore(token);
        precedence_parser.appendBranch(inference);
        break;
      }
      rq::Expression &expression = this->parsePrecedence0();
      precedence_parser.setRecent(expression);
    }
    previous_call = false;
    if (this->getRanger().getIsDone()) {
      precedence_parser.appendRecent();
      break;
    }
    const rq::Token &post_token = this->getRanger().getToken();
    switch (post_token.getKind()) {
    case rq::TokenKind::HASH_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_ARRAY);
      continue;
    case rq::TokenKind::ARROW_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_EXTEND);
      continue;
    case rq::TokenKind::DOT_OPERATOR:
      this->getRanger().incrementToken(1);
      precedence_parser.parseNary(post_token, rq::Keyword::_REFLECT);
      continue;
    case rq::TokenKind::LEFT_PARENTHESIS_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_parser.appendRecent();
      rq::Expression &callee = precedence_parser.getOuter();
      rq::Expression &call =
          this->getContext().getTopStaticFrame().acquireExpression();
      call.setKeyword(rq::Keyword::_CALL);
      call.setBranch(callee);
      call.setSource(callee, post_token);
      std::ignore = this->parseCommaSeparatedBranches(
          call, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, true);
      precedence_parser.setOnlyRecent(call);
      previous_call = true;
      continue;
    }
    case rq::TokenKind::LEFT_BRACE_GROUPING: {
      this->getRanger().incrementToken(1);
      precedence_parser.appendRecent();
      rq::Expression &target = precedence_parser.getOuter();
      rq::Expression &specialization =
          this->getContext().getTopStaticFrame().acquireExpression();
      specialization.setKeyword(rq::Keyword::_SPECIALIZATION);
      specialization.setBranch(target);
      specialization.setSource(target, post_token);
      std::ignore = this->parseCommaSeparatedBranches(
          specialization, rq::TokenKind::RIGHT_BRACE_GROUPING, true);
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
  const rq::Token &token = this->getRanger().getToken();
  switch (token.getKind()) {
  case rq::TokenKind::LEFT_BRACKET_GROUPING:
    return this->parseEnclosedBracketExpression();
  case rq::TokenKind::LEFT_PARENTHESIS_GROUPING:
    return this->parseEnclosedParenthesisExpression();
  case rq::TokenKind::LEFT_BRACE_GROUPING:
    return this->parseEnclosedBraceExpression();
  case rq::TokenKind::IDENTIFIER_LITERAL:
    return this->parseLiteral(rq::Keyword::__IDENTIFIER_LITERAL);
  case rq::TokenKind::CODEUNIT_LITERAL:
    return this->parseLiteral(rq::Keyword::__CODEUNIT_LITERAL);
  case rq::TokenKind::STRING_LITERAL:
    return this->parseLiteral(rq::Keyword::__STRING_LITERAL);
  case rq::TokenKind::INTEGER_LITERAL:
    return this->parseLiteral(rq::Keyword::__INTEGER_LITERAL);
  case rq::TokenKind::FLOAT_LITERAL:
    return this->parseLiteral(rq::Keyword::__FLOAT_LITERAL);
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
  error.setKeyword(rq::Keyword::__ERROR);
  error.setSource(token);
  return error;
}

bool NormativeParser::parseCommaSeparatedBranches(
    rq::Expression &operation, rq::TokenKind end,
    bool must_not_have_parameter_marks) {
  RQ_ASSERT(!operation.getHasSemicolonSeparatedBranches(),
            "operation has semicolon separated branches");
  if (this->getRanger().getIsDone()) {
    RQ_UNREACHABLE();
  }
  bool found_invalid_parameter_mark = false;
  rq::TreeParser grouping_parser;
  grouping_parser.startTree(operation);
  bool has_parameter_marks = false;
  while (!this->getRanger().getIsDone()) {
    const rq::Token &first_token = this->getRanger().getToken();
    if (first_token.getKind() == end) {
      this->getRanger().incrementToken(1);
      grouping_parser.finishOperation(first_token);
      return has_parameter_marks;
    }
    while (!this->getRanger().getIsDone()) {
      while (!this->getRanger().getIsDone()) {
        const rq::Token &before_token = this->getRanger().getToken();
        const rq::TokenKind previous_kind = before_token.getKind();
        if (previous_kind == end) {
          this->getRanger().incrementToken(1);
          grouping_parser.finishOperation(before_token);
          return has_parameter_marks;
        } else if (previous_kind == rq::TokenKind::GREATER_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(before_token);
            this->setNotOk();
          }
          rq::Expression &mark =
              this->getContext().getTopStaticFrame().acquireExpression();
          mark.setKeyword(rq::Keyword::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(before_token);
          grouping_parser.appendBranch(mark);
          this->getRanger().incrementToken(1);
          continue;
        } else if (previous_kind == rq::TokenKind::LESS_OPERATOR) {
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(before_token);
            this->setNotOk();
          }
          rq::Expression &mark =
              this->getContext().getTopStaticFrame().acquireExpression();
          mark.setKeyword(rq::Keyword::_POSITIONAL_PARAMETERS_END);
          mark.setSource(before_token);
          grouping_parser.appendBranch(mark);
          this->getRanger().incrementToken(1);
          continue;
        }
        break;
      }
      if (this->getRanger().getIsDone()) {
        break;
      }
      rq::Expression &branch = this->parseExpression();
      grouping_parser.appendBranch(branch);
      while (!this->getRanger().getIsDone()) {
        const rq::Token &after_token = this->getRanger().getToken();
        const rq::TokenKind next_kind = after_token.getKind();
        if (next_kind == end) {
          this->getRanger().incrementToken(1);
          grouping_parser.finishOperation(after_token);
          return has_parameter_marks;
        } else if (next_kind == rq::TokenKind::GREATER_OPERATOR) {
          this->getRanger().incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(after_token);
            this->setNotOk();
          }
          rq::Expression &mark =
              this->getContext().getTopStaticFrame().acquireExpression();
          mark.setKeyword(rq::Keyword::_NAMED_PARAMETERS_BEGIN);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (next_kind == rq::TokenKind::LESS_OPERATOR) {
          this->getRanger().incrementToken(1);
          has_parameter_marks = true;
          if (must_not_have_parameter_marks) {
            if (!found_invalid_parameter_mark) {
              found_invalid_parameter_mark = true;
              this->getContext().logErrorMustNotHaveParameterMarks(operation);
            }
            this->getContext().logErrorUnexpectedParameterMark(after_token);
            this->setNotOk();
          }
          rq::Expression &mark =
              this->getContext().getTopStaticFrame().acquireExpression();
          mark.setKeyword(rq::Keyword::_POSITIONAL_PARAMETERS_END);
          mark.setSource(after_token);
          grouping_parser.appendBranch(mark);
          continue;
        } else if (next_kind != rq::TokenKind::COMMA_SEPARATOR) {
          this->getContext().logErrorExpectedCommaSeparator(branch);
          this->setNotOk();
          break;
        }
        this->getRanger().incrementToken(1);
        break;
      }
    }
  }
  RQ_UNREACHABLE();
}

rq::Keyword NormativeParser::parseKeyword() {
  const rq::Token &token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  const rq::TokenKind kind = token.getKind();
  rq::Keyword keyword;
  if (kind == rq::TokenKind::IDENTIFIER_LITERAL) {
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
        llvm::Twine(rq::getDescription(kind)) + " does not represent a keyword",
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
  return keyword;
}

rq::Expression &NormativeParser::parseEnclosedBracketExpression() {
  const rq::Token &left_token = this->getRanger().getToken();
  this->getRanger().incrementToken(1);
  const rq::Token &keyword_token = this->getRanger().getToken();
  rq::TokenRanger keyword_ranger = this->getRanger();
  rq::Expression &operation =
      this->getContext().getTopStaticFrame().acquireExpression();
  if (keyword_token.getKind() ==
      rq::TokenKind::LEFT_PARENTHESIS_GROUPING) { // its an
                                                  // _anonymous_function
                                                  // expression
    operation.setKeyword(rq::Keyword::_ANONYMOUS_FUNCTION);
    operation.setSource(left_token);
    rq::TreeParser parser;
    parser.startTree(operation);
    rq::Expression &capture =
        this->getContext().getTopStaticFrame().acquireExpression();
    capture.setKeyword(rq::Keyword::_DYNAMIC_CAPTURE);
    capture.setSource(keyword_token);
    this->getRanger().incrementToken(1);
    std::ignore = this->parseCommaSeparatedBranches(
        capture, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, true);
    parser.appendBranch(capture);
  } else {
    const rq::Keyword keyword = this->parseKeyword();
    operation.setKeyword(keyword);
    operation.setSource(left_token);
  }
  if (operation.getHasSemicolonSeparatedBranches()) {
    rq::TreeParser parser;
    parser.startTree(operation);
    const unsigned comma_count = operation.getCommaBranchCount();
    if (comma_count != 0) {
      unsigned commas_left = comma_count;
      while (commas_left > 0) {
        const rq::Token &before_token = this->getRanger().getToken();
        const rq::TokenKind previous_kind = before_token.getKind();
        if (previous_kind == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
          if ((commas_left != comma_count && commas_left != 0) ||
              !operation.getIsNullaryWhenNoBranches()) {
            this->parseTacitCommas(
                commas_left, before_token.getAfterSourceTextPtr(), parser);
          }
          this->getRanger().incrementToken(1);
          parser.finishOperation(before_token);
          return operation;
        } else if (previous_kind == rq::TokenKind::TRAILER_SEPARATOR) {
          if (commas_left != comma_count && commas_left != 0) {
            this->parseTacitCommas(
                commas_left, before_token.getAfterSourceTextPtr(), parser);
          }
          this->parseTrailer(operation, keyword_ranger);
          const rq::Token &last_token = this->getRanger().getToken();
          this->getRanger().incrementToken(1);
          parser.finishOperation(last_token);
          return operation;
        }
        rq::Expression &next = this->parseExpression();
        const rq::Token &after_token = this->getRanger().getToken();
        const rq::TokenKind next_kind = after_token.getKind();
        if (next_kind == rq::TokenKind::SEMICOLON_SEPARATOR ||
            next_kind == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
          if (commas_left != comma_count && commas_left != 0) {
            this->parseTacitCommas(
                commas_left, after_token.getBeforeSourceTextPtr(), parser);
          }
          parser.appendBranch(next);
          if (next_kind == rq::TokenKind::SEMICOLON_SEPARATOR) {
            this->getRanger().incrementToken(1);
            const rq::Token &next_after = this->getRanger().getToken();
            const rq::TokenKind next_next_kind = next_after.getKind();
            if (next_next_kind == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
              this->getRanger().incrementToken(1);
              parser.finishOperation(next_after);
              return operation;
            }
          }
          break;
        }
        commas_left--;
        parser.appendBranch(next);
        switch (next_kind) {
        case rq::TokenKind::COMMA_SEPARATOR:
          this->getRanger().incrementToken(1);
          break;
        case rq::TokenKind::RIGHT_BRACKET_GROUPING:
          break;
        default:
          this->getContext().logErrorExpectedCommaSeparator(next);
          this->setNotOk();
          break;
        }
      }
    }
    bool in_if_chunk = false;
    while (!this->getRanger().getIsDone()) { // semicolons
      const rq::Token &before_token = this->getRanger().getToken();
      const rq::TokenKind previous_kind = before_token.getKind();
      if (previous_kind == rq::TokenKind::RIGHT_BRACKET_GROUPING) {
        this->getRanger().incrementToken(1);
        parser.finishOperation(before_token);
        return operation;
      } else if (previous_kind == rq::TokenKind::TRAILER_SEPARATOR) {
        this->parseTrailer(operation, keyword_ranger);
        const rq::Token &last_token = this->getRanger().getToken();
        this->getRanger().incrementToken(1);
        parser.finishOperation(last_token);
        return operation;
      }
      rq::Expression &next = this->parseExpression();
      parser.appendBranch(next);
      if (in_if_chunk) {
        in_if_chunk = false;
        if (!next.getIsIfChunkNotStart()) {
          this->getContext().logErrorNotSecondOrSubsequentIfChunkExpression(
              next);
        }
      }
      if (next.getCanBeArm()) {
        continue;
      }
      const rq::Token &after_token = this->getRanger().getToken();
      const rq::TokenKind next_kind = after_token.getKind();
      switch (next_kind) {
      case rq::TokenKind::SEMICOLON_SEPARATOR:
        this->getRanger().incrementToken(1);
        break;
      case rq::TokenKind::COMMA_SEPARATOR:
        this->getRanger().incrementToken(1);
        [[fallthrough]];
      case rq::TokenKind::RIGHT_BRACKET_GROUPING:
        [[fallthrough]];
      default:
        if (next.getIsIfChunkNotEnd()) {
          in_if_chunk = true;
          break;
        }
        this->getContext().logErrorExpectedSemicolonSeparator(next);
        this->setNotOk();
        break;
      }
    }
    RQ_UNREACHABLE();
  }
  std::ignore = this->parseCommaSeparatedBranches(
      operation, rq::TokenKind::RIGHT_BRACKET_GROUPING, true);
  return operation;
}

rq::Expression &NormativeParser::parseEnclosedBraceExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &brace =
      this->getContext().getTopStaticFrame().acquireExpression();
  brace.setKeyword(rq::Keyword::_TUPLE);
  brace.setSource(first_token);
  this->getRanger().incrementToken(1);
  const rq::Token &second_token = this->getRanger().getToken();
  switch (second_token.getKind()) {
  case rq::TokenKind::GREATER_OPERATOR: {
    const rq::Token &third_token = this->getRanger().getToken(1);
    switch (third_token.getKind()) {
    case rq::TokenKind::LESS_OPERATOR: {
      const rq::Token &fourth_token = this->getRanger().getToken(2);
      switch (fourth_token.getKind()) {
      case rq::TokenKind::RIGHT_BRACE_GROUPING: {
        brace.changeKeyword(rq::Keyword::_NULL_TYPE);
        brace.extendSourceOver(fourth_token);
        this->getRanger().incrementToken(3);
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
  case rq::TokenKind::LESS_OPERATOR: {
    const rq::Token &third_token = this->getRanger().getToken(1);
    switch (third_token.getKind()) {
    case rq::TokenKind::GREATER_OPERATOR: {
      const rq::Token &fourth_token = this->getRanger().getToken(2);
      switch (fourth_token.getKind()) {
      case rq::TokenKind::RIGHT_BRACE_GROUPING: {
        brace.changeKeyword(rq::Keyword::_NULL_TYPE);
        brace.extendSourceOver(fourth_token);
        this->getRanger().incrementToken(3);
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
  bool has_parameter_marks = this->parseCommaSeparatedBranches(
      brace, rq::TokenKind::RIGHT_BRACE_GROUPING, false);
  if (has_parameter_marks) {
    brace.changeKeyword(rq::Keyword::_LAYOUT_TYPE);
  } else if (!brace.getHasBranch()) {
    brace.changeKeyword(rq::Keyword::_NULL);
  }
  return brace;
}

void NormativeParser::parseTrailer(rq::Expression &operation,
                                   rq::TokenRanger &keyword_ranger) {
  const rq::Token &first_token = this->getRanger().getToken();
  RQ_ASSERT(first_token.getKind() == rq::TokenKind::TRAILER_SEPARATOR,
            "first token not trailer separator");
  this->getRanger().incrementToken(1);
  if (operation.getKeyword() == rq::Keyword::_ANONYMOUS_FUNCTION) {
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

void NormativeParser::parseTacitCommas(unsigned count,
                                       const char *source_text_ptr,
                                       rq::TreeParser &parser) {
  for (unsigned comma_i = 0; comma_i < count; comma_i++) {
    rq::Expression &inference =
        this->getContext().getTopStaticFrame().acquireExpression();
    inference.setKeyword(rq::Keyword::_TACIT_COMMA_EXPRESSION);
    inference.setSourceInsertedAt(source_text_ptr);
    parser.appendBranch(inference);
  }
}

rq::Expression &NormativeParser::parseStatementAttribute() {
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
    std::ignore = this->parseCommaSeparatedBranches(
        attribute, rq::TokenKind::RIGHT_BRACKET_GROUPING, true);
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_BRACE_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::TEMPLATE);
    attribute.setSource(at_token);
    this->getRanger().incrementToken(1);
    bool has_parameter_marks = this->parseCommaSeparatedBranches(
        attribute, rq::TokenKind::RIGHT_BRACE_GROUPING, false);
    if (!has_parameter_marks) {
      this->getContext().logErrorMustHaveParameterMarks(attribute);
      this->setNotOk();
      return attribute;
    }
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_PARENTHESIS_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::STATIC_CAPTURE);
    attribute.setSource(at_token);
    this->getRanger().incrementToken(1);
    std::ignore = this->parseCommaSeparatedBranches(
        attribute, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, true);
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

rq::Expression &NormativeParser::parseTypeAttribute() {
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
    std::ignore = this->parseCommaSeparatedBranches(
        attribute, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, true);
    return attribute;
  } else if (next_token.getKind() == rq::TokenKind::LEFT_PARENTHESIS_GROUPING) {
    rq::Expression &attribute =
        this->getContext().getTopStaticFrame().acquireExpression();
    attribute.setKeyword(rq::Keyword::DYNAMIC_CAPTURE_LAYOUT);
    attribute.setSource(dollar_token);
    this->getRanger().incrementToken(1);
    bool has_parameter_marks = this->parseCommaSeparatedBranches(
        attribute, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, false);
    if (!has_parameter_marks) {
      this->getContext().logErrorMustHaveParameterMarks(attribute);
      this->setNotOk();
      return attribute;
    }
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

rq::Expression &NormativeParser::parseEnclosedParenthesisExpression() {
  const rq::Token &first_token = this->getRanger().getToken();
  rq::Expression &parenthesis =
      this->getContext().getTopStaticFrame().acquireExpression();
  parenthesis.setKeyword(rq::Keyword::_PARENTHESIS_GROUP);
  parenthesis.setSource(first_token);
  this->getRanger().incrementToken(1);
  bool has_parameter_marks = this->parseCommaSeparatedBranches(
      parenthesis, rq::TokenKind::RIGHT_PARENTHESIS_GROUPING, false);
  if (has_parameter_marks) {
    parenthesis.changeKeyword(rq::Keyword::_SIGNATURE_TYPE);
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
  const rq::Token &token = this->getRanger().getToken();
  RQ_ASSERT(token.getIsLiteral(), "token is not literal");
  rq::Expression &identifier =
      this->getContext().getTopStaticFrame().acquireExpression();
  identifier.setKeyword(keyword);
  identifier.setSource(token);
  this->getRanger().incrementToken(1);
  return identifier;
}

rq::Expression &NormativeParser::parseNullaryOperator(rq::Keyword keyword) {
  const rq::Token &token = this->getRanger().getToken();
  rq::Expression &expression =
      this->getContext().getTopStaticFrame().acquireExpression();
  expression.setKeyword(keyword);
  expression.setSource(token);
  this->getRanger().incrementToken(1);
  return expression;
}

rq::Expression &NormativeParser::parseInterpolatedString() {
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
      string.setKeyword(rq::Keyword::__LEFT_INTERPOLATION_LITERAL);
      string.setSource(token);
      first_ptr = &string;
      previous_ptr = &string;
      this->getRanger().incrementToken(1);
      continue;
    }
    case rq::TokenKind::MIDDLE_INTERPOLATION_LITERAL: {
      rq::Expression &string =
          this->getContext().getTopStaticFrame().acquireExpression();
      string.setKeyword(rq::Keyword::__MIDDLE_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      this->getRanger().incrementToken(1);
      continue;
    }
    case rq::TokenKind::RIGHT_INTERPOLATION_LITERAL: {
      rq::Expression &string =
          this->getContext().getTopStaticFrame().acquireExpression();
      string.setKeyword(rq::Keyword::__RIGHT_INTERPOLATION_LITERAL);
      string.setSource(token);
      rq::dereferencePtr(previous_ptr).setNext(string);
      previous_ptr = &string;
      rq::Expression &tuple =
          this->getContext().getTopStaticFrame().acquireExpression();
      tuple.setKeyword(rq::Keyword::_TUPLE);
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
  error.setKeyword(rq::Keyword::__ERROR);
  return error;
}

void SymbolicParser::parseTrailingCommma(rq::Expression &expression) {
  if (!this->getRanger().getIsDone()) {
    rq::Token &after_token = this->getRanger().getToken();
    if (after_token.getKind() == rq::TokenKind::COMMA_SEPARATOR) {
      this->getRanger().incrementToken(1);
      expression.setIsCommaTerminated();
    }
  }
}

rq::Expression &SymbolicParser::parseLiteral(rq::Keyword keyword) {
  const rq::Token &token = this->getRanger().getToken();
  RQ_ASSERT(token.getIsLiteral(), "token is not literal");
  this->getRanger().incrementToken(1);
  rq::Expression &expression =
      this->getContext().getTopStaticFrame().acquireExpression();
  expression.setSource(token);
  expression.setKeyword(keyword);
  this->parseTrailingCommma(expression);
  return expression;
}

rq::Expression *SymbolicParser::parseExpressions() {
  llvm::SmallVector<rq::ForestParser, 16> forest_stack;
  forest_stack.emplace_back();
  while (!this->getRanger().getIsDone()) {
    const rq::Token &token = this->getRanger().getToken();
    using namespace rq;
    using T = TokenKind;
    switch (const rq::TokenKind kind = token.getKind()) {
    case T::LEFT_BRACKET_GROUPING: {
      this->getRanger().incrementToken(1);
      const rq::Token &keyword_token = this->getRanger().getToken();
      [[unlikely]] if (keyword_token.getKind() !=
                       rq::TokenKind::IDENTIFIER_LITERAL) {
        this->getContext().logMessage(
            token.getLlvmSourceStart(), rq::LogType::ERROR,
            llvm::Twine(rq::getDescription(token.getKind())) +
                " not valid token for keyword",
            {token.getLlvmSourceRange()}, {});
        this->setNotOk();
        break;
      }
      rq::Keyword keyword =
          this->getContext().getKeyword(keyword_token.getSourceText());
      [[unlikely]] if (rq::getIsInternal(keyword)) {
        this->getContext().logMessage(
            token.getLlvmSourceStart(), rq::LogType::ERROR,
            llvm::Twine(rq::getName(keyword)) + "is for internal use only",
            {token.getLlvmSourceRange()}, {});
        this->setNotOk();
        break;
      }
      this->getRanger().incrementToken(1);
      rq::Expression &expression =
          this->getContext().getTopStaticFrame().acquireExpression();
      expression.setSource(token);
      expression.setKeyword(keyword);
      this->parseTrailingCommma(expression);
      forest_stack.back().appendTree(expression);
      std::ignore = forest_stack.emplace_back();
      break;
    }
    case T::RIGHT_BRACKET_GROUPING: {
      this->getRanger().incrementToken(1);
      if (!forest_stack.back().getHasOperation()) {
        forest_stack.pop_back();
        RQ_ASSERT(!forest_stack.empty(),
                  "forest stack size can not go down to 0");
        break;
      }
      rq::Expression &finished = forest_stack.back().getOperation();
      this->parseTrailingCommma(finished);
      forest_stack.pop_back();
      RQ_ASSERT(!forest_stack.empty(),
                "forest stack size can not go down to 0");
      rq::Expression &last = forest_stack.back().getLast();
      last.extendSourceOver(token);
      last.setBranch(finished);
      break;
    }
    case T::IDENTIFIER_LITERAL:
      forest_stack.back().appendTree(
          this->parseLiteral(rq::Keyword::__IDENTIFIER_LITERAL));
      break;
    case T::CODEUNIT_LITERAL:
      forest_stack.back().appendTree(
          this->parseLiteral(rq::Keyword::__CODEUNIT_LITERAL));
      break;
    case T::STRING_LITERAL:
      forest_stack.back().appendTree(
          this->parseLiteral(rq::Keyword::__STRING_LITERAL));
      break;
    case T::INTEGER_LITERAL:
      forest_stack.back().appendTree(
          this->parseLiteral(rq::Keyword::__INTEGER_LITERAL));
      break;
    case T::FLOAT_LITERAL:
      forest_stack.back().appendTree(
          this->parseLiteral(rq::Keyword::__FLOAT_LITERAL));
      break;
    default:
      this->getRanger().incrementToken(1);
      this->getContext().logErrorUnexpectedToken(token);
      this->setNotOk();
      break;
    }
  }
  return forest_stack.front().getOperationPtr();
}

} // namespace rq