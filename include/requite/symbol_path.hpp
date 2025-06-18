#pragma once

#include <llvm/ADT/StringRef.h>

#include <variant>
#include <vector>

namespace requite {

struct Scope;
struct Expression;


struct SymbolPath final {
    using Self = requite::SymbolPath;

    requite::Scope *_head_scope_ptr = nullptr;
    std::vector<llvm::StringRef> _body = {};

    // symbol_path.cpp
    SymbolPath() = default;
    SymbolPath(const Self&) = default;
    SymbolPath(Self&&) = default;
    ~SymbolPath() = default;
    [[nodiscard]] bool operator==(const Self&) const = default;
    [[nodiscard]] bool operator!=(const Self&) const = default;
    [[nodiscard]] bool getIsEmpty() const;
    [[nodiscard]] bool getHasHeadScope() const;
    void setHeadScope(requite::Scope &scope);
    [[nodiscard]] requite::Scope &getHeadScope();
    [[nodiscard]] const requite::Scope &getHeadScope() const;
    [[nodiscard]] std::vector<llvm::StringRef> &getBody();
    [[nodiscard]] const std::vector<llvm::StringRef> &getBody() const;
    [[nodiscard]] bool makeContainingPath(requite::Scope& scope);
};

} // namespace requite