// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/codeunits.hpp>
#include <requite/context.hpp>
#include <requite/source.hpp>

#include <llvm/ADT/SmallString.h>

#include <cstddef>

namespace requite {

bool Context::validateSourceText(requite::Source &source) {
  bool is_ok = true;
  unsigned continue_bytes = 0;
  llvm::SMLoc location;
  int line = 0;
  int column = 0;
  for (const char &c : source.getText()) {
    if (!requite::getIsValid(c)) {
      llvm::Twine twine = llvm::Twine("invalid utf-8 codeunit. found \"") +
                          requite::getUtf8Name(c) + "\"";
      llvm::SmallString<64> buffer;
      llvm::StringRef message = twine.toStringRef(buffer);
      REQUITE_ASSERT(buffer.size() <= 64);
      this->logMessage(message);
      is_ok = false;
    }
    if (requite::getIsExtended(c)) {
      const unsigned new_continue_bytes = requite::getExtendedStartCount(c);
      if (continue_bytes != 0) {
        if (new_continue_bytes != 0) {
          llvm::Twine twine =
              llvm::Twine("expected utf-8 continuation codeunit. found \"") +
              requite::getUtf8Name(c) + "\"";
          llvm::SmallString<64> buffer;
          llvm::StringRef message = twine.toStringRef(buffer);
          REQUITE_ASSERT(buffer.size() <= 64);
          this->logMessage(

              message);
          is_ok = false;
          continue_bytes = 0;
        } else {
          continue_bytes--;
        }
      } else {
        continue_bytes = new_continue_bytes;
      }
    }
  }
  return is_ok;
}

} // namespace requite