// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

template <unsigned SMALL_SIZE_PARAM>
llvm::StringRef getCsvValueText(llvm::SmallString<SMALL_SIZE_PARAM> &buffer,
                                llvm::StringRef text) {
  buffer.clear();
  std::size_t buffer_size = 0;
  buffer += "\" ";
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
  buffer += " \"";
  return buffer;
}

} // namespace requite