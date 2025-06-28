// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/signature.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <vector>

namespace requite {

bool Context::prototypeEntryPoint(requite::Procedure &procedure) {
    REQUITE_ASSERT(procedure.getType() == requite::ProcedureType::ENTRY_POINT);
    procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
    requite::Signature &signature = procedure.getSignature();
    requite::Symbol &return_type = signature.getReturnType();
    requite::RootSymbol &return_root = return_type.getRoot();
    return_root.setType(requite::RootSymbolType::SIGNED_INTEGER);
    const unsigned pointer_depth = this->getAddressDepth();
    return_root.setDepth(pointer_depth);
    return true;
}

} // namespace requite