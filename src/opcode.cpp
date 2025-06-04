// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/opcode.hpp>

#include <memory>
#include <type_traits>
#include <utility>

namespace requite {

requite::Opcode Context::getOpcode(llvm::StringRef text) const {
  REQUITE_ASSERT(!this->_opcode_table.empty());
  if (!this->_opcode_table.contains(text)) {
    return requite::Opcode::__NONE;
  }
  return this->_opcode_table.at(text);
}

void Context::createOpcodeTable() {
  REQUITE_ASSERT(this->_opcode_table.empty());
  for (std::underlying_type_t<requite::Opcode> opcode_i = 0;
       opcode_i <= requite::OPCODE_COUNT; opcode_i++) {
    const requite::Opcode opcode = static_cast<requite::Opcode>(opcode_i);
    std::string_view name = requite::getName(opcode);
    this->_opcode_table.insert({name, opcode});
  }
}

} // namespace requite