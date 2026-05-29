#pragma once

#include <llvm/ADT/PointerUnion.h>

namespace rq {

struct Expression;
struct Symbol;
struct Constant;
struct Instruction;

struct Entity final {
    using Self = rq::Entity;

    llvm::PointerUnion<rq::Expression*, rq::Symbol*, rq::Constant*, rq::Instruction*> _data_ptr{nullptr}; 
};

}