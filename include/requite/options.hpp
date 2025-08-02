// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ArrayRef.h>

#include <string>

namespace requite {

enum Emit {
  EMIT_TOKENS,
  EMIT_PARSED,
  EMIT_SITUATED,
  EMIT_SYMBOLS,
  EMIT_IMPLEMENTED,
  EMIT_IR,
  EMIT_ASSEMBLY,
  EMIT_OBJECT
};

[[nodiscard]] bool parseCommandLineOptions(unsigned argc, const char** argv);

[[nodiscard]] llvm::StringRef getInputFilePath();

[[nodiscard]] llvm::StringRef getOutputFilePath();

[[nodiscard]] llvm::ArrayRef<std::string> getImportDirectories();

[[nodiscard]] requite::Emit getEmitMode();

[[nodiscard]] bool getIsNormativeRequiteOk();

[[nodiscard]] bool getIsIntermediateRequiteOk();

} // namespace requite