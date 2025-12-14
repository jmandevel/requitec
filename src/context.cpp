#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/context.hpp>
#include <rq/options.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/tabulator.hpp>
#include <rq/tokenize.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>

#include <system_error>
#include <utility>

namespace rq {

bool Context::validateSourceText(const rq::Module &module) {
  bool is_ok = true;
  unsigned continue_bytes = 0;
  llvm::SMLoc extended_char_start;
  for (const char &c : module.getSourceText()) {
    if (!rq::getIsValid(c)) {
      llvm::Twine message =
          llvm::Twine("invalid utf-8 codeunit \"") + rq::getUtf8Name(c) + "\"";
      this->logMessage(llvm::SMLoc::getFromPointer(&c), rq::LogType::ERROR,
                       message, {}, {});
      is_ok = false;
    }
    if (rq::getIsExtended(c)) {
      const unsigned new_continue_bytes = rq::getExtendedStartCount(c);
      if (continue_bytes != 0) {
        if (new_continue_bytes != 0) {
          llvm::Twine message =
              llvm::Twine("invalid utf-8 continuation codeunit \"") +
              rq::getUtf8Name(c) + "\"";
          llvm::SMLoc cur_location = llvm::SMLoc::getFromPointer(&c);
          this->logMessage(cur_location, rq::LogType::ERROR, message,
                           {llvm::SMRange(extended_char_start, cur_location)},
                           {});
          is_ok = false;
          extended_char_start = llvm::SMLoc();
          continue_bytes = 0;
        } else {
          continue_bytes--;
        }
      } else {
        extended_char_start = llvm::SMLoc::getFromPointer(&c);
        continue_bytes = new_continue_bytes;
      }
    }
  }
  return is_ok;
}

bool Context::tokenizeSourceText(const rq::Module &module,
                                 std::vector<rq::Token> &tokens) {
  rq::Tokenizer tokenizer(*this, module.getSourceText(), tokens);
  const bool is_ok = tokenizer.tokenizeSourceText();
  return is_ok;
}

void Context::initializeKeywordMap() {
  RQ_ASSERT(this->_keyword_map.empty(), "keyword map not empty");
  for (std::underlying_type_t<rq::Keyword> keyword_i = 0;
       keyword_i <= rq::KEYWORD_COUNT; keyword_i++) {
    const rq::Keyword keyword = static_cast<rq::Keyword>(keyword_i);
    std::string_view name = rq::getName(keyword);
    this->_keyword_map.insert({name, keyword});
  }
}

rq::SourceLocation Context::getSourceLocation(llvm::SMLoc llvm_location) {
  const int buffer_i =
      this->_llvm_source_mgr.FindBufferContainingLoc(llvm_location);
  auto line_and_column =
      this->_llvm_source_mgr.getLineAndColumn(llvm_location, buffer_i);
  rq::SourceLocation source_location = {};
  source_location.file =
      this->_llvm_source_mgr.getMemoryBuffer(buffer_i)->getBufferIdentifier();
  source_location.line = line_and_column.first;
  source_location.column = line_and_column.second;
  return source_location;
}

rq::Keyword Context::getKeyword(llvm::Twine name) {
  llvm::SmallString<64> buffer;
  llvm::StringMapIterator<rq::Keyword> it =
      this->_keyword_map.find(name.toStringRef(buffer));
  if (it == this->_keyword_map.end()) {
    return rq::Keyword::__NONE;
  }
  return it->getValue();
}

rq::SourceRange Context::getSourceRange(const rq::Expression &expression) {
  rq::SourceRange source_range = {};
  source_range.start = this->getSourceLocation(expression.getLlvmSourceStart());
  source_range.end = this->getSourceLocation(expression.getLlvmSourceEnd());
  return source_range;
}

std::error_code Context::canonicalizePath(llvm::SmallVectorImpl<char> &path) {
  std::error_code ec = llvm::sys::fs::make_absolute(path);
  if (ec) {
    return ec;
  }
  ec = llvm::sys::fs::real_path(path, path, false);
  if (ec) {
    return ec;
  }
  llvm::sys::path::remove_dots(path, /*remove_dot_dot=*/true);
  llvm::sys::path::native(path);
  return ec;
}

llvm::ErrorOr<llvm::MemoryBufferRef>
Context::loadRequiteFileBuffer(llvm::StringRef path) {
  RQ_ASSERT(!path.empty(), "path is empty");
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer_eo =
      llvm::MemoryBuffer::getFile(path, /*IsText=*/true,
                                  /*RequiresNullTerminator=*/true,
                                  /*IsVolatile=*/false, std::nullopt);
  if (buffer_eo) {
    llvm::MemoryBufferRef ref = buffer_eo.get().get()->getMemBufferRef();
    std::ignore = this->_llvm_source_mgr.AddNewSourceBuffer(
        std::move(buffer_eo.get()), llvm::SMLoc());
    return ref;
  }
  return buffer_eo.getError();
}

bool Context::loadSourceModule() {
  RQ_ASSERT(!this->getHasSourceModule(), "already has source module");
  llvm::SmallString<128> input_path = rq::getInputFilePath();
  llvm::StringRef file_extension = llvm::sys::path::extension(input_path);
  if (file_extension != rq::REQUITE_EXTENSION) {
    this->logMessage(
        llvm::Twine(
            "error: source file does not have a \".rq\" extension\n\tpath: ") +
        input_path);
    return false;
  }
  std::error_code ec = this->canonicalizePath(input_path);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to canonicalize source file path\n\tpath: ") +
        input_path + "\n\treason: " + ec.message());
    return false;
  }
  llvm::ErrorOr<llvm::MemoryBufferRef> buffer_eo =
      this->loadRequiteFileBuffer(input_path);
  if (!buffer_eo) {
    this->logMessage(
        llvm::Twine("error: failed to load source file buffer\n\tpath: ") +
        input_path + "\n\treason:" + buffer_eo.getError().message());
    return false;
  }
  llvm::StringRef final_path = this->getTopStaticFrame().saveString(input_path);
  rq::Module &source_module =
      this->getTopStaticFrame().allocateValue<rq::Module>(
          rq::ModuleKind::SOURCE, final_path, std::move(buffer_eo.get()));
  rq::assignSingleValue(this->_source_module_ptr, &source_module);
  this->_module_map.insert(std::pair<llvm::StringRef, rq::Module *>(
      input_path, &this->getSourceModule()));
  return true;
}

