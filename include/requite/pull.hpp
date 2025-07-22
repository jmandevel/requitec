#pragma once

#include <requite/attribute_flags.hpp>

namespace requite {

struct Module;
struct Expression;
struct Scope;

struct Pull final {
    using Self = requite::Pull;

    requite::Expression* _expression_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    bool is_exported = false;
    requite::Scope* _pulled_ptr = nullptr;
    requite::Module* _module_ptr = nullptr;
    requite::Pull *_next_ptr = nullptr;

    // pull.cpp

};

}