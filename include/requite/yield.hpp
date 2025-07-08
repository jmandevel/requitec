#pragma once

#include <requite/situation.hpp>

namespace requite {

struct Expression;
struct Object;
struct Procedure;
struct Table;
struct Property;
struct Global;
struct Alias;

enum class YieldType {
    NONE,
    OBJECT,
    PROCEDURE,
    TABLE,
    PROPERTY,
    GLOBAL,
    ALIAS,
    EXPAND_STATEMENT
};

struct Yield final {
    using Self = Yield;
    
    requite::YieldType _type = requite::YieldType::NONE;
    requite::Situation _situation = requite::Situation::NONE;
    union {
        void* _nothing_ptr = nullptr;
        requite::Object* _object_ptr;
        requite::Procedure* _procedure_ptr;
        requite::Table* _table_ptr;
        requite::Property* _property_ptr;
        requite::Global* _global_ptr;
        requite::Alias* _alias_ptr;
        requite::Expression* _expand_statement_ptr;
    };  
};

}