#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/see.hpp>
#include <rq/symbols.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/FoldingSet.h>
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
struct Module;
enum class TokenKind : std::uint_fast8_t;
enum class NumericResultCode : unsigned;

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

struct Context final : public rq::BumpPtrAllocator {
  using Self = rq::Context;

  std::string _executable_path;
  llvm::SourceMgr _llvm_source_mgr;
  llvm::TargetMachine *_llvm_target_machine_ptr{nullptr};
  llvm::StringMap<rq::Keyword> _keyword_map;
  llvm::StringMap<rq::Module *> _module_map;
  std::unique_ptr<llvm::LLVMContext> _llvm_context_uptr;
  std::unique_ptr<llvm::Module> _llvm_module_uptr;
  std::unique_ptr<llvm::IRBuilder<>> _llvm_ir_builder_uptr;
  rq::SymbolicExecutionEngine _see{};
  rq::Module *_source_module_ptr = nullptr;
  rq::Top _top{};
  rq::Expression *_free_expression_ptr{nullptr};
  rq::IntegerLiteralType _integer_literal_type{};
  rq::FloatLiteralType _float_literal_type{};
  rq::StringLiteralType _string_literal_type{};
  rq::CodeunitLiteralType _codeunit_literal_type{};
  rq::OutValue _out_value{};
  rq::ThisValue _this_value{};
  rq::ResultValue _result_value{};
  rq::IndexValue _index_value{};
  rq::DiscriminantValue _discrimnant_value{};
  rq::CommandLineArgumentsValue _command_line_arguments_value{};
  rq::CallsiteValue _callsite_value{};
  rq::InferenceType _inference_type{};
  rq::VoidType _void_type{};
  rq::NoReturnType _no_return_type{};
  rq::AnchorType _anchor_type{};
  rq::OpaqueType _opaque_type{};
  rq::GlobalType _global_type{};
  rq::AccessType _access_type{};
  rq::PartialMutateType _partial_mutate_type{};
  rq::StaticType _static_type{};
  rq::CaptureType _capture_type{};
  rq::InlineType _inline_type{};
  rq::MangleType _mangle_type{};
  rq::PackType _pack_type{};
  rq::BranchTrendType _branch_trend_type{};
  rq::DepreciateType _depreciate_type{};
  rq::StableAddressType _stable_address_type{};
  rq::VariadicType _variadic_type{};
  rq::LocationType _location_type{};
  rq::TemplateType _template_type{};
  rq::ConstraintType _constraint_type{};
  rq::WeightType _weight_type{};
  rq::RequireType _require_type{};
  rq::EnsureType _ensure_type{};
  rq::VarType _var_type{};
  rq::VolatileType _volatile_type{};
  rq::AtomicType _atomic_type{};
  rq::NullTerminateType _null_terminate_type{};
  rq::SymbolType _symbol_type{};
  rq::ExpressionType _expression_type{};
  rq::BooleanType _boolean_type{};
  rq::HalfType _half_type{};
  rq::SingleType _single_type{};
  rq::DoubleType _double_type{};
  rq::QuadrupleType _quadruple_type{};
  rq::SignedIntegerType _signed_integer_type{};
  rq::UnsignedIntegerType _unsigned_integer_type{};
  rq::SignedIndexType _signed_index_type{};
  rq::UnsignedIndexType _unsigned_index_type{};
  rq::SignedAddressType _signed_address_type{};
  rq::UnsignedAddressType _unsigned_address_type{};
  rq::CharType _char_type{};
  rq::Binary16Type _binary16_type{};
  rq::Binary32Type _binary32_type{};
  rq::Binary64Type _binary64_type{};
  rq::Binary128Type _binary128_type{};
  rq::Bfloat16Type _bfloat16_type{};
  rq::AsciiType _ascii_type{};
  rq::Utf8Type _utf8_type{};
  rq::VariadicArgumentsType _variadic_arguments_type{};
  llvm::FoldingSet<rq::ScaledPrimitiveType> _scaled_primitive_types{};
  llvm::FoldingSet<rq::ArraySubtype> _array_subtypes{};
  llvm::FoldingSet<rq::UncountedSubtype> _uncounted_subtypes{};
  llvm::FoldingSet<rq::JuxtapositionalListItem> _juxtapositional_list_items{};
  llvm::FoldingSet<rq::JuxtapositionalListType> _juxtapositional_list_types{};
  llvm::FoldingSet<rq::ArithmeticSequenceType> _arithmetic_sequence_types{};

