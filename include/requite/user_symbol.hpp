#pragma once 

#include <requite/symbol.hpp>

namespace requite {

struct Use;
struct Import;

enum class UserSymbolType {
    INTERNAL,
    USED,
    IMPORTED
};

struct UserSymbol final {
    using Self = UserSymbol;

    requite::RootSymbol _symbol;
    requite::UserSymbolType _type;
    union {
        void* _nothing_ptr = nullptr;
        requite::Use* _use_ptr;
        requite::Import* _import_ptr;
    };

    // user_symbol.cpp
    UserSymbol(requite::RootSymbol root);
    UserSymbol(requite::RootSymbol root, requite::Use& use);
    UserSymbol(requite::RootSymbol root, requite::Import& import);
    UserSymbol(const Self&) = default;
    UserSymbol(Self&&) = default;
    ~UserSymbol() = default;
    Self& operator=(const Self&) = default;
    Self& operator=(Self&&) = default;
    [[nodiscard]] requite::UserSymbolType getType() const;
    [[nodiscard]] requite::RootSymbol & getRoot();
    [[nodiscard]] const requite::RootSymbol &getRoot() const;
    [[nodiscard]] requite::Use& getUse();
    [[nodiscard]] const requite::Use& getUse() const;
    [[nodiscard]] requite::Import& getImport();
    [[nodiscard]] const requite::Import& getImport() const;    
};




}