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

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolInfoFlags
getInfoFlags(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  using SIF = rq::SymbolInfoFlags;
  switch (kind) {
  case S::NONE:
    break;
  case S::INTEGER_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::FLOAT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::STRING_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::CODEUNIT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::UNKNOWN_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::VALUE_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::INDEX_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::UNKNOWN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::INFERENCE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::VOID_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::NO_RETURN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::ANCHOR_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::CONTAINER_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::VISIBILITY_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::ACCESS_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::MUTATE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::COHORT_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::CAPTURE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::LINKAGE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::MANGLE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::PACK_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::BRANCH_TREND_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::SUPPORT_NOTICE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::STABLE_ADDRESS_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::VARIADIC_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::OFFSET_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::TEMPLATE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::CONSTRAINT_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::WEIGHT_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::DEDUCTION_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::VIRTUALITY_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::ENSURE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::REQUIRE_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::RANGER_MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::MODIFIER_TYPE | SIF::IS_TYPE;
  case S::VAR_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::VOLATILE_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::ATOMIC_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::NULL_TERMINATE_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::SYMBOL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTIVE_TYPE | SIF::IS_TYPE;
  case S::SYMBOL_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTIVE_TYPE | SIF::IS_TYPE;
  case S::EXPRESSION_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTIVE_TYPE | SIF::IS_TYPE;
  case S::EXPRESSION_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTIVE_TYPE | SIF::IS_TYPE;
  case S::BOOLEAN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE;
  case S::HALF_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SINGLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::DOUBLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::QUADRUPLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_INDEX_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_ADDRESS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INDEX_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_ADDRESS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::CHAR_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_CODEUNIT_TYPE;
  case S::FAST_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::FAST_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::LEAST_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::LEAST_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::BINARY16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY32_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY64_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY128_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BFLOAT16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BFLOAT_TYPE;
  case S::ASCII_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_CODEUNIT_TYPE;
  case S::UTF8_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_CODEUNIT_TYPE;
  case S::VARIADIC_ARGUMENTS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::REFERENCE_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::POINTER_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::SLICE_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::MODULE:
    return SIF::NONE;
  case S::IMPORT:
    return SIF::NONE;
  case S::CONFORMITY:
    return SIF::NONE;
  case S::ADAPTAION:
    return SIF::NONE;
  case S::JUXT_LIST_TYPE:
    return SIF::IS_TYPE;
  case S::JUXT_LIST_ITEM:
    return SIF::NONE;
  case S::ARITHMETIC_INTERVAL_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::ANCHOR:
    return SIF::TABLE_MEMBER | SIF::LOCAL_DECLARATION;
  case S::ENUMERATOR:
    return SIF::TABLE_MEMBER | SIF::LOCAL_DECLARATION;
  case S::LOCAL_VARIABLE:
    return SIF::TABLE_MEMBER | SIF::LOCAL_DECLARATION;
  case S::PARAMETER:
    return SIF::PARAMETER_LIST;
  case S::SIGNATURE_TYPE:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;
  case S::LAYOUT_TYPE:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;
  case S::PLACEMENT_TYPE:
    return SIF::IS_TYPE;
  case S::COMPOSITION_COMPONENT:
    return SIF::NONE;
  case S::COMPOSITION_TYPE:
    return SIF::IS_TYPE;
  case S::SYNONYM_TYPE:
    return SIF::IS_TYPE;
  case S::C_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;
  case S::TOP_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::IS_FRAME_SCOPE;
  case S::IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::ELSE_IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::ELSE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::SWITCH_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::CASE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::DEFAULT_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::FOR_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::WHILE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::SPIN_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::WEAVE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::SCOPE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::LOCAL_SCOPE;
  case S::NAMESPACE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE;
  case S::CLASS_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::CLASS_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ENUM_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::ENUM_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::INTERFACE_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::ADAPTER_IMPLEMENTATION | SIF::IS_OBJECT_SCOPE;
  case S::CONSTRUCTOR_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::CONSTRUCTOR_IMPLEMENTATION | SIF::IS_FRAME_SCOPE;
  case S::LAYOUT_CONSTRUCTOR_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::CONSTRUCTOR_IMPLEMENTATION | SIF::IS_FRAME_SCOPE;
  case S::FUNCTION_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::FUNCTION_IMPLEMENTATION | SIF::IS_FRAME_SCOPE;
  case S::GLOBAL_VARIABLE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::GLOBAL_VARIABLE_IMPLEMENTATION;
  case S::CLASS_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::CLASS_IMPLEMENTATION | SIF::IS_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ENUM_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::ENUM_IMPLEMENTATION | SIF::IS_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::INTERFACE_IMPLEMENTATION |
           SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::ADAPTER_IMPLEMENTATION |
           SIF::IS_OBJECT_SCOPE;
  case S::FUNCTION_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::FUNCTION_IMPLEMENTATION |
           SIF::IS_FRAME_SCOPE;
  case S::GLOBAL_VARIABLE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::GLOBAL_VARIABLE_IMPLEMENTATION;
  case S::CLASS_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE;
  case S::ENUM_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE;
  case S::INTERFACE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE;
  case S::ADAPTER_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE;
  case S::FUNCTION_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE | SIF::IS_FRAME_SCOPE;
  case S::GLOBAL_VARIABLE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::GLOBAL_DECLARATION | SIF::TEMPLATE;
  case S::CLASS_POLYMORPH:
    return SIF::POLYMORPH;
  case S::ENUM_POLYMORPH:
    return SIF::POLYMORPH;
  case S::INTERFACE_POLYMORPH:
    return SIF::POLYMORPH;
  case S::ADAPTER_POLYMORPH:
    return SIF::POLYMORPH;
  case S::FUNCTION_POLYMORPH:
    return SIF::POLYMORPH;
  case S::GLOBAL_VARIABLE_POLYMORPH:
    return SIF::POLYMORPH;
  case S::CLASS_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::ENUM_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::INTERFACE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::ADAPTER_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::FUNCTION_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::GLOBAL_VARIABLE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::CONTEXTUAL_TYPE |
                                   rq::SymbolInfoFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralSymbol(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::REFLECTIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags,
                        rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFittingPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardFittingType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsQualifierType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::QUALIFIER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsModifierType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::MODIFIER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ARITHMETIC_SEQUENCE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSpecializationSet(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SPECIALIZATION_SET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TABLE_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDeclarataion(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsClassImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CLASS_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsEnumImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ENUM_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsInterfaceImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::INTERFACE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsAdapterImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ADAPTER_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsConstructorImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONSTRUCTOR_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFunctionImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::FUNCTION_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalVariableImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags,
                       rq::SymbolInfoFlags::GLOBAL_VARIABLE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE |
                                   rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_SIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_UNSIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BINARY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BFLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FRAME_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_OBJECT_SCOPE);
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind)
    : rq::Entity(rq::getSymbolId(kind)) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind Symbol::getKind() const {
  return static_cast<rq::SymbolKind>(this->_id - rq::SYMBOL_OFFSET);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Symbol::getDerivedExpressionPtr() {
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Symbol::getDerivedExpressionPtr() const {
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] inline rq::SymbolInfoFlags Symbol::getInfoFlags() const {
  return rq::getInfoFlags(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSimpleSymbol() const {
  return rq::getIsSimpleSymbol(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextual() const {
  return rq::getIsContextual(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualType() const {
  return rq::getIsContextualType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualValue() const {
  return rq::getIsContextualValue(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLiteralSymbol() const {
  return rq::getIsLiteralSymbol(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsReflectiveType() const {
  return rq::getIsReflectiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPrimitiveType() const {
  return rq::getIsPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFittingPrimitiveType() const {
  return rq::getIsFittingPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsStandardFittingType() const {
  return rq::getIsStandardFittingType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPlatformPrimitiveType() const {
  return rq::getIsPlatformPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsQualifierType() const {
  return rq::getIsQualifierType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsModifierType() const {
  return rq::getIsModifierType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsScaledPrimitiveType() const {
  return rq::getIsScaledPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSubtype() const {
  return rq::getIsSubtype(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsArithmeticSequenceType() const {
  return rq::getIsArithmeticSequenceType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSpecializationSet() const {
  return rq::getIsSpecializationSet(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsParameterList() const {
  return rq::getIsParameterList(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTableMember() const {
  return rq::getIsTableMember(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLocalDeclaration() const {
  const rq::SymbolInfoFlags flags = this->getInfoFlags();
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPolymorph() const {
  return rq::getIsPolymorph(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsWeightLevel() const {
  return rq::getIsWeightLevel(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTemplate() const {
  return rq::getIsTemplate(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSymbolTable() const {
  return rq::getIsSymbolTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLocalScope() const {
  return rq::getIsLocalScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNamedTable() const {
  return rq::getIsNamedTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsGlobalDeclarataion() const {
  return rq::getIsGlobalDeclarataion(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsImplementation() const {
  return rq::getIsImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsClassImplementation() const {
  return rq::getIsClassImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsEnumImplementation() const {
  return rq::getIsEnumImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsInterfaceImplementation() const {
  return rq::getIsInterfaceImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsAdapterImplementation() const {
  return rq::getIsAdapterImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsConstructorImplementation() const {
  return rq::getIsConstructorImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsFunctionImplementation() const {
  return rq::getIsFunctionImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsGlobalVariableImplementation() const {
  return rq::getIsGlobalVariableImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsType() const {
  return rq::getIsType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNumericType() const {
  return rq::getIsNumericType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSignedType() const {
  return rq::getIsSignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsignedType() const {
  return rq::getIsUnsignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsIntegerType() const {
  return rq::getIsIntegerType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFloatType() const {
  return rq::getIsFloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBinaryType() const {
  return rq::getIsBinaryType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBfloatType() const {
  return rq::getIsBfloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunitType() const {
  return rq::getIsCodeunitType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFrameScope() const {
  return rq::getIsFrameScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsObjectScope() const {
  return rq::getIsObjectScope(this->getKind());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id >= rq::SYMBOL_OFFSET && id < rq::CONSTANT_OFFSET;
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::SymbolKind kind)
    : Symbol(kind) {}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return symbol.getIsSimpleSymbol();
}

} // namespace rq