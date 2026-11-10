#pragma once

#include <rq/bump_ptr_allocator.hpp>

#include <utility>

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    return "None";
  case S::INTEGER_LITERAL_TYPE:
    return "IntegerLiteral";
  case S::FLOAT_LITERAL_TYPE:
    return "FloatLiteral";
  case S::STRING_LITERAL_TYPE:
    return "StringLiteral";
  case S::CODEUNIT_LITERAL_TYPE:
    return "CodeunitLiteral";
  case S::UNKNOWN_TYPE:
    return "Unknown";
  case S::INFERENCE_TYPE:
    return "Inference";
  case S::VOID_TYPE:
    return "Void";
  case S::NO_RETURN_TYPE:
    return "NoReturn";
  case S::SYMBOL_RANGE_TYPE:
    return "SymbolRange";
  case S::EXPRESSION_TYPE:
    return "Expression";
  case S::EXPRESSION_RANGE_TYPE:
    return "ExpressionRange";
  case S::FITTING_FSINT_TYPE:
    return "FSInt";
  case S::FITTING_FUINT_TYPE:
    return "FUInt";
  case S::FITTING_LSINT_TYPE:
    return "LSInt";
  case S::FITTING_LUINT_TYPE:
    return "LUInt";
  case S::BOOLEAN_TYPE:
    return "Bool";
  case S::HALF_TYPE:
    return "Half";
  case S::SINGLE_TYPE:
    return "Single";
  case S::DOUBLE_TYPE:
    return "Double";
  case S::QUADRUPLE_TYPE:
    return "Quadruple";
  case S::SINT_TYPE:
    return "SInt";
  case S::UINT_TYPE:
    return "UInt";
  case S::SSIZE_TYPE:
    return "SSize";
  case S::USIZE_TYPE:
    return "USize";
  case S::SINDEX_TYPE:
    return "SIndex";
  case S::UINDEX_TYPE:
    return "UIndex";
  case S::CHAR_TYPE:
    return "Char";
  case S::BINARY16_TYPE:
    return "Binary16";
  case S::BINARY32_TYPE:
    return "Binary32";
  case S::BINARY64_TYPE:
    return "Binary64";
  case S::BINARY128_TYPE:
    return "Binary128";
  case S::BFLOAT16_TYPE:
    return "BFloat16";
  case S::ASCII_TYPE:
    return "Ascii";
  case S::UTF8_TYPE:
    return "Utf8";
  case S::SCALED_SINT_TYPE:
    return "ScaledSInt";
  case S::SCALED_UINT_TYPE:
    return "ScaledUInt";
  case S::SCALED_FSINT_TYPE:
    return "ScaledFSInt";
  case S::SCALED_FUINT_TYPE:
    return "ScaledFUInt";
  case S::SCALED_LSINT_TYPE:
    return "ScaledLSInt";
  case S::SCALED_LUINT_TYPE:
    return "ScaledLUInt";
  case S::NO_MODIFIER:
    return "NoModifier";
  case S::ANCHOR_MODIFIER:
    return "AnchorModifier";
  case S::INITS_MODIFIER:
    return "InitsModifier";
  case S::RESIDENT_MODIFIER:
    return "ResidentModifier";
  case S::FLANK_MODIFIER:
    return "FlankModifier";
  case S::TRANSPARENT_MODIFIER:
    return "TransparentModifier";
  case S::OPAQUE_MODIFIER:
    return "OpaqueModifier";
  case S::PRIVATE_MODIFIER:
    return "PrivateModifier";
  case S::PUBLIC_MODIFIER:
    return "PublicModifier";
  case S::EXPORT_MODIFIER:
    return "ExportModifier";
  case S::MUTABLE_MODIFIER:
    return "MutableModifier";
  case S::PARTIALLY_MUTABLE_MODIFIER:
    return "PartiallyMutableModifier";
  case S::CONSTANT_MODIFIER:
    return "ConstantModifier";
  case S::EAGER_MODIFIER:
    return "EagerModifier";
  case S::LAZY_MODIFIER:
    return "LazyModifier";
  case S::POST_MODIFIER:
    return "PostModifier";
  case S::PRE_MODIFIER:
    return "PreModifier";
  case S::ALIAS_MODIFIER:
    return "AliasModifier";
  case S::ENUMERATOR_MODIFIER:
    return "EnumeratorModifier";
  case S::CONCEPT_MODIFIER:
    return "ConceptModifier";
  case S::SPLIT_MODIFIER:
    return "SplitModifier";
  case S::RUNTIME_MODIFIER:
    return "RuntimeModifier";
  case S::COMTIME_MODIFIER:
    return "ComtimeModifier";
  case S::ANYTIME_MODIFIER:
    return "AnytimeModifier";
  case S::PRESET_MODIFIER:
    return "PresetModifier";
  case S::SINGLETON_MODIFIER:
    return "SingletonModifier";
  case S::CAPTURE_MODIFIER:
    return "CaptureModifier";
  case S::STATELESS_MODIFIER:
    return "StatelessModifier";
  case S::LINKED_MODIFIER:
    return "LinkedModifier";
  case S::INLINE_MODIFIER:
    return "InlineModifier";
  case S::PURE_MODIFIER:
    return "PureModifier";
  case S::IMPURE_MODIFIER:
    return "ImpureModifier";
  case S::STANDARD_MANGLE_MODIFIER:
    return "StandardMangleModifier";
  case S::MANGLE_MODIFIER:
    return "MangleModifier";
  case S::PAD_MODIFIER:
    return "PadModifier";
  case S::PACK_MODIFIER:
    return "PackModifier";
  case S::EQUIVOCAL_MODIFIER:
    return "EquivocalModifier";
  case S::LIKELY_MODIFIER:
    return "LikelyModifier";
  case S::UNLIKELY_MODIFIER:
    return "UnlikelyModifier";
  case S::SUPPORTED_MODIFIER:
    return "SupportedModifier";
  case S::DEPRECIATED_MODIFIER:
    return "DepreciatedModifier";
  case S::EXPERIMENTAL_MODIFIER:
    return "ExperimentalModifier";
  case S::MOVABLE_MODIFIER:
    return "MovableModifier";
  case S::COPYABLE_MODIFIER:
    return "CopyableModifier";
  case S::INVARIADIC_MODIFIER:
    return "InvariadicModifier";
  case S::VARIADIC_MODIFIER:
    return "VariadicModifier";
  case S::BEST_LOCATION_MODIFIER:
    return "BestLocationModifier";
  case S::LOCATION_MODIFIER:
    return "LocationModifier";
  case S::TEMPLATE_MODIFIER:
    return "TemplateModifier";
  case S::OVERLOAD_MODIFIER:
    return "OverloadModifier";
  case S::CONSTRAINT_MODIFIER:
    return "ConstraintModifier";
  case S::MANUAL_MODIFIER:
    return "ManualModifier";
  case S::AUTO_MODIFIER:
    return "AutoModifier";
  case S::DIRECT_MODIFIER:
    return "DirectModifier";
  case S::VIRTUAL_MODIFIER:
    return "VirtualModifier";
  case S::RANGER_MODIFIER:
    return "RangerModifier";
  case S::REQUIRE_MODIFIER:
    return "RequireModifier";
  case S::ENSURE_MODIFIER:
    return "EnsureModifier";
  case S::NO_MUT_QUALIFIER:
    return "NoMutQualifier";
  case S::PARTIAL_MUT_QUALIFIER:
    return "PartialMutQualifier";
  case S::MUT_QUALIFIER:
    return "MutQualifier";
  case S::NO_VOLATILE_QUALIFIER:
    return "NoVolatileQualifier";
  case S::VOLATILE_QUALIFIER:
    return "VolatileQualifier";
  case S::NO_ATOMIC_QUALIFIER:
    return "NoAtomicQualifier";
  case S::ATOMIC_QUALIFIER:
    return "AtomicQualifier";
  case S::NO_NULL_TERMINATE_QUALIFIER:
    return "NoNullTerminateQualifier";
  case S::NULL_TERMINATE_QUALIFIER:
    return "NullTerminateQualifier";
  case S::NO_MARGIN_QUALIFIER:
    return "NoMarginQualifier";
  case S::MARGIN_QUALIFIER:
    return "MarginQualifier";
  case S::NO_DETACH_QUALIFIER:
    return "NoDetachQualifier";
  case S::DETACH_QUALIFIER:
    return "DetachQualifier";
  case S::NO_THROWS_QUALIFIER:
    return "NoThrowsQualifier";
  case S::THROWS_QUALIFIER:
    return "ThrowsQualifier";
  case S::ANY_CONCEPT:
    return "AnyConcept";
  case S::TYPE_CONCEPT:
    return "TypeConcept";
  case S::COMTIME_CONCEPT:
    return "ComtimeConcept";
  case S::RUNTIME_CONCEPT:
    return "RuntimeConcept";
  case S::ANYTIME_CONCEPT:
    return "AnytimeConcept";
  case S::RANGE_CONCEPT:
    return "RangeConcept";
  case S::ROUTE_CONCEPT:
    return "RouteConcept";
  case S::FIGURATIVE_CONCEPT:
    return "FigurativeConcept";
  case S::REFLECTION_CONCEPT:
    return "ReflectionConcept";
  case S::PRIMITIVE_CONCEPT:
    return "PrimitiveConcept";
  case S::FITTING_PRIMITIVE_CONCEPT:
    return "FittingPrimitiveConcept";
  case S::PLATFORM_PRIMITIVE_CONCEPT:
    return "PlatformPrimitiveConcept";
  case S::STANDARD_PRIMITIVE_CONCEPT:
    return "StandardPrimitiveConcept";
  case S::SCALED_PRIMITIVE_CONCEPT:
    return "ScaledPrimitiveConcept";
  case S::SIGNED_CONCEPT:
    return "SignedConcept";
  case S::UNSIGNED_CONCEPT:
    return "UnsignedConcept";
  case S::INT_CONCEPT:
    return "IntConcept";
  case S::EXACT_INT_CONCEPT:
    return "ExactIntConcept";
  case S::FAST_INT_CONCEPT:
    return "FastIntConcept";
  case S::LEAST_INT_CONCEPT:
    return "LeastIntConcept";
  case S::FLOAT_CONCEPT:
    return "FloatConcept";
  case S::BINARY_CONCEPT:
    return "BinaryConcept";
  case S::BFLOAT_CONCEPT:
    return "BFloatConcept";
  case S::CONCEPT_CONCEPT:
    return "ConceptConcept";
  case S::SUBTYPE_CONCEPT:
    return "SubtypeConcept";
  case S::ARRAY_CONCEPT:
    return "ArrayConcept";
  case S::REF_CONCEPT:
    return "RefConcept";
  case S::PTR_CONCEPT:
    return "PtrConcept";
  case S::SLICE_CONCEPT:
    return "SliceConcept";
  case S::SPLIT_CONCEPT:
    return "SplitConcept";
  case S::MODULE_CONCEPT:
    return "ModuleConcept";
  case S::IMPORT_CONCEPT:
    return "ImportConcept";
  case S::PORTAL_CONCEPT:
    return "PortalConcept";
  case S::REALIZATION_CONCEPT:
    return "RealizationConcept";
  case S::CONFORMITY_CONCEPT:
    return "ConformityConcept";
  case S::ADAPTION_CONCEPT:
    return "AdaptionConcept";
  case S::TRIP_CONCEPT:
    return "TripConcept";
  case S::JUXT_CONCEPT:
    return "JuxtConcept";
  case S::SPECIALIZATION_SET_CONCEPT:
    return "SpecializationSetConcept";
  case S::ADAPTER_SPECIALIZATION_SET_CONCEPT:
    return "AdapterSpecializationSetConcept";
  case S::PROC_SPECIALIZATION_SET_CONCEPT:
    return "ProcSpecializationSetConcept";
  case S::ARITHMETIC_SEQUENCE_CONCEPT:
    return "ArithmeticSequenceConcept";
  case S::ARITHMETIC_INTERVAL_CONCEPT:
    return "ArithmeticIntervalConcept";
  case S::INFINITE_ARITHMETIC_SEQUENCE_CONCEPT:
    return "InfiniteArithmeticSequenceConcept";
  case S::FINITE_ARITHMETIC_SEQUENCE_CONCEPT:
    return "FiniteArithmeticSequenceConcept";
  case S::EAGER_DECLARATION_CONCEPT:
    return "EagerDeclarationConcept";
  case S::ANCHOR_CONCEPT:
    return "AnchorConcept";
  case S::ENUMERATOR_CONCEPT:
    return "EnumeratorConcept";
  case S::VARIABLE_CONCEPT:
    return "VariableConcept";
  case S::EAGER_VARIABLE_CONCEPT:
    return "EagerVariableConcept";
  case S::POST_EAGER_VARIABLE_CONCEPT:
    return "PostEagerVariableConcept";
  case S::PRE_EAGER_VARIABLE_CONCEPT:
    return "PreEagerVariableConcept";
  case S::PARAMETER_CONCEPT:
    return "ParameterConcept";
  case S::SIGNATURE_PARAMETER_CONCEPT:
    return "SignatureParameterConcept";
  case S::LAYOUT_PARAMETER_CONCEPT:
    return "LayoutParameterConcept";
  case S::PARAMETER_LIST_CONCEPT:
    return "ParameterListConcept";
  case S::SIGNATURE_CONCEPT:
    return "SignatureConcept";
  case S::LAYOUT_CONCEPT:
    return "LayoutConcept";
  case S::PLACEMENT_CONCEPT:
    return "PlacementConcept";
  case S::COMPOSITION_CONCEPT:
    return "CompositionConcept";
  case S::SYNONYM_CONCEPT:
    return "SynonymConcept";
  case S::SYMBOL_TABLE_CONCEPT:
    return "SymbolTableConcept";
  case S::NAMESPACE_CONCEPT:
    return "NamespaceConcept";
  case S::EAGER_SCOPING_STATEMENT_CONCEPT:
    return "EagerScopingStatementConcept";
  case S::IF_STATEMENT_CONCEPT:
    return "IfStatementConcept";
  case S::ELSE_IF_STATEMENT_CONCEPT:
    return "ElseIfStatementConcept";
  case S::ELSE_STATEMENT_CONCEPT:
    return "ElseStatementConcept";
  case S::SWITCH_STATEMENT_CONCEPT:
    return "SwitchStatementConcept";
  case S::CASE_STATEMENT_CONCEPT:
    return "CaseStatementConcept";
  case S::DEFAULT_STATEMENT_CONCEPT:
    return "DefaultStatementConcept";
  case S::FOR_STATEMENT_CONCEPT:
    return "ForStatementConcept";
  case S::WHILE_STATEMENT_CONCEPT:
    return "WhileStatementConcept";
  case S::SCOPE_STATEMENT_CONCEPT:
    return "ScopeStatementConcept";
  case S::IMPLEMENTATION_CONCEPT:
    return "ImplementationConcept";
  case S::OVERLOAD_CONCEPT:
    return "OverloadConcept";
  case S::SPECIALIZATION_CONCEPT:
    return "SpecializationConcept";
  case S::TEMPLATE_CONCEPT:
    return "TemplateConcept";
  case S::CLASS_IMPLEMENTATION_CONCEPT:
    return "ClassImplementationConcept";
  case S::CLASS_OVERLOAD_CONCEPT:
    return "ClassOverloadConcept";
  case S::CLASS_SPECIALIZATION_CONCEPT:
    return "ClassSpecializationConcept";
  case S::CLASS_TEMPLATE_CONCEPT:
    return "ClassTemplateConcept";
  case S::ENUM_IMPLEMENTATION_CONCEPT:
    return "EnumImplementationConcept";
  case S::ENUM_OVERLOAD_CONCEPT:
    return "EnumOverloadConcept";
  case S::ENUM_SPECIALIZATION_CONCEPT:
    return "EnumSpecializationConcept";
  case S::ENUM_TEMPLATE_CONCEPT:
    return "EnumTemplateConcept";
  case S::INTERFACE_IMPLEMENTATION_CONCEPT:
    return "InterfaceImplementationConcept";
  case S::INTERFACE_OVERLOAD_CONCEPT:
    return "InterfaceOverloadConcept";
  case S::INTERFACE_SPECIALIZATION_CONCEPT:
    return "InterfaceSpecializationConcept";
  case S::INTERFACE_TEMPLATE_CONCEPT:
    return "InterfaceTemplateConcept";
  case S::ADAPTER_IMPLEMENTATION_CONCEPT:
    return "AdapterImplementationConcept";
  case S::ADAPTER_OVERLOAD_CONCEPT:
    return "AdapterOverloadConcept";
  case S::ADAPTER_SPECIALIZATION_CONCEPT:
    return "AdapterSpecializationConcept";
  case S::ADAPTER_TEMPLATE_CONCEPT:
    return "AdapterTemplateConcept";
  case S::PROC_IMPLEMENTATION_CONCEPT:
    return "ProcImplementationConcept";
  case S::PROC_OVERLOAD_CONCEPT:
    return "ProcOverloadConcept";
  case S::PROC_SPECIALIZATION_CONCEPT:
    return "ProcSpecializationConcept";
  case S::PROC_TEMPLATE_CONCEPT:
    return "ProcTemplateConcept";
  case S::LAZY_VARIABLE_IMPLEMENTATION_CONCEPT:
    return "LazyVariableImplementationConcept";
  case S::LAZY_VARIABLE_OVERLOAD_CONCEPT:
    return "LazyVariableOverloadConcept";
  case S::LAZY_VARIABLE_SPECIALIZATION_CONCEPT:
    return "LazyVariableSpecializationConcept";
  case S::LAZY_VARIABLE_TEMPLATE_CONCEPT:
    return "LazyVariableTemplateConcept";
  case S::ALIAS_IMPLEMENTATION_CONCEPT:
    return "AliasImplementationConcept";
  case S::ALIAS_OVERLOAD_CONCEPT:
    return "AliasOverloadConcept";
  case S::ALIAS_SPECIALIZATION_CONCEPT:
    return "AliasSpecializationConcept";
  case S::ALIAS_TEMPLATE_CONCEPT:
    return "AliasTemplateConcept";
  case S::CONCEPT_IMPLEMENTATION_CONCEPT:
    return "ConceptImplementationConcept";
  case S::CONCEPT_OVERLOAD_CONCEPT:
    return "ConceptOverloadConcept";
  case S::CONCEPT_SPECIALIZATION_CONCEPT:
    return "ConceptSpecializationConcept";
  case S::CONCEPT_TEMPLATE_CONCEPT:
    return "ConceptTemplateConcept";
  case S::VARIANT_IMPLEMENTATION_CONCEPT:
    return "VariantImplementationConcept";
  case S::VARIANT_OVERLOAD_CONCEPT:
    return "VariantOverloadConcept";
  case S::VARIANT_SPECIALIZATION_CONCEPT:
    return "VariantSpecializationConcept";
  case S::VARIANT_TEMPLATE_CONCEPT:
    return "VariantTemplateConcept";
  case S::ATTRIBUTE_CONCEPT:
    return "AttributeConcept";
  case S::MODIFIER_CONCEPT:
    return "ModifierConcept";
  case S::ANCHOR_MODIFIER_CONCEPT:
    return "AnchorModifierConcept";
  case S::FORKED_INITIALIZATION_MODIFIER_CONCEPT:
    return "ForkedInitializationModifierConcept";
  case S::CONTAINER_MODIFIER_CONCEPT:
    return "ContainerModifierConcept";
  case S::VISIBILITY_MODIFIER_CONCEPT:
    return "VisibilityModifierConcept";
  case S::ACCESS_MODIFIER_CONCEPT:
    return "AccessModifierConcept";
  case S::MUTABILITY_MODIFIER_CONCEPT:
    return "MutabilityModifierConcept";
  case S::EVALUATION_TIME_MODIFIER_CONCEPT:
    return "EvaluationTimeModifierConcept";
  case S::DIALECT_MODIFIER_CONCEPT:
    return "DialectModifierConcept";
  case S::TENURE_MODIFIER_CONCEPT:
    return "TenureModifierConcept";
  case S::INITIALIZATION_TIME_MODIFIER_CONCEPT:
    return "InitializationTimeModifierConcept";
  case S::CLOSURE_MODIFIER_CONCEPT:
    return "ClosureModifierConcept";
  case S::LINKAGE_MODIFIER_CONCEPT:
    return "LinkageModifierConcept";
  case S::SIDE_EFFECTS_MODIFIER_CONCEPT:
    return "SideEffectsModifierConcept";
  case S::MANGLE_MODIFIER_CONCEPT:
    return "MangleModifierConcept";
  case S::PACK_MODIFIER_CONCEPT:
    return "PackModifierConcept";
  case S::BRANCH_TREND_MODIFIER_CONCEPT:
    return "BranchTrendModifierConcept";
  case S::SUPPORT_NOTICE_MODIFIER_CONCEPT:
    return "SupportNoticeModifierConcept";
  case S::ADDRESS_STABILITY_MODIFIER_CONCEPT:
    return "AddressStabilityModifierConcept";
  case S::VARIADIC_MODIFIER_CONCEPT:
    return "VariadicModifierConcept";
  case S::OFFSET_MODIFIER_CONCEPT:
    return "OffsetModifierConcept";
  case S::LAZY_DECLARATION_MODIFIER_CONCEPT:
    return "LazyDeclarationModifierConcept";
  case S::CONSTRAINT_MODIFIER_CONCEPT:
    return "ConstraintModifierConcept";
  case S::DEDUCTION_MODIFIER_CONCEPT:
    return "DeductionModifierConcept";
  case S::VIRTUALITY_MODIFIER_CONCEPT:
    return "VirtualityModifierConcept";
  case S::RANGER_MODIFIER_CONCEPT:
    return "RangerModifierConcept";
  case S::REQUIRE_MODIFIER_CONCEPT:
    return "RequireModifierConcept";
  case S::ENSURE_MODIFIER_CONCEPT:
    return "EnsureModifierConcept";
  case S::QUALIFIER_CONCEPT:
    return "QualifierConcept";
  case S::MUT_QUALIFIER_CONCEPT:
    return "MutQualifierConcept";
  case S::VOLATILE_QUALIFIER_CONCEPT:
    return "VolatileQualifierConcept";
  case S::ATOMIC_QUALIFIER_CONCEPT:
    return "AtomicQualifierConcept";
  case S::NULL_TERMINATE_QUALIFIER_CONCEPT:
    return "NullTerminateQualifierConcept";
  case S::DETATCH_QUALIFIER_CONCEPT:
    return "DetatchQualifierConcept";
  case S::MARGIN_QUALIFIER_CONCEPT:
    return "MarginQualifierConcept";
  case S::THROWS_QUALIFIER_CONCEPT:
    return "ThrowsQualifierConcept";
  case S::VARIADIC_ARGUMENTS_TYPE:
    return "VariadicArguments";
  case S::QUALIFIED_TYPE:
    return "QualifiedType";
  case S::ARRAY_SUBTYPE:
    return "Array";
  case S::REF_SUBTYPE:
    return "Ref";
  case S::PTR_SUBTYPE:
    return "Ptr";
  case S::SLICE_SUBTYPE:
    return "Slice";
  case S::SPLIT_SUBTYPE:
    return "Split";
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return "InferenceCountArray";
  case S::GENERIC_SUBSYMBOL:
    return "GenericSubsymbol";
  case S::MODULE:
    return "Module";
  case S::IMPORT:
    return "Import";
  case S::PORTAL:
    return "Portal";
  case S::REALIZATION:
    return "Realization";
  case S::CONFORMITY:
    return "Conformity";
  case S::ADAPTION:
    return "Adaption";
  case S::JUXT_LIST_TYPE:
    return "JuxtListType";
  case S::TRIP:
    return "Trip";
  case S::SPECIALIZATION_SET_ARGUMENT:
    return "SpecializationSetArgument";
  case S::SPECIALIATION_SET:
    return "SpecializationSet";
  case S::ADAPTER_SPECIALIZATION_SET:
    return "AdapterSpecializationSet";
  case S::PROC_SPECIALIZATION_SET:
    return "ProcSpecializationSet";
  case S::ARITHMETIC_INTERVAL_TYPE:
    return "ArithmeticInterval";
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "InfiniteArithmeticSequence";
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "FiniteArithmeticSequence";
  case S::ANCHOR:
    return "Anchor";
  case S::ENUMERATOR:
    return "Enumerator";
  case S::POST_EAGER_VARIABLE:
    return "PostEagerVariable";
  case S::PRE_EAGER_VARIABLE:
    return "PreEagerVariable";
  case S::PARAMETER:
    return "Parameter";
  case S::SIGNATURE_TYPE:
    return "SignatureType";
  case S::LAYOUT_TYPE:
    return "LayoutType";
  case S::PLACEMENT_TYPE:
    return "PlacementType";
  case S::COMPOSITION_TYPE:
    return "CompositionType";
  case S::SYNONYM_TYPE:
    return "SynonymnType";
  case S::C_TABLE:
    return "CTable";
  case S::NAMESPACE:
    return "Namespace";
  case S::IF_STATEMENT:
    return "IfStatement";
  case S::ELSE_IF_STATEMENT:
    return "ElseIfStatement";
  case S::ELSE_STATEMENT:
    return "ElseStatement";
  case S::SWITCH_STATEMENT:
    return "SwitchStatement";
  case S::CASE_STATEMENT:
    return "CaseStatement";
  case S::DEFAULT_STATEMENT:
    return "DefaultStatement";
  case S::FOR_STATEMENT:
    return "ForStatement";
  case S::WHILE_STATEMENT:
    return "WhileStatement";
  case S::SCOPE_STATEMENT:
    return "ScopeStatement";
  case S::CLASS_OVERLOAD:
    return "ClassOverload";
  case S::ENUM_OVERLOAD:
    return "EnumOverload";
  case S::INTERFACE_OVERLOAD:
    return "InterfaceOverload";
  case S::ADAPTER_OVERLOAD:
    return "AdapterOverload";
  case S::PROC_OVERLOAD:
    return "ProcOverload";
  case S::LAZY_VARIABLE_OVERLOAD:
    return "LazyVariableOverload";
  case S::ALIAS_OVERLOAD:
    return "AliasOverload";
  case S::CONCEPT_OVERLOAD:
    return "ConceptOverload";
  case S::VARIANT_OVERLOAD:
    return "VariantOverload";
  case S::CLASS_SPECIALIZATION:
    return "ClassSpecialization";
  case S::ENUM_SPECIALIZATION:
    return "EnumSpecialization";
  case S::INTERFACE_SPECIALIZATION:
    return "InterfaceSpecialization";
  case S::ADAPTER_SPECIALIZATION:
    return "AdapterSpecialization";
  case S::PROC_SPECIALIZATION:
    return "ProcSpecialization";
  case S::LAZY_VARIABLE_SPECIALIZATION:
    return "LazyVariableSpecialization";
  case S::ALIAS_SPECIALIZATION:
    return "AliasSpecialization";
  case S::CONCEPT_SPECIALIZATION:
    return "ConceptSpecialization";
  case S::VARIANT_SPECIALIZATION:
    return "VariantSpecialization";
  case S::CLASS_TEMPLATE:
    return "ClassTemplate";
  case S::ENUM_TEMPLATE:
    return "EnumTemplate";
  case S::INTERFACE_TEMPLATE:
    return "InterfaceTemplate";
  case S::ADAPTER_TEMPLATE:
    return "AdapterTemplate";
  case S::PROC_TEMPLATE:
    return "ProcTemplate";
  case S::LAZY_VARIABLE_TEMPLATE:
    return "LazyVariableTemplate";
  case S::ALIAS_TEMPLATE:
    return "AliasTemplate";
  case S::CONCEPT_TEMPLATE:
    return "ConceptTemplate";
  case S::VARIANT_TEMPLATE:
    return "VariantTemplate";
  case S::CLASS_POLYMORPH:
    return "ClassPolymorph";
  case S::ENUM_POLYMORPH:
    return "EnumPolymorph";
  case S::INTERFACE_POLYMORPH:
    return "InterfacePolymorph";
  case S::ADAPTER_POLYMORPH:
    return "AdapterPolymorph";
  case S::PROC_POLYMORPH:
    return "ProcPolymorph";
  case S::LAZY_VARIABLE_POLYMORPH:
    return "LazyVariablePolymorph";
  case S::ALIAS_POLYMORPH:
    return "AliasPolymorph";
  case S::CONCEPT_POLYMORPH:
    return "ConceptPolymorph";
  case S::VARIANT_POLYMORPH:
    return "VariantPolymorph";
  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline llvm::StringRef getName(rq::EvaluationState state) {
  using ES = rq::EvaluationState;
  switch (state) {
  case ES::NONE:
    break;
  case ES::SURVEYED:
    return "surveyed";
  case ES::DECLARING:
    return "declaring";
  case ES::DECLARED:
    return "declared";
  case ES::IMPLEMENTING:
    return "implementing";
  case ES::IMPLEMENTED:
    return "implemented";
  case ES::ERROR:
    return "error";
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind) : Entity(rq::getId(kind)) {
  RQ_ASSERT(rq::getIsInRange(kind), "symbol kind out of range");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbolKind(entity.getId());
}

template <typename ParentParam, rq::SymbolInfoFlags FLAG_PARAM>
template <typename... ParentConstructorArgsNParam>
RQ_ALWAYS_INLINE SimpleBranch<ParentParam, FLAG_PARAM>::SimpleBranch(
    rq::SymbolKind kind, ParentConstructorArgsNParam &&...n_args)
    : Parent(kind, std::forward<ParentConstructorArgsNParam>(n_args)...) {
  RQ_ASSERT(rq::getHasAll(rq::getInfoFlags(kind), FLAG), "not of parent");
}

template <typename ParentParam, rq::SymbolInfoFlags FLAG_PARAM>
[[nodiscard]] RQ_ALWAYS_INLINE bool
SimpleBranch<ParentParam, FLAG_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::SymbolInfoFlags flags = entity.getSymbolInfoFlags();
  return rq::getHasAll(flags, FLAG);
}

