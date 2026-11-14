#pragma once

#include <rq/bump_ptr_allocator.hpp>
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

namespace llvm {
struct Type;
}

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
  rq::Expression *_free_expression_ptr{nullptr};

  rq::IntegerLiteralType _integer_literal_type{};
  rq::FloatLiteralType _float_literal_type{};
  rq::StringLiteralType _string_literal_type{};
  rq::CodeunitLiteralType _codeunit_literal_type{};
  rq::UnknownType _unknown_type{};
  rq::InferenceType _inference_type{};
  rq::VoidType _void_type{};
  rq::NoReturnType _no_return_type{};
  rq::ExpressionType _expression_type{};
  rq::ExpressionRangeType _expression_range_type{};
  rq::SymbolRangeType _symbol_range_type{};
  rq::FittingFSIntType _fitting_fsint_type{};
  rq::FittingFUIntType _fitting_uint_type{};
  rq::FittingLSIntType _fitting_lsint_type{};
  rq::FittingLUIntType _fitting_luint_type{};
  rq::BooleanType _boolean_type{};
  rq::HalfType _half_type{};
  rq::SingleType _single_type{};
  rq::DoubleType _double_type{};
  rq::QuadrupleType _quadruple_type{};
  rq::SIntType _sint_type{};
  rq::UIntType _uint_type{};
  rq::SSizeType _ssize_type{};
  rq::USizeType _usize_type{};
  rq::SIndexType _sindex_type{};
  rq::UIndexType _uindex_type{};
  rq::CharType _char_type{};
  rq::Binary16Type _binary16_type{};
  rq::Binary32Type _binary32_type{};
  rq::Binary64Type _binary64_type{};
  rq::Binary128Type _binary128_type{};
  rq::BFloat16Type _bfloat16_type{};
  rq::AsciiType _ascii_type{};
  rq::Utf8Type _utf8_type{};
  llvm::FoldingSet<rq::ScaledPrimitiveType> _scaled_primitive_type_set{};
  rq::NoModifier _no_modifier{};
  rq::AnchorModifier _anchor_modifier{};
  rq::InitsModifier _inits_modifier{};
  rq::ResidentModifier _resident_modifier{};
  rq::FlankModifier _flank_modifier{};
  rq::TransparentModifier _transparent_modifier{};
  rq::OpaqueModifier _opaque_modifier{};
  rq::PrivateModifier _private_modifier{};
  rq::PublicModifier _public_modifier{};
  rq::ExportModifier _export_modifier{};
  rq::MutableModifier _mutable_modifier{};
  rq::PartiallyMutableModifier _partially_mutable_modifier{};
  rq::ConstantModifier _constant_modifier{};
  rq::EagerModifier _eager_modifier{};
  rq::LazyModifier _lazy_modifier{};
  rq::PostModifier _post_modifier{};
  rq::PreModifier _pre_modifier{};
  rq::AliasModifier _alias_modifier{};
  rq::EnumeratorModifier _enumerator_modifier{};
  rq::ConceptModifier _concept_modifier{};
  rq::SplitModifier _split_modifier{};
  rq::RuntimeModifier _runtime_modifier{};
  rq::ComtimeModifier _comtime_modifier{};
  rq::AnytimeModifier _anytime_modifier{};
  rq::PresetModifier _preset_modifier{};
  rq::SingletonModifier _singleton_modifier{};
  rq::CaptureModifier _capture_modifier{};
  rq::StatelessModifier _stateless_modifier{};
  rq::LinkedModifier _linked_modifier{};
  rq::InlineModifier _inline_modifier{};
  rq::PureModifier _pure_modifier{};
  rq::ImpureModifier _impure_modifier{};
  rq::StandardMangleModifier _standard_mangle_modifier{};
  rq::MangleModifier _mangle_modifier{};
  rq::PadModifier _pad_modifier{};
  rq::PackModifier _pack_modifier{};
  rq::EquivocalModifier _equivocal_modifier{};
  rq::LikelyModifier _likely_modifier{};
  rq::UnlikelyModifier _unlikely_modifier{};
  rq::SupportedModifier _supported_modifier{};
  rq::DepreciatedModifier _depreciated_modifier{};
  rq::ExperimentalModifier _experimental_modifier{};
  rq::MovableModifier _movable_modifier{};
  rq::CopyableModifier _copyable_modifier{};
  rq::InvariadicModifier _invariadic_modifier{};
  rq::VariadicModifier _variadic_modifier{};
  rq::BestLocationModifier _best_location_modifier{};
  rq::LocationModifier _location_modifier{};
  rq::TemplateModifier _template_modifier{};
  rq::OverloadModifier _overload_modifier{};
  rq::ConstraintModifier _constraint_modifier{};
  rq::ManualModifier _manual_modifier{};
  rq::AutoModifier _auto_modifier{};
  rq::DirectModifier _direct_modifier{};
  rq::VirtualModifier _virtual_modifier{};
  rq::RangerModifier _ranger_modifier{};
  rq::RequireModifier _require_modifier{};
  rq::EnsureModifier _ensure_modifier{};
  rq::NoMutQualifier _no_mut_qualifier{};
  rq::PartialMutQualifier _partial_mut_qualifier{};
  rq::MutQualifier _mut_qualifier{};
  rq::NoVolatileQualifier _no_volatile_qualifier{};
  rq::VolatileQualifier _volatile_qualifier{};
  rq::NoAtomicQualifier _no_atomic_qualifier{};
  rq::AtomicQualifier _atomic_qualifier{};
  rq::NoNullTerminateQualifier _no_null_terminate_qualifier{};
  rq::NullTerminateQualifier _null_terminate_qualifier{};
  rq::NoMarginQualifier _no_margin_qualifier{};
  rq::MarginQualifier _margin_qualifier{};
  rq::NoDetachQualifier _no_detach_qualifier{};
  rq::DetachQualifier _detach_qualifier{};
  rq::NoThrowsQualifier _no_throws_qualifier{};
  rq::ThrowsQualifier _throws_qualifier{};
  rq::AnyConcept _any_concept{};
  rq::TypeConcept _type_concept{};
  rq::ComtimeConcept _comtime_concept{};
  rq::RuntimeConcept _runtime_concept{};
  rq::AnytimeConcept _anytime_concept{};
  rq::RangeConcept _range_concept{};
  rq::RouteConcept _route_concept{};
  rq::FigurativeConcept _figurative_concept{};
  rq::ReflectionConcept _reflection_concept{};
  rq::PrimitiveConcept _primitive_concept{};
  rq::FittingPrimitiveConcept _fitting_primative_concept{};
  rq::PlatformPrimitiveConcept _platform_primitive_concept{};
  rq::StandardPrimitiveConcept _standard_primitive_concept{};
  rq::ScaledPrimitiveConcept _scaled_primitive_concept{};
  rq::SignedConcept _scaled_concept{};
  rq::UnsignedConcept _unsigned_concept{};
  rq::IntConcept _int_concept{};
  rq::ExactIntConcept _exact_int_concept{};
  rq::FastIntConcept _fast_int_concept{};
  rq::LeastIntConcept _least_int_concept{};
  rq::FloatConcept _float_concept{};
  rq::BinaryConcept _binary_concept{};
  rq::BFloatConcept _bfloat_concept{};
  rq::ConceptConcept _concept_concept{};
  rq::SubtypeConcept _subtype_concept{};
  rq::ArrayConcept _array_concept{};
  rq::RefConcept _ref_concept{};
  rq::PtrConcept _ptr_concept{};
  rq::SliceConcept _slice_concept{};
  rq::SplitConcept _split_concept{};
  rq::ModuleConcept _module_concept{};
  rq::ImportConcept _import_concept{};
  rq::PortalConcept _portal_concept{};
  rq::RealizationConcept _realization_concept{};
  rq::ConformityConcept _conformity_concept{};
  rq::AdaptionConcept _adaption_concept{};
  rq::TripConcept _trip_concept{};
  rq::JuxtConcept _juxt_concept{};
  rq::SpecializationSetConcept _specialization_set_concept{};
  rq::AdapterSpecializationSetConcept _adapter_specialization_set_concept{};
  rq::ProcSpecializationSetConcept _proc_specialization_set_concept{};
  rq::ArithmeticSequenceConcept _arithmetic_sequence_concept{};
  rq::ArithmeticIntervalConcept _arithmetic_interval_concept{};
  rq::InfiniteArithmeticSequenceConcept _infinite_arithmetic_sequence_concept{};
  rq::FiniteArithmeticSequenceConcept _finite_arithmetic_sequence_concept{};
  rq::EagerDeclarationConcept _eager_declaration_concept{};
  rq::AnchorConcept _anchor_concept{};
  rq::EnumeratorConcept _enumerator_concept{};
  rq::VariableConcept _variable_concept{};
  rq::EagerVariableConcept _eager_variable_concept{};
  rq::PostEagerVariableConcept _post_eager_variable_concept{};
  rq::PreEagerVariableConcept _pre_eager_variable_concept{};
  rq::ParameterConcept _parameter_concept{};
  rq::SignatureConcept _signature_concept{};
  rq::LayoutConcept _layout_concept{};
  rq::PlacementConcept _placement_concept{};
  rq::CompositionConcept _composition_concept{};
  rq::SynonymConcept _synonym_concept{};
  rq::SymbolTableConcept _symbol_table_concept{};
  rq::NamespaceConcept _namespace_concept{};
  rq::EagerScopingStatementConcept _eager_scoping_statement_concept{};
  rq::IfStatementConcept _if_statement_concept{};
  rq::ElseIfStatementConcept _else_if_statement_concept{};
  rq::ElseStatementConcept _else_statement_concept{};
  rq::SwitchStatementConcept _switch_statement_concept{};
  rq::CaseStatementConcept _case_statement_concept{};
  rq::ForStatementConcept _for_statement_concept{};
  rq::WhileStatementConcept _while_statement_concept{};
  rq::ScopeStatementConcept _scope_statement_concept{};
  rq::ImplementationConcept _implementation_concept{};
  rq::OverloadConcept _overload_concept{};
  rq::SpecializationConcept _specialization_concept{};
  rq::TemplateConcept _template_concept{};
  rq::ClassImplementationConcept _class_implementation_concept{};
  rq::ClassOverloadConcept _class_overload_concept{};
  rq::ClassSpecializationConcept _class_specialization_concept{};
  rq::ClassTemplateConcept _class_template_concept{};
  rq::EnumImplementationConcept _enum_implementation_concept{};
  rq::EnumOverloadConcept _enum_overload_concept{};
  rq::InterfaceImplementationConcept _interface_implementation_concept{};
  rq::InterfaceOverloadConcept _interface_overload_concept{};
  rq::InterfaceSpecializationConcept _interface_specialization_concept{};
  rq::AdapterImplementationConcept _adapter_implementation_concept{};
  rq::AdapterOverloadConcept _adapter_overload_concept{};
  rq::AdapterSpecializationConcept _adapter_specialization_concept{};
  rq::AdapterTemplateConcept _adapter_template_concept{};
  rq::ProcImplementationConcept _proc_implementation_concept{};
  rq::ProcOverloadConcept _proc_overload_concept{};
  rq::ProcSpecializationConcept _proc_specialization_concept{};
  rq::ProcTemplateConcept _proc_template_concept{};
  rq::LazyVariableImplementationConcept _lazy_variable_implementation_concept{};
  rq::LazyVariableSpecializationConcept _lazy_variable_specialization_concept{};
  rq::LazyVariableTemplateConcept _lazy_variable_template_concept{};
  rq::AliasImplementationConcept _alias_implementation_concept{};
  rq::AliasOverloadConcept _alias_overload_concept{};
  rq::AliasSpecializationConcept _alias_specialization_concept{};
  rq::AliasTemplateConcept _alias_template_concept{};
  rq::ConceptImplementationConcept _concept_implementation_concept{};
  rq::VariantImplementationConcept _variant_implementation_concept{};
  rq::VariantOverloadConcept _variant_overload_concept{};
  rq::VariantSpecializationConcept _variant_specialization_concept{};
  rq::VariantTemplateConcept _variant_template_concept{};
  rq::AttributeConcept _attribute_concept{};
  rq::ModifierConcept _modifier_concept{};
  rq::AnchorModifierConcept _anchor_modifier_concept{};
  rq::ForkedInitializationModifierConcept
      _forked_intialization_modifier_concept{};
  rq::ContainerModifierConcept _container_modifier_concept{};
  rq::VisibilityModifierConcept _visibility_modifier_concept{};
  rq::AccessModifierConcept _access_modifier_concept{};
  rq::MutabilityModifierConcept _mutability_modifier_concept{};
  rq::EvaluationTimeModifierConcept _evaluation_time_modifier_concept{};
  rq::DialectModifierConcept _dialect_modifier_concept{};
  rq::TenureModifierConcept _tenure_modifier_concept{};
  rq::InitializationTimeModifierConcept _intialization_time_modifier_concept{};
  rq::ClosureModifierConcept _closure_modifier_concept{};
  rq::LinkageModifierConcept _linkage_modifier_concept{};
  rq::SideEffectsModifierConcept _side_effects_modifier_concept{};
  rq::MangleModifierConcept _mangle_modifier_concept{};
  rq::PackModifierConcept _pack_modifier_concept{};
  rq::BranchTrendModifierConcept _branch_trend_modifier_concept{};
  rq::SupportNoticeModifierConcept _support_notice_modifier_concept{};
  rq::MovableModifierConcept _movable_modifier_concept{};
  rq::CopyableModifierConcept _copyable_modifier_concept{};
  rq::VariadicModifierConcept _variadic_modifier_concept{};
  rq::OffsetModifierConcept _offset_modifier_concept{};
  rq::LazyDeclarationModifierConcept _lazy_declaration_modifier_concept{};
  rq::ConstraintModifierConcept _constraint_modifier_concept{};
  rq::DeductionModifierConcept _deduction_modifier_concept{};
  rq::VirtualityModifierConcept _virtuality_modifier_concept{};
  rq::RangerModifierConcept _ranger_modifier_concept{};
  rq::RequireModifierConcept _require_modifier_concept{};
  rq::EnsureModifierConcept _ensure_modifier_concept{};
  rq::QualifierConcept _qualifier_concept{};
  rq::MutQualifierConcept _mut_qualifier_concept{};
  rq::VolatileQualifierConcept _volatile_qualifier_concept{};
  rq::AtomicQualifierConcept _atomic_qualifier_concept{};
  rq::NullTerminateQualifierConcept _null_terminate_qualifier_concept{};
  rq::DetachQualifierConcept _detach_qualifier_concept{};
  rq::MarginQualifierConcept _margin_qualifier_concept{};
  rq::ThrowsQualifierConcept _throws_qualifier_concept{};
  llvm::FoldingSet<rq::QualifiedType> _qualified_type_set{};
  llvm::FoldingSet<rq::ArraySubtype> _array_set{};
  llvm::FoldingSet<rq::SimpleSubtype> _simple_subtype_set{};
  llvm::FoldingSet<rq::GenericSubsymbol> _generic_subsymbol_set{};
  llvm::FoldingSet<rq::Adaption> _adaption_set{};
  llvm::FoldingSet<rq::Realization> _realization_set{};
  llvm::FoldingSet<rq::Conformity> _conformity_set{};
  llvm::FoldingSet<rq::JuxtListType> _juxt_list_type_set{};
  llvm::FoldingSet<rq::Trip> _trip_set{};
  llvm::FoldingSet<rq::ArithmeticSequenceType> _arithmetic_sequence_type_set{};
  llvm::FoldingSet<rq::SpecializationSetArgument>
      _specialization_set_argument{};
  llvm::FoldingSet<rq::SpecializationSet> _specialization_set{};
  llvm::FoldingSet<rq::ProcSpecializationSet> _proc_specialization_set{};
  llvm::FoldingSet<rq::AdapterSpecializationSet> _adapter_specialization_set{};
  llvm::FoldingSet<rq::Parameter> _parameter_set{};
  llvm::FoldingSet<rq::CompositionType> _composition_type_set{};
  llvm::FoldingSet<rq::SignatureType> _signature_type_set{};
  llvm::FoldingSet<rq::LayoutType> _layout_type_set{};
  llvm::FoldingSet<rq::PlacementType> _placement_type_set{};

  Context(std::string &&executable_path)
      : _executable_path(std::move(executable_path)) {}
  Context(const Self &) = delete;
  Context(Self &&) = delete;
  inline ~Context() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
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
  [[nodiscard]] bool validateSourceText(const rq::ModuleDetail &detail);
  [[nodiscard]] bool tokenizeSourceText(rq::ModuleDetail &detail);
  [[nodiscard]] bool parseRequite(rq::ModuleDetail &detail);
  [[nodiscard]] bool situateModule(rq::ModuleDetail &detail);
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
  [[nodiscard]] llvm::Type *getLlvmTypePtr(rq::Symbol &symbol);
  [[nodiscard]] unsigned getByteDepth() const;
  [[nodiscard]] unsigned getDefaultIntegerDepth() const;
  [[nodiscard]] unsigned getDepth(rq::Symbol &symbol);
  [[nodiscard]] bool getIsSret(rq::Symbol &symbol);
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
  // source file errors
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
  // parse errors
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
  void logErrorMustHaveParameterMark(rq::Situation situation,
                                     const rq::Expression &expression);
  void logErrorIsFirst(const rq::Expression &mark);
  void logErrorIsLast(const rq::Expression &mark);
  void logErrorExpectedCommaSeparator(const rq::Expression &expression);
  void logErrorExpectedSeparatorOrRightBracket(const rq::Token &token);
  void logErrorExpectedSemicolonSeparator(const rq::Expression &expression);
  void logErrorExpressionShouldNeverOccur(const rq::Expression &expression);
  void logErrorDuplicateParameterMark(const rq::Expression &mark);
  void logErrorDuplicateModifier(const rq::Expression &modifier);
  void logErrorNonpositionalBeginAfterPositionalEnd(
      const rq::Expression &named_begin);
  void
  logErrorNonpositionalBeginAfterLockedBegin(const rq::Expression &named_begin);
  void logErrorPositionalEndAfterLockedBegin(const rq::Expression &named_begin);
  // situator errors
  void logErrorNotExactBranchCount(rq::Situation situation,
                                   const rq::Expression &expression,
                                   unsigned count);
  void logErrorNotAtLeastBranchCount(rq::Situation situation,
                                     const rq::Expression &expression,
                                     unsigned count);
  void logErrorTooManyBranchCount(rq::Situation situation,
                                  const rq::Expression &expression,
                                  unsigned max_count);
  void logErrorInvalidBranchSituation(rq::Situation situation,
                                      const rq::Expression &branch);
  void logErrorExpectedTagBranch(const rq::Expression &expresison);
  void logErrorExpectedTagExpression(const rq::Expression &expresison);
  // generation errors
  void logErrorExpectedStatementExpression(const rq::Expression &expresison);
  void logErrorNotDeterminateComtimeValue(const rq::Expression &expression);
  void logErrorInvalidModifier(const rq::Expression &unascribed,
                               const rq::Expression &instantiation_ex,
                               rq::Modifier modifier);
  void logErrorFailedToAscribeExpression(const rq::Expression &unascribed,
                                         const rq::Expression &modifier);
  void logErrorNotSymbol(const rq::Expression &expression);
  void logErrorNotLabel(const rq::Expression &expression);
  void logErrorNotInTop(const rq::Expression &expression);
  void logErrorUnableToEvaluateName(const rq::Expression &expression);
  void logErrorUnableToEvaluateUtf8Cstr(const rq::Expression &expression);
  void logErrorFailedToImportModule(const rq::Expression &expression,
                                    llvm::StringRef path);
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
  getLlvmFltSemantics(rq::SymbolKind kind);

  [[nodiscard]] rq::Expression &acquireExpression();
  inline void discardExpression(rq::Expression &expression) {
    RQ_ASSERT(!expression.getHasBranch(), "has branch");
    RQ_ASSERT(!expression.getHasNext(), "has next");
    expression.clearData();
    expression._branch_ptr = this->_free_expression_ptr;
    this->_free_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] rq::Module &acquireModule(rq::ModuleDetail &&detail);
};

} // namespace rq
