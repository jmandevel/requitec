#pragma once

#include <rq/constants.hpp>

#include <functional>

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    return "None";

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
  case S::VALUE_VALUE:
    return "ValueValue";
  case S::INDEX_VALUE:
    return "IndexValue";

  // CONTEXTUAL TYPE
  case S::INFERENCE_TYPE:
    return "InferenceType";
  case S::VOID_TYPE:
    return "VoidType";
  case S::NO_RETURN_TYPE:
    return "NoReturnType";

  // LOW ATTRIBUTE TYPES
  case S::ANCHOR_ATTRIBUTE_TYPE:
    return "AnchorAttributeType";
  case S::OPAQUE_ATTRIBUTE_TYPE:
    return "OpaqueAttributeType";
  case S::GLOBAL_ATTRIBUTE_TYPE:
    return "GlobalAttributeType";
  case S::PUBLIC_ATTRIBUTE_TYPE:
    return "PublicAttributeType";
  case S::PARTIAL_MUTATE_ATTRIBUTE_TYPE:
    return "PartialMutateAttributeType";
  case S::STATIC_ATTRIBUTE_TYPE:
    return "StaticAttributeType";
  case S::CAPTURE_ATTRIBUTE_TYPE:
    return "CaptureAttributeType";
  case S::INLINE_ATTRIBUTE_TYPE:
    return "InlineAttributeType";
  case S::MANGLE_ATTRIBUTE_TYPE:
    return "MangleAttributeType";
  case S::PACK_ATTRIBUTE_TYPE:
    return "PackAttributeType";
  case S::BRANCH_TREND_ATTRIBUTE_TYPE:
    return "BranchTrendAttributeType";
  case S::SUPPORT_STATUS_ATTRIBUTE_TYPE:
    return "SupportStatusAttributeType";
  case S::STABLE_ADDRESS_ATTRIBUTE_TYPE:
    return "StableAddressAttributeType";
  case S::VARIADIC_ATTRIBUTE_TYPE:
    return "VariadicAttributeType";
  case S::LOCATION_ATTRIBUTE_TYPE:
    return "LocationAttributeType";
  case S::TEMPLATE_ATTRIBUTE_TYPE:
    return "TemplateAttributeType";
  case S::CONSTRAINT_ATTRIBUTE_TYPE:
    return "ConstraintAttributeType";
  case S::WEIGHT_ATTRIBUTE_TYPE:
    return "WeightAttributeType";
  case S::AUTO_ATTRIBUTE_TYPE:
    return "AutoAttributeType";

  // HIGH ATTRIBUTE TYPES
  case S::VAR_ATTRIBUTE_TYPE:
    return "VarAttributeType";
  case S::VOLATILE_ATTRIBUTE_TYPE:
    return "VolatileAttributeType";
  case S::ATOMIC_ATTRIBUTE_TYPE:
    return "AtomicAttributeType";
  case S::NULL_TERMINATE_ATTRIBUTE_TYPE:
    return "NullTerminateAttributeType";
  case S::REQUIRE_ATTRIBUTE_TYPE:
    return "RequireAttributeType";
  case S::ENSURE_ATTRIBUTE_TYPE:
    return "EnsureAttributeType";

  // REFLECTIVE TYPES
  case S::SYMBOL_TYPE:
    return "SymbolType";
  case S::SYMBOL_RANGE_TYPE:
    return "SymbolRangeType";
  case S::EXPRESSION_TYPE:
    return "ExpressionType";

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
    return "Bfloat16Type";
  case S::ASCII_TYPE:
    return "AsciiType";
  case S::UTF8_TYPE:
    return "Utf8Type";

  // VARIADIC ARGUMENTS
  case S::VARIADIC_ARGUMENTS_TYPE:
    return "VariadicArgumentsType";

  // SCALED PRIMITIVE TYPES
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

  // COUNTED SUBTYPES
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

  // BLOCK
  case S::BLOCK:
    return "Block";

  // JUXTAPOSITIONAL LIST
  case S::JUXTAPOSITIONAL_LIST_ITEM:
    return "JuxtapositionalListItem";
  case S::JUXTAPOSITIONAL_LIST_TYPE:
    return "JuxtapositionalListType";

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

  // LOCAL VARIABLES
  case S::LOCAL_DYNAMIC_VARIABLE:
    return "LocalDynamicVariable";
  case S::LOCAL_STATIC_VARIABLE:
    return "LocalStaticVariable";
  case S::CAPTURE:
    return "Capture";

  // ARGUMENTS
  case S::TEMPLATE_ARGUMENT:
    return "TemplateArgument";
  case S::FUNCTION_ARGUMENT:
    return "FunctionArgument";

  // PARAMETERS
  case S::PARAMETER:
    return "Parameter";

  // PARAMETER LISTS
  case S::SIGNATURE:
    return "Signature";
  case S::LAYOUT:
    return "Layout";

  // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return "PlacementTYpe";

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return "CompositionComponent";
  case S::COMPOSITION_TYPE:
    return "CompositionType";

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return "SynonymType";

  // SYMBOL TABLES
  case S::C:
    return "C";
  case S::TOP:
    return "Top";

  // LOCAL STATEMENTS
  case S::IF_STATEMENT:
    return "IfStatement";
  case S::ELSE_IF_STATEMENT:
    return "ElseIfStatement";
  case S::ELSE_STATEMENT:
    return "ElseStatement";
  case S::MATCH_STATEMENT:
    return "MatchStatement";
  case S::SWITCH_STATEMENT:
    return "SwitchStatement";
  case S::CASE_STATEMENT:
    return "CaseStatement";
  case S::WITH_STATEMENT:
    return "WithStatement";
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
  case S::GLOBAL_DYNAMIC_VARIABLE_OVERLOAD:
    return "GlobalDynamicVariableOverload";
  case S::GLOBAL_STATIC_VARIABLE_OVERLOAD:
    return "GlobalStaticVariableOverload";

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
  case S::GLOBAL_DYNAMIC_VARIABLE_SPECIALIZATION:
    return "GlobalDynamicVariableSpecialization";
  case S::GLOBAL_STATIC_VARIABLE_SPECIALIZATION:
    return "GlobalStaticVariableSpecialization";

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
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
    return "GlobalDynamicVariableTemplate";
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return "GlobalStaticVariableTemplate";

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
  case S::GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
    return "GlobalDynamicVariablePolymorph";
  case S::GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return "GlobalStaticVariablePolymorph";

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
  case S::GLOBAL_DYNAMIC_VARIABLE_WEIGHT_LEVEL:
    return "GlobalDynamicVariableWeightLevel";
  case S::GLOBAL_STATIC_VARIABLE_WEIGHT_LEVEL:
    return "GlobalStaticVariableWeightLevel";

  // OVERLOAD OVERRIDES
  case S::ADAPTER_OVERLOAD_OVERRIDE:
    return "AdapterOverloadOverride";
  case S::FUNCTION_OVERLOAD_OVERRIDE:
    return "FunctionOverloadOverride";

  // TEMPLATE OVERRIDES
  case S::CLASS_TEMPLATE_OVERRIDE:
    return "ClassTemplateOverride";
  case S::ENUM_TEMPLATE_OVERRIDE:
    return "EnumTemplateOverride";
  case S::INTERFACE_TEMPLATE_OVERRIDE:
    return "InterfaceTemplateOverride";
  case S::ADAPTER_TEMPLATE_OVERRIDE:
    return "AdapterTemplateOverride";
  case S::FUNCTION_TEMPLATE_OVERRIDE:
    return "FunctionTemplateOverride";
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE_OVERRIDE:
    return "GlobalDynamicVariableTemplateOverride";
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE_OVERRIDE:
    return "GlobalStaticVariableTemplateOverride";

  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::SymbolInfoFlags getInfoFlags(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  using SIF = rq::SymbolInfoFlags;
  switch (kind) {
  case S::NONE:
    return SIF::NONE;

  // LITERALS
  case S::INTEGER_LITERAL_TYPE:
    return SIF::LITERAL | SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::FLOAT_LITERAL_TYPE:
    return SIF::LITERAL | SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::STRING_LITERAL_TYPE:
    return SIF::LITERAL | SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::CODEUNIT_LITERAL_TYPE:
    return SIF::LITERAL | SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;

  // CONTEXTUAL VALUE
  case S::VALUE_VALUE:
    return SIF::CONTEXTUAL_VALUE | SIF::SIMPLE_SYMBOL;
  case S::INDEX_VALUE:
    return SIF::CONTEXTUAL_VALUE | SIF::SIMPLE_SYMBOL;

  // CONTEXTUAL TYPE
  case S::INFERENCE_TYPE:
    return SIF::CONTEXTUAL_TYPE | SIF::SIMPLE_SYMBOL;
  case S::VOID_TYPE:
    return SIF::CONTEXTUAL_TYPE | SIF::SIMPLE_SYMBOL;
  case S::NO_RETURN_TYPE:
    return SIF::CONTEXTUAL_TYPE | SIF::SIMPLE_SYMBOL;

  // LOW ATTRIBUTE TYPES
  case S::ANCHOR_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::OPAQUE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::GLOBAL_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::PUBLIC_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::PARTIAL_MUTATE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::STATIC_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::CAPTURE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::INLINE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::MANGLE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::PACK_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::BRANCH_TREND_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::SUPPORT_STATUS_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::STABLE_ADDRESS_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::VARIADIC_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::LOCATION_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::TEMPLATE_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::CONSTRAINT_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::WEIGHT_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::AUTO_ATTRIBUTE_TYPE:
    return SIF::LOW_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;

  // HIGH ATTRIBUTE TYPES
  case S::VAR_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::VOLATILE_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::ATOMIC_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::NULL_TERMINATE_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::REQUIRE_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::ENSURE_ATTRIBUTE_TYPE:
    return SIF::HIGH_ATTRIBUTE_TYPE | SIF::SIMPLE_SYMBOL;

  // REFLECTIVE TYPES
  case S::SYMBOL_TYPE:
    return SIF::REFLECTIVE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::SYMBOL_RANGE_TYPE:
    return SIF::REFLECTIVE_TYPE | SIF::SIMPLE_SYMBOL;
  case S::EXPRESSION_TYPE:
    return SIF::REFLECTIVE_TYPE | SIF::SIMPLE_SYMBOL;

  // PLATFORM PRIMITIVE TYPES
  case S::BOOLEAN_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL;
  case S::HALF_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SINGLE_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::DOUBLE_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::QUADRUPLE_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SIGNED_INTEGER_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INTEGER_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_INDEX_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INDEX_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_ADDRESS_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_ADDRESS_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::CHAR_TYPE:
    return SIF::PLATFORM_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;

  // FITTING PRIMITIVE TYPES
  case S::FAST_SIGNED_INTEGER_TYPE:
    return SIF::FITTING_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;
  case S::FAST_UNSIGNED_INTEGER_TYPE:
    return SIF::FITTING_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;
  case S::LEAST_SIGNED_INTEGER_TYPE:
    return SIF::FITTING_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;
  case S::LEAST_UNSIGNED_INTEGER_TYPE:
    return SIF::FITTING_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;

  // STANDARD PRIMITIVE TYPES
  case S::BINARY16_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE |
           SIF::IS_BINARY_TYPE;
  case S::BINARY32_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE |
           SIF::IS_BINARY_TYPE;
  case S::BINARY64_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE |
           SIF::IS_BINARY_TYPE;
  case S::BINARY128_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE |
           SIF::IS_BINARY_TYPE;
  case S::BFLOAT16_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE |
           SIF::IS_BFLOAT_TYPE;
  case S::ASCII_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;
  case S::UTF8_TYPE:
    return SIF::STANDARD_PRIMITIVE_TYPE | SIF::PRIMITIVE_TYPE |
           SIF::SIMPLE_SYMBOL | SIF::IS_CODEUNIT_TYPE;

  // VARIADIC ARGUMENTS
  case S::VARIADIC_ARGUMENTS_TYPE:
    return SIF::SIMPLE_SYMBOL;

  // SCALED PRIMITIVE TYPES
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_SIGNED_TYPE |
           SIF::IS_INTEGER_TYPE;
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_UNSIGNED_TYPE |
           SIF::IS_INTEGER_TYPE;

    // SUBTYPES
  case S::REFERENCE_SUBTYPE:
    return SIF::SUBTYPE;
  case S::POINTER_SUBTYPE:
    return SIF::SUBTYPE;
  case S::SLICE_SUBTYPE:
    return SIF::SUBTYPE;
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return SIF::SUBTYPE;

  // COUNTED SUBTYPES
  case S::ARRAY_SUBTYPE:
    return SIF::COUNTED_SUBTYPE | SIF::SUBTYPE;

  // MODULES
  case S::MODULE:
    return SIF::NONE;

  // IMPORTS
  case S::IMPORT:
    return SIF::HAS_LOW_ATTRIBUTES;

  // CONFORMITY
  case S::CONFORMITY:
    return SIF::NONE;

  // BLOCK
  case S::BLOCK:
    return SIF::NONE;

  // JUXTAPOSITIONAL LIST
  case S::JUXTAPOSITIONAL_LIST_ITEM:
    return SIF::NONE;
  case S::JUXTAPOSITIONAL_LIST_TYPE:
    return SIF::IS_TYPE;

  // ARITHMETIC SEQUENCES
  case S::ARITHMETIC_INTERVAL_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;

  // LOCAL DECLARATIONS
  case S::ANCHOR:
    return SIF::LOCAL_DECLARATION;
  case S::ENUMERATOR:
    return SIF::LOCAL_DECLARATION;

  // LOCAL VARIABLES
  case S::LOCAL_DYNAMIC_VARIABLE:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION |
           SIF::HAS_LOW_ATTRIBUTES;
  case S::LOCAL_STATIC_VARIABLE:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION |
           SIF::HAS_LOW_ATTRIBUTES;
  case S::CAPTURE:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION;
  case S::TEMPLATE_ARGUMENT:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION |
           SIF::HAS_LOW_ATTRIBUTES;
  case S::FUNCTION_ARGUMENT:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION |
           SIF::HAS_LOW_ATTRIBUTES;

  // PARAMETERS
  case S::PARAMETER:
    return SIF::LOCAL_VARIABLE | SIF::LOCAL_DECLARATION |
           SIF::HAS_LOW_ATTRIBUTES;

  // PARAMETER LISTS
  case S::SIGNATURE:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;
  case S::LAYOUT:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;

  // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return SIF::IS_TYPE;

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return SIF::NONE;
  case S::COMPOSITION_TYPE:
    return SIF::IS_TYPE;

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return SIF::IS_TYPE;

  // SYMBOL TABLES
  case S::C:
    return SIF::SYMBOL_TABLE;
  case S::TOP:
    return SIF::SYMBOL_TABLE | SIF::IS_FRAME_SCOPE;

  // LOCAL STATEMENTS
  case S::IF_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::ELSE_IF_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::ELSE_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::MATCH_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::SWITCH_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::CASE_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::WITH_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::DEFAULT_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::FOR_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::WHILE_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::SPIN_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::WEAVE_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;
  case S::SCOPE_STATEMENT:
    return SIF::LOCAL_STATEMENT | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;

  // NAMED TABLE
  case S::NAMESPACE:
    return SIF::NAMED_TABLE | SIF::SYMBOL_TABLE | SIF::HAS_LOW_ATTRIBUTES;

  // OVERLOADS
  case S::CLASS_OVERLOAD:
    return SIF::OVERLOAD | SIF::CLASS_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ENUM_OVERLOAD:
    return SIF::OVERLOAD | SIF::ENUM_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_OVERLOAD:
    return SIF::OVERLOAD | SIF::INTERFACE_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_OVERLOAD:
    return SIF::OVERLOAD | SIF::ADAPTER_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_OBJECT_SCOPE;
  case S::CONSTRUCTOR_OVERLOAD:
    return SIF::OVERLOAD | SIF::CONSTRUCTOR_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_FRAME_SCOPE;
  case S::LAYOUT_CONSTRUCTOR_OVERLOAD:
    return SIF::OVERLOAD | SIF::CONSTRUCTOR_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_FRAME_SCOPE;
  case S::FUNCTION_OVERLOAD:
    return SIF::OVERLOAD | SIF::FUNCTION_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE |
           SIF::IS_FRAME_SCOPE;
  case S::GLOBAL_DYNAMIC_VARIABLE_OVERLOAD:
    return SIF::OVERLOAD | SIF::GLOBAL_VARIABLE_OVERLOAD |
           SIF::GLOBAL_DYNAMIC_VARIABLE_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::GLOBAL_STATIC_VARIABLE_OVERLOAD:
    return SIF::OVERLOAD | SIF::GLOBAL_VARIABLE_OVERLOAD |
           SIF::GLOBAL_STATIC_VARIABLE_VARIANT | SIF::VARIANT |
           SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;

    // TODO rest of symbol details flags
  // SPECIALIZATIONS
  case S::CLASS_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE | SIF::IS_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ENUM_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::INTERFACE_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::ADAPTER_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::FUNCTION_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::GLOBAL_DYNAMIC_VARIABLE_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::GLOBAL_STATIC_VARIABLE_SPECIALIZATION:
    return SIF::SPECIALIZATION | SIF::VARIANT | SIF::GLOBAL_DECLARATION |
           SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::ENUM_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::INTERFACE_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::ADAPTER_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::FUNCTION_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return SIF::GLOBAL_DECLARATION | SIF::NAMED_TABLE | SIF::SYMBOL_TABLE;

  // POLYMORPHS
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
  case S::GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
    return SIF::POLYMORPH;
  case S::GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return SIF::POLYMORPH;

  // WEIGHT LEVELS
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
  case S::GLOBAL_DYNAMIC_VARIABLE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::GLOBAL_STATIC_VARIABLE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;

  // OVERLOAD OVERRIDES
  case S::ADAPTER_OVERLOAD_OVERRIDE:
    return SIF::OVERLOAD_OVERRIDE | SIF::OVERRIDE;
  case S::FUNCTION_OVERLOAD_OVERRIDE:
    return SIF::OVERLOAD_OVERRIDE | SIF::OVERRIDE;

  // TEMPLATE OVERRIDES
  case S::CLASS_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::ENUM_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::INTERFACE_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::ADAPTER_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::FUNCTION_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE_OVERRIDE:
    return SIF::TEMPLATE_OVERRIDE | SIF::OVERRIDE;

  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::CONTEXTUAL_VALUE |
                                   rq::SymbolInfoFlags::CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLowAttributeType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOW_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsHighAttributeType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::HIGH_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::REFLECTIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFittingPrimitiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledPrimitiveType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::COUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ARITHMETIC_SEQUENCE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArgument(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LOCAL_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDeclaration(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CLASS_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ENUM_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInterfaceVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::INTERFACE_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsAdapterVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ADAPTER_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunctionVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::FUNCTION_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDynamicVariableVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags,
                       rq::SymbolInfoFlags::GLOBAL_DYNAMIC_VARIABLE_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalStaticVariableVariant(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags,
                       rq::SymbolInfoFlags::GLOBAL_STATIC_VARIABLE_VARIANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverload(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::OVERLOAD);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecialization(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SPECIALIZATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalVariableOverload(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::GLOBAL_VARIABLE_OVERLOAD);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverride(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::OVERRIDE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverloadOverride(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::OVERLOAD_OVERRIDE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateOverride(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TEMPLATE_OVERRIDE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE |
                                   rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_SIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_UNSIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BINARY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BFLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLowAttributes(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::HAS_LOW_ATTRIBUTES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FRAME_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope(rq::SymbolKind kind) {
  rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_OBJECT_SCOPE);
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind)
    : Entity(rq::getUnderlyingValue(kind) + rq::SYMBOL_OFFSET) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::operator==(const Self &rhs) const {
  return this == &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::operator!=(const Self &rhs) const {
  return this != &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind Symbol::getKind() const {
  return static_cast<rq::SymbolKind>(this->getId() - rq::SYMBOL_OFFSET);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolInfoFlags
Symbol::getInfoFlags() const {
  return rq::getInfoFlags(this->getKind());
}

[[nodiscard]] inline rq::LowFuseFlags Symbol::getDerivedLowFuseFlags() const {
  if (llvm::isa<rq::Import>(*this)) {
    const rq::Import &import = llvm::cast<rq::Import>(*this);
    return import.getLowFuseFlags();
  }
  if (llvm::isa<rq::LocalVariable>(*this)) {
    const rq::LocalVariable &local = llvm::cast<rq::LocalVariable>(*this);
    return local.getLowFuseFlags();
  }
  if (llvm::isa<rq::SymbolParameter>(*this)) {
    const rq::SymbolParameter &parameter =
        llvm::cast<rq::SymbolParameter>(*this);
    return parameter.getLowFuseFlags();
  }
  if (llvm::isa<rq::LocalScope>(*this)) {
    const rq::LocalScope &statement = llvm::cast<rq::LocalScope>(*this);
    return statement.getLowFuseFlags();
  }
  if (llvm::isa<rq::GlobalDeclaration>(*this)) {
    const rq::GlobalDeclaration &decl =
        llvm::cast<rq::GlobalDeclaration>(*this);
    return decl.getLowFuseFlags();
  }
  return {};
}

[[nodiscard]] inline const rq::Expression *
Symbol::getDerivedExpressionPtr() const {
  if (llvm::isa<rq::Module>(*this)) {
    const rq::Module &module = llvm::cast<rq::Module>(*this);
    return &module.getExpression();
  }
  if (llvm::isa<rq::Import>(*this)) {
    const rq::Import &import = llvm::cast<rq::Import>(*this);
    return &import.getExpression();
  }
  if (llvm::isa<rq::SymbolParameter>(*this)) {
    const rq::SymbolParameter &parameter =
        llvm::cast<rq::SymbolParameter>(*this);
    return &parameter.getExpression();
  }
  if (llvm::isa<rq::SymbolParameterList>(*this)) {
    const rq::SymbolParameterList &list =
        llvm::cast<rq::SymbolParameterList>(*this);
    return &list.getExpression();
  }
  if (llvm::isa<rq::LocalScope>(*this)) {
    const rq::LocalScope &statement = llvm::cast<rq::LocalScope>(*this);
    return &statement.getExpression();
  }
  if (llvm::isa<rq::GlobalDeclaration>(*this)) {
    const rq::GlobalDeclaration &decl =
        llvm::cast<rq::GlobalDeclaration>(*this);
    return &decl.getExpression();
  }
  return nullptr;
}

[[nodiscard]] inline rq::Expression *Symbol::getDerivedExpressionPtr() {
  if (llvm::isa<rq::Module>(*this)) {
    rq::Module &module = llvm::cast<rq::Module>(*this);
    return &module.getExpression();
  }
  if (llvm::isa<rq::Import>(*this)) {
    rq::Import &import = llvm::cast<rq::Import>(*this);
    return &import.getExpression();
  }
  if (llvm::isa<rq::SymbolParameter>(*this)) {
    rq::SymbolParameter &parameter = llvm::cast<rq::SymbolParameter>(*this);
    return &parameter.getExpression();
  }
  if (llvm::isa<rq::SymbolParameterList>(*this)) {
    rq::SymbolParameterList &list = llvm::cast<rq::SymbolParameterList>(*this);
    return &list.getExpression();
  }
  if (llvm::isa<rq::LocalScope>(*this)) {
    rq::LocalScope &statement = llvm::cast<rq::LocalScope>(*this);
    return &statement.getExpression();
  }
  if (llvm::isa<rq::GlobalDeclaration>(*this)) {
    rq::GlobalDeclaration &decl = llvm::cast<rq::GlobalDeclaration>(*this);
    return &decl.getExpression();
  }
  return nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLiteralType() const {
  return rq::getIsLiteralType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsObliqueLiteralType() const {
  if (rq::getIsLiteralType(this->getKind())) {
    return true;
  }
  if (llvm::isa<rq::Subtype>(*this)) {
    const rq::Subtype &subtype = llvm::cast<rq::Subtype>(*this);
    return subtype.getChild().getSymbol().getIsObliqueLiteralType();
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextual() const {
  return rq::getIsContextual(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualValue() const {
  return rq::getIsContextualValue(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualType() const {
  return rq::getIsContextualType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLowAttributeType() const {
  return rq::getIsLowAttributeType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsHighAttributeType() const {
  return rq::getIsHighAttributeType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsReflectiveType() const {
  return rq::getIsReflectiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPlatformPrimitiveType() const {
  return rq::getIsPlatformPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsStandardPrimitiveType() const {
  return rq::getIsStandardPrimitiveType(this->getKind());
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

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasLowAttributes() const {
  return rq::getHasLowAttributes(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFrameScope() const {
  return rq::getIsFrameScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsObjectScope() const {
  return rq::getIsObjectScope(this->getKind());
}

[[nodiscard]] inline bool Symbol::getIsCompleteType() const {
  if (llvm::isa<rq::InferenceType>(*this)) {
    return false;
  }
  if (llvm::isa<rq::InferenceCountArraySubtype>(*this)) {
    return false;
  }
  if (llvm::isa<rq::Subtype>(*this)) {
    const rq::Subtype &subtype = llvm::cast<rq::Subtype>(*this);
    return subtype.getChild().getSymbol().getIsCompleteType();
  }
  return true;
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id >= rq::SYMBOL_OFFSET && id < rq::CONSTANT_OFFSET;
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::SymbolKind kind)
    : Symbol(kind) {
  RQ_ASSERT(rq::getIsSimpleSymbol(kind), "not simple symbol");
}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsSimpleSymbol(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedSimpleSymbol<KIND_PARAM>::DerivedSimpleSymbol()
    : SimpleSymbol(KIND_PARAM) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedSimpleSymbol<KIND_PARAM>::classof(const Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(KIND_PARAM);
}

RQ_ALWAYS_INLINE
ScaledPrimitiveType::ScaledPrimitiveType(rq::SymbolKind kind,
                                         rq::ScaleKind scale_kind,
                                         unsigned scale,
                                         std::uint64_t synonym_id)
    : Symbol(kind), _scale_kind(scale_kind), _scale(scale),
      _synonym_id(synonym_id) {
  RQ_ASSERT(rq::getIsScaledPrimitive(kind), "not scaled primitive");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaleKind
ScaledPrimitiveType::getScaleKind() const {
  return this->_scale_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned ScaledPrimitiveType::getScale() const {
  return this->_scale;
}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t
ScaledPrimitiveType::getSynonymTypeId() const {
  return this->_synonym_id;
}

[[nodiscard]] inline bool
ScaledPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsScaledPrimitive(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

inline void
ScaledPrimitiveType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileScaledPrimitiveType(inout_id, this->getKind(),
                                 this->getScaleKind(), this->getScale(),
                                 this->getSynonymTypeId());
}

RQ_ALWAYS_INLINE void
profileScaledPrimitiveType(llvm::FoldingSetNodeID &inout_id,
                           rq::SymbolKind kind, rq::ScaleKind scale_kind,
                           unsigned scale, std::uint64_t synonum_id) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddInteger(rq::getUnderlyingValue(scale_kind));
  inout_id.AddInteger(scale);
  inout_id.AddInteger(synonum_id);
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE
DerivedScaledPrimitiveType<KIND_PARAM>::DerivedScaledPrimitiveType(
    rq::ScaleKind scale_kind, unsigned scale, std::uint64_t synonym_id)
    : ScaledPrimitiveType(KIND_PARAM, scale_kind, scale, synonym_id) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedScaledPrimitiveType<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(KIND_PARAM);
}

RQ_ALWAYS_INLINE Subtype::Subtype(rq::SymbolKind kind,
                                  rq::ConstantSymbol &child)
    : Symbol(kind), _child_ptr(&child) {
  RQ_ASSERT(rq::getIsSubtype(kind), "not subtype");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Subtype::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Subtype::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] inline bool Subtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsSubtype(static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(rq::ConstantSymbol &child,
                                            std::uint64_t count)
    : Subtype(rq::SymbolKind::ARRAY_SUBTYPE, child), _count(count) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t ArraySubtype::getCount() const {
  return this->_count;
}

[[nodiscard]] inline bool ArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::ARRAY_SUBTYPE);
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileArraySubtype(inout_id, this->getChild(), this->getCount());
}

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &inout_id,
                                          const rq::ConstantSymbol &child,
                                          std::uint64_t count) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(count);
}

RQ_ALWAYS_INLINE UncountedSubtype::UncountedSubtype(rq::SymbolKind kind,
                                                    rq::ConstantSymbol &child)
    : Subtype(kind, child) {
  RQ_ASSERT(rq::getIsUncountedSubtype(kind), "not uncounted subtype");
}

[[nodiscard]] inline bool
UncountedSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsUncountedSubtype(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

inline void UncountedSubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  return rq::profileUncountedSubtype(inout_id, this->getKind(),
                                     this->getChild());
}

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &inout_id,
                                              rq::SymbolKind kind,
                                              const rq::ConstantSymbol &child) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(&child);
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE
DerivedUncountableSubtype<KIND_PARAM>::DerivedUncountableSubtype(
    rq::ConstantSymbol &child)
    : UncountedSubtype(KIND_PARAM, child) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedUncountableSubtype<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(KIND_PARAM);
}

RQ_ALWAYS_INLINE
ModuleFactory::ModuleFactory(rq::ModuleKind kind, llvm::StringRef path,
                             llvm::StringRef buffer)
    : _module_kind(kind), _path(path), _buffer(buffer) {
  RQ_ASSERT(kind != rq::ModuleKind::NONE, "none module kind");
  RQ_ASSERT(!path.empty(), "empty path");
  RQ_ASSERT(!buffer.empty(), "empty file buffer");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ModuleFactory::getIsEmpty() const {
  return this->_module_kind == rq::ModuleKind::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind ModuleFactory::getKind() const {
  return this->_module_kind;
}

RQ_ALWAYS_INLINE void
ModuleFactory::setOrChangeExpression(rq::Expression *expression_ptr) {
  this->_expression_ptr = expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
ModuleFactory::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
ModuleFactory::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleFactory::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
ModuleFactory::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &
ModuleFactory::getTokens() {
  return this->_tokens;
}

[[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
ModuleFactory::getTokens() const {
  return this->_tokens;
}

RQ_ALWAYS_INLINE Module::Module(rq::ModuleFactory &&factory)
    : Symbol(rq::SymbolKind::MODULE), _module_kind(factory.getKind()),
      _expression_ptr(llvm::cast<rq::Expression>(factory.getExpressionPtr())),
      _path(factory.getPath()), _buffer(factory.getBuffer()) {
  RQ_ASSERT(factory.getExpressionPtr() != nullptr, "expression not set");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind Module::getModuleKind() const {
  return this->_module_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Module::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Module::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] inline bool Module::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUNDERLYING_VALUE(rq::SymbolKind::MODULE);
}

RQ_ALWAYS_INLINE Import::Import(rq::LowFuseFlags flags,
                                rq::Expression &expression,
                                rq::Module &imported, rq::Module &module)
    : Symbol(rq::SymbolKind::IMPORT), _low_flags(flags),
      _expression_ptr(&expression), _imported_ptr(&imported),
      _module_ptr(&module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Import::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Import::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags
Import::getLowFuseFlags() const {
  return this->_low_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &Import::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &Import::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &Import::getImported() const {
  return rq::dereferencePtr(this->_imported_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &Import::getImported() {
  return rq::dereferencePtr(this->_imported_ptr);
}

[[nodiscard]] inline bool Import::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUNDERLYING_VALUE(rq::SymbolKind::IMPORT);
}

RQ_ALWAYS_INLINE Conformity::Conformity(rq::Interface &interface,
                                        rq::Symbol &type)
    : Symbol(rq::SymbolKind::CONFORMITY), _interface_ptr(&interface),
      _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface &
Conformity::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Interface &Conformity::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &Conformity::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &Conformity::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] inline bool Conformity::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::CONFORMITY);
}

RQ_ALWAYS_INLINE Block::Block(rq::Instruction *outer_instruction_ptr)
    : Symbol(rq::SymbolKind::BLOCK),
      _outer_instruction_ptr(outer_instruction_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction *
Block::getOuterInstructionPtr() const {
  return this->_outer_instruction_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction *
Block::getOuterInstructionPtr() {
  return this->_outer_instruction_ptr;
}

RQ_ALWAYS_INLINE void Block::setLlvmBlock(llvm::BasicBlock &llvm_block) {
  rq::assignSingleValue(this->_llvm_block_ptr, &llvm_block);
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::BasicBlock &
Block::getLlvmBlock() const {
  return rq::dereferencePtr(this->_llvm_block_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::BasicBlock &Block::getLlvmBlock() {
  return rq::dereferencePtr(this->_llvm_block_ptr);
}

[[nodiscard]] inline bool Block::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::BLOCK);
}

RQ_ALWAYS_INLINE
JuxtapositionalListItem::JuxtapositionalListItem(
    rq::ConstantSymbol &type, rq::JuxtapositionalListItem *next_ptr)
    : Symbol(rq::SymbolKind::JUXTAPOSITIONAL_LIST_ITEM), _next_ptr(next_ptr),
      _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
JuxtapositionalListItem::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &
JuxtapositionalListItem::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] inline bool
JuxtapositionalListItem::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::JUXTAPOSITIONAL_LIST_ITEM);
}

inline void
JuxtapositionalListItem::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileJuxtapositionalListItem(inout_id, this->getType(),
                                     this->_next_ptr);
}

RQ_ALWAYS_INLINE void
profileJuxtapositionalListItem(llvm::FoldingSetNodeID &inout_id,
                               const rq::ConstantSymbol &type,
                               const rq::JuxtapositionalListItem *next_ptr) {
  inout_id.AddPointer(&type);
  inout_id.AddPointer(next_ptr);
}

RQ_ALWAYS_INLINE
JuxtapositionalListType::JuxtapositionalListType(
    rq::JuxtapositionalListItem &first_item)
    : Symbol(rq::SymbolKind::JUXTAPOSITIONAL_LIST_TYPE),
      _first_item_ptr(&first_item) {}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::JuxtapositionalListItem>,
                          rq::NextIterator<rq::JuxtapositionalListItem>,
                          std::ranges::subrange_kind::unsized>
    JuxtapositionalListType::getJuxtapositionalListItemSubrange() {
  return std::ranges::subrange<rq::NextIterator<rq::JuxtapositionalListItem>,
                               rq::NextIterator<rq::JuxtapositionalListItem>,
                               std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::JuxtapositionalListItem>(this->_first_item_ptr),
      rq::NextIterator<rq::JuxtapositionalListItem>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstNextIterator<rq::JuxtapositionalListItem>,
                          rq::ConstNextIterator<rq::JuxtapositionalListItem>,
                          std::ranges::subrange_kind::unsized>
    JuxtapositionalListType::getJuxtapositionalListItemSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::JuxtapositionalListItem>,
      rq::ConstNextIterator<rq::JuxtapositionalListItem>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::JuxtapositionalListItem>(this->_first_item_ptr),
      rq::ConstNextIterator<rq::JuxtapositionalListItem>());
}

[[nodiscard]] inline bool
JuxtapositionalListType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::JUXTAPOSITIONAL_LIST_TYPE);
}

inline void
JuxtapositionalListType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  const rq::JuxtapositionalListItem &first_item =
      rq::dereferencePtr(this->_first_item_ptr);
  rq::profileJuxtapositionalListType(inout_id, first_item);
}

inline void
profileJuxtapositionalListType(llvm::FoldingSetNodeID &inout_id,
                               const rq::JuxtapositionalListItem &first_item) {
  inout_id.AddPointer(&first_item);
}

RQ_ALWAYS_INLINE ArithmeticSequenceType::ArithmeticSequenceType(
    rq::SymbolKind kind, rq::ConstantSymbol &child,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step)
    : Symbol(kind), _child_ptr(&child), _condition(condition), _step(step) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
ArithmeticSequenceType::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &
ArithmeticSequenceType::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
ArithmeticSequenceType::getCondition() const {
  return this->_condition;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep
ArithmeticSequenceType::getStep() const {
  return this->_step;
}

[[nodiscard]] inline bool
ArithmeticSequenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsArithmeticSequenceType(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

inline void
ArithmeticSequenceType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileArithmeticSequenceType(inout_id, this->getKind(), this->getChild(),
                                    this->getCondition(), this->getStep());
}

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
    const rq::ConstantSymbol &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  inout_id.AddInteger(rq::getUNDERLYING_VALUE(kind));
  inout_id.AddPointer(&child);
  inout_id.AddInteger(rq::getUNDERLYING_VALUE(condition));
  inout_id.AddInteger(rq::getUNDERLYING_VALUE(step));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE
DerivedArithmeticSequenceType<KIND_PARAM>::DerivedArithmeticSequenceType(
    rq::ConstantSymbol &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step)
    : ArithmeticSequenceType(KIND_PARAM, child, condition, step) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool DerivedArithmeticSequenceType<KIND_PARAM>::classof(
    const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(KIND_PARAM);
}

RQ_ALWAYS_INLINE
LocalDeclaration::LocalDeclaration(rq::SymbolKind kind, rq::Name name,
                                   rq::SymbolTable &container,
                                   rq::SymbolTable &host, rq::Module &module)
    : Symbol(kind), _name(name), _container_ptr(&container), _host_ptr(&host),
      _module_ptr(&module) {
  RQ_ASSERT(rq::getIsLocalDeclaration(kind), "not local declaration");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name LocalDeclaration::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
LocalDeclaration::getContainer() const {
  return rq::dereferencePtr(this->_container_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
LocalDeclaration::getContainer() {
  return rq::dereferencePtr(this->_container_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
LocalDeclaration::getHost() const {
  return rq::dereferencePtr(this->_host_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &LocalDeclaration::getHost() {
  return rq::dereferencePtr(this->_host_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
LocalDeclaration::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &LocalDeclaration::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] inline bool
LocalDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsLocalDeclaration(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE
Anchor::Anchor(rq::Name name, rq::SymbolTable &container, rq::Module &module,
               rq::SymbolTable &vessel)
    : LocalDeclaration(rq::SymbolKind::ANCHOR, name, container, container,
                       module),
      _vessel_ptr(&vessel) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
Anchor::getVessel() const {
  return rq::dereferencePtr(this->_vessel_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &Anchor::getVessel() {
  return rq::dereferencePtr(this->_vessel_ptr);
}

[[nodiscard]] inline bool Anchor::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::ANCHOR);
}

RQ_ALWAYS_INLINE
Enumerator::Enumerator(rq::Name name, rq::SymbolTable &container,
                       rq::SymbolTable &host, rq::Module &module,
                       rq::ConstantSymbol *type_ptr, llvm::APInt discriminant)
    : LocalDeclaration(rq::SymbolKind::ENUMERATOR, name, container, host,
                       module),
      _type_ptr(type_ptr), _discriminant_value(discriminant) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
Enumerator::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *Enumerator::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt
Enumerator::getDiscriminantValue() const {
  return this->_discriminant_value;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt Enumerator::getDiscriminantValue() {
  return this->_discriminant_value;
}

[[nodiscard]] inline bool Enumerator::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::ENUMERATOR);
}

RQ_ALWAYS_INLINE
LocalVariable::LocalVariable(rq::SymbolKind kind, rq::Name name,
                             rq::SymbolTable &container, rq::SymbolTable &host,
                             rq::Module &module, rq::LowFuseFlags flags,
                             rq::ConstantSymbol &type)
    : LocalDeclaration(kind, name, container, host, module), _low_flags(flags),
      _type_ptr(&type) {
  RQ_ASSERT(rq::getIsLocalVariable(kind), "not local variable");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags
LocalVariable::getLowFuseFlags() const {
  return this->_low_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
LocalVariable::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &LocalVariable::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

RQ_ALWAYS_INLINE void LocalVariable::completeType(rq::ConstantSymbol &type) {
  RQ_ASSERT(!this->getType().getSymbol().getIsCompleteType(),
            "already complete");
  RQ_ASSERT(type.getSymbol().getIsCompleteType(), "not complete");
  this->_type_ptr = &type;
}

[[nodiscard]] inline bool LocalVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsLocalVariable(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE LocalDynamicVariable::LocalDynamicVariable(
    rq::Name name, rq::SymbolTable &container, rq::SymbolTable &host,
    rq::Module &module, rq::LowFuseFlags flags, rq::ConstantSymbol &type)
    : LocalVariable(rq::SymbolKind::LOCAL_DYNAMIC_VARIABLE, name, container,
                    host, module, flags, type) {}

RQ_ALWAYS_INLINE void
LocalDynamicVariable::setLlvmLocation(llvm::Value &llvm_location) {
  rq::assignSingleValue(this->_llvm_location_ptr, &llvm_location);
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *
LocalDynamicVariable::getLlvmLocationPtr() const {
  return this->_llvm_location_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *
LocalDynamicVariable::getLlvmLocationPtr() {
  return this->_llvm_location_ptr;
}

[[nodiscard]] inline bool
LocalDynamicVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::LOCAL_DYNAMIC_VARIABLE);
}

RQ_ALWAYS_INLINE LocalStaticVariable::LocalStaticVariable(
    rq::Name name, rq::SymbolTable &container, rq::SymbolTable &host,
    rq::Module &module, rq::LowFuseFlags flags, rq::ConstantSymbol &type,
    rq::Gendex<rq::StaticValue> value)
    : LocalVariable(rq::SymbolKind::LOCAL_STATIC_VARIABLE, name, container,
                    host, module, flags, type),
      _value(value) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Gendex<rq::StaticValue> &
LocalStaticVariable::getValue() const {
  return this->_value;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Gendex<rq::StaticValue> &
LocalStaticVariable::getValue() {
  return this->_value;
}

[[nodiscard]] inline bool
LocalStaticVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::LOCAL_STATIC_VARIABLE);
}

RQ_ALWAYS_INLINE Capture::Capture(rq::SymbolTable &container,
                                  rq::SymbolTable &host, rq::Module &module,
                                  rq::ConstantSymbol &type, rq::Constant &value)
    : LocalVariable(rq::SymbolKind::CAPTURE, rq::Name(rq::Keyword::CAPTURE),
                    container, host, module, rq::LowFuseFlags::STATIC, type),
      _value_ptr(&value) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Constant &Capture::getValue() const {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Constant &Capture::getValue() {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] inline bool Capture::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::CAPTURE);
}

RQ_ALWAYS_INLINE TemplateArgument::TemplateArgument(
    rq::Name name, rq::SymbolTable &container, rq::SymbolTable &host,
    rq::Module &module, rq::LowFuseFlags flags, rq::ConstantSymbol &type,
    rq::Entity &value, rq::LayoutParameter &parameter)
    : LocalVariable(rq::SymbolKind::TEMPLATE_ARGUMENT, name, container, host,
                    module, flags, type),
      _value_ptr(&value), _parameter_ptr(&parameter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
TemplateArgument::getValue() const {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &TemplateArgument::getValue() {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter &
TemplateArgument::getLayoutParameter() const {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter &
TemplateArgument::getLayoutParameter() {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] inline bool
TemplateArgument::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::TEMPLATE_ARGUMENT);
}

RQ_ALWAYS_INLINE
FunctionArgument::FunctionArgument(rq::Name name, rq::SymbolTable &host,
                                   rq::Module &module,
                                   rq::SignatureParameter &parameter)
    : LocalVariable(rq::SymbolKind::TEMPLATE_ARGUMENT, name, host, host, module,
                    parameter.getLowFuseFlags(), parameter.getType()),
      _parameter_ptr(&parameter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter &
FunctionArgument::getSignatureParameter() const {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter &
FunctionArgument::getSignatureParameter() {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] inline bool
FunctionArgument::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::FUNCTION_ARGUMENT);
}

RQ_ALWAYS_INLINE
Parameter::Parameter(rq::SymbolKind kind, rq::Parameter *next_ptr,
                     rq::Name name, rq::ConstantSymbol &type)
    : Symbol(kind), _next_ptr(next_ptr), _name(name), _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name Parameter::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Parameter::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Parameter::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *
Parameter::getNextParameterPtr() const {
  return this->_next_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *Parameter::getNextParameterPtr() {
  return this->_next_ptr;
}

[[nodiscard]] inline bool Parameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsParameter(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE
SymbolParameter::SymbolParameter(
    rq::SymbolKind kind, rq::SymbolParameter *next_ptr, rq::Name name,
    rq::ConstantSymbol &type, rq::SymbolTable &host,
    rq::LowFuseFlags expression_flags, bool is_positional,
    bool is_nonpositional, bool is_locked, rq::Expression &expression,
    rq::Expression &name_expression, rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : Parameter(kind, next_ptr, name, type), _is_positional(is_positional),
      _is_nonpositional(is_nonpositional), _is_locked(is_locked),
      _low_flags(expression_flags), _host_ptr(&host),
      _expression_ptr(&expression), _name_expression_ptr(&name_expression),
      _type_expression_ptr(&type_expression),
      _default_value_expression_ptr(default_value_expression_ptr),
      _module_ptr(&module) {
  RQ_ASSERT(rq::getIsSymbolParameter(kind), "not symbol parameter");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags
SymbolParameter::getLowFuseFlags() const {
  return this->_low_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
SymbolParameter::getNextSymbolParameterPtr() const {
  return llvm::cast<rq::SymbolParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
SymbolParameter::getNextSymbolParameterPtr() {
  return llvm::cast<rq::SymbolParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool SymbolParameter::getIsPositional() const {
  return this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameter::getIsNonpositional() const {
  return this->_is_nonpositional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool SymbolParameter::getIsLocked() const {
  return this->_is_locked;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
SymbolParameter::getHost() const {
  return rq::dereferencePtr(this->_host_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &SymbolParameter::getHost() {
  return rq::dereferencePtr(this->_host_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameter::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
SymbolParameter::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameter::getNameExpression() const {
  return rq::dereferencePtr(this->_name_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
SymbolParameter::getNameExpression() {
  return rq::dereferencePtr(this->_name_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameter::getTypeExpression() const {
  return rq::dereferencePtr(this->_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
SymbolParameter::getTypeExpression() {
  return rq::dereferencePtr(this->_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
SymbolParameter::getDefaultValueExpressionPtr() const {
  return this->_default_value_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
SymbolParameter::getDefaultValueExpressionPtr() {
  return this->_default_value_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
SymbolParameter::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &SymbolParameter::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] inline bool
SymbolParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsSymbolParameter(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE SignatureParameter::SignatureParameter(
    rq::SymbolParameter *next_ptr, rq::Name name, rq::ConstantSymbol &type,
    rq::SymbolTable &host, rq::LowFuseFlags expression_flags,
    bool is_positional, bool is_nonpositional, bool is_locked,
    rq::Expression &expression, rq::Expression &name_expression,
    rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : SymbolParameter(rq::SymbolKind::SIGNATURE_PARAMETER, next_ptr, name, type,
                      host, expression_flags, is_positional, is_nonpositional,
                      is_locked, expression, name_expression, type_expression,
                      default_value_expression_ptr, module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
SignatureParameter::getNextSignatureParameterPtr() const {
  return llvm::cast<rq::SignatureParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
SignatureParameter::getNextSignatureParameterPtr() {
  return llvm::cast<rq::SignatureParameter>(this->getNextParameterPtr());
}

[[nodiscard]] inline bool
SignatureParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::SIGNATURE_PARAMETER);
}

RQ_ALWAYS_INLINE LayoutParameter::LayoutParameter(
    rq::SymbolParameter *next_ptr, rq::Name name, rq::ConstantSymbol &type,
    rq::SymbolTable &host, rq::LowFuseFlags expression_flags,
    bool is_positional, bool is_nonpositional, bool is_locked,
    rq::Expression &expression, rq::Expression &name_expression,
    rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : SymbolParameter(rq::SymbolKind::LAYOUT_PARAMETER, next_ptr, name, type,
                      host, expression_flags, is_positional, is_nonpositional,
                      is_locked, expression, name_expression, type_expression,
                      default_value_expression_ptr, module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
LayoutParameter::getNextLayoutParameterPtr() const {
  return llvm::cast<rq::LayoutParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
LayoutParameter::getNextLayoutParameterPtr() {
  return llvm::cast<rq::LayoutParameter>(this->getNextParameterPtr());
}

[[nodiscard]] inline bool
LayoutParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::LAYOUT_PARAMETER);
}

RQ_ALWAYS_INLINE
TypeParameter::TypeParameter(rq::SymbolKind kind, rq::TypeParameter *next_ptr,
                             rq::Name name, rq::ConstantSymbol &type,
                             unsigned location, bool is_positional)
    : Parameter(kind, next_ptr, name, type), _location(location),
      _is_positional(is_positional) {
  RQ_ASSERT(rq::getIsTypeParameter(kind), "not type parameter");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
TypeParameter::getNextTypeParameterPtr() const {
  return llvm::cast<rq::TypeParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *
TypeParameter::getNextTypeParameterPtr() {
  return llvm::cast<rq::TypeParameter>(this->getNextParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned TypeParameter::getLocation() const {
  return this->_location;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TypeParameter::getIsPositional() const {
  return this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TypeParameter::getIsNonpositional() const {
  return !this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeParameter::getIsPositionPassable() const {
  return this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TypeParameter::getIsTypePassable() const {
  return !this->_is_positional && this->getName().getIsEmpty();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TypeParameter::getIsNamePassable() const {
  return !this->_is_positional && !this->getName().getIsEmpty();
}

[[nodiscard]] inline bool TypeParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsTypeParameter(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

inline void TypeParameter::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileTypeParameter(inout_id, this->getKind(),
                           this->getNextTypeParameterPtr(), this->getName(),
                           this->getType(), this->getLocation(),
                           this->getIsPositional());
}

RQ_ALWAYS_INLINE void
profileTypeParameter(llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
                     const rq::TypeParameter *next_ptr, rq::Name name,
                     const rq::ConstantSymbol &type, unsigned location,
                     bool is_positional) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(next_ptr);
  inout_id.Add(name);
  inout_id.AddPointer(&type);
  inout_id.AddInteger(location);
  inout_id.AddBoolean(is_positional);
}

RQ_ALWAYS_INLINE
ProcedureParameter::ProcedureParameter(rq::TypeParameter *next_ptr,
                                       rq::Name name, rq::ConstantSymbol &type,
                                       unsigned location)
    : TypeParameter(rq::SymbolKind::PROCEDURE_PARAMETER, next_ptr, name, type,
                    location, name.getIsEmpty()) {}

[[nodiscard]] inline bool
ProcedureParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::PROCEDURE_PARAMETER);
}

RQ_ALWAYS_INLINE
TupleParameter::TupleParameter(rq::TypeParameter *next_ptr, rq::Name name,
                               rq::ConstantSymbol &type, unsigned location,
                               bool is_positional)
    : TypeParameter(rq::SymbolKind::TUPLE_PARAMETER, next_ptr, name, type,
                    location, is_positional) {}

[[nodiscard]] inline bool
TupleParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::TUPLE_PARAMETER);
}

RQ_ALWAYS_INLINE
ParameterList::ParameterList(rq::SymbolKind kind,
                             rq::Parameter *first_parameter_ptr,
                             unsigned parameter_count,
                             unsigned positional_parameter_count,
                             unsigned nonpositional_parameter_count)
    : Symbol(kind), _first_parameter_ptr(first_parameter_ptr),
      _parameter_count(parameter_count),
      _positional_parameter_count(positional_parameter_count),
      _nonpositional_parameter_count(nonpositional_parameter_count) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *
ParameterList::getFirstParameterPtr() const {
  return this->_first_parameter_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *
ParameterList::getFirstParameterPtr() {
  return this->_first_parameter_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
ParameterList::getParameterCount() const {
  return this->_parameter_count;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
ParameterList::getPositionalParameterCount() const {
  return this->_positional_parameter_count;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
ParameterList::getNonpositionalParameterCount() const {
  return this->_nonpositional_parameter_count;
}

[[nodiscard]] inline const rq::Parameter *
ParameterList::getParameterPtrOfName(rq::Name name) const {
  for (const rq::Parameter &parameter : this->getParameterSubrange()) {
    if (parameter.getName() == name) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] inline rq::Parameter *
ParameterList::getParameterPtrOfName(rq::Name name) {
  for (rq::Parameter &parameter : this->getParameterSubrange()) {
    if (parameter.getName() == name) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Parameter>,
                          rq::NextIterator<rq::Parameter>,
                          std::ranges::subrange_kind::unsized>
    ParameterList::getParameterSubrange() {
  return std::ranges::subrange<rq::NextIterator<rq::Parameter>,
                               rq::NextIterator<rq::Parameter>,
                               std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter>(this->_first_parameter_ptr),
      rq::NextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstNextIterator<rq::Parameter>,
                          rq::ConstNextIterator<rq::Parameter>,
                          std::ranges::subrange_kind::unsized>
    ParameterList::getParameterSubrange() const {
  return std::ranges::subrange<rq::ConstNextIterator<rq::Parameter>,
                               rq::ConstNextIterator<rq::Parameter>,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_parameter_ptr),
      rq::ConstNextIterator<rq::Parameter>());
}

[[nodiscard]] inline bool ParameterList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsParameterList(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE SymbolParameterList::SymbolParameterList(
    rq::SymbolKind kind, rq::SymbolParameter *first_parameter_ptr,
    unsigned parameter_count, unsigned positional_parameter_count,
    unsigned nonpositional_parameter_count, rq::Expression &expression,
    unsigned locked_parameter_count, rq::Module &module)
    : ParameterList(kind, first_parameter_ptr, parameter_count,
                    positional_parameter_count, nonpositional_parameter_count),
      _expression_ptr(&expression),
      _locked_parameter_count(locked_parameter_count), _module_ptr(&module) {
  RQ_ASSERT(rq::getIsSymbolParameterList(kind), "not symbol parameter list");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameterList::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
SymbolParameterList::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
SymbolParameterList::getLockedParameterCount() const {
  return this->_locked_parameter_count;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
SymbolParameterList::getFirstSymbolParameterPtr() const {
  return llvm::cast<rq::SymbolParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
SymbolParameterList::getFirstSymbolParameterPtr() {
  return llvm::cast<rq::SymbolParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
SymbolParameterList::getSymbolParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::SymbolParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
SymbolParameterList::getSymbolParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::SymbolParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
SymbolParameterList::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &SymbolParameterList::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
                          rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
                          std::ranges::subrange_kind::unsized>
    SymbolParameterList::getSymbolParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>(
          llvm::cast<rq::SymbolParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
    std::ranges::subrange_kind::unsized>
SymbolParameterList::getSymbolParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>(
          llvm::cast<rq::SymbolParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>());
}

[[nodiscard]] inline bool
SymbolParameterList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsSymbolParameterList(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE Signature::Signature(
    rq::SignatureParameter *first_parameter_ptr, unsigned parameter_count,
    unsigned positional_parameter_count, unsigned nonpositional_parameter_count,
    rq::Expression &expression, unsigned locked_parameter_count,
    rq::Module &module, rq::ConstantSymbol &return_type,
    rq::ConstantSymbol *reciever_type_ptr)
    : SymbolParameterList(rq::SymbolKind::SIGNATURE, first_parameter_ptr,
                          parameter_count, positional_parameter_count,
                          nonpositional_parameter_count, expression,
                          locked_parameter_count, module),
      _return_type_ptr(&return_type), _reciever_type_ptr(reciever_type_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Signature::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Signature::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
Signature::getRecieverTypePtr() const {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *
Signature::getRecieverTypePtr() {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
Signature::getFirstSignatureParameterPtr() const {
  return llvm::cast<rq::SignatureParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
Signature::getFirstSignatureParameterPtr() {
  return llvm::cast<rq::SignatureParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
Signature::getSignatureParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::SignatureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
Signature::getSignatureParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::SignatureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
    rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
    std::ranges::subrange_kind::unsized>
Signature::getSignatureParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>(
          llvm::cast<rq::SignatureParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
    std::ranges::subrange_kind::unsized>
Signature::getSignatureParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>(
          llvm::cast<rq::SignatureParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>());
}

[[nodiscard]] inline bool Signature::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::SIGNATURE);
}

RQ_ALWAYS_INLINE
Layout::Layout(rq::SymbolParameter *first_parameter_ptr,
               unsigned parameter_count, unsigned positional_parameter_count,
               unsigned nonpositional_parameter_count,
               rq::Expression &expression, unsigned locked_parameter_count,
               rq::Module &module)
    : SymbolParameterList(rq::SymbolKind::LAYOUT, first_parameter_ptr,
                          parameter_count, positional_parameter_count,
                          nonpositional_parameter_count, expression,
                          locked_parameter_count, module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
Layout::getFirstLayoutParameterPtr() const {
  return llvm::cast<rq::LayoutParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
Layout::getFirstLayoutParameterPtr() {
  return llvm::cast<rq::LayoutParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
Layout::getLayoutParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::LayoutParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
Layout::getLayoutParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::LayoutParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
                          rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
                          std::ranges::subrange_kind::unsized>
    Layout::getLayoutParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>(
          llvm::cast<rq::LayoutParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
    std::ranges::subrange_kind::unsized>
Layout::getLayoutParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>(
          llvm::cast<rq::LayoutParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>());
}

[[nodiscard]] inline bool Layout::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::LAYOUT);
}

RQ_ALWAYS_INLINE
TypeParameterList::TypeParameterList(rq::SymbolKind kind,
                                     rq::TypeParameter *first_parameter_ptr,
                                     unsigned parameter_count,
                                     unsigned positional_parameter_count,
                                     unsigned nonpositional_parameter_count)
    : ParameterList(kind, first_parameter_ptr, parameter_count,
                    positional_parameter_count, nonpositional_parameter_count) {
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
TypeParameterList::getFirstTypeParameterPtr() const {
  return llvm::cast<rq::TypeParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *
TypeParameterList::getFirstTypeParameterPtr() {
  return llvm::cast<rq::TypeParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
TypeParameterList::getTypeParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::TypeParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *
TypeParameterList::getTypeParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::TypeParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::TypeParameter>,
                          rq::NextIterator<rq::Parameter, rq::TypeParameter>,
                          std::ranges::subrange_kind::unsized>
    TypeParameterList::getTypeParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::TypeParameter>,
      rq::NextIterator<rq::Parameter, rq::TypeParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::TypeParameter>(
          llvm::cast<rq::TypeParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::TypeParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
    std::ranges::subrange_kind::unsized>
TypeParameterList::getTypeParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>(
          llvm::cast<rq::TypeParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>());
}

[[nodiscard]] inline bool
TypeParameterList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsTypeParameterList(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE
ProcedureType::ProcedureType(rq::ProcedureParameter *first_parameter_ptr,
                             unsigned parameter_count,
                             unsigned positional_parameter_count,
                             unsigned nonpositional_parameter_count,
                             rq::ConstantSymbol &return_type,
                             rq::ConstantSymbol *reciever_type_ptr)
    : TypeParameterList(rq::SymbolKind::PROCEDURE_TYPE, first_parameter_ptr,
                        parameter_count, positional_parameter_count,
                        nonpositional_parameter_count),
      _return_type_ptr(&return_type), _reciever_type_ptr(reciever_type_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
ProcedureType::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &
ProcedureType::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
ProcedureType::getRecieverTypePtr() const {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *
ProcedureType::getRecieverTypePtr() {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter *
ProcedureType::getFirstProcedureParameterPtr() const {
  return llvm::cast<rq::ProcedureParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter *
ProcedureType::getFirstProcedureParameterPtr() {
  return llvm::cast<rq::ProcedureParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter *
ProcedureType::getProcedureParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::ProcedureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter *
ProcedureType::getProcedureParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::ProcedureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
    rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
    std::ranges::subrange_kind::unsized>
ProcedureType::getProcedureParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>(
          llvm::cast<rq::ProcedureParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
    std::ranges::subrange_kind::unsized>
ProcedureType::getProcedureParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>(
          llvm::cast<rq::ProcedureParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>());
}

[[nodiscard]] inline bool ProcedureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::PROCEDURE_TYPE);
}

inline void ProcedureType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileProcedureType(inout_id, this->getFirstProcedureParameterPtr(),
                           this->getReturnType(), this->getRecieverTypePtr());
}

inline void
profileProcedureType(llvm::FoldingSetNodeID &inout_id,
                     const rq::ProcedureParameter *first_parameter_ptr,
                     const rq::ConstantSymbol &return_type,
                     const rq::ConstantSymbol *reciever_type_ptr) {
  inout_id.AddPointer(first_parameter_ptr);
  inout_id.AddPointer(&return_type);
  inout_id.AddPointer(reciever_type_ptr);
}

RQ_ALWAYS_INLINE TupleType::TupleType(rq::TupleParameter *first_parameter_ptr,
                                      unsigned parameter_count,
                                      unsigned positional_parameter_count,
                                      unsigned nonpositional_parameter_count,
                                      unsigned type_keyed_parameter_count)
    : TypeParameterList(rq::SymbolKind::TUPLE_TYPE, first_parameter_ptr,
                        parameter_count, positional_parameter_count,
                        nonpositional_parameter_count),
      _type_keyed_parameter_count(type_keyed_parameter_count) {}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
TupleType::getNameKeyedParameterCount() const {
  return this->getNonpositionalParameterCount() -
         this->getTypeKeyedParameterCount();
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned
TupleType::getTypeKeyedParameterCount() const {
  return this->_type_keyed_parameter_count;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TupleParameter *
TupleType::getFirstTupleParameterPtr() const {
  return llvm::cast<rq::TupleParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter *
TupleType::getFirstTupleParameterPtr() {
  return llvm::cast<rq::TupleParameter>(this->getFirstParameterPtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TupleParameter *
TupleType::getTupleParameterPtrOfName(rq::Name name) const {
  return llvm::cast<rq::TupleParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter *
TupleType::getTupleParameterPtrOfName(rq::Name name) {
  return llvm::cast<rq::TupleParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] inline const rq::TupleParameter *
TupleType::getTupleParameterPtrOfType(const rq::ConstantSymbol &type) const {
  for (const rq::TupleParameter &parameter :
       this->getTupleParameterSubrange()) {
    if (!parameter.getName().getIsEmpty()) {
      continue;
    }
    if (parameter.getType() == type) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] inline rq::TupleParameter *
TupleType::getTupleParameterPtrOfType(const rq::ConstantSymbol &type) {
  for (rq::TupleParameter &parameter : this->getTupleParameterSubrange()) {
    if (!parameter.getName().getIsEmpty()) {
      continue;
    }
    if (parameter.getType() == type) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::TupleParameter>,
                          rq::NextIterator<rq::Parameter, rq::TupleParameter>,
                          std::ranges::subrange_kind::unsized>
    TupleType::getTupleParameterSubrange() {
  return std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::TupleParameter>,
      rq::NextIterator<rq::Parameter, rq::TupleParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Parameter, rq::TupleParameter>(
          llvm::cast<rq::TupleParameter>(this->_first_parameter_ptr)),
      rq::NextIterator<rq::Parameter, rq::TupleParameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
    rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
    rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
    std::ranges::subrange_kind::unsized>
TupleType::getTupleParameterSubrange() const {
  return std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
      std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>(
          llvm::cast<rq::TupleParameter>(this->_first_parameter_ptr)),
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>());
}

[[nodiscard]] inline bool TupleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::TUPLE_TYPE);
}

inline void TupleType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileTupleType(
      inout_id, llvm::cast<rq::TupleParameter>(this->_first_parameter_ptr));
}

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &inout_id,
                 const rq::TupleParameter *first_parameter_ptr) {
  inout_id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE PlacementType::PlacementType(rq::Function &function)
    : Symbol(rq::SymbolKind::PLACEMENT_TYPE), _function_ptr(&function) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Function &
PlacementType::getFunction() const {
  return rq::dereferencePtr(this->_function_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Function &PlacementType::getFunction() {
  return rq::dereferencePtr(this->_function_ptr);
}

[[nodiscard]] inline bool PlacementType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::PLACEMENT_TYPE);
}

inline void PlacementType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profilePlacement(inout_id, this->getFunction());
}

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &inout_id,
                                       const rq::Function &function) {
  inout_id.AddPointer(&function);
}

RQ_ALWAYS_INLINE
CompositionComponent::CompositionComponent(rq::CompositionComponent *next_ptr)
    : Symbol(rq::SymbolKind::COMPOSITION_COMPONENT), _next_ptr(next_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent *
CompositionComponent::getNextComponentPtr() const {
  return this->_next_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent *
CompositionComponent::getNextComponentPtr() {
  return this->_next_ptr;
}

inline void
CompositionComponent::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileCompositionComponent(inout_id, this->_next_ptr);
}

RQ_ALWAYS_INLINE void profileCompositionComponent(
    llvm::FoldingSetNodeID &inout_id,
    const rq::CompositionComponent *next_component_ptr) {
  inout_id.AddPointer(next_component_ptr);
}

RQ_ALWAYS_INLINE
CompositionType::CompositionType(rq::CompositionComponent &first_component)
    : Symbol(rq::SymbolKind::COMPOSITION_TYPE),
      _first_component_ptr(&first_component) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent &
CompositionType::getFirstComponent() const {
  return rq::dereferencePtr(this->_first_component_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent &
CompositionType::getFirstComponent() {
  return rq::dereferencePtr(this->_first_component_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::CompositionComponent>,
                          rq::NextIterator<rq::CompositionComponent>,
                          std::ranges::subrange_kind::unsized>
    CompositionType::getComponentSubrange() {
  return std::ranges::subrange<rq::NextIterator<rq::CompositionComponent>,
                               rq::NextIterator<rq::CompositionComponent>,
                               std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::CompositionComponent>(this->_first_component_ptr),
      rq::NextIterator<rq::CompositionComponent>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstNextIterator<rq::CompositionComponent>,
                          rq::ConstNextIterator<rq::CompositionComponent>,
                          std::ranges::subrange_kind::unsized>
    CompositionType::getComponentSubrange() const {
  return std::ranges::subrange<rq::ConstNextIterator<rq::CompositionComponent>,
                               rq::ConstNextIterator<rq::CompositionComponent>,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::CompositionComponent>(
          this->_first_component_ptr),
      rq::ConstNextIterator<rq::CompositionComponent>());
}

[[nodiscard]] inline bool
CompositionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlyingValue(rq::SymbolKind::COMPOSITION_TYPE);
}

inline void CompositionType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileCompositionType(inout_id, this->getFirstComponent());
}

inline void
profileCompositionType(llvm::FoldingSetNodeID &inout_id,
                       const rq::CompositionComponent &first_component) {
  inout_id.AddPointer(&first_component);
}

RQ_ALWAYS_INLINE SynonymType::SynonymType(rq::Symbol &original)
    : Symbol(rq::SymbolKind::SYNONYM_TYPE), _original_ptr(&original) {
  RQ_ASSERT(!llvm::isa<rq::ScaledPrimitiveType>(original),
            "scaled primitive type synonym is uniqued by uid");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
SynonymType::getOriginal() const {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &SynonymType::getOriginal() {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] inline bool SynonymType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::SYNONYM_TYPE);
}

RQ_ALWAYS_INLINE SymbolTable::SymbolTable(rq::SymbolKind kind,
                                          rq::SymbolTable *container_ptr)
    : Symbol(kind), _container_ptr(container_ptr) {
  using S = rq::SymbolKind;
  RQ_ASSERT(rq::getIsSymbolTable(kind), "not symbol table");
  if (kind == S::FUNCTION) {
    this->_function_container_ptr = llvm::cast<rq::Function>(this);
  } else if (container_ptr != nullptr) {
    rq::SymbolTable &container = rq::dereferencePtr(container_ptr);
    this->_function_container_ptr = container.getFunctionContainerPtr();
  }
  if (this->getIsObjectScope()) {
    this->_object_container_ptr = llvm::cast<rq::GlobalDeclaration>(this);
  } else if (container_ptr != nullptr) {
    rq::SymbolTable &container = rq::dereferencePtr(container_ptr);
    this->_object_container_ptr = container.getObjectContainerPtr();
  }
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
SymbolTable::getContainerPtr() const {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *SymbolTable::getContainerPtr() {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Function *
SymbolTable::getFunctionContainerPtr() const {
  return this->_function_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Function *
SymbolTable::getFunctionContainerPtr() {
  return this->_function_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDeclaration *
SymbolTable::getObjectContainerPtr() const {
  return this->_object_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDeclaration *
SymbolTable::getObjectContainerPtr() {
  return this->_object_container_ptr;
}

inline void SymbolTable::addMember(rq::BumpPtrAllocator &allocator,
                                   rq::Name name, rq::Symbol &symbol) {
  llvm::DenseMapIterator<rq::Name, rq::BumpPtrList<rq::Symbol>> it =
      this->_member_map.find(name);
  if (it == this->_member_map.end()) {
    if (!llvm::isa<rq::Instance>(symbol)) {
      this->_member_map.emplace_or_assign(name, symbol);
      return;
    }
    rq::Instance &instance = llvm::cast<rq::Instance>(symbol);
    rq::SymbolKind poly_kind = instance.getPolymorphKind();
    rq::Polymorph &poly =
        allocator.allocateValue<rq::Polymorph>(poly_kind, name, *this);
    poly.addInstance(instance);
    this->_member_map.emplace_or_assign(name, poly);
    return;
  }
  rq::BumpPtrList<rq::Symbol> &list = it->getSecond();
  if (!llvm::isa<rq::Instance>(symbol)) {
    list.insertFront(allocator, symbol);
    return;
  }
  rq::Instance &instance = llvm::cast<rq::Instance>(symbol);
  rq::SymbolKind poly_kind = instance.getPolymorphKind();
  for (rq::Symbol &symbol : list) {
    if (!llvm::isa<rq::Polymorph>(symbol)) {
      continue;
    }
    rq::Polymorph &poly = llvm::cast<rq::Polymorph>(symbol);
    if (poly.getKind() == poly_kind) {
      poly.addInstance(instance);
      return;
    }
  }
  rq::Polymorph &poly =
      allocator.allocateValue<rq::Polymorph>(poly_kind, name, *this);
  list.insertFront(allocator, poly);
  poly.addInstance(instance);
}

[[nodiscard]] RQ_ALWAYS_INLINE const
    llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>> &
    SymbolTable::getMap() const {
  return this->_member_map;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstBumpPtrListRef<rq::Symbol>
SymbolTable::lookupList(rq::Name name) const {
  for (const rq::SymbolTable &table : this->getInclusiveAscendingSubrange()) {
    const auto &map = table._member_map;
    auto it = map.find(name);
    if (it == map.end()) {
      continue;
    }
    return rq::ConstBumpPtrListRef<rq::Symbol>(it->getSecond());
  }
  return rq::ConstBumpPtrListRef<rq::Symbol>();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
SymbolTable::lookupList(rq::Name name) {
  for (rq::SymbolTable &table : this->getInclusiveAscendingSubrange()) {
    auto &map = table._member_map;
    auto it = map.find(name);
    if (it == map.end()) {
      continue;
    }
    return rq::BumpPtrListRef<rq::Symbol>(it->getSecond());
  }
  return rq::BumpPtrListRef<rq::Symbol>();
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::SymbolTableIterator, rq::SymbolTableIterator,
                          std::ranges::subrange_kind::unsized>
    SymbolTable::getInclusiveAscendingSubrange() {
  return std::ranges::subrange<rq::SymbolTableIterator, rq::SymbolTableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::SymbolTableIterator(this), rq::SymbolTableIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstSymbolTableIterator,
                          rq::ConstSymbolTableIterator,
                          std::ranges::subrange_kind::unsized>
    SymbolTable::getInclusiveAscendingSubrange() const {
  return std::ranges::subrange<rq::ConstSymbolTableIterator,
                               rq::ConstSymbolTableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstSymbolTableIterator(this), rq::ConstSymbolTableIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE auto SymbolTable::getSymbolListSubrange() const {
  return std::ranges::subrange<
      llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>>::const_iterator,
      llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>>::const_iterator,
      std::ranges::subrange_kind::unsized>(this->_member_map.begin(),
                                           this->_member_map.end());
}

[[nodiscard]] RQ_ALWAYS_INLINE auto SymbolTable::getSymbolListSubrange() {
  return std::ranges::subrange<
      llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>>::iterator,
      llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>>::iterator,
      std::ranges::subrange_kind::unsized>(this->_member_map.begin(),
                                           this->_member_map.end());
}

[[nodiscard]] inline bool SymbolTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsSymbolTable(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE C::C() : SymbolTable(rq::SymbolKind::C, nullptr) {}

[[nodiscard]] inline bool C::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::C);
}

RQ_ALWAYS_INLINE Top::Top() : SymbolTable(rq::SymbolKind::TOP, nullptr) {}

[[nodiscard]] inline bool Top::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::TOP);
}

RQ_ALWAYS_INLINE
LocalScope::LocalScope(rq::SymbolKind kind, rq::SymbolTable &container,
                       rq::Expression &expression, rq::LowFuseFlags flags,
                       rq::Module &module)
    : SymbolTable(kind, &container), _expression_ptr(&expression),
      _low_flags(flags), _module_ptr(&module) {
  RQ_ASSERT(rq::getIsLocalScope(kind), "not local statement");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
LocalScope::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &LocalScope::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] rq::LowFuseFlags LocalScope::getLowFuseFlags() const {
  return this->_low_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &LocalScope::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &LocalScope::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] inline bool LocalScope::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsLocalScope(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedLocalScope<KIND_PARAM>::DerivedLocalScope(
    rq::SymbolTable &container, rq::Expression &expression,
    rq::LowFuseFlags flags, rq::Module &module)
    : LocalScope(KIND_PARAM, container, expression, flags, module) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedLocalScope<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(KIND_PARAM);
}

RQ_ALWAYS_INLINE NamedTable::NamedTable(rq::SymbolKind kind,
                                        rq::SymbolTable &container,
                                        rq::Name name)
    : SymbolTable(kind, &container), _name(name) {
  RQ_ASSERT(rq::getIsNamedTable(kind), "not named table");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name NamedTable::getName() const {
  return this->_name;
}

RQ_ALWAYS_INLINE void NamedTable::setMangledName(llvm::StringRef mangled_name) {
  RQ_ASSERT(this->_mangled_name.empty(), "mangled name already set");
  this->_mangled_name = mangled_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
NamedTable::getMangledName() const {
  return this->_mangled_name;
}

[[nodiscard]] inline bool NamedTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsNamedTable(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE Namespace::Namespace(rq::SymbolTable &container, rq::Name name)
    : NamedTable(rq::SymbolKind::NAMESPACE, container, name) {}

[[nodiscard]] inline bool Namespace::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::NAMESPACE);
}

} // namespace rq