template <typename ParentParam, rq::SymbolKind KIND_PARAM>
template <typename... ParentConstructorArgsNParam>
RQ_ALWAYS_INLINE SimpleLeaf<ParentParam, KIND_PARAM>::SimpleLeaf(
    ParentConstructorArgsNParam &&...n_args)
    : Parent(KIND, std::forward<ParentConstructorArgsNParam>(n_args)...) {}

template <typename ParentParam, rq::SymbolKind KIND_PARAM>
[[nodiscard]] RQ_ALWAYS_INLINE bool
SimpleLeaf<ParentParam, KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(KIND_PARAM);
}

RQ_ALWAYS_INLINE ScaledPrimitiveType::ScaledPrimitiveType(rq::SymbolKind kind,
                                                          unsigned depth)
    : Symbol(kind), _depth(depth) {
  RQ_ASSERT(rq::getIsScaledPrimitiveType(kind), "not scaled primitive type");
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned ScaledPrimitiveType::getDepth() const {
  return this->_depth;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsScaledPrimitiveType(entity.getUnsafeSymbolKind());
}

inline void ScaledPrimitiveType::Profile(llvm::FoldingSetNodeID &inout_id,
                                         rq::SymbolKind kind, unsigned depth) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddInteger(depth);
}

RQ_ALWAYS_INLINE void
ScaledPrimitiveType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  ScaledPrimitiveType::Profile(inout_id, this->getSymbolKind(),
                               this->getDepth());
}

