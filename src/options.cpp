#include <rq/options.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>

#include <string>

namespace rq {

static llvm::cl::opt<std::string>
    INPUT_FILE(llvm::cl::Positional,
               llvm::cl::desc("Path to the input source file."),
               llvm::cl::value_desc("<input file>"), llvm::cl::Required);

static llvm::cl::opt<bool>
    NO_COMMENT("no-comment",
               llvm::cl::desc("Do not add comments to output files."),
               llvm::cl::value_desc("<boolean>"), llvm::cl::Optional,
               llvm::cl::init(false));

static llvm::cl::opt<std::string>
    OUTPUT_FILE("o", llvm::cl::desc("Path to the output build file."),
                llvm::cl::value_desc("<output file>"), llvm::cl::Required);

static llvm::cl::list<std::string>
    IMPORT_DIRECTORIES("I", llvm::cl::desc("import directories"),
                       llvm::cl::ZeroOrMore, llvm::cl::value_desc("dir"));

static llvm::cl::opt<Emit> EMIT(
    "emit", llvm::cl::desc("Choose the type of target to build."),
    llvm::cl::values(
        clEnumValN(EMIT_TOKENS, "tokens", "Output csv token data."),
        clEnumValN(EMIT_PARSED, "parsed",
                   "Output intermediate requite source code of "
                   "the ast immediatly after parsing."),
        clEnumValN(EMIT_SITUATED, "situated",
                   "Output intermediate requite source code of "
                   "the ast immediatly after situating."),
        clEnumValN(EMIT_SYMBOLS, "symbols",
                   "Output a markup language file listing user symbols."),
        clEnumValN(EMIT_IR, "ir", "Output an llvm ir source file."),
        clEnumValN(EMIT_ASSEMBLY, "assembly",
                   "Output an assembly source file."),
        clEnumValN(EMIT_OBJECT, "object", "Output an object file.")),
    llvm::cl::init(EMIT_OBJECT));

bool parseCommandLineOptions(int argc, const char **argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv);
  llvm::SmallString<256> buffer;
  bool is_ok = true;
  for (std::string &import_directory : rq::IMPORT_DIRECTORIES) {
    buffer.clear();
    std::error_code ec =
        llvm::sys::fs::real_path(import_directory, buffer, true);
    if (ec) {
      llvm::errs() << "Failed to resolve path of import directory\n\tpath: "
                   << import_directory << "\n\treason: " << ec.message()
                   << "\n";
      is_ok = false;
      continue;
    }
    if (!llvm::sys::fs::is_directory(buffer)) {
      llvm::errs() << "Import directory path is not directory\n\tpath: "
                   << import_directory << "\n";
      is_ok = false;
      continue;
    }
    import_directory = buffer.str();
  }
  {
    buffer.clear();
    std::error_code ec = llvm::sys::fs::real_path(rq::INPUT_FILE, buffer, true);
    if (ec) {
      llvm::errs() << "Failed to resolve path of input file\n\tpath: "
                   << rq::INPUT_FILE << "\n\treason: " << ec.message() << "\n";
      is_ok = false;
    } else {
      if (!llvm::sys::fs::is_regular_file(buffer)) {
        llvm::errs() << "input file path is not file\n\tpath: "
                     << rq::INPUT_FILE << "\n";
        is_ok = false;
      } else {
        rq::INPUT_FILE.setValueStr(buffer.str());
      }
    }
  }
  return is_ok;
}

llvm::StringRef getInputFilePath() { return rq::INPUT_FILE.getValue(); }

llvm::StringRef getOutputFilePath() { return rq::OUTPUT_FILE.getValue(); }

llvm::ArrayRef<std::string> getImportDirectories() {
  return rq::IMPORT_DIRECTORIES;
}

rq::Emit getEmitMode() { return rq::EMIT.getValue(); }

bool getNoComment() { return rq::NO_COMMENT.getValue(); }

} // namespace rq
