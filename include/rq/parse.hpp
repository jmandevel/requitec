#pragma once

#include <rq/tokens.hpp>

#include <cstdint>
#include <functional>
#include <llvm/ADT/StringRef.h>

namespace rq {

struct TokenRanger final {
  using Self = rq::TokenRanger;

  const rq::Token *_it;
  const rq::Token *_end;

  TokenRanger(llvm::ArrayRef<rq::Token> tokens)
      : _it(tokens.begin()), _end(tokens.end()) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_it >= this->_end;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone(unsigned offset) const {
    return this->_it + offset >= this->_end;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Token getToken() const {
    if (this->_it < this->_end) {
      return *this->_it;
    }
    return rq::Token();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Token &
  getToken(unsigned offset) const {
    RQ_ASSERT(this->_it + offset < this->_end, "offset is past token range");
    return *(this->_it + offset);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Token &getPreviousToken() const {
    RQ_ASSERT(this->_it <= this->_end, "parser is done");
    return *(this->_it - 1);
  }
  RQ_ALWAYS_INLINE void incrementToken(std::size_t offset) {
    this->_it += offset;
  }
  [[nodiscard]] bool getIsToken(rq::TokenKind kind) const;
};

struct Context;
struct Module;
struct Expression;
struct Expression;
struct Token;

struct ForestFactory final {
  using Self = rq::ForestFactory;

  rq::Expression *_expression_ptr = nullptr;
  rq::Expression *_last_ptr = nullptr;

  ForestFactory() = default;
  ForestFactory(const Self &) = default;
  ForestFactory(Self &&) = default;
  ~ForestFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr() {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getExpressionPtr() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLast() const {
    return this->_last_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setLast(rq::Expression &last) {
    this->_last_ptr = &last;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLast() {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLast() const {
    return rq::dereferencePtr(this->_last_ptr);
  }
  void appendTree(rq::Expression &tree);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression* build() {
    rq::Expression *expression_ptr = this->getExpressionPtr();
    this->_expression_ptr = nullptr;
    this->_last_ptr = nullptr;
    return expression_ptr;
  }
};

struct PrecedenceFactory final {
  using Self = rq::PrecedenceFactory;

  // the context is used only for acquiring new expressions
  std::reference_wrapper<rq::Context> _context_ref;
  // the outermost expression that is returned at the end of the precedence
  rq::Expression *_outer_ptr = nullptr;
  // the current expression that is being filled with branches
  rq::Expression *_expression_ptr = nullptr;
  // the most recent parsed branch, which is not yet appended because it might
  // need to nest inside the next expression
  rq::Expression *_recent_ptr = nullptr;
  // the last branch that was appended to the expression
  rq::Expression *_last_ptr = nullptr;

  PrecedenceFactory(rq::Context &context) : _context_ref(context) {}
  PrecedenceFactory(const Self &) = delete;
  PrecedenceFactory(Self &&) = delete;
  ~PrecedenceFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] rq::Context &getContext() { return this->_context_ref.get(); }
  [[nodiscard]] const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  void parseUnary(const rq::Token &token, rq::Keyword keyword);
  void parseAscribe(llvm::StringRef initial_source, rq::Keyword keyword);
  void parseBinary(const rq::Token &token, rq::Keyword keyword);
  void parseOuterBinary(const rq::Token &token, rq::Keyword keyword);
  void parseNary(const rq::Token &token, rq::Keyword keyword);
  void parseSequenceBranch(const rq::Token &token, rq::Keyword keyword,
                           rq::Expression &rvalue);
  void appendBranch(rq::Expression &branch);
  void appendNullaryAttribute(const rq::Token &token, rq::Keyword keyword);
  void setRecent(rq::Expression &branch);
  void setOnlyRecent(rq::Expression &branch);
  void appendRecent();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &popRecent() {
    rq::Expression &recent = rq::dereferencePtr(this->_recent_ptr);
    this->_recent_ptr = nullptr;
    return recent;
  }
  RQ_ALWAYS_INLINE void setOuterExpression(rq::Expression &expression) {
    this->_outer_ptr = &expression;
    this->_expression_ptr = &expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOuter() const {
    return this->_outer_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getOuter() {
    return rq::dereferencePtr(this->_outer_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRecent() const {
    return this->_recent_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getRecent() const {
    return rq::dereferencePtr(this->_recent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getRecent() {
    return rq::dereferencePtr(this->_recent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLast() const {
    return this->_last_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLast() const {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLast() {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnary() const {
    return this->getHasOuter() && !this->getHasLast();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getUnary() const {
    RQ_ASSERT(this->getHasUnary(),
              "previous operator in precedence was not unary");
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getUnary() {
    RQ_ASSERT(this->getHasUnary(),
              "previous operator in precedence was not unary");
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

struct ParseBranchesResult final {
  using Self = rq::ParseBranchesResult;

  const rq::Token _last_token{};
  rq::Expression *_first_ptr{nullptr};
  bool _found_parameter_mark{false};

  explicit RQ_ALWAYS_INLINE ParseBranchesResult() = default;
  explicit RQ_ALWAYS_INLINE ParseBranchesResult(rq::Expression *first_ptr,
                                                const rq::Token & last_token,
                                                bool found_parameter_mark)
      : _last_token(last_token), _first_ptr(first_ptr),
        _found_parameter_mark(found_parameter_mark) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Token &getLastToken() const {
    return this->_last_token;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getFirstBranchPtr() const {
    return this->_first_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getFirstBranchPtr() {
    return this->_first_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getFoundParameterMark() const {
    return this->_found_parameter_mark;
  }
};

struct RequiteParser final {
  using Self = rq::RequiteParser;

  std::reference_wrapper<rq::Context> _context_ref;
  rq::TokenRanger _token_ranger;
  bool _is_ok = true;

  RequiteParser(rq::Context &context, llvm::ArrayRef<rq::Token> tokens)
      : _context_ref(context), _token_ranger(tokens) {}
  RequiteParser(const Self &) = delete;
  RequiteParser(Self &&) = delete;
  ~RequiteParser() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() { return this->_is_ok; }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TokenRanger &getRanger() const {
    return this->_token_ranger;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TokenRanger &getRanger() {
    return this->_token_ranger;
  }
  [[nodiscard]] rq::Expression *parseExpressions();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &parseAscribableExpression() {
    return this->parsePrecedence11();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  parseNonascribableExpression() {
    return this->parsePrecedence10();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &parseTypeAscribedExpression() {
    return this->parsePrecedence2(true);
  }
  [[nodiscard]] rq::Expression &parsePrecedence11();
  [[nodiscard]] rq::Expression &parsePrecedence10();
  [[nodiscard]] rq::Expression &parsePrecedence9();
  [[nodiscard]] rq::Expression &parsePrecedence8();
  [[nodiscard]] rq::Expression &parsePrecedence7();
  [[nodiscard]] rq::Expression &parsePrecedence6();
  [[nodiscard]] rq::Expression &parsePrecedence5();
  [[nodiscard]] rq::Expression &parsePrecedence4();
  [[nodiscard]] rq::Expression &parsePrecedence3();
  [[nodiscard]] rq::Expression &parsePrecedence2(bool is_type_ascribed);
  [[nodiscard]] rq::Expression &parsePrecedence1(bool is_type_ascribed);
  [[nodiscard]] rq::Expression &parsePrecedence0();
  [[nodiscard]] rq::ParseBranchesResult parseBranches(rq::TokenKind end);
  [[nodiscard]] rq::Keyword parseKeyword();
  [[nodiscard]] rq::Expression &parseEnclosedBracketExpression();
  [[nodiscard]] rq::Expression &parseEnclosedParenthesisExpression();
  [[nodiscard]] rq::Expression &parseEnclosedBraceExpression();
  [[nodiscard]] rq::Expression &parseLiteralOrMark(rq::Keyword keyword);
};

} // namespace rq
