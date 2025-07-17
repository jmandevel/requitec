#pragma once

#include <requite/user_symbol.hpp>

#include <llvm/ADT/SmallVector.h>

namespace requite {

struct LookupTableEntry final {
    using Self = requite::LookupTableEntry;

    llvm::SmallVector<requite::UserSymbol, 1> _symbols = {};

    // lookup_table_entry.cpp
    LookupTableEntry() = default;
    LookupTableEntry(const Self&) = default;
    LookupTableEntry(Self&&) = default;
    ~LookupTableEntry() = default;
    Self& operator=(const Self&) = default;
    Self& operator=(Self&&) = default;
    void addSymbol(requite::RootSymbol root);
    void addSymbol(requite::RootSymbol root, requite::Use& use);
    void addSymbol(requite::RootSymbol root, requite::Import& import);
    [[nodiscard]] llvm::SmallVector<requite::UserSymbol, 1> &getUserSymbols();
    [[nodiscard]] const llvm::SmallVector<requite::UserSymbol, 1> &getUserSymbols() const;
    [[nodiscard]] bool getIsAmbiguous() const;
};

}