#pragma once

#include <rq/static.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/StringSaver.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

namespace rq {

struct Token;
struct Expression;
struct Module;
enum class Keyword : std::uint32_t;
enum class Situation : std::uint_fast8_t;
enum class TokenKind : std::uint_fast8_t;

enum class LogType : std::underlying_type_t<llvm::SourceMgr::DiagKind> {
  ERROR = llvm::SourceMgr::DiagKind::DK_Error,
  WARN = llvm::SourceMgr::DiagKind::DK_Warning,
  REMARK = llvm::SourceMgr::DiagKind::DK_Remark,
  NOTE = llvm::SourceMgr::DiagKind::DK_Note
};

enum class System : std::uint_fast8_t {
  // before first stage (before source file is loaded)
  PRE,
  // loading source files
  LOAD,
  // STAGE 1: validation of raw source file text
  VALIDATION,
  // STAGE 2: seperation of source lexemes into tokens
  TOKENIZATION,
  // STAGE 3: construction of abstract-syntax-tree (AST)
  PARSING,
  // STAGE 4: constext-sensitive error detection and modification of AST
  SITUATION,
  // STAGE 5: construction of symbol tables
  TABULATION,
  // STAGE 6: generation of LLVM ir
  GENERATION,
  // after last stage (after ir is generated)
  POST,
  // when emitting to an output or intermediate file
  EMIT,
  // systems used by stages 5 and 6
  TYPE_SYSTEM,
  SYMBOLIC_EXECUTION_ENGINE
};

[[nodiscard]] inline llvm::StringLiteral getLogTag(rq::System system) {
  switch (system) {
  case rq::System::PRE:
    return "pre";
  case rq::System::LOAD:
    return "load";
  case rq::System::VALIDATION:
    return "vald";
  case rq::System::TOKENIZATION:
    return "tokn";
  case rq::System::PARSING:
    return "pars";
  case rq::System::SITUATION:
    return "sit";
  case rq::System::TABULATION:
    return "tab";
  case rq::System::GENERATION:
    return "gen";
  case rq::System::POST:
    return "post";
  case rq::System::EMIT:
    return "emit";
  case rq::System::TYPE_SYSTEM:
    return "tsys";
  case rq::System::SYMBOLIC_EXECUTION_ENGINE:
    return "see";
  }
  RQ_UNREACHABLE();
}

struct SourceLocation final {
  llvm::StringRef file = {};
  unsigned line = 0;
  unsigned column = 0;

  [[nodiscard]] inline bool operator==(const rq::SourceLocation &other) const {
    return this->file == other.file && this->line == other.line &&
           this->column == other.column;
  }

  [[nodiscard]] inline bool operator!=(const rq::SourceLocation &other) const {
    return !(*this == other);
  }
};

struct SourceRange final {
  rq::SourceLocation start = {};
  rq::SourceLocation end = {};
};

struct Context final {
  using Self = rq::Context;

  std::string _executable_path;
  llvm::SourceMgr _llvm_source_mgr;
  llvm::TargetMachine *_llvm_target_machine_ptr = nullptr;
  llvm::StringMap<rq::Keyword> _keyword_map;
  llvm::StringMap<rq::Module *> _module_map;
  std::unique_ptr<llvm::LLVMContext> _llvm_context_uptr;
  std::unique_ptr<llvm::Module> _llvm_module_uptr;
  std::unique_ptr<llvm::IRBuilder<>> _llvm_ir_builder_uptr;
  rq::StaticFrame _top_static_frame{};
  rq::Module *_source_module_ptr = nullptr;

