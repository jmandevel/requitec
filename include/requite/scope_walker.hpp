#pragma once

#include <requite/scope_walker_result.hpp>

#include <llvm/ADT/SmallPtrSet.h>

#include <functional>

namespace requite {

struct Scope;
struct Context;
struct Module;
struct Node;

struct ScopeWalker final {
  using Self = requite::ScopeWalker;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Scope> _scope_ref;
  requite::ScopeWalkerResult _result = {};
  llvm::SmallPtrSet<requite::Scope *, 16> _searched_scopes = {};

  // scope_walker.cpp
  ScopeWalker(requite::Context &context, requite::Scope &scope);
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::ScopeWalkerResult &getResult();
  [[nodiscard]] const requite::ScopeWalkerResult &getResult() const;
  [[nodiscard]] requite::ScopeWalkerResult doSearch(
      const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
          &predicate);
  void doModuleExportsSearch(
      requite::Module &module,
      const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
          &predicate);
  [[nodiscard]] bool doImportSearch(
      requite::Node &node,
      const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
          &predicate);
  [[nodiscard]] bool doUseSearch(
      requite::Node &node,
      requite::Scope& scope,
      const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
          &predicate);
};

} // namespace requite