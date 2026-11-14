#pragma once

#include <algorithm>
#include <functional>

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerLiteralType &
Context::acquireIntegerLiteralType() {
  return this->_integer_literal_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FloatLiteralType &
Context::acquireFloatLiteralType() {
  return this->_float_literal_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::StringLiteralType &
Context::acquireStringLiteralType() {
  return this->_string_literal_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CodeunitLiteralType &
Context::acquireCodeunitLiteralType() {
  return this->_codeunit_literal_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::UnknownType &Context::acquireUnknownType() {
  return this->_unknown_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceType &
Context::acquireInferenceType() {
  return this->_inference_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VoidType &Context::acquireVoidType() {
  return this->_void_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnType &
Context::acquireNoReturnType() {
  return this->_no_return_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionType &
Context::acquireExpressionType() {
  return this->_expression_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionRangeType &
Context::acquireExpressionRangeType() {
  return this->_expression_range_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolRangeType &
Context::acquireSymbolRangeType() {
  return this->_symbol_range_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FittingFSIntType &
Context::acquireFittingFSIntType() {
  return this->_fitting_fsint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FittingFUIntType &
Context::acquireFittingFUIntType() {
  return this->_fitting_uint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FittingLSIntType &
Context::acquireFittingLSIntType() {
  return this->_fitting_lsint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FittingLUIntType &
Context::acquireFittingLUIntType() {
  return this->_fitting_luint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanType &Context::acquireBooleanType() {
  return this->_boolean_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::HalfType &Context::acquireHalfType() {
  return this->_half_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SingleType &Context::acquireSingleType() {
  return this->_single_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DoubleType &Context::acquireDoubleType() {
  return this->_double_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QuadrupleType &
Context::acquireQuadrupleType() {
  return this->_quadruple_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SIntType &Context::acquireSIntType() {
  return this->_sint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::UIntType &Context::acquireUIntType() {
  return this->_uint_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SSizeType &Context::acquireSSizeType() {
  return this->_ssize_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::USizeType &Context::acquireUSizeType() {
  return this->_usize_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SIndexType &Context::acquireSIndexType() {
  return this->_sindex_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::UIndexType &Context::acquireUIndexType() {
  return this->_uindex_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CharType &Context::acquireCharType() {
  return this->_char_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Type &
Context::acquireBinary16Type() {
  return this->_binary16_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Type &
Context::acquireBinary32Type() {
  return this->_binary32_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Type &
Context::acquireBinary64Type() {
  return this->_binary64_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Type &
Context::acquireBinary128Type() {
  return this->_binary128_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BFloat16Type &
Context::acquireBFloat16Type() {
  return this->_bfloat16_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AsciiType &Context::acquireAsciiType() {
  return this->_ascii_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Type &Context::acquireUtf8Type() {
  return this->_utf8_type;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoModifier &Context::acquireNoModifier() {
  return this->_no_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorModifier &
Context::acquireAnchorModifier() {
  return this->_anchor_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InitsModifier &
Context::acquireInitsModifier() {
  return this->_inits_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ResidentModifier &
Context::acquireResidentModifier() {
  return this->_resident_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FlankModifier &
Context::acquireFlankModifier() {
  return this->_flank_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TransparentModifier &
Context::acquireTransparentModifier() {
  return this->_transparent_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpaqueModifier &
Context::acquireOpaqueModifier() {
  return this->_opaque_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PrivateModifier &
Context::acquirePrivateModifier() {
  return this->_private_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PublicModifier &
Context::acquirePublicModifier() {
  return this->_public_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExportModifier &
Context::acquireExportModifier() {
  return this->_export_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MutableModifier &
Context::acquireMutableModifier() {
  return this->_mutable_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PartiallyMutableModifier &
Context::acquirePartiallyMutableModifier() {
  return this->_partially_mutable_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantModifier &
Context::acquireConstantModifier() {
  return this->_constant_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerModifier &
Context::acquireEagerModifier() {
  return this->_eager_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyModifier &
Context::acquireLazyModifier() {
  return this->_lazy_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PostModifier &
Context::acquirePostModifier() {
  return this->_post_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PreModifier &Context::acquirePreModifier() {
  return this->_pre_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AliasModifier &
Context::acquireAliasModifier() {
  return this->_alias_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumeratorModifier &
Context::acquireEnumeratorModifier() {
  return this->_enumerator_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptModifier &
Context::acquireConceptModifier() {
  return this->_concept_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SplitModifier &
Context::acquireSplitModifier() {
  return this->_split_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RuntimeModifier &
Context::acquireRuntimeModifier() {
  return this->_runtime_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ComtimeModifier &
Context::acquireComtimeModifier() {
  return this->_comtime_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnytimeModifier &
Context::acquireAnytimeModifier() {
  return this->_anytime_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PresetModifier &
Context::acquirePresetModifier() {
  return this->_preset_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SingletonModifier &
Context::acquireSingletonModifier() {
  return this->_singleton_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CaptureModifier &
Context::acquireCaptureModifier() {
  return this->_capture_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::StatelessModifier &
Context::acquireStatelessModifier() {
  return this->_stateless_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LinkedModifier &
Context::acquireLinkedModifier() {
  return this->_linked_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InlineModifier &
Context::acquireInlineModifier() {
  return this->_inline_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PureModifier &
Context::acquirePureModifier() {
  return this->_pure_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ImpureModifier &
Context::acquireImpureModifier() {
  return this->_impure_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::StandardMangleModifier &
Context::acquireStandardMangleModifier() {
  return this->_standard_mangle_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MangleModifier &
Context::acquireMangleModifier() {
  return this->_mangle_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PadModifier &Context::acquirePadModifier() {
  return this->_pad_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PackModifier &
Context::acquirePackModifier() {
  return this->_pack_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EquivocalModifier &
Context::acquireEquivocalModifier() {
  return this->_equivocal_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LikelyModifier &
Context::acquireLikelyModifier() {
  return this->_likely_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::UnlikelyModifier &
Context::acquireUnlikelyModifier() {
  return this->_unlikely_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SupportedModifier &
Context::acquireSupportedModifier() {
  return this->_supported_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DepreciatedModifier &
Context::acquireDepreciatedModifier() {
  return this->_depreciated_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExperimentalModifier &
Context::acquireExperimentalModifier() {
  return this->_experimental_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MovableModifier &
Context::acquireMovableModifier() {
  return this->_movable_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CopyableModifier &
Context::acquireCopyableModifier() {
  return this->_copyable_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InvariadicModifier &
Context::acquireInvariadicModifier() {
  return this->_invariadic_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicModifier &
Context::acquireVariadicModifier() {
  return this->_variadic_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BestLocationModifier &
Context::acquireBestLocationModifier() {
  return this->_best_location_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LocationModifier &
Context::acquireLocationModifier() {
  return this->_location_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateModifier &
Context::acquireTemplateModifier() {
  return this->_template_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OverloadModifier &
Context::acquireOverloadModifier() {
  return this->_overload_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintModifier &
Context::acquireConstraintModifier() {
  return this->_constraint_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ManualModifier &
Context::acquireManualModifier() {
  return this->_manual_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AutoModifier &
Context::acquireAutoModifier() {
  return this->_auto_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DirectModifier &
Context::acquireDirectModifier() {
  return this->_direct_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VirtualModifier &
Context::acquireVirtualModifier() {
  return this->_virtual_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RangerModifier &
Context::acquireRangerModifier() {
  return this->_ranger_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RequireModifier &
Context::acquireRequireModifier() {
  return this->_require_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureModifier &
Context::acquireEnsureModifier() {
  return this->_ensure_modifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoMutQualifier &
Context::acquireNoMutQualifier() {
  return this->_no_mut_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialMutQualifier &
Context::acquirePartialMutQualifier() {
  return this->_partial_mut_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MutQualifier &
Context::acquireMutQualifier() {
  return this->_mut_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoVolatileQualifier &
Context::acquireNoVolatileQualifier() {
  return this->_no_volatile_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileQualifier &
Context::acquireVolatileQualifier() {
  return this->_volatile_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoAtomicQualifier &
Context::acquireNoAtomicQualifier() {
  return this->_no_atomic_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicQualifier &
Context::acquireAtomicQualifier() {
  return this->_atomic_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoNullTerminateQualifier &
Context::acquireNoNullTerminateQualifier() {
  return this->_no_null_terminate_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateQualifier &
Context::acquireNullTerminateQualifier() {
  return this->_null_terminate_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoMarginQualifier &
Context::acquireNoMarginQualifier() {
  return this->_no_margin_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MarginQualifier &
Context::acquireMarginQualifier() {
  return this->_margin_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoDetachQualifier &
Context::acquireNoDetachQualifier() {
  return this->_no_detach_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DetachQualifier &
Context::acquireDetachQualifier() {
  return this->_detach_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NoThrowsQualifier &
Context::acquireNoThrowsQualifier() {
  return this->_no_throws_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ThrowsQualifier &
Context::acquireThrowsQualifier() {
  return this->_throws_qualifier;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnyConcept &Context::acquireAnyConcept() {
  return this->_any_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConcept &Context::acquireTypeConcept() {
  return this->_type_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ComtimeConcept &
Context::acquireComtimeConcept() {
  return this->_comtime_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RuntimeConcept &
Context::acquireRuntimeConcept() {
  return this->_runtime_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnytimeConcept &
Context::acquireAnytimeConcept() {
  return this->_anytime_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RangeConcept &
Context::acquireRangeConcept() {
  return this->_range_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RouteConcept &
Context::acquireRouteConcept() {
  return this->_route_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FigurativeConcept &
Context::acquireFigurativeConcept() {
  return this->_figurative_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ReflectionConcept &
Context::acquireReflectionConcept() {
  return this->_reflection_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PrimitiveConcept &
Context::acquirePrimitiveConcept() {
  return this->_primitive_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FittingPrimitiveConcept &
Context::acquireFittingPrimitiveConcept() {
  return this->_fitting_primative_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PlatformPrimitiveConcept &
Context::acquirePlatformPrimitiveConcept() {
  return this->_platform_primitive_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::StandardPrimitiveConcept &
Context::acquireStandardPrimitiveConcept() {
  return this->_standard_primitive_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledPrimitiveConcept &
Context::acquireScaledPrimitiveConcept() {
  return this->_scaled_primitive_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignedConcept &
Context::acquireSignedConcept() {
  return this->_scaled_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedConcept &
Context::acquireUnsignedConcept() {
  return this->_unsigned_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::IntConcept &Context::acquireIntConcept() {
  return this->_int_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExactIntConcept &
Context::acquireExactIntConcept() {
  return this->_exact_int_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FastIntConcept &
Context::acquireFastIntConcept() {
  return this->_fast_int_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LeastIntConcept &
Context::acquireLeastIntConcept() {
  return this->_least_int_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConcept &
Context::acquireFloatConcept() {
  return this->_float_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BinaryConcept &
Context::acquireBinaryConcept() {
  return this->_binary_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BFloatConcept &
Context::acquireBFloatConcept() {
  return this->_bfloat_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptConcept &
Context::acquireConceptConcept() {
  return this->_concept_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SubtypeConcept &
Context::acquireSubtypeConcept() {
  return this->_subtype_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArrayConcept &
Context::acquireArrayConcept() {
  return this->_array_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RefConcept &Context::acquireRefConcept() {
  return this->_ref_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PtrConcept &Context::acquirePtrConcept() {
  return this->_ptr_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SliceConcept &
Context::acquireSliceConcept() {
  return this->_slice_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SplitConcept &
Context::acquireSplitConcept() {
  return this->_split_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleConcept &
Context::acquireModuleConcept() {
  return this->_module_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ImportConcept &
Context::acquireImportConcept() {
  return this->_import_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PortalConcept &
Context::acquirePortalConcept() {
  return this->_portal_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RealizationConcept &
Context::acquireRealizationConcept() {
  return this->_realization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConformityConcept &
Context::acquireConformityConcept() {
  return this->_conformity_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdaptionConcept &
Context::acquireAdaptionConcept() {
  return this->_adaption_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TripConcept &Context::acquireTripConcept() {
  return this->_trip_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::JuxtConcept &Context::acquireJuxtConcept() {
  return this->_juxt_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SpecializationSetConcept &
Context::acquireSpecializationSetConcept() {
  return this->_specialization_set_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterSpecializationSetConcept &
Context::acquireAdapterSpecializationSetConcept() {
  return this->_adapter_specialization_set_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcSpecializationSetConcept &
Context::acquireProcSpecializationSetConcept() {
  return this->_proc_specialization_set_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceConcept &
Context::acquireArithmeticSequenceConcept() {
  return this->_arithmetic_sequence_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticIntervalConcept &
Context::acquireArithmeticIntervalConcept() {
  return this->_arithmetic_interval_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticSequenceConcept &
Context::acquireInfiniteArithmeticSequenceConcept() {
  return this->_infinite_arithmetic_sequence_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticSequenceConcept &
Context::acquireFiniteArithmeticSequenceConcept() {
  return this->_finite_arithmetic_sequence_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerDeclarationConcept &
Context::acquireEagerDeclarationConcept() {
  return this->_eager_declaration_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorConcept &
Context::acquireAnchorConcept() {
  return this->_anchor_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumeratorConcept &
Context::acquireEnumeratorConcept() {
  return this->_enumerator_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariableConcept &
Context::acquireVariableConcept() {
  return this->_variable_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerVariableConcept &
Context::acquireEagerVariableConcept() {
  return this->_eager_variable_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PostEagerVariableConcept &
Context::acquirePostEagerVariableConcept() {
  return this->_post_eager_variable_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PreEagerVariableConcept &
Context::acquirePreEagerVariableConcept() {
  return this->_pre_eager_variable_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterConcept &
Context::acquireParameterConcept() {
  return this->_parameter_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureConcept &
Context::acquireSignatureConcept() {
  return this->_signature_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutConcept &
Context::acquireLayoutConcept() {
  return this->_layout_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PlacementConcept &
Context::acquirePlacementConcept() {
  return this->_placement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionConcept &
Context::acquireCompositionConcept() {
  return this->_composition_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SynonymConcept &
Context::acquireSynonymConcept() {
  return this->_synonym_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableConcept &
Context::acquireSymbolTableConcept() {
  return this->_symbol_table_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NamespaceConcept &
Context::acquireNamespaceConcept() {
  return this->_namespace_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScopingStatementConcept &
Context::acquireEagerScopingStatementConcept() {
  return this->_eager_scoping_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::IfStatementConcept &
Context::acquireIfStatementConcept() {
  return this->_if_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ElseIfStatementConcept &
Context::acquireElseIfStatementConcept() {
  return this->_else_if_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ElseStatementConcept &
Context::acquireElseStatementConcept() {
  return this->_else_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SwitchStatementConcept &
Context::acquireSwitchStatementConcept() {
  return this->_switch_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CaseStatementConcept &
Context::acquireCaseStatementConcept() {
  return this->_case_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ForStatementConcept &
Context::acquireForStatementConcept() {
  return this->_for_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::WhileStatementConcept &
Context::acquireWhileStatementConcept() {
  return this->_while_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeStatementConcept &
Context::acquireScopeStatementConcept() {
  return this->_scope_statement_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ImplementationConcept &
Context::acquireImplementationConcept() {
  return this->_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OverloadConcept &
Context::acquireOverloadConcept() {
  return this->_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SpecializationConcept &
Context::acquireSpecializationConcept() {
  return this->_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateConcept &
Context::acquireTemplateConcept() {
  return this->_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassImplementationConcept &
Context::acquireClassImplementationConcept() {
  return this->_class_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassOverloadConcept &
Context::acquireClassOverloadConcept() {
  return this->_class_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassSpecializationConcept &
Context::acquireClassSpecializationConcept() {
  return this->_class_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassTemplateConcept &
Context::acquireClassTemplateConcept() {
  return this->_class_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumImplementationConcept &
Context::acquireEnumImplementationConcept() {
  return this->_enum_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumOverloadConcept &
Context::acquireEnumOverloadConcept() {
  return this->_enum_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementationConcept &
Context::acquireInterfaceImplementationConcept() {
  return this->_interface_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceOverloadConcept &
Context::acquireInterfaceOverloadConcept() {
  return this->_interface_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceSpecializationConcept &
Context::acquireInterfaceSpecializationConcept() {
  return this->_interface_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementationConcept &
Context::acquireAdapterImplementationConcept() {
  return this->_adapter_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterOverloadConcept &
Context::acquireAdapterOverloadConcept() {
  return this->_adapter_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterSpecializationConcept &
Context::acquireAdapterSpecializationConcept() {
  return this->_adapter_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterTemplateConcept &
Context::acquireAdapterTemplateConcept() {
  return this->_adapter_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcImplementationConcept &
Context::acquireProcImplementationConcept() {
  return this->_proc_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcOverloadConcept &
Context::acquireProcOverloadConcept() {
  return this->_proc_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcSpecializationConcept &
Context::acquireProcSpecializationConcept() {
  return this->_proc_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcTemplateConcept &
Context::acquireProcTemplateConcept() {
  return this->_proc_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableImplementationConcept &
Context::acquireLazyVariableImplementationConcept() {
  return this->_lazy_variable_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableSpecializationConcept &
Context::acquireLazyVariableSpecializationConcept() {
  return this->_lazy_variable_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableTemplateConcept &
Context::acquireLazyVariableTemplateConcept() {
  return this->_lazy_variable_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AliasImplementationConcept &
Context::acquireAliasImplementationConcept() {
  return this->_alias_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AliasOverloadConcept &
Context::acquireAliasOverloadConcept() {
  return this->_alias_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AliasSpecializationConcept &
Context::acquireAliasSpecializationConcept() {
  return this->_alias_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AliasTemplateConcept &
Context::acquireAliasTemplateConcept() {
  return this->_alias_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConceptImplementationConcept &
Context::acquireConceptImplementationConcept() {
  return this->_concept_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariantImplementationConcept &
Context::acquireVariantImplementationConcept() {
  return this->_variant_implementation_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariantOverloadConcept &
Context::acquireVariantOverloadConcept() {
  return this->_variant_overload_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariantSpecializationConcept &
Context::acquireVariantSpecializationConcept() {
  return this->_variant_specialization_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariantTemplateConcept &
Context::acquireVariantTemplateConcept() {
  return this->_variant_template_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AttributeConcept &
Context::acquireAttributeConcept() {
  return this->_attribute_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierConcept &
Context::acquireModifierConcept() {
  return this->_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AnchorModifierConcept &
Context::acquireAnchorModifierConcept() {
  return this->_anchor_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ContainerModifierConcept &
Context::acquireContainerModifierConcept() {
  return this->_container_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VisibilityModifierConcept &
Context::acquireVisibilityModifierConcept() {
  return this->_visibility_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AccessModifierConcept &
Context::acquireAccessModifierConcept() {
  return this->_access_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MutabilityModifierConcept &
Context::acquireMutabilityModifierConcept() {
  return this->_mutability_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EvaluationTimeModifierConcept &
Context::acquireEvaluationTimeModifierConcept() {
  return this->_evaluation_time_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DialectModifierConcept &
Context::acquireDialectModifierConcept() {
  return this->_dialect_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TenureModifierConcept &
Context::acquireTenureModifierConcept() {
  return this->_tenure_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InitializationTimeModifierConcept &
Context::acquireInitializationTimeModifierConcept() {
  return this->_intialization_time_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClosureModifierConcept &
Context::acquireClosureModifierConcept() {
  return this->_closure_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LinkageModifierConcept &
Context::acquireLinkageModifierConcept() {
  return this->_linkage_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SideEffectsModifierConcept &
Context::acquireSideEffectsModifierConcept() {
  return this->_side_effects_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MangleModifierConcept &
Context::acquireMangleModifierConcept() {
  return this->_mangle_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::PackModifierConcept &
Context::acquirePackModifierConcept() {
  return this->_pack_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BranchTrendModifierConcept &
Context::acquireBranchTrendModifierConcept() {
  return this->_branch_trend_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SupportNoticeModifierConcept &
Context::acquireSupportNoticeModifierConcept() {
  return this->_support_notice_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MovableModifierConcept &
Context::acquireMovableModifierConcept() {
  return this->_movable_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CopyableModifierConcept &
Context::acquireCopyableModifierConcept() {
  return this->_copyable_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicModifierConcept &
Context::acquireVariadicModifierConcept() {
  return this->_variadic_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OffsetModifierConcept &
Context::acquireOffsetModifierConcept() {
  return this->_offset_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyDeclarationModifierConcept &
Context::acquireLazyDeclarationModifierConcept() {
  return this->_lazy_declaration_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstraintModifierConcept &
Context::acquireConstraintModifierConcept() {
  return this->_constraint_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DeductionModifierConcept &
Context::acquireDeductionModifierConcept() {
  return this->_deduction_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VirtualityModifierConcept &
Context::acquireVirtualityModifierConcept() {
  return this->_virtuality_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RangerModifierConcept &
Context::acquireRangerModifierConcept() {
  return this->_ranger_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::RequireModifierConcept &
Context::acquireRequireModifierConcept() {
  return this->_require_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnsureModifierConcept &
Context::acquireEnsureModifierConcept() {
  return this->_ensure_modifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifierConcept &
Context::acquireQualifierConcept() {
  return this->_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MutQualifierConcept &
Context::acquireMutQualifierConcept() {
  return this->_mut_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::VolatileQualifierConcept &
Context::acquireVolatileQualifierConcept() {
  return this->_volatile_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AtomicQualifierConcept &
Context::acquireAtomicQualifierConcept() {
  return this->_atomic_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NullTerminateQualifierConcept &
Context::acquireNullTerminateQualifierConcept() {
  return this->_null_terminate_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::DetachQualifierConcept &
Context::acquireDetachQualifierConcept() {
  return this->_detach_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MarginQualifierConcept &
Context::acquireMarginQualifierConcept() {
  return this->_margin_qualifier_concept;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ThrowsQualifierConcept &
Context::acquireThrowsQualifierConcept() {
  return this->_throws_qualifier_concept;
}

[[nodiscard]] inline rq::ScaledSIntType &
Context::acquireScaledSIntType(unsigned depth) {
  return rq::acquire<rq::ScaledSIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledUIntType &
Context::acquireScaledUIntType(unsigned depth) {
  return rq::acquire<rq::ScaledUIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledFSIntType &
Context::acquireScaledFSIntType(unsigned depth) {
  return rq::acquire<rq::ScaledFSIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledFUIntType &
Context::acquireScaledFUIntType(unsigned depth) {
  return rq::acquire<rq::ScaledFUIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledLSIntType &
Context::acquireScaledLSIntType(unsigned depth) {
  return rq::acquire<rq::ScaledLSIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledLUIntType &
Context::acquireScaledLUIntType(unsigned depth) {
  return rq::acquire<rq::ScaledLUIntType>(
      *this, this->_scaled_primitive_type_set, depth);
}

[[nodiscard]] inline rq::ScaledPrimitiveType &
Context::acquireScaledPrimitiveType(rq::SymbolKind kind, unsigned depth) {
  switch (kind) {
  case rq::SymbolKind::SCALED_SINT_TYPE:
    return this->acquireScaledSIntType(depth);
  case rq::SymbolKind::SCALED_UINT_TYPE:
    return this->acquireScaledUIntType(depth);
  case rq::SymbolKind::SCALED_FSINT_TYPE:
    return this->acquireScaledFSIntType(depth);
  case rq::SymbolKind::SCALED_FUINT_TYPE:
    return this->acquireScaledFUIntType(depth);
  case rq::SymbolKind::SCALED_LSINT_TYPE:
    return this->acquireScaledLSIntType(depth);
  case rq::SymbolKind::SCALED_LUINT_TYPE:
    return this->acquireScaledLUIntType(depth);
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::RefSubtype &
Context::acquireRefSubtype(rq::QualifiedType &child) {
  return rq::acquire<rq::RefSubtype>(*this, this->_simple_subtype_set, child);
}

[[nodiscard]] inline rq::PtrSubtype &
Context::acquirePtrSubtype(rq::QualifiedType &child) {
  return rq::acquire<rq::PtrSubtype>(*this, this->_simple_subtype_set, child);
}

[[nodiscard]] inline rq::SliceSubtype &
Context::acquireSliceSubtype(rq::QualifiedType &child) {
  return rq::acquire<rq::SliceSubtype>(*this, this->_simple_subtype_set, child);
}

[[nodiscard]] inline rq::SplitSubtype &
Context::acquireSplitSubtype(rq::QualifiedType &child) {
  return rq::acquire<rq::SplitSubtype>(*this, this->_simple_subtype_set, child);
}

[[nodiscard]] inline rq::InferenceCountArraySubtype &
Context::acquireInferenceCountArraySubtype(rq::QualifiedType &child) {
  return rq::acquire<rq::InferenceCountArraySubtype>(
      *this, this->_simple_subtype_set, child);
}

[[nodiscard]] inline rq::SimpleSubtype &
Context::acquireSimpleSubtype(rq::SymbolKind kind, rq::QualifiedType &child) {
  switch (kind) {
  case rq::SymbolKind::REF_SUBTYPE:
    return this->acquireRefSubtype(child);
  case rq::SymbolKind::PTR_SUBTYPE:
    return this->acquirePtrSubtype(child);
  case rq::SymbolKind::SLICE_SUBTYPE:
    return this->acquireSliceSubtype(child);
  case rq::SymbolKind::SPLIT_SUBTYPE:
    return this->acquireSplitSubtype(child);
  case rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return this->acquireInferenceCountArraySubtype(child);
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::GenericSubsymbol &
Context::acquireGenericSubsymbol(rq::Symbol &child) {
  return rq::acquire<rq::GenericSubsymbol>(*this, this->_generic_subsymbol_set,
                                           child);
}

[[nodiscard]] inline rq::Subsymbol &
Context::acquireSubsymbol(rq::SymbolKind kind, rq::Symbol &child) {
  switch (kind) {
  case rq::SymbolKind::GENERIC_SUBSYMBOL:
    return this->acquireGenericSubsymbol(child);
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::ArithmeticIntervalType &
Context::acquireArithmeticIntervalType(rq::QualifiedType &child,
                                       rq::ArithmeticCondition condition,
                                       rq::ArithmeticStep step) {
  return rq::acquire<rq::ArithmeticIntervalType>(
      *this, this->_arithmetic_sequence_type_set, child, condition, step);
}

[[nodiscard]] inline rq::InfiniteArithmeticSequenceType &
Context::acquireInfiniteArithmeticSequenceType(
    rq::QualifiedType &child, rq::ArithmeticCondition condition,
    rq::ArithmeticStep step) {
  return rq::acquire<rq::InfiniteArithmeticSequenceType>(
      *this, this->_arithmetic_sequence_type_set, child, condition, step);
}

[[nodiscard]] inline rq::FiniteArithmeticSequenceType &
Context::acquireFiniteArithmeticSequenceType(rq::QualifiedType &child,
                                             rq::ArithmeticCondition condition,
                                             rq::ArithmeticStep step) {
  return rq::acquire<rq::FiniteArithmeticSequenceType>(
      *this, this->_arithmetic_sequence_type_set, child, condition, step);
}

[[nodiscard]] inline rq::ArithmeticSequenceType &
Context::acquireArithmeticSequenceType(rq::SymbolKind kind,
                                       rq::QualifiedType &child,
                                       rq::ArithmeticCondition condition,
                                       rq::ArithmeticStep step) {
  switch (kind) {
  case rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE:
    return this->acquireArithmeticIntervalType(child, condition, step);
  case rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return this->acquireInfiniteArithmeticSequenceType(child, condition, step);
  case rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return this->acquireFiniteArithmeticSequenceType(child, condition, step);
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::QualifiedType &Context::acquireQualifiedType(
    rq::Symbol &type, rq::QualifierFuseFlags qualifiers, unsigned margin) {
  return rq::acquire(*this, this->_qualified_type_set, type, qualifiers,
                     margin);
}

[[nodiscard]] inline rq::ArraySubtype &
Context::acquireArraySubtype(rq::QualifiedType &child, std::size_t count) {
  return rq::acquire(*this, this->_array_set, child, count);
}

[[nodiscard]] inline rq::Adaption &
Context::acquireAdaption(rq::QualifiedType &reciever,
                         rq::InterfaceImplementation &interface,
                         rq::AdapterImplementation &adapter) {
  return rq::acquire(*this, this->_adaption_set, reciever, interface, adapter);
}

[[nodiscard]] inline rq::Realization &
Context::acquireRealization(rq::QualifiedType &reciever,
                            rq::InterfaceImplementation &interface) {
  return rq::acquire(*this, this->_realization_set, reciever, interface);
}

[[nodiscard]] inline rq::Conformity &
Context::acquireConformity(rq::InterfaceImplementation &interface,
                           rq::AdapterImplementation &adapter) {
  return rq::acquire(*this, this->_conformity_set, interface, adapter);
}

[[nodiscard]] inline rq::JuxtListType &
Context::acquireJuxtListType(rq::NodeList<rq::QualifiedType> &&type_list) {
  return rq::acquire(*this, this->_juxt_list_type_set, std::move(type_list));
}

[[nodiscard]] inline rq::Trip &
Context::acquireTrip(llvm::SmallVector<rq::Entity *, 8> &entities) {
#ifndef _NDEBUG
  for (rq::Entity *entity_ptr : entities) {
    RQ_ASSERT(entity_ptr != nullptr, "null trip member");
    std::ignore = entity_ptr;
  }
#endif
  std::sort(entities.begin(), entities.end(), std::less<rq::Entity *>{});
  rq::NodeList<rq::Entity> entity_list;
  for (rq::Entity *entity_ptr : entities) {
    rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    entity_list.insertFront(*this, entity);
  }
  return rq::acquire(*this, this->_trip_set, std::move(entity_list));
}

[[nodiscard]] inline rq::SpecializationSetArgument &
Context::acquireSpecializationSetArgument(
    rq::Name name, rq::Entity &value, rq::SpecializationSetArgument *next_ptr) {
  return rq::acquire(*this, this->_specialization_set_argument, name, value,
                     next_ptr);
}

[[nodiscard]] inline rq::SpecializationSet &
Context::acquireSpecializationSet(rq::SymbolKind kind,
                                  rq::SpecializationSetArgument *first_ptr) {
  switch (kind) {
  case rq::SymbolKind::SPECIALIATION_SET:
    return rq::acquire(*this, this->_specialization_set, kind, first_ptr);
  case rq::SymbolKind::PROC_SPECIALIZATION_SET:
    return this->acquireProcSpecializationSet(first_ptr);
  case rq::SymbolKind::ADAPTER_SPECIALIZATION_SET:
    return this->acquireAdapterSpecializationSet(first_ptr);
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::ProcSpecializationSet &
Context::acquireProcSpecializationSet(
    rq::SpecializationSetArgument *first_ptr) {
  return rq::acquire(*this, this->_proc_specialization_set, first_ptr);
}

[[nodiscard]] inline rq::AdapterSpecializationSet &
Context::acquireAdapterSpecializationSet(
    rq::SpecializationSetArgument *first_ptr) {
  return rq::acquire(*this, this->_adapter_specialization_set, first_ptr);
}

[[nodiscard]] inline rq::Parameter &Context::acquireParameter(
    rq::Parameter *next_ptr, rq::Name name, rq::QualifiedType &type,
    rq::ModifierFuseFlags modifier_flags, rq::ParameterInfoFlags param_flags,
    rq::Entity *default_ptr) {
  return rq::acquire(*this, this->_parameter_set, next_ptr, name, type,
                     modifier_flags, param_flags, default_ptr);
}

[[nodiscard]] inline rq::CompositionType &Context::acquireCompositionType(
    rq::NodeList<rq::InterfaceImplementation> &&interface_list) {
  return rq::acquire(*this, this->_composition_type_set,
                     std::move(interface_list));
}

[[nodiscard]] inline rq::SignatureType &
Context::acquireSignatureType(rq::Parameter *first_parameter_ptr,
                              rq::QualifiedType &return_type) {
  return rq::acquire(*this, this->_signature_type_set, first_parameter_ptr,
                     return_type);
}

[[nodiscard]] inline rq::LayoutType &
Context::acquireLayoutType(rq::Parameter *first_parameter_ptr) {
  return rq::acquire(*this, this->_layout_type_set, first_parameter_ptr);
}

[[nodiscard]] inline rq::PlacementType &
Context::acquirePlacementType(rq::ProcImplementation &function) {
  return rq::acquire(*this, this->_placement_type_set, function);
}

[[nodiscard]] inline rq::Import &Context::acquireImport() {
  return this->allocateValue<rq::Import>();
}

[[nodiscard]] inline rq::Anchor &Context::acquireAnchor() {
  return this->allocateValue<rq::Anchor>();
}

[[nodiscard]] inline rq::Enumerator &Context::acquireEnumerator() {
  return this->allocateValue<rq::Enumerator>();
}

[[nodiscard]] inline rq::Portal &Context::acquirePortal() {
  return this->allocateValue<rq::Portal>();
}

[[nodiscard]] inline rq::CTable &Context::acquireCTable() {
  return this->allocateValue<rq::CTable>();
}

[[nodiscard]] inline rq::ScopeStatement &
Context::acquirePrequiteScopeStatement() {
  return this->allocateValue<rq::ScopeStatement>();
}

[[nodiscard]] inline rq::ScopeStatement &
Context::acquirePostquiteScopeStatement() {
  return this->allocateValue<rq::ScopeStatement>();
}

[[nodiscard]] inline rq::WhileStatement &
Context::acquirePrequiteWhileStatement() {
  return this->allocateValue<rq::WhileStatement>();
}

[[nodiscard]] inline rq::WhileStatement &
Context::acquirePostquiteWhileStatement() {
  return this->allocateValue<rq::WhileStatement>();
}

[[nodiscard]] inline rq::ForStatement &Context::acquirePrequiteForStatement() {
  return this->allocateValue<rq::ForStatement>();
}

[[nodiscard]] inline rq::ForStatement &Context::acquirePostquiteForStatement() {
  return this->allocateValue<rq::ForStatement>();
}

[[nodiscard]] inline rq::CaseStatement &
Context::acquirePrequiteCaseStatement() {
  return this->allocateValue<rq::CaseStatement>();
}

[[nodiscard]] inline rq::CaseStatement &
Context::acquirePostquiteCaseStatement() {
  return this->allocateValue<rq::CaseStatement>();
}

[[nodiscard]] inline rq::SwitchStatement &
Context::acquirePrequiteSwitchStatement() {
  return this->allocateValue<rq::SwitchStatement>();
}

[[nodiscard]] inline rq::SwitchStatement &
Context::acquirePostquiteSwitchStatement() {
  return this->allocateValue<rq::SwitchStatement>();
}

[[nodiscard]] inline rq::IfStatement &Context::acquirePrequiteIfStatement() {
  return this->allocateValue<rq::IfStatement>();
}

[[nodiscard]] inline rq::IfStatement &Context::acquirePostquiteIfStatement() {
  return this->allocateValue<rq::IfStatement>();
}

[[nodiscard]] inline rq::ElseIfStatement &
Context::acquirePrequiteElseIfStatement() {
  return this->allocateValue<rq::ElseIfStatement>();
}

[[nodiscard]] inline rq::ElseIfStatement &
Context::acquirePostquiteElseIfStatement() {
  return this->allocateValue<rq::ElseIfStatement>();
}

[[nodiscard]] inline rq::ElseStatement &
Context::acquirePrequiteElseStatement() {
  return this->allocateValue<rq::ElseStatement>();
}

[[nodiscard]] inline rq::ElseStatement &
Context::acquirePostquiteElseStatement() {
  return this->allocateValue<rq::ElseStatement>();
}

[[nodiscard]] inline rq::Namespace &Context::acquireNamespace() {
  return this->allocateValue<rq::Namespace>();
}

[[nodiscard]] inline rq::EnumOverload &Context::acquireEnumOverload() {
  return this->allocateValue<rq::EnumOverload>();
}

[[nodiscard]] inline rq::EnumSpecialization &
Context::acquireEnumSpecialization() {
  return this->allocateValue<rq::EnumSpecialization>();
}

[[nodiscard]] inline rq::AdapterOverload &Context::acquireAdapterOverload() {
  return this->allocateValue<rq::AdapterOverload>();
}

[[nodiscard]] inline rq::AdapterSpecialization &
Context::acquireAdapterSpecialization() {
  return this->allocateValue<rq::AdapterSpecialization>();
}

[[nodiscard]] inline rq::InterfaceOverload &
Context::acquireInterfaceOverload() {
  return this->allocateValue<rq::InterfaceOverload>();
}

[[nodiscard]] inline rq::InterfaceSpecialization &
Context::acquireInterfaceSpecialization() {
  return this->allocateValue<rq::InterfaceSpecialization>();
}

[[nodiscard]] inline rq::ClassOverload &Context::acquireClassOverload() {
  return this->allocateValue<rq::ClassOverload>();
}

[[nodiscard]] inline rq::ClassSpecialization &
Context::acquireClassSpecialization() {
  return this->allocateValue<rq::ClassSpecialization>();
}

[[nodiscard]] inline rq::ProcOverload &Context::acquireProcOverload() {
  return this->allocateValue<rq::ProcOverload>();
}

[[nodiscard]] inline rq::ProcSpecialization &
Context::acquireProcSpecialization() {
  return this->allocateValue<rq::ProcSpecialization>();
}

[[nodiscard]] inline rq::AliasOverload &Context::acquireAliasOverload() {
  return this->allocateValue<rq::AliasOverload>();
}

[[nodiscard]] inline rq::AliasSpecialization &
Context::acquireAliasSpecialization() {
  return this->allocateValue<rq::AliasSpecialization>();
}

[[nodiscard]] inline rq::ConceptOverload &Context::acquireConceptOverload() {
  return this->allocateValue<rq::ConceptOverload>();
}

[[nodiscard]] inline rq::ConceptSpecialization &
Context::acquireConceptSpecialization() {
  return this->allocateValue<rq::ConceptSpecialization>();
}

[[nodiscard]] inline rq::VariantOverload &Context::acquireVariantOverload() {
  return this->allocateValue<rq::VariantOverload>();
}

[[nodiscard]] inline rq::VariantSpecialization &
Context::acquireVariantSpecialization() {
  return this->allocateValue<rq::VariantSpecialization>();
}

[[nodiscard]] inline rq::EnumTemplate &Context::acquireEnumTemplate() {
  return this->allocateValue<rq::EnumTemplate>();
}

[[nodiscard]] inline rq::ClassTemplate &Context::acquireClassTemplate() {
  return this->allocateValue<rq::ClassTemplate>();
}

[[nodiscard]] inline rq::InterfaceTemplate &
Context::acquireInterfaceTemplate() {
  return this->allocateValue<rq::InterfaceTemplate>();
}

[[nodiscard]] inline rq::LazyVariableTemplate &
Context::acquireLazyVariableTemplate() {
  return this->allocateValue<rq::LazyVariableTemplate>();
}

[[nodiscard]] inline rq::AdapterTemplate &Context::acquireAdapterTemplate() {
  return this->allocateValue<rq::AdapterTemplate>();
}

[[nodiscard]] inline rq::ProcTemplate &Context::acquireProcTemplate() {
  return this->allocateValue<rq::ProcTemplate>();
}

[[nodiscard]] inline rq::AliasTemplate &Context::acquireAliasTemplate() {
  return this->allocateValue<rq::AliasTemplate>();
}

[[nodiscard]] inline rq::ConceptTemplate &Context::acquireConceptTemplate() {
  return this->allocateValue<rq::ConceptTemplate>();
}

[[nodiscard]] inline rq::VariantTemplate &Context::acquireVariantTemplate() {
  return this->allocateValue<rq::VariantTemplate>();
}

[[nodiscard]] inline rq::ClassPolymorph &Context::acquireClassPolymorph() {
  return this->allocateValue<rq::ClassPolymorph>();
}

[[nodiscard]] inline rq::EnumPolymorph &Context::acquireEnumPolymorph() {
  return this->allocateValue<rq::EnumPolymorph>();
}

[[nodiscard]] inline rq::InterfacePolymorph &
Context::acquireInterfacePolymorph() {
  return this->allocateValue<rq::InterfacePolymorph>();
}

[[nodiscard]] inline rq::AdapterPolymorph &Context::acquireAdapterPolymorph() {
  return this->allocateValue<rq::AdapterPolymorph>();
}

[[nodiscard]] inline rq::ProcPolymorph &Context::acquireProcPolymorph() {
  return this->allocateValue<rq::ProcPolymorph>();
}

[[nodiscard]] inline rq::LazyVariablePolymorph &
Context::acquireLazyVariablePolymorph() {
  return this->allocateValue<rq::LazyVariablePolymorph>();
}

[[nodiscard]] inline rq::AliasPolymorph &Context::acquireAliasPolymorph() {
  return this->allocateValue<rq::AliasPolymorph>();
}

[[nodiscard]] inline rq::VariantPolymorph &Context::acquireVariantPolymorph() {
  return this->allocateValue<rq::VariantPolymorph>();
}

[[nodiscard]] inline rq::ConceptPolymorph &Context::acquireConceptPolymorph() {
  return this->allocateValue<rq::ConceptPolymorph>();
}

[[nodiscard]] inline rq::SynonymType &
Context::acquireSynonymType(rq::Symbol &original) {
  return this->allocateValue<rq::SynonymType>(original);
}

[[nodiscard]] inline rq::Module &
Context::acquireModule(rq::ModuleDetail &&detail) {
  return this->allocateValue<rq::Module>(std::move(detail));
}

[[nodiscard]] inline rq::ConstantInt &
Context::acquireConstantInt(rq::QualifiedType &type, const llvm::APInt &value) {
  return rq::acquire(*this, this->_constant_int_set, type, value);
}

[[nodiscard]] inline rq::ConstantFloat &
Context::acquireConstantFloat(rq::QualifiedType &type,
                              const llvm::APFloat &value) {
  return rq::acquire(*this, this->_constant_float_set, type, value);
}

[[nodiscard]] inline rq::ConstantDetachedArrayElement &
Context::acquireConstantDetachedArrayElement(
    rq::Entity &entity, rq::ConstantDetachedArrayElement *next_ptr) {
  return rq::acquire(*this, this->_constant_detached_array_element_set, entity,
                     next_ptr);
}

[[nodiscard]] inline rq::ConstantDetachedArray &
Context::acquireConstantDetachedArray(
    rq::QualifiedType &type, rq::ConstantDetachedArrayElement *first_ptr) {
  return rq::acquire(*this, this->_constant_detached_array_set, type,
                     first_ptr);
}

[[nodiscard]] inline rq::ConstantDetachedArray &
Context::acquireConstantDetachedArray(rq::QualifiedType &type,
                                      llvm::ArrayRef<rq::Entity *> elements) {
  rq::ConstantDetachedArrayElement *first_ptr = nullptr;
  for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
    first_ptr = &this->acquireConstantDetachedArrayElement(
        rq::dereferencePtr(*it), first_ptr);
  }
  return this->acquireConstantDetachedArray(type, first_ptr);
}

[[nodiscard]] inline rq::MutableInt &
Context::acquireMutableInt(rq::QualifiedType &type, const llvm::APInt &value) {
  return this->allocateGcValue<rq::MutableInt>(type, value);
}

[[nodiscard]] inline rq::MutableFloat &
Context::acquireMutableFloat(rq::QualifiedType &type,
                             const llvm::APFloat &value) {
  return this->allocateGcValue<rq::MutableFloat>(type, value);
}

[[nodiscard]] inline rq::MutableDetachedArray &
Context::acquireMutableDetachedArray(rq::QualifiedType &type,
                                     llvm::ArrayRef<rq::Entity *> data) {
  return this->allocateGcValue<rq::MutableDetachedArray>(type, data);
}

[[nodiscard]] inline rq::PreEagerVariable &
Context::acquirePrequitePreEagerVariable() {
  return this->allocateValue<rq::PreEagerVariable>();
}

[[nodiscard]] inline rq::PreEagerVariable &
Context::acquirePostquitePreEagerVariable() {
  return this->allocateValue<rq::PreEagerVariable>();
}

[[nodiscard]] inline rq::PostEagerVariable &
Context::acquirePrequitePostEagerVariable() {
  return this->allocateValue<rq::PostEagerVariable>();
}

[[nodiscard]] inline rq::PostEagerVariable &
Context::acquirePostquitePostEagerVariable() {
  return this->allocateValue<rq::PostEagerVariable>();
}

[[nodiscard]] inline rq::LazyVariableOverload &
Context::acquireLazyVariableOverload() {
  return this->allocateValue<rq::LazyVariableOverload>();
}

[[nodiscard]] inline rq::LazyVariableSpecialization &
Context::acquireLazyVariableSpecialization() {
  return this->allocateValue<rq::LazyVariableSpecialization>();
}

[[nodiscard]] inline rq::EagerVariable &
Context::acquirePrequiteEagerVariable(rq::SymbolKind kind) {
  switch (kind) {
  case rq::SymbolKind::PRE_EAGER_VARIABLE:
    return this->acquirePrequitePreEagerVariable();
  case rq::SymbolKind::POST_EAGER_VARIABLE:
    return this->acquirePrequitePostEagerVariable();
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::EagerScope &
Context::acquirePrequiteEagerScope(rq::SymbolKind kind) {
  switch (kind) {
  case rq::SymbolKind::SCOPE_STATEMENT:
    return this->acquirePrequiteScopeStatement();
  case rq::SymbolKind::WHILE_STATEMENT:
    return this->acquirePrequiteWhileStatement();
  case rq::SymbolKind::FOR_STATEMENT:
    return this->acquirePrequiteForStatement();
  case rq::SymbolKind::CASE_STATEMENT:
    return this->acquirePrequiteCaseStatement();
  case rq::SymbolKind::SWITCH_STATEMENT:
    return this->acquirePrequiteSwitchStatement();
  case rq::SymbolKind::IF_STATEMENT:
    return this->acquirePrequiteIfStatement();
  case rq::SymbolKind::ELSE_IF_STATEMENT:
    return this->acquirePrequiteElseIfStatement();
  case rq::SymbolKind::ELSE_STATEMENT:
    return this->acquirePrequiteElseStatement();
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::EagerVariable &
Context::acquirePostquiteEagerVariable(rq::SymbolKind kind) {
  switch (kind) {
  case rq::SymbolKind::PRE_EAGER_VARIABLE:
    return this->acquirePostquitePreEagerVariable();
  case rq::SymbolKind::POST_EAGER_VARIABLE:
    return this->acquirePostquitePostEagerVariable();
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::EagerScope &
Context::acquirePostquiteEagerScope(rq::SymbolKind kind) {
  switch (kind) {
  case rq::SymbolKind::SCOPE_STATEMENT:
    return this->acquirePostquiteScopeStatement();
  case rq::SymbolKind::WHILE_STATEMENT:
    return this->acquirePostquiteWhileStatement();
  case rq::SymbolKind::FOR_STATEMENT:
    return this->acquirePostquiteForStatement();
  case rq::SymbolKind::CASE_STATEMENT:
    return this->acquirePostquiteCaseStatement();
  case rq::SymbolKind::SWITCH_STATEMENT:
    return this->acquirePostquiteSwitchStatement();
  case rq::SymbolKind::IF_STATEMENT:
    return this->acquirePostquiteIfStatement();
  case rq::SymbolKind::ELSE_IF_STATEMENT:
    return this->acquirePostquiteElseIfStatement();
  case rq::SymbolKind::ELSE_STATEMENT:
    return this->acquirePostquiteElseStatement();
  default:
    RQ_UNREACHABLE();
  }
}

} // namespace rq