rq::Module *Context::loadImportModule(rq::Expression &expression,
                                      llvm::StringRef import_string) {
  llvm::SmallString<128> found_path;
  bool file_found = false;
  for (const std::string &dir : rq::getImportDirectories()) {
    llvm::SmallString<128> candidate_path(dir);
    llvm::sys::path::append(candidate_path, import_string,
                            rq::REQUITE_EXTENSION);
    if (llvm::sys::fs::exists(candidate_path)) {
      found_path = candidate_path;
      file_found = true;
      break;
    }
  }
  if (!file_found) {
    this->logMessage(expression.getLlvmSourceStart(), rq::LogType::NOTE,
                     llvm::Twine("error: could not locate import file \"") +
                         import_string + "\" in any import directory\n",
                     {expression.getLlvmSourceRange()}, {});
    return nullptr;
  }
  std::error_code ec = this->canonicalizePath(found_path);
  if (ec) {
    this->logMessage(
        expression.getLlvmSourceStart(), rq::LogType::NOTE,
        llvm::Twine(
            "error: failed to canonicalize import file path\n\treason:") +
            ec.message(),
        {expression.getLlvmSourceRange()}, {});
    return nullptr;
  }
  auto found_it = this->_module_map.find(found_path);
  if (found_it != this->_module_map.end()) {
    return found_it->second;
  }
  llvm::ErrorOr<llvm::MemoryBufferRef> buffer_eo =
      this->loadRequiteFileBuffer(found_path);
  if (!buffer_eo) {
    this->logMessage(
        expression.getLlvmSourceStart(), rq::LogType::NOTE,
        llvm::Twine("error: failed to load import file buffer\n\treason:") +
            ec.message(),
        {expression.getLlvmSourceRange()}, {});
    return nullptr;
  }
  llvm::StringRef final_path = this->getTopStaticFrame().saveString(found_path);
  rq::Module &import_module =
      this->getTopStaticFrame().allocateValue<rq::Module>(
          rq::ModuleKind::IMPORT, final_path, std::move(buffer_eo.get()));
  this->_module_map.insert(std::pair<llvm::StringRef, rq::Module *>(
      import_module.getPath(), &import_module));
  return &import_module;
}

bool Context::initializeLlvm() {
  this->_llvm_context_uptr = std::make_unique<llvm::LLVMContext>();
  this->_llvm_module_uptr = std::make_unique<llvm::Module>(
      this->getSourceModule().getPath(), this->getLlvmContext());
  this->_llvm_ir_builder_uptr =
      std::make_unique<llvm::IRBuilder<>>(this->getLlvmContext());
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();
  std::string target_triple = llvm::sys::getDefaultTargetTriple();
  std::string error;
  const llvm::Target *target_ptr =
      llvm::TargetRegistry::lookupTarget(target_triple, error);
  if (target_ptr == nullptr) {
    this->logMessage(
        llvm::Twine("error: failed to find llvm target.\n\ttriple: ") +
        llvm::Twine(target_triple.c_str()) + llvm::Twine("\n\terror: ") +
        llvm::Twine(error.c_str()));
    return false;
  }
  llvm::TargetOptions options;
  this->_llvm_target_machine_ptr =
      rq::dereferencePtr(target_ptr)
          .createTargetMachine(target_triple, "generic", "", options,
                               llvm::Reloc::PIC_);
  return true;
}

