#pragma once

#include <requite/symbol.hpp>

#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/StringRef.h>

#include <functional>
#include <vector>
#include <span>

namespace requite {

struct Scope;
struct Expression;

//struct LookupContext final {
//  using Self = requite::LookupContext;
//
//  llvm::SmallPtrSet<requite::Scope *, 64> _searched_scope_set = {};
//  std::vector<llvm::StringRef> _containing_path;
//  std::reference_wrapper<requite::Scope> _scope_ref;
//
//  // lookup_context.cpp
//  LookupContext(requite::Scope& scope);
//  LookupContext(const Self &) = delete;
//  LookupContext(Self &&) = delete;
//  ~LookupContext() = default;
//  Self &operator=(const Self &) = delete;
//  Self &operator=(Self &&) = delete;
//  [[nodiscard]] bool operator==(const Self& rhs) const;
//  [[nodiscard]] bool operator!=(const Self& rhs) const;
//  [[nodiscard]] llvm::SmallPtrSet<requite::Scope *, 16>& getSearchedScopeSet();
//  [[nodiscard]] const llvm::SmallPtrSet<requite::Scope *, 16>& getSearchedScopeSet() const;
//  [[nodiscard]] std::span<const llvm::StringRef> getContainingPath() const;
//  [[nodiscard]] requite::Scope& getScope();
//  [[nodiscard]] requite::Scope& getScope() const;
//
//  requite::RootSymbol lookupSymbol(llvm::StringRef name);
//};

} // namespace requite