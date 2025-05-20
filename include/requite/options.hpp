// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/Support/CommandLine.h>

#include <string>

namespace requite::options {

inline llvm::cl::opt<std::string>
    OUTPUT_FILE("output-file", llvm::cl::Optional,
                llvm::cl::desc("Specify output file"),
                llvm::cl::value_desc("filename"));

inline llvm::cl::opt<std::string>
    SOURCE_DIRECTORY("source-directory", llvm::cl::init("."),
                     llvm::cl::Optional,
                     llvm::cl::desc("Specify source file input directory"),
                     llvm::cl::value_desc("directory"));

inline llvm::cl::opt<std::string>
    BUILD_DIRECTORY("build-directory", llvm::cl::init("build/"),
                    llvm::cl::Optional,
                    llvm::cl::desc("Specify executable file output directory"),
                    llvm::cl::value_desc("directory"));

inline llvm::cl::opt<std::string> INTERMEDIATE_DIRECTORY(
    "intermediate-directory", llvm::cl::init("intermediate/"),
    llvm::cl::Optional,
    llvm::cl::desc("Specify intermediate file output directory"),
    llvm::cl::value_desc("directory"));

inline llvm::cl::opt<bool>
    QUIET("quiet", llvm::cl::init(false), llvm::cl::Optional,
          llvm::cl::desc("Silence log messages to stdout and stderr"));

enum class HaltStage {
  VALIDATE_SOURCE_TEXT,
  PARSE_TOKENS,
  VALIDATE_TOKENS,
  PARSE_AST,
  VALIDATE_AST,
  TABULATE,
  GENERATE_IR,
  COMPILE,
  LINK
};

inline llvm::cl::opt<requite::options::HaltStage> STOP_STAGE(
    "halt-stage", llvm::cl::desc("The stage to stop compilation"),
    llvm::cl::init(requite::options::HaltStage::LINK), llvm::cl::Optional,
    llvm::cl::values(
        clEnumValN(requite::options::HaltStage::VALIDATE_SOURCE_TEXT,
                   "validate-source-text",
                   "Halt after source file text validation"),
        clEnumValN(requite::options::HaltStage::PARSE_TOKENS, "parse-tokens",
                   "Halt after parsing tokens"),
        clEnumValN(requite::options::HaltStage::VALIDATE_TOKENS,
                   "validate-tokens", "Halt after token validation"),
        clEnumValN(requite::options::HaltStage::PARSE_AST, "parse-ast",
                   "Halt after AST parsing"),
        clEnumValN(requite::options::HaltStage::VALIDATE_AST, "validate-ast",
                   "Halt after AST validation"),
        clEnumValN(requite::options::HaltStage::TABULATE, "tabulate",
                   "Halt after tabulating symbols"),
        clEnumValN(requite::options::HaltStage::GENERATE_IR, "generate-ir",
                   "Halt after generating LLVM IR"),
        clEnumValN(requite::options::HaltStage::COMPILE, "compile",
                   "Halt after compilation"),
        clEnumValN(requite::options::HaltStage::LINK, "link",
                   "Halt after linking")));

inline llvm::cl::list<std::string>
    MODULE_FILES(llvm::cl::Positional, llvm::cl::desc("List of module files"),
                 // llvm::cl::Required, // instead of using this, we manually
                 // check to output custom error message.
                 llvm::cl::value_desc("filename"));

inline llvm::cl::opt<bool> NAME_INTERMEDIATE_OPCODES(
    "name-intermediate-opcodes",
    llvm::cl::desc("Name the opcodes output to intermediate source files"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> INTERMEDIATE_TOKEN_CSV(
    "token-csv",
    llvm::cl::desc("Output csv files with token data per source file"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> INTERMEDIATE_PARSED_AST(
    "parse-ast",
    llvm::cl::desc("Output rq files with abstract syntax tree data per source "
                   "file after parse stage"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> INTERMEDIATE_SITUATED_AST(
    "situated-ast",
    llvm::cl::desc("Output rq files with abstract syntax tree data per source "
                   "file after initial situation stage"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> INTERMEDATE_TABULATION_TRON(
    "tabulation-tron",
    llvm::cl::desc("Output tron files of generic scopes per source file after "
                   "tabulation stage"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> INTERMEDIATE_LLVM_IR(
    "llvm-ir",
    llvm::cl::desc("Output ir files with generated LLVM IR per source file"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> DEBUG("debug",
                                 llvm::cl::desc("Build with debug info"),
                                 llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> RUNTIME_BAKE(
    "runtime-bake",
    llvm::cl::desc("Execute baked expressions at runtime before entry_point "
                   "point instead of at compile time"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool>
    USE_HYPER_THREADS("use-hyper-threads", llvm::cl::desc("Use hyper-threads"),
                      llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> LIMIT_THREADS_TO_CORES(
    "limit-threads-to-cores",
    llvm::cl::desc("Limit the amount of threads to the amount of CPU cores"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<bool> SINGLE_THREAD_EXECUTION(
    "single-thread-execution",
    llvm::cl::desc("Execute everything on the main thread"),
    llvm::cl::Optional, llvm::cl::init(false));

inline llvm::cl::opt<unsigned>
    THREAD_COUNT("thread-count",
                 llvm::cl::desc("The amount of threads to use in the thread "
                                "pool. Specify 0 to match core count"),
                 llvm::cl::Optional, llvm::cl::init(0));

inline llvm::cl::list<std::string> LINK_TARGETS(
    "link-target",
    llvm::cl::desc("Specify an .obj file or static library to link with"),
    llvm::cl::ZeroOrMore, llvm::cl::value_desc("target"));

[[nodiscard]]
bool parseOptions(int argc, const char **argv);

} // namespace requite::options