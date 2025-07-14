#pragma once

namespace requite {

struct LookupTableEntry;

struct LookupTableResult final {
    using Self = requite::LookupTableResult;

    requite::LookupTableEntry *_entry_ptr = nullptr;

    // lookup_table_result.cpp
    LookupTableResult() = default;
    LookupTableResult(requite::LookupTableEntry& entry);
    LookupTableResult(const Self&) = default;
    LookupTableResult(Self&&) = default;
    ~LookupTableResult() = default;
    Self& operator=(const Self&) = default;
    Self& operator=(Self&&) = default;
    [[nodiscard]] bool getHasEntry() const;
    [[nodiscard]] requite::LookupTableEntry& getEntry();
    [[nodiscard]] const requite::LookupTableEntry& getEntry() const;
};

}