bool Context::run() {
  if (!this->loadSourceModule()) {
    return false;
  }
  if (!this->validateSourceText(this->getSourceModule())) {
    return false;
  }
  {
    std::vector<rq::Token> tokens = {};
    if (!this->tokenizeSourceText(this->getSourceModule(), tokens)) {
      return false;
    }
    if (rq::getEmitMode() == rq::EMIT_TOKENS) {
      if (!this->emitTokens(rq::getOutputFilePath(), tokens)) {
        return false;
      }
      return true;
    }
    this->initializeKeywordMap();
    if (!this->parseRequite(this->getSourceModule(), tokens)) {
      return false;
    }
  }
  if (rq::getEmitMode() == rq::EMIT_PARSED) {
    if (!this->emitRequite(rq::getOutputFilePath(),
                           this->getSourceModule().getExpression())) {
      return false;
    }
    return true;
  }
  if (!this->situateAst(this->getSourceModule())) {
    return false;
  }
  if (rq::getEmitMode() == rq::EMIT_SITUATED) {
    if (!this->emitRequite(rq::getOutputFilePath(),
                           this->getSourceModule().getExpression())) {
      return false;
    }
    return true;
  }
  if (!this->initializeLlvm()) {
    return false;
  }
  if (!this->tabulateModule(this->getSourceModule())) {
    return false;
  }
  // this->propogateExportedImports();
  // this->diffuseUses();
  if (rq::getEmitMode() == rq::EMIT_SYMBOLS) {
    // if (!this->writeUserSymbols(output_path)) {
    //   return false;
    // }
    return true;
  }
  // if (!this->implementAll()) {
  //   return false;
  // }
  // if (!this->buildIr()) {
  //   return false;
  // }
  if (rq::getEmitMode() == rq::EMIT_IR) {
    if (!this->emitLlvmIr(rq::getOutputFilePath())) {
      return false;
    }
    return true;
  }
  if (rq::getEmitMode() == rq::EMIT_ASSEMBLY) {
    if (!this->emitAssembly(rq::getOutputFilePath())) {
      return false;
    }
    return true;
  }
  if (rq::getEmitMode() == rq::EMIT_OBJECT) {
    if (!this->emitObject(rq::getOutputFilePath())) {
      return false;
    }
    return true;
  }
  return true;
}

bool Context::parseRequite(rq::Module &module,
                           const std::vector<rq::Token> &tokens) {
  rq::RequiteParser parser(*this, tokens);
  rq::Expression *root_ptr = parser.parseExpressions();
  module.setExpression(root_ptr);
  return parser.getIsOk();
}

bool Context::situateAst(rq::Module &module) {
  rq::Situator situator(*this, this->getTopStaticFrame());
  situator.situateRoot(module);
  return situator.getIsOk();
}

bool Context::tabulateModule(rq::Module &module) {
  rq::Tabulator tabulator(*this, module);
  tabulator.tabulateModule();
  return tabulator.getIsOk();
}

bool Context::emitTokens(llvm::StringRef path,
                         llvm::ArrayRef<rq::Token> tokens) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine("error: failed to open output file for writing\n\tpath: ") +
        path + llvm::Twine("\n\treason: ") + llvm::Twine(ec.message()) + "\n");
    return false;
  }
  for (const rq::Token &token : tokens) {
    rq::SourceLocation location =
        this->getSourceLocation(token.getLlvmSourceStart());
    llvm::StringRef text = token.getSourceText();
    fout << token.getName() << ",";
    fout << location.line << ",";
    fout << location.column << ",";
    fout << token.getSourceTextLength() << ",";
    fout << "\"";
    for (const char c : text) {
      fout << rq::getCsvQuotedValueText(c);
    }
    fout << "\",\n";
  }
  fout.close();
  return true;
}

static void emitIndent(llvm::raw_fd_ostream &fout, unsigned indent) {
  for (unsigned i = 0; i < indent; i++) {
    fout << "  ";
  }
}

