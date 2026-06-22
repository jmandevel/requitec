#include <rq/build.hpp>
#include <rq/codeunits.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/evaluate.hpp>
#include <rq/json.hpp>
#include <rq/literals.hpp>
#include <rq/options.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/tokenize.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>

#include <system_error>
#include <utility>

namespace rq {

bool Context::validateSourceText(const rq::ModuleFactory &factory) {
  bool is_ok = true;
  unsigned continue_bytes = 0;
  llvm::SMLoc extended_char_start;
  for (const char &c : factory.getBuffer()) {
    if (!rq::getIsValid(c)) {
      this->logErrorInvalidUtf8Codeunit(llvm::SMLoc::getFromPointer(&c), c);
      is_ok = false;
    }
    if (rq::getIsExtended(c)) {
      const unsigned new_continue_bytes = rq::getExtendedStartCount(c);
      if (continue_bytes != 0) {
        if (new_continue_bytes != 0) {
          llvm::SMLoc cur_location = llvm::SMLoc::getFromPointer(&c);
          this->logErrorInvalidUtf8Continuation(extended_char_start,
                                                cur_location, c);
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

bool Context::tokenizeSourceText(rq::ModuleFactory &factory) {
  rq::Tokenizer tokenizer(*this, factory.getBuffer(), factory.getTokens());
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
  const unsigned buffer_i =
      this->_llvm_source_mgr.FindBufferContainingLoc(llvm_location);
  const auto [line, column] =
      this->_llvm_source_mgr.getLineAndColumn(llvm_location, buffer_i);
  rq::SourceLocation source_location = {};
  source_location.file =
      this->_llvm_source_mgr.getMemoryBuffer(buffer_i)->getBufferIdentifier();
  source_location.line = line;
  source_location.column = column;
  return source_location;
}

rq::Keyword Context::getKeyword(llvm::Twine name) {
  llvm::SmallString<64> buffer;
  auto it = this->_keyword_map.find(name.toStringRef(buffer));
  if (it == this->_keyword_map.end()) {
    return rq::Keyword::NONE;
  }
  return it->getValue();
}

rq::SourceRange Context::getSourceRange(const rq::Expression &expression) {
  rq::SourceRange source_range = {};
  source_range.start = this->getSourceLocation(expression.getLlvmSourceBegin());
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
    this->logErrorSourceFileNoRqExtension(input_path);
    return false;
  }
  std::error_code ec = this->canonicalizePath(input_path);
  if (ec) {
    this->logErrorFailedToCanonicalizePath(input_path, ec);
    return false;
  }
  llvm::ErrorOr<llvm::MemoryBufferRef> buffer_eo =
      this->loadRequiteFileBuffer(input_path);
  if (!buffer_eo) {
    this->logErrorFailedToLoadSourceFileBuffer(input_path,
                                               buffer_eo.getError());
    return false;
  }
  llvm::StringRef final_path = this->saveString(input_path);
  rq::ModuleFactory factory(rq::ModuleKind::SOURCE, final_path,
                            buffer_eo.get().getBuffer());
  if (!this->validateSourceText(factory)) {
    return false;
  }
  const bool tokens_ok = this->tokenizeSourceText(factory);
  if (rq::getEmitMode() == rq::EMIT_TOKENS) {
    if (!this->emitTokens(rq::getOutputFilePath(), factory.getTokens())) {
      return false;
    }
    return tokens_ok;
  }
  if (!tokens_ok) {
    return false;
  }
  this->initializeKeywordMap();
  const bool parsed_ok = this->parseRequite(factory);
  if (rq::getEmitMode() == rq::EMIT_PARSED) {
    if (!this->emitRequite(
            rq::getOutputFilePath(),
            llvm::cast<rq::Expression>(factory.getExpressionPtr()))) {
      return false;
    }
    return parsed_ok;
  }
  if (!parsed_ok) {
    return false;
  }
  const bool situated_ok = this->situateModule(factory);
  if (rq::getEmitMode() == rq::EMIT_SITUATED) {
    if (!this->emitRequite(
            rq::getOutputFilePath(),
            llvm::cast<rq::Expression>(factory.getExpressionPtr()))) {
      return false;
    }
    return situated_ok;
  }
  if (!situated_ok) {
    return false;
  }
  rq::Module &source_module = this->acquireModule(std::move(factory));
  this->_source_module_ptr = &source_module;
  return true;
}

rq::Module *Context::loadImportModule(const rq::Expression &expression,
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
    this->logErrorImportFileNotFound(expression, import_string);
    return nullptr;
  }
  std::error_code ec = this->canonicalizePath(found_path);
  if (ec) {
    this->logErrorFailedToCanonicializeImportPath(expression, ec);
    return nullptr;
  }
  auto found_it = this->_module_map.find(found_path);
  if (found_it != this->_module_map.end()) {
    return found_it->second;
  }
  llvm::ErrorOr<llvm::MemoryBufferRef> buffer_eo =
      this->loadRequiteFileBuffer(found_path);
  if (!buffer_eo) {
    this->logErrorFailedToLoadImportFileBuffer(expression, ec);
    return nullptr;
  }
  llvm::StringRef final_path = this->saveString(found_path);
  rq::ModuleFactory factory(rq::ModuleKind::IMPORT, final_path,
                            buffer_eo.get().getBuffer());
  if (!this->validateSourceText(factory)) {
    return nullptr;
  }
  if (!this->tokenizeSourceText(factory)) {
    return nullptr;
  }
  if (!this->parseRequite(factory)) {
    return nullptr;
  }
  if (!this->situateModule(factory)) {
    return nullptr;
  }
  rq::Module &import_module = this->acquireModule(std::move(factory));
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
  std::string triple_text = llvm::sys::getDefaultTargetTriple();
  std::string error;
  llvm::Triple triple = llvm::Triple(triple_text);
  const llvm::Target *target_ptr =
      llvm::TargetRegistry::lookupTarget(triple, error);
  if (target_ptr == nullptr) {
    this->logErrorFailedToFindLlvmTarget(triple_text, error);
    return false;
  }
  llvm::TargetOptions options;
  this->_llvm_target_machine_ptr =
      rq::dereferencePtr(target_ptr)
          .createTargetMachine(triple, "generic", "", options,
                               llvm::Reloc::PIC_);
  return true;
}

bool Context::run() {
  if (!this->loadSourceModule()) {
    return false;
  }
  if (!this->getHasSourceModule()) {
    return true;
  }
  if (!this->initializeLlvm()) {
    return false;
  }
  if (!this->evaluateSourceModule()) {
    return false;
  }
  if (rq::getEmitMode() == rq::EMIT_SYMBOLS) {
    if (!this->emitSymbol(rq::getOutputFilePath(), this->getTop())) {
      return false;
    }
    return true;
  }
  if (!this->buildLlvmIr()) {
    return false;
  }
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

bool Context::parseRequite(rq::ModuleFactory &factory) {
  rq::RequiteParser parser(*this, factory.getTokens());
  rq::Expression *root_ptr = parser.parseExpressions();
  factory.setOrChangeExpression(root_ptr);
  return parser.getIsOk();
}

bool Context::situateModule(rq::ModuleFactory &factory) {
  rq::Situator situator(*this);
  const bool is_ok = situator.situateModule(factory);
  return is_ok;
}

bool Context::evaluateSourceModule() {
  rq::Evaluator evaluator(*this);
  evaluator.evaluateSourceModule();
  return evaluator.getIsOk();
}

bool Context::buildLlvmIr() {
  rq::LlvmIrBuilder builder(*this);
  builder.buildLlvmIr();
  return builder.getIsOk();
}

bool Context::emitTokens(llvm::StringRef path,
                         llvm::ArrayRef<rq::Token> tokens) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logErrorFailedToOpenOutputFile(path, ec);
    return false;
  }
  for (const rq::Token &token : tokens) {
    rq::SourceLocation location =
        this->getSourceLocation(token.getLlvmSourceBegin());
    llvm::StringRef text = token.getSourceText();
    fout << token.getName() << ",";
    fout << location.line << ",";
    fout << location.column << ",";
    fout << token.getSourceTextLength() << ",";
    fout << "\"";
    for (const char c : text) {
      fout << rq::getCsvQuotedValueText(c);
    }
    if (&token != &tokens.back()) {
      fout << "\",\n";
    } else {
      fout << "\"";
    }
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
                              const rq::Expression &top, unsigned indent) {
  if (!rq::getNoComment()) {
    rq::emitIndent(fout, indent);
    fout << "//";
    if (top.getIsInternal()) {
      fout << " " << top.getName();
    }
    if (!top.getHasSourceText()) {
      fout << " (no position)";
    } else if (top.getSourceTextLength() == 0) {
      rq::SourceLocation location =
          context.getSourceLocation(top.getLlvmSourceBegin());
      fout << " " << location.file << ":" << location.line << ":"
           << location.column;
    } else {
      rq::SourceRange range = context.getSourceRange(top);
      fout << " " << range.start.file << ":" << range.start.line << ":"
           << range.start.column << "-";
      if (range.start.file != range.end.file) {
        fout << range.end.file << ":";
      }
      fout << range.end.line << ":" << range.end.column;
    }
    if (top.getIsInserted()) {
      fout << " (inserted)";
    }
    if (top.getHasSituatorError()) {
      fout << " (situator error)";
    }
    fout << "\n";
  }
  rq::emitIndent(fout, indent);
  if (top.getIsLiteral()) {
    if (top.getHasUnquotedLeft()) {
      fout << "\"";
    }
    fout << top.getSourceText();
    if (top.getHasUnquotedRight()) {
      fout << "\"";
    }
    return;
  }
  fout << "[" << top.getName();
  if (top.getHasBranch()) {
    fout << '\n';
    for (const rq::Expression &branch : top.getBranchSubrange()) {
      rq::emitRequiteBranch(context, fout, branch, indent + 1);
      if (branch.getIsStatement()) {
        fout << ";\n";
      } else if (branch.getIsChainLink()) {
        fout << "\n";
      } else {
        if (branch.getHasNext()) {
          fout << ",\n";
        } else {
          fout << "\n";
        }
      }
    }
    rq::emitIndent(fout, indent);
  }
  fout << "]";
}

bool Context::emitRequite(llvm::StringRef path, const rq::Expression *top_ptr) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logErrorFailedToOpenOutputFile(path, ec);
    return false;
  }
  if (top_ptr == nullptr) {
    fout.close();
    return true;
  }
  const rq::Expression &top = rq::dereferencePtr(top_ptr);
  for (const rq::Expression &branch : top.getInclusiveNextSubrange()) {
    rq::emitRequiteBranch(*this, fout, branch, 0);
    if (branch.getIsStatement()) {
      fout << ";\n";
    } else if (branch.getIsChainLink()) {
      fout << "\n";
    } else {
      if (branch.getHasNext()) {
        fout << ",\n";
      } else {
        fout << "\n";
      }
    }
  }
  fout.close();
  return true;
}

static void emitSymbol(rq::Context &context, rq::JsonEmitter &json,
                       const rq::Symbol &symbol);

// static void emitLocation(rq::Context &context, rq::JsonEmitter &json,
//                          const rq::Expression &expression) {
//   if (!expression.getHasSourceText()) {
//     json.emitNull("location");
//   } else if (expression.getSourceTextLength() == 0) {
//     rq::SourceLocation location =
//         context.getSourceLocation(expression.getLlvmSourceBegin());
//     json.beginObject("location");
//     json.emitString("file", location.file);
//     json.emit("line", location.line);
//     json.emit("column", location.column);
//     json.endObject();
//   } else {
//     rq::SourceRange range = context.getSourceRange(expression);
//     json.beginObject("location");
//     json.beginObject("start");
//     json.emitString("file", range.start.file);
//     json.emit("line", range.start.line);
//     json.emit("column", range.start.column);
//     json.endObject();
//     json.beginObject("end");
//     json.emitString("file", range.end.file);
//     json.emit("line", range.end.line);
//     json.emit("column", range.end.column);
//     json.endObject();
//     json.endObject();
//   }
// }

// static void emitAttributes(rq::JsonEmitter &json, const rq::Symbol &symbol) {
//   json.beginArray("attributes");
//   using EA = rq::ExpressionAttribute;
//   using EF = rq::ExpressionFlags;
//   EF flags = symbol.getDerivedExpressionFlags();
//   for (unsigned attribute_i = static_cast<unsigned>(EA::NONE) + 1;
//        attribute_i < static_cast<unsigned>(EA::LAST); attribute_i++) {
//     EA attribute = static_cast<EA>(attribute_i);
//     EF attribute_flags = rq::getFlags(attribute);
//     if (rq::getHasAll(flags, attribute_flags)) {
//       json.emitString(rq::getName(attribute));
//     }
//   }
//   json.endArray();
// }

// template <typename SymbolType>
// static void emitModuleMemberSymbol(rq::Context &context, rq::JsonEmitter
// &json,
//                                    const SymbolType &symbol) {
//   json.emitString("module", symbol.getContainingModule().getPath());
//   rq::emitAttributes(json, symbol);
//   rq::emitLocation(context, json, symbol.getExpression());
// }

// static void emiSymboltTable(rq::Context &context, rq::JsonEmitter &json,
//                             const rq::SymbolTable &table) {
//   json.beginArray("named");
//   for (const auto &[name, list] : table.getNamedMemberMap()) {
//     json.beginObject();
//     json.emitString("name", name);
//     json.beginArray("symbols");
//     for (const rq::Symbol &symbol : list) {
//       rq::emitSymbol(context, json, symbol);
//     }
//     json.endArray();
//     json.endObject();
//   }
//   json.endArray();
//   json.beginArray("unnamed");
//   for (const rq::Symbol &symbol : table.getUnamedMemberList()) {
//     rq::emitSymbol(context, json, symbol);
//   }
//   json.endArray();
// }

static void emitSymbol(rq::Context &context, rq::JsonEmitter &json,
                       const rq::Symbol &symbol) {
  json.beginObject();
  json.emitString("kind", rq::getName(symbol.getKind()));
  std::ignore = context;
  RQ_TODO_IMPLEMENTATION();
  json.endObject();
}

bool Context::emitSymbol(llvm::StringRef path, const rq::Symbol &symbol) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logErrorFailedToOpenIntermediateFile(path, ec);
    return false;
  }
  rq::JsonEmitter json(fout);
  rq::emitSymbol(*this, json, symbol);
  fout << "\n";
  fout.close();
  return true;
}

