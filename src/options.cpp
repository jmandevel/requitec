// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <llvm/Support/CommandLine.h>

#include <requite/options.hpp>

namespace requite {

static llvm::cl::opt<std::string>
    INPUT_FILE(llvm::cl::Positional,
               llvm::cl::desc("Path to the input source file."),
               llvm::cl::value_desc("<input file>"), llvm::cl::Required);

static llvm::cl::opt<std::string>
    OUTPUT_FILE("o", llvm::cl::desc("Path to the output build file."),
                llvm::cl::value_desc("<output file>"), llvm::cl::Required);

static llvm::cl::opt<Emit>
    EMIT("emit", llvm::cl::desc("Choose the type of target to build."),
         llvm::cl::values(
             clEnumValN(EMIT_TOKENS, "tokens", "Output csv token data."),
             clEnumValN(EMIT_PARSED, "parsed",
                        "Output intermediate requite source code of "
                        "the ast immediatly after parsing."),
             clEnumValN(EMIT_SITUATED, "situated",
                        "Output intermediate requite source code of "
                        "the ast immediatly situating."),
             clEnumValN(EMIT_EXPANDED, "expanded",
                        "Output intermediate requite source code of "
                        "the ast immediatly after expanding."),
             clEnumValN(EMIT_IR, "ir", "Output an llvm ir source file."),
             clEnumValN(EMIT_ASSEMBLY, "assembly",
                        "Output an assembly source file."),
             clEnumValN(EMIT_OBJECT, "object", "Output an object file.")),
         llvm::cl::init(EMIT_OBJECT));

static llvm::cl::opt<Form> FORM(
    "form", llvm::cl::desc("Choose the accepted form of requite."),
    llvm::cl::values(
        clEnumValN(FORM_NORMATIVE, "normative", "Only accept standard requite."),
        clEnumValN(FORM_INTERMEDIATE, "intermediate",
                   "Only accept requite source code with operations including "
                   "operations with intermediate opcodes."),
        clEnumValN(FORM_MULTIPLICATIVE, "multiplicative",
                   "Accept a mixture of normative and intermediate requite "
                   "source code.")),
    llvm::cl::init(FORM_NORMATIVE));

llvm::StringRef getInputFilePath() {
  return requite::INPUT_FILE.getValue();
}

llvm::StringRef getOutputFilePath() {
  return requite::OUTPUT_FILE.getValue();
}

requite::Emit getEmitMode() {
  return requite::EMIT.getValue();
}

requite::Form getForm() {
  return requite::FORM.getValue();
}

bool getIsNormativeRequiteOk() {
  return (requite::FORM.getValue() &
          requite::FORM_NORMATIVE) == requite::FORM_NORMATIVE;
}

bool getIsIntermediateRequiteOk() {
  return (requite::FORM.getValue() &
          requite::FORM_INTERMEDIATE) ==
         requite::FORM_INTERMEDIATE;
}

} // namespace requite::options