// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

namespace requite {

struct SourceLocation final {

    llvm::StringRef file = {};
    unsigned line = 0;
    unsigned column = 0;

    [[nodiscard]] inline
    bool operator==(const requite::SourceLocation &other) const {
        return this->file == other.file && this->line == other.line &&
               this->column == other.column;
    }

    [[nodiscard]] inline
    bool operator!=(const requite::SourceLocation &other) const {
        return !(*this == other);
    }
};

}