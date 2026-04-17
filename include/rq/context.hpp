#pragma once

#include <rq/bump_ptr_allocator.hpp>
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
  struct {
    rq::Expression *_first_unused_expression_ptr{nullptr};
    rq::NullaryInstruction *_first_unused_nullary_instruction_ptr{nullptr};
    rq::UnaryInstruction *_first_unused_unary_instruction_ptr{nullptr};
    rq::BinaryInstruction *_first_unused_binary_instruction_ptr{nullptr};
    rq::Result _result{};
    rq::Out _out{};
    rq::IntegerLiteral _integer_literal{};
    rq::FloatLiteral _float_literal{};
    rq::StringLiteral _string_literal{};
    rq::CodeunitLiteral _codeunit_literal{};
    rq::Inference _inference{};
    rq::SymbolReflection _symbol_reflection{};
    rq::Type _type{};
    rq::Void _void{};
    rq::NoReturn _no_return{};
    rq::VariadicArguments _variadic_arguments{};
    rq::Boolean _boolean{};
    rq::SignedConstraint _signed_constraint{};
    rq::UnsignedConstraint _unsigned_constraint{};
    rq::FloatConstraint _float_constraint{};
    rq::BinaryConstraint _binary_constraint{};
    rq::BfloatConstraint _bfloat_constraint{};
    rq::Half _half{};
    rq::Single _single{};
    rq::Double _double{};
    rq::Quadruple _quadruple{};
    rq::Binary16 _binary16{};
    rq::Binary32 _binary32{};
    rq::Binary64 _binary64{};
    rq::Binary128 _binary128{};
    rq::Bfloat16 _bfloat16{};
    rq::IntegerConstraint _integer_constraint{};
    rq::SignedIntegerConstraint _signed_integer_constraint{};
    rq::UnsignedIntegerConstraint _unsigned_integer_constraint{};
    rq::CodeunitConstraint _codeunit_constraint{};
    rq::StringConstraint _string_constraint{};
    rq::InterpolatedStringConstraint _interpolated_string_constraint{};
    rq::Char _char{};
    rq::Ascii _ascii{};
    rq::Utf8 _utf8{};
    rq::Visibility _visibility{};
    rq::Scoping _scoping{};
    rq::Availability _availability{};
    rq::Accessibility _accessibility{};
    rq::PropertyMutability _property_mutability{};
    rq::Exporting _exporting{};
    rq::Runtime _runtime{};
    rq::Capturing _capturing{};
    rq::EvaluationTime _evaluation_time{};
    rq::Parentability _parentability{};
    rq::PropertyAssociation _property_association{};
    rq::Tangibility _tangibility{};
    rq::Overriding _overriding{};
    rq::Inlining _inlining{};
    rq::Mangling _mangling{};
    unsigned _scaled_builtin_generation{1};
    llvm::FoldingSet<rq::ScaledBuiltin> _scaled_builtin_set{};
    llvm::FoldingSet<rq::UnarySubtype> _unary_subtype_set{};
    llvm::FoldingSet<rq::CountedSubtype> _counted_subtype_set{};
    llvm::FoldingSet<rq::ArithmeticSequence> _arithmetic_sequence_set{};
    llvm::FoldingSet<rq::TypeConstant> _type_constant_set{};
    llvm::FoldingSet<rq::ExpressionConstant> _expression_constant_set{};
    rq::BooleanConstant _true{true};
    rq::BooleanConstant _false{false};
    rq::ExpressionAttributeConstant _ea_none{rq::ExpressionAttribute::NONE};
    rq::ExpressionAttributeConstant _ea_transparent{
        rq::ExpressionAttribute::TRANSPARENT};
    rq::ExpressionAttributeConstant _ea_opaque{rq::ExpressionAttribute::OPAQUE};
    rq::ExpressionAttributeConstant _ea_inside_scope{
        rq::ExpressionAttribute::INSIDE_SCOPE};
    rq::ExpressionAttributeConstant _ea_outside_scope{
        rq::ExpressionAttribute::OUTSIDE_SCOPE};
    rq::ExpressionAttributeConstant _ea_local{rq::ExpressionAttribute::LOCAL};
    rq::ExpressionAttributeConstant _ea_global{rq::ExpressionAttribute::GLOBAL};
    rq::ExpressionAttributeConstant _ea_private{
        rq::ExpressionAttribute::PRIVATE};
    rq::ExpressionAttributeConstant _ea_public{rq::ExpressionAttribute::PUBLIC};
    rq::ExpressionAttributeConstant _ea_protected{
        rq::ExpressionAttribute::PROTECTED};
    rq::ExpressionAttributeConstant _ea_no_partial_mutate{
        rq::ExpressionAttribute::NO_PARTIAL_MUTATE};
    rq::ExpressionAttributeConstant _ea_partial_mutate{
        rq::ExpressionAttribute::PARTIAL_MUTATE};
    rq::ExpressionAttributeConstant _ea_no_export{
        rq::ExpressionAttribute::NO_EXPORT};
    rq::ExpressionAttributeConstant _ea_export{rq::ExpressionAttribute::EXPORT};
    rq::ExpressionAttributeConstant _ea_dynamic{
        rq::ExpressionAttribute::DYNAMIC};
    rq::ExpressionAttributeConstant _ea_static{rq::ExpressionAttribute::STATIC};
    rq::ExpressionAttributeConstant _ea_no_capture{
        rq::ExpressionAttribute::NO_CAPTURE};
    rq::ExpressionAttributeConstant _ea_capture{
        rq::ExpressionAttribute::CAPTURE};
    rq::ExpressionAttributeConstant _ea_lazy{rq::ExpressionAttribute::LAZY};
    rq::ExpressionAttributeConstant _ea_eager{rq::ExpressionAttribute::EAGER};
    rq::ExpressionAttributeConstant _ea_no_parent{
        rq::ExpressionAttribute::NO_PARENT};
    rq::ExpressionAttributeConstant _ea_may_parent{
        rq::ExpressionAttribute::MAY_PARENT};
    rq::ExpressionAttributeConstant _ea_mixin{rq::ExpressionAttribute::MIXIN};
    rq::ExpressionAttributeConstant _ea_parent{rq::ExpressionAttribute::PARENT};
    rq::ExpressionAttributeConstant _ea_tangible{
        rq::ExpressionAttribute::TANGIBLE};
    rq::ExpressionAttributeConstant _ea_abstract{
        rq::ExpressionAttribute::ABSTRACT};
    rq::ExpressionAttributeConstant _ea_virtual{
        rq::ExpressionAttribute::VIRTUAL};
    rq::ExpressionAttributeConstant _ea_no_override{
        rq::ExpressionAttribute::NO_OVERRIDE};
    rq::ExpressionAttributeConstant _ea_override{
        rq::ExpressionAttribute::OVERRIDE};
    rq::ExpressionAttributeConstant _ea_no_inline{
        rq::ExpressionAttribute::NO_INLINE};
    rq::ExpressionAttributeConstant _ea_inline{rq::ExpressionAttribute::INLINE};
    rq::ExpressionAttributeConstant _ea_implicit_mangle{
        rq::ExpressionAttribute::IMPLICIT_MANGLE};
    rq::ExpressionAttributeConstant _ea_explicit_mangle{
        rq::ExpressionAttribute::EXPLICIT_MANGLE};
    rq::ExpressionAttributeConstant _ea_no_pack{
        rq::ExpressionAttribute::NO_PACK};
    rq::ExpressionAttributeConstant _ea_pack{rq::ExpressionAttribute::PACK};
    rq::ExpressionAttributeConstant _ea_label{rq::ExpressionAttribute::LABEL};
    rq::ExpressionAttributeConstant _ea_no_template{
        rq::ExpressionAttribute::NO_TEMPLATE};
    rq::ExpressionAttributeConstant _ea_template{
        rq::ExpressionAttribute::TEMPLATE};
    rq::ExpressionAttributeConstant _ea_specialize{
        rq::ExpressionAttribute::SPECIALIZE};
    rq::ExpressionAttributeConstant _ea_equivocal{
        rq::ExpressionAttribute::EQUIVOCAL};
    rq::ExpressionAttributeConstant _ea_likely{rq::ExpressionAttribute::LIKELY};
    rq::ExpressionAttributeConstant _ea_unlikely{
        rq::ExpressionAttribute::UNLIKELY};
    rq::ExpressionAttributeConstant _ea_supported{
        rq::ExpressionAttribute::SUPPORTED};
    rq::ExpressionAttributeConstant _ea_depreciated{
        rq::ExpressionAttribute::DEPRECIATED};
    rq::ExpressionAttributeConstant _ea_experimental{
        rq::ExpressionAttribute::EXPERIMENTAL};
    rq::ExpressionAttributeConstant _ea_no_copy{
        rq::ExpressionAttribute::NO_COPY};
    rq::ExpressionAttributeConstant _ea_may_copy{
        rq::ExpressionAttribute::MAY_COPY};
    rq::ExpressionAttributeConstant _ea_unstable_address{
        rq::ExpressionAttribute::UNSTABLE_ADDRESS};
    rq::ExpressionAttributeConstant _ea_stable_address{
        rq::ExpressionAttribute::STABLE_ADDRESS};
    rq::ExpressionAttributeConstant _ea_explicit_drop{
        rq::ExpressionAttribute::EXPLICIT_DROP};
    rq::ExpressionAttributeConstant _ea_implicit_drop{
        rq::ExpressionAttribute::IMPLICIT_DROP};
    rq::ExpressionAttributeConstant _ea_not_ok{rq::ExpressionAttribute::NOT_OK};
    rq::ExpressionAttributeConstant _ea_ok{rq::ExpressionAttribute::OK};
    rq::TypeAttributeConstant _ta_none{rq::TypeAttribute::NONE};
    rq::TypeAttributeConstant _ta_constant{rq::TypeAttribute::CONSTANT};
    rq::TypeAttributeConstant _ta_var{rq::TypeAttribute::VAR};
    rq::TypeAttributeConstant _ta_partially_var{
        rq::TypeAttribute::PARTIALLY_VAR};
    rq::TypeAttributeConstant _ta_not_volatile{rq::TypeAttribute::NOT_VOLATILE};
    rq::TypeAttributeConstant _ta_volatile{rq::TypeAttribute::VOLATILE};
    rq::TypeAttributeConstant _ta_determinate{rq::TypeAttribute::DETERMINATE};
    rq::TypeAttributeConstant _ta_indeterminate{
        rq::TypeAttribute::INDETERMINATE};
    rq::TypeAttributeConstant _ta_not_atomic{rq::TypeAttribute::NOT_ATOMIC};
    rq::TypeAttributeConstant _ta_atomic{rq::TypeAttribute::ATOMIC};
    rq::TypeAttributeConstant _ta_not_null_terminated{
        rq::TypeAttribute::NOT_NULL_TERMINATED};
    rq::TypeAttributeConstant _ta_null_terminated{
        rq::TypeAttribute::NULL_TERMINATED};
    llvm::FoldingSet<rq::IntegerConstant> _integer_constant_set{};
    llvm::FoldingSet<rq::FloatConstant> _float_constant_set{};
    llvm::FoldingSet<rq::StringConstant> _string_constant_set{};
  } acquired;

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
  [[nodiscard]] rq::Module *loadImportModule(const rq::Expression &expression,
                                             llvm::StringRef import_string);
  [[nodiscard]] bool initializeLlvm();
  [[nodiscard]] bool run();
  [[nodiscard]] bool parseRequite(rq::Module &module,
                                  const std::vector<rq::Token> &tokens);
  [[nodiscard]] bool situateModule(rq::Module &module);
  [[nodiscard]] bool generateSourceModule();
  [[nodiscard]] bool buildLlvmIr();
  [[nodiscard]] bool emitTokens(llvm::StringRef path,
                                llvm::ArrayRef<rq::Token> tokens);
  [[nodiscard]] bool emitRequite(llvm::StringRef path,
                                 const rq::Expression &top);
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
  void logErrorOutsideNotInFrame(const rq::Expression &outside_expression);
  void logErrorOutsideNotAncestor(const rq::Expression &outside_expression);
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
    expression._branch_ptr = this->acquired._first_unused_expression_ptr;
    this->acquired._first_unused_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Result &acquireResult() {
    return this->acquired._result;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Out &acquireOut() {
    return this->acquired._out;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerLiteral &acquireIntegerLiteral() {
    return this->acquired._integer_literal;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatLiteral &acquireFloatLiteral() {
    return this->acquired._float_literal;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringLiteral &acquireStringLiteral() {
    return this->acquired._string_literal;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitLiteral &acquireCodeunitLiteral() {
    return this->acquired._codeunit_literal;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Inference &acquireInference() {
    return this->acquired._inference;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolReflection &
  acquireSymbolReflection() {
    return this->acquired._symbol_reflection;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Type &acquireType() {
    return this->acquired._type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Void &acquireVoid() {
    return this->acquired._void;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturn &acquireNoReturn() {
    return this->acquired._no_return;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicArguments &
  acquireVariadicArguments() {
    return this->acquired._variadic_arguments;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Boolean &acquireBoolean() {
    return this->acquired._boolean;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedConstraint &
  acquireSignedConstraint() {
    return this->acquired._signed_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedConstraint &
  acquireUnsignedConstraint() {
    return this->acquired._unsigned_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConstraint &acquireFloatConstraint() {
    return this->acquired._float_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BinaryConstraint &
  acquireBinaryConstraint() {
    return this->acquired._binary_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BfloatConstraint &
  acquireBfloatConstraint() {
    return this->acquired._bfloat_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Half &acquireHalf() {
    return this->acquired._half;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Single &acquireSingle() {
    return this->acquired._single;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Double &acquireDouble() {
    return this->acquired._double;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Quadruple &acquireQuadruple() {
    return this->acquired._quadruple;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16 &acquireBinary16() {
    return this->acquired._binary16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32 &acquireBinary32() {
    return this->acquired._binary32;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64 &acquireBinary64() {
    return this->acquired._binary64;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128 &acquireBinary128() {
    return this->acquired._binary128;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Bfloat16 &acquireBfloat16() {
    return this->acquired._bfloat16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerConstraint &
  acquireIntegerConstraint() {
    return this->acquired._integer_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedIntegerConstraint &
  acquireSignedIntegerConstraint() {
    return this->acquired._signed_integer_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedIntegerConstraint &
  acquireUnsignedIntegerConstraint() {
    return this->acquired._unsigned_integer_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitConstraint &
  acquireCodeunitConstraint() {
    return this->acquired._codeunit_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringConstraint &
  acquireStringConstraint() {
    return this->acquired._string_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterpolatedStringConstraint &
  acquireInterpolatedStringConstraint() {
    return this->acquired._interpolated_string_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Char &acquireChar() {
    return this->acquired._char;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Ascii &acquireAscii() {
    return this->acquired._ascii;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8 &acquireUtf8() {
    return this->acquired._utf8;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Visibility &acquireVisibility() {
    return this->acquired._visibility;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Scoping &acquireScoping() {
    return this->acquired._scoping;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Availability &acquireAvailability() {
    return this->acquired._availability;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Accessibility &acquireAccessibility() {
    return this->acquired._accessibility;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PropertyMutability &
  acquirePropertyMutability() {
    return this->acquired._property_mutability;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Exporting &acquireExporting() {
    return this->acquired._exporting;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Runtime &acquireRuntime() {
    return this->acquired._runtime;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Capturing &acquireCapturing() {
    return this->acquired._capturing;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EvaluationTime &acquireEvaluationTime() {
    return this->acquired._evaluation_time;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parentability &acquireParentability() {
    return this->acquired._parentability;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PropertyAssociation &
  acquirePropertyAssociation() {
    return this->acquired._property_association;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Tangibility &acquireTangibility() {
    return this->acquired._tangibility;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Overriding &acquireOverriding() {
    return this->acquired._overriding;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Inlining &acquireInlining() {
    return this->acquired._inlining;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Mangling &acquireMangling() {
    return this->acquired._mangling;
  }
  [[nodiscard]] inline rq::ExpressionAttributeConstant &
  acquireExpressionAttributeConstant(rq::ExpressionAttribute attribute) {
    using EA = rq::ExpressionAttribute;
    switch (attribute) {
    case EA::NONE:
      return this->acquired._ea_none;
    case EA::TRANSPARENT:
      return this->acquired._ea_transparent;
    case EA::OPAQUE:
      return this->acquired._ea_opaque;
    case EA::INSIDE_SCOPE:
      return this->acquired._ea_inside_scope;
    case EA::OUTSIDE_SCOPE:
      return this->acquired._ea_outside_scope;
    case EA::LOCAL:
      return this->acquired._ea_local;
    case EA::GLOBAL:
      return this->acquired._ea_global;
    case EA::PRIVATE:
      return this->acquired._ea_private;
    case EA::PUBLIC:
      return this->acquired._ea_public;
    case EA::PROTECTED:
      return this->acquired._ea_protected;
    case EA::NO_PARTIAL_MUTATE:
      return this->acquired._ea_no_partial_mutate;
    case EA::PARTIAL_MUTATE:
      return this->acquired._ea_partial_mutate;
    case EA::NO_EXPORT:
      return this->acquired._ea_no_export;
    case EA::EXPORT:
      return this->acquired._ea_export;
    case EA::DYNAMIC:
      return this->acquired._ea_dynamic;
    case EA::STATIC:
      return this->acquired._ea_static;
    case EA::NO_CAPTURE:
      return this->acquired._ea_no_capture;
    case EA::CAPTURE:
      return this->acquired._ea_capture;
    case EA::LAZY:
      return this->acquired._ea_lazy;
    case EA::EAGER:
      return this->acquired._ea_eager;
    case EA::NO_PARENT:
      return this->acquired._ea_no_parent;
    case EA::MAY_PARENT:
      return this->acquired._ea_may_parent;
    case EA::MIXIN:
      return this->acquired._ea_mixin;
    case EA::PARENT:
      return this->acquired._ea_parent;
    case EA::TANGIBLE:
      return this->acquired._ea_tangible;
    case EA::ABSTRACT:
      return this->acquired._ea_abstract;
    case EA::VIRTUAL:
      return this->acquired._ea_virtual;
    case EA::NO_OVERRIDE:
      return this->acquired._ea_no_override;
    case EA::OVERRIDE:
      return this->acquired._ea_override;
    case EA::NO_INLINE:
      return this->acquired._ea_no_inline;
    case EA::INLINE:
      return this->acquired._ea_inline;
    case EA::IMPLICIT_MANGLE:
      return this->acquired._ea_implicit_mangle;
    case EA::EXPLICIT_MANGLE:
      return this->acquired._ea_explicit_mangle;
    case EA::NO_PACK:
      return this->acquired._ea_no_pack;
    case EA::PACK:
      return this->acquired._ea_pack;
    case EA::LABEL:
      return this->acquired._ea_label;
    case EA::NO_TEMPLATE:
      return this->acquired._ea_no_template;
    case EA::TEMPLATE:
      return this->acquired._ea_template;
    case EA::SPECIALIZE:
      return this->acquired._ea_specialize;
    case EA::EQUIVOCAL:
      return this->acquired._ea_equivocal;
    case EA::LIKELY:
      return this->acquired._ea_likely;
    case EA::UNLIKELY:
      return this->acquired._ea_unlikely;
    case EA::SUPPORTED:
      return this->acquired._ea_supported;
    case EA::DEPRECIATED:
      return this->acquired._ea_depreciated;
    case EA::EXPERIMENTAL:
      return this->acquired._ea_experimental;
    case EA::NO_COPY:
      return this->acquired._ea_no_copy;
    case EA::MAY_COPY:
      return this->acquired._ea_may_copy;
    case EA::UNSTABLE_ADDRESS:
      return this->acquired._ea_unstable_address;
    case EA::STABLE_ADDRESS:
      return this->acquired._ea_stable_address;
    case EA::EXPLICIT_DROP:
      return this->acquired._ea_explicit_drop;
    case EA::IMPLICIT_DROP:
      return this->acquired._ea_implicit_drop;
    case EA::NOT_OK:
      return this->acquired._ea_not_ok;
    case EA::OK:
      return this->acquired._ea_ok;
    case EA::LAST:
      RQ_UNREACHABLE();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] inline rq::TypeAttributeConstant &
  acquireTypeAttributeConstant(rq::TypeAttribute attribute) {
    using TA = rq::TypeAttribute;
    switch (attribute) {
    case TA::NONE:
      return this->acquired._ta_none;
    case TA::CONSTANT:
      return this->acquired._ta_constant;
    case TA::VAR:
      return this->acquired._ta_var;
    case TA::PARTIALLY_VAR:
      return this->acquired._ta_partially_var;
    case TA::NOT_VOLATILE:
      return this->acquired._ta_not_volatile;
    case TA::VOLATILE:
      return this->acquired._ta_volatile;
    case TA::DETERMINATE:
      return this->acquired._ta_determinate;
    case TA::INDETERMINATE:
      return this->acquired._ta_indeterminate;
    case TA::NOT_ATOMIC:
      return this->acquired._ta_not_atomic;
    case TA::ATOMIC:
      return this->acquired._ta_atomic;
    case TA::NOT_NULL_TERMINATED:
      return this->acquired._ta_not_null_terminated;
    case TA::NULL_TERMINATED:
      return this->acquired._ta_null_terminated;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] inline rq::Symbol &acquireSynonym(rq::Symbol &original) {
    if (llvm::isa<rq::ScaledBuiltin>(original)) {
      rq::ScaledBuiltin &original_scaled =
          llvm::cast<rq::ScaledBuiltin>(original);
      const unsigned uid = this->acquired._scaled_builtin_generation++;
      llvm::FoldingSetNodeID id;
      rq::profileScaledBuiltin(id, original_scaled.getOpcode(),
                               original_scaled.getScalar(), uid,
                               original_scaled.getFlags());
      void *insert_pos = nullptr;
      rq::ScaledBuiltin &new_type =
          this->allocateAcquiredValue<rq::ScaledBuiltin>(
              original_scaled.getOpcode(), original_scaled.getScalar(),
              this->acquired._scaled_builtin_generation++,
              original_scaled.getFlags());
      this->acquired._scaled_builtin_set.InsertNode(&new_type, insert_pos);
      return llvm::cast<rq::ScaledSignedInteger>(new_type);
    } else if (llvm::isa<rq::Synonym>(original)) {
      rq::Synonym &synonym = llvm::cast<rq::Synonym>(original);
      rq::Symbol &original_original = synonym.getOriginal();
      return this->allocateAcquiredValue<rq::Synonym>(original_original);
    }
    return this->allocateAcquiredValue<rq::Synonym>(original);
  }
  [[nodiscard]] inline rq::ScaledSignedInteger &
  acquireScaledSignedInteger(unsigned scalar, rq::ScaledBuiltinFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledBuiltin(id, rq::Opcode::SY_SCALED_SIGNED_INTEGER, scalar,
                             0, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledBuiltin *existing =
            this->acquired._scaled_builtin_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::ScaledSignedInteger>(rq::dereferencePtr(existing));
    }
    rq::ScaledBuiltin &new_type =
        this->allocateAcquiredValue<rq::ScaledBuiltin>(
            rq::Opcode::SY_SCALED_SIGNED_INTEGER, scalar, 0, flags);
    this->acquired._scaled_builtin_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::ScaledSignedInteger>(new_type);
  }
  [[nodiscard]] inline rq::ScaledUnsignedInteger &
  acquireScaledUnsignedInteger(unsigned scalar, rq::ScaledBuiltinFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledBuiltin(id, rq::Opcode::SY_SCALED_UNSIGNED_INTEGER, scalar,
                             0, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledBuiltin *existing =
            this->acquired._scaled_builtin_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::ScaledUnsignedInteger>(
          rq::dereferencePtr(existing));
    }
    rq::ScaledBuiltin &new_type =
        this->allocateAcquiredValue<rq::ScaledBuiltin>(
            rq::Opcode::SY_SCALED_UNSIGNED_INTEGER, scalar, 0, flags);
    this->acquired._scaled_builtin_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::ScaledUnsignedInteger>(new_type);
  }
  [[nodiscard]] inline rq::Reference &
  acquireReference(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::Opcode::SY_REFERENCE, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::Reference>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::Opcode::SY_REFERENCE, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::Reference>(new_type);
  }
  [[nodiscard]] inline rq::Pointer &
  acquirePointer(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::Opcode::SY_POINTER, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::Pointer>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::Opcode::SY_POINTER, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::Pointer>(new_type);
  }
  [[nodiscard]] inline rq::FatPointer &
  acquireFatPointer(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::Opcode::SY_FAT_POINTER, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::FatPointer>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::Opcode::SY_FAT_POINTER, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::FatPointer>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArray &
  acquireInferencedCountArray(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::Opcode::SY_INFERENCED_COUNT_ARRAY,
                            descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::InferencedCountArray>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::Opcode::SY_INFERENCED_COUNT_ARRAY, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::InferencedCountArray>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Array &
  acquireArray(rq::TypeConstant &descendent, unsigned count) {
    llvm::FoldingSetNodeID id;
    rq::profileCountedSubtype(id, rq::Opcode::SY_ARRAY, descendent, count);
    void *insert_pos = nullptr;
    if (rq::CountedSubtype *existing =
            this->acquired._counted_subtype_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::Array>(rq::dereferencePtr(existing));
    }
    rq::CountedSubtype &new_type =
        this->allocateAcquiredValue<rq::CountedSubtype>(rq::Opcode::SY_ARRAY,
                                                        descendent, count);
    this->acquired._counted_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::Array>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticInterval &
  acquireArithmeticInterval(rq::TypeConstant &descendent,
                            rq::ArithmeticSequenceCondition condition) {
    llvm::FoldingSetNodeID id;
    rq::profileArithmeticSequence(id, descendent, condition,
                                  rq::ArithmeticSequenceStep::NONE);
    void *insert_pos = nullptr;
    if (rq::ArithmeticSequence *existing =
            this->acquired._arithmetic_sequence_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::ArithmeticInterval>(rq::dereferencePtr(existing));
    }
    rq::ArithmeticSequence &new_type =
        this->allocateAcquiredValue<rq::ArithmeticSequence>(
            rq::Opcode::SY_ARITHMETIC_INTERVAL, descendent, condition,
            rq::ArithmeticSequenceStep::NONE);
    this->acquired._arithmetic_sequence_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::ArithmeticInterval>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticProgression &
  acquireInfiniteArithmeticProgression(rq::TypeConstant &descendent,
                                       rq::ArithmeticSequenceStep step) {
    llvm::FoldingSetNodeID id;
    rq::profileArithmeticSequence(id, descendent,
                                  rq::ArithmeticSequenceCondition::NONE, step);
    void *insert_pos = nullptr;
    if (rq::ArithmeticSequence *existing =
            this->acquired._arithmetic_sequence_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::InfiniteArithmeticProgression>(
          rq::dereferencePtr(existing));
    }
    rq::ArithmeticSequence &new_type =
        this->allocateAcquiredValue<rq::ArithmeticSequence>(
            rq::Opcode::SY_INFINITE_ARITHMETIC_PROGRESSION, descendent,
            rq::ArithmeticSequenceCondition::NONE, step);
    this->acquired._arithmetic_sequence_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::InfiniteArithmeticProgression>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgression &
  acquireFiniteArithmeticProgression(rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition,
                                     rq::ArithmeticSequenceStep step) {
    llvm::FoldingSetNodeID id;
    rq::profileArithmeticSequence(id, descendent, condition, step);
    void *insert_pos = nullptr;
    if (rq::ArithmeticSequence *existing =
            this->acquired._arithmetic_sequence_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::FiniteArithmeticProgression>(
          rq::dereferencePtr(existing));
    }
    rq::ArithmeticSequence &new_type =
        this->allocateAcquiredValue<rq::ArithmeticSequence>(
            rq::Opcode::SY_FINITE_ARITHMETIC_PROGRESSION, descendent, condition,
            step);
    this->acquired._arithmetic_sequence_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::FiniteArithmeticProgression>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &
  acquireTypeConstant(rq::Symbol &symbol, rq::TypeFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileTypeConstant(id, symbol, flags);
    void *insert_pos = nullptr;
    if (rq::TypeConstant *existing =
            this->acquired._type_constant_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::TypeConstant &new_type =
        this->allocateAcquiredValue<rq::TypeConstant>(symbol, flags);
    this->acquired._type_constant_set.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionConstant &
  acquireExpressionConstant(const rq::Expression &expression) {
    llvm::FoldingSetNodeID id;
    rq::profileExpressionConstant(id, expression);
    void *insert_pos = nullptr;
    if (rq::ExpressionConstant *existing =
            this->acquired._expression_constant_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::ExpressionConstant &new_type =
        this->allocateAcquiredValue<rq::ExpressionConstant>(expression);
    this->acquired._expression_constant_set.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanConstant &
  acquireBooleanConstant(bool value) {
    if (value == true) {
      return this->acquired._true;
    }
    return this->acquired._false;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerConstant &
  acquireIntegerConstant(const llvm::APInt &int_) {
    llvm::FoldingSetNodeID id;
    rq::profileIntegerConstant(id, int_);
    void *insert_pos = nullptr;
    if (rq::IntegerConstant *existing =
            this->acquired._integer_constant_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::IntegerConstant &new_type =
        this->allocateAcquiredValue<rq::IntegerConstant>(int_);
    this->acquired._integer_constant_set.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConstant &
  acquireFloatConstant(const llvm::APFloat &float_) {
    llvm::FoldingSetNodeID id;
    rq::profileFloatConstant(id, float_);
    void *insert_pos = nullptr;
    if (rq::FloatConstant *existing =
            this->acquired._float_constant_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::FloatConstant &new_type =
        this->allocateAcquiredValue<rq::FloatConstant>(float_);
    this->acquired._float_constant_set.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringConstant &
  acquireStringConstant(llvm::StringRef string) {
    llvm::FoldingSetNodeID id;
    rq::profileStringConstant(id, string);
    void *insert_pos = nullptr;
    if (rq::StringConstant *existing =
            this->acquired._string_constant_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return rq::dereferencePtr(existing);
    }
    rq::StringConstant &new_type =
        this->allocateAcquiredValue<rq::StringConstant>(string);
    this->acquired._string_constant_set.InsertNode(&new_type, insert_pos);
    return new_type;
  }
  [[nodiscard]] rq::NullaryInstruction &
  acquireNullaryInstruction(rq::Opcode opcode);
  void discardInstruction(rq::NullaryInstruction &instruction);
  [[nodiscard]] rq::UnaryInstruction &
  acquireUnaryInstruction(rq::Opcode opcode);
  void discardInstruction(rq::UnaryInstruction &instruction);
  [[nodiscard]] rq::BinaryInstruction &
  acquireBinaryInstruction(rq::Opcode opcode);
  void discardInstruction(rq::BinaryInstruction &instruction);
  [[nodiscard]] rq::TypeConstant &acquireEntrySignature();
};

} // namespace rq