static void emitRequiteBranch(rq::Context &context, llvm::raw_fd_ostream &fout,
                              const rq::Expression &trunk, unsigned indent) {
  if (!rq::getNoComment()) {
    rq::emitIndent(fout, indent);
    fout << "// ";
    if (trunk.getSourceTextLength() == 0) {
      rq::SourceLocation location =
          context.getSourceLocation(trunk.getLlvmSourceStart());
      fout << location.file << ":" << location.line << ":" << location.column;
    } else {
      rq::SourceRange range = context.getSourceRange(trunk);
      fout << range.start.file << ":" << range.start.line << ":"
           << range.start.column << "-";
      if (range.start.file != range.end.file) {
        fout << range.end.file << ":";
      }
      fout << range.end.line << ":" << range.end.column;
    }
    if (trunk.getIsInserted()) {
      fout << " (inserted)";
    }
  }
  rq::emitIndent(fout, indent);
  if (trunk.getIsLiteral()) {
    if (trunk.getHasUnquotedLeft()) {
      fout << "\"";
    }
    fout << trunk.getSourceText();
    if (trunk.getHasUnquotedRight()) {
      fout << "\"";
    }
    if (!rq::getNoComment()) {
      fout << " // " << trunk.getName();
    }
    return;
  }
  fout << "[" << trunk.getName();
  if (trunk.getHasBranch()) {
    fout << '\n';
    for (const rq::Expression &branch : trunk.getBranchSubrange()) {
      rq::emitRequiteBranch(context, fout, branch, indent + 1);
      if (trunk.getHasStatementBranches()) {
        if (branch.getIsBold()) {
          fout << ",\n";
        } else if (branch.getIsChainLink()) {
          fout << "\n";
        } else {
          fout << ";\n";
        }
      } else if (trunk.getHasNonStatementBranches())  {
        fout << ",\n";
      } else {
        RQ_UNREACHABLE();
      }
    }
    rq::emitIndent(fout, indent);
  }
  fout << "]";
}

bool Context::emitRequite(llvm::StringRef path, const rq::Expression &trunk) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine("error: failed to open output file for writing\n\tpath: ") +
        path + llvm::Twine("\n\treason: ") + llvm::Twine(ec.message()) + "\n");
    return false;
  }
  for (const rq::Expression &branch : trunk.getHorizontalSubrange()) {
    rq::emitRequiteBranch(*this, fout, branch, 0);
    if (branch.getIsBold()) {
      fout << ",\n";
    } else if (branch.getIsChainLink()) {
      fout << "\n";
    } else {
      fout << ";\n";
    }
  }
  fout.close();
  return true;
}

bool Context::emitLlvmIr(llvm::StringRef path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        path + llvm::Twine("\n\treason: ") + llvm::Twine(ec.message()) + "\n");
    return false;
  }
  this->getLlvmModule().print(fout, nullptr);
  fout.close();
  return true;
}

bool Context::emitAssembly(llvm::StringRef path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        llvm::Twine(path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()) + "\n");
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::AssemblyFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logMessage(
        llvm::Twine("error: failed to add passes to emit file\n\tpath: ") +
        path);
    return false;
  }
  pass.run(this->getLlvmModule());
  fout.flush();
  return true;
}

bool Context::emitObject(llvm::StringRef path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        path + llvm::Twine("\n\treason: ") + llvm::Twine(ec.message()) + "\n");
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::ObjectFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logMessage(
        llvm::Twine("error: failed to add passes to emit file\n\tpath: ") +
        path + "\n");
    return false;
  }
  pass.run(this->getLlvmModule());
  fout.flush();
  return true;
}

void Context::logErrorFoundErrorToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine("found ") + rq::getDescription(token.getKind()),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine("found unexpected ") +
                       rq::getDescription(token.getKind()),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorMustNotHaveParameterMarks(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine(rq::getName(expression.getKeyword())) +
                       " must not have parameter marks",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedParameterMark(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "invalid parameter mark", {token.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedCommaSeparator(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceEnd(), rq::LogType::ERROR,
                   "expected comma separator after expression",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSeparatorOrRightBracket(const rq::Token &token) {
  this->logMessage(
      token.getLlvmSourceStart(), rq::LogType::ERROR,
      "expected separator or right bracket after statement or expression",
      {token.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSemicolonSeparator(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceEnd(), rq::LogType::ERROR,
                   "expected semicolon separator after statement",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNotSecondOrSubsequentIfChunkExpression(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBefore(), rq::LogType::ERROR,
                   "expected semicolon before expression because can not be "
                   "second or subsequent branch of if chunk",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorMustHaveParameterMarks(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine(rq::getName(expression.getKeyword())) +
                       " must have parameter marks",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorInvalidBranchSituation(
    rq::Expression &branch, rq::Situation situation,
    rq::Situation branch_situation, rq::Keyword outer_keyword,
    rq::Keyword branch_keyword, unsigned branch_i, llvm::Twine log_context) {
  this->logMessage(branch.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine(rq::getDescription(branch_situation)) +
                       " expression expected for " + log_context + " of " +
                       rq::getDescription(situation) + " " +
                       rq::getName(outer_keyword) + " but found " +
                       rq::getName(branch_keyword) + " at index " +
                       llvm::Twine(branch_i) + ".\n",
                   {branch.getLlvmSourceRange()}, {});
}

} // namespace rq