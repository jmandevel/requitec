#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/constants.hpp>
#include <rq/instructions.hpp>
#include <rq/static_value.hpp>
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
struct NullaryInstruction;
struct UnaryInstruction;
struct BinaryInstruction;

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
  rq::Module *_source_module_ptr = nullptr;
  rq::Top _top{};
  rq::FactoryExpression *_free_expression_ptr{nullptr};
  rq::NullaryInstruction *_free_nullary_instruction_ptr{nullptr};
  rq::UnaryInstruction *_free_unary_instruction_ptr{nullptr};
  rq::BinaryInstruction *_free_binary_instruction_ptr{nullptr};
  rq::IntegerLiteral _integer_literal_type{};
  rq::FloatLiteral _float_literal_type{};
  rq::StringLiteral _string_literal_type{};
  rq::CodeunitLiteral _codeunit_literal_type{};
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
  rq::AnchorAttributeType _anchor_attribute_type{};
  rq::OpaqueAttributeType _opaque_attribute_type{};
  rq::GlobalAttributeType _global_attribute_type{};
  rq::AccessAttributeType _access_attribute_type{};
  rq::PartialMutateAttributeType _partial_mutate_attribute_type{};
  rq::StaticAttributeType _static_attribute_type{};
  rq::CaptureAttributeType _capture_attribute_type{};
  rq::InlineAttributeType _inline_attribute_type{};
  rq::MangleAttributeType _mangle_attribute_type{};
  rq::PackAttributeType _pack_attribute_type{};
  rq::BranchTrendAttributeType _branch_trend_attribute_type{};
  rq::SupportStatusAttributeType _support_status_attribute_type{};
  rq::StableAddressAttributeType _stable_address_attribute_type{};
  rq::VariadicAttributeType _variadic_attribute_type{};
  rq::LocationAttributeType _location_attribute_type{};
  rq::TemplateAttributeType _template_attribute_type{};
  rq::ConstraintAttributeType _constraint_attribute_type{};
  rq::WeightAttributeType _weight_attribute_type{};
  rq::RequireAttributeType _require_attribute_type{};
  rq::EnsureAttributeType _ensure_attribute_type{};
  rq::VarAttributeType _var_attribute_type{};
  rq::VolatileAttributeType _volatile_attribute_type{};
  rq::AtomicAttributeType _atomic_attribute_type{};
  rq::NullTerminateAttributeType _null_terminate_attribute_type{};
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
  llvm::FoldingSet<rq::TypeParameter> _type_parameters{};
  llvm::FoldingSet<rq::ProcedureType> _procedure_types{};
  llvm::FoldingSet<rq::TupleType> _tuple_types{};
  llvm::FoldingSet<rq::PlacementType> _placement_types{};
  llvm::FoldingSet<rq::CompositionComponent> _composition_components{};
  llvm::FoldingSet<rq::CompositionType> _composition_types{};
  llvm::FoldingSet<rq::WordConstant> _word_constants{};
  llvm::FoldingSet<rq::ArrayConstant> _array_constants{};
  llvm::FoldingSet<rq::DataArrayConstant> _data_array_constants{};
  llvm::FoldingSet<rq::SymbolConstant> _symbol_constants{};

  Context(std::string &&executable_path)
      : _executable_path(std::move(executable_path)) {}
  Context(const Self &) = delete;
  Context(Self &&) = delete;
  inline ~Context() {
    this->_top.release();
    for (auto &ct : this->_word_constants) {
      std::destroy_at(&ct);
    }
    for (auto &ct : this->_array_constants) {
      std::destroy_at(&ct);
    }
    for (auto &ct : this->_data_array_constants) {
      std::destroy_at(&ct);
    }
    for (auto &ct : this->_symbol_constants) {
      std::destroy_at(&ct);
    }
  }
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
  [[nodiscard]] bool evaluateSourceModule();
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
                                     const rq::FactoryExpression &expression);
  void logErrorIsFirst(const rq::FactoryExpression &mark);
  void logErrorIsLast(const rq::FactoryExpression &mark);
  void logErrorExpectedCommaSeparator(const rq::FactoryExpression &expression);
  void logErrorExpectedSeparatorOrRightBracket(const rq::Token &token);
  void
  logErrorExpectedSemicolonSeparator(const rq::FactoryExpression &expression);
  void
  logErrorExpressionShouldNeverOccur(const rq::FactoryExpression &expression);
  void logErrorDuplicateParameterMark(const rq::FactoryExpression &mark);
  void logErrorDuplicateAttribute(const rq::FactoryExpression &attribute);
  void logErrorNonpositionalBeginAfterPositionalEnd(
      const rq::FactoryExpression &named_begin);
  void logErrorNonpositionalBeginAfterLockedBegin(
      const rq::FactoryExpression &named_begin);
  void logErrorPositionalEndAfterLockedBegin(
      const rq::FactoryExpression &named_begin);
  void logErrorNotExactBranchCount(rq::Situation situation,
                                   const rq::FactoryExpression &expression,
                                   unsigned count);
  void logErrorNotAtLeastBranchCount(rq::Situation situation,
                                     const rq::FactoryExpression &expression,
                                     unsigned count);
  void logErrorTooManyBranchCount(rq::Situation situation,
                                  const rq::FactoryExpression &expression,
                                  unsigned count);
  void logErrorInvalidBranchSituation(rq::Situation situation,
                                      const rq::FactoryExpression &branch);
  void
  logErrorExpectedHeaderExpression(const rq::FactoryExpression &expresison);
  void
  logErrorUnexpectedHeaderExpression(const rq::FactoryExpression &expresison);
  void
  logErrorExpectedChainLinkExpression(const rq::FactoryExpression &expresison);
  void logErrorUnexpectedChainLinkExpression(
      const rq::FactoryExpression &expresison);
  void
  logErrorNotDeterminateStaticValue(const rq::FactoryExpression &expression);
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
  void logErrorUnexpectedRvalueType(const Expression &expression);

  [[nodiscard]] const llvm::fltSemantics &
  getLlvmFloatSemantics(rq::SymbolKind kind);

  [[nodiscard]] rq::FactoryExpression &acquireExpression();
  inline void discardExpression(rq::FactoryExpression &expression) {
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    RQ_ASSERT(!expression.getHasNext(), "has next");
    expression.clear();
    expression._branch_ptr = this->_free_expression_ptr;
    this->_free_expression_ptr = &expression;
  }
  [[nodiscard]] rq::FactoryExpression &
  copyExpression(rq::FactoryExpression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerLiteral &
  acquireIntegerLiteralType() {
    return this->_integer_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatLiteral &acquireFloatLiteralType() {
    return this->_float_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringLiteral &acquireStringLiteralType() {
    return this->_string_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitLiteral &
  acquireCodeunitLiteralType() {
    return this->_codeunit_literal_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OutValue &acquireOutValue() {
    return this->_out_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ThisValue &acquireThisValue() {
    return this->_this_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ResultValue &acquireResultValue() {
    return this->_result_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IndexValue &acquireIndexValue() {
    return this->_index_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DiscriminantValue &
  acquireDiscriminantValue() {
    return this->_discrimnant_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SimpleSymbol &
  acquireCommandLineArgumentsValue() {
    return this->_command_line_arguments_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CallsiteValue &acquireCallsiteValue() {
    return this->_callsite_value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceType &acquireInferenceType() {
    return this->_inference_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VoidType &acquireVoidType() {
    return this->_void_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnType &acquireNoReturnType() {
    return this->_no_return_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorAttributeType &acquireAnchorType() {
    return this->_anchor_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OpaqueAttributeType &acquireOpaqueType() {
    return this->_opaque_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalAttributeType &acquireGlobalType() {
    return this->_global_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AccessAttributeType &acquireAccessType() {
    return this->_access_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PartialMutateAttributeType &
  acquirePartialMutateAttributeType() {
    return this->_partial_mutate_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticAttributeType &
  acquireStaticAttributeType() {
    return this->_static_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CaptureAttributeType &
  acquireCaptureAttributeType() {
    return this->_capture_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InlineAttributeType &
  acquireInlineAttributeType() {
    return this->_inline_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MangleAttributeType &
  acquireMangleAttributeType() {
    return this->_mangle_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PackAttributeType &
  acquirePackAttributeType() {
    return this->_pack_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BranchTrendAttributeType &
  acquireBranchTrendAttributeType() {
    return this->_branch_trend_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SupportStatusAttributeType &
  acquireSupportStatusAttributeType() {
    return this->_support_status_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StableAddressAttributeType &
  acquireStableAddressAttributeType() {
    return this->_stable_address_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicAttributeType &
  acquireVariadicAttributeType() {
    return this->_variadic_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocationAttributeType &
  acquireLocationAttributeType() {
    return this->_location_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateAttributeType &
  acquireTemplateAttributeType() {
    return this->_template_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintAttributeType &
  acquireConstraintAttributeType() {
    return this->_constraint_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WeightAttributeType &
  acquireWeightAttributeType() {
    return this->_weight_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RequireAttributeType &
  acquireRequireAttributeType() {
    return this->_require_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureAttributeType &
  acquireEnsureAttributeType() {
    return this->_ensure_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VarAttributeType &
  acquireVarAttributeType() {
    return this->_var_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileAttributeType &
  acquireVolatileAttributeType() {
    return this->_volatile_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicAttributeType &
  acquireAtomicAttributeType() {
    return this->_atomic_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateAttributeType &
  acquireNullTerminateAttributeType() {
    return this->_null_terminate_attribute_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolType &acquireSymbolType() {
    return this->_symbol_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionType &acquireExpressionType() {
    return this->_expression_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanType &acquireBooleanType() {
    return this->_boolean_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::HalfType &acquireHalfType() {
    return this->_half_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SingleType &acquireSingleType() {
    return this->_single_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DoubleType &acquireDoubleType() {
    return this->_double_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QuadrupleType &acquireQuadrupleType() {
    return this->_quadruple_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedIntegerType &
  acquireSignedIntegerType() {
    return this->_signed_integer_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedIntegerType &
  acquireUnsignedIntegerType() {
    return this->_unsigned_integer_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedIndexType &acquireSignedIndexType() {
    return this->_signed_index_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedIndexType &
  acquireUnsignedIndexType() {
    return this->_unsigned_index_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedAddressType &
  acquireSignedAddressType() {
    return this->_signed_address_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedAddressType &
  acquireUnsignedAddressType() {
    return this->_unsigned_address_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CharType &acquireCharType() {
    return this->_char_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Type &acquireBinary16Type() {
    return this->_binary16_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Type &acquireBinary32Type() {
    return this->_binary32_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Type &acquireBinary64Type() {
    return this->_binary64_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Type &acquireBinary128Type() {
    return this->_binary128_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Bfloat16Type &acquireBfloat16Type() {
    return this->_bfloat16_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AsciiType &acquireAsciiType() {
    return this->_ascii_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Type &acquireUtf8Type() {
    return this->_utf8_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicArgumentsType &
  acquireVariadicArgumentsType() {
    return this->_variadic_arguments_type;
  }
  [[nodiscard]] inline rq::ScaledPrimitiveType &
  acquireScaledPrimitiveType(rq::SymbolKind kind, rq::ScaleKind scale_kind,
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
  acquireScaledSignedIntegerType(rq::ScaleKind kind, unsigned scale,
                                 std::uint64_t synonym_id) {
    return llvm::cast<rq::ScaledSignedIntegerType>(
        this->acquireScaledPrimitiveType(
            rq::SymbolKind::SCALED_SIGNED_INTEGER_TYPE, kind, scale,
            synonym_id));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledUnsignedIntegerType &
  acquireScaledUnsignedIntegerType(rq::ScaleKind kind, unsigned scale,
                                   std::uint64_t synonym_id) {
    return llvm::cast<ScaledUnsignedIntegerType>(
        this->acquireScaledPrimitiveType(
            rq::SymbolKind::SCALED_UNSIGNED_INTEGER_TYPE, kind, scale,
            synonym_id));
  }
  [[nodiscard]] inline rq::ArraySubtype &
  acquireArraySubtype(rq::SymbolConstant &child, std::uint64_t count) {
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
  acquireUncountedSubtype(rq::SymbolKind kind, rq::SymbolConstant &child) {
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
  acquireReferenceSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::ReferenceSubtype>(this->acquireUncountedSubtype(
        rq::SymbolKind::REFERENCE_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSubtype &
  acquirePointerSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::PointerSubtype>(
        this->acquireUncountedSubtype(rq::SymbolKind::POINTER_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SliceSubtype &
  acquireSliceSubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::SliceSubtype>(
        this->acquireUncountedSubtype(rq::SymbolKind::SLICE_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceCountArraySubtype &
  acquireInferenceCountArraySubtype(rq::SymbolConstant &child) {
    return llvm::cast<rq::InferenceCountArraySubtype>(
        this->acquireUncountedSubtype(
            rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE, child));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &
  acquireModule(rq::ModuleFactory &&factory) {
    return this->allocateValue<rq::Module>(std::move(factory));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Import &
  acquireImport(rq::ExpressionFlags flags, rq::Expression &expression,
                rq::Module &module, rq::Module &imported) {
    return this->allocateValue<rq::Import>(flags, expression, module, imported);
  }
  [[nodiscard]] inline rq::JuxtapositionalListType &
  acquireJuxtapositionalListType(rq::JuxtapositionalListItem &first_item) {
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
  acquireArithmeticSequenceType(rq::SymbolKind kind, rq::SymbolConstant &child,
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
  acquireArithmeticIntervalType(rq::SymbolConstant &child,
                                rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::ArithmeticIntervalType>(
        this->acquireArithmeticSequenceType(
            rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE, child, condition,
            rq::ArithmeticSequenceStep::NONE));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticSequenceType &
  acquireInfiniteArithmeticSequenceType(rq::SymbolConstant &child,
                                        rq::ArithmeticSequenceStep step) {
    return llvm::cast<rq::InfiniteArithmeticSequenceType>(
        this->acquireArithmeticSequenceType(
            rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE, child,
            rq::ArithmeticSequenceCondition::NONE, step));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticSequenceType &
  acquireFiniteArithmeticSequenceType(
      rq::SymbolConstant &child, rq::ArithmeticSequenceStep step,
      rq::ArithmeticSequenceCondition condition) {
    return llvm::cast<rq::FiniteArithmeticSequenceType>(
        this->acquireArithmeticSequenceType(
            rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE, child, condition,
            step));
  }
  [[nodiscard]] inline rq::TypeParameter &
  acquireTypeParameter(rq::SymbolKind kind, rq::TypeParameter *next_ptr,
                       llvm::StringRef name, rq::SymbolConstant &type,
                       unsigned location, bool is_positional) {
    llvm::FoldingSetNodeID id;
    rq::profileTypeParameter(id, kind, next_ptr, name, type, location,
                             is_positional);
    void *insert_pos;
    rq::TypeParameter *found_ptr =
        this->_type_parameters.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::TypeParameter &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::TypeParameter &created = this->allocateValue<rq::TypeParameter>(
        kind, next_ptr, name, type, location, is_positional);
    this->_type_parameters.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter &
  acquireProcedureParameter(rq::TypeParameter *next_ptr, llvm::StringRef name,
                            rq::SymbolConstant &type, unsigned location,
                            bool is_positional) {
    return llvm::cast<rq::ProcedureParameter>(this->acquireTypeParameter(
        rq::SymbolKind::PROCEDURE_PARAMETER, next_ptr, name, type, location,
        is_positional));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter &
  acquireTupleParameter(rq::TypeParameter *next_ptr, llvm::StringRef name,
                        rq::SymbolConstant &type, unsigned location,
                        bool is_positional) {
    return llvm::cast<rq::TupleParameter>(
        this->acquireTypeParameter(rq::SymbolKind::TUPLE_PARAMETER, next_ptr,
                                   name, type, location, is_positional));
  }
  [[nodiscard]] inline rq::ProcedureType &acquireProcedureType(
      rq::ProcedureParameter *first_parameter_ptr, unsigned parameter_count,
      unsigned positional_parameter_count,
      unsigned nonpositional_parameter_count, rq::SymbolConstant &return_type,
      rq::SymbolConstant *reciever_type_ptr) {
    llvm::FoldingSetNodeID id;
    rq::profileProcedureType(id, first_parameter_ptr, return_type,
                             reciever_type_ptr);
    void *insert_pos;
    rq::ProcedureType *found_ptr =
        this->_procedure_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::ProcedureType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::ProcedureType &created = this->allocateValue<rq::ProcedureType>(
        first_parameter_ptr, parameter_count, positional_parameter_count,
        nonpositional_parameter_count, return_type, reciever_type_ptr);
    this->_procedure_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] inline rq::TupleType &
  acquireTupleType(rq::TupleParameter *first_parameter_ptr,
                   unsigned parameter_count,
                   unsigned positional_parameter_count,
                   unsigned nonpositional_parameter_count,
                   unsigned type_keyed_parameter_count) {
    llvm::FoldingSetNodeID id;
    rq::profileTupleType(id, first_parameter_ptr);
    void *insert_pos;
    rq::TupleType *found_ptr =
        this->_tuple_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::TupleType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::TupleType &created = this->allocateValue<rq::TupleType>(
        first_parameter_ptr, parameter_count, positional_parameter_count,
        nonpositional_parameter_count, type_keyed_parameter_count);
    this->_tuple_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PlacementType &
  acquirePlacementType(rq::Procedure &procedure) {
    llvm::FoldingSetNodeID id;
    rq::profilePlacement(id, procedure);
    void *insert_pos;
    rq::PlacementType *found_ptr =
        this->_placement_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::PlacementType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::PlacementType &created =
        this->allocateValue<rq::PlacementType>(procedure);
    this->_placement_types.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] inline rq::CompositionComponent &
  acquireCompositionComponent(rq::CompositionComponent *next_ptr) {
    llvm::FoldingSetNodeID id;
    rq::profileCompositionComponent(id, next_ptr);
    void *insert_pos;
    rq::CompositionComponent *found_ptr =
        this->_composition_components.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::CompositionComponent &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::CompositionComponent &created =
        this->allocateValue<rq::CompositionComponent>(next_ptr);
    this->_composition_components.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] inline rq::CompositionType &
  acquireCompositionType(rq::CompositionComponent &first_component) {
    llvm::FoldingSetNodeID id;
    rq::profileCompositionType(id, first_component);
    void *insert_pos;
    rq::CompositionType *found_ptr =
        this->_composition_types.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::CompositionType &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::CompositionType &created =
        this->allocateValue<rq::CompositionType>(first_component);
    this->_composition_types.InsertNode(&created, insert_pos);
    return created;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::WordConstant &
  acquireWordConstant(const llvm::APInt &word) {
    llvm::FoldingSetNodeID id;
    rq::profileWordConstant(id, word);
    void *insert_pos;
    rq::WordConstant *found_ptr =
        this->_word_constants.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::WordConstant &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::WordConstant &created = this->allocateValue<rq::WordConstant>(word);
    this->_word_constants.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WordConstant &
  acquireWordConstant(llvm::APFloat float_) {
    llvm::APInt word = float_.bitcastToAPInt();
    return this->acquireWordConstant(word);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArrayConstant &
  acquireArrayConstant(llvm::ArrayRef<rq::Constant *> array) {
    llvm::FoldingSetNodeID id;
    rq::profileArrayConstant(id, array);
    void *insert_pos;
    rq::ArrayConstant *found_ptr =
        this->_array_constants.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::ArrayConstant &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::ArrayConstant &created = this->allocateValue<rq::ArrayConstant>(array);
    this->_array_constants.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DataArrayConstant &
  acquireDataArrayConstant(llvm::ArrayRef<std::byte> data_array) {
    llvm::FoldingSetNodeID id;
    rq::profileDataArrayConstant(id, data_array);
    void *insert_pos;
    rq::DataArrayConstant *found_ptr =
        this->_data_array_constants.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::DataArrayConstant &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::DataArrayConstant &created =
        this->allocateValue<rq::DataArrayConstant>(data_array);
    this->_data_array_constants.InsertNode(&created, insert_pos);
    return created;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
  acquireSymbolConstant(rq::ExpressionFlags flags, rq::Symbol &symbol) {
    llvm::FoldingSetNodeID id;
    rq::profileSymbolConstant(id, flags, symbol);
    void *insert_pos;
    rq::SymbolConstant *found_ptr =
        this->_symbol_constants.FindNodeOrInsertPos(id, insert_pos);
    if (found_ptr != nullptr) {
      rq::SymbolConstant &found = rq::dereferencePtr(found_ptr);
      return found;
    }
    rq::SymbolConstant &created =
        this->allocateValue<rq::SymbolConstant>(flags, symbol);
    this->_symbol_constants.InsertNode(&created, insert_pos);
    return created;
  }

  [[nodiscard]] inline rq::NullaryInstruction &
  acquireNullaryInstruction(rq::Opcode opcode, rq::Expression &expression) {
    if (this->_free_nullary_instruction_ptr == nullptr) {
      rq::NullaryInstruction &instruction =
          this->allocateValue<rq::NullaryInstruction>(opcode, &expression);
      return instruction;
    }
    rq::NullaryInstruction &instruction =
        rq::dereferencePtr(this->_free_nullary_instruction_ptr);
    this->_free_nullary_instruction_ptr =
        std::bit_cast<rq::NullaryInstruction *>(instruction._expression_ptr);
    instruction._id = rq::getUnderlying(opcode) + rq::OPCODE_OFFSET;
    instruction._expression_ptr = &expression;
    return instruction;
  }

  [[nodiscard]] inline rq::UnaryInstruction &
  acquireUnaryInstruction(rq::Opcode opcode, rq::Expression &expression,
                          rq::Entity &address0) {
    if (this->_free_unary_instruction_ptr == nullptr) {
      rq::UnaryInstruction &instruction =
          this->allocateValue<rq::UnaryInstruction>(opcode, &expression,
                                                    address0);
      return instruction;
    }
    rq::UnaryInstruction &instruction =
        rq::dereferencePtr(this->_free_unary_instruction_ptr);
    this->_free_unary_instruction_ptr =
        std::bit_cast<rq::UnaryInstruction *>(instruction._expression_ptr);
    instruction._id = rq::getUnderlying(opcode) + rq::OPCODE_OFFSET;
    instruction._expression_ptr = &expression;
    instruction._address0_ptr = &address0;
    return instruction;
  }

  [[nodiscard]] inline rq::BinaryInstruction &
  acquireBinaryInstruction(rq::Opcode opcode, rq::Expression &expression,
                           rq::Entity &address0, rq::Entity &address1) {
    if (this->_free_binary_instruction_ptr == nullptr) {
      rq::BinaryInstruction &instruction =
          this->allocateValue<rq::BinaryInstruction>(opcode, &expression,
                                                     address0, address1);
      return instruction;
    }
    rq::BinaryInstruction &instruction =
        rq::dereferencePtr(this->_free_binary_instruction_ptr);
    this->_free_binary_instruction_ptr =
        std::bit_cast<rq::BinaryInstruction *>(instruction._expression_ptr);
    instruction._id = rq::getUnderlying(opcode) + rq::OPCODE_OFFSET;
    instruction._expression_ptr = &expression;
    instruction._address0_ptr = &address0;
    instruction._address1_ptr = &address1;
    return instruction;
  }

  inline void discardInstruction(rq::Instruction &instruction) {
    if (llvm::isa<rq::NullaryInstruction>(instruction)) {
      rq::NullaryInstruction &nullary =
          llvm::cast<rq::NullaryInstruction>(instruction);
      nullary._expression_ptr =
          std::bit_cast<rq::Expression *>(this->_free_nullary_instruction_ptr);
      this->_free_nullary_instruction_ptr = &nullary;
      return;
    }
    if (llvm::isa<rq::UnaryInstruction>(instruction)) {
      rq::UnaryInstruction &unary =
          llvm::cast<rq::UnaryInstruction>(instruction);
      if (llvm::isa<rq::Instruction>(unary.getAddress0())) {
        rq::Instruction &address0 =
            llvm::cast<rq::Instruction>(unary.getAddress0());
        this->discardInstruction(address0);
      }
      unary._expression_ptr =
          std::bit_cast<rq::Expression *>(this->_free_unary_instruction_ptr);
      this->_free_unary_instruction_ptr = &unary;
      return;
    }
    if (llvm::isa<rq::BinaryInstruction>(instruction)) {
      rq::BinaryInstruction &binary =
          llvm::cast<rq::BinaryInstruction>(instruction);
      if (llvm::isa<rq::Instruction>(binary.getAddress0())) {
        rq::Instruction &address0 =
            llvm::cast<rq::Instruction>(binary.getAddress0());
        this->discardInstruction(address0);
      }
      if (llvm::isa<rq::Instruction>(binary.getAddress1())) {
        rq::Instruction &address1 =
            llvm::cast<rq::Instruction>(binary.getAddress1());
        this->discardInstruction(address1);
      }
      binary._expression_ptr =
          std::bit_cast<rq::Expression *>(this->_free_binary_instruction_ptr);
      this->_free_binary_instruction_ptr = &binary;
      return;
    }
  }
};

} // namespace rq
