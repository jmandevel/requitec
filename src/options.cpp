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
    TOKS_FILE("toks", llvm::cl::desc("Path to the output token csv file."),
              llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    AST_FILE("ast", llvm::cl::desc("Path to the output ast rq file."),
             llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    SITAST_FILE("sitast",
                llvm::cl::desc("Path to the output situated ast rq file."),
                llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    SYMB_FILE("symb", llvm::cl::desc("Path to the output symbols json file."),
              llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    LL_FILE("llvm", llvm::cl::desc("Path to the output llvm ll file."),
            llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    ASM_FILE("asm", llvm::cl::desc("Path to the output s file."),
             llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::opt<std::string>
    OBJ_FILE("obj", llvm::cl::desc("Path to the output o file."),
             llvm::cl::value_desc("<output file>"), llvm::cl::Optional);

static llvm::cl::list<std::string>
    IMPORT_DIRECTORIES("I", llvm::cl::desc("import directories"),
                       llvm::cl::ZeroOrMore, llvm::cl::value_desc("dir"));

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
    // context does not exist yet so log directly to error output stream
    std::error_code ec = llvm::sys::fs::real_path(rq::INPUT_FILE, buffer, true);
    if (ec) {
      llvm::errs() << "Failed to resolve path of input file\n\tpath: "
                   << rq::INPUT_FILE << "\n\treason: " << ec.message() << "\n";
      is_ok = false;
    } else {
      if (!llvm::sys::fs::is_regular_file(buffer)) {
        llvm::errs() << "Input file path is not file\n\tpath: "
                     << rq::INPUT_FILE << "\n";
        is_ok = false;
      } else {
        rq::INPUT_FILE.setValueStr(buffer.str());
      }
    }
  }
  if (!rq::getHasToksFile() && !rq::getHasAstFile() &&
      !rq::getHasSitastFile() && !rq::getHasSymbFile() &&
      !rq::getHasLlvmFile() && !rq::getHasAsmFile() && !rq::getHasObjFile()) {
        llvm::errs() << "no output file specified\n";
        is_ok = false;
      }
    return is_ok;
}

llvm::StringRef getInputFilePath() { return rq::INPUT_FILE.getValue(); }

bool getHasToksFile() {
  return rq::TOKS_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getToksFilePath() {
  return rq::TOKS_FILE.getValue();
}

bool getHasAstFile() {
  return rq::AST_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getAstFilePath() {
  return rq::AST_FILE.getValue();
}

bool getHasSitastFile() {
  return rq::SITAST_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getSitastFilePath() {
  return rq::SITAST_FILE.getValue();
}

bool getHasSymbFile() {
  return rq::SYMB_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getSymbFilePath() {
  return rq::SYMB_FILE.getValue();
}

bool getHasLlvmFile() {
  return rq::LL_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getLlvmFilePath() {
  return rq::LL_FILE.getValue();
}

bool getHasAsmFile() {
  return rq::ASM_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getAsmFilePath() {
  return rq::ASM_FILE.getValue();
}

bool getHasObjFile() {
  return rq::OBJ_FILE.getNumOccurrences() == 1;
}

llvm::StringRef getObjFilePath() {
  return rq::OBJ_FILE.getValue();
}

llvm::ArrayRef<std::string> getImportDirectories() {
  return rq::IMPORT_DIRECTORIES;
}

bool getNoComment() { return rq::NO_COMMENT.getValue(); }

} // namespace rq
