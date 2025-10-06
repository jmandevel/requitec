#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/context.hpp>
#include <rq/module.hpp>
#include <rq/options.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
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

rq::Expression &Context::acquireExpression() {
  if (this->_unused_expression_ptrs.empty()) {
    rq::Expression &new_expression = this->allocateValue<rq::Expression>();
    return new_expression;
  }
  rq::Expression &unused_expression =
      rq::dereferencePtr(this->_unused_expression_ptrs.back());
  this->_unused_expression_ptrs.pop_back();
  unused_expression.clear();
  return unused_expression;
}

rq::Expression &Context::copyExpression(rq::Expression &expression) {
  rq::Expression &new_expression = rq::dereferencePtr(new rq::Expression());
  if (expression.getHasBranch()) {
    new_expression.setBranch(this->copyExpression(expression.getBranch()));
  }
  if (expression.getHasNext()) {
    new_expression.setNext(this->copyExpression(expression.getNext()));
  }
  new_expression._keyword = expression._keyword;
  new_expression._source_text_ptr = expression._source_text_ptr;
  new_expression._source_text_length = expression._source_text_length;
  return new_expression;
}

void Context::replaceWithRecursiveCopy(rq::Expression &initial,
                                       rq::Expression &replacement) {
  if (initial.getHasBranch()) {
    rq::Expression &branch = initial.popBranch();
    this->discardExpression(branch);
  }
  if (initial.getHasNext()) {
    rq::Expression &next = initial.popNext();
    this->discardExpression(next);
  }
  if (initial.getHasBranch()) {
    rq::Expression &branch = replacement.getBranch();
    initial.setBranch(this->copyExpression(branch));
  }
  if (replacement.getHasNext()) {
    rq::Expression &next = replacement.getNext();
    initial.setNext(this->copyExpression(next));
  }
  initial.changeKeyword(replacement.getKeyword());
  replacement.setSource(replacement);
}

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

bool Context::loadFileBuffer(rq::Module &module, llvm::StringRef path) {
  RQ_ASSERT(!module.getHasLlvmBuffer(), "module already has file buffer");
  RQ_ASSERT(!path.empty(), "path is empty");
  module._path = path;
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer_eo =
      llvm::MemoryBuffer::getFile(path, true, true, false, std::nullopt);
  if (!buffer_eo) {
    this->logMessage(
        llvm::Twine("error: failed to create read buffer for file\n\tfile: ") +
        path + llvm::Twine("\n\treason: ") + buffer_eo.getError().message() +
        "\n");
    return false;
  }
  std::unique_ptr<llvm::MemoryBuffer> &buffer = buffer_eo.get();
  module._llvm_buffer_ref = buffer->getMemBufferRef();
  std::ignore = this->_llvm_source_mgr.AddNewSourceBuffer(std::move(buffer),
                                                          llvm::SMLoc());
  return true;
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
  rq::Module &source_module = this->getSourceModule();
  llvm::SmallString<128> input_path = rq::getInputFilePath();
  llvm::StringRef output_path = rq::getOutputFilePath();
  std::error_code ec = llvm::sys::fs::make_absolute(input_path);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "failed to determine absolute input file path\n\treason: ") +
        ec.message());
    return false;
  }
  ec = llvm::sys::fs::real_path(input_path, input_path, false);
  if (ec) {
    this->logMessage(
        llvm::Twine("failed to determine real input file path\n\treason: ") +
        ec.message() + "\n");
    return false;
  }
  if (!this->loadFileBuffer(source_module, input_path)) {
    return false;
  }
  this->_module_map.insert(
      std::pair<llvm::StringRef, rq::Module *>(input_path, &source_module));
  if (!this->validateSourceText(source_module)) {
    return false;
  }
  std::vector<rq::Token> tokens = {};
  if (!this->tokenizeSourceText(source_module, tokens)) {
    return false;
  }
  if (rq::getEmitMode() == rq::EMIT_TOKENS) {
    if (!this->emitTokens(output_path, tokens)) {
      return false;
    }
    return true;
  }
  this->initializeKeywordMap();
  if (!this->parseNormativeRequite(source_module, tokens)) {
    return false;
  }
  if (rq::getEmitMode() == rq::EMIT_PARSED) {
    if (!this->emitSymbolicRequite(output_path,
                                   source_module.getExpression())) {
      return false;
    }
    return true;
  }
  if (!this->situateAst(source_module)) {
    return false;
  }
  if (rq::getEmitMode() == rq::EMIT_SITUATED) {
    if (!this->emitSymbolicRequite(output_path,
                                   source_module.getExpression())) {
      return false;
    }
    return true;
  }
  if (!this->initializeLlvm()) {
    return false;
  }
  // if (!this->tabulateModule(source_module)) {
  //   return false;
  // }
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
  // if (!this->checkEntryPointCount()) {
  //   return false;
  // }
  // if (!this->buildIr()) {
  //   return false;
  // }
  if (rq::getEmitMode() == rq::EMIT_IR) {
    if (!this->emitLlvmIr(output_path)) {
      return false;
    }
    return true;
  }
  if (rq::getEmitMode() == rq::EMIT_ASSEMBLY) {
    if (!this->emitAssembly(output_path)) {
      return false;
    }
    return true;
  }
  if (rq::getEmitMode() == rq::EMIT_OBJECT) {
    if (!this->emitObject(output_path)) {
      return false;
    }
    return true;
  }
  return true;
}

