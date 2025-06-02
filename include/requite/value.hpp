#pragma once

#include <requite/symbol.hpp>

#include <llvm/IR/Value.h>

namespace requite {

struct Value final {
    using Self = requite::Value;

    requite::Symbol _type = {};
    llvm::Value* _llvm_value_ptr = nullptr;
    
    // value.cpp
    Value() = default;
    explicit Value(const Self&) = default;
    explicit Value(Self&&) = default;
    ~Value() = default;
    Self& operator=(const Self&) = default;
    Self& operator=(Self&&) = default;
};

}