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

[[nodiscard]] inline rq::SymbolInfoFlags Symbol::getInfoFlags() {
  return rq::getInfoFlags(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSimpleSymbol() {
  return rq::getIsSimpleSymbol(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextual() {
  return rq::getIsContextual(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualType() {
  return rq::getIsContextualType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualValue() {
  return rq::getIsContextualValue(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLiteralSymbol() {
  return rq::getIsLiteralSymbol(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsReflectiveType() {
  return rq::getIsReflectiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPrimitiveType() {
  return rq::getIsPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFittingPrimitiveType() {
  return rq::getIsFittingPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsStandardFittingType() {
  return rq::getIsStandardFittingType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPlatformPrimitiveType() {
  return rq::getIsPlatformPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsQualifierType() {
  return rq::getIsQualifierType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsModifierType() {
  return rq::getIsModifierType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsScaledPrimitiveType() {
  return rq::getIsScaledPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSubtype() {
  return rq::getIsSubtype(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsArithmeticSequenceType() {
  return rq::getIsArithmeticSequenceType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSpecializationSet() {
  return rq::getIsSpecializationSet(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsParameterList() {
  return rq::getIsParameterList(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTableMember() {
  return rq::getIsTableMember(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLocalDeclaration() {
  const rq::SymbolInfoFlags flags = this->getInfoFlags();
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPolymorph() {
  return rq::getIsPolymorph(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsWeightLevel() {
  return rq::getIsWeightLevel(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTemplate() {
  return rq::getIsTemplate(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSymbolTable() {
  return rq::getIsSymbolTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLocalScope() {
  return rq::getIsLocalScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNamedTable() {
  return rq::getIsNamedTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsGlobalDeclarataion() {
  return rq::getIsGlobalDeclarataion(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsImplementation() {
  return rq::getIsImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsClassImplementation() {
  return rq::getIsClassImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsEnumImplementation() {
  return rq::getIsEnumImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsInterfaceImplementation() {
  return rq::getIsInterfaceImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsAdapterImplementation() {
  return rq::getIsAdapterImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsConstructorImplementation() {
  return rq::getIsConstructorImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFunctionImplementation() {
  return rq::getIsFunctionImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsGlobalVariableImplementation() {
  return rq::getIsGlobalVariableImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsType() {
  return rq::getIsType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNumericType() {
  return rq::getIsNumericType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSignedType() {
  return rq::getIsSignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsignedType() {
  return rq::getIsUnsignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsIntegerType() {
  return rq::getIsIntegerType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFloatType() {
  return rq::getIsFloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBinaryType() {
  return rq::getIsBinaryType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBfloatType() {
  return rq::getIsBfloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunitType() {
  return rq::getIsCodeunitType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFrameScope() {
  return rq::getIsFrameScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsObjectScope() {
  return rq::getIsObjectScope(this->getKind());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id >= rq::SYMBOL_OFFSET && id < rq::CONSTANT_OFFSET;
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::SymbolKind kind)
    : rq::Symbol(kind) {}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!entity.getIsSymbol()) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsSimpleSymbol(symbol.getKind());
}

RQ_ALWAYS_INLINE Literal::Literal(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool Literal::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsLiteralSymbol(symbol.getKind());
}

RQ_ALWAYS_INLINE Contextual::Contextual(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool Contextual::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsContextual(symbol.getKind());
}

RQ_ALWAYS_INLINE ReflectiveType::ReflectiveType(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool
ReflectiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsReflectiveType(symbol.getKind());
}

RQ_ALWAYS_INLINE PrimitiveType::PrimitiveType(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool PrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsPrimitiveType(symbol.getKind());
}

RQ_ALWAYS_INLINE QualifierType::QualifierType(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool QualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsQualifierType(symbol.getKind());
}

RQ_ALWAYS_INLINE ModifierType::ModifierType(rq::SymbolKind kind)
    : rq::SimpleSymbol(kind) {}

[[nodiscard]] inline bool ModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::SimpleSymbol>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsModifierType(symbol.getKind());
}

RQ_ALWAYS_INLINE IntegerLiteral::IntegerLiteral()
    : rq::Literal(rq::SymbolKind::INTEGER_LITERAL_TYPE) {}

[[nodiscard]] inline bool
IntegerLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);

  if (!llvm::isa<rq::Literal>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::INTEGER_LITERAL_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE FloatLiteral::FloatLiteral()
    : rq::Literal(rq::SymbolKind::FLOAT_LITERAL_TYPE) {}

[[nodiscard]] inline bool FloatLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Literal>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::FLOAT_LITERAL_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE StringLiteral::StringLiteral()
    : rq::Literal(rq::SymbolKind::STRING_LITERAL_TYPE) {}

[[nodiscard]] inline bool StringLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Literal>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::STRING_LITERAL_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE CodeunitLiteral::CodeunitLiteral()
    : rq::Literal(rq::SymbolKind::CODEUNIT_LITERAL_TYPE) {}

[[nodiscard]] inline bool
CodeunitLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Literal>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::CODEUNIT_LITERAL_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ContextualType::ContextualType(rq::SymbolKind kind)
    : rq::Contextual(kind) {}

[[nodiscard]] inline bool
ContextualType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Contextual>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsContextualType(symbol.getKind());
}

RQ_ALWAYS_INLINE InferenceType::InferenceType()
    : rq::ContextualType(rq::SymbolKind::INFERENCE_TYPE) {}

[[nodiscard]] inline bool InferenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::INFERENCE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VoidType::VoidType()
    : rq::ContextualType(rq::SymbolKind::VOID_TYPE) {}

[[nodiscard]] inline bool VoidType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::VOID_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE NoReturnType::NoReturnType()
    : rq::ContextualType(rq::SymbolKind::NO_RETURN_TYPE) {}

[[nodiscard]] inline bool NoReturnType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::NO_RETURN_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE UnknownType::UnknownType()
    : rq::ContextualType(rq::SymbolKind::UNKNOWN_TYPE) {}

[[nodiscard]] inline bool UnknownType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::UNKNOWN_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ContextualValue::ContextualValue(rq::SymbolKind kind)
    : rq::Contextual(kind) {}

[[nodiscard]] inline bool
ContextualValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Contextual>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsContextualValue(symbol.getKind());
}

RQ_ALWAYS_INLINE UnknownValue::UnknownValue()
    : rq::ContextualValue(rq::SymbolKind::UNKNOWN_VALUE) {}

[[nodiscard]] inline bool UnknownValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualValue>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::UNKNOWN_VALUE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ValueValue::ValueValue()
    : rq::ContextualValue(rq::SymbolKind::VALUE_VALUE) {}

[[nodiscard]] inline bool ValueValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualValue>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::VALUE_VALUE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE IndexValue::IndexValue()
    : rq::ContextualValue(rq::SymbolKind::INDEX_VALUE) {}

[[nodiscard]] inline bool IndexValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ContextualValue>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::INDEX_VALUE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SymbolType::SymbolType()
    : rq::ReflectiveType(rq::SymbolKind::SYMBOL_TYPE) {}

[[nodiscard]] inline bool SymbolType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ReflectiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SYMBOL_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ExpressionType::ExpressionType()
    : rq::ReflectiveType(rq::SymbolKind::EXPRESSION_TYPE) {}

[[nodiscard]] inline bool
ExpressionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ReflectiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::EXPRESSION_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SymbolRangeType::SymbolRangeType()
    : rq::ReflectiveType(rq::SymbolKind::SYMBOL_RANGE_TYPE) {}

[[nodiscard]] inline bool
SymbolRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ReflectiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SYMBOL_RANGE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ExpressionRangeType::ExpressionRangeType()
    : rq::ReflectiveType(rq::SymbolKind::EXPRESSION_RANGE_TYPE) {}

[[nodiscard]] inline bool
ExpressionRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ReflectiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::EXPRESSION_RANGE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE FittingPrimitiveType::FittingPrimitiveType(rq::SymbolKind kind)
    : rq::PrimitiveType(kind) {}

[[nodiscard]] inline bool
FittingPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PrimitiveType>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsFittingPrimitiveType(symbol.getKind());
}

RQ_ALWAYS_INLINE FastSignedIntegerType::FastSignedIntegerType()
    : rq::FittingPrimitiveType(rq::SymbolKind::FAST_SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
FastSignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::FittingPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::FAST_SIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE FastUnsignedIntegerType::FastUnsignedIntegerType()
    : rq::FittingPrimitiveType(rq::SymbolKind::FAST_UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
FastUnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::FittingPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::FAST_UNSIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE LeastSignedIntegerType::LeastSignedIntegerType()
    : rq::FittingPrimitiveType(rq::SymbolKind::LEAST_SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
LeastSignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::FittingPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::LEAST_SIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE LeastUnsignedIntegerType::LeastUnsignedIntegerType()
    : rq::FittingPrimitiveType(rq::SymbolKind::LEAST_UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
LeastUnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::FittingPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::LEAST_UNSIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE
StandardPrimitiveType::StandardPrimitiveType(rq::SymbolKind kind)
    : rq::PrimitiveType(kind) {}

[[nodiscard]] inline bool
StandardPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PrimitiveType>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsStandardFittingType(symbol.getKind());
}

RQ_ALWAYS_INLINE Binary16Type::Binary16Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::BINARY16_TYPE) {}

[[nodiscard]] inline bool Binary16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY16_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE Binary32Type::Binary32Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::BINARY32_TYPE) {}

[[nodiscard]] inline bool Binary32Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY32_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE Binary64Type::Binary64Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::BINARY64_TYPE) {}

[[nodiscard]] inline bool Binary64Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY64_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE Binary128Type::Binary128Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::BINARY128_TYPE) {}

[[nodiscard]] inline bool Binary128Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY128_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE BFloat16Type::BFloat16Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::BFLOAT16_TYPE) {}

[[nodiscard]] inline bool BFloat16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BFLOAT16_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE AsciiType::AsciiType()
    : rq::StandardPrimitiveType(rq::SymbolKind::ASCII_TYPE) {}

[[nodiscard]] inline bool AsciiType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::ASCII_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE Utf8Type::Utf8Type()
    : rq::StandardPrimitiveType(rq::SymbolKind::UTF8_TYPE) {}

[[nodiscard]] inline bool Utf8Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::StandardPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::UTF8_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE
PlatformPrimitiveType::PlatformPrimitiveType(rq::SymbolKind kind)
    : rq::PrimitiveType(kind) {}

[[nodiscard]] inline bool
PlatformPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PrimitiveType>(entity)) {
    return false;
  }
  const rq::Symbol &symbol = llvm::cast<const rq::Symbol>(entity);
  return rq::getIsPlatformPrimitiveType(symbol.getKind());
}

RQ_ALWAYS_INLINE HalfType::HalfType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::HALF_TYPE) {}

[[nodiscard]] inline bool HalfType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::HALF_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SingleType::SingleType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::SINGLE_TYPE) {}

[[nodiscard]] inline bool SingleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SINGLE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE DoubleType::DoubleType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::DOUBLE_TYPE) {}

[[nodiscard]] inline bool DoubleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::DOUBLE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE QuadrupleType::QuadrupleType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool QuadrupleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::QUADRUPLE_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE BooleanType::BooleanType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::BOOLEAN_TYPE) {}

[[nodiscard]] inline bool BooleanType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::BOOLEAN_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE UnsignedIntegerType::UnsignedIntegerType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
UnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::UNSIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SignedIntegerType::SignedIntegerType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
SignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_INTEGER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE UnsignedIndexType::UnsignedIndexType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::UNSIGNED_INDEX_TYPE) {}

[[nodiscard]] inline bool
UnsignedIndexType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::UNSIGNED_INDEX_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SignedIndexType::SignedIndexType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::SIGNED_INDEX_TYPE) {}

[[nodiscard]] inline bool
SignedIndexType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_INDEX_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE UnsignedAddressType::UnsignedAddressType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::UNSIGNED_ADDRESS_TYPE) {}

[[nodiscard]] inline bool
UnsignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::UNSIGNED_ADDRESS_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SignedAddressType::SignedAddressType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::SIGNED_ADDRESS_TYPE) {}

[[nodiscard]] inline bool
SignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_ADDRESS_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE CharType::CharType()
    : rq::PlatformPrimitiveType(rq::SymbolKind::CHAR_TYPE) {}

[[nodiscard]] inline bool CharType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::PlatformPrimitiveType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::CHAR_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VarQualifierType::VarQualifierType()
    : rq::QualifierType(rq::SymbolKind::VAR_QUALIFIER_TYPE) {}

[[nodiscard]] inline bool
VarQualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::QualifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::VAR_QUALIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VolatileQualifierType::VolatileQualifierType()
    : rq::QualifierType(rq::SymbolKind::VOLATILE_QUALIFIER_TYPE) {}

[[nodiscard]] inline bool
VolatileQualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::QualifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::VOLATILE_QUALIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE AtomicQualifierType::AtomicQualifierType()
    : rq::QualifierType(rq::SymbolKind::ATOMIC_QUALIFIER_TYPE) {}

[[nodiscard]] inline bool
AtomicQualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::QualifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::ATOMIC_QUALIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE NullTerminateQualifierType::NullTerminateQualifierType()
    : rq::QualifierType(rq::SymbolKind::NULL_TERMINATE_QUALIFIER_TYPE) {}

[[nodiscard]] inline bool
NullTerminateQualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::QualifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::NULL_TERMINATE_QUALIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE AnchorModifierType::AnchorModifierType()
    : rq::ModifierType(rq::SymbolKind::ANCHOR_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
AnchorModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::ANCHOR_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ContainerModifierType::ContainerModifierType()
    : rq::ModifierType(rq::SymbolKind::CONTAINER_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
ContainerModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::CONTAINER_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VisibilityModifierType::VisibilityModifierType()
    : rq::ModifierType(rq::SymbolKind::VISIBILITY_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
VisibilityModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::VISIBILITY_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE AccessModifierType::AccessModifierType()
    : rq::ModifierType(rq::SymbolKind::ACCESS_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
AccessModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::ACCESS_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE MutateModifierType::MutateModifierType()
    : rq::ModifierType(rq::SymbolKind::MUTATE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
MutateModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::MUTATE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE CohortModifierType::CohortModifierType()
    : rq::ModifierType(rq::SymbolKind::COHORT_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
CohortModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::COHORT_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE CaptureModifierType::CaptureModifierType()
    : rq::ModifierType(rq::SymbolKind::CAPTURE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
CaptureModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::CAPTURE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE LinkageModifierType::LinkageModifierType()
    : rq::ModifierType(rq::SymbolKind::LINKAGE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
LinkageModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::LINKAGE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE MangleModifierType::MangleModifierType()
    : rq::ModifierType(rq::SymbolKind::MANGLE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
MangleModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::MANGLE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE PackModifierType::PackModifierType()
    : rq::ModifierType(rq::SymbolKind::PACK_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
PackModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::PACK_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE BranchTrendModifierType::BranchTrendModifierType()
    : rq::ModifierType(rq::SymbolKind::BRANCH_TREND_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
BranchTrendModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::BRANCH_TREND_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE SupportNoticeModifierType::SupportNoticeModifierType()
    : rq::ModifierType(rq::SymbolKind::SUPPORT_NOTICE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
SupportNoticeModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::SUPPORT_NOTICE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE StableAddressModifierType::StableAddressModifierType()
    : rq::ModifierType(rq::SymbolKind::STABLE_ADDRESS_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
StableAddressModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::STABLE_ADDRESS_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VariadicModifierType::VariadicModifierType()
    : rq::ModifierType(rq::SymbolKind::VARIADIC_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
VariadicModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::VARIADIC_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE OffsetModifierType::OffsetModifierType()
    : rq::ModifierType(rq::SymbolKind::OFFSET_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
OffsetModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::OFFSET_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE TemplateModifierType::TemplateModifierType()
    : rq::ModifierType(rq::SymbolKind::TEMPLATE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
TemplateModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::TEMPLATE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE ConstraintModifierType::ConstraintModifierType()
    : rq::ModifierType(rq::SymbolKind::CONSTRAINT_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
ConstraintModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::CONSTRAINT_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE WeightModifierType::WeightModifierType()
    : rq::ModifierType(rq::SymbolKind::WEIGHT_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
WeightModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::WEIGHT_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE DeductionModifierType::DeductionModifierType()
    : rq::ModifierType(rq::SymbolKind::DEDUCTION_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
DeductionModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::DEDUCTION_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE VirtualityModifierType::VirtualityModifierType()
    : rq::ModifierType(rq::SymbolKind::VIRTUALITY_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
VirtualityModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::VIRTUALITY_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE RangerModifierType::RangerModifierType()
    : rq::ModifierType(rq::SymbolKind::RANGER_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
RangerModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::RANGER_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

RQ_ALWAYS_INLINE RequireModifierType::RequireModifierType()
    : rq::ModifierType(rq::SymbolKind::REQUIRE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
RequireModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() ==
      rq::getSymbolId(rq::SymbolKind::REQUIRE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}
RQ_ALWAYS_INLINE EnsureModifierType::EnsureModifierType()
    : rq::ModifierType(rq::SymbolKind::ENSURE_MODIFIER_TYPE) {}

[[nodiscard]] inline bool
EnsureModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::ModifierType>(entity)) {
    return false;
  }
  if (entity.getId() == rq::getSymbolId(rq::SymbolKind::ENSURE_MODIFIER_TYPE)) {
    return true;
  }
  return false;
}

explicit RQ_ALWAYS_INLINE Subtype::Subtype(rq::SymbolKind kind,
                                           rq::ConstantSymbol &child) Symbol(kind), _child_ptr(&child) {

                                           }

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Subtype::getChild() const {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Subtype::getChild() {}

[[nodiscard]] static inline bool
Subtype::classof(const rq::Entity *entity_ptr) {}

inline void Subtype::Profile(llvm::FoldingSetNodeID &inout_id) const {}

} // namespace rq