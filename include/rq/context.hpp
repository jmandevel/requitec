#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/symbols.hpp>
#include <rq/utility.hpp>
#include <rq/values.hpp>

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

  llvm::FoldingSet<rq::ConstantInt> _constant_int_set{};
  llvm::FoldingSet<rq::ConstantFloat> _constant_float_set{};
  llvm::FoldingSet<rq::ConstantDetachedArrayElement>
      _constant_detached_array_element_set{};
  llvm::FoldingSet<rq::ConstantDetachedArray> _constant_detached_array_set{};

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
  // Singleton symbols.
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerLiteralType &
  acquireIntegerLiteralType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatLiteralType &
  acquireFloatLiteralType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StringLiteralType &
  acquireStringLiteralType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitLiteralType &
  acquireCodeunitLiteralType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnknownType &acquireUnknownType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceType &acquireInferenceType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VoidType &acquireVoidType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnType &acquireNoReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionType &acquireExpressionType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionRangeType &
  acquireExpressionRangeType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolRangeType &acquireSymbolRangeType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FittingFSIntType &
  acquireFittingFSIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FittingFUIntType &
  acquireFittingFUIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FittingLSIntType &
  acquireFittingLSIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FittingLUIntType &
  acquireFittingLUIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanType &acquireBooleanType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::HalfType &acquireHalfType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SingleType &acquireSingleType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DoubleType &acquireDoubleType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QuadrupleType &acquireQuadrupleType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SIntType &acquireSIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UIntType &acquireUIntType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SSizeType &acquireSSizeType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::USizeType &acquireUSizeType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SIndexType &acquireSIndexType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UIndexType &acquireUIndexType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CharType &acquireCharType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Type &acquireBinary16Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Type &acquireBinary32Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Type &acquireBinary64Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Type &acquireBinary128Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BFloat16Type &acquireBFloat16Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AsciiType &acquireAsciiType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Type &acquireUtf8Type();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoModifier &acquireNoModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorModifier &acquireAnchorModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InitsModifier &acquireInitsModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ResidentModifier &
  acquireResidentModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FlankModifier &acquireFlankModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TransparentModifier &
  acquireTransparentModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OpaqueModifier &acquireOpaqueModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PrivateModifier &acquirePrivateModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PublicModifier &acquirePublicModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExportModifier &acquireExportModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutableModifier &acquireMutableModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PartiallyMutableModifier &
  acquirePartiallyMutableModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantModifier &
  acquireConstantModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerModifier &acquireEagerModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyModifier &acquireLazyModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PostModifier &acquirePostModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PreModifier &acquirePreModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AliasModifier &acquireAliasModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumeratorModifier &
  acquireEnumeratorModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptModifier &acquireConceptModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SplitModifier &acquireSplitModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RuntimeModifier &acquireRuntimeModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ComtimeModifier &acquireComtimeModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnytimeModifier &acquireAnytimeModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PresetModifier &acquirePresetModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SingletonModifier &
  acquireSingletonModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CaptureModifier &acquireCaptureModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StatelessModifier &
  acquireStatelessModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LinkedModifier &acquireLinkedModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InlineModifier &acquireInlineModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PureModifier &acquirePureModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ImpureModifier &acquireImpureModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StandardMangleModifier &
  acquireStandardMangleModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MangleModifier &acquireMangleModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PadModifier &acquirePadModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PackModifier &acquirePackModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EquivocalModifier &
  acquireEquivocalModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LikelyModifier &acquireLikelyModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnlikelyModifier &
  acquireUnlikelyModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SupportedModifier &
  acquireSupportedModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DepreciatedModifier &
  acquireDepreciatedModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExperimentalModifier &
  acquireExperimentalModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MovableModifier &acquireMovableModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CopyableModifier &
  acquireCopyableModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InvariadicModifier &
  acquireInvariadicModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicModifier &
  acquireVariadicModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BestLocationModifier &
  acquireBestLocationModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocationModifier &
  acquireLocationModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateModifier &
  acquireTemplateModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OverloadModifier &
  acquireOverloadModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintModifier &
  acquireConstraintModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ManualModifier &acquireManualModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AutoModifier &acquireAutoModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DirectModifier &acquireDirectModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VirtualModifier &acquireVirtualModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RangerModifier &acquireRangerModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RequireModifier &acquireRequireModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureModifier &acquireEnsureModifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoMutQualifier &acquireNoMutQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PartialMutQualifier &
  acquirePartialMutQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutQualifier &acquireMutQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoVolatileQualifier &
  acquireNoVolatileQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileQualifier &
  acquireVolatileQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoAtomicQualifier &
  acquireNoAtomicQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicQualifier &acquireAtomicQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoNullTerminateQualifier &
  acquireNoNullTerminateQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateQualifier &
  acquireNullTerminateQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoMarginQualifier &
  acquireNoMarginQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MarginQualifier &acquireMarginQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoDetachQualifier &
  acquireNoDetachQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DetachQualifier &acquireDetachQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoThrowsQualifier &
  acquireNoThrowsQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ThrowsQualifier &acquireThrowsQualifier();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnyConcept &acquireAnyConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConcept &acquireTypeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ComtimeConcept &acquireComtimeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RuntimeConcept &acquireRuntimeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnytimeConcept &acquireAnytimeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RangeConcept &acquireRangeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RouteConcept &acquireRouteConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FigurativeConcept &
  acquireFigurativeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ReflectionConcept &
  acquireReflectionConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PrimitiveConcept &
  acquirePrimitiveConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FittingPrimitiveConcept &
  acquireFittingPrimitiveConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PlatformPrimitiveConcept &
  acquirePlatformPrimitiveConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StandardPrimitiveConcept &
  acquireStandardPrimitiveConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledPrimitiveConcept &
  acquireScaledPrimitiveConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedConcept &acquireSignedConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedConcept &acquireUnsignedConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IntConcept &acquireIntConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExactIntConcept &acquireExactIntConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FastIntConcept &acquireFastIntConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LeastIntConcept &acquireLeastIntConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConcept &acquireFloatConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BinaryConcept &acquireBinaryConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BFloatConcept &acquireBFloatConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptConcept &acquireConceptConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SubtypeConcept &acquireSubtypeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArrayConcept &acquireArrayConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RefConcept &acquireRefConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PtrConcept &acquirePtrConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SliceConcept &acquireSliceConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SplitConcept &acquireSplitConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleConcept &acquireModuleConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ImportConcept &acquireImportConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PortalConcept &acquirePortalConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RealizationConcept &
  acquireRealizationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConformityConcept &
  acquireConformityConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdaptionConcept &acquireAdaptionConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TripConcept &acquireTripConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::JuxtConcept &acquireJuxtConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SpecializationSetConcept &
  acquireSpecializationSetConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterSpecializationSetConcept &
  acquireAdapterSpecializationSetConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcSpecializationSetConcept &
  acquireProcSpecializationSetConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceConcept &
  acquireArithmeticSequenceConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticIntervalConcept &
  acquireArithmeticIntervalConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticSequenceConcept &
  acquireInfiniteArithmeticSequenceConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticSequenceConcept &
  acquireFiniteArithmeticSequenceConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerDeclarationConcept &
  acquireEagerDeclarationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorConcept &acquireAnchorConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumeratorConcept &
  acquireEnumeratorConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariableConcept &acquireVariableConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerVariableConcept &
  acquireEagerVariableConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PostEagerVariableConcept &
  acquirePostEagerVariableConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PreEagerVariableConcept &
  acquirePreEagerVariableConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterConcept &
  acquireParameterConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureConcept &
  acquireSignatureConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutConcept &acquireLayoutConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PlacementConcept &
  acquirePlacementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionConcept &
  acquireCompositionConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SynonymConcept &acquireSynonymConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableConcept &
  acquireSymbolTableConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NamespaceConcept &
  acquireNamespaceConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScopingStatementConcept &
  acquireEagerScopingStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::IfStatementConcept &
  acquireIfStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ElseIfStatementConcept &
  acquireElseIfStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ElseStatementConcept &
  acquireElseStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SwitchStatementConcept &
  acquireSwitchStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CaseStatementConcept &
  acquireCaseStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ForStatementConcept &
  acquireForStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WhileStatementConcept &
  acquireWhileStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeStatementConcept &
  acquireScopeStatementConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ImplementationConcept &
  acquireImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OverloadConcept &acquireOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SpecializationConcept &
  acquireSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateConcept &acquireTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassImplementationConcept &
  acquireClassImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassOverloadConcept &
  acquireClassOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassSpecializationConcept &
  acquireClassSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassTemplateConcept &
  acquireClassTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumImplementationConcept &
  acquireEnumImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumOverloadConcept &
  acquireEnumOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementationConcept &
  acquireInterfaceImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceOverloadConcept &
  acquireInterfaceOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceSpecializationConcept &
  acquireInterfaceSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementationConcept &
  acquireAdapterImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterOverloadConcept &
  acquireAdapterOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterSpecializationConcept &
  acquireAdapterSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterTemplateConcept &
  acquireAdapterTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcImplementationConcept &
  acquireProcImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcOverloadConcept &
  acquireProcOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcSpecializationConcept &
  acquireProcSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcTemplateConcept &
  acquireProcTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableImplementationConcept &
  acquireLazyVariableImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableSpecializationConcept &
  acquireLazyVariableSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableTemplateConcept &
  acquireLazyVariableTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AliasImplementationConcept &
  acquireAliasImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AliasOverloadConcept &
  acquireAliasOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AliasSpecializationConcept &
  acquireAliasSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AliasTemplateConcept &
  acquireAliasTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptImplementationConcept &
  acquireConceptImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariantImplementationConcept &
  acquireVariantImplementationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariantOverloadConcept &
  acquireVariantOverloadConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariantSpecializationConcept &
  acquireVariantSpecializationConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariantTemplateConcept &
  acquireVariantTemplateConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AttributeConcept &
  acquireAttributeConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierConcept &acquireModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorModifierConcept &
  acquireAnchorModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ContainerModifierConcept &
  acquireContainerModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VisibilityModifierConcept &
  acquireVisibilityModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AccessModifierConcept &
  acquireAccessModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutabilityModifierConcept &
  acquireMutabilityModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EvaluationTimeModifierConcept &
  acquireEvaluationTimeModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DialectModifierConcept &
  acquireDialectModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TenureModifierConcept &
  acquireTenureModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InitializationTimeModifierConcept &
  acquireInitializationTimeModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClosureModifierConcept &
  acquireClosureModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LinkageModifierConcept &
  acquireLinkageModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SideEffectsModifierConcept &
  acquireSideEffectsModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MangleModifierConcept &
  acquireMangleModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PackModifierConcept &
  acquirePackModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BranchTrendModifierConcept &
  acquireBranchTrendModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SupportNoticeModifierConcept &
  acquireSupportNoticeModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MovableModifierConcept &
  acquireMovableModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CopyableModifierConcept &
  acquireCopyableModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicModifierConcept &
  acquireVariadicModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OffsetModifierConcept &
  acquireOffsetModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyDeclarationModifierConcept &
  acquireLazyDeclarationModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintModifierConcept &
  acquireConstraintModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DeductionModifierConcept &
  acquireDeductionModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VirtualityModifierConcept &
  acquireVirtualityModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RangerModifierConcept &
  acquireRangerModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RequireModifierConcept &
  acquireRequireModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureModifierConcept &
  acquireEnsureModifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifierConcept &
  acquireQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutQualifierConcept &
  acquireMutQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileQualifierConcept &
  acquireVolatileQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicQualifierConcept &
  acquireAtomicQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateQualifierConcept &
  acquireNullTerminateQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::DetachQualifierConcept &
  acquireDetachQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MarginQualifierConcept &
  acquireMarginQualifierConcept();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ThrowsQualifierConcept &
  acquireThrowsQualifierConcept();

  // Structurally unique symbols.
  [[nodiscard]] inline rq::ScaledSIntType &
  acquireScaledSIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledUIntType &
  acquireScaledUIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledFSIntType &
  acquireScaledFSIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledFUIntType &
  acquireScaledFUIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledLSIntType &
  acquireScaledLSIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledLUIntType &
  acquireScaledLUIntType(unsigned depth);
  [[nodiscard]] inline rq::ScaledPrimitiveType &
  acquireScaledPrimitiveType(rq::SymbolKind kind, unsigned depth);
  [[nodiscard]] inline rq::RefSubtype &
  acquireRefSubtype(rq::QualifiedType &child);
  [[nodiscard]] inline rq::PtrSubtype &
  acquirePtrSubtype(rq::QualifiedType &child);
  [[nodiscard]] inline rq::SliceSubtype &
  acquireSliceSubtype(rq::QualifiedType &child);
  [[nodiscard]] inline rq::SplitSubtype &
  acquireSplitSubtype(rq::QualifiedType &child);
  [[nodiscard]] inline rq::InferenceCountArraySubtype &
  acquireInferenceCountArraySubtype(rq::QualifiedType &child);
  [[nodiscard]] inline rq::SimpleSubtype &
  acquireSimpleSubtype(rq::SymbolKind kind, rq::QualifiedType &child);
  [[nodiscard]] inline rq::GenericSubsymbol &
  acquireGenericSubsymbol(rq::Symbol &child);
  [[nodiscard]] inline rq::Subsymbol &acquireSubsymbol(rq::SymbolKind kind,
                                                       rq::Symbol &child);
  [[nodiscard]] inline rq::ArithmeticIntervalType &
  acquireArithmeticIntervalType(rq::QualifiedType &child,
                                rq::ArithmeticCondition condition,
                                rq::ArithmeticStep step);
  [[nodiscard]] inline rq::InfiniteArithmeticSequenceType &
  acquireInfiniteArithmeticSequenceType(rq::QualifiedType &child,
                                        rq::ArithmeticCondition condition,
                                        rq::ArithmeticStep step);
  [[nodiscard]] inline rq::FiniteArithmeticSequenceType &
  acquireFiniteArithmeticSequenceType(rq::QualifiedType &child,
                                      rq::ArithmeticCondition condition,
                                      rq::ArithmeticStep step);
  [[nodiscard]] inline rq::ArithmeticSequenceType &
  acquireArithmeticSequenceType(rq::SymbolKind kind, rq::QualifiedType &child,
                                rq::ArithmeticCondition condition,
                                rq::ArithmeticStep step);
  [[nodiscard]] inline rq::QualifiedType &
  acquireQualifiedType(rq::Symbol &type, rq::QualifierFuseFlags qualifiers,
                       unsigned margin);
  [[nodiscard]] inline rq::ArraySubtype &
  acquireArraySubtype(rq::QualifiedType &child, std::size_t count);
  [[nodiscard]] inline rq::Adaption &
  acquireAdaption(rq::QualifiedType &reciever,
                  rq::InterfaceImplementation &interface,
                  rq::AdapterImplementation &adapter);
  [[nodiscard]] inline rq::Realization &
  acquireRealization(rq::QualifiedType &reciever,
                     rq::InterfaceImplementation &interface);
  [[nodiscard]] inline rq::Conformity &
  acquireConformity(rq::InterfaceImplementation &interface,
                    rq::AdapterImplementation &adapter);
  [[nodiscard]] inline rq::JuxtListType &
  acquireJuxtListType(rq::NodeList<rq::QualifiedType> &&type_list);
  [[nodiscard]] inline rq::Trip &
  acquireTrip(llvm::SmallVector<rq::Entity *, 8> &entities);
  [[nodiscard]] inline rq::SpecializationSetArgument &
  acquireSpecializationSetArgument(rq::Name name, rq::Entity &value,
                                   rq::SpecializationSetArgument *next_ptr);
  [[nodiscard]] inline rq::SpecializationSet &
  acquireSpecializationSet(rq::SymbolKind kind,
                           rq::SpecializationSetArgument *first_ptr);
  [[nodiscard]] inline rq::ProcSpecializationSet &
  acquireProcSpecializationSet(rq::SpecializationSetArgument *first_ptr);
  [[nodiscard]] inline rq::AdapterSpecializationSet &
  acquireAdapterSpecializationSet(rq::SpecializationSetArgument *first_ptr);
  [[nodiscard]] inline rq::Parameter &
  acquireParameter(rq::Parameter *next_ptr, rq::Name name,
                   rq::QualifiedType &type,
                   rq::ModifierFuseFlags modifier_flags,
                   rq::ParameterInfoFlags param_flags, rq::Entity *default_ptr);
  [[nodiscard]] inline rq::CompositionType &acquireCompositionType(
      rq::NodeList<rq::InterfaceImplementation> &&interface_list);
  [[nodiscard]] inline rq::SignatureType &
  acquireSignatureType(rq::Parameter *first_parameter_ptr,
                       rq::QualifiedType &return_type);
  [[nodiscard]] inline rq::LayoutType &
  acquireLayoutType(rq::Parameter *first_parameter_ptr);
  [[nodiscard]] inline rq::PlacementType &
  acquirePlacementType(rq::ProcImplementation &function);

  // Each declaration, scope, overload, specialization and template has
  // identity.
  [[nodiscard]] inline rq::PreEagerVariable &acquirePrequitePreEagerVariable();
  [[nodiscard]] inline rq::PreEagerVariable &acquirePostquitePreEagerVariable();
  [[nodiscard]] inline rq::PostEagerVariable &
  acquirePrequitePostEagerVariable();
  [[nodiscard]] inline rq::PostEagerVariable &
  acquirePostquitePostEagerVariable();
  [[nodiscard]] inline rq::LazyVariableOverload &acquireLazyVariableOverload();
  [[nodiscard]] inline rq::LazyVariableSpecialization &
  acquireLazyVariableSpecialization();
  [[nodiscard]] inline rq::EagerVariable &
  acquirePrequiteEagerVariable(rq::SymbolKind kind);
  [[nodiscard]] inline rq::EagerScope &
  acquirePrequiteEagerScope(rq::SymbolKind kind);
  [[nodiscard]] inline rq::EagerVariable &
  acquirePostquiteEagerVariable(rq::SymbolKind kind);
  [[nodiscard]] inline rq::EagerScope &
  acquirePostquiteEagerScope(rq::SymbolKind kind);
  [[nodiscard]] inline rq::Import &acquireImport();
  [[nodiscard]] inline rq::Anchor &acquireAnchor();
  [[nodiscard]] inline rq::Enumerator &acquireEnumerator();
  [[nodiscard]] inline rq::Portal &acquirePortal();
  [[nodiscard]] inline rq::CTable &acquireCTable();
  [[nodiscard]] inline rq::ScopeStatement &acquirePrequiteScopeStatement();
  [[nodiscard]] inline rq::ScopeStatement &acquirePostquiteScopeStatement();
  [[nodiscard]] inline rq::WhileStatement &acquirePrequiteWhileStatement();
  [[nodiscard]] inline rq::WhileStatement &acquirePostquiteWhileStatement();
  [[nodiscard]] inline rq::ForStatement &acquirePrequiteForStatement();
  [[nodiscard]] inline rq::ForStatement &acquirePostquiteForStatement();
  [[nodiscard]] inline rq::CaseStatement &acquirePrequiteCaseStatement();
  [[nodiscard]] inline rq::CaseStatement &acquirePostquiteCaseStatement();
  [[nodiscard]] inline rq::SwitchStatement &acquirePrequiteSwitchStatement();
  [[nodiscard]] inline rq::SwitchStatement &acquirePostquiteSwitchStatement();
  [[nodiscard]] inline rq::IfStatement &acquirePrequiteIfStatement();
  [[nodiscard]] inline rq::IfStatement &acquirePostquiteIfStatement();
  [[nodiscard]] inline rq::ElseIfStatement &acquirePrequiteElseIfStatement();
  [[nodiscard]] inline rq::ElseIfStatement &acquirePostquiteElseIfStatement();
  [[nodiscard]] inline rq::ElseStatement &acquirePrequiteElseStatement();
  [[nodiscard]] inline rq::ElseStatement &acquirePostquiteElseStatement();
  [[nodiscard]] inline rq::Namespace &acquireNamespace();
  [[nodiscard]] inline rq::EnumOverload &acquireEnumOverload();
  [[nodiscard]] inline rq::EnumSpecialization &acquireEnumSpecialization();
  [[nodiscard]] inline rq::AdapterOverload &acquireAdapterOverload();
  [[nodiscard]] inline rq::AdapterSpecialization &
  acquireAdapterSpecialization();
  [[nodiscard]] inline rq::InterfaceOverload &acquireInterfaceOverload();
  [[nodiscard]] inline rq::InterfaceSpecialization &
  acquireInterfaceSpecialization();
  [[nodiscard]] inline rq::ClassOverload &acquireClassOverload();
  [[nodiscard]] inline rq::ClassSpecialization &acquireClassSpecialization();
  [[nodiscard]] inline rq::ProcOverload &acquireProcOverload();
  [[nodiscard]] inline rq::ProcSpecialization &acquireProcSpecialization();
  [[nodiscard]] inline rq::AliasOverload &acquireAliasOverload();
  [[nodiscard]] inline rq::AliasSpecialization &acquireAliasSpecialization();
  [[nodiscard]] inline rq::ConceptOverload &acquireConceptOverload();
  [[nodiscard]] inline rq::ConceptSpecialization &
  acquireConceptSpecialization();
  [[nodiscard]] inline rq::VariantOverload &acquireVariantOverload();
  [[nodiscard]] inline rq::VariantSpecialization &
  acquireVariantSpecialization();
  [[nodiscard]] inline rq::EnumTemplate &acquireEnumTemplate();
  [[nodiscard]] inline rq::ClassTemplate &acquireClassTemplate();
  [[nodiscard]] inline rq::InterfaceTemplate &acquireInterfaceTemplate();
  [[nodiscard]] inline rq::LazyVariableTemplate &acquireLazyVariableTemplate();
  [[nodiscard]] inline rq::AdapterTemplate &acquireAdapterTemplate();
  [[nodiscard]] inline rq::ProcTemplate &acquireProcTemplate();
  [[nodiscard]] inline rq::AliasTemplate &acquireAliasTemplate();
  [[nodiscard]] inline rq::ConceptTemplate &acquireConceptTemplate();
  [[nodiscard]] inline rq::VariantTemplate &acquireVariantTemplate();
  [[nodiscard]] inline rq::ClassPolymorph &acquireClassPolymorph();
  [[nodiscard]] inline rq::EnumPolymorph &acquireEnumPolymorph();
  [[nodiscard]] inline rq::InterfacePolymorph &acquireInterfacePolymorph();
  [[nodiscard]] inline rq::AdapterPolymorph &acquireAdapterPolymorph();
  [[nodiscard]] inline rq::ProcPolymorph &acquireProcPolymorph();
  [[nodiscard]] inline rq::LazyVariablePolymorph &
  acquireLazyVariablePolymorph();
  [[nodiscard]] inline rq::AliasPolymorph &acquireAliasPolymorph();
  [[nodiscard]] inline rq::VariantPolymorph &acquireVariantPolymorph();
  [[nodiscard]] inline rq::ConceptPolymorph &acquireConceptPolymorph();
  [[nodiscard]] inline rq::SynonymType &
  acquireSynonymType(rq::Symbol &original);
  [[nodiscard]] inline rq::Module &acquireModule(rq::ModuleDetail &&detail);

  // Constant values are interned; mutable values are collectible.
  [[nodiscard]] inline rq::ConstantInt &
  acquireConstantInt(rq::QualifiedType &type, const llvm::APInt &value);
  [[nodiscard]] inline rq::ConstantFloat &
  acquireConstantFloat(rq::QualifiedType &type, const llvm::APFloat &value);
  [[nodiscard]] inline rq::ConstantDetachedArrayElement &
  acquireConstantDetachedArrayElement(
      rq::Entity &entity, rq::ConstantDetachedArrayElement *next_ptr);
  [[nodiscard]] inline rq::ConstantDetachedArray &
  acquireConstantDetachedArray(rq::QualifiedType &type,
                               rq::ConstantDetachedArrayElement *first_ptr);
  [[nodiscard]] inline rq::ConstantDetachedArray &
  acquireConstantDetachedArray(rq::QualifiedType &type,
                               llvm::ArrayRef<rq::Entity *> elements);
  [[nodiscard]] inline rq::MutableInt &
  acquireMutableInt(rq::QualifiedType &type, const llvm::APInt &value);
  [[nodiscard]] inline rq::MutableFloat &
  acquireMutableFloat(rq::QualifiedType &type, const llvm::APFloat &value);
  [[nodiscard]] inline rq::MutableDetachedArray &
  acquireMutableDetachedArray(rq::QualifiedType &type,
                              llvm::ArrayRef<rq::Entity *> data = {});
};

} // namespace rq

#include <rq/detail/context.hpp>