bool Context::emitLlvmIr(llvm::StringRef path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logErrorFailedToOpenIntermediateFile(path, ec);
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
    this->logErrorFailedToOpenIntermediateFile(path, ec);
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::AssemblyFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logErrorFailedToAddPassesToEmitFile(path);
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
    this->logErrorFailedToOpenIntermediateFile(path, ec);
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::ObjectFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logErrorFailedToAddPassesToEmitFile(path);
    return false;
  }
  pass.run(this->getLlvmModule());
  fout.flush();
  return true;
}

[[nodiscard]] llvm::Type *Context::getLlvmTypePtr(rq::Symbol &symbol) {
  using S = rq::SymbolKind;
  switch (symbol.getKind()) {
  case S::SIGNED_INTEGER_TYPE: {
    unsigned best_size =
        this->getLlvmModule().getDataLayout().getPointerSizeInBits();
    return this->getLlvmIrBuilder().getIntNTy(best_size);
  }
  case S::SIGNATURE: {
    rq::Signature &sig = llvm::cast<rq::Signature>(symbol);
    if (sig.getFirstParameterPtr() != nullptr) {
      RQ_TODO_IMPLEMENTATION();
    }
    llvm::Type *llvm_result_ty_ptr =
        this->getLlvmTypePtr(sig.getReturnType().getSymbol());
    if (llvm_result_ty_ptr == nullptr) {
      return nullptr;
    }
    return llvm::FunctionType::get(llvm_result_ty_ptr, false);
  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] unsigned Context::getDepth(rq::Symbol &symbol) {
  llvm::Type *llvm_ty_ptr = this->getLlvmTypePtr(symbol);
  if (llvm_ty_ptr == nullptr) {
    return 0;
  }
  llvm::Type &llvm_ty = rq::dereferencePtr(llvm_ty_ptr);
  return llvm_ty.getIntegerBitWidth();
}

void Context::logErrorInvalidUtf8Codeunit(llvm::SMLoc location, char c) {
  llvm::Twine message =
      llvm::Twine("invalid utf-8 codeunit \"") + rq::getUtf8Name(c) + "\"";
  this->logMessage(location, rq::LogType::ERROR, message, {}, {});
}

void Context::logErrorInvalidUtf8Continuation(llvm::SMLoc start,
                                              llvm::SMLoc cur, char c) {
  llvm::Twine message = llvm::Twine("invalid utf-8 continuation codeunit \"") +
                        rq::getUtf8Name(c) + "\"";
  this->logMessage(cur, rq::LogType::ERROR, message,
                   {llvm::SMRange(start, cur)}, {});
}

void Context::logErrorSourceFileNoRqExtension(llvm::StringRef path) {
  this->logMessage(
      llvm::Twine("error: source file does not have a \".rq\" extension\n") +
      llvm::Twine("\tpath: ") + path);
}

void Context::logErrorFailedToCanonicalizePath(llvm::StringRef path,
                                               const std::error_code &ec) {
  this->logMessage(
      llvm::Twine("error: failed to canonicalize source file path\n") +
      llvm::Twine("\tpath: ") + path + llvm::Twine("\n\treason: ") +
      ec.message());
}

void Context::logErrorFailedToLoadSourceFileBuffer(llvm::StringRef path,
                                                   const std::error_code &ec) {
  this->logMessage(
      llvm::Twine("error: failed to load source file buffer\n\tpath: ") + path +
      llvm::Twine("\n\treason:") + ec.message());
}

void Context::logErrorImportFileNotFound(const rq::Expression &expression,
                                         llvm::StringRef import_string) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   llvm::Twine("error: could not locate import file \"") +
                       import_string + "\" in any import directory\n",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFailedToCanonicializeImportPath(
    const rq::Expression &expression, const std::error_code &ec) {
  this->logMessage(
      expression.getLlvmSourceBegin(), rq::LogType::NOTE,
      llvm::Twine("error: failed to canonicalize import file path\n") +
          llvm::Twine("\treason:") + ec.message(),
      {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFailedToLoadImportFileBuffer(
    const rq::Expression &expression, const std::error_code &ec) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   llvm::Twine("error: failed to load import file buffer\n") +
                       llvm::Twine("\treason:") + ec.message(),
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFailedToFindLlvmTarget(llvm::StringRef target_triple,
                                             llvm::StringRef error) {
  this->logMessage(
      llvm::Twine("error: failed to find llvm target.\n\ttriple: ") +
      target_triple + llvm::Twine("\n\terror: ") + error);
}

void Context::logErrorFailedToOpenOutputFile(llvm::StringRef path,
                                             const std::error_code &ec) {
  this->logMessage(
      llvm::Twine("error: failed to open output file for writing\n") +
      llvm::Twine("\tpath: ") + path + llvm::Twine("\n\treason: ") +
      ec.message() + "\n");
}

void Context::logErrorFailedToOpenIntermediateFile(llvm::StringRef path,
                                                   const std::error_code &ec) {
  this->logMessage(
      llvm::Twine("error: failed to open intermediate file for writing\n") +
      llvm::Twine("\tpath: ") + path + llvm::Twine("\n\treason: ") +
      ec.message() + "\n");
}

void Context::logErrorFailedToAddPassesToEmitFile(llvm::StringRef path) {
  this->logMessage(
      llvm::Twine("error: failed to add passes to emit file\n\tpath: ") + path);
}

void Context::logErrorFoundErrorToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("found ") + token.getDescription(),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("found unexpected ") + token.getDescription(),
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedIdentifierLiteral(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "expected identifier literal", {token.getLlvmSourceRange()},
                   {});
}

void Context::logErrorNotKeyword(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(token.getSourceText()) + " is not keyword",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorInternalUseOnlyKeyword(const rq::Token &token,
                                             rq::Keyword keyword) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(rq::getName(keyword)) +
                       " is for internal use only",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnmatchedRightToken(const rq::Token &left_token,
                                          const rq::Token &right_token) {
  this->logMessage(right_token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(right_token.getDescription()) +
                       " does not match previous left grouping token",
                   {right_token.getLlvmSourceRange()}, {});
  this->logMessage(left_token.getLlvmSourceBegin(), rq::LogType::NOTE,
                   llvm::Twine("previous left grouping token is ") +
                       left_token.getDescription(),
                   {left_token.getLlvmSourceRange()}, {});
}

void Context::logErrorSoloRightToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(token.getDescription()) +
                       " does not follow a left grouping token",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnterminatedStringLiteral(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "unterminated string literal", {token.getLlvmSourceRange()},
                   {});
}

void Context::logErrorUnmatchedLeftToken(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(token.getDescription()) + " has no match",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorTrailerTokenMismatch(const rq::Token &trailer_token,
                                           const rq::Token &front_token,
                                           const rq::Expression &expression) {
  this->logMessage(
      trailer_token.getLlvmSourceBegin(), rq::LogType::ERROR,
      "trailer token does not match token from start of expression",
      {trailer_token.getLlvmSourceRange()}, {});
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   "for expression", {expression.getLlvmSourceRange()}, {});
  this->logMessage(front_token.getLlvmSourceBegin(), rq::LogType::NOTE,
                   "for token from start of expression",
                   {front_token.getLlvmSourceRange()}, {});
}

void Context::logErrorUnterminatedInterpolatedString(const rq::Token &token) {
  this->logMessage(token.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "found unterminated interpolated string",
                   {token.getLlvmSourceRange()}, {});
}

void Context::logErrorMustHaveParameterMark(rq::Situation situation,
                                            const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(rq::getDescription(situation)) + " " +
                       expression.getName() + " must have parameter mark",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorIsFirst(const rq::Expression &mark) {
  this->logMessage(mark.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(mark.getName()) + " is first",
                   {mark.getLlvmSourceRange()}, {});
}

void Context::logErrorIsLast(const rq::Expression &mark) {
  this->logMessage(mark.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(mark.getName()) + " is last",
                   {mark.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedCommaSeparator(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceEnd(), rq::LogType::ERROR,
                   "expected comma separator after expression",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSeparatorOrRightBracket(const rq::Token &token) {
  this->logMessage(
      token.getLlvmSourceBegin(), rq::LogType::ERROR,
      "expected separator or right bracket after statement or expression",
      {token.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedSemicolonSeparator(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceEnd(), rq::LogType::ERROR,
                   "expected semicolon separator after statement",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorExpressionShouldNeverOccur(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(expression.getName()) +
                       " expression should never occur.",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorDuplicateParameterMark(const rq::Expression &mark) {
  this->logMessage(mark.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("duplicate ") + mark.getName(),
                   {mark.getLlvmSourceRange()}, {});
}

void Context::logErrorDuplicateAttribute(const rq::Expression &attribute) {
  this->logMessage(attribute.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("duplicate ") + attribute.getName(),
                   {attribute.getLlvmSourceRange()}, {});
}

void Context::logErrorNonpositionalBeginAfterPositionalEnd(
    const rq::Expression &named_begin) {
  this->logMessage(named_begin.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "nonpositional begin after positional end",
                   {named_begin.getLlvmSourceRange()}, {});
}

void Context::logErrorNonpositionalBeginAfterLockedBegin(
    const rq::Expression &named_begin) {
  this->logMessage(named_begin.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "nonpositional begin after locked begin",
                   {named_begin.getLlvmSourceRange()}, {});
}

void Context::logErrorPositionalEndAfterLockedBegin(
    const rq::Expression &named_begin) {
  this->logMessage(named_begin.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "positional end after locked begin",
                   {named_begin.getLlvmSourceRange()}, {});
}

void Context::logErrorNotExactBranchCount(rq::Situation situation,
                                          const rq::Expression &expression,
                                          unsigned count) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(rq::getDescription(situation)) + " " +
                       expression.getName() + " must have exactly " +
                       llvm::Twine(count) + " branches",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNotAtLeastBranchCount(rq::Situation situation,
                                            const rq::Expression &expression,
                                            unsigned count) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(rq::getDescription(situation)) + " " +
                       expression.getName() + " must have at least " +
                       llvm::Twine(count) + " branches",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorTooManyBranchCount(rq::Situation situation,
                                         const rq::Expression &expression,
                                         unsigned count) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(rq::getDescription(situation)) + " " +
                       expression.getName() + " must not have more than " +
                       llvm::Twine(count) + " branches",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorInvalidBranchSituation(rq::Situation situation,
                                             const rq::Expression &branch) {
  this->logMessage(branch.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(branch.getName()) + " can not be " +
                       rq::getDescription(situation),
                   {branch.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedHeaderBranch(const rq::Expression &expresison) {
  this->logMessage(expresison.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expresison.getName() + " has no header branch",
                   {expresison.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedHeaderExpression(
    const rq::Expression &expresison) {
  this->logMessage(expresison.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expresison.getName() + " is not header",
                   {expresison.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedChainLinkExpression(
    const rq::Expression &expresison) {
  this->logMessage(expresison.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expresison.getName() + " is not chain-link",
                   {expresison.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedChainLinkExpression(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is chain-link",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorExpectedStatementOrChainLinkExpression(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not statement or chain-link",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNotDeterminateStaticValue(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not determinate static value",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorInvalidExpressionAttribute(
    const rq::Expression &unascribed, const rq::Expression &attribute) {
  this->logMessage(attribute.getLlvmSourceBegin(), rq::LogType::ERROR,
                   attribute.getName() +
                       " is is not a valid attribute for expression " +
                       unascribed.getName(),
                   {attribute.getLlvmSourceRange()}, {});
}

void Context::logErrorFailedToAscribeExpression(
    const rq::Expression &unascribed, const rq::Expression &attribute) {
  this->logMessage(attribute.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("failed to ascribe ") + attribute.getName() +
                       " to " + unascribed.getName(),
                   {attribute.getLlvmSourceRange()}, {});
}

void Context::logErrorNotSymbol(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not symbol",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNotLabel(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not label",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorLabelSubjectNotTable(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "label does not refer to symbol table",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNotInTop(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " must be in top",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorUnableToEvaluateName(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not evaluatable name",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorUnableToEvaluateUtf8Cstr(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not evaluatable utf8 cstr",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFailedToImportModule(const rq::Expression &expression,
                                           llvm::StringRef path) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("failed to import ") + path,
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFlankNotInFrame(const rq::Expression &flank_expression) {
  this->logMessage(
      flank_expression.getLlvmSourceBegin(), rq::LogType::ERROR,
      llvm::Twine(
          "flank attribute refeers to symbol table that is not in frame"),
      {flank_expression.getLlvmSourceRange()}, {});
}

void Context::logErrorFlankNotAncestor(const rq::Expression &flank_expression) {
  this->logMessage(
      flank_expression.getLlvmSourceBegin(), rq::LogType::ERROR,
      llvm::Twine("flank attribute refeers to symbol table is not ancestor"),
      {flank_expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNumeric(const rq::Expression &expression,
                              rq::NumericResultCode code) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("error parsing numeric literal: ") +
                       rq::getDescription(code),
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorNameCollision(const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "name collision", expression.getLlvmSourceRange(), {});
}

void Context::logInfoNameCollisionDeclaration(rq::Symbol &symbol) {
  const rq::Expression &decl_expression =
      rq::dereferencePtr(symbol.getDerivedExpressionPtr());
  this->logMessage(decl_expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   llvm::Twine("name collision with ") +
                       rq::getName(symbol.getKind()),
                   decl_expression.getLlvmSourceRange(), {});
}

void Context::logErrorInvalidValueSymbol(const rq::Expression &expression,
                                         rq::Symbol &symbol) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("invalid value symbol of kind ") +
                       rq::getName(symbol.getKind()),
                   expression.getLlvmSourceRange(), {});
  const rq::Expression *decl_expression_ptr = symbol.getDerivedExpressionPtr();
  if (decl_expression_ptr == nullptr) {
    return;
  }
  const rq::Expression &decl_expression =
      rq::dereferencePtr(decl_expression_ptr);
  this->logMessage(decl_expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   "referencing symbol", decl_expression.getLlvmSourceRange(),
                   {});
}

void Context::logErrorIndeterminateVariableValue(
    const rq::Expression &expression, rq::Symbol &symbol) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine("indeterminate variable value of kind ") +
                       rq::getName(symbol.getKind()),
                   expression.getLlvmSourceRange(), {});
  const rq::Expression *decl_expression_ptr = symbol.getDerivedExpressionPtr();
  if (decl_expression_ptr == nullptr) {
    return;
  }
  const rq::Expression &decl_expression =
      rq::dereferencePtr(decl_expression_ptr);
  this->logMessage(decl_expression.getLlvmSourceBegin(), rq::LogType::NOTE,
                   "referencing variable", decl_expression.getLlvmSourceRange(),
                   {});
}

void Context::logErrorUnexpectedRvalueType(const Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "unexpected rvalue type", expression.getLlvmSourceRange(),
                   {});
}

[[nodiscard]] const llvm::fltSemantics &
Context::getLlvmFltSemantics(rq::SymbolKind kind) {
  // TODO pick best float type for platform with platform float types
  using K = rq::SymbolKind;
  switch (kind) {
  case K::HALF_TYPE:
    return llvm::APFloat::IEEEhalf();
  case K::SINGLE_TYPE:
    return llvm::APFloat::IEEEsingle();
  case K::DOUBLE_TYPE:
    return llvm::APFloat::IEEEdouble();
  case K::QUADRUPLE_TYPE:
    return llvm::APFloat::IEEEquad();
  case K::BINARY16_TYPE:
    return llvm::APFloat::IEEEhalf();
  case K::BINARY32_TYPE:
    return llvm::APFloat::IEEEsingle();
  case K::BINARY64_TYPE:
    return llvm::APFloat::IEEEdouble();
  case K::BINARY128_TYPE:
    return llvm::APFloat::IEEEquad();
  case K::BFLOAT16_TYPE:
    return llvm::APFloat::BFloat();
  default:
    break;
  }
  RQ_UNREACHABLE();
}

rq::Expression &Context::acquireExpression() {
  if (this->_free_expression_ptr == nullptr) {
    rq::Expression &new_expression = this->allocateValue<rq::Expression>();
    return new_expression;
  }
  rq::Expression &unused_expression =
      rq::dereferencePtr(this->_free_expression_ptr);
  this->_free_expression_ptr =
      static_cast<rq::Expression *>(unused_expression._branch_ptr);
  unused_expression._branch_ptr = nullptr;
  return unused_expression;
}

rq::Expression &Context::copyExpression(rq::Expression &expression) {
  rq::Expression &new_expression = this->acquireExpression();
  if (expression.getHasBranch()) {
    new_expression.setBranch(this->copyExpression(expression.getBranch()));
  }
  new_expression._next_ptr_flags = expression._next_ptr_flags;
  new_expression._id = expression._id;
  new_expression._source_ptr_flags = expression._source_ptr_flags;
  new_expression._source_text_length = expression._source_text_length;
  return new_expression;
}

} // namespace rq
