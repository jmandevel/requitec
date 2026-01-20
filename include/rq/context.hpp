#pragma once

#include <rq/symbol.hpp>
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
struct ModuleSymbol;
enum class Keyword : std::uint32_t;
enum class Situation : std::uint_fast8_t;
enum class TokenKind : std::uint_fast8_t;

enum class LogType : std::underlying_type_t<llvm::SourceMgr::DiagKind> {
  ERROR = llvm::SourceMgr::DiagKind::DK_Error,
  WARN = llvm::SourceMgr::DiagKind::DK_Warning,
  REMARK = llvm::SourceMgr::DiagKind::DK_Remark,
  NOTE = llvm::SourceMgr::DiagKind::DK_Note
};

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
  llvm::TargetMachine *_llvm_target_machine_ptr{nullptr};
  llvm::StringMap<rq::Keyword> _keyword_map;
  llvm::StringMap<rq::ModuleSymbol *> _module_map;
  std::unique_ptr<llvm::LLVMContext> _llvm_context_uptr;
  std::unique_ptr<llvm::Module> _llvm_module_uptr;
  std::unique_ptr<llvm::IRBuilder<>> _llvm_ir_builder_uptr;
  rq::TopSymbol _top_scope{};
  rq::ModuleSymbol *_source_module_ptr = nullptr;
  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs{};
  llvm::FoldingSet<rq::TypeSymbol> _type_symbols{};
  rq::InferenceSymbol *_inference_symbol{nullptr};
  rq::VoidSymbol *_void_symbol{nullptr};
  rq::NullSymbol *_null_symbol{nullptr};
  rq::NoReturnSymbol *_no_return_symbol{nullptr};
  rq::VariadicArgumentsSymbol *_variadic_arguments_symbol{nullptr};
  rq::BooleanSymbol *_boolean_symbol{nullptr};
  rq::AsciiSymbol *_ascii_symbol{nullptr};
  rq::Utf8Symbol *_utf8_symbol{nullptr};
  llvm::FoldingSet<rq::ScaledIntegerSymbol> _scaled_integer_symbol{};
  llvm::FoldingSet<rq::ScaledRealSymbol> _scaled_real_symbol{};
  llvm::FoldingSet<rq::UnarySubtypeSymbol> _unary_subtype_symbols{};
  llvm::FoldingSet<rq::CountedSubtypeSymbol> _counted_subtype_symbols{};
  llvm::FoldingSet<rq::LayoutSymbol> _layout_symbols{};
  llvm::FoldingSet<rq::SignatureSymbol> _signature_symbols{};
  llvm::FoldingSet<rq::ExtensionSymbol> _extension_symbols{};
  llvm::FoldingSet<rq::ArithmeticSequenceSymbol> _arithmetic_sequence_symbols{};

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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &getTopScope() {
    return this->_top_scope;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  getTopScope() const {
    return this->_top_scope;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getExecutablePath() const {
    return this->_executable_path;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceModule() const {
    return this->_source_module_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getSourceModule() {
    return rq::dereferencePtr(this->_source_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &
  getSourceModule() const {
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
  [[nodiscard]] bool validateSourceText(const rq::ModuleSymbol &module);
  [[nodiscard]] bool tokenizeSourceText(const rq::ModuleSymbol &module,
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
  [[nodiscard]] rq::ModuleSymbol *
  loadImportModule(rq::Expression &expression, llvm::StringRef import_string);
  [[nodiscard]] bool initializeLlvm();
  [[nodiscard]] bool run();
  [[nodiscard]] bool parseRequite(rq::ModuleSymbol &module,
                                  const std::vector<rq::Token> &tokens);
  [[nodiscard]] bool situateModule(rq::ModuleSymbol &module);
  [[nodiscard]] bool tabulateModule(rq::ModuleSymbol &module);
  [[nodiscard]] bool emitTokens(llvm::StringRef path,
                                llvm::ArrayRef<rq::Token> tokens);
  [[nodiscard]] bool emitRequite(llvm::StringRef path,
                                 const rq::Expression &trunk);
  [[nodiscard]] bool emitSymbol(llvm::StringRef path, const rq::Symbol &symbol);
  [[nodiscard]] bool emitLlvmIr(llvm::StringRef path);
  [[nodiscard]] bool emitAssembly(llvm::StringRef path);
  [[nodiscard]] bool emitObject(llvm::StringRef path);
  RQ_ALWAYS_INLINE void logMessage(llvm::Twine twine) {
    llvm::outs() << twine << "\n";
  }
  RQ_ALWAYS_INLINE void logMessage(llvm::SMLoc location, rq::LogType type,
                                   llvm::Twine message,
                                   llvm::ArrayRef<llvm::SMRange> ranges,
                                   llvm::ArrayRef<llvm::SMFixIt> fixits) {
    this->_llvm_source_mgr.PrintMessage(
        llvm::outs(), location, static_cast<llvm::SourceMgr::DiagKind>(type),
        message, ranges, fixits, true);
#if !defined(_NDEBUG)
    if (type == rq::LogType::ERROR) {
#if __has_builtin(__builtin_debugtrap)
      __builtin_debugtrap();
#endif
    }
#endif
  }
  void logErrorInvalidUtf8Codeunit(llvm::SMLoc location, char c);
  void logErrorInvalidUtf8Continuation(llvm::SMLoc start, llvm::SMLoc cur,
                                       char c);
  void logErrorSourceFileNoRqExtension(llvm::StringRef path);
  void logErrorFailedToCanonicalizePath(llvm::StringRef path,
                                        const std::error_code &ec);
  void logErrorFailedToLoadSourceFileBuffer(llvm::StringRef path,
                                            const std::error_code &ec);
  void logErrorImportFileNotFound(const rq::Expression &expression,
                                  llvm::StringRef import_string);
  void logErrorFailedToCanonicializeImportPath(const rq::Expression &expression,
                                               const std::error_code &ec);
  void logErrorFailedToLoadImportFileBuffer(const rq::Expression &expression,
                                            const std::error_code &ec);
  void logErrorFailedToFindLlvmTarget(llvm::StringRef target_triple,
                                      llvm::StringRef error);
  void logErrorFailedToOpenOutputFile(llvm::StringRef path,
                                      const std::error_code &ec);
  void logErrorFailedToOpenIntermediateFile(llvm::StringRef path,
                                            const std::error_code &ec);
  void logErrorFailedToAddPassesToEmitFile(llvm::StringRef path);
  void logErrorFoundErrorToken(const rq::Token &token);
  void logErrorUnexpectedToken(const rq::Token &token);
  void logErrorExpectedIdentifierLiteral(const rq::Token &token);
  void logErrorNotKeyword(const rq::Token &token);
  void logErrorInternalUseOnlyKeyword(const rq::Token &token,
                                      rq::Keyword keyword);
  void logErrorUnmatchedRightToken(const rq::Token &left_token,
                                   const rq::Token &right_token);
  void logErrorSoloRightToken(const rq::Token &token);
  void logErrorUnterminatedStringLiteral(const rq::Token &token);
  void logErrorUnmatchedLeftToken(const rq::Token &token);
  void logErrorTrailerTokenMismatch(const rq::Token &trailer_token,
                                    const rq::Token &front_token,
                                    const rq::Expression &expression);
  void logErrorUnterminatedInterpolatedString(const rq::Token &token);
  void logErrorMustHaveParameterMark(rq::Situation situation,
                                     const rq::Expression &expression);
  void logErrorPositionalEndIsFirst(const rq::Expression &mark);
  void logErrorNamedBeginIsLast(const rq::Expression &mark);
  void logErrorExpectedCommaSeparator(const rq::Expression &expression);
  void logErrorExpectedSeparatorOrRightBracket(const rq::Token &token);
  void logErrorExpectedSemicolonSeparator(const rq::Expression &expression);
  void logErrorExpressionShouldNeverOccur(const rq::Expression &expression);
  void logErrorDuplicateParameterMark(const rq::Expression &mark);
  void logErrorDuplicateAttribute(const rq::Expression &attribute);
  void logErrorNamedBeginAfterPositionalEnd(const rq::Expression &named_begin);
  void logErrorNotExactBranchCount(rq::Situation situation,
                                   const rq::Expression &expression,
                                   unsigned count);
  void logErrorNotAtLeastBranchCount(rq::Situation situation,
                                     const rq::Expression &expression,
                                     unsigned count);
  void logErrorTooManyBranchCount(rq::Situation situation,
                                  const rq::Expression &expression,
                                  unsigned count);
  void logErrorInvalidBranchSituation(rq::Situation situation,
                                      const rq::Expression &branch);
  void logErrorExpectedHeaderExpression(const rq::Expression &expresison);
  void logErrorUnexpectedHeaderExpression(const rq::Expression &expresison);
  void logErrorExpectedChainLinkExpression(const rq::Expression &expresison);
  void logErrorUnexpectedChainLinkExpression(const rq::Expression &expresison);
  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateValue(ArgNParam &&...arg_n) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }

  inline llvm::StringRef saveString(llvm::Twine twine) {
    return this->_llvm_string_saver.save(twine);
  }
  [[nodiscard]] rq::Expression &acquireExpression();
  inline void discardExpression(rq::Expression &expression) {
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    RQ_ASSERT(!expression.getHasNext(), "has next");
    this->_unused_expression_ptrs.emplace_back(&expression);
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);

  inline rq::InferenceSymbol &getInference() {
    if (!this->_inference_symbol) {
      this->_inference_symbol = &this->allocateValue<rq::InferenceSymbol>();
    }
    return rq::dereferencePtr(this->_inference_symbol);
  }

  inline rq::VoidSymbol &getVoid() {
    if (!this->_void_symbol) {
      this->_void_symbol = &this->allocateValue<rq::VoidSymbol>();
    }
    return rq::dereferencePtr(this->_void_symbol);
  }

  inline rq::NullSymbol &getNull() {
    if (!this->_null_symbol) {
      this->_null_symbol = &this->allocateValue<rq::NullSymbol>();
    }
    return rq::dereferencePtr(this->_null_symbol);
  }

  inline rq::NoReturnSymbol &getNoReturn() {
    if (!this->_no_return_symbol) {
      this->_no_return_symbol = &this->allocateValue<rq::NoReturnSymbol>();
    }
    return rq::dereferencePtr(this->_no_return_symbol);
  }

  inline rq::VariadicArgumentsSymbol &getVariadicArguments() {
    if (!this->_variadic_arguments_symbol) {
      this->_variadic_arguments_symbol =
          &this->allocateValue<rq::VariadicArgumentsSymbol>();
    }
    return rq::dereferencePtr(this->_variadic_arguments_symbol);
  }

  inline rq::BooleanSymbol &getBoolean() {
    if (!this->_boolean_symbol) {
      this->_boolean_symbol = &this->allocateValue<rq::BooleanSymbol>();
    }
    return rq::dereferencePtr(this->_boolean_symbol);
  }

  [[nodiscard]] inline rq::AsciiSymbol &getAscii() {
    if (!this->_ascii_symbol) {
      this->_ascii_symbol = &this->allocateValue<rq::AsciiSymbol>();
    }
    return rq::dereferencePtr(this->_ascii_symbol);
  }
  [[nodiscard]] inline rq::Utf8Symbol &getUtf8() {
    if (!this->_utf8_symbol) {
      this->_utf8_symbol = &this->allocateValue<rq::Utf8Symbol>();
    }
    return rq::dereferencePtr(this->_utf8_symbol);
  }
  [[nodiscard]] inline rq::ScaledIntegerSymbol &
  _getOrInsertScaledIntegerSymbol(rq::SymbolKind kind, unsigned scalar,
                                  unsigned uid, rq::ScaledIntegerFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledIntegerSymbol(id, kind, scalar, uid, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledIntegerSymbol *existing =
            this->_scaled_integer_symbol.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ScaledIntegerSymbol &new_type =
        this->allocateValue<rq::ScaledIntegerSymbol>(kind, scalar, uid, flags);
    this->_scaled_integer_symbol.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] inline rq::UnsignedSymbol &
  getUnsigned(unsigned scalar, unsigned uid, rq::ScaledIntegerFlags flags) {
    return llvm::cast<rq::UnsignedSymbol>(this->_getOrInsertScaledIntegerSymbol(
        rq::SymbolKind::UNSIGNED, scalar, uid, flags));
  }
  [[nodiscard]] inline rq::SignedSymbol &
  getSigned(unsigned scalar, unsigned uid, rq::ScaledIntegerFlags flags) {
    return llvm::cast<rq::SignedSymbol>(this->_getOrInsertScaledIntegerSymbol(
        rq::SymbolKind::UNSIGNED, scalar, uid, flags));
  }
  [[nodiscard]] inline rq::ScaledRealSymbol &
  _getOrInsertScaledRealSymbol(rq::SymbolKind kind, unsigned scalar,
                               rq::ScaledRealFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledRealSymbol(id, kind, scalar, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledRealSymbol *existing =
            this->_scaled_real_symbol.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ScaledRealSymbol &new_type =
        this->allocateValue<rq::ScaledRealSymbol>(kind, scalar, flags);
    this->_scaled_real_symbol.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] inline rq::FloatSymbol &getFloat(unsigned scalar,
                                                 rq::ScaledRealFlags flags) {
    return llvm::cast<rq::FloatSymbol>(this->_getOrInsertScaledRealSymbol(
        rq::SymbolKind::FLOAT, scalar, flags));
  }
  [[nodiscard]] inline rq::UnarySubtypeSymbol &
  _getOrInsertUnarySubtypeSymbol(rq::SymbolKind kind, rq::Symbol &subtype) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtypeSymbol(id, kind, subtype);
    void *insert_pos = nullptr;
    if (rq::UnarySubtypeSymbol *existing =
            this->_unary_subtype_symbols.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::UnarySubtypeSymbol &new_type =
        this->allocateValue<rq::UnarySubtypeSymbol>(kind, subtype);
    this->_unary_subtype_symbols.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RangeSymbol &
  getRange(rq::TypeSymbol &root) {
    return llvm::cast<rq::RangeSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::SymbolKind::RANGE, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ReferenceSymbol &
  getReference(rq::TypeSymbol &root) {
    return llvm::cast<rq::ReferenceSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::SymbolKind::REFERENCE, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSymbol &
  getPointer(rq::TypeSymbol &root) {
    return llvm::cast<rq::PointerSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::SymbolKind::POINTER, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FatPointerSymbol &
  getFatPointer(rq::TypeSymbol &root) {
    return llvm::cast<rq::FatPointerSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::SymbolKind::FAT_POINTER,
                                             root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArraySymbol &
  getInferecedCountArray(rq::TypeSymbol &root) {
    return llvm::cast<rq::InferencedCountArraySymbol>(
        this->_getOrInsertUnarySubtypeSymbol(
            rq::SymbolKind::INFERENCED_COUNT_ARRAY, root));
  }
  [[nodiscard]] inline rq::CountedSubtypeSymbol &
  _getOrInsertCountedSubtypeSymbol(rq::SymbolKind kind, rq::TypeSymbol &root,
                                   unsigned count) {
    llvm::FoldingSetNodeID id;
    rq::profileCountedSubtypeSymbol(id, kind, root, count);
    void *insert_pos = nullptr;
    if (rq::CountedSubtypeSymbol *existing =
            this->_counted_subtype_symbols.FindNodeOrInsertPos(id,
                                                               insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::CountedSubtypeSymbol &new_type =
        this->allocateValue<rq::CountedSubtypeSymbol>(kind, root, count);
    this->_counted_subtype_symbols.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArraySymbol &getArray(rq::TypeSymbol &root,
                                                           unsigned count) {
    return llvm::cast<rq::ArraySymbol>(this->_getOrInsertCountedSubtypeSymbol(
        rq::SymbolKind::ARRAY, root, count));
  }
  [[nodiscard]] inline rq::ArithmeticSequenceSymbol &
  _getOrInsertArithmeticSequenceSymbol(
      rq::SymbolKind kind, rq::TypeSymbol &root,
      rq::ArithmeticSequenceCondition condition,
      rq::ArithmeticSequenceStep step) {
    llvm::FoldingSetNodeID id;
    rq::profileArithmeticSequenceSymbol(id, root, condition, step);
    void *insert_pos = nullptr;
    if (rq::ArithmeticSequenceSymbol *existing =
            this->_arithmetic_sequence_symbols.FindNodeOrInsertPos(
                id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ArithmeticSequenceSymbol &new_type =
        this->allocateValue<rq::ArithmeticSequenceSymbol>(kind, root, condition,
                                                          step);
    this->_arithmetic_sequence_symbols.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticIntervalSymbol &
  getArithmeticInterval(rq::TypeSymbol &root,
                        rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::ArithmeticIntervalSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::SymbolKind::ARITHMETIC_INTERVAL, root, condition,
            rq::ArithmeticSequenceStep::NONE));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &
  getFiniteArithmeticProgression(rq::TypeSymbol &root,
                                 rq::ArithmeticSequenceStep step,
                                 rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::FiniteArithmeticProgressionSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::SymbolKind::FINITE_ARITHMETIC_PROGRESSION, root, condition,
            step));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &
  getInfiniteArithmeticProgression(rq::TypeSymbol &root,
                                   rq::ArithmeticSequenceStep step) {
    return llvm::cast<rq::FiniteArithmeticProgressionSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::SymbolKind::INFINITE_ARITHMETIC_PROGRESSION, root,
            rq::ArithmeticSequenceCondition::NONE, step));
  }
};

} // namespace rq
