#include <rq/build.hpp>
#include <rq/codeunits.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/json.hpp>
#include <rq/options.hpp>
#include <rq/parse.hpp>
#include <rq/situate.hpp>
#include <rq/tabulator.hpp>
#include <rq/tokenize.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/SmallString.h>
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

bool Context::validateSourceText(const rq::Module &module) {
  bool is_ok = true;
  unsigned continue_bytes = 0;
  llvm::SMLoc extended_char_start;
  for (const char &c : module.getSourceText()) {
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

bool Context::tokenizeSourceText(const rq::Module &module,
                                 std::vector<rq::Token> &tokens) {
  rq::Tokenizer tokenizer(*this, module.getSourceText(), tokens);
  const bool is_ok = tokenizer.tokenizeSourceText();
  return is_ok;
}

void Context::initializeKeywordMap() {
  RQ_ASSERT(this->_keyword_map.empty(), "keyword map not empty");
  for (std::underlying_type_t<rq::EntityKind> keyword_i = 0;
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
  llvm::StringMapIterator<rq::Keyword> it =
      this->_keyword_map.find(name.toStringRef(buffer));
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
  rq::Module &source_module = this->allocateAcquiredValue<rq::Module>(
      rq::ModuleKind::SOURCE, final_path, std::move(buffer_eo.get()));
  rq::assignSingleValue(this->_source_module_ptr, &source_module);
  this->_module_map.insert(std::pair<llvm::StringRef, rq::Module *>(
      input_path, &this->getSourceModule()));
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
  rq::Module &import_module = this->allocateAcquiredValue<rq::Module>(
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
    this->logErrorFailedToFindLlvmTarget(target_triple, error);
    return false;
  }
  llvm::TargetOptions options;
  this->_llvm_target_machine_ptr =
      rq::dereferencePtr(target_ptr)
          .createTargetMachine(llvm::Triple(target_triple), "generic", "",
                               options, llvm::Reloc::PIC_);
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
  if (!this->situateModule(this->getSourceModule())) {
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
  RQ_UNREACHABLE();
}

bool Context::parseRequite(rq::Module &module,
                           const std::vector<rq::Token> &tokens) {
  rq::RequiteParser parser(*this, tokens);
  rq::Expression *root_ptr = parser.parseExpressions();
  module.setExpression(root_ptr);
  return parser.getIsOk();
}

bool Context::situateModule(rq::Module &module) {
  rq::Situator situator(*this);
  const bool is_ok = situator.situateModule(module);
  return is_ok;
}

bool Context::tabulateModule(rq::Module &module) {
  rq::Tabulator tabulator(*this, module);
  tabulator.tabulateModule();
  return tabulator.getIsOk();
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
                              const rq::Expression &trunk, unsigned indent) {
  if (!rq::getNoComment()) {
    rq::emitIndent(fout, indent);
    fout << "//";
    if (trunk.getIsInternal()) {
      fout << " " << trunk.getName();
    }
    if (!trunk.getHasSourceText()) {
      fout << " (no position)";
    } else if (trunk.getSourceTextLength() == 0) {
      rq::SourceLocation location =
          context.getSourceLocation(trunk.getLlvmSourceBegin());
      fout << " " << location.file << ":" << location.line << ":"
           << location.column;
    } else {
      rq::SourceRange range = context.getSourceRange(trunk);
      fout << " " << range.start.file << ":" << range.start.line << ":"
           << range.start.column << "-";
      if (range.start.file != range.end.file) {
        fout << range.end.file << ":";
      }
      fout << range.end.line << ":" << range.end.column;
    }
    if (trunk.getIsInserted()) {
      fout << " (inserted)";
    }
    if (trunk.getHasSituatorError()) {
      fout << " (situator error)";
    }
    fout << "\n";
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
    return;
  }
  fout << "[" << trunk.getName();
  if (trunk.getHasBranch()) {
    fout << '\n';
    for (const rq::Expression &branch : trunk.getBranchSubrange()) {
      rq::emitRequiteBranch(context, fout, branch, indent + 1);
      if (trunk.getHasStatementBranches()) {
        if (branch.getIsHeader()) {
          fout << ",\n";
        } else if (branch.getIsChainLink()) {
          fout << "\n";
        } else {
          fout << ";\n";
        }
      } else if (trunk.getHasExpressionBranches()) {
        if (branch.getHasNext()) {
          fout << ",\n";
        } else {
          fout << "\n";
        }
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
    this->logErrorFailedToOpenOutputFile(path, ec);
    return false;
  }
  for (const rq::Expression &branch : trunk.getInclusiveNextSubrange()) {
    rq::emitRequiteBranch(*this, fout, branch, 0);
    if (branch.getIsHeader()) {
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

static void emitSymbol(rq::Context &context, rq::JsonEmitter &json,
                       const rq::Symbol &symbol);

static void emitLocation(rq::Context &context, rq::JsonEmitter &json,
                         const rq::Expression &expression) {
  if (!expression.getHasSourceText()) {
    json.emitNull("location");
  } else if (expression.getSourceTextLength() == 0) {
    rq::SourceLocation location =
        context.getSourceLocation(expression.getLlvmSourceBegin());
    json.beginObject("location");
    json.emitString("file", location.file);
    json.emit("line", location.line);
    json.emit("column", location.column);
    json.endObject();
  } else {
    rq::SourceRange range = context.getSourceRange(expression);
    json.beginObject("location");
    json.beginObject("start");
    json.emitString("file", range.start.file);
    json.emit("line", range.start.line);
    json.emit("column", range.start.column);
    json.endObject();
    json.beginObject("end");
    json.emitString("file", range.end.file);
    json.emit("line", range.end.line);
    json.emit("column", range.end.column);
    json.endObject();
    json.endObject();
  }
}

static void emitAttributes(rq::JsonEmitter &json,
                           const rq::InitialExpressionFlags &symbol) {
  json.beginArray("attributes");
  using EA = rq::ExpressionAttribute;
  for (unsigned attribute_i = static_cast<unsigned>(EA::NONE) + 1;
       attribute_i < static_cast<unsigned>(EA::LAST); attribute_i++) {
    EA attribute = static_cast<EA>(attribute_i);
    if (symbol.getHasAttribute(attribute)) {
      json.emitString(rq::getName(attribute));
    }
  }
  json.endArray();
}

template <typename SymbolType>
static void emitModuleMemberSymbol(rq::Context &context, rq::JsonEmitter &json,
                                   const SymbolType &symbol) {
  json.emitString("module", symbol.getContainingModule().getPath());
  rq::emitAttributes(json, symbol);
  rq::emitLocation(context, json, symbol.getExpression());
}

static void emitSymbolTable(rq::Context &context, rq::JsonEmitter &json,
                            const rq::SymbolTable &table) {
  json.beginArray("named");
  for (const auto &node : table.getNamedSymbolsRange()) {
    llvm::StringRef name = node.name;
    rq::ConstBumpPtrList<rq::Symbol> list =
        rq::ConstBumpPtrList<rq::Symbol>(node.list);
    json.beginObject();
    json.emitString("name", name);
    json.beginArray("symbols");
    for (const rq::Symbol &symbol : list) {
      rq::emitSymbol(context, json, symbol);
    }
    json.endArray();
    json.endObject();
  }
  json.endArray();
  json.beginArray("unnamed");
  for (const rq::Symbol &symbol : table.getUnamedSymbolsList()) {
    rq::emitSymbol(context, json, symbol);
  }
  json.endArray();
}

static void emitSymbol(rq::Context &context, rq::JsonEmitter &json,
                       const rq::Symbol &symbol) {
  json.beginObject();
  json.emitString("kind", rq::getName(symbol.getKind()));
  switch (symbol.getKind()) {
  case rq::EntityKind::SY_IMPORT: {
    const auto &import = llvm::cast<rq::Import>(symbol);
    rq::emitModuleMemberSymbol(context, json, import);
  } break;
  case rq::EntityKind::SY_LOCAL_VARIABLE: {
    const auto &variable = llvm::cast<rq::LocalVariable>(symbol);
    json.emitString("name", variable.getName());
    rq::emitModuleMemberSymbol(context, json, variable);
  } break;
  case rq::EntityKind::SY_GLOBAL_VARIABLE: {
    const auto &variable = llvm::cast<rq::GlobalVariable>(symbol);
    json.emitString("name", variable.getName());
    rq::emitModuleMemberSymbol(context, json, variable);
  } break;
  case rq::EntityKind::SY_NAMESPACE: {
    const auto &namespace_ = llvm::cast<rq::Namespace>(symbol);
    json.emitString("name", namespace_.getName());
  } break;
  case rq::EntityKind::SY_CLASS: {
    const auto &class_ = llvm::cast<rq::Class>(symbol);
    json.emitString("name", class_.getName());
    rq::emitModuleMemberSymbol(context, json, class_);
  } break;
  case rq::EntityKind::SY_ENUMERATION: {
    const auto &enumeration = llvm::cast<rq::Enumeration>(symbol);
    json.emitString("name", enumeration.getName());
    rq::emitModuleMemberSymbol(context, json, enumeration);
  } break;
  case rq::EntityKind::SY_ENTRY: {
    const auto &entry = llvm::cast<rq::Entry>(symbol);
    rq::emitModuleMemberSymbol(context, json, entry);
  } break;
  case rq::EntityKind::SY_FUNCTION: {
    const auto &function = llvm::cast<rq::Function>(symbol);
    json.emitString("name", function.getName());
    rq::emitModuleMemberSymbol(context, json, function);
  } break;
  case rq::EntityKind::SY_METHOD: {
    const auto &method = llvm::cast<rq::Method>(symbol);
    json.emitString("name", method.getName());
    rq::emitModuleMemberSymbol(context, json, method);
  } break;
  case rq::EntityKind::SY_RANGER: {
    const auto &ranger = llvm::cast<rq::Ranger>(symbol);
    rq::emitModuleMemberSymbol(context, json, ranger);
  } break;
  case rq::EntityKind::SY_EXTENSION_FUNCTION: {
    const auto &extension_function = llvm::cast<rq::ExtensionFunction>(symbol);
    json.emitString("name", extension_function.getName());
    rq::emitModuleMemberSymbol(context, json, extension_function);
  } break;
  case rq::EntityKind::SY_EXTENSION_METHOD: {
    const auto &extension_method = llvm::cast<rq::ExtensionMethod>(symbol);
    json.emitString("name", extension_method.getName());
    rq::emitModuleMemberSymbol(context, json, extension_method);
  } break;
  case rq::EntityKind::SY_TOP: {
    const auto &top = llvm::cast<rq::Top>(symbol);
    rq::emitSymbolTable(context, json, top);
  } break;
  default:
    RQ_TODO_IMPLEMENTATION();
  }
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

void Context::logErrorPositionalEndIsFirst(const rq::Expression &mark) {
  this->logMessage(mark.getLlvmSourceBegin(), rq::LogType::ERROR,
                   llvm::Twine(mark.getName()) + " is first",
                   {mark.getLlvmSourceRange()}, {});
}

void Context::logErrorNamedBeginIsLast(const rq::Expression &mark) {
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

void Context::logErrorNamedBeginAfterPositionalEnd(
    const rq::Expression &named_begin) {
  this->logMessage(named_begin.getLlvmSourceBegin(), rq::LogType::ERROR,
                   "named begin after positional end",
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

void Context::logErrorExpectedHeaderExpression(
    const rq::Expression &expresison) {
  this->logMessage(expresison.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expresison.getName() + " is not header",
                   {expresison.getLlvmSourceRange()}, {});
}

void Context::logErrorUnexpectedHeaderExpression(
    const rq::Expression &expresison) {
  this->logMessage(expresison.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expresison.getName() + " is header",
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

void Context::logErrorNotDeterminateStaticValue(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() + " is not determinate static value",
                   {expression.getLlvmSourceRange()}, {});
}

void Context::logErrorGlobalIndeterminateDynamicExpression(
    const rq::Expression &expression) {
  this->logMessage(expression.getLlvmSourceBegin(), rq::LogType::ERROR,
                   expression.getName() +
                       " is not global indeterminate dynamic expression",
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

void Context::logErrorLabelSubjectNotSymbolTable(
    const rq::Expression &expression) {
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

void Context::logErrorOutsideNotInFrame(
    const rq::Expression &outside_expression) {
  this->logMessage(
      outside_expression.getLlvmSourceBegin(), rq::LogType::ERROR,
      llvm::Twine(
          "outside attribute refeers to symbol table that is not in frame"),
      {outside_expression.getLlvmSourceRange()}, {});
}

void Context::logErrorOutsideNotAncestor(
    const rq::Expression &outside_expression) {
  this->logMessage(
      outside_expression.getLlvmSourceBegin(), rq::LogType::ERROR,
      llvm::Twine("outside attribute refeers to symbol table is not ancestor"),
      {outside_expression.getLlvmSourceRange()}, {});
}

rq::Expression &Context::acquireExpression() {
  if (this->acquired._first_unused_expression_ptr == nullptr) {
    rq::Expression &new_expression =
        this->allocateAcquiredValue<rq::Expression>();
    return new_expression;
  }
  rq::Expression &unused_expression =
      rq::dereferencePtr(this->acquired._first_unused_expression_ptr);
  this->acquired._first_unused_expression_ptr = unused_expression._branch_ptr;
  return unused_expression;
}

rq::Expression &Context::copyExpression(rq::Expression &expression) {
  rq::Expression &new_expression = this->acquireExpression();
  if (expression.getHasBranch()) {
    new_expression.setBranch(this->copyExpression(expression.getBranch()));
  }
  new_expression._next_ptr_flags = expression._next_ptr_flags;
  new_expression._keyword = expression._keyword;
  new_expression._source_ptr_flags = expression._source_ptr_flags;
  new_expression._source_text_length = expression._source_text_length;
  return new_expression;
}

rq::InstructionNode &Context::acquireInstructionNode() {
  if (this->acquired._first_unused_instruction_node_ptr == nullptr) {
    rq::InstructionNode &new_node =
        this->allocateAcquiredValue<rq::InstructionNode>();
    return new_node;
  }
  rq::InstructionNode &unused_node =
      rq::dereferencePtr(this->acquired._first_unused_instruction_node_ptr);
  this->acquired._first_unused_instruction_node_ptr =
      llvm::cast<rq::InstructionNode *>(unused_node._car);
  return unused_node;
}

rq::Instruction &Context::acquireInstruction() {
  if (this->acquired._first_unused_instruction_ptr == nullptr) {
    rq::Instruction &new_instruction =
        this->allocateAcquiredValue<rq::Instruction>();
    return new_instruction;
  }
  rq::Instruction &unused_instruction =
      rq::dereferencePtr(this->acquired._first_unused_instruction_ptr);
  this->acquired._first_unused_instruction_ptr = static_cast<rq::Instruction *>(
      llvm::cast<rq::Entity *>(unused_instruction._cdr));
  return unused_instruction;
}

} // namespace rq