  Context(std::string &&executable_path)
      : _executable_path(std::move(executable_path)) {}
  Context(const Self &) = delete;
  Context(Self &&) = delete;
  inline ~Context() { this->_top.release(); }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Top &getTop() { return this->_top; }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Top &getTop() const {
    return this->_top;
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
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::IRBuilder<> &getLlvmIrBuilder() {
    return rq::dereferenceUptr(this->_llvm_ir_builder_uptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::IRBuilder<> &
  getLlvmIrBuilder() const {
    return rq::dereferenceUptr(this->_llvm_ir_builder_uptr);
  }
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
  [[nodiscard]] rq::Module *loadImportModule(const rq::Expression &expression,
                                             llvm::StringRef import_string);
  [[nodiscard]] bool initializeLlvm();
  [[nodiscard]] bool run();
  [[nodiscard]] bool validateSourceText(const rq::ModuleFactory &factory);
  [[nodiscard]] bool tokenizeSourceText(rq::ModuleFactory &factory);
  [[nodiscard]] bool parseRequite(rq::ModuleFactory &factory);
  [[nodiscard]] bool situateModule(rq::ModuleFactory &factory);
  [[nodiscard]] bool generateSourceModule();
  [[nodiscard]] bool buildLlvmIr();
  [[nodiscard]] bool emitTokens(llvm::StringRef path,
                                llvm::ArrayRef<rq::Token> tokens);
  [[nodiscard]] bool emitRequite(llvm::StringRef path,
                                 const rq::Expression *top_ptr);
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
  void logErrorIsFirst(const rq::Expression &mark);
  void logErrorIsLast(const rq::Expression &mark);
  void logErrorExpectedCommaSeparator(const rq::Expression &expression);
  void logErrorExpectedSeparatorOrRightBracket(const rq::Token &token);
  void logErrorExpectedSemicolonSeparator(const rq::Expression &expression);
  void logErrorExpressionShouldNeverOccur(const rq::Expression &expression);
  void logErrorDuplicateParameterMark(const rq::Expression &mark);
  void logErrorDuplicateAttribute(const rq::Expression &attribute);
  void logErrorNonpositionalBeginAfterPositionalEnd(
      const rq::Expression &named_begin);
  void
  logErrorNonpositionalBeginAfterLockedBegin(const rq::Expression &named_begin);
  void logErrorPositionalEndAfterLockedBegin(const rq::Expression &named_begin);
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
  void logErrorGlobalIndeterminateDynamicExpression(
      const rq::Expression &expression);
  void logErrorInvalidExpressionAttribute(const rq::Expression &unascribed,
                                          const rq::Expression &attribute);
  void logErrorFailedToAscribeExpression(const rq::Expression &unascribed,
                                         const rq::Expression &attribute);
  void logErrorNotSymbol(const rq::Expression &expression);
  void logErrorNotLabel(const rq::Expression &expression);
  void logErrorLabelSubjectNotTable(const rq::Expression &expression);
  void logErrorNotInTop(const rq::Expression &expression);
  void logErrorUnableToEvaluateName(const rq::Expression &expression);
  void logErrorUnableToEvaluateUtf8Cstr(const rq::Expression &expression);
  void logErrorFailedToImportModule(const rq::Expression &expression,
                                    llvm::StringRef path);
  void logErrorFlankNotInFrame(const rq::Expression &flank_expression);
  void logErrorFlankNotAncestor(const rq::Expression &flank_expression);
  void logErrorNumeric(const rq::Expression &expression,
                       rq::NumericResultCode code);
  void logErrorNameCollision(const rq::Expression &expression);
  void logInfoNameCollisionDeclaration(rq::Symbol &symbol);
  void logErrorInvalidValueSymbol(const rq::Expression &expression,
                                  rq::Symbol &symbol);
  void logErrorIndeterminateVariableValue(const rq::Expression &expression,
                                          rq::Symbol &symbol);
  [[nodiscard]] rq::Expression &acquireExpression();
  inline void discardExpression(rq::Expression &expression) {
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    RQ_ASSERT(!expression.getHasNext(), "has next");
    expression.clear();
    expression._branch_ptr = this->_free_expression_ptr;
    this->_free_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerLiteralType &
  getIntegerLiteralType() {
    return this->_integer_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatLiteralType &getFloatLiteralType() {
    return this->_float_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringLiteralType &getStringLiteralType() {
    return this->_string_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitLiteralType &
  getCodeunitLiteralType() {
    return this->_codeunit_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OutValue &getOutValue() {
    return this->_out_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ThisValue &getThisValue() {
    return this->_this_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ResultValue &getResultValue() {
    return this->_result_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IndexValue &getIndexValue() {
    return this->_index_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DiscriminantValue &getDiscriminantValue() {
    return this->_discrimnant_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CommandLineArgumentsValue &
  getCommandLineArgumentsValue() {
    return this->_command_line_arguments_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CallsiteValue &getCallsiteValue() {
    return this->_callsite_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceType &getInferenceType() {
    return this->_inference_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VoidType &getVoidType() {
    return this->_void_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnType &getNoReturnType() {
    return this->_no_return_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorType &getAnchorType() {
    return this->_anchor_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OpaqueType &getOpaqueType() {
    return this->_opaque_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalType &getGlobalType() {
    return this->_global_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AccessType &getAccessType() {
    return this->_access_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PartialMutateType &getPartialMutateType() {
    return this->_partial_mutate_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticType &getStaticType() {
    return this->_static_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CaptureType &getCaptureType() {
    return this->_capture_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InlineType &getInlineType() {
    return this->_inline_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MangleType &getMangleType() {
    return this->_mangle_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PackType &getPackType() {
    return this->_pack_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BranchTrendType &getBranchTrendType() {
    return this->_branch_trend_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DepreciateType &getDepreciateType() {
    return this->_depreciate_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StableAddressType &getStableAddressType() {
    return this->_stable_address_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicType &getVariadicType() {
    return this->_variadic_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocationType &getLocationType() {
    return this->_location_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateType &getTemplateType() {
    return this->_template_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintType &getConstraintType() {
    return this->_constraint_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WeightType &getWeightType() {
    return this->_weight_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RequireType &getRequireType() {
    return this->_require_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureType &getEnsureType() {
    return this->_ensure_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VarType &getVarType() {
    return this->_var_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileType &getVolatileType() {
    return this->_volatile_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicType &getAtomicType() {
    return this->_atomic_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateType &getNullTerminateType() {
    return this->_null_terminate_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolType &getSymbolType() {
    return this->_symbol_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionType &getExpressionType() {
    return this->_expression_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanType &getBooleanType() {
    return this->_boolean_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::HalfType &getHalfType() {
    return this->_half_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SingleType &getSingleType() {
    return this->_single_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DoubleType &getDoubleType() {
    return this->_double_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QuadrupleType &getQuadrupleType() {
    return this->_quadruple_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedIntegerType &getSignedIntegerType() {
    return this->_signed_integer_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedIntegerType &
  getUnsignedIntegerType() {
    return this->_unsigned_integer_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedIndexType &getSignedIndexType() {
    return this->_signed_index_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedIndexType &getUnsignedIndexType() {
    return this->_unsigned_index_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedAddressType &getSignedAddressType() {
    return this->_signed_address_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedAddressType &
  getUnsignedAddressType() {
    return this->_unsigned_address_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CharType &getCharType() {
    return this->_char_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Type &getBinary16Type() {
    return this->_binary16_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Type &getBinary32Type() {
    return this->_binary32_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Type &getBinary64Type() {
    return this->_binary64_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Type &getBinary128Type() {
    return this->_binary128_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Bfloat16Type &getBfloat16Type() {
    return this->_bfloat16_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AsciiType &getAsciiType() {
    return this->_ascii_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Type &getUtf8Type() {
    return this->_utf8_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicArgumentsType &
  getVariadicArgumentsType() {
    return this->_variadic_arguments_type;
  }
  [[nodiscard]] inline rq::ScaledPrimitiveType &
  getScaledPrimitiveType(rq::SymbolKind kind, rq::ScaleKind scale_kind,
                         unsigned scale, std::uint64_t synonym_id) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledPrimitiveType(id, kind, scale_kind, scale, synonym_id);
    void *insert_pos;
    rq::ScaledPrimitiveType *found_ptr =
        this->_scaled_primitive_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::ScaledPrimitiveType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::ScaledPrimitiveType &created =
        this->allocateValue<rq::ScaledPrimitiveType>(kind, scale_kind, scale,
                                                     synonym_id);
    this->_scaled_primitive_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledSignedIntegerType &
  getScaledSignedIntegerType(rq::ScaleKind kind, unsigned scale,
                             std::uint64_t synonym_id) {
    return llvm::cast<rq::ScaledSignedIntegerType>(this->getScaledPrimitiveType(
        rq::SymbolKind::SCALED_SIGNED_INTEGER_TYPE, kind, scale, synonym_id));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledUnsignedIntegerType &
  getScaledUnsignedIntegerType(rq::ScaleKind kind, unsigned scale,
                               std::uint64_t synonym_id) {
    return llvm::cast<rq::ScaledUnsignedIntegerType>(
        this->getScaledPrimitiveType(
            rq::SymbolKind::SCALED_UNSIGNED_INTEGER_TYPE, kind, scale,
            synonym_id));
  }
  [[nodiscard]] inline rq::ArraySubtype &
  getArraySubtype(rq::SymbolConstant &child, std::uint64_t count) {
    llvm::FoldingSetNodeID id;
    rq::profileArraySubtype(id, child, count);
    void *insert_pos;
    rq::ArraySubtype *found_ptr =
        this->_array_subtypes.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::ArraySubtype &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::ArraySubtype &created =
        this->allocateValue<rq::ArraySubtype>(child, count);
    this->_array_subtypes.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] inline rq::UncountedSubtype &
  getUncountedSubtype(rq::SymbolKind kind, rq::SymbolConstant &child) {
    llvm::FoldingSetNodeID id;
    rq::profileUncountedSubtype(id, kind, child);
    void *insert_pos;
    rq::UncountedSubtype *found_ptr =
        this->_uncounted_subtypes.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::UncountedSubtype &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::UncountedSubtype &created =
        this->allocateValue<rq::UncountedSubtype>(kind, child);
    this->_uncounted_subtypes.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ReferenceSubtype &
  getReferenceSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::ReferenceSubtype>(
        this->getUncountedSubtype(rq::SymbolKind::REFERENCE_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSubtype &
  getPointerSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::PointerSubtype>(
        this->getUncountedSubtype(rq::SymbolKind::POINTER_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SliceSubtype &
  getSliceSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::SliceSubtype>(
        this->getUncountedSubtype(rq::SymbolKind::SLICE_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceCountArraySubtype &
  getInferenceCountArraySubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::InferenceCountArraySubtype>(this->getUncountedSubtype(
        rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &
  getModule(rq::ModuleFactory &&factory) {
    return this->allocateValue<rq::Module>(std::move(factory));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Import &
  getImport(rq::ExpressionFlags flags, const rq::Expression &expression,
            rq::Module &imported) {
    return this->allocateValue<rq::Import>(flags, expression, imported);
  }
  [[nodiscard]] inline rq::JuxtapositionalListType &
  getJuxtapositionalListType(rq::JuxtapositionalListItem &first_item) {
    llvm::FoldingSetNodeID id;
    rq::profileJuxtapositionalListType(id, first_item);
    void *insert_pos;
    rq::JuxtapositionalListType *found_ptr =
        this->_juxtapositional_list_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::JuxtapositionalListType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::JuxtapositionalListType &created =
        this->allocateValue<rq::JuxtapositionalListType>(first_item);
    this->_juxtapositional_list_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] inline rq::ArithmeticSequenceType &
  getArithmeticSequenceType(rq::SymbolKind kind, rq::SymbolConstant &child,
                            rq::ArithmeticSequenceCondition condition,
                            rq::ArithmeticSequenceStep step) {
    llvm::FoldingSetNodeID id;
    rq::profileArithmeticSequenceType(id, kind, child, condition, step);
    void *insert_pos;
    rq::ArithmeticSequenceType *found_ptr =
        this->_arithmetic_sequence_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::ArithmeticSequenceType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::ArithmeticSequenceType &created =
        this->allocateValue<rq::ArithmeticSequenceType>(kind, child, condition,
                                                        step);
    this->_arithmetic_sequence_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticIntervalType &
  getArithmeticIntervalType(rq::SymbolConstant &child,
                            rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::ArithmeticIntervalType>(
        this->getArithmeticSequenceType(
            rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE, child, condition,
            rq::ArithmeticSequenceStep::NONE));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticSequenceType &
  getInfiniteArithmeticSequenceType(rq::SymbolConstant &child,
                                    rq::ArithmeticSequenceStep step) {
    return llvm::cast<rq::InfiniteArithmeticSequenceType>(
        this->getArithmeticSequenceType(
            rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE, child,
            rq::ArithmeticSequenceCondition::NONE, step));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticSequenceType &
  getFiniteArithmeticSequenceType(rq::SymbolConstant &child,
                                  rq::ArithmeticSequenceStep step,
                                  rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::FiniteArithmeticSequenceType>(
        this->getArithmeticSequenceType(
            rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE, child, condition,
            step));
  }
};

} // namespace rq
