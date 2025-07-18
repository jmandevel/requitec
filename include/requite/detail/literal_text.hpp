// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/codeunits.hpp>

#include <llvm/ADT/StringRef.h>

namespace requite {

template <unsigned SMALL_SIZE_PARAM>
[[nodiscard]] inline requite::TextResult
getTextValue(llvm::StringRef text,
             llvm::SmallString<SMALL_SIZE_PARAM> &text_out) {
  REQUITE_ASSERT(text_out.empty());
  text_out.clear();
  text_out.reserve(text_out.size());
  for (llvm::StringRef sub_str = text; !sub_str.empty();
       sub_str = sub_str.drop_front()) {
    switch (const char c = sub_str.front()) {
    case '\\':
      sub_str = sub_str.drop_front();
      switch (const char c2 = sub_str.front()) {
      case 'a':
        text_out += '\a';
        break;
      case 'b':
        text_out += '\b';
        break;
      case 'e':
        text_out += '\e';
        break;
      case 'f':
        text_out += '\f';
        break;
      case 'n':
        text_out += '\n';
        break;
      case 'r':
        text_out += '\r';
        break;
      case 't':
        text_out += '\t';
        break;
      case 'v':
        text_out += '\v';
        break;
      case '\\':
        text_out += '\\';
        break;
      case '\"':
        text_out += '\"';
        break;
      case '\'':
        text_out += '\'';
        break;
      default:
        if (requite::getIsDigit(c2)) {
          unsigned value = 0;
          for (unsigned i = 0; i < 2; i++) {
            if (sub_str.empty()) {
              return requite::TextResult::ERROR_INVALID_ESCAPE;
            }
            const char c3 = sub_str.front();
            if (c3 == '\\') {
              if (i == 0) {
                return requite::TextResult::ERROR_INVALID_ESCAPE;
              }
              break;
            }
            if (!requite::getIsDigit(sub_str.front())) {
              return requite::TextResult::ERROR_INVALID_ESCAPE;
            }
            const unsigned multiplier =
                requite::getDigitBaseMultiplier(requite::getLowercase(c3));
            value = value * 16 + (multiplier - '0');
            sub_str = sub_str.drop_front();
          }
          text_out += static_cast<char>(value);
        }
      }
      break;
    default:
      text_out += c;
    }
  }
  return requite::TextResult::OK;
}

inline std::string getLiteralValue(llvm::StringRef text) {
  std::string buffer;
  buffer += "\"";
  for (const char c : text) {
    switch (c) {
    case '\"':
      buffer.append("\"\"");
      break;
    case '\t':
      buffer.append("\\t");
      break;
    case '\v':
      buffer.append("\\v");
      break;
    case '\n':
      buffer.append("\\v");
      break;
    case '\r':
      buffer.append("\\r");
      break;
    case '\\':
      buffer.append("\\\\");
      break;
    default:
      buffer += c;
    }
  }
  buffer += "\"";
  return buffer;
}

} // namespace requite