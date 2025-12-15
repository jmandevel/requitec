#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ArrayRef.h>

namespace rq {

enum Emit {
    EMIT_TOKENS,
    EMIT_PARSED,
    EMIT_SITUATED,
    EMIT_SYMBOLS,
    EMIT_IR,
    EMIT_ASSEMBLY,
    EMIT_OBJECT
};

[[nodiscard]] bool parseCommandLineOptions(int argc, const char** argv);

[[nodiscard]] llvm::StringRef getInputFilePath();

[[nodiscard]] llvm::StringRef getOutputFilePath();

[[nodiscard]] llvm::ArrayRef<std::string> getImportDirectories();

[[nodiscard]] rq::Emit getEmitMode();

[[nodiscard]] bool getNoComment();

}
