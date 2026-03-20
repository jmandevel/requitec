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
    rq::Instruction *_first_unused_instruction_ptr{nullptr};
    rq::Result _result{};
    rq::In _in{};
    rq::Out _out{};
    rq::Inference _inference{};
    rq::SymbolConstraint _symbol_constraint{};
    rq::TypeConstraint _type_constraint{};
    rq::Void _void{};
    rq::Null _null{};
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
    rq::Char _char{};
    rq::Ascii _ascii{};
    rq::Utf8 _utf8{};
    unsigned _scaled_builtin_generation{1};
    llvm::FoldingSet<rq::ScaledBuiltin> _scaled_builtin_set{};
    llvm::FoldingSet<rq::UnarySubtype> _unary_subtype_set{};
    llvm::FoldingSet<rq::CountedSubtype> _counted_subtype_set{};
    llvm::FoldingSet<rq::ArithmeticSequence> _arithmetic_sequence_set{};
    llvm::FoldingSet<rq::TypeConstant> _type_constant_set{};
    llvm::FoldingSet<rq::ExpressionConstant> _expression_constant_set{};
    rq::BooleanConstant _true{true};
    rq::BooleanConstant _false{false};
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
  [[nodiscard]] bool tabulateModule(rq::Module &module);
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
  void logErrorGlobalIndeterminateDynamicExpression(
      const rq::Expression &expression);
  void logErrorInvalidExpressionAttribute(const rq::Expression &unascribed,
                                          const rq::Expression &attribute);
  void logErrorFailedToAscribeExpression(const rq::Expression &unascribed,
                                         const rq::Expression &attribute);
  void logErrorNotSymbol(const rq::Expression &expression);
  void logErrorNotLabel(const rq::Expression &expression);
  void logErrorLabelSubjectNotSymbolTable(const rq::Expression &expression);
  void logErrorNotInTop(const rq::Expression &expression);
  void logErrorUnableToEvaluateName(const rq::Expression &expression);
  void logErrorUnableToEvaluateUtf8Cstr(const rq::Expression &expression);
  void logErrorFailedToImportModule(const rq::Expression &expression,
                                    llvm::StringRef path);
  void logErrorOutsideNotInFrame(const rq::Expression &outside_expression);
  void logErrorOutsideNotAncestor(const rq::Expression &outside_expression);
  void logErrorNumeric(const rq::Expression &expression, rq::NumericResultCode code);
  [[nodiscard]] rq::Expression &acquireExpression();
  inline void discardExpression(rq::Expression &expression) {
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    RQ_ASSERT(!expression.getHasNext(), "has next");
    expression.clear();
    expression._branch_ptr = this->acquired._first_unused_expression_ptr;
    this->acquired._first_unused_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] rq::Instruction &acquireInstruction();
  inline void discardInstruction(rq::Instruction &instruction) {
    if (instruction.getHasHead()) {
      rq::Entity& head = instruction.popHead();
      if (llvm::isa<rq::Instruction>(head)) {
        rq::Instruction &head_instruction = llvm::cast<rq::Instruction>(head);
        this->discardInstruction(head_instruction);
      }
    }
    if (instruction.getHasTail()) {
      rq::Entity& tail = instruction.popTail();
      if (llvm::isa<rq::Instruction>(tail)) {
        rq::Instruction &tail_instruction = llvm::cast<rq::Instruction>(tail);
        this->discardInstruction(tail_instruction);
      }
    }
    instruction._head_ptr = this->acquired._first_unused_instruction_ptr;
    this->acquired._first_unused_instruction_ptr = &instruction;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Result &acquireResult() {
    return this->acquired._result;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::In &acquireIn() {
    return this->acquired._in;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Out &acquireOut() {
    return this->acquired._out;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Inference &acquireInference() {
    return this->acquired._inference;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstraint &acquireSymbolConstraint() {
    return this->acquired._symbol_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstraint &acquireTypeConstraint() {
    return this->acquired._type_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Void &acquireVoid() {
    return this->acquired._void;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Null &acquireNull() {
    return this->acquired._null;
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedConstraint &acquireSignedConstraint() {
    return this->acquired._signed_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedConstraint &acquireUnsignedConstraint() {
    return this->acquired._unsigned_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConstraint &acquireFloatConstraint() {
    return this->acquired._float_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BinaryConstraint &acquireBinaryConstraint() {
    return this->acquired._binary_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BfloatConstraint &acquireBfloatConstraint() {
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerConstraint &acquireIntegerConstraint() {
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitConstraint &acquireCodeunitConstraint() {
    return this->acquired._codeunit_constraint;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringConstraint &acquireStringConstraint() {
    return this->acquired._string_constraint;
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
  [[nodiscard]] inline rq::Symbol &acquireSynonym(rq::Symbol &original) {
    if (llvm::isa<rq::ScaledBuiltin>(original)) {
      rq::ScaledBuiltin &original_scaled =
          llvm::cast<rq::ScaledBuiltin>(original);
      const unsigned uid = this->acquired._scaled_builtin_generation++;
      llvm::FoldingSetNodeID id;
      rq::profileScaledBuiltin(id, original_scaled.getKind(),
                               original_scaled.getScalar(), uid,
                               original_scaled.getFlags());
      void *insert_pos = nullptr;
      rq::ScaledBuiltin &new_type =
          this->allocateAcquiredValue<rq::ScaledBuiltin>(
              original_scaled.getKind(), original_scaled.getScalar(),
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
    rq::profileScaledBuiltin(id, rq::EntityKind::SY_SCALED_SIGNED_INTEGER,
                             scalar, 0, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledBuiltin *existing =
            this->acquired._scaled_builtin_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::ScaledSignedInteger>(rq::dereferencePtr(existing));
    }
    rq::ScaledBuiltin &new_type =
        this->allocateAcquiredValue<rq::ScaledBuiltin>(
            rq::EntityKind::SY_SCALED_SIGNED_INTEGER, scalar, 0, flags);
    this->acquired._scaled_builtin_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::ScaledSignedInteger>(new_type);
  }
  [[nodiscard]] inline rq::ScaledUnsignedInteger &
  acquireScaledUnsignedInteger(unsigned scalar, rq::ScaledBuiltinFlags flags) {
    llvm::FoldingSetNodeID id;
    rq::profileScaledBuiltin(id, rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER,
                             scalar, 0, flags);
    void *insert_pos = nullptr;
    if (rq::ScaledBuiltin *existing =
            this->acquired._scaled_builtin_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::ScaledUnsignedInteger>(
          rq::dereferencePtr(existing));
    }
    rq::ScaledBuiltin &new_type =
        this->allocateAcquiredValue<rq::ScaledBuiltin>(
            rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER, scalar, 0, flags);
    this->acquired._scaled_builtin_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::ScaledUnsignedInteger>(new_type);
  }
  [[nodiscard]] inline rq::Reference &
  acquireReference(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::EntityKind::SY_REFERENCE, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::Reference>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::EntityKind::SY_REFERENCE, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::Reference>(new_type);
  }
  [[nodiscard]] inline rq::Pointer &
  acquirePointer(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::EntityKind::SY_POINTER, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::Pointer>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::EntityKind::SY_POINTER, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::Pointer>(new_type);
  }
  [[nodiscard]] inline rq::FatPointer &
  acquireFatPointer(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::EntityKind::SY_FAT_POINTER, descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::FatPointer>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::EntityKind::SY_FAT_POINTER, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::FatPointer>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArray &
  acquireInferencedCountArray(rq::TypeConstant &descendent) {
    llvm::FoldingSetNodeID id;
    rq::profileUnarySubtype(id, rq::EntityKind::SY_INFERENCED_COUNT_ARRAY,
                            descendent);
    void *insert_pos = nullptr;
    if (rq::UnarySubtype *existing =
            this->acquired._unary_subtype_set.FindNodeOrInsertPos(id,
                                                                  insert_pos)) {
      return llvm::cast<rq::InferencedCountArray>(rq::dereferencePtr(existing));
    }
    rq::UnarySubtype &new_type = this->allocateAcquiredValue<rq::UnarySubtype>(
        rq::EntityKind::SY_INFERENCED_COUNT_ARRAY, descendent);
    this->acquired._unary_subtype_set.InsertNode(&new_type, insert_pos);
    return llvm::cast<rq::InferencedCountArray>(new_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Array &
  acquireArray(rq::TypeConstant &descendent, unsigned count) {
    llvm::FoldingSetNodeID id;
    rq::profileCountedSubtype(id, rq::EntityKind::SY_ARRAY, descendent, count);
    void *insert_pos = nullptr;
    if (rq::CountedSubtype *existing =
            this->acquired._counted_subtype_set.FindNodeOrInsertPos(
                id, insert_pos)) {
      return llvm::cast<rq::Array>(rq::dereferencePtr(existing));
    }
    rq::CountedSubtype &new_type =
        this->allocateAcquiredValue<rq::CountedSubtype>(
            rq::EntityKind::SY_ARRAY, descendent, count);
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
            rq::EntityKind::SY_ARITHMETIC_INTERVAL, descendent, condition,
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
            rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION, descendent,
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
            rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION, descendent,
            condition, step);
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
  [[nodiscard]] rq::TypeConstant& acquireEntrySignature();
};

} // namespace rq
