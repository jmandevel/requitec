#pragma once

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    break;

  // LITERALS
  case S::INTEGER_LITERAL_TYPE:
    return "IntegerLiteralType";
  case S::FLOAT_LITERAL_TYPE:
    return "FloatLiteralType";
  case S::STRING_LITERAL_TYPE:
    return "StringLiteralType";
  case S::CODEUNIT_LITERAL_TYPE:
    return "CodeunitLiteralType";

  // CONTEXTUAL VALUE
  case S::UNKNOWN_VALUE:
    return "UnknownValue";
  case S::VALUE_VALUE:
    return "ValueValue";
  case S::INDEX_VALUE:
    return "IndexValue";

  // CONTEXTUAL TYPE
  case S::UNKNOWN_TYPE:
    return "UnknownType";
  case S::INFERENCE_TYPE:
    return "InferenceType";
  case S::VOID_TYPE:
    return "VoidType";
  case S::NO_RETURN_TYPE:
    return "NoReturnType";

  // MODIFIER TYPES
  case S::ANCHOR_MODIFIER_TYPE:
    return "AnchorModifierType";
  case S::CONTAINER_MODIFIER_TYPE:
    return "ContainerModifierType";
  case S::VISIBILITY_MODIFIER_TYPE:
    return "VisibilityModifierType";
  case S::ACCESS_MODIFIER_TYPE:
    return "AccessModifierType";
  case S::MUTATE_MODIFIER_TYPE:
    return "MutateModifierType";
  case S::COHORT_MODIFIER_TYPE:
    return "CohortModifierType";
  case S::CAPTURE_MODIFIER_TYPE:
    return "CaptureModifierType";
  case S::LINKAGE_MODIFIER_TYPE:
    return "LinkageModifierType";
  case S::MANGLE_MODIFIER_TYPE:
    return "MangleModifierType";
  case S::PACK_MODIFIER_TYPE:
    return "PackModifierType";
  case S::BRANCH_TREND_MODIFIER_TYPE:
    return "BranchTrendModifierType";
  case S::SUPPORT_NOTICE_MODIFIER_TYPE:
    return "SupportNoticeModifierType";
  case S::STABLE_ADDRESS_MODIFIER_TYPE:
    return "StableAddressModifierType";
  case S::VARIADIC_MODIFIER_TYPE:
    return "VariadicModifierType";
  case S::OFFSET_MODIFIER_TYPE:
    return "OffsetModifierType";
  case S::TEMPLATE_MODIFIER_TYPE:
    return "TemplateModifierType";
  case S::CONSTRAINT_MODIFIER_TYPE:
    return "ConstraintModifierType";
  case S::WEIGHT_MODIFIER_TYPE:
    return "WeightModifierType";
  case S::DEDUCTION_MODIFIER_TYPE:
    return "DeductionModifierType";
  case S::VIRTUALITY_MODIFIER_TYPE:
    return "VirtualityModifierType";
  case S::ENSURE_MODIFIER_TYPE:
    return "EnsureModifierType";
  case S::REQUIRE_MODIFIER_TYPE:
    return "RequireModifierType";
  case S::RANGER_MODIFIER_TYPE:
    return "RangerModifierType";

  // QUALIFIER TYPES
  case S::VAR_QUALIFIER_TYPE:
    return "VarQualifierType";
  case S::VOLATILE_QUALIFIER_TYPE:
    return "VolatileQualifierType";
  case S::ATOMIC_QUALIFIER_TYPE:
    return "AtomicQualifierType";
  case S::NULL_TERMINATE_QUALIFIER_TYPE:
    return "NullTerminateQualifierType";

  // REFLECTIVE TYPES
  case S::SYMBOL_TYPE:
    return "SymbolType";
  case S::SYMBOL_RANGE_TYPE:
    return "SymbolRangeType";
  case S::EXPRESSION_TYPE:
    return "ExpressionType";
  case S::EXPRESSION_RANGE_TYPE:
    return "ExpressionRangeType";

  // PLATFORM PRIMITIVE TYPES
  case S::BOOLEAN_TYPE:
    return "BooleanType";
  case S::HALF_TYPE:
    return "HalfType";
  case S::SINGLE_TYPE:
    return "SingleType";
  case S::DOUBLE_TYPE:
    return "DoubleType";
  case S::QUADRUPLE_TYPE:
    return "QuadrupleType";
  case S::SIGNED_INTEGER_TYPE:
    return "SignedIntegerType";
  case S::UNSIGNED_INTEGER_TYPE:
    return "UnsignedIntegerType";
  case S::SIGNED_INDEX_TYPE:
    return "SignedIndexType";
  case S::UNSIGNED_INDEX_TYPE:
    return "UnsignedIndexType";
  case S::SIGNED_ADDRESS_TYPE:
    return "SignedAddressType";
  case S::UNSIGNED_ADDRESS_TYPE:
    return "UnsignedAddressType";
  case S::CHAR_TYPE:
    return "CharType";

  // FITTING PRIMITIVE TYPES
  case S::FAST_SIGNED_INTEGER_TYPE:
    return "FastSignedIntegerType";
  case S::FAST_UNSIGNED_INTEGER_TYPE:
    return "FastUnsignedIntegerType";
  case S::LEAST_SIGNED_INTEGER_TYPE:
    return "LeastSignedIntegerType";
  case S::LEAST_UNSIGNED_INTEGER_TYPE:
    return "LeastUnsignedIntegerType";

  // STANDARD PRIMITIVE TYPE
  case S::BINARY16_TYPE:
    return "Binary16Type";
  case S::BINARY32_TYPE:
    return "Binary32Type";
  case S::BINARY64_TYPE:
    return "Binary64Type";
  case S::BINARY128_TYPE:
    return "Binary128Type";
  case S::BFLOAT16_TYPE:
    return "BFloat16Type";
  case S::ASCII_TYPE:
    return "AsciiType";
  case S::UTF8_TYPE:
    return "Utf8Type";

  // VARIADIC ARGUMENTS
  case S::VARIADIC_ARGUMENTS_TYPE:
    return "VariadicArgumentsType";
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return "ScaledSignedIntegerType";
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return "ScaledUnsignedIntegerType";

  // SUBTYPES
  case S::REFERENCE_SUBTYPE:
    return "ReferenceSubtype";
  case S::POINTER_SUBTYPE:
    return "PointerSubtype";
  case S::SLICE_SUBTYPE:
    return "SliceSubtype";
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return "InferenceCountArraySubtype";
  case S::ARRAY_SUBTYPE:
    return "ArraySubtype";

  // MODULES
  case S::MODULE:
    return "Module";

  // IMPORTS
  case S::IMPORT:
    return "Import";

  // CONFORMITY
  case S::CONFORMITY:
    return "Conformity";

  // ADAPTION
  case S::ADAPTAION:
    return "Adaptaion";

  // JUXT LIST
  case S::JUXT_LIST_TYPE:
    return "JuxtListType";
  case S::JUXT_LIST_ITEM:
    return "JuxtListItem";

  // ARITHMETIC SEQUENCES
  case S::ARITHMETIC_INTERVAL_TYPE:
    return "ArithmeticIntervalType";
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "InfiniteArithmeticSequenceType";
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "FiniteArithmeticSequenceType";

  // LOCAL DECLARATIONS
  case S::ANCHOR:
    return "Anchor";
  case S::ENUMERATOR:
    return "Enumerator";
  case S::LOCAL_VARIABLE:
    return "LocalVariable";

  // PARAMETERS
  case S::PARAMETER:
    return "Parameter";

  // PARAMETER LISTS
  case S::SIGNATURE_TYPE:
    return "SignatureType";
  case S::LAYOUT_TYPE:
    return "LayoutType";

  // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return "PlacementType";

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return "CompositionComponent";
  case S::COMPOSITION_TYPE:
    return "CompositionType";

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return "SynonymType";

  // SYMBOL TABLES
  case S::C_TABLE:
    return "CTable";
  case S::TOP_TABLE:
    return "TopTable";

  // LOCAL STATEMENTS
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
  case S::SPIN_STATEMENT:
    return "SpinStatement";
  case S::WEAVE_STATEMENT:
    return "WeaveStatement";
  case S::SCOPE_STATEMENT:
    return "ScopeStatement";

  // NAMED TABLE
  case S::NAMESPACE:
    return "Namespace";

  // OVERLOADS
  case S::CLASS_OVERLOAD:
    return "ClassOverload";
  case S::ENUM_OVERLOAD:
    return "EnumOverload";
  case S::INTERFACE_OVERLOAD:
    return "InterfaceOverload";
  case S::ADAPTER_OVERLOAD:
    return "AdapterOverload";
  case S::CONSTRUCTOR_OVERLOAD:
    return "ConstructorOverload";
  case S::LAYOUT_CONSTRUCTOR_OVERLOAD:
    return "LayoutConstructorOverload";
  case S::FUNCTION_OVERLOAD:
    return "FunctionOverload";
  case S::GLOBAL_VARIABLE_OVERLOAD:
    return "GlobalVariableOverload";

  // SPECIALIZATIONS
  case S::CLASS_SPECIALIZATION:
    return "ClassSpecialization";
  case S::ENUM_SPECIALIZATION:
    return "EnumSpecialization";
  case S::INTERFACE_SPECIALIZATION:
    return "InterfaceSpecialization";
  case S::ADAPTER_SPECIALIZATION:
    return "AdapterSpecialization";
  case S::FUNCTION_SPECIALIZATION:
    return "FunctionSpecialization";
  case S::GLOBAL_VARIABLE_SPECIALIZATION:
    return "GlobalVariableSpecialization";

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return "ClassTemplate";
  case S::ENUM_TEMPLATE:
    return "EnumTemplate";
  case S::INTERFACE_TEMPLATE:
    return "InterfaceTemplate";
  case S::ADAPTER_TEMPLATE:
    return "AdapterTemplate";
  case S::FUNCTION_TEMPLATE:
    return "FunctionTemplate";
  case S::GLOBAL_VARIABLE_TEMPLATE:
    return "GlobalVariableTemplate";

  // POLYMORPHS
  case S::CLASS_POLYMORPH:
    return "ClassPolymorph";
  case S::ENUM_POLYMORPH:
    return "EnumPolymorph";
  case S::INTERFACE_POLYMORPH:
    return "InterfacePolymorph";
  case S::ADAPTER_POLYMORPH:
    return "AdapterPolymorph";
  case S::FUNCTION_POLYMORPH:
    return "FunctionPolymorph";
  case S::GLOBAL_VARIABLE_POLYMORPH:
    return "GlobalVariablePolymorph";

  // WEIGHT LEVELS
  case S::CLASS_WEIGHT_LEVEL:
    return "ClassWeightLevel";
  case S::ENUM_WEIGHT_LEVEL:
    return "EnumWeightLevel";
  case S::INTERFACE_WEIGHT_LEVEL:
    return "InterfaceWeightLevel";
  case S::ADAPTER_WEIGHT_LEVEL:
    return "AdapterWeightLevel";
  case S::FUNCTION_WEIGHT_LEVEL:
    return "FunctionWeightLevel";
  case S::GLOBAL_VARIABLE_WEIGHT_LEVEL:
    return "GlobalVariableWeightLevel";

  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}
} // namespace rq