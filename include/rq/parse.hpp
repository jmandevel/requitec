#pragma once

#include <rq/tokens.hpp>

#include <cstdint>
#include <functional>
#include <llvm/ADT/StringRef.h>

namespace rq {

struct TokenRanger final {
  using Self = rq::TokenRanger;

  const rq::Token* _it;
  const rq::Token* _end;

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
struct Expression;
struct Token;

struct ForestBuilder final {
  using Self = rq::ForestBuilder;

  rq::Expression *_operation_ptr = nullptr;
  rq::Expression *_last_ptr = nullptr;

  ForestBuilder() = default;
  ForestBuilder(const Self &) = default;
  ForestBuilder(Self &&) = default;
  ~ForestBuilder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOperation() const {
    return this->_operation_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setOperation(rq::Expression &operation) {
    rq::assignSingleValue(this->_operation_ptr, &operation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getOperation() {
    return rq::dereferencePtr(this->_operation_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getOperation() const {
    return rq::dereferencePtr(this->_operation_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getOperationPtr() {
    return this->_operation_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getOperationPtr() const {
    return this->_operation_ptr;
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
};

struct TreeBuilder final {
  using Self = rq::TreeBuilder;

  rq::Expression *_operation_ptr = nullptr;
  rq::Expression *_last_ptr = nullptr;

  TreeBuilder() = default;
  TreeBuilder(const Self &) = default;
  TreeBuilder(Self &&) = default;
  ~TreeBuilder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOperation() const {
    return this->_operation_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setOperation(rq::Expression &operation) {
    rq::assignSingleValue(this->_operation_ptr, &operation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getOperation() {
    return rq::dereferencePtr(this->_operation_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getOperation() const {
    return rq::dereferencePtr(this->_operation_ptr);
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
  void startTree(rq::Expression &trunk);
  void appendBranch(rq::Expression &branch);
  void finishOperation(const rq::Token &last_token);
};

struct PrecedenceBuilder final {
  using Self = rq::PrecedenceBuilder;

  // the static static_frame is used only for acquiring new expressions
  std::reference_wrapper<rq::StaticFrame> _static_frame_ref;
  // the outermost operation that is returned at the end of the precedence
  rq::Expression *_outer_ptr = nullptr;
  // the current operation that is being filled with branches
  rq::Expression *_operation_ptr = nullptr;
  // the most recent parsed branch, which is not yet appended because it might
  // need to nest inside the next operation
  rq::Expression *_recent_ptr = nullptr;
  // the last branch that was appended to the operation
  rq::Expression *_last_ptr = nullptr;

  PrecedenceBuilder(rq::StaticFrame &static_frame) : _static_frame_ref(static_frame) {}
  PrecedenceBuilder(const Self &) = delete;
  PrecedenceBuilder(Self &&) = delete;
  ~PrecedenceBuilder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] rq::StaticFrame &getStaticFrame() { return this->_static_frame_ref.get(); }
  [[nodiscard]] const rq::StaticFrame &getStaticFrame() const {
    return this->_static_frame_ref.get();
  }
  void parseDoubleUnary(const rq::Token &token, rq::Keyword keyword);
  void parseUnary(const rq::Token &token, rq::Keyword keyword);
  void parseAscribe(const rq::Token &token, rq::Keyword keyword);
  void parseBinary(const rq::Token &token, rq::Keyword keyword);
  void parseNary(const rq::Token &token, rq::Keyword keyword);
  void parseNestingNary(const rq::Token &token, rq::Keyword keyword);
  void parseSequenceBranch(const rq::Token &token, rq::Keyword keyword,
                           rq::Expression &rvalue);
  void appendBranch(rq::Expression &branch);
  void appendUnaryAttribute(const rq::Token &token, rq::Keyword keyword);
  void setRecent(rq::Expression &branch);
  void setOnlyRecent(rq::Expression &branch);
  void appendRecent();
  RQ_ALWAYS_INLINE void setOuterOperation(rq::Expression &expression) {
    this->_outer_ptr = &expression;
    this->_operation_ptr = &expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOuter() const {
    return this->_outer_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getOuter() {
    return rq::dereferencePtr(this->_outer_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOperation() const {
    return this->_operation_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getOperation() const {
    return rq::dereferencePtr(this->_operation_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getOperation() {
    return rq::dereferencePtr(this->_operation_ptr);
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
    return rq::dereferencePtr(this->_operation_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getUnary() {
    RQ_ASSERT(this->getHasUnary(),
              "previous operator in precedence was not unary");
    return rq::dereferencePtr(this->_operation_ptr);
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &parseExpression() {
    return this->parsePrecedence11();
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
  [[nodiscard]] rq::Expression &parsePrecedence2();
  [[nodiscard]] rq::Expression &parsePrecedence1();
  [[nodiscard]] rq::Expression &parsePrecedence0();
  void parseNonStatementBranches(rq::Expression &operation, rq::TokenKind end);
  [[nodiscard]] rq::Keyword parseKeyword();
  [[nodiscard]] rq::Expression &parseEnclosedBracketExpression();
  [[nodiscard]] rq::Expression &parseEnclosedParenthesisExpression();
  [[nodiscard]] rq::Expression &parseEnclosedBraceExpression();
  void parseTrailer(rq::Expression& operation, rq::TokenRanger& keyword_ranger);
  [[nodiscard]] rq::Expression &parseStatementAttribute();
  [[nodiscard]] rq::Expression &parseTypeAttribute();
  [[nodiscard]] rq::Expression &parseLiteralOrMark(rq::Keyword keyword);
  [[nodiscard]] rq::Expression &parseInterpolatedString();
};

} // namespace rq
