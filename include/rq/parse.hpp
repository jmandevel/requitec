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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Token &getToken() const {
    RQ_ASSERT(this->_it < this->_end, "parser is done");
    return *this->_it;
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

enum class Keyword : std::uint32_t;
struct Context;
struct Module;
struct Node;
struct Token;

struct ForestFactory final {
  using Self = rq::ForestFactory;

  rq::Node *_expression_ptr = nullptr;
  rq::Node *_last_ptr = nullptr;

  ForestFactory() = default;
  ForestFactory(const Self &) = default;
  ForestFactory(Self &&) = default;
  ~ForestFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Node &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node *getExpressionPtr() {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node *
  getExpressionPtr() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLast() const {
    return this->_last_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setLast(rq::Node &last) {
    this->_last_ptr = &last;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getLast() {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getLast() const {
    return rq::dereferencePtr(this->_last_ptr);
  }
  void appendTree(rq::Node &tree);
};

struct TreeFactory final {
  using Self = rq::TreeFactory;

  rq::Node *_expression_ptr = nullptr;
  rq::Node *_last_ptr = nullptr;

  TreeFactory() = default;
  TreeFactory(const Self &) = default;
  TreeFactory(Self &&) = default;
  ~TreeFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Node &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLast() const {
    return this->_last_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setLast(rq::Node &last) {
    this->_last_ptr = &last;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getLast() {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getLast() const {
    return rq::dereferencePtr(this->_last_ptr);
  }
  void startTree(rq::Node &trunk);
  void appendBranch(rq::Node &branch);
  void finishExpression(const rq::Token &last_token);
};

struct PrecedenceFactory final {
  using Self = rq::PrecedenceFactory;

  // the context is used only for acquiring new expressions
  std::reference_wrapper<rq::Context> _context_ref;
  // the outermost expression that is returned at the end of the precedence
  rq::Node *_outer_ptr = nullptr;
  // the current expression that is being filled with branches
  rq::Node *_expression_ptr = nullptr;
  // the most recent parsed branch, which is not yet appended because it might
  // need to nest inside the next expression
  rq::Node *_recent_ptr = nullptr;
  // the last branch that was appended to the expression
  rq::Node *_last_ptr = nullptr;

  PrecedenceFactory(rq::Context &context)
      : _context_ref(context) {}
  PrecedenceFactory(const Self &) = delete;
  PrecedenceFactory(Self &&) = delete;
  ~PrecedenceFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  void parseUnary(const rq::Token &token, rq::Keyword keyword);
  void parseAscribe(const rq::Token &token, rq::Keyword keyword);
  void parseBinary(const rq::Token &token, rq::Keyword keyword);
  void parseOuterBinary(const rq::Token &token, rq::Keyword keyword);
  void parseNary(const rq::Token &token, rq::Keyword keyword);
  void parseSequenceBranch(const rq::Token &token, rq::Keyword keyword,
                           rq::Node &rvalue);
  void appendBranch(rq::Node &branch);
  void appendNullaryAttribute(const rq::Token &token, rq::Keyword keyword);
  void setRecent(rq::Node &branch);
  void setOnlyRecent(rq::Node &branch);
  void appendRecent();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &popRecent() {
    rq::Node &recent = rq::dereferencePtr(this->_recent_ptr);
    this->_recent_ptr = nullptr;
    return recent;
  }
  RQ_ALWAYS_INLINE void setOuterExpression(rq::Node &expression) {
    this->_outer_ptr = &expression;
    this->_expression_ptr = &expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOuter() const {
    return this->_outer_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getOuter() {
    return rq::dereferencePtr(this->_outer_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRecent() const {
    return this->_recent_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getRecent() const {
    return rq::dereferencePtr(this->_recent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getRecent() {
    return rq::dereferencePtr(this->_recent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLast() const {
    return this->_last_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getLast() const {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getLast() {
    return rq::dereferencePtr(this->_last_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnary() const {
    return this->getHasOuter() && !this->getHasLast();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node &getUnary() const {
    RQ_ASSERT(this->getHasUnary(),
              "previous operator in precedence was not unary");
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &getUnary() {
    RQ_ASSERT(this->getHasUnary(),
              "previous operator in precedence was not unary");
    return rq::dereferencePtr(this->_expression_ptr);
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
  [[nodiscard]] rq::Node *parseExpressions();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node &parseExpression() {
    return this->parsePrecedence12();
  }
  [[nodiscard]] rq::Node &parsePrecedence12();
  [[nodiscard]] rq::Node &parsePrecedence11();
  [[nodiscard]] rq::Node &parsePrecedence10();
  [[nodiscard]] rq::Node &parsePrecedence9();
  [[nodiscard]] rq::Node &parsePrecedence8();
  [[nodiscard]] rq::Node &parsePrecedence7();
  [[nodiscard]] rq::Node &parsePrecedence6();
  [[nodiscard]] rq::Node &parsePrecedence5();
  [[nodiscard]] rq::Node &parsePrecedence4();
  [[nodiscard]] rq::Node &parsePrecedence3();
  [[nodiscard]] rq::Node &parsePrecedence2();
  [[nodiscard]] rq::Node &parsePrecedence1();
  [[nodiscard]] rq::Node &parsePrecedence0();
  [[nodiscard]] bool parseExpressionBranches(rq::Node &expression,
                                               rq::TokenKind end);
  [[nodiscard]] rq::Keyword parseKeyword();
  [[nodiscard]] rq::Node &parseEnclosedBracketExpression();
  [[nodiscard]] rq::Node &parseEnclosedDoubleBracketExpression();
  [[nodiscard]] rq::Node &parseEnclosedParenthesisExpression();
  [[nodiscard]] rq::Node &parseEnclosedBraceExpression();
  void parseTrailer(rq::Node &expression,
                    rq::TokenRanger &keyword_ranger);
  [[nodiscard]] rq::Node &parseAttribute();
  [[nodiscard]] rq::Node &parseLiteralOrMark(rq::Keyword keyword);
  [[nodiscard]] rq::Node &parseInterpolatedString();
};

} // namespace rq