bool Context::parseNormativeRequite(rq::Module &module,
                                    std::vector<rq::Token> &tokens) {
  rq::NormativeParser parser(*this, tokens);
  rq::Expression &trunk = parser.parseExpressions();
  module.setExpression(trunk);
  return parser.getIsOk();
}

bool Context::parseSymbolicRequite(rq::Module &module) {
  rq::SymbolicParser parser(*this, module.getSourceText());
  rq::Expression &trunk = parser.parseExpressions();
  module.setExpression(trunk);
  return parser.getIsOk();
}

bool Context::situateAst(rq::Module &module) {
  rq::Situator situator(*this);
  rq::Expression &trunk = module.getExpression();
  situator.situateTrunk<rq::Situation::TOP_STATEMENT>(trunk);
  return situator.getIsOk();
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
    llvm::StringRef text = token.getSourceText();
    fout << token.getName() << ",";
    fout << token.getLine() << ",";
    fout << token.getColumn() << ",";
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

static void emitSymbolicRequiteBranch(rq::Context &context,
                                      llvm::raw_fd_ostream &fout,
                                      const rq::Expression &trunk,
                                      unsigned indent) {
  if (!rq::getNoComment()) {
    rq::emitIndent(fout, indent);
    fout << "// ";
    if (trunk.getIsInserted()) {
      rq::SourceLocation location =
          context.getSourceLocation(trunk.getLlvmSourceStart());
      fout << location.file << ":" << location.line << ":" << location.column
           << " (inserted)";
    } else {
      rq::SourceRange range = context.getSourceRange(trunk);
      fout << range.start.file << ":" << range.start.line << ":"
           << range.start.column << "-";
      if (range.start.file != range.end.file) {
        fout << range.end.file << ":";
      }
      fout << range.end.line << ":" << range.end.column;
    }
    fout << '\n';
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
    fout << '\n';
    return;
  }
  fout << "[" << trunk.getName();
  if (trunk.getHasBranch()) {
    fout << '\n';
    for (const rq::Expression &branch : trunk.getBranchSubrange()) {
      rq::emitSymbolicRequiteBranch(context, fout, branch, indent + 1);
    }
    rq::emitIndent(fout, indent);
  }
  fout << "]\n";
}

bool Context::emitSymbolicRequite(llvm::StringRef path,
                                  const rq::Expression &trunk) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine("error: failed to open output file for writing\n\tpath: ") +
        path + llvm::Twine("\n\treason: ") + llvm::Twine(ec.message()) + "\n");
    return false;
  }
  for (const rq::Expression &branch : trunk.getHorizontalSubrange()) {
    rq::emitSymbolicRequiteBranch(*this, fout, branch, 0);
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
                   llvm::Twine("found ") + rq::getDescription(token.getType()),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   llvm::Twine("found unexpected ") +
                       rq::getDescription(token.getType()),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnterminatedExpression(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceStart(), rq::LogType::ERROR,
                   "unterminated expression", {expression.getLlvmSourceRange()},
                   {});
}

void Context::logErrorUnterminatedStatementAttribute(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "unterminated statement attribute",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnterminatedTypeAttribute(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "unterminated statement attribute",
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

void Context::logErrorExpectedCommaSeperator(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "expected comma seperator", {token.getLlvmSourceRange()},
                   {});
}

void Context::logErrorExpectedSeperatorOrRightBracket(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "expected seperator or right bracket",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSemicolonSeperator(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "expected semicolon seperator", {token.getLlvmSourceRange()},
                   {});
}

void Context::logErrorExpectedSemicolonSeperatorAtEndOfFile(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceStart(), rq::LogType::ERROR,
                   "expected semicolon seperator",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSeperator(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceStart(), rq::LogType::ERROR,
                   "expected seperator", {token.getLlvmSourceRange()}, {});
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