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

enum Form {
  FORM_NORMATIVE = (1 << 0),
  FORM_INTERMEDIATE = (1 << 1),
  FORM_MULTIPLICATIVE = (FORM_NORMATIVE | FORM_INTERMEDIATE)
};

[[nodiscard]] bool parseCommandLineOptions(unsigned argc, const char** argv);

[[nodiscard]] llvm::StringRef getInputFilePath();

[[nodiscard]] llvm::StringRef getOutputFilePath();

[[nodiscard]] llvm::ArrayRef<std::string> getImportDirectories();

[[nodiscard]] requite::Emit getEmitMode();

[[nodiscard]] requite::Form getForm();

[[nodiscard]] bool getIsNormativeRequiteOk();

[[nodiscard]] bool getIsIntermediateRequiteOk();

} // namespace requite