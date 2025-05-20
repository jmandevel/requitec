// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>

namespace requite {

struct Context;
struct Module;

struct Finalizer final {
    using Self = requite::Finalizer;

    std::reference_wrapper<requite::Context> _context_ref;
    std::reference_wrapper<requite::Module> _module_ref;
    bool _is_ok = true;

    // finalizer.cpp
    Finalizer(requite::Context &context, requite::Module &module);
    Finalizer(Self &that) = delete;
    Finalizer(Self &&that) = default;
    ~Finalizer() = default;
    Self &operator=(Self &rhs) = delete;
    Self &operator=(Self &&rhs) = default;
    [[nodiscard]]
    requite::Context &getContext() const;
    [[nodiscard]]
    const requite::Context &getContext();
    [[nodiscard]]
    requite::Module &getModule() const;
    [[nodiscard]]
    const requite::Module &getModule();
    [[nodiscard]]
    bool getIsOk() const;
    void setNotOk();

    // finalize_locals.cpp
    void finalizeLocals();
    // TODO    
};

} // namespace requite