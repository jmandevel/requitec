#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ArrayRef.h>

namespace rq {

[[nodiscard]] bool parseCommandLineOptions(int argc, const char** argv);

[[nodiscard]] llvm::StringRef getInputFilePath();

[[nodiscard]] bool getHasToksFile();

[[nodiscard]] llvm::StringRef getToksFilePath();

[[nodiscard]] bool getHasAstFile();

[[nodiscard]] llvm::StringRef getAstFilePath();

[[nodiscard]] bool getHasSitastFile();

[[nodiscard]] llvm::StringRef getSitastFilePath();

[[nodiscard]] bool getHasSymbFile();

[[nodiscard]] llvm::StringRef getSymbFilePath();

[[nodiscard]] bool getHasLlvmFile();

[[nodiscard]] llvm::StringRef getLlvmFilePath();

[[nodiscard]] bool getHasAsmFile();

[[nodiscard]] llvm::StringRef getAsmFilePath();

[[nodiscard]] bool getHasObjFile();

[[nodiscard]] llvm::StringRef getObjFilePath();

[[nodiscard]] llvm::ArrayRef<std::string> getImportDirectories();

[[nodiscard]] bool getNoComment();

}