  Context(std::string &&executable_path)
      : _executable_path(std::move(executable_path)) {}
  Context(const Self &) = delete;
  Context(Self &&) = delete;
  ~Context() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticFrame &getTopStaticFrame() {
    return this->_top_static_frame;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticFrame &
  getTopStaticFrame() const {
    return this->_top_static_frame;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getExecutablePath() const {
    return this->_executable_path;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceModule() const {
    return this->_source_module_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getSourceModule() {
    return rq::dereferencePtr(this->_source_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getSourceModule() const {
    return rq::dereferencePtr(this->_source_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::TargetMachine &getLlvmTargetMachine() {
    return rq::dereferencePtr(this->_llvm_target_machine_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::TargetMachine &
  getLlvmTargetMachine() const {
    return rq::dereferencePtr(this->_llvm_target_machine_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Module &getLlvmModule() {
    return rq::dereferenceUptr(this->_llvm_module_uptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Module &getLlvmModule() const {
    return rq::dereferenceUptr(this->_llvm_module_uptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::LLVMContext &getLlvmContext() {
    return rq::dereferenceUptr(this->_llvm_context_uptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::LLVMContext &
  getLlvmContext() const {
    return rq::dereferenceUptr(this->_llvm_context_uptr);
  }
  [[nodiscard]] bool validateSourceText(const rq::Module &module);
  [[nodiscard]] bool tokenizeSourceText(const rq::Module &module,
                                        std::vector<rq::Token> &tokens);
  void initializeKeywordMap();
  [[nodiscard]] rq::Keyword getKeyword(llvm::Twine name);
  [[nodiscard]] rq::SourceLocation getSourceLocation(llvm::SMLoc llvm_location);
  [[nodiscard]] inline rq::SourceRange
  getSourceRange(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE std::error_code
  canonicalizePath(llvm::SmallVectorImpl<char> &path);
  [[nodiscard]] llvm::ErrorOr<llvm::MemoryBufferRef>
  loadRequiteFileBuffer(llvm::StringRef path);
  [[nodiscard]] bool loadSourceModule();
  [[nodiscard]] rq::Module *loadImportModule(rq::Expression &expression,
                                             llvm::StringRef import_string);
  [[nodiscard]] bool initializeLlvm();
  [[nodiscard]] bool run();
  [[nodiscard]] bool parseRequite(rq::Module &module,
                                  const std::vector<rq::Token> &tokens);
  [[nodiscard]] bool situateModule(rq::Module &module);
  [[nodiscard]] bool tabulateModule(rq::Module &module);
  [[nodiscard]] bool emitTokens(llvm::StringRef path,
                                llvm::ArrayRef<rq::Token> tokens);
  [[nodiscard]] bool emitRequite(llvm::StringRef path,
                                 const rq::Expression &trunk);
  // [[nodiscard]] bool emitSymbols(llvm::StringRef path, const rq::SymbolTable&
  // table);
  [[nodiscard]] bool emitLlvmIr(llvm::StringRef path);
  [[nodiscard]] bool emitAssembly(llvm::StringRef path);
  [[nodiscard]] bool emitObject(llvm::StringRef path);
  RQ_ALWAYS_INLINE void logMessage(rq::System system, llvm::Twine twine) {
    llvm::outs() << "[" << rq::getLogTag(system) << "]" << twine << "\n";
  }
  RQ_ALWAYS_INLINE void logMessage(rq::System system, llvm::SMLoc location,
                                   rq::LogType type, llvm::Twine message,
                                   llvm::ArrayRef<llvm::SMRange> ranges,
                                   llvm::ArrayRef<llvm::SMFixIt> fixits) {
    this->_llvm_source_mgr.PrintMessage(
        llvm::outs(), location, static_cast<llvm::SourceMgr::DiagKind>(type),
        llvm::Twine("[") + rq::getLogTag(system) + "] " + message, ranges,
        fixits, true);
#if !defined(_NDEBUG) && __has_builtin(__builtin_debugtrap)
    if (type == rq::LogType::ERROR) {
      __builtin_debugtrap();
    }
#endif
  }
  void logErrorInvalidUtf8Codeunit(rq::System system, llvm::SMLoc location,
                                   char c);
  void logErrorInvalidUtf8Continuation(rq::System system, llvm::SMLoc start,
                                       llvm::SMLoc cur, char c);
  void logErrorSourceFileNoRqExtension(rq::System system, llvm::StringRef path);
  void logErrorFailedToCanonicalizePath(rq::System system, llvm::StringRef path,
                                        const std::error_code &ec);
  void logErrorFailedToLoadSourceFileBuffer(rq::System system,
                                            llvm::StringRef path,
                                            const std::error_code &ec);
  void logErrorImportFileNotFound(rq::System system,
                                  const rq::Expression &expression,
                                  llvm::StringRef import_string);
  void logErrorFailedToCanonicializeImportPath(rq::System system,
                                               const rq::Expression &expression,
                                               const std::error_code &ec);
  void logErrorFailedToLoadImportFileBuffer(rq::System system,
                                            const rq::Expression &expression,
                                            const std::error_code &ec);
  void logErrorFailedToFindLlvmTarget(rq::System system,
                                      llvm::StringRef target_triple,
                                      llvm::StringRef error);
  void logErrorFailedToOpenOutputFile(rq::System system, llvm::StringRef path,
                                      const std::error_code &ec);
  void logErrorFailedToOpenIntermediateFile(rq::System system,
                                            llvm::StringRef path,
                                            const std::error_code &ec);
  void logErrorFailedToAddPassesToEmitFile(rq::System system,
                                           llvm::StringRef path);
  void logErrorFoundErrorToken(rq::System system, const rq::Token &token);
  void logErrorUnexpectedToken(rq::System system, const rq::Token &token);
  void logErrorExpectedIdentifierLiteral(rq::System system,
                                         const rq::Token &token);
  void logErrorNotKeyword(rq::System system, const rq::Token &token);
  void logErrorInternalUseOnlyKeyword(rq::System system, const rq::Token &token,
                                      rq::Keyword keyword);
  void logErrorUnmatchedRightToken(rq::System system,
                                   const rq::Token &left_token,
                                   const rq::Token &right_token);
  void logErrorSoloRightToken(rq::System system, const rq::Token &token);
  void logErrorUnterminatedStringLiteral(rq::System system,
                                         const rq::Token &token);
  void logErrorUnmatchedLeftToken(rq::System system, const rq::Token &token);
  void logErrorTrailerTokenMismatch(rq::System system,
                                    const rq::Token &trailer_token,
                                    const rq::Token &front_token,
                                    const rq::Expression &expression);
  void logErrorUnterminatedInterpolatedString(rq::System system,
                                              const rq::Token &token);
  void logErrorMustHaveParameterMark(rq::System system, rq::Situation situation,
                                     const rq::Expression &expression);
  void logErrorPositionalEndIsFirst(rq::System system, rq::Situation situation,
                                    const rq::Expression &expression,
                                    const rq::Expression &positional_end,
                                    unsigned positional_end_i);
  void logErrorNamedBeginIsLast(rq::System system, rq::Situation situation,
                                const rq::Expression &expression,
                                const rq::Expression &named_begin,
                                unsigned named_begin_i);
  void logErrorExpectedCommaSeparator(rq::System system,
                                      const rq::Expression &expression);
  void logErrorExpectedSeparatorOrRightBracket(rq::System system,
                                               const rq::Token &token);
  void logErrorExpectedSemicolonSeparator(rq::System system,
                                          const rq::Expression &expression);
  void logErrorExpressionShouldNeverOccur(rq::System system,
                                          const rq::Expression &expression);
  void logErrorDuplicateParameterMark(rq::System system,
                                      rq::Situation situation,
                                      rq::Expression &expression,
                                      rq::Expression &parameter,
                                      unsigned branch_i, rq::Expression &first,
                                      unsigned first_i);
  void logErrorNamedBeginAfterPositionalEnd(
      rq::System system, rq::Situation situation, rq::Expression &expression,
      rq::Expression &named_begin, unsigned named_begin_i,
      rq::Expression &first_positional_end, unsigned first_positional_end_i);
  void logErrorNotExactBranchCount(rq::System system, rq::Situation situation,
                                   const rq::Expression &expression,
                                   unsigned count);
  void logErrorNotAtLeastBranchCount(rq::System system, rq::Situation situation,
                                     const rq::Expression &expression,
                                     unsigned count);
  void logErrorTooManyBranchCount(rq::System system, rq::Situation situation,
                                  const rq::Expression &expression,
                                  unsigned count);
  void logErrorInvalidBranchSituation(rq::System system,
                                      rq::Situation outer_situation,
                                      const rq::Expression &outer,
                                      rq::Situation branch_situation,
                                      rq::Expression &branch, unsigned branch_i,
                                      llvm::Twine log_context);
  void logErrorFirstBranchNotHeader(rq::System system, rq::Situation situation,
                                    const rq::Expression &expression,
                                    const rq::Expression &branch0);
};

} // namespace rq
