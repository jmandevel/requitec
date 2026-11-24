#pragma once

#include <rq/name.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallPtrSet.h>

namespace rq {

struct Context;
struct SymbolTable;
struct Expression;

struct PathComponent final {
  using Self = rq::PathComponent;

  rq::Keyword _keyword;
  std::string _name;
  rq::Expression *_expression_ptr;

  explicit RQ_ALWAYS_INLINE PathComponent(llvm::StringRef name,
                                          rq::Keyword keyword,
                                          rq::Expression &expression)
      : _keyword(keyword), _name(name), _expression_ptr(&expression) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const {
    if (this->_keyword == rq::Keyword::NONE) {
      return rq::Name{this->_name};
    }
    return rq::Name{this->_keyword};
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

struct SymbolicPathSnapshot final {
  using Self = rq::SymbolicPathSnapshot;

  rq::Route &_route;
  rq::SymbolicPath &_symbolic_path;
  rq::SymbolTable& _host;

  explicit RQ_ALWAYS_INLINE
  SymbolicPathSnapshot(rq::Route &route, rq::SymbolicPath &symbolic_path, rq::SymbolTable& host);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Route &getRoute() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Route &getRoute();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolicPath &
  getSymbolicPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolicPath &getSymbolicPath();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable& getHost() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable& getHost();
};

struct RValue final {
  using Self = rq::RValue;
};

struct Generator final {
  using Self = rq::Generator;

  rq::Context *_context_ptr;

  explicit RQ_ALWAYS_INLINE Generator(rq::Context &context)
      : _context_ptr(&context) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_context_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_context_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::RValue
  resolveRvalue(rq::SymbolTable &host, rq::Expression &expression);

  void lookup(rq::SymbolTable &host, llvm::ArrayRef<rq::PathComponent> path,
              llvm::SmallPtrSet<rq::Entity *, 1> &candidates);

  void dfsPath(rq::Module &module, rq::TableMember &receiver,
               llvm::ArrayRef<rq::PathComponent> path,
               llvm::SmallPtrSet<rq::Entity *, 1> &candidates);

  void dfsSymbolicPath(rq::Module &module, rq::SymbolTable &host,
    llvm::ArrayRef<rq::PathComponent> path,
                       llvm::SmallPtrSet<rq::Route *, 1> found_redirects,
                       llvm::ArrayRef<rq::SymbolicPathSnapshot> snapshot_stack,
                       rq::Route &redirect,
                       rq::SymbolicPath& symbolic_path,
                       llvm::SmallPtrSet<rq::Entity *, 1> &candidates);

  [[nodiscard]] bool tryImplement(rq::TableMember &member);

  [[nodiscard]] rq::Entity *tryLookupBuiltin(rq::Name name);
};

} // namespace rq
