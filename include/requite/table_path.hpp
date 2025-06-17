#pragma once

#include <llvm/ADT/StringRef.h>

#include <vector>
#include <span>

namespace requite {

struct Scope;

struct TablePath final {
    using Self = requite::TablePath;
    
    std::vector<llvm::StringRef> _tables = {};
    bool _is_absolute = false;

    // table_path.cpp
    TablePath() = default;
    TablePath(const Self&) = delete;
    TablePath(Self &&) = delete;
    ~TablePath() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] const std::vector<llvm::StringRef>& getTables() const;
    [[nodiscard]] bool getIsAbsolute() const;
    void buildPath(requite::Scope& scope);
};

}