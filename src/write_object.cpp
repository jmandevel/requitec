#include <requite/context.hpp>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>

namespace requite {

bool Context::writeObject(llvm::StringRef output_path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(output_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        llvm::Twine(output_path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()));
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::ObjectFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logMessage(
        llvm::Twine("error: failed to add passes to emit file\n\tpath: ") +
        llvm::Twine(output_path));
    return false;
  }
  pass.run(this->getLlvmModule());
  fout.flush();
  return true;
}

} // namespace requite