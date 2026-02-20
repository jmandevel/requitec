#pragma once

#include <rq/entity.hpp>
#include <rq/see.hpp>
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
struct ModuleSymbol;
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
  rq::SymbolicExecutionEngine _see{};
  rq::TopSymbol _top_scope{};
  rq::ModuleSymbol *_source_module_ptr = nullptr;
  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs{};
  std::vector<rq::Instruction *> _unused_instruction_ptrs{};
  std::vector<rq::InstructionNode *> _unused_instruction_node_ptrs{};
  llvm::FoldingSet<rq::TypeSymbol> _type_symbols{};
  rq::InferenceSymbol *_inference_symbol{nullptr};
  rq::ExpressionSymbol *_expression_symbol{nullptr};
  rq::VoidSymbol *_void_symbol_ptr{nullptr};
  rq::NullSymbol *_null_symbol_ptr{nullptr};
  rq::NoReturnSymbol *_no_return_symbol_ptr{nullptr};
  rq::VariadicArgumentsSymbol *_variadic_arguments_symbol_ptr{nullptr};
  rq::BooleanSymbol *_boolean_symbol_ptr{nullptr};
  rq::GenericFloatSymbol *_generic_float_symbol_ptr{nullptr};
  rq::HalfSymbol *_half_symbol_ptr{nullptr};
  rq::SingleSymbol *_single_symbol_ptr{nullptr};
  rq::DoubleSymbol *_double_symbol_ptr{nullptr};
  rq::QuadrupleSymbol *_quadruple_symbol_ptr{nullptr};
  rq::GenericBinarySymbol *_generic_binary_symbol_ptr{nullptr};
  rq::GenericBfloatSymbol *_generic_bfloat_symbol_ptr{nullptr};
  rq::Binary16Symbol *_binary16_symbol_ptr{nullptr};
  rq::Binary32Symbol *_binary32_symbol_ptr{nullptr};
  rq::Binary64Symbol *_binary64_symbol_ptr{nullptr};
  rq::Binary128Symbol *_binary128_symbol_ptr{nullptr};
  rq::Bfloat16Symbol *_bfloat16_symbol_ptr{nullptr};
  rq::GenericIntegerSymbol *_generic_integer_symbol_ptr{nullptr};
  rq::GenericSignedSymbol *_generic_signed_symbol_ptr{nullptr};
  rq::GenericUnsignedSymbol *_generic_unsigned_symbol_ptr{nullptr};
  rq::GenericStringSymbol *_generic_string_symbol_ptr{nullptr};
  rq::GenericCodeunitSymbol *_generic_codeunit_symbol_ptr{nullptr};
  rq::AsciiSymbol *_ascii_symbol_ptr{nullptr};
  rq::Utf8Symbol *_utf8_symbol_ptr{nullptr};
  llvm::FoldingSet<rq::ScaledBuiltinSymbol> _scaled_builtin_symbols{};
  llvm::FoldingSet<rq::UnarySubtypeSymbol> _unary_subtype_symbols{};
  llvm::FoldingSet<rq::CountedSubtypeSymbol> _counted_subtype_symbols{};
  llvm::FoldingSet<rq::LayoutSymbol> _layout_symbols{};
  llvm::FoldingSet<rq::SignatureSymbol> _signature_symbols{};
  llvm::FoldingSet<rq::ExtensionSymbol> _extension_symbols{};
  llvm::FoldingSet<rq::ArithmeticSequenceSymbol> _arithmetic_sequence_symbols{};
  llvm::FoldingSet<rq::IntegerConstant> _integer_constants{};
  llvm::FoldingSet<rq::FloatConstant> _float_constants{};
  llvm::FoldingSet<rq::StringConstant> _string_constants{};
  llvm::FoldingSet<rq::ArrayConstant> _array_constants{};

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
  [[nodiscard]] bool buildLlvmIr();
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
  void logErrorNotDeterminateStaticValue(const rq::Expression &expression);
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

  inline rq::InferenceSymbol &acquireInferenceSymbol() {
    if (!this->_inference_symbol) {
      this->_inference_symbol = &this->allocateValue<rq::InferenceSymbol>();
    }
    return rq::dereferencePtr(this->_inference_symbol);
  }

  inline rq::ExpressionSymbol &acquireExpressionSymbol() {
    if (!this->_expression_symbol) {
      this->_expression_symbol = &this->allocateValue<rq::ExpressionSymbol>();
    }
    return rq::dereferencePtr(this->_expression_symbol);
  }

  inline rq::VoidSymbol &acquireVoidSymbol() {
    if (!this->_void_symbol_ptr) {
      this->_void_symbol_ptr = &this->allocateValue<rq::VoidSymbol>();
    }
    return rq::dereferencePtr(this->_void_symbol_ptr);
  }

  inline rq::NullSymbol &acquireNullSymbol() {
    if (!this->_null_symbol_ptr) {
      this->_null_symbol_ptr = &this->allocateValue<rq::NullSymbol>();
    }
    return rq::dereferencePtr(this->_null_symbol_ptr);
  }

  inline rq::NoReturnSymbol &acquireNoReturnSymbol() {
    if (!this->_no_return_symbol_ptr) {
      this->_no_return_symbol_ptr = &this->allocateValue<rq::NoReturnSymbol>();
    }
    return rq::dereferencePtr(this->_no_return_symbol_ptr);
  }

  inline rq::VariadicArgumentsSymbol &acquireVariadicArgumentsSymbol() {
    if (!this->_variadic_arguments_symbol_ptr) {
      this->_variadic_arguments_symbol_ptr =
          &this->allocateValue<rq::VariadicArgumentsSymbol>();
    }
    return rq::dereferencePtr(this->_variadic_arguments_symbol_ptr);
  }

  inline rq::BooleanSymbol &acquireBooleanSymbol() {
    if (!this->_boolean_symbol_ptr) {
      this->_boolean_symbol_ptr = &this->allocateValue<rq::BooleanSymbol>();
    }
    return rq::dereferencePtr(this->_boolean_symbol_ptr);
  }

  inline rq::GenericFloatSymbol &acquireGenericFloatSymbol() {
    if (!this->_generic_float_symbol_ptr) {
      this->_generic_float_symbol_ptr =
          &this->allocateValue<rq::GenericFloatSymbol>();
    }
    return rq::dereferencePtr(this->_generic_float_symbol_ptr);
  }

  inline rq::GenericBinarySymbol &acquireGenericBinarySymbol() {
    if (!this->_generic_binary_symbol_ptr) {
      this->_generic_binary_symbol_ptr =
          &this->allocateValue<rq::GenericBinarySymbol>();
    }
    return rq::dereferencePtr(this->_generic_binary_symbol_ptr);
  }

  inline rq::GenericBfloatSymbol &acquireGenericBfloatSymbol() {
    if (!this->_generic_bfloat_symbol_ptr) {
      this->_generic_bfloat_symbol_ptr =
          &this->allocateValue<rq::GenericBfloatSymbol>();
    }
    return rq::dereferencePtr(this->_generic_bfloat_symbol_ptr);
  }

  inline rq::GenericIntegerSymbol &acquireGenericIntegerSymbol() {
    if (!this->_generic_integer_symbol_ptr) {
      this->_generic_integer_symbol_ptr =
          &this->allocateValue<rq::GenericIntegerSymbol>();
    }
    return rq::dereferencePtr(this->_generic_integer_symbol_ptr);
  }

  inline rq::GenericSignedSymbol &acquireGenericSignedSymbol() {
    if (!this->_generic_signed_symbol_ptr) {
      this->_generic_signed_symbol_ptr =
          &this->allocateValue<rq::GenericSignedSymbol>();
    }
    return rq::dereferencePtr(this->_generic_signed_symbol_ptr);
  }

  inline rq::GenericUnsignedSymbol &acquireGenericUnsignedSymbol() {
    if (!this->_generic_unsigned_symbol_ptr) {
      this->_generic_unsigned_symbol_ptr =
          &this->allocateValue<rq::GenericUnsignedSymbol>();
    }
    return rq::dereferencePtr(this->_generic_unsigned_symbol_ptr);
  }
  [[nodiscard]] inline rq::GenericStringSymbol &acquireGenericStringSymbol() {
    if (!this->_generic_string_symbol_ptr) {
      this->_generic_string_symbol_ptr =
          &this->allocateValue<rq::GenericStringSymbol>();
    }
    return rq::dereferencePtr(this->_generic_string_symbol_ptr);
  }
  [[nodiscard]] inline rq::GenericCodeunitSymbol &
  acquireGenericCodeunitSymbol() {
    if (!this->_generic_codeunit_symbol_ptr) {
      this->_generic_codeunit_symbol_ptr =
          &this->allocateValue<rq::GenericCodeunitSymbol>();
    }
    return rq::dereferencePtr(this->_generic_codeunit_symbol_ptr);
  }
  [[nodiscard]] inline rq::AsciiSymbol &acquireAsciiSymbol() {
    if (!this->_ascii_symbol_ptr) {
      this->_ascii_symbol_ptr = &this->allocateValue<rq::AsciiSymbol>();
    }
    return rq::dereferencePtr(this->_ascii_symbol_ptr);
  }
  [[nodiscard]] inline rq::Utf8Symbol &acquireUtf8Symbol() {
    if (!this->_utf8_symbol_ptr) {
      this->_utf8_symbol_ptr = &this->allocateValue<rq::Utf8Symbol>();
    }
    return rq::dereferencePtr(this->_utf8_symbol_ptr);
  }
  [[nodiscard]] inline rq::ScaledBuiltinSymbol &
  _getOrInsertScaledBuiltin(rq::EntityKind kind, unsigned scalar, unsigned uid,
                            rq::ScaledBuiltinFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledIntegerSymbol(id, kind, scalar, uid, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledBuiltinSymbol *existing =
            this->_scaled_builtin_symbols.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ScaledBuiltinSymbol &new_type =
        this->allocateValue<rq::ScaledBuiltinSymbol>(kind, scalar, uid, flags);
    this->_scaled_builtin_symbols.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledSignedSymbol &
  acquireScaledSignedSymbol(unsigned scalar, rq::ScaledBuiltinFlags flags) {
    return llvm::cast<rq::ScaledSignedSymbol>(this->_getOrInsertScaledBuiltin(
        rq::EntityKind::SY_SCALED_SIGNED, scalar, 0, flags));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledUnsignedSymbol &
  acquireScaledUnsignedSymbol(unsigned scalar, rq::ScaledBuiltinFlags flags) {
    return llvm::cast<rq::ScaledUnsignedSymbol>(this->_getOrInsertScaledBuiltin(
        rq::EntityKind::SY_SCALED_UNSIGNED, scalar, 0, flags));
  }
  [[nodiscard]] inline rq::UnarySubtypeSymbol &
  _getOrInsertUnarySubtypeSymbol(rq::EntityKind kind, rq::Symbol &subtype) {
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
  acquireRangeSymbol(rq::TypeSymbol &root) {
    return llvm::cast<rq::RangeSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::EntityKind::SY_RANGE, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ReferenceSymbol &
  acquireReferenceSymbol(rq::TypeSymbol &root) {
    return llvm::cast<rq::ReferenceSymbol>(this->_getOrInsertUnarySubtypeSymbol(
        rq::EntityKind::SY_REFERENCE, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSymbol &
  acquirePointerSymbol(rq::TypeSymbol &root) {
    return llvm::cast<rq::PointerSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::EntityKind::SY_POINTER, root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FatPointerSymbol &
  acquireFatPointerSymbol(rq::TypeSymbol &root) {
    return llvm::cast<rq::FatPointerSymbol>(
        this->_getOrInsertUnarySubtypeSymbol(rq::EntityKind::SY_FAT_POINTER,
                                             root));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArraySymbol &
  acquireInferencedCountArraySymbol(rq::TypeSymbol &root) {
    return llvm::cast<rq::InferencedCountArraySymbol>(
        this->_getOrInsertUnarySubtypeSymbol(
            rq::EntityKind::SY_INFERENCED_COUNT_ARRAY, root));
  }
  [[nodiscard]] inline rq::CountedSubtypeSymbol &
  _getOrInsertCountedSubtypeSymbol(rq::EntityKind kind, rq::TypeSymbol &root,
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArraySymbol &
  acquireArraySymbol(rq::TypeSymbol &root, unsigned count) {
    return llvm::cast<rq::ArraySymbol>(this->_getOrInsertCountedSubtypeSymbol(
        rq::EntityKind::SY_ARRAY, root, count));
  }
  [[nodiscard]] inline rq::ArithmeticSequenceSymbol &
  _getOrInsertArithmeticSequenceSymbol(
      rq::EntityKind kind, rq::TypeSymbol &root,
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
  acquireArithmeticIntervalSymbol(rq::TypeSymbol &root,
                                  rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::ArithmeticIntervalSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::EntityKind::SY_ARITHMETIC_INTERVAL, root, condition,
            rq::ArithmeticSequenceStep::NONE));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &
  acquireFiniteArithmeticProgressionSymbol(
      rq::TypeSymbol &root, rq::ArithmeticSequenceStep step,
      rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::FiniteArithmeticProgressionSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION, root, condition,
            step));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticProgressionSymbol &
  acquireInfiniteArithmeticProgressionSymbol(rq::TypeSymbol &root,
                                             rq::ArithmeticSequenceStep step) {
    return llvm::cast<rq::InfiniteArithmeticProgressionSymbol>(
        this->_getOrInsertArithmeticSequenceSymbol(
            rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION, root,
            rq::ArithmeticSequenceCondition::NONE, step));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerConstant &
  acquireIntegerContant(const llvm::APInt &value) {
    llvm::FoldingSetNodeID id;
    rq::profileIntegerConstant(id, value);
    void *insert_pos = nullptr;
    if (rq::IntegerConstant *existing =
            this->_integer_constants.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::IntegerConstant &c = this->allocateValue<rq::IntegerConstant>(value);
    this->_integer_constants.InsertNode(&c, insert_pos);
    return c;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConstant &
  acquireFloatConstant(const llvm::APFloat &value) {
    llvm::FoldingSetNodeID id;
    rq::profileFloatConstant(id, value);
    void *insert_pos = nullptr;
    if (rq::FloatConstant *existing =
            this->_float_constants.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::FloatConstant &c = this->allocateValue<rq::FloatConstant>(value);
    this->_float_constants.InsertNode(&c, insert_pos);
    return c;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringConstant &
  acquireStringConstant(llvm::StringRef value) {
    llvm::StringRef saved = this->saveString(value);
    llvm::FoldingSetNodeID id;
    rq::profileStringConstant(id, saved);
    void *insert_pos = nullptr;
    if (rq::StringConstant *existing =
            this->_string_constants.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::StringConstant &c = this->allocateValue<rq::StringConstant>(saved);
    this->_string_constants.InsertNode(&c, insert_pos);
    return c;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArrayConstant &
  acquireArrayConstant(const rq::BumpPtrList<rq::Entity *> &elements) {
    llvm::FoldingSetNodeID id;
    rq::profileArrayConstant(id, elements);
    void *insert_pos = nullptr;
    if (rq::ArrayConstant *existing =
            this->_array_constants.FindNodeOrInsertPos(id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ArrayConstant &c = this->allocateValue<rq::ArrayConstant>(elements);
    this->_array_constants.InsertNode(&c, insert_pos);
    return c;
  }
  inline void discardInstruction(rq::Instruction &instruction);
  inline void discardInstructionNode(rq::InstructionNode &node);
  inline void _discardInstructionSlot(rq::InstructionSlot &slot) {
    if (slot.getIsInstruction()) {
      this->discardInstruction(slot.getInstruction());
    } else if (slot.getIsInstructionNode()) {
      this->discardInstructionNode(slot.getInstructionNode());
    }
  }
  [[nodiscard]] rq::InstructionNode &acquireInstructionNode();
  [[nodiscard]] rq::Instruction &acquireInstruction();
};
inline void Context::discardInstructionNode(rq::InstructionNode &node) {
  this->_discardInstructionSlot(node.getCar());
  this->_discardInstructionSlot(node.getCdr());
  this->_unused_instruction_node_ptrs.emplace_back(&node);
}
inline void Context::discardInstruction(rq::Instruction &instruction) {
  this->_discardInstructionSlot(instruction.getCdr());
  this->_unused_instruction_ptrs.emplace_back(&instruction);
}

} // namespace rq