RQ_ALWAYS_INLINE BuiltinConcept::BuiltinConcept(rq::SymbolKind kind,
                                                rq::Entity &predecessors)
    : Symbol(kind), _predecessor_ptr(&predecessors) {
  RQ_ASSERT(rq::getHasAll(rq::getInfoFlags(kind),
                          rq::SymbolInfoFlags::BUILTIN_CONCEPT),
            "not builtin concept");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
BuiltinConcept::getPredecessor() const {
  return rq::dereferencePtr(this->_predecessor_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &BuiltinConcept::getPredecessor() {
  return rq::dereferencePtr(this->_predecessor_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
BuiltinConcept::classof(const rq::Entity *entity_ptr) {
  if (!Symbol::classof(entity_ptr)) {
    return false;
  }
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  rq::SymbolInfoFlags flags = entity.getSymbolInfoFlags();
  return rq::getHasAll(flags, rq::SymbolInfoFlags::BUILTIN_CONCEPT);
}

RQ_ALWAYS_INLINE QualifiedType::QualifiedType(rq::Symbol &type,
                                              rq::QualifierFuseFlags qualifiers,
                                              unsigned margin)
    : Symbol(rq::SymbolKind::QUALIFIED_TYPE), _type_ptr(&type),
      _qualifier_fuse_flags(qualifiers), _margin(margin) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
QualifiedType::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &QualifiedType::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifierFuseFlags
QualifiedType::getFlags() const {
  return this->_qualifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned QualifiedType::getMargin() const {
  return this->_margin;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
QualifiedType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::QUALIFIED_TYPE);
}

inline void QualifiedType::Profile(llvm::FoldingSetNodeID &inout_id,
                                   const rq::Symbol &type,
                                   rq::QualifierFuseFlags qualifiers,
                                   unsigned margin) {
  inout_id.AddPointer(&type);
  inout_id.AddInteger(rq::getUnderlyingValue(qualifiers));
  inout_id.AddInteger(margin);
}

RQ_ALWAYS_INLINE void
QualifiedType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  QualifiedType::Profile(inout_id, this->getType(), this->getFlags(),
                         this->getMargin());
}

RQ_ALWAYS_INLINE Subentity::Subentity(rq::SymbolKind kind, rq::Entity &child)
    : Symbol(kind), _child_ptr(&child) {
  RQ_ASSERT(Subentity::classof(this), "not subentity");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &Subentity::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &Subentity::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Subentity::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSubentity(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Subtype::Subtype(rq::SymbolKind kind, rq::QualifiedType &child)
    : Subentity(kind, child) {
  RQ_ASSERT(Subtype::classof(this), "not subtype");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
Subtype::getChildType() const {
  return llvm::cast<rq::QualifiedType>(this->getChild());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &Subtype::getChildType() {
  return llvm::cast<rq::QualifiedType>(this->getChild());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Subtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSubtype(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(rq::QualifiedType &child,
                                            std::size_t &count)
    : Subtype(rq::SymbolKind::ARRAY_SUBTYPE, child), _count(&count) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::size_t ArraySubtype::getCount() const {
  return rq::dereferencePtr(this->_count);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ARRAY_SUBTYPE);
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  ArraySubtype::Profile(inout_id, this->getChildType(), this->getCount());
}

RQ_ALWAYS_INLINE void ArraySubtype::Profile(llvm::FoldingSetNodeID &inout_id,
                                            const rq::QualifiedType &child,
                                            std::size_t count) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(count);
}

RQ_ALWAYS_INLINE SimpleSubtype::SimpleSubtype(rq::SymbolKind kind,
                                              rq::QualifiedType &child)
    : Subtype(kind, child) {
  RQ_ASSERT(SimpleSubtype::classof(this), "not simple subtype");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SimpleSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return Subtype::classof(entity_ptr) &&
         entity.getId() != rq::getId(rq::SymbolKind::ARRAY_SUBTYPE);
}

inline void SimpleSubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  SimpleSubtype::Profile(inout_id, this->getChildType());
}

RQ_ALWAYS_INLINE void SimpleSubtype::Profile(llvm::FoldingSetNodeID &inout_id,
                                             const rq::QualifiedType &child) {
  inout_id.AddPointer(&child);
}

RQ_ALWAYS_INLINE Subsymbol::Subsymbol(rq::SymbolKind kind, rq::Symbol &child)
    : Subentity(kind, child) {
  RQ_ASSERT(Subsymbol::classof(this), "not subsymbol");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
Subsymbol::getChildSymbol() const {
  return llvm::cast<rq::Symbol>(this->getChild());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &Subsymbol::getChildSymbol() {
  return llvm::cast<rq::Symbol>(this->getChild());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Subsymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSubsymbol(entity.getUnsafeSymbolKind());
}

inline void Subsymbol::Profile(llvm::FoldingSetNodeID &inout_id,
                               const rq::Symbol &child) {
  inout_id.AddPointer(&child);
}

RQ_ALWAYS_INLINE void
Subsymbol::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Subsymbol::Profile(inout_id, this->getChildSymbol());
}

RQ_ALWAYS_INLINE Adaption::Adaption(rq::QualifiedType &reciever,
                                    rq::InterfaceImplementation &interface,
                                    rq::AdapterImplementation &adapter)
    : Symbol(rq::SymbolKind::ADAPTION), _reciever_ptr(&reciever),
      _interface_ptr(&interface), _adapter_ptr(&adapter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
Adaption::getReciever() const {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &Adaption::getReciever() {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Adaption::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Adaption::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
Adaption::getAdapter() const {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &
Adaption::getAdapter() {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Adaption::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTION);
}

inline void Adaption::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Adaption::Profile(inout_id, this->getInterface(), this->getAdapter());
}

RQ_ALWAYS_INLINE void
Adaption::Profile(llvm::FoldingSetNodeID &inout_id,
                  const rq::InterfaceImplementation &interface,
                  const rq::AdapterImplementation &adapter) {
  inout_id.AddPointer(&interface);
  inout_id.AddPointer(&adapter);
}

RQ_ALWAYS_INLINE
Realization::Realization(rq::QualifiedType &reciever,
                         rq::InterfaceImplementation &interface)
    : Symbol(rq::SymbolKind::REALIZATION), _reciever_ptr(&reciever),
      _interface_ptr(&interface) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
Realization::getReciever() const {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &Realization::getReciever() {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Realization::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Realization::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Realization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::REALIZATION);
}

inline void Realization::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Realization::Profile(inout_id, this->getReciever(), this->getInterface());
}

RQ_ALWAYS_INLINE void
Realization::Profile(llvm::FoldingSetNodeID &inout_id,
                     const rq::QualifiedType &reciever,
                     const rq::InterfaceImplementation &interface) {
  inout_id.AddPointer(&reciever);
  inout_id.AddPointer(&interface);
}

RQ_ALWAYS_INLINE Conformity::Conformity(rq::InterfaceImplementation &interface,
                                        rq::AdapterImplementation &adapter)
    : Symbol(rq::SymbolKind::CONFORMITY), _interface_ptr(&interface),
      _adapter_ptr(&adapter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Conformity::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Conformity::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
Conformity::getAdapter() const {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &
Conformity::getAdapter() {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Conformity::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONFORMITY);
}

inline void Conformity::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Conformity::Profile(inout_id, this->getInterface(), this->getAdapter());
}

RQ_ALWAYS_INLINE void
Conformity::Profile(llvm::FoldingSetNodeID &inout_id,
                    const rq::InterfaceImplementation &interface,
                    const rq::AdapterImplementation &adapter) {
  inout_id.AddPointer(&interface);
  inout_id.AddPointer(&adapter);
}

RQ_ALWAYS_INLINE
JuxtListType::JuxtListType(rq::NodeList<rq::QualifiedType> &&list)
    : Symbol(rq::SymbolKind::JUXT_LIST_TYPE), _type_list(std::move(list)) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListSubrange<rq::QualifiedType>
JuxtListType::getItemSubrange() {
  return rq::NodeListSubrange<rq::QualifiedType>(this->_type_list.begin(),
                                                 this->_type_list.end());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::QualifiedType>
JuxtListType::getItemSubrange() const {
  return rq::ConstNodeListSubrange<rq::QualifiedType>(this->_type_list.begin(),
                                                      this->_type_list.end());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::QualifiedType>
JuxtListType::getConstItemSubrange() const {
  return this->getItemSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
JuxtListType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::JUXT_LIST_TYPE);
}

inline void
JuxtListType::Profile(llvm::FoldingSetNodeID &inout_id,
                      rq::ConstNodeListRef<rq::QualifiedType> list) {
  list.Profile(inout_id);
}

RQ_ALWAYS_INLINE void
JuxtListType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  JuxtListType::Profile(inout_id, this->_type_list);
}

RQ_ALWAYS_INLINE Trip::Trip(rq::NodeList<rq::Entity> &&list)
    : Symbol(rq::SymbolKind::TRIP), _entity_list(std::move(list)) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListSubrange<rq::Entity>
Trip::getItemSubrange() {
  return rq::NodeListSubrange<rq::Entity>(this->_entity_list.begin(),
                                          this->_entity_list.end());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::Entity>
Trip::getItemSubrange() const {
  return rq::ConstNodeListSubrange<rq::Entity>(this->_entity_list.begin(),
                                               this->_entity_list.end());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::Entity>
Trip::getConstItemSubrange() const {
  return this->getItemSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Trip::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::TRIP);
}

inline void Trip::Profile(llvm::FoldingSetNodeID &inout_id,
                          rq::ConstNodeListRef<rq::Entity> list) {
  list.Profile(inout_id);
}

RQ_ALWAYS_INLINE void Trip::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Trip::Profile(inout_id, this->_entity_list);
}

RQ_ALWAYS_INLINE SynonymType::SynonymType(rq::Symbol &original)
    : Symbol(rq::SymbolKind::SYNONYM_TYPE), _original_ptr(&original) {}

[[nodiscard]] inline const rq::Symbol &SynonymType::getOriginal() const {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] inline rq::Symbol &SynonymType::getOriginal() {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SynonymType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SYNONYM_TYPE);
}

RQ_ALWAYS_INLINE
ArithmeticSequenceType::ArithmeticSequenceType(
    rq::SymbolKind kind, rq::QualifiedType &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step)
    : Symbol(kind), _child_ptr(&child), _condition(condition), _step(step) {
  using S = rq::SymbolKind;
  RQ_ASSERT(rq::getIsArithmeticSequenceType(kind), "not arithmetic sequence");
  RQ_ASSERT(kind != S::ARITHMETIC_INTERVAL_TYPE ||
                step == rq::ArithmeticStep::NONE,
            "interval must have no step");
  RQ_ASSERT(kind != S::INFINITE_ARITHMETIC_SEQUENCE_TYPE ||
                condition == rq::ArithmeticCondition::NONE,
            "infinite must have no condition");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
ArithmeticSequenceType::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &
ArithmeticSequenceType::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticCondition
ArithmeticSequenceType::getCondition() const {
  return this->_condition;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticStep
ArithmeticSequenceType::getStep() const {
  return this->_step;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ArithmeticSequenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsArithmeticSequenceType(entity.getUnsafeSymbolKind());
}

inline void
ArithmeticSequenceType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  ArithmeticSequenceType::Profile(inout_id, this->getChild(),
                                  this->getCondition(), this->getStep());
}

RQ_ALWAYS_INLINE void ArithmeticSequenceType::Profile(
    llvm::FoldingSetNodeID &inout_id, const rq::QualifiedType &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(rq::getUnderlyingValue(condition));
  inout_id.AddInteger(rq::getUnderlyingValue(step));
}

RQ_ALWAYS_INLINE
SpecializationSetArgument::SpecializationSetArgument(
    rq::Name name, rq::Entity &value, rq::SpecializationSetArgument *next_ptr)
    : Symbol(rq::SymbolKind::SPECIALIZATION_SET_ARGUMENT), _name(name),
      _value_ptr(&value), _next_ptr(next_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name
SpecializationSetArgument::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
SpecializationSetArgument::getValue() const {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
SpecializationSetArgument::getValue() {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SpecializationSetArgument::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::SPECIALIZATION_SET_ARGUMENT);
}

inline void
SpecializationSetArgument::Profile(llvm::FoldingSetNodeID &inout_id) const {
  SpecializationSetArgument::Profile(inout_id, this->getName(),
                                     this->getValue(), this->_next_ptr);
}

RQ_ALWAYS_INLINE void
SpecializationSetArgument::Profile(llvm::FoldingSetNodeID &inout_id,
                                   rq::Name name, const rq::Entity &value,
                                   rq::SpecializationSetArgument *next_ptr) {
  inout_id.Add(name);
  inout_id.AddPointer(&value);
  inout_id.AddPointer(next_ptr);
}

RQ_ALWAYS_INLINE
SpecializationSet::SpecializationSet(rq::SymbolKind kind,
                                     rq::SpecializationSetArgument *first_ptr)
    : Symbol(kind), _first_ptr(first_ptr) {
  RQ_ASSERT(rq::getIsSpecializationSet(kind), "not specialization set");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::SpecializationSetArgument>
SpecializationSet::getArgumentSubrange() {
  return rq::NextSubrange<rq::SpecializationSetArgument>(
      rq::NextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::NextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]]
RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::SpecializationSetArgument>
SpecializationSet::getArgumentSubrange() const {
  return rq::ConstNextSubrange<rq::SpecializationSetArgument>(
      rq::ConstNextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::ConstNextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::SpecializationSetArgument>
    SpecializationSet::getConstArgumentSubrange() const {
  return rq::ConstNextSubrange<rq::SpecializationSetArgument>(
      rq::ConstNextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::ConstNextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSpecializationSet(entity.getUnsafeSymbolKind());
}

inline void SpecializationSet::Profile(llvm::FoldingSetNodeID &inout_id) const {
  SpecializationSet::Profile(inout_id, this->getSymbolKind(), this->_first_ptr);
}

RQ_ALWAYS_INLINE void
SpecializationSet::Profile(llvm::FoldingSetNodeID &inout_id,
                           rq::SymbolKind kind,
                           const rq::SpecializationSetArgument *first_ptr) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(first_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcSpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROC_SPECIALIZATION_SET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterSpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::ADAPTER_SPECIALIZATION_SET);
}

RQ_ALWAYS_INLINE
ParameterDetail::ParameterDetail(rq::Name name, rq::QualifiedType &type,
                                 rq::ModifierFuseFlags modifier_fuse_flags,
                                 rq::ParameterInfoFlags param_info_flags,
                                 rq::Entity *default_ptr)
    : _name(name), _type_ptr(&type), _modifier_fuse_flags(modifier_fuse_flags),
      _param_info_flags(param_info_flags), _default_ptr(default_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name ParameterDetail::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
ParameterDetail::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &ParameterDetail::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
ParameterDetail::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
ParameterDetail::getParameterInfoFlags() const {
  return this->_param_info_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
ParameterDetail::getDefaultPtr() const {
  return this->_default_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *ParameterDetail::getDefaultPtr() {
  return this->_default_ptr;
}

RQ_ALWAYS_INLINE Parameter::Parameter(rq::Parameter *next_ptr, rq::Name name,
                                      rq::QualifiedType &type,
                                      rq::ModifierFuseFlags modifier_flags,
                                      rq::ParameterInfoFlags param_flags,
                                      rq::Entity *default_ptr)
    : Symbol(rq::SymbolKind::PARAMETER), _next_ptr(next_ptr), _name(name),
      _type_ptr(&type), _modifier_fuse_flags(modifier_flags),
      _param_info_flags(param_flags), _default_ptr(default_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name Parameter::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
Parameter::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &Parameter::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
Parameter::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
Parameter::getParameterFlags() const {
  return this->_param_info_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
Parameter::getDefaultPtr() const {
  return this->_default_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *Parameter::getDefaultPtr() {
  return this->_default_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Parameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PARAMETER);
}

inline void Parameter::Profile(llvm::FoldingSetNodeID &inout_id) const {
  Parameter::Profile(inout_id, this->_next_ptr, this->getName(),
                     this->getType(), this->getModifierFuseFlags(),
                     this->getParameterFlags(), this->getDefaultPtr());
}

RQ_ALWAYS_INLINE void Parameter::Profile(
    llvm::FoldingSetNodeID &inout_id, const rq::Parameter *next_ptr,
    rq::Name name, const rq::QualifiedType &type,
    rq::ModifierFuseFlags modifier_fuse_flags,
    rq::ParameterInfoFlags param_flags, const rq::Entity *default_ptr) {
  inout_id.AddPointer(next_ptr);
  inout_id.Add(name);
  inout_id.AddPointer(&type);
  inout_id.AddInteger(rq::getUnderlyingValue(modifier_fuse_flags));
  inout_id.AddInteger(rq::getUnderlyingValue(param_flags));
  inout_id.AddPointer(default_ptr);
}

RQ_ALWAYS_INLINE CompositionType::CompositionType(
    rq::NodeList<rq::InterfaceImplementation> &&interface_list)
    : Symbol(rq::SymbolKind::COMPOSITION_TYPE),
      _interface_list(std::move(interface_list)) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
CompositionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::COMPOSITION_TYPE);
}

inline void CompositionType::Profile(
    llvm::FoldingSetNodeID &inout_id,
    rq::ConstNodeListRef<rq::InterfaceImplementation> interface_list) {
  interface_list.Profile(inout_id);
}

RQ_ALWAYS_INLINE void
CompositionType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  CompositionType::Profile(inout_id, this->_interface_list);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundPositionalParametersEnd() const {
  return rq::getHasAll(this->_found_parameter_marks,
                       rq::ParameterInfoFlags::POSITIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundNonpositionalParametersBegin() const {
  return rq::getHasAll(this->_found_parameter_marks,
                       rq::ParameterInfoFlags::NONPOSITIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundLockedParametersBegin() const {
  return rq::getHasAll(this->_found_parameter_marks,
                       rq::ParameterInfoFlags::LOCKED);
}

RQ_ALWAYS_INLINE void ParameterListDetail::appendPositionaParametersEnd() {
  RQ_ASSERT(!this->getFoundPositionalParametersEnd(), "already found");
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "locked already began");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::POSITIONAL;
  this->_flags &= ~rq::ParameterInfoFlags::POSITIONAL;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendNonpositionalParametersBegin() {
  RQ_ASSERT(!this->getFoundNonpositionalParametersBegin(), "already found");
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "locked already began");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::NONPOSITIONAL;
  this->_flags |= rq::ParameterInfoFlags::NONPOSITIONAL;
}

RQ_ALWAYS_INLINE void ParameterListDetail::appendLockedParametersBegin() {
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "already found");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::LOCKED;
  this->_flags = rq::ParameterInfoFlags::LOCKED;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendParameter(rq::Name name, rq::QualifiedType &type,
                                     rq::ModifierFuseFlags modifier_fuse_flags,
                                     rq::Entity *default_ptr) {
  this->_parameter_detail_list.emplace_back(name, type, modifier_fuse_flags,
                                            this->_flags, default_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::ParameterDetail>
ParameterListDetail::getParameterDetailSpan() const {
  return this->_parameter_detail_list;
}

RQ_ALWAYS_INLINE
ParameterList::ParameterList(rq::SymbolKind kind,
                             rq::Parameter *first_parameter_ptr)
    : Symbol(kind), _first_ptr(first_parameter_ptr) {
  RQ_ASSERT(rq::getIsParameterList(kind), "not parameter list");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
ParameterList::getParameterSubrange() const {
  return rq::ConstNextSubrange<rq::Parameter>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_ptr),
      rq::ConstNextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
ParameterList::getConstParameterSubrange() const {
  return rq::ConstNextSubrange<rq::Parameter>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_ptr),
      rq::ConstNextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Parameter>
ParameterList::getParameterSubrange() {
  return rq::NextSubrange<rq::Parameter>(
      rq::NextIterator<rq::Parameter>(this->_first_ptr),
      rq::NextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsParameterList(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE
SignatureType::SignatureType(rq::Parameter *first_parameter_ptr,
                             rq::QualifiedType &return_type)
    : ParameterList(rq::SymbolKind::SIGNATURE_TYPE, first_parameter_ptr),
      _return_type_ptr(&return_type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &
SignatureType::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &
SignatureType::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SignatureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SIGNATURE_TYPE);
}

inline void SignatureType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  SignatureType::Profile(inout_id, this->_first_ptr, this->getReturnType());
}

RQ_ALWAYS_INLINE void
SignatureType::Profile(llvm::FoldingSetNodeID &inout_id,
                       const rq::Parameter *first_parameter_ptr,
                       const rq::QualifiedType &return_type) {
  inout_id.AddPointer(first_parameter_ptr);
  inout_id.AddPointer(&return_type);
}

RQ_ALWAYS_INLINE LayoutType::LayoutType(rq::Parameter *first_parameter_ptr)
    : ParameterList(rq::SymbolKind::LAYOUT_TYPE, first_parameter_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LayoutType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAYOUT_TYPE);
}

inline void LayoutType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  LayoutType::Profile(inout_id, this->_first_ptr);
}

RQ_ALWAYS_INLINE void
LayoutType::Profile(llvm::FoldingSetNodeID &inout_id,
                    const rq::Parameter *first_parameter_ptr) {
  inout_id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE
PlacementType::PlacementType(rq::ProcImplementation &function)
    : Symbol(rq::SymbolKind::PLACEMENT_TYPE), _proc_ptr(&function) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcImplementation &
PlacementType::getProc() const {
  return rq::dereferencePtr(this->_proc_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcImplementation &
PlacementType::getProc() {
  return rq::dereferencePtr(this->_proc_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
PlacementType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PLACEMENT_TYPE);
}

inline void PlacementType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  PlacementType::Profile(inout_id, this->getProc());
}

RQ_ALWAYS_INLINE void
PlacementType::Profile(llvm::FoldingSetNodeID &inout_id,
                       const rq::ProcImplementation &function) {
  inout_id.AddPointer(&function);
}

RQ_ALWAYS_INLINE Polymorph::Polymorph(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsPolymorph(kind), "not polymorph");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Polymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsPolymorph(entity.getUnsafeSymbolKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation>(
      rq::NextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getConstOverloadSubrange() const {
  return this->getOverloadSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template>
Polymorph::getTemplateSubrange() {
  return rq::NextSubrange<rq::Template>(
      rq::NextIterator<rq::Template>(this->_first_template_ptr),
      rq::NextIterator<rq::Template>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
Polymorph::getTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template>(
      rq::ConstNextIterator<rq::Template>(this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
Polymorph::getConstTemplateSubrange() const {
  return this->getTemplateSubrange();
}

RQ_ALWAYS_INLINE void
ClassPolymorph::addClassTemplate(rq::ClassTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
EnumPolymorph::addEnumTemplate(rq::EnumTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
InterfacePolymorph::addInterfaceTemplate(rq::InterfaceTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
AdapterPolymorph::addAdapterTemplate(rq::AdapterTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
ProcPolymorph::addProcTemplate(rq::ProcTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void LazyVariablePolymorph::addLazyVariableTemplate(
    rq::LazyVariableTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
AliasPolymorph::addAliasOverload(rq::AliasOverload &overload) {
  rq::assignSingleValue(overload._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &overload;
}

RQ_ALWAYS_INLINE void
AliasPolymorph::addAliasTemplate(rq::AliasTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
ConceptPolymorph::addConceptOverload(rq::ConceptOverload &overload) {
  rq::assignSingleValue(overload._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &overload;
}

RQ_ALWAYS_INLINE void
ConceptPolymorph::addConceptTemplate(rq::ConceptTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE void
VariantPolymorph::addConceptOverload(rq::VariantOverload &overload) {
  rq::assignSingleValue(overload._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &overload;
}

RQ_ALWAYS_INLINE void
VariantPolymorph::addVariantTemplate(rq::VariantTemplate &template_) {
  rq::assignSingleValue(template_._next_ptr, this->_first_template_ptr);
  this->_first_template_ptr = &template_;
}

RQ_ALWAYS_INLINE AliasPolymorph::AliasPolymorph()
    : Polymorph(rq::SymbolKind::ALIAS_POLYMORPH) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AliasPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ALIAS_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::AliasOverload>
    AliasPolymorph::getAliasOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::AliasOverload>(
      rq::NextIterator<rq::Implementation, rq::AliasOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::AliasOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AliasOverload>
    AliasPolymorph::getAliasOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AliasOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::AliasOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AliasOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AliasOverload>
    AliasPolymorph::getConstAliasOverloadSubrange() const {
  return this->getAliasOverloadSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::AliasTemplate>
AliasPolymorph::getAliasTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::AliasTemplate>(
      rq::NextIterator<rq::Template, rq::AliasTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::AliasTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AliasTemplate>
    AliasPolymorph::getAliasTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::AliasTemplate>(
      rq::ConstNextIterator<rq::Template, rq::AliasTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::AliasTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AliasTemplate>
    AliasPolymorph::getConstAliasTemplateSubrange() const {
  return this->getAliasTemplateSubrange();
}

RQ_ALWAYS_INLINE VariantPolymorph::VariantPolymorph()
    : Polymorph(rq::SymbolKind::VARIANT_POLYMORPH) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
VariantPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::VARIANT_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::VariantOverload>
    VariantPolymorph::getVariantOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::VariantOverload>(
      rq::NextIterator<rq::Implementation, rq::VariantOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::VariantOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::VariantOverload>
    VariantPolymorph::getVariantOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::VariantOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::VariantOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::VariantOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::VariantOverload>
    VariantPolymorph::getConstVariantOverloadSubrange() const {
  return this->getVariantOverloadSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::VariantTemplate>
    VariantPolymorph::getVariantTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::VariantTemplate>(
      rq::NextIterator<rq::Template, rq::VariantTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::VariantTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::VariantTemplate>
    VariantPolymorph::getVariantTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::VariantTemplate>(
      rq::ConstNextIterator<rq::Template, rq::VariantTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::VariantTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::VariantTemplate>
    VariantPolymorph::getConstVariantTemplateSubrange() const {
  return this->getVariantTemplateSubrange();
}

RQ_ALWAYS_INLINE ConceptPolymorph::ConceptPolymorph()
    : Polymorph(rq::SymbolKind::CONCEPT_POLYMORPH) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConceptPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONCEPT_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ConceptOverload>
    ConceptPolymorph::getConceptOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ConceptOverload>(
      rq::NextIterator<rq::Implementation, rq::ConceptOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ConceptOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ConceptOverload>
    ConceptPolymorph::getConceptOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ConceptOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ConceptOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ConceptOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ConceptOverload>
    ConceptPolymorph::getConstConceptOverloadSubrange() const {
  return this->getConceptOverloadSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::ConceptTemplate>
    ConceptPolymorph::getConceptTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::ConceptTemplate>(
      rq::NextIterator<rq::Template, rq::ConceptTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::ConceptTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ConceptTemplate>
    ConceptPolymorph::getConceptTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ConceptTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ConceptTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::ConceptTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ConceptTemplate>
    ConceptPolymorph::getConceptConceptTemplateSubrange() const {
  return this->getConceptTemplateSubrange();
}

RQ_ALWAYS_INLINE ClassPolymorph::ClassPolymorph()
    : Polymorph(rq::SymbolKind::CLASS_POLYMORPH) {}

RQ_ALWAYS_INLINE void
ClassPolymorph::addClassOverload(rq::ClassOverload &class_) {
  rq::assignSingleValue(class_._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &class_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::NextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getConstClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::ClassTemplate>
ClassPolymorph::getClassTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::ClassTemplate>(
      rq::NextIterator<rq::Template, rq::ClassTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
    ClassPolymorph::getClassTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
    ClassPolymorph::getConstClassTemplateSubrang() const {
  return rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ClassPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_POLYMORPH);
}

RQ_ALWAYS_INLINE EnumPolymorph::EnumPolymorph()
    : Polymorph(rq::SymbolKind::ENUM_POLYMORPH) {}

RQ_ALWAYS_INLINE void EnumPolymorph::addEnumOverload(rq::EnumOverload &enum_) {
  rq::assignSingleValue(enum_._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &enum_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::NextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getConstEnumOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::EnumTemplate>
EnumPolymorph::getEnumTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::EnumTemplate>(
      rq::NextIterator<rq::Template, rq::EnumTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
    EnumPolymorph::getEnumTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>(
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
    EnumPolymorph::getConstEnumTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>(
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EnumPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_POLYMORPH);
}

RQ_ALWAYS_INLINE InterfacePolymorph::InterfacePolymorph()
    : Polymorph(rq::SymbolKind::INTERFACE_POLYMORPH) {}

RQ_ALWAYS_INLINE void
InterfacePolymorph::addInterfaceOverload(rq::InterfaceOverload &interface) {
  rq::assignSingleValue(interface._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &interface;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::NextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getConstInterfaceOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfacePolymorph::getInterfaceTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfacePolymorph::getInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfacePolymorph::getConstInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InterfacePolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_POLYMORPH);
}

RQ_ALWAYS_INLINE LazyVariablePolymorph::LazyVariablePolymorph()
    : Polymorph(rq::SymbolKind::LAZY_VARIABLE_POLYMORPH) {}

RQ_ALWAYS_INLINE void LazyVariablePolymorph::addLazyVariableOverload(
    rq::LazyVariableOverload &lazyvariable) {
  rq::assignSingleValue(lazyvariable._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &lazyvariable;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getConstLazyVariableOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariablePolymorph::getLazyVariableTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::NextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariablePolymorph::getLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariablePolymorph::getConstLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyVariablePolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAZY_VARIABLE_POLYMORPH);
}

RQ_ALWAYS_INLINE AdapterPolymorph::AdapterPolymorph()
    : Polymorph(rq::SymbolKind::ADAPTER_POLYMORPH) {}

RQ_ALWAYS_INLINE void
AdapterPolymorph::addAdapterOverload(rq::AdapterOverload &adapter) {
  rq::assignSingleValue(adapter._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &adapter;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::NextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getConstAdapterOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterPolymorph::getAdapterTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::NextIterator<rq::Template, rq::AdapterTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterPolymorph::getAdapterTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterPolymorph::getConstAdapterTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_POLYMORPH);
}

RQ_ALWAYS_INLINE ProcPolymorph::ProcPolymorph()
    : Polymorph(rq::SymbolKind::PROC_POLYMORPH) {}

RQ_ALWAYS_INLINE void ProcPolymorph::addProcOverload(rq::ProcOverload &proc) {
  rq::assignSingleValue(proc._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &proc;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ProcOverload>
    ProcPolymorph::getProcOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ProcOverload>(
      rq::NextIterator<rq::Implementation, rq::ProcOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ProcOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>
    ProcPolymorph::getProcOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>
    ProcPolymorph::getConstProcOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::ProcTemplate>
ProcPolymorph::getProcTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::ProcTemplate>(
      rq::NextIterator<rq::Template, rq::ProcTemplate>(
          this->_first_template_ptr),
      rq::NextIterator<rq::Template, rq::ProcTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>
    ProcPolymorph::getProcTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ProcTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::ProcTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>
    ProcPolymorph::getConstProcTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ProcTemplate>(
          this->_first_template_ptr),
      rq::ConstNextIterator<rq::Template, rq::ProcTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROC_POLYMORPH);
}

RQ_ALWAYS_INLINE TableMember::TableMember(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsTableMember(kind), "not table member");
}

RQ_ALWAYS_INLINE void TableMember::setContainer(rq::SymbolTable &container) {
  rq::assignSingleValue(this->_container_ptr, &container);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *TableMember::getContainerPtr() {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
TableMember::getContainerPtr() const {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TableMember::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsTableMember(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Import::Import() : Symbol(rq::SymbolKind::IMPORT) {}

RQ_ALWAYS_INLINE void
Import::setModifierFuseFlags(rq::ModifierFuseFlags flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "already set");
  this->_modifier_fuse_flags = flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
Import::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

RQ_ALWAYS_INLINE void Import::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Import::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *Import::getExpressionPtr() {
  return this->_expression_ptr;
}

RQ_ALWAYS_INLINE void Import::setImported(rq::Module &imported) {
  rq::assignSingleValue(this->_imported_ptr, &imported);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *
Import::getImportedPtr() const {
  return this->_imported_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module *Import::getImportedPtr() {
  return this->_imported_ptr;
}

RQ_ALWAYS_INLINE void Import::setModule(rq::Module &module) {
  rq::assignSingleValue(this->_module_ptr, &module);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *Import::getModulePtr() const {
  return this->_module_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module *Import::getModulePtr() {
  return this->_module_ptr;
}

RQ_ALWAYS_INLINE void Import::addPortal(rq::Portal &portal) {
  rq::assignSingleValue(portal._next_ptr, this->_first_portal_ptr);
  this->_first_portal_ptr = &portal;
}

[[nodiscard]] inline rq::NextSubrange<rq::Portal> Import::getPortalSubrange() {
  return rq::NextSubrange<rq::Portal>(
      rq::NextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::NextIterator<rq::Portal>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Portal>
Import::getPortalSubrange() const {
  return rq::ConstNextSubrange<rq::Portal>(
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::ConstNextIterator<rq::Portal>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Portal>
Import::getConstPortalSubrange() const {
  return rq::ConstNextSubrange<rq::Portal>(
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::ConstNextIterator<rq::Portal>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Import::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::IMPORT);
}

RQ_ALWAYS_INLINE Portal::Portal() : TableMember(rq::SymbolKind::PORTAL) {}

RQ_ALWAYS_INLINE void Portal::setDest(rq::SymbolTable &dest) {
  rq::assignSingleValue(this->_dest_ptr, &dest);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &Portal::getDest() const {
  return rq::dereferencePtr(this->_dest_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &Portal::getDest() {
  return rq::dereferencePtr(this->_dest_ptr);
}

RQ_ALWAYS_INLINE void
Portal::setWithoutList(rq::NodeList<rq::SymbolTable> &&without_list) {
  RQ_ASSERT(this->_without_list.getIsEmpty(), "without list already set");
  this->_without_list = std::move(without_list);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::SymbolTable>
Portal::getWithoutList() const {
  return this->_without_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::SymbolTable>
Portal::getWithoutList() {
  return this->_without_list;
}

RQ_ALWAYS_INLINE void Portal::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Portal::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Portal::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Portal::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PORTAL);
}

RQ_ALWAYS_INLINE EagerDeclaration::EagerDeclaration(rq::SymbolKind kind)
    : TableMember(kind) {}

RQ_ALWAYS_INLINE void EagerDeclaration::setName(rq::Name name) {
  RQ_ASSERT(this->_name.getIsEmpty(), "already set");
  this->_name = name;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name EagerDeclaration::getName() const {
  return this->_name;
}

RQ_ALWAYS_INLINE void EagerDeclaration::setHost(rq::SymbolTable &host) {
  rq::assignSingleValue(this->_host_ptr, &host);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *EagerDeclaration::getHostPtr() {
  return this->_host_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
EagerDeclaration::getHostPtr() const {
  return this->_host_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EagerDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsEagerDeclaration(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Anchor::Anchor() : EagerDeclaration(rq::SymbolKind::ANCHOR) {}

RQ_ALWAYS_INLINE void Anchor::setVessel(rq::EagerScope &scope) {
  rq::assignSingleValue(this->_vessel_ptr, &scope);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScope *Anchor::getVesselPtr() {
  return this->_vessel_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::EagerScope *
Anchor::getVesselPtr() const {
  return this->_vessel_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Anchor::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ANCHOR);
}

RQ_ALWAYS_INLINE Enumerator::Enumerator()
    : EagerDeclaration(rq::SymbolKind::ENUMERATOR) {}

RQ_ALWAYS_INLINE void Enumerator::setWord(rq::ConstantInt &word) {
  rq::assignSingleValue(this->_word_ptr, &word);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantInt *Enumerator::getWordPtr() {
  return this->_word_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantInt *
Enumerator::getWordPtr() const {
  return this->_word_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Enumerator::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUMERATOR);
}

RQ_ALWAYS_INLINE EagerVariable::EagerVariable(rq::SymbolKind kind)
    : EagerDeclaration(kind) {
  RQ_ASSERT(rq::getIsEagerVariable(kind), "not eager variable");
}

RQ_ALWAYS_INLINE void
EagerVariable::setLowFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "already set");
  this->_modifier_fuse_flags = modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
EagerVariable::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

RQ_ALWAYS_INLINE void EagerVariable::setType(rq::QualifiedType &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *EagerVariable::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
EagerVariable::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EagerVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsEagerVariable(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE PostEagerVariable::PostEagerVariable()
    : EagerVariable(rq::SymbolKind::POST_EAGER_VARIABLE) {}

RQ_ALWAYS_INLINE void PostEagerVariable::setLlvmValue(llvm::Value &llvm_value) {
  rq::assignSingleValue(this->_llvm_value_ptr, &llvm_value);
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *
PostEagerVariable::getLlvmValuePtr() {
  return this->_llvm_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *
PostEagerVariable::getLlvmValuePtr() const {
  return this->_llvm_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
PostEagerVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::POST_EAGER_VARIABLE);
}

RQ_ALWAYS_INLINE PreEagerVariable::PreEagerVariable()
    : EagerVariable(rq::SymbolKind::PRE_EAGER_VARIABLE) {}

RQ_ALWAYS_INLINE void PreEagerVariable::setValue(rq::Value &value) {
  rq::assignSingleValue(this->_value_ptr, &value);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Value *PreEagerVariable::getValuePtr() {
  return this->_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Value *
PreEagerVariable::getValuePtr() const {
  return this->_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
PreEagerVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PRE_EAGER_VARIABLE);
}

RQ_ALWAYS_INLINE SymbolTable::SymbolTable(rq::SymbolKind kind)
    : TableMember(kind) {
  RQ_ASSERT(rq::getIsSymbolTable(kind), "not symbol table");
}

RQ_ALWAYS_INLINE void SymbolTable::setContainer(rq::SymbolTable &container) {
  TableMember::setContainer(container);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *SymbolTable::getContainerPtr() {
  return TableMember::getContainerPtr();
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
SymbolTable::getContainerPtr() const {
  return TableMember::getContainerPtr();
}

inline void SymbolTable::addMember(rq::BumpPtrAllocator &allocator,
                                   rq::Name name, rq::TableMember &member) {
  auto [it, _] = this->_member_map.try_emplace(name);
  auto &list = it->getSecond();
  list.insertFront(allocator, member);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstNodeListRef<rq::TableMember>
SymbolTable::lookupList(rq::Name name) const {
  auto found = this->_member_map.find(name);
  if (found == this->_member_map.end()) {
    return {};
  }
  return found->getSecond();
}

RQ_ALWAYS_INLINE void ContainingTableIteratorPolicy::next(
    State *&inout_state_ptr) const {
  State &state = rq::dereferencePtr(inout_state_ptr);
  inout_state_ptr = state.getContainerPtr();
}

[[nodiscard]] RQ_ALWAYS_INLINE ContainingTableIteratorPolicy::State &
ContainingTableIteratorPolicy::value(
    State *const &inout_state_ptr) const {
  return rq::dereferencePtr(inout_state_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ContainingTableIteratorPolicy::getIsDone(
    State *const &inout_state_ptr) const {
  return inout_state_ptr == nullptr;
}

RQ_ALWAYS_INLINE void ConstContainingTableIteratorPolicy::next(
    const State *&inout_state_ptr) const {
  const State &state = rq::dereferencePtr(inout_state_ptr);
  inout_state_ptr = state.getContainerPtr();
}

[[nodiscard]] RQ_ALWAYS_INLINE const ConstContainingTableIteratorPolicy::State &
ConstContainingTableIteratorPolicy::value(
    const State *const &inout_state_ptr) const {
  return rq::dereferencePtr(inout_state_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstContainingTableIteratorPolicy::getIsDone(
    const State *const &inout_state_ptr) const {
  return inout_state_ptr == nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::Subrange<rq::SymbolTable::InclusiveAscendingIterator>
SymbolTable::getInclusiveAscendingSubrange() {
  return rq::Subrange<rq::SymbolTable::InclusiveAscendingIterator>(
      rq::SymbolTable::InclusiveAscendingIterator(this),
      rq::SymbolTable::InclusiveAscendingIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::Subrange<rq::SymbolTable::ConstInclusiveAscendingIterator>
SymbolTable::getInclusiveAscendingSubrange() const {
  return rq::Subrange<rq::SymbolTable::ConstInclusiveAscendingIterator>(
      rq::SymbolTable::ConstInclusiveAscendingIterator(this),
      rq::SymbolTable::ConstInclusiveAscendingIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::Subrange<rq::SymbolTable::ConstInclusiveAscendingIterator>
SymbolTable::getConstInclusiveAscendingSubrange() const {
  return rq::Subrange<rq::SymbolTable::ConstInclusiveAscendingIterator>(
      rq::SymbolTable::ConstInclusiveAscendingIterator(this),
      rq::SymbolTable::ConstInclusiveAscendingIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbolTable(entity.getUnsafeSymbolKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  using MK = rq::ModuleKind;
  switch (kind) {
  case MK::NONE:
    return "none";
  case MK::SOURCE:
    return "source";
  case MK::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE ModuleDetail::ModuleDetail(rq::ModuleKind kind,
                                            llvm::StringRef path,
                                            llvm::StringRef buffer)
    : _module_kind(kind), _path(path), _buffer(buffer) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind
ModuleDetail::getModuleKind() const {
  return this->_module_kind;
}

RQ_ALWAYS_INLINE void
ModuleDetail::setOrChangeExpression(rq::Expression *expression_ptr) {
  this->_expression_ptr = expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
ModuleDetail::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
ModuleDetail::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleDetail::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleDetail::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &
ModuleDetail::getTokens() {
  return this->_tokens;
}

[[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
ModuleDetail::getTokens() const {
  return this->_tokens;
}

RQ_ALWAYS_INLINE Module::Module(rq::ModuleDetail &&detail)
    : SymbolTable(rq::SymbolKind::MODULE), _module_kind(detail.getModuleKind()),
      _expression_ptr(detail.getExpressionPtr()), _path(detail.getPath()),
      _buffer(detail.getBuffer()) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind Module::getModuleKind() const {
  return this->_module_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Module::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Module::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

RQ_ALWAYS_INLINE void Module::addImport(rq::Import &import) {
  rq::assignSingleValue(import._next_ptr, this->_first_import_ptr);
  this->_first_import_ptr = &import;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Import>
Module::getImportSubrange() {
  return rq::NextSubrange<rq::Import>(
      rq::NextIterator<rq::Import>(this->_first_import_ptr),
      rq::NextIterator<rq::Import>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
Module::getImportSubrange() const {
  return rq::ConstNextSubrange<rq::Import>(
      rq::ConstNextIterator<rq::Import>(this->_first_import_ptr),
      rq::ConstNextIterator<rq::Import>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
Module::getConstImportSubrange() const {
  return rq::ConstNextSubrange<rq::Import>(
      rq::ConstNextIterator<rq::Import>(this->_first_import_ptr),
      rq::ConstNextIterator<rq::Import>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Module::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::MODULE);
}

RQ_ALWAYS_INLINE CTable::CTable() : SymbolTable(rq::SymbolKind::C_TABLE) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
CTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::C_TABLE);
}

RQ_ALWAYS_INLINE EagerScope::EagerScope(rq::SymbolKind kind)
    : SymbolTable(kind) {
  RQ_ASSERT(rq::getIsEagerScope(kind), "not eager scope");
}

RQ_ALWAYS_INLINE void EagerScope::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *EagerScope::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
EagerScope::getExpressionPtr() const {
  return this->_expression_ptr;
}

RQ_ALWAYS_INLINE void
EagerScope::setModifierFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "flags already set");
  this->_modifier_fuse_flags = modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
EagerScope::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EagerScope::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsEagerScope(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ScopeStatement::ScopeStatement()
    : EagerScope(rq::SymbolKind::SCOPE_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ScopeStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SCOPE_STATEMENT);
}

RQ_ALWAYS_INLINE WhileStatement::WhileStatement()
    : EagerScope(rq::SymbolKind::WHILE_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
WhileStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::WHILE_STATEMENT);
}

RQ_ALWAYS_INLINE ForStatement::ForStatement()
    : EagerScope(rq::SymbolKind::FOR_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ForStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::FOR_STATEMENT);
}

RQ_ALWAYS_INLINE DefaultStatement::DefaultStatement()
    : EagerScope(rq::SymbolKind::DEFAULT_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DefaultStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::DEFAULT_STATEMENT);
}

RQ_ALWAYS_INLINE CaseStatement::CaseStatement()
    : EagerScope(rq::SymbolKind::CASE_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
CaseStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CASE_STATEMENT);
}

RQ_ALWAYS_INLINE SwitchStatement::SwitchStatement()
    : EagerScope(rq::SymbolKind::SWITCH_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SwitchStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SWITCH_STATEMENT);
}

RQ_ALWAYS_INLINE IfStatement::IfStatement()
    : EagerScope(rq::SymbolKind::IF_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
IfStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::IF_STATEMENT);
}

RQ_ALWAYS_INLINE ElseIfStatement::ElseIfStatement()
    : EagerScope(rq::SymbolKind::ELSE_IF_STATEMENT) {}

[[nodiscard]] bool ElseIfStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ELSE_IF_STATEMENT);
}

RQ_ALWAYS_INLINE ElseStatement::ElseStatement()
    : EagerScope(rq::SymbolKind::ELSE_STATEMENT) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ElseStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ELSE_STATEMENT);
}

RQ_ALWAYS_INLINE NamedTable::NamedTable(rq::SymbolKind kind)
    : SymbolTable(kind) {
  RQ_ASSERT(rq::getIsNamedTable(kind), "not named table");
}

RQ_ALWAYS_INLINE void NamedTable::setName(rq::Name name) {
  RQ_ASSERT(this->_name.getIsEmpty(), "name already set");
  this->_name = name;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name NamedTable::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
NamedTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsNamedTable(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Namespace::Namespace()
    : NamedTable(rq::SymbolKind::NAMESPACE) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Namespace::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::NAMESPACE);
}

RQ_ALWAYS_INLINE LazyDeclaration::LazyDeclaration(rq::SymbolKind kind)
    : NamedTable(kind) {
  RQ_ASSERT(rq::getIsLazyDeclaration(kind), "not lazy declaration");
}

RQ_ALWAYS_INLINE void
LazyDeclaration::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
LazyDeclaration::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
LazyDeclaration::getExpressionPtr() const {
  return this->_expression_ptr;
}

RQ_ALWAYS_INLINE void LazyDeclaration::setModifierFuseFlags(
    rq::ModifierFuseFlags modifier_fuse_flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "flags already set");
  this->_modifier_fuse_flags = modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
LazyDeclaration::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

RQ_ALWAYS_INLINE void LazyDeclaration::setHostTable(rq::SymbolTable &host) {
  rq::assignSingleValue(this->_host_ptr, &host);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *
LazyDeclaration::getHostTablePtr() {
  return this->_host_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
LazyDeclaration::getHostTablePtr() const {
  return this->_host_ptr;
}

inline void LazyDeclaration::setMangledName(llvm::StringRef mangled_name) {
  RQ_ASSERT(this->_mangled_name.empty(), "mangled name already set");
  RQ_ASSERT(!mangled_name.empty(), "empty mangled name");
  this->_mangled_name = mangled_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
LazyDeclaration::getMangledName() const {
  return this->_mangled_name;
}

RQ_ALWAYS_INLINE void LazyDeclaration::setModule(rq::Module &module) {
  rq::assignSingleValue(this->_module_ptr, &module);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *
LazyDeclaration::getModulePtr() const {
  return this->_module_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module *LazyDeclaration::getModulePtr() {
  return this->_module_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLazyDeclaration(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Implementation::Implementation(rq::SymbolKind kind)
    : LazyDeclaration(kind) {
  RQ_ASSERT(Implementation::classof(this), "not implementation");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *Implementation::getParentPtr() {
  return this->_parent_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *
Implementation::getParentPtr() const {
  return this->_parent_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Implementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE EnumImplementation::EnumImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsEnumImplementation(kind), "not enum implementation");
}

RQ_ALWAYS_INLINE void
EnumImplementation::setUnderlyingType(rq::QualifiedType *underlying_type) {
  rq::assignSingleValue(this->_underlying_type_ptr, underlying_type);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *
EnumImplementation::getUnderlyingTypePtr() {
  return this->_underlying_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
EnumImplementation::getUnderlyingTypePtr() const {
  return this->_underlying_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EnumImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsEnumImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE EnumOverload::EnumOverload()
    : EnumImplementation(rq::SymbolKind::ENUM_OVERLOAD) {}

RQ_ALWAYS_INLINE void
EnumOverload::setParentEnumPolymorph(rq::EnumPolymorph &parent) {
  rq::assignSingleValue(this->_parent_ptr, &parent);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *
EnumOverload::getParentEnumPolymorphPtr() {
  return llvm::cast<rq::EnumPolymorph>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
EnumOverload::getParentEnumPolymorphPtr() const {
  return llvm::cast<rq::EnumPolymorph>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
EnumOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
EnumOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EnumOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_OVERLOAD);
}

RQ_ALWAYS_INLINE EnumSpecialization::EnumSpecialization()
    : EnumImplementation(rq::SymbolKind::ENUM_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void
EnumSpecialization::setParentEnumTemplate(rq::EnumTemplate &template_) {
  rq::assignSingleValue(this->_parent_ptr, &template_);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *
EnumSpecialization::getParentEnumTemplatePtr() {
  return llvm::cast<rq::EnumTemplate>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
EnumSpecialization::getParentEnumTemplatePtr() const {
  return llvm::cast<rq::EnumTemplate>(this->_parent_ptr);
}

RQ_ALWAYS_INLINE void EnumSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
EnumSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
EnumSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EnumSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
LazyVariableImplementation::LazyVariableImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsLazyVariableImplementation(kind),
            "not lazy variable implementation");
}

RQ_ALWAYS_INLINE void
LazyVariableImplementation::setType(rq::QualifiedType &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *
LazyVariableImplementation::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
LazyVariableImplementation::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyVariableImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLazyVariableImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE LazyVariableOverload::LazyVariableOverload()
    : LazyVariableImplementation(rq::SymbolKind::LAZY_VARIABLE_OVERLOAD) {}

RQ_ALWAYS_INLINE void LazyVariableOverload::setParentLazyVariablePolymorph(
    rq::LazyVariablePolymorph &parent) {
  rq::assignSingleValue(this->_parent_ptr, &parent);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariablePolymorph *
LazyVariableOverload::getParentPtr() {
  return llvm::cast<rq::LazyVariablePolymorph>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariablePolymorph *
LazyVariableOverload::getParentPtr() const {
  return llvm::cast<rq::LazyVariablePolymorph>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
LazyVariableOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
LazyVariableOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyVariableOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAZY_VARIABLE_OVERLOAD);
}

RQ_ALWAYS_INLINE LazyVariableSpecialization::LazyVariableSpecialization()
    : LazyVariableImplementation(rq::SymbolKind::LAZY_VARIABLE_SPECIALIZATION) {
}

RQ_ALWAYS_INLINE void LazyVariableSpecialization::setParentLazyVariableTemplate(
    rq::LazyVariableTemplate &parent) {
  rq::assignSingleValue(this->_parent_ptr, &parent);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableTemplate *
LazyVariableSpecialization::getParentLazyVariableTemplatePtr() {
  return llvm::cast<rq::LazyVariableTemplate>(this->_parent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariableTemplate *
LazyVariableSpecialization::getParentLazyVariableTemplatePtr() const {
  return llvm::cast<rq::LazyVariableTemplate>(this->_parent_ptr);
}

RQ_ALWAYS_INLINE void LazyVariableSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
LazyVariableSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
LazyVariableSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyVariableSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::LAZY_VARIABLE_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
AdapterImplementation::AdapterImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsAdapterImplementation(kind), "not adapter implementation");
}

RQ_ALWAYS_INLINE void
AdapterImplementation::setRealization(rq::Realization &realization) {
  rq::assignSingleValue(this->_realization_ptr, &realization);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Realization *
AdapterImplementation::getRealizationPtr() {
  return this->_realization_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Realization *
AdapterImplementation::getRealizationPtr() const {
  return this->_realization_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsAdapterImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE AdapterOverload::AdapterOverload()
    : AdapterImplementation(rq::SymbolKind::ADAPTER_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
AdapterOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
AdapterOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_OVERLOAD);
}

RQ_ALWAYS_INLINE AdapterSpecialization::AdapterSpecialization()
    : AdapterImplementation(rq::SymbolKind::ADAPTER_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void AdapterSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
AdapterSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
AdapterSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
InterfaceImplementation::InterfaceImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsInterfaceImplementation(kind),
            "not interface implementation");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InterfaceImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsInterfaceImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE InterfaceOverload::InterfaceOverload()
    : InterfaceImplementation(rq::SymbolKind::INTERFACE_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
InterfaceOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
InterfaceOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InterfaceOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_OVERLOAD);
}

RQ_ALWAYS_INLINE InterfaceSpecialization::InterfaceSpecialization()
    : InterfaceImplementation(rq::SymbolKind::INTERFACE_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void InterfaceSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
InterfaceSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
InterfaceSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InterfaceSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_SPECIALIZATION);
}

RQ_ALWAYS_INLINE ClassImplementation::ClassImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsClassImplementation(kind), "not class implementation");
}

RQ_ALWAYS_INLINE void
ClassImplementation::setBestLayout(rq::QualifiedType &best) {
  RQ_ASSERT(best.getType().getSymbolKind() == rq::SymbolKind::UNKNOWN_TYPE ||
                best.getType().getSymbolKind() == rq::SymbolKind::LAYOUT_TYPE,
            "invalid symbol for layout");
  rq::assignSingleValue(this->_best_layout_ptr, &best);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *
ClassImplementation::getBestLayoutPtr() {
  return this->_best_layout_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
ClassImplementation::getBestLayoutPtr() const {
  return this->_best_layout_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ClassImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsClassImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ClassOverload::ClassOverload()
    : ClassImplementation(rq::SymbolKind::CLASS_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
ClassOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
ClassOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

RQ_ALWAYS_INLINE void ClassOverload::changeBestLayout(rq::QualifiedType &best) {
  RQ_ASSERT(best.getType().getSymbolKind() == rq::SymbolKind::UNKNOWN_TYPE ||
                best.getType().getSymbolKind() == rq::SymbolKind::LAYOUT_TYPE,
            "invalid symbol for layout");
  rq::QualifiedType &prev_best = rq::dereferencePtr(this->_best_layout_ptr);
  RQ_ASSERT(prev_best.getType().getSymbolKind() != rq::SymbolKind::LAYOUT_TYPE,
            "prev best is already a layout type");
  RQ_ASSERT(best.getType().getSymbolKind() == rq::SymbolKind::LAYOUT_TYPE ||
                (best.getMargin() <= prev_best.getMargin()),
            "updating best layout with larger margin");
  rq::replaceValue(this->_best_layout_ptr, &best);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ClassOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_OVERLOAD);
}

RQ_ALWAYS_INLINE ClassSpecialization::ClassSpecialization()
    : ClassImplementation(rq::SymbolKind::CLASS_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void ClassSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
ClassSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
ClassSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ClassSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
ProcImplementation::ProcImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsProcImplementation(kind), "not proc implementation");
}

RQ_ALWAYS_INLINE void
ProcImplementation::setSignatureType(rq::SignatureType &signature) {
  rq::assignSingleValue(this->_signature_type_ptr, &signature);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureType *
ProcImplementation::getSignatureTypePtr() {
  return this->_signature_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureType *
ProcImplementation::getSignatureTypePtr() const {
  return this->_signature_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsProcImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ProcOverload::ProcOverload()
    : ProcImplementation(rq::SymbolKind::PROC_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
ProcOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
ProcOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROC_OVERLOAD);
}

RQ_ALWAYS_INLINE ProcSpecialization::ProcSpecialization()
    : ProcImplementation(rq::SymbolKind::PROC_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void ProcSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
ProcSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
ProcSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROC_SPECIALIZATION);
}

RQ_ALWAYS_INLINE AliasImplementation::AliasImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsAliasImplementation(kind), "not alias implementation");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AliasImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsAliasImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE AliasOverload::AliasOverload()
    : AliasImplementation(rq::SymbolKind::ALIAS_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
AliasOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
AliasOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AliasOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ALIAS_OVERLOAD);
}

RQ_ALWAYS_INLINE AliasSpecialization::AliasSpecialization()
    : AliasImplementation(rq::SymbolKind::ALIAS_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void AliasSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
AliasSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
AliasSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AliasSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ALIAS_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
ConceptImplementation::ConceptImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(rq::getIsConceptImplementation(kind), "not concept implementation");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConceptImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsConceptImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ConceptOverload::ConceptOverload()
    : ConceptImplementation(rq::SymbolKind::CONCEPT_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
ConceptOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
ConceptOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConceptOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONCEPT_OVERLOAD);
}

RQ_ALWAYS_INLINE ConceptSpecialization::ConceptSpecialization()
    : ConceptImplementation(rq::SymbolKind::CONCEPT_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void ConceptSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
ConceptSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
ConceptSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConceptSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONCEPT_SPECIALIZATION);
}

RQ_ALWAYS_INLINE
VariantImplementation::VariantImplementation(rq::SymbolKind kind)
    : Implementation(kind) {
  RQ_ASSERT(VariantImplementation::classof(this), "not variant implementation");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
VariantImplementation::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsVariantImplementation(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE VariantOverload::VariantOverload()
    : VariantImplementation(rq::SymbolKind::VARIANT_OVERLOAD) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
VariantOverload::getPrototypeExpressionPtrList() {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
VariantOverload::getPrototypeExpressionPtrList() const {
  return this->_prototype_expression_ptr_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
VariantOverload::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::VARIANT_OVERLOAD);
}

RQ_ALWAYS_INLINE VariantSpecialization::VariantSpecialization()
    : VariantImplementation(rq::SymbolKind::VARIANT_SPECIALIZATION) {}

RQ_ALWAYS_INLINE void VariantSpecialization::setTemplateTuple(
    rq::ConstantDetachedArray &template_tuple) {
  rq::assignSingleValue(this->_template_tuple_ptr, &template_tuple);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
VariantSpecialization::getTemplateTuplePtr() {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
VariantSpecialization::getTemplateTuplePtr() const {
  return this->_template_tuple_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
VariantSpecialization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::VARIANT_SPECIALIZATION);
}

RQ_ALWAYS_INLINE Template::Template(rq::SymbolKind kind)
    : LazyDeclaration(kind) {
  RQ_ASSERT(rq::getIsTemplate(kind), "not template");
}

RQ_ALWAYS_INLINE void Template::setLayout(rq::LayoutType &layout) {
  rq::assignSingleValue(this->_layout_ptr, &layout);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutType *Template::getLayoutPtr() {
  return this->_layout_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutType *
Template::getLayoutPtr() const {
  return this->_layout_ptr;
}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation>
Template::getSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation>(
      rq::NextIterator<rq::Implementation>(this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation>
Template::getSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation>
Template::getConstSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Template::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsTemplate(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE EnumTemplate::EnumTemplate()
    : Template(rq::SymbolKind::ENUM_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::EnumSpecialization>
EnumTemplate::getEnumSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::EnumSpecialization>(
      rq::NextIterator<rq::Implementation, rq::EnumSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::EnumSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::EnumSpecialization>
EnumTemplate::getEnumSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::EnumSpecialization>
EnumTemplate::getConstEnumSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
EnumTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_TEMPLATE);
}

RQ_ALWAYS_INLINE ClassTemplate::ClassTemplate()
    : Template(rq::SymbolKind::CLASS_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::ClassSpecialization>
ClassTemplate::getClassSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ClassSpecialization>(
      rq::NextIterator<rq::Implementation, rq::ClassSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::ClassSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ClassSpecialization>
ClassTemplate::getClassSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ClassSpecialization>
ClassTemplate::getConstClassSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ClassTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_TEMPLATE);
}

RQ_ALWAYS_INLINE InterfaceTemplate::InterfaceTemplate()
    : Template(rq::SymbolKind::INTERFACE_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::InterfaceSpecialization>
InterfaceTemplate::getInterfaceSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::InterfaceSpecialization>(
      rq::NextIterator<rq::Implementation, rq::InterfaceSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::InterfaceSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::InterfaceSpecialization>
InterfaceTemplate::getInterfaceSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::InterfaceSpecialization>
InterfaceTemplate::getConstInterfaceSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InterfaceTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_TEMPLATE);
}

RQ_ALWAYS_INLINE LazyVariableTemplate::LazyVariableTemplate()
    : Template(rq::SymbolKind::LAZY_VARIABLE_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::LazyVariableSpecialization>
LazyVariableTemplate::getLazyVariableTemplateSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::LazyVariableSpecialization>(
      rq::NextIterator<rq::Implementation, rq::LazyVariableSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::LazyVariableSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::LazyVariableSpecialization>
LazyVariableTemplate::getLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation,
                               rq::LazyVariableSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation,
                            rq::LazyVariableSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::LazyVariableSpecialization>
LazyVariableTemplate::getConstLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation,
                               rq::LazyVariableSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation,
                            rq::LazyVariableSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
LazyVariableTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAZY_VARIABLE_TEMPLATE);
}

RQ_ALWAYS_INLINE AdapterTemplate::AdapterTemplate()
    : Template(rq::SymbolKind::ADAPTER_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::AdapterSpecialization>
AdapterTemplate::getAdapterSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::AdapterSpecialization>(
      rq::NextIterator<rq::Implementation, rq::AdapterSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::AdapterSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::AdapterSpecialization>
AdapterTemplate::getAdapterSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::AdapterSpecialization>
AdapterTemplate::getConstAdapterSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AdapterTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_TEMPLATE);
}

RQ_ALWAYS_INLINE ProcTemplate::ProcTemplate()
    : Template(rq::SymbolKind::PROC_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::ProcSpecialization>
ProcTemplate::getProcSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ProcSpecialization>(
      rq::NextIterator<rq::Implementation, rq::ProcSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::ProcSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ProcSpecialization>
ProcTemplate::getProcSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ProcSpecialization>
ProcTemplate::getConstProcSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcSpecialization>());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ProcTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROC_TEMPLATE);
}

RQ_ALWAYS_INLINE AliasTemplate::AliasTemplate()
    : Template(rq::SymbolKind::ALIAS_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::AliasSpecialization>
AliasTemplate::getAliasSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::AliasSpecialization>(
      rq::NextIterator<rq::Implementation, rq::AliasSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::AliasSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::AliasSpecialization>
AliasTemplate::getAliasSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AliasSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::AliasSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AliasSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::AliasSpecialization>
AliasTemplate::getConstAliasSpecializationSubrange() const {
  return this->getAliasSpecializationSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
AliasTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ALIAS_TEMPLATE);
}

RQ_ALWAYS_INLINE ConceptTemplate::ConceptTemplate()
    : Template(rq::SymbolKind::CONCEPT_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::ConceptSpecialization>
ConceptTemplate::getConceptSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ConceptSpecialization>(
      rq::NextIterator<rq::Implementation, rq::ConceptSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::ConceptSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ConceptSpecialization>
ConceptTemplate::getConceptSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ConceptSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::ConceptSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ConceptSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::ConceptSpecialization>
ConceptTemplate::getConstConceptSpecializationSubrange() const {
  return this->getConceptSpecializationSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConceptTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONCEPT_TEMPLATE);
}

RQ_ALWAYS_INLINE VariantTemplate::VariantTemplate()
    : Template(rq::SymbolKind::VARIANT_TEMPLATE) {}

[[nodiscard]] inline rq::NextSubrange<rq::Implementation,
                                      rq::VariantSpecialization>
VariantTemplate::getVariantSpecializationSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::VariantSpecialization>(
      rq::NextIterator<rq::Implementation, rq::VariantSpecialization>(
          this->_first_specialization_ptr),
      rq::NextIterator<rq::Implementation, rq::VariantSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::VariantSpecialization>
VariantTemplate::getVariantSpecializationSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::VariantSpecialization>(
      rq::ConstNextIterator<rq::Implementation, rq::VariantSpecialization>(
          this->_first_specialization_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::VariantSpecialization>());
}

[[nodiscard]] inline rq::ConstNextSubrange<rq::Implementation,
                                           rq::VariantSpecialization>
VariantTemplate::getConstVariantSpecializationSubrange() const {
  return this->getVariantSpecializationSubrange();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
VariantTemplate::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::VARIANT_TEMPLATE);
}

} // namespace rq
