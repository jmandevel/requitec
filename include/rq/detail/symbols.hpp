#pragma once

#include <rq/constants.hpp>

#include <functional>

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    return "NONE";

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
  case S::OUT_VALUE:
    return "OutValue";
  case S::THIS_VALUE:
    return "ThisValue";
  case S::RESULT_VALUE:
    return "ResultValue";
  case S::VALUE_VALUE:
    return "ValueValue";
  case S::INDEX_VALUE:
    return "IndexValue";
  case S::DISCRIMINANT_VALUE:
    return "DiscriminantValue";
  case S::COMMAND_LINE_ARGUMENTS_VALUE:
    return "CommandLineArgumentsValue";
  case S::CALLSITE_VALUE:
    return "CallsiteValue";

  // CONTEXTUAL TYPE
  case S::INFERENCE_TYPE:
    return "InferenceType";
  case S::VOID_TYPE:
    return "VoidType";
  case S::NO_RETURN_TYPE:
    return "NoReturnType";

  // EXPRESSION ATTRIBUTES
  case S::ANCHOR_ATTRIBUTE_TYPE:
    return "AnchorAttributeType";
  case S::OPAQUE_ATTRIBUTE_TYPE:
    return "OpaqueAttributeType";
  case S::GLOBAL_ATTRIBUTE_TYPE:
    return "GlobalAttributeType";
  case S::ACCESS_ATTRIBUTE_TYPE:
    return "AccessAttributeType";
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
  case S::REQUIRE_ATTRIBUTE_TYPE:
    return "RequireAttributeType";
  case S::ENSURE_ATTRIBUTE_TYPE:
    return "EnsureAttributeType";

  // TYPE ATTRIBUTES
  case S::VAR_ATTRIBUTE_TYPE:
    return "VarAttributeType";
  case S::VOLATILE_ATTRIBUTE_TYPE:
    return "VolatileAttributeType";
  case S::ATOMIC_ATTRIBUTE_TYPE:
    return "AtomicAttributeType";
  case S::NULL_TERMINATE_ATTRIBUTE_TYPE:
    return "NullTerminateAttributeType";

  // REFLECTIVE
  case S::SYMBOL_TYPE:
    return "SymbolType";
  case S::EXPRESSION_TYPE:
    return "ExpressionType";

  // PLATFORM PRIMITIVE
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

  // STANDARD PRIMITIVE
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

  // SCALED PRIMITIVES
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return "ScaledIntegerType";
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return "ScaledUnsignedIntegerType";

  // SUBTYPES
  case S::ARRAY_SUBTYPE:
    return "ArraySubtype";

  // UNCOUNTED SUBTYPES => SUBTYPES
  case S::REFERENCE_SUBTYPE:
    return "ReferenceSubtype";
  case S::POINTER_SUBTYPE:
    return "PointerSubtype";
  case S::SLICE_SUBTYPE:
    return "SliceSubtype";
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return "InferenceCountArraySubtype";

  // MODULES
  case S::MODULE:
    return "Module";

  // IMPORTS
  case S::IMPORT:
    return "Import";

  // WEIGHTS
  case S::CLASS_WEIGHT_LEVEL:
    return "ClassWeightLevel";
  case S::ENUMERATION_WEIGHT_LEVEL:
    return "EnumerationWeightLevel";
  case S::INTERFACE_WEIGHT_LEVEL:
    return "InterfaceWeightLevel";
  case S::ADAPTER_WEIGHT_LEVEL:
    return "AdapterWeightLevel";
  case S::GLOBAL_DYNAMIC_VARIABLE_WEIGHT_LEVEL:
    return "GlobalDynamicVariableWeightLevel";
  case S::GLOBAL_STATIC_VARIABLE_WEIGHT_LEVEL:
    return "GlobalStaticVariableWeightLevel";
  case S::FORWARD_RANGER_WEIGHT_LEVEL:
    return "ForwardRangerWeightLevel";
  case S::BACKWARD_RANGER_WEIGHT_LEVEL:
    return "BackwardRangerWeightLevel";
  case S::FUNCTION_WEIGHT_LEVEL:
    return "FunctionWeightLevel";
  case S::METHOD_WEIGHT_LEVEL:
    return "MethodWeightLevel";
  case S::EXTENSION_METHOD_WEIGHT_LEVEL:
    return "ExtensionMethodWeightLevel";

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
  case S::LABEL:
    return "Label";
  case S::ANCHOR:
    return "Anchor";

  // LOCAL VARIABLES => local declaration
  case S::LOCAL_DYNAMIC_VARIABLE:
    return "LocalDynamicVariable";
  case S::LOCAL_STATIC_VARIABLE:
    return "LocalStaticVariable";
  case S::TEMPLATE_ARGUMENT:
    return "TemplateArgument";
  case S::PROCEDURE_ARGUMENT:
    return "ProcedureArgument";
  case S::ENUMERATOR:
    return "Enumerator";

  // SYMBOL PARAMETERS
  case S::SIGNATURE_PARAMETER:
    return "SignatureParameter";
  case S::LAYOUT_PARAMETER:
    return "LayoutParameter";

  // TYPE PARAMETERS
  case S::TUPLE_PARAMETER:
    return "TupleParameter";
  case S::PROCEDURE_PARAMETER:
    return "ProcedureParameter";

  // SYMBOL PARAMETER LISTS
  case S::SIGNATURE:
    return "Signature";
  case S::LAYOUT:
    return "Layout";

  // TYPE PARAMETER LISTS
  case S::TUPLE_TYPE:
    return "TupleType";
  case S::PROCEDURE_TYPE:
    return "ProcedureType";

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

  // GLOBAL DECLARATION
  case S::CLASS_TYPE:
    return "ClassType";
  case S::ENUMERATION_TYPE:
    return "EnumerationType";
  case S::INTERFACE:
    return "Interface";
  case S::ADAPTER:
    return "Adapter";

  // GLOBAL VARIABLE
  case S::GLOBAL_DYNAMIC_VARIABLE:
    return "GlobalDynamicVariable";
  case S::GLOBAL_STATIC_VARIABLE:
    return "GlobalStaticVariable";

  // CALLABLE
  case S::DESTRUCTOR:
    return "Destructor";
  case S::MAIN:
    return "Main";

  // RANGERS
  case S::FORWARD_RANGER:
    return "ForwardRanger";
  case S::BACKWARD_RANGER:
    return "BackwardRanger";

  // PROCEDURES
  case S::FUNCTION:
    return "Function";
  case S::METHOD:
    return "Method";
  case S::EXTENSION_METHOD:
    return "ExtensionMethod";

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return "ClassTemplate";
  case S::ENUMERATION_TEMPLATE:
    return "EnumerationTemplate";
  case S::INTERFACE_TEMPLATE:
    return "InterfaceTemplate";
  case S::ADAPTER_TEMPLATE:
    return "AdapterTemplate";
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
    return "GlobalDynamicVariableTemplate";
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return "GlobalStaticVariableTemplate";
  case S::FORWARD_RANGER_TEMPLATE:
    return "ForwardRangerTemplate";
  case S::BACKWARD_RANGER_TEMPLATE:
    return "BackwardRangerTemplate";
  case S::FUNCTION_TEMPLATE:
    return "FunctionTemplate";
  case S::METHOD_TEMPLATE:
    return "MethodTemplate";
  case S::EXTENSION_METHOD_TEMPLATE:
    return "ExtensionMethodTemplate";

  // POLYMORPHS
  case S::FORWARD_RANGER_POLYMORPH:
    return "ForwardRangerPolymorph";
  case S::BACKWARD_RANGER_POLYMORPH:
    return "BackwardRangerPolymorph";
  case S::FUNCTION_POLYMORPH:
    return "FunctionPolymorph";
  case S::METHOD_POLYMORPH:
    return "MethodPolymorph";
  case S::EXTENSION_METHOD_POLYMORPH:
    return "ExtensionMethodPolymorph";
  case S::CLASS_POLYMORPH:
    return "ClassPolymorph";
  case S::ENUMERATION_POLYMORPH:
    return "EnumerationPolymorph";
  case S::INTERFACE_POLYMORPH:
    return "InterfacePolymorph";
  case S::ADAPTER_POLYMORPH:
    return "AdapterPolymorph";
  case S::GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
    return "GlobalDynamicVariablePolymorph";
  case S::GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return "GlobalStaticVariablePolymorph";
  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::SymbolFlags getFlags(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  using SF = rq::SymbolFlags;
  switch (kind) {
  case S::NONE:
    return SF::NONE;

  // LITERALS
  case S::INTEGER_LITERAL_TYPE:
    return SF::SIMPLE_SYMBOL | SF::LITERAL | SF::IS_TYPE | SF::IS_INTEGER_TYPE;
  case S::FLOAT_LITERAL_TYPE:
    return SF::SIMPLE_SYMBOL | SF::LITERAL | SF::IS_TYPE | SF::IS_FLOAT_TYPE;
  case S::STRING_LITERAL_TYPE:
    return SF::SIMPLE_SYMBOL | SF::LITERAL | SF::IS_TYPE;
  case S::CODEUNIT_LITERAL_TYPE:
    return SF::SIMPLE_SYMBOL | SF::LITERAL | SF::IS_TYPE;

  // CONTEXTUAL VALUE
  case S::OUT_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::THIS_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::RESULT_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::VALUE_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::INDEX_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::DISCRIMINANT_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::COMMAND_LINE_ARGUMENTS_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;
  case S::CALLSITE_VALUE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_VALUE;

  // CONTEXTUAL TYPE
  case S::INFERENCE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_TYPE |
           SF::IS_TYPE;
  case S::VOID_TYPE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_TYPE |
           SF::IS_TYPE;
  case S::NO_RETURN_TYPE:
    return SF::SIMPLE_SYMBOL | SF::CONTEXTUAL | SF::CONTEXTUAL_TYPE |
           SF::IS_TYPE;

  // EXPRESSION ATTRIBUTES
  case S::ANCHOR_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::OPAQUE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::GLOBAL_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::ACCESS_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::PARTIAL_MUTATE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::STATIC_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::CAPTURE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::INLINE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::MANGLE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::PACK_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::BRANCH_TREND_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::SUPPORT_STATUS_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::STABLE_ADDRESS_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::VARIADIC_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::LOCATION_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::TEMPLATE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::CONSTRAINT_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::WEIGHT_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::EXPRESSION_ATTRIBUTE_TYPE | SF::IS_TYPE;

  // TYPE ATTRIBUTES
  case S::VAR_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::VOLATILE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::ATOMIC_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::NULL_TERMINATE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::REQUIRE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;
  case S::ENSURE_ATTRIBUTE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::TYPE_ATTRIBUTE_TYPE | SF::IS_TYPE;

  // REFLECTIVE
  case S::SYMBOL_TYPE:
    return SF::SIMPLE_SYMBOL | SF::REFLECTIVE_TYPE | SF::IS_TYPE;
  case S::EXPRESSION_TYPE:
    return SF::SIMPLE_SYMBOL | SF::REFLECTIVE_TYPE | SF::IS_TYPE;

  // PLATFORM PRIMITIVE
  case S::BOOLEAN_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE;
  case S::HALF_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE;
  case S::SINGLE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE;
  case S::DOUBLE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE;
  case S::QUADRUPLE_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE;
  case S::SIGNED_INTEGER_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INTEGER_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_UNSIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::SIGNED_INDEX_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INDEX_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_UNSIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::SIGNED_ADDRESS_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::UNSIGNED_ADDRESS_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_UNSIGNED_TYPE | SF::IS_INTEGER_TYPE;
  case S::CHAR_TYPE:
    return SF::SIMPLE_SYMBOL | SF::PLATFORM_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_CODEUNIT_TYPE;

  // STANDARD PRIMITIVE TYPE
  case S::BINARY16_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE | SF::IS_BINARY_TYPE;
  case S::BINARY32_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE | SF::IS_BINARY_TYPE;
  case S::BINARY64_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE | SF::IS_BINARY_TYPE;
  case S::BINARY128_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE | SF::IS_BINARY_TYPE;
  case S::BFLOAT16_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_SIGNED_TYPE | SF::IS_FLOAT_TYPE | SF::IS_BFLOAT_TYPE;
  case S::ASCII_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_CODEUNIT_TYPE;
  case S::UTF8_TYPE:
    return SF::SIMPLE_SYMBOL | SF::STANDARD_PRIMITIVE_TYPE | SF::IS_TYPE |
           SF::IS_CODEUNIT_TYPE;

  // VARIADIC ARGUMENTS
  case S::VARIADIC_ARGUMENTS_TYPE:
    return SF::SIMPLE_SYMBOL | SF::IS_TYPE;

  // SCALED PRIMITIVES
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return SF::SCALED_PRIMITIVE_TYPE | SF::IS_TYPE | SF::IS_SIGNED_TYPE |
           SF::IS_INTEGER_TYPE;
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return SF::SCALED_PRIMITIVE_TYPE | SF::IS_TYPE | SF::IS_UNSIGNED_TYPE |
           SF::IS_INTEGER_TYPE;

  // ARRAY
  case S::ARRAY_SUBTYPE:
    return SF::SUBTYPE | SF::IS_TYPE;

  // UNCOUNTED SUBTYPES
  case S::REFERENCE_SUBTYPE:
    return SF::SUBTYPE | SF::UNCOUNTED_SUBTYPE | SF::IS_TYPE;
  case S::POINTER_SUBTYPE:
    return SF::SUBTYPE | SF::UNCOUNTED_SUBTYPE | SF::IS_TYPE;
  case S::SLICE_SUBTYPE:
    return SF::SUBTYPE | SF::UNCOUNTED_SUBTYPE | SF::IS_TYPE;
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return SF::SUBTYPE | SF::UNCOUNTED_SUBTYPE | SF::IS_TYPE;

  // MODULES
  case S::MODULE:
    return SF::NONE;

  // IMPORTS
  case S::IMPORT:
    return SF::HAS_EXPRESSION_ATTRIBUTES;

  // WEIGHTS
  case S::CLASS_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::ENUMERATION_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::INTERFACE_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::ADAPTER_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::GLOBAL_DYNAMIC_VARIABLE_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::GLOBAL_STATIC_VARIABLE_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::FORWARD_RANGER_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::BACKWARD_RANGER_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::FUNCTION_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::METHOD_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;
  case S::EXTENSION_METHOD_WEIGHT_LEVEL:
    return SF::WEIGHT_LEVEL;

  // JUXTAPOSITIONAL LIST
  case S::JUXTAPOSITIONAL_LIST_ITEM:
    return SF::NONE;
  case S::JUXTAPOSITIONAL_LIST_TYPE:
    return SF::IS_TYPE;

  // ARITHMETIC SEQUENCES
  case S::ARITHMETIC_INTERVAL_TYPE:
    return SF::ARITHMETIC_SEQUENCE | SF::IS_TYPE;
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SF::ARITHMETIC_SEQUENCE | SF::IS_TYPE;
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SF::ARITHMETIC_SEQUENCE | SF::IS_TYPE;

  // LOCAL DECLARATIONS
  case S::LABEL:
    return SF::LOCAL_DECLARATION;
  case S::ANCHOR:
    return SF::LOCAL_DECLARATION;
  case S::ENUMERATOR:
    return SF::LOCAL_DECLARATION | SF::HAS_EXPRESSION_ATTRIBUTES;

  // LOCAL VARIABLES
  case S::LOCAL_DYNAMIC_VARIABLE:
    return SF::LOCAL_DECLARATION | SF::LOCAL_VARIABLE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::LOCAL_STATIC_VARIABLE:
    return SF::LOCAL_DECLARATION | SF::LOCAL_VARIABLE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::TEMPLATE_ARGUMENT:
    return SF::LOCAL_DECLARATION | SF::LOCAL_VARIABLE;
  case S::PROCEDURE_ARGUMENT:
    return SF::LOCAL_DECLARATION | SF::LOCAL_VARIABLE;

  // SYMBOL PARAMETERS
  case S::SIGNATURE_PARAMETER:
    return SF::SYMBOL_PARAMETER;
  case S::LAYOUT_PARAMETER:
    return SF::SYMBOL_PARAMETER;

  // TYPE PARAMETERS
  case S::TUPLE_PARAMETER:
    return SF::TYPE_PARAMETER;
  case S::PROCEDURE_PARAMETER:
    return SF::TYPE_PARAMETER;

  // SYMBOL PARAMETER LISTS
  case S::SIGNATURE:
    return SF::SYMBOL_PARAMETER_LIST;
  case S::LAYOUT:
    return SF::SYMBOL_PARAMETER_LIST;

  // TYPE PARAMETER LISTS
  case S::TUPLE_TYPE:
    return SF::TYPE_PARAMETER_LIST | SF::IS_TYPE;
  case S::PROCEDURE_TYPE:
    return SF::TYPE_PARAMETER_LIST | SF::IS_TYPE;

    // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return SF::IS_TYPE;

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return SF::NONE;
  case S::COMPOSITION_TYPE:
    return SF::IS_TYPE;

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return SF::IS_TYPE;

  // SYMBOL TABLES
  case S::TOP:
    return SF::SYMBOL_TABLE | SF::IS_FRAME;

  // LOCAL STATEMENTS
  case S::IF_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::ELSE_IF_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::ELSE_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::MATCH_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::SWITCH_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::CASE_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::WITH_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::DEFAULT_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::FOR_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::WHILE_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::SPIN_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::WEAVE_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::SCOPE_STATEMENT:
    return SF::SYMBOL_TABLE | SF::LOCAL_STATEMENT |
           SF::HAS_EXPRESSION_ATTRIBUTES;

  // NAMED TABLES
  case S::NAMESPACE:
    return SF::NAMED_TABLE | SF::SYMBOL_TABLE | SF::IS_FRAME;

  // GLOBAL DECLARATION
  case S::DESTRUCTOR:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::SYMBOL_TABLE |
           SF::HAS_EXPRESSION_ATTRIBUTES | SF::IS_FRAME;
  case S::MAIN:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::SYMBOL_TABLE |
           SF::PROCEDURE | SF::HAS_EXPRESSION_ATTRIBUTES | SF::IS_FRAME;

  // POLYMORPH ITEM
  case S::CLASS_TYPE:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::IS_TYPE | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;
  case S::ENUMERATION_TYPE:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::IS_TYPE | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;
  case S::INTERFACE:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::HAS_EXPRESSION_ATTRIBUTES | SF::IS_FRAME;
  case S::ADAPTER:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::HAS_EXPRESSION_ATTRIBUTES | SF::IS_FRAME;

  // GLOBAL VARIABLE
  case S::GLOBAL_DYNAMIC_VARIABLE:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::GLOBAL_VARIABLE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::GLOBAL_STATIC_VARIABLE:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::GLOBAL_VARIABLE |
           SF::HAS_EXPRESSION_ATTRIBUTES;

  // RANGERS
  case S::FORWARD_RANGER:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::RANGER | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;
  case S::BACKWARD_RANGER:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::RANGER | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;

  // PROCEDURES
  case S::FUNCTION:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::PROCEDURE | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;
  case S::METHOD:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::PROCEDURE | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;
  case S::EXTENSION_METHOD:
    return SF::INSTANCE | SF::GLOBAL_DECLARATION | SF::NAMED_TABLE |
           SF::SYMBOL_TABLE | SF::PROCEDURE | SF::HAS_EXPRESSION_ATTRIBUTES |
           SF::IS_FRAME;

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::ENUMERATION_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::INTERFACE_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::ADAPTER_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::FORWARD_RANGER_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::BACKWARD_RANGER_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::FUNCTION_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::METHOD_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;
  case S::EXTENSION_METHOD_TEMPLATE:
    return SF::GLOBAL_DECLARATION | SF::NAMED_TABLE | SF::TEMPLATE |
           SF::HAS_EXPRESSION_ATTRIBUTES;

  // POLYMORPHS
  case S::FORWARD_RANGER_POLYMORPH:
    return SF::POLYMORPH;
  case S::BACKWARD_RANGER_POLYMORPH:
    return SF::POLYMORPH;
  case S::FUNCTION_POLYMORPH:
    return SF::POLYMORPH;
  case S::METHOD_POLYMORPH:
    return SF::POLYMORPH;
  case S::EXTENSION_METHOD_POLYMORPH:
    return SF::POLYMORPH;
  case S::CLASS_POLYMORPH:
    return SF::POLYMORPH;
  case S::ENUMERATION_POLYMORPH:
    return SF::POLYMORPH;
  case S::ADAPTER_POLYMORPH:
    return SF::POLYMORPH;
  case S::INTERFACE_POLYMORPH:
    return SF::POLYMORPH;
  case S::GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
    return SF::POLYMORPH;
  case S::GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return SF::POLYMORPH;

  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionFlags
getValidExpressionFlags(rq::SymbolKind kind) {
  RQ_ASSERT(rq::getHasExpressionAttributes(kind), "has no attributes");
  using S = rq::SymbolKind;
  using EF = rq::ExpressionFlags;
  switch (kind) {
  case S::IMPORT:
    return EF::EXPORT;
    return EF::PUBLIC | EF::PARTIAL_MUTATE | EF::LOCATION;
  case S::SIGNATURE_PARAMETER:
    return EF::VARIADIC | EF::LOCATION;
  case S::SIGNATURE:
    return EF::ENSURE | EF::REQUIRE;
  case S::IF_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case S::ELSE_IF_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case S::ELSE_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case S::MATCH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::SWITCH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::CASE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::WITH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::DEFAULT_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::FOR_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::WHILE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::SPIN_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::WEAVE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::SCOPE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case S::CLASS_TYPE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE | EF::PACK |
           EF::DEPRECIATE | EF::EXPERIMENTAL | EF::STABLE_ADDRESS;
  case S::ENUMERATION_TYPE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case S::INTERFACE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case S::ADAPTER:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case S::GLOBAL_DYNAMIC_VARIABLE:
    return EF::OPAQUE | EF::GLOBAL | EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case S::GLOBAL_STATIC_VARIABLE:
    return EF::GLOBAL | EF::EXPORT | EF::STATIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case S::FORWARD_RANGER:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL;
  case S::BACKWARD_RANGER:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL;
  case S::DESTRUCTOR:
    return EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL;
  case S::MAIN:
    return EF::CAPTURE | EF::MANGLE;
  case S::FUNCTION:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::MANGLE;
  case S::METHOD:
    return EF::OPAQUE | EF::PUBLIC | EF::CAPTURE | EF::INLINE | EF::MANGLE;
  case S::EXTENSION_METHOD:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::MANGLE;
  case S::CLASS_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::PACK | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::ENUMERATION_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::INTERFACE_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::ADAPTER_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::FORWARD_RANGER_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::BACKWARD_RANGER_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL |
           EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::FUNCTION_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::METHOD_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case S::EXTENSION_METHOD_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;

  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasNone(flags, rq::SymbolFlags::SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::CONTEXTUAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttributeType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::EXPRESSION_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsTypeAttributeType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TYPE_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::REFLECTIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledPrimitive(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUncountedSubtype(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::UNCOUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::LOCAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::LOCAL_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::SymbolFlags::SYMBOL_PARAMETER |
                                   rq::SymbolFlags::TYPE_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolParameter(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SYMBOL_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameter(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TYPE_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::SymbolFlags::SYMBOL_PARAMETER_LIST |
                                   rq::SymbolFlags::TYPE_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolParameterList(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SYMBOL_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsTypeParameterList(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TYPE_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::LOCAL_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstance(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::INSTANCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDeclaration(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalVariable(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::GLOBAL_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::RANGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_INTEGER_TYPE |
                                  rq::SymbolFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_SIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_UNSIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_INTEGER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_BINARY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_BFLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionAttributes(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_EXPRESSION_ATTRIBUTES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrame(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_FRAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureRelated(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_PROCEDURE_RELATED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsProcedurePolymorph(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_PROCEDURE_RELATED |
                                  rq::SymbolFlags::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsProcedureWeightLevel(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_PROCEDURE_RELATED |
                                  rq::SymbolFlags::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsProcedureInstance(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_PROCEDURE_RELATED |
                                  rq::SymbolFlags::INSTANCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsProcedureTemplate(rq::SymbolKind kind) {
  const rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::IS_PROCEDURE_RELATED |
                                  rq::SymbolFlags::TEMPLATE);
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind)
    : Entity(rq::getUnderlying(kind) + rq::SYMBOL_OFFSET) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::operator==(const Self &rhs) const {
  return this == &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::operator!=(const Self &rhs) const {
  return this != &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind Symbol::getKind() const {
  return static_cast<rq::SymbolKind>(this->getId() - rq::SYMBOL_OFFSET);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolFlags Symbol::getFlags() const {
  return rq::getFlags(this->getKind());
}

[[nodiscard]] inline rq::ExpressionFlags
Symbol::getDerivedExpressionFlags() const {
  if (llvm::isa<rq::Import>(*this)) {
    const rq::Import &import = llvm::cast<rq::Import>(*this);
    return import.getExpressionFlags();
  }
  if (llvm::isa<rq::LocalVariable>(*this)) {
    const rq::LocalVariable &local = llvm::cast<rq::LocalVariable>(*this);
    return local.getExpressionFlags();
  }
  if (llvm::isa<rq::SymbolParameter>(*this)) {
    const rq::SymbolParameter &parameter =
        llvm::cast<rq::SymbolParameter>(*this);
    return parameter.getExpressionFlags();
  }
  if (llvm::isa<rq::LocalStatement>(*this)) {
    const rq::LocalStatement &statement = llvm::cast<rq::LocalStatement>(*this);
    return statement.getExpressionFlags();
  }
  if (llvm::isa<rq::GlobalDeclaration>(*this)) {
    const rq::GlobalDeclaration &decl =
        llvm::cast<rq::GlobalDeclaration>(*this);
    return decl.getExpressionFlags();
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
  if (llvm::isa<rq::LocalStatement>(*this)) {
    const rq::LocalStatement &statement = llvm::cast<rq::LocalStatement>(*this);
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
  if (llvm::isa<rq::LocalStatement>(*this)) {
    rq::LocalStatement &statement = llvm::cast<rq::LocalStatement>(*this);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextual() const {
  return rq::getIsContextual(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualValue() const {
  return rq::getIsContextualValue(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualType() const {
  return rq::getIsContextualType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsExpressionAttributeType() const {
  return rq::getIsExpressionAttributeType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTypeAttributeType() const {
  return rq::getIsTypeAttributeType(this->getKind());
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

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasExpressionAttributes() const {
  return rq::getHasExpressionAttributes(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFrame() const {
  return rq::getIsFrame(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsProcedureRelated() const {
  return rq::getIsProcedureRelated(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsProcedurePolymorph() const {
  return rq::getIsProcedurePolymorph(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsProcedureWeightLevel() const {
  return rq::getIsProcedureWeightLevel(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsProcedureInstance() const {
  return rq::getIsProcedureInstance(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsProcedureTemplate() const {
  return rq::getIsProcedureTemplate(this->getKind());
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
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

inline void ScaledPrimitiveType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileScaledPrimitiveType(out_id, this->getKind(), this->getScaleKind(),
                                 this->getScale(), this->getSynonymTypeId());
}

RQ_ALWAYS_INLINE void profileScaledPrimitiveType(llvm::FoldingSetNodeID &out_id,
                                                 rq::SymbolKind kind,
                                                 rq::ScaleKind scale_kind,
                                                 unsigned scale,
                                                 std::uint64_t synonum_id) {
  out_id.AddInteger(rq::getUnderlying(kind));
  out_id.AddInteger(rq::getUnderlying(scale_kind));
  out_id.AddInteger(scale);
  out_id.AddInteger(synonum_id);
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE Subtype::Subtype(rq::SymbolKind kind,
                                  rq::SymbolConstant &child)
    : Symbol(kind), _child_ptr(&child) {
  RQ_ASSERT(rq::getIsSubtype(kind), "not subtype");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
Subtype::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &Subtype::getChild() {
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

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(rq::SymbolConstant &child,
                                            std::uint64_t count)
    : Subtype(rq::SymbolKind::ARRAY_SUBTYPE, child), _count(count) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t ArraySubtype::getCount() const {
  return this->_count;
}

[[nodiscard]] inline bool ArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::ARRAY_SUBTYPE);
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileArraySubtype(out_id, this->getChild(), this->getCount());
}

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &out_id,
                                          const rq::SymbolConstant &child,
                                          std::uint64_t count) {
  out_id.AddPointer(&child);
  out_id.AddInteger(count);
}

RQ_ALWAYS_INLINE UncountedSubtype::UncountedSubtype(rq::SymbolKind kind,
                                                    rq::SymbolConstant &child)
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

inline void UncountedSubtype::Profile(llvm::FoldingSetNodeID &out_id) const {
  return rq::profileUncountedSubtype(out_id, this->getKind(), this->getChild());
}

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &out_id,
                                              rq::SymbolKind kind,
                                              const rq::SymbolConstant &child) {
  out_id.AddInteger(rq::getUnderlying(kind));
  out_id.AddPointer(&child);
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE
DerivedUncountableSubtype<KIND_PARAM>::DerivedUncountableSubtype(
    rq::SymbolConstant &child)
    : UncountedSubtype(KIND_PARAM, child) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedUncountableSubtype<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
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
ModuleFactory::setOrChangeExpression(rq::FactoryExpression *expression_ptr) {
  this->_expression_ptr = expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::FactoryExpression *
ModuleFactory::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FactoryExpression *
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::MODULE);
}

RQ_ALWAYS_INLINE Import::Import(rq::ExpressionFlags flags,
                                rq::Expression &expression,
                                rq::Module &imported, rq::Module &module)
    : Symbol(rq::SymbolKind::IMPORT), _expression_flags(flags),
      _expression_ptr(&expression), _imported_ptr(&imported),
      _module_ptr(&module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Import::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Import::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
Import::getExpressionFlags() const {
  return this->_expression_flags;
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::IMPORT);
}

RQ_ALWAYS_INLINE
JuxtapositionalListItem::JuxtapositionalListItem(
    rq::SymbolConstant &type, rq::JuxtapositionalListItem *next_ptr)
    : Symbol(rq::SymbolKind::JUXTAPOSITIONAL_LIST_ITEM), _next_ptr(next_ptr),
      _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
JuxtapositionalListItem::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
JuxtapositionalListItem::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] inline bool
JuxtapositionalListItem::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::JUXTAPOSITIONAL_LIST_ITEM);
}

inline void
JuxtapositionalListItem::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileJuxtapositionalListItem(out_id, this->getType(), this->_next_ptr);
}

RQ_ALWAYS_INLINE void
profileJuxtapositionalListItem(llvm::FoldingSetNodeID &out_id,
                               const rq::SymbolConstant &type,
                               const rq::JuxtapositionalListItem *next_ptr) {
  out_id.AddPointer(&type);
  out_id.AddPointer(next_ptr);
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
                   rq::getUnderlying(rq::SymbolKind::JUXTAPOSITIONAL_LIST_TYPE);
}

inline void
JuxtapositionalListType::Profile(llvm::FoldingSetNodeID &out_id) const {
  const rq::JuxtapositionalListItem &first_item =
      rq::dereferencePtr(this->_first_item_ptr);
  rq::profileJuxtapositionalListType(out_id, first_item);
}

inline void
profileJuxtapositionalListType(llvm::FoldingSetNodeID &out_id,
                               const rq::JuxtapositionalListItem &first_item) {
  out_id.AddPointer(&first_item);
}

RQ_ALWAYS_INLINE ArithmeticSequenceType::ArithmeticSequenceType(
    rq::SymbolKind kind, rq::SymbolConstant &child,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step)
    : Symbol(kind), _child_ptr(&child), _condition(condition), _step(step) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
ArithmeticSequenceType::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
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
ArithmeticSequenceType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileArithmeticSequenceType(out_id, this->getKind(), this->getChild(),
                                    this->getCondition(), this->getStep());
}

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &out_id, rq::SymbolKind kind,
    const rq::SymbolConstant &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  out_id.AddInteger(rq::getUnderlying(kind));
  out_id.AddPointer(&child);
  out_id.AddInteger(rq::getUnderlying(condition));
  out_id.AddInteger(rq::getUnderlying(step));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE
DerivedArithmeticSequenceType<KIND_PARAM>::DerivedArithmeticSequenceType(
    rq::SymbolConstant &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step)
    : ArithmeticSequenceType(KIND_PARAM, child, condition, step) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool DerivedArithmeticSequenceType<KIND_PARAM>::classof(
    const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE
LocalDeclaration::LocalDeclaration(rq::SymbolKind kind, llvm::StringRef name,
                                   rq::SymbolTable &containing_table,
                                   rq::SymbolTable &hosting_table,
                                   rq::Module &module)
    : Symbol(kind), _name(name), _containing_table_ptr(&containing_table),
      _hosting_table_ptr(&hosting_table), _module_ptr(&module) {
  RQ_ASSERT(rq::getIsLocalDeclaration(kind), "not local declaration");
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
LocalDeclaration::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
LocalDeclaration::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
LocalDeclaration::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
LocalDeclaration::getHostingTable() const {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
LocalDeclaration::getHostingTable() {
  return rq::dereferencePtr(this->_hosting_table_ptr);
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
Label::Label(llvm::StringRef name, rq::SymbolTable &containing_table,
             rq::Module &module, rq::Instruction &instruction)
    : LocalDeclaration(rq::SymbolKind::LABEL, name, containing_table,
                       containing_table, module),
      _target_instruction_ptr(&instruction) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
Label::getTargetInstruction() const {
  return rq::dereferencePtr(this->_target_instruction_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &Label::getTargetInstruction() {
  return rq::dereferencePtr(this->_target_instruction_ptr);
}

[[nodiscard]] inline bool Label::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::LABEL);
}

RQ_ALWAYS_INLINE
Anchor::Anchor(llvm::StringRef name, rq::SymbolTable &containing_table,
               rq::Module &module, rq::LocalStatement &local_table)
    : LocalDeclaration(rq::SymbolKind::ANCHOR, name, containing_table,
                       containing_table, module),
      _local_table_ptr(&local_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LocalStatement &
Anchor::getLocalStatement() const {
  return rq::dereferencePtr(this->_local_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LocalStatement &Anchor::getLocalStatement() {
  return rq::dereferencePtr(this->_local_table_ptr);
}

[[nodiscard]] inline bool Anchor::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::ANCHOR);
}

RQ_ALWAYS_INLINE
Enumerator::Enumerator(llvm::StringRef name, rq::SymbolTable &containing_table,
                       rq::SymbolTable &hosting_table, rq::Module &module,
                       rq::SymbolConstant *type_ptr, llvm::APInt discriminant)
    : LocalDeclaration(rq::SymbolKind::ENUMERATOR, name, containing_table,
                       hosting_table, module),
      _type_ptr(type_ptr), _discriminant_value(discriminant) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
Enumerator::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *Enumerator::getTypePtr() {
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
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::ENUMERATOR);
}

RQ_ALWAYS_INLINE
LocalVariable::LocalVariable(rq::SymbolKind kind, llvm::StringRef name,
                             rq::SymbolTable &containing_table,
                             rq::SymbolTable &hosting_table, rq::Module &module,
                             rq::ExpressionFlags flags,
                             rq::SymbolConstant &type)
    : LocalDeclaration(kind, name, containing_table, hosting_table, module),
      _expression_flags(flags), _type_ptr(&type) {
  RQ_ASSERT(rq::getIsLocalVariable(kind), "not local variable");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
LocalVariable::getExpressionFlags() const {
  return this->_expression_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
LocalVariable::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &LocalVariable::getType() {
  return rq::dereferencePtr(this->_type_ptr);
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
    llvm::StringRef name, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::Module &module,
    rq::ExpressionFlags flags, rq::SymbolConstant &type)
    : LocalVariable(rq::SymbolKind::LOCAL_DYNAMIC_VARIABLE, name,
                    containing_table, hosting_table, module, flags, type) {}

[[nodiscard]] inline bool
LocalDynamicVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::LOCAL_DYNAMIC_VARIABLE);
}

RQ_ALWAYS_INLINE LocalStaticVariable::LocalStaticVariable(
    llvm::StringRef name, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::Module &module,
    rq::ExpressionFlags flags, rq::SymbolConstant &type,
    rq::Gendex<rq::StaticValue> value)
    : LocalVariable(rq::SymbolKind::LOCAL_STATIC_VARIABLE, name,
                    containing_table, hosting_table, module, flags, type),
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
                   rq::getUnderlying(rq::SymbolKind::LOCAL_STATIC_VARIABLE);
}

RQ_ALWAYS_INLINE TemplateArgument::TemplateArgument(
    llvm::StringRef name, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::Module &module,
    rq::ExpressionFlags flags, rq::SymbolConstant &type, rq::Entity &value,
    rq::LayoutParameter &parameter)
    : LocalVariable(rq::SymbolKind::TEMPLATE_ARGUMENT, name, containing_table,
                    hosting_table, module, flags, type),
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
                   rq::getUnderlying(rq::SymbolKind::TEMPLATE_ARGUMENT);
}

RQ_ALWAYS_INLINE ProcedureArgument::ProcedureArgument(
    llvm::StringRef name, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::Module &module,
    rq::ExpressionFlags flags, rq::SymbolConstant &type,
    rq::ProcedureParameter &parameter)
    : LocalVariable(rq::SymbolKind::TEMPLATE_ARGUMENT, name, containing_table,
                    hosting_table, module, flags, type),
      _parameter_ptr(&parameter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter &
ProcedureArgument::getProcedureParameter() const {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter &
ProcedureArgument::getProcedureParameter() {
  return rq::dereferencePtr(this->_parameter_ptr);
}

[[nodiscard]] inline bool
ProcedureArgument::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::PROCEDURE_ARGUMENT);
}

RQ_ALWAYS_INLINE
Parameter::Parameter(rq::SymbolKind kind, rq::Parameter *next_ptr,
                     llvm::StringRef name, rq::SymbolConstant &type)
    : Symbol(kind), _next_ptr(next_ptr), _name(name), _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Parameter::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
Parameter::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &Parameter::getType() {
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
    rq::SymbolKind kind, rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &hosting_table,
    rq::ExpressionFlags expression_flags, bool is_positional,
    bool is_nonpositional, bool is_locked, rq::Expression &expression,
    rq::Expression &name_expression, rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : Parameter(kind, next_ptr, name, type), _is_positional(is_positional),
      _is_nonpositional(is_nonpositional), _is_locked(is_locked),
      _expression_flags(expression_flags), _hosting_table_ptr(&hosting_table),
      _expression_ptr(&expression), _name_expression_ptr(&name_expression),
      _type_expression_ptr(&type_expression),
      _default_value_expression_ptr(default_value_expression_ptr),
      _module_ptr(&module) {
  RQ_ASSERT(rq::getIsSymbolParameter(kind), "not symbol parameter");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
SymbolParameter::getExpressionFlags() const {
  return this->_expression_flags;
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
SymbolParameter::getHostingTable() const {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolParameter::getHostingTable() {
  return rq::dereferencePtr(this->_hosting_table_ptr);
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
    rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &hosting_table,
    rq::ExpressionFlags expression_flags, bool is_positional,
    bool is_nonpositional, bool is_locked, rq::Expression &expression,
    rq::Expression &name_expression, rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : SymbolParameter(rq::SymbolKind::SIGNATURE_PARAMETER, next_ptr, name, type,
                      hosting_table, expression_flags, is_positional,
                      is_nonpositional, is_locked, expression, name_expression,
                      type_expression, default_value_expression_ptr, module) {}

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
                   rq::getUnderlying(rq::SymbolKind::SIGNATURE_PARAMETER);
}

RQ_ALWAYS_INLINE LayoutParameter::LayoutParameter(
    rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &hosting_table,
    rq::ExpressionFlags expression_flags, bool is_positional,
    bool is_nonpositional, bool is_locked, rq::Expression &expression,
    rq::Expression &name_expression, rq::Expression &type_expression,
    rq::Expression *default_value_expression_ptr, rq::Module &module)
    : SymbolParameter(rq::SymbolKind::LAYOUT_PARAMETER, next_ptr, name, type,
                      hosting_table, expression_flags, is_positional,
                      is_nonpositional, is_locked, expression, name_expression,
                      type_expression, default_value_expression_ptr, module) {}

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
                   rq::getUnderlying(rq::SymbolKind::LAYOUT_PARAMETER);
}

RQ_ALWAYS_INLINE
TypeParameter::TypeParameter(rq::SymbolKind kind, rq::TypeParameter *next_ptr,
                             llvm::StringRef name, rq::SymbolConstant &type,
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
  return !this->_is_positional && this->getName().empty();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TypeParameter::getIsNamePassable() const {
  return !this->_is_positional && !this->getName().empty();
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

inline void TypeParameter::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileTypeParameter(
      out_id, this->getKind(), this->getNextTypeParameterPtr(), this->getName(),
      this->getType(), this->getLocation(), this->getIsPositional());
}

RQ_ALWAYS_INLINE void
profileTypeParameter(llvm::FoldingSetNodeID &out_id, rq::SymbolKind kind,
                     const rq::TypeParameter *next_ptr, llvm::StringRef name,
                     const rq::SymbolConstant &type, unsigned location,
                     bool is_positional) {
  out_id.AddInteger(rq::getUnderlying(kind));
  out_id.AddPointer(next_ptr);
  out_id.AddString(name);
  out_id.AddPointer(&type);
  out_id.AddInteger(location);
  out_id.AddBoolean(is_positional);
}

RQ_ALWAYS_INLINE ProcedureParameter::ProcedureParameter(
    rq::TypeParameter *next_ptr, llvm::StringRef name, rq::SymbolConstant &type,
    unsigned location)
    : TypeParameter(rq::SymbolKind::PROCEDURE_PARAMETER, next_ptr, name, type,
                    location, name.empty()) {}

[[nodiscard]] inline bool
ProcedureParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::PROCEDURE_PARAMETER);
}

RQ_ALWAYS_INLINE
TupleParameter::TupleParameter(rq::TypeParameter *next_ptr,
                               llvm::StringRef name, rq::SymbolConstant &type,
                               unsigned location, bool is_positional)
    : TypeParameter(rq::SymbolKind::TUPLE_PARAMETER, next_ptr, name, type,
                    location, is_positional) {}

[[nodiscard]] inline bool
TupleParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::TUPLE_PARAMETER);
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
ParameterList::getParameterPtrOfName(llvm::StringRef name) const {
  for (const rq::Parameter &parameter : this->getParameterSubrange()) {
    if (parameter.getName() == name) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] inline rq::Parameter *
ParameterList::getParameterPtrOfName(llvm::StringRef name) {
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
SymbolParameterList::getSymbolParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::SymbolParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
SymbolParameterList::getSymbolParameterPtrOfName(llvm::StringRef name) {
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
    rq::Module &module, rq::SymbolConstant &return_type,
    rq::SymbolConstant *reciever_type_ptr,
    rq::Expression *precondition_expression_ptr,
    rq::Expression *postcondition_expression_ptr)
    : SymbolParameterList(rq::SymbolKind::SIGNATURE, first_parameter_ptr,
                          parameter_count, positional_parameter_count,
                          nonpositional_parameter_count, expression,
                          locked_parameter_count, module),
      _return_type_ptr(&return_type), _reciever_type_ptr(reciever_type_ptr),
      _precondition_expression_ptr(precondition_expression_ptr),
      _postcondition_expression_ptr(postcondition_expression_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
Signature::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &Signature::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
Signature::getRecieverTypePtr() const {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *
Signature::getRecieverTypePtr() {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Signature::getPreconditionExpressionPtr() const {
  return this->_precondition_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Signature::getPreconditionExpressionPtr() {
  return this->_precondition_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Signature::getPostconditionExpressionPtr() const {
  return this->_postcondition_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Signature::getPostconditionExpressionPtr() {
  return this->_postcondition_expression_ptr;
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
Signature::getSignatureParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::SignatureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
Signature::getSignatureParameterPtrOfName(llvm::StringRef name) {
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::SIGNATURE);
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
Layout::getLayoutParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::LayoutParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
Layout::getLayoutParameterPtrOfName(llvm::StringRef name) {
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
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::LAYOUT);
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
TypeParameterList::getTypeParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::TypeParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *
TypeParameterList::getTypeParameterPtrOfName(llvm::StringRef name) {
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
                             rq::SymbolConstant &return_type,
                             rq::SymbolConstant *reciever_type_ptr)
    : TypeParameterList(rq::SymbolKind::PROCEDURE_TYPE, first_parameter_ptr,
                        parameter_count, positional_parameter_count,
                        nonpositional_parameter_count),
      _return_type_ptr(&return_type), _reciever_type_ptr(reciever_type_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
ProcedureType::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
ProcedureType::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
ProcedureType::getRecieverTypePtr() const {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *
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
ProcedureType::getProcedureParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::ProcedureParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter *
ProcedureType::getProcedureParameterPtrOfName(llvm::StringRef name) {
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
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::PROCEDURE_TYPE);
}

inline void ProcedureType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileProcedureType(out_id, this->getFirstProcedureParameterPtr(),
                           this->getReturnType(), this->getRecieverTypePtr());
}

inline void
profileProcedureType(llvm::FoldingSetNodeID &out_id,
                     const rq::ProcedureParameter *first_parameter_ptr,
                     const rq::SymbolConstant &return_type,
                     const rq::SymbolConstant *reciever_type_ptr) {
  out_id.AddPointer(first_parameter_ptr);
  out_id.AddPointer(&return_type);
  out_id.AddPointer(reciever_type_ptr);
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
TupleType::getTupleParameterPtrOfName(llvm::StringRef name) const {
  return llvm::cast<rq::TupleParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter *
TupleType::getTupleParameterPtrOfName(llvm::StringRef name) {
  return llvm::cast<rq::TupleParameter>(this->getParameterPtrOfName(name));
}

[[nodiscard]] inline const rq::TupleParameter *
TupleType::getTupleParameterPtrOfType(const rq::SymbolConstant &type) const {
  for (const rq::TupleParameter &parameter :
       this->getTupleParameterSubrange()) {
    if (!parameter.getName().empty()) {
      continue;
    }
    if (parameter.getType() == type) {
      return &parameter;
    }
  }
  return nullptr;
}

[[nodiscard]] inline rq::TupleParameter *
TupleType::getTupleParameterPtrOfType(const rq::SymbolConstant &type) {
  for (rq::TupleParameter &parameter : this->getTupleParameterSubrange()) {
    if (!parameter.getName().empty()) {
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
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::TUPLE_TYPE);
}

inline void TupleType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileTupleType(
      out_id, llvm::cast<rq::TupleParameter>(this->_first_parameter_ptr));
}

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &out_id,
                 const rq::TupleParameter *first_parameter_ptr) {
  out_id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE PlacementType::PlacementType(rq::Procedure &procedure)
    : Symbol(rq::SymbolKind::PLACEMENT_TYPE), _procedure_ptr(&procedure) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &
PlacementType::getProcedure() const {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &PlacementType::getProcedure() {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] inline bool PlacementType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::PLACEMENT_TYPE);
}

inline void PlacementType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profilePlacement(out_id, this->getProcedure());
}

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &out_id,
                                       const rq::Procedure &procedure) {
  out_id.AddPointer(&procedure);
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
CompositionComponent::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileCompositionComponent(out_id, this->_next_ptr);
}

RQ_ALWAYS_INLINE void profileCompositionComponent(
    llvm::FoldingSetNodeID &out_id,
    const rq::CompositionComponent *next_component_ptr) {
  out_id.AddPointer(next_component_ptr);
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
                   rq::getUnderlying(rq::SymbolKind::COMPOSITION_TYPE);
}

inline void CompositionType::Profile(llvm::FoldingSetNodeID &out_id) const {
  rq::profileCompositionType(out_id, this->getFirstComponent());
}

inline void
profileCompositionType(llvm::FoldingSetNodeID &out_id,
                       const rq::CompositionComponent &first_component) {
  out_id.AddPointer(&first_component);
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
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::SYNONYM_TYPE);
}

RQ_ALWAYS_INLINE SymbolTable::SymbolTable(rq::SymbolKind kind,
                                          rq::SymbolTable *containing_table_ptr)
    : Symbol(kind), _containing_table_ptr(containing_table_ptr) {
  RQ_ASSERT(rq::getIsSymbolTable(kind), "not symbol table");
}

inline void SymbolTable::release() {
  for (auto &kvp : this->_named_member_map) {
    for (rq::Symbol &symbol : kvp.second) {
      if (llvm::isa<rq::SymbolTable>(symbol)) {
        rq::SymbolTable &table = llvm::cast<rq::SymbolTable>(symbol);
        table.release();
      }
    }
  }
  for (rq::Symbol &symbol : this->_unamed_member_list) {
    if (llvm::isa<rq::SymbolTable>(symbol)) {
      rq::SymbolTable &table = llvm::cast<rq::SymbolTable>(symbol);
      table.release();
    }
  }
  std::destroy_at(&this->_named_member_map);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
SymbolTable::getContainingTablePtr() const {
  return this->_containing_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *
SymbolTable::getContainingTablePtr() {
  return this->_containing_table_ptr;
}

inline void SymbolTable::addNamedMember(rq::BumpPtrAllocator &allocator,
                                        llvm::StringRef name,
                                        rq::Symbol &symbol) {
  llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>> it =
      this->_named_member_map.find(name);
  if (it == this->_named_member_map.end()) {
    this->_named_member_map.emplace_or_assign(name, symbol);
    return;
  }
  rq::BumpPtrList<rq::Symbol> &list = it->getSecond();
  list.insertFront(allocator, symbol);
}

RQ_ALWAYS_INLINE void
SymbolTable::addUnamedMember(rq::BumpPtrAllocator &allocator,
                             rq::Symbol &symbol) {
  this->_unamed_member_list.insertFront(allocator, symbol);
}

[[nodiscard]] RQ_ALWAYS_INLINE const
    llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>> &
    SymbolTable::getNamedMemberMap() const {
  return this->_named_member_map;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListRef<rq::Symbol>
SymbolTable::getUnamedMemberList() const {
  return rq::ConstBumpPtrListRef<rq::Symbol>(this->_unamed_member_list);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
SymbolTable::getUnamedMemberList() {
  return this->_unamed_member_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstBumpPtrListRef<rq::Symbol>
SymbolTable::findNamedList(llvm::StringRef name) const {
  for (const rq::SymbolTable &table : this->getInclusiveFrameSubrange()) {
    const auto &map = table._named_member_map;
    auto it = map.find(name);
    if (it == map.end()) {
      continue;
    }
    return rq::ConstBumpPtrListRef<rq::Symbol>(it->getSecond());
  }
  return rq::ConstBumpPtrListRef<rq::Symbol>();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
SymbolTable::findNamedList(llvm::StringRef name) {
  for (rq::SymbolTable &table : this->getInclusiveFrameSubrange()) {
    auto &map = table._named_member_map;
    auto it = map.find(name);
    if (it == map.end()) {
      continue;
    }
    return rq::BumpPtrListRef<rq::Symbol>(it->getSecond());
  }
  return rq::BumpPtrListRef<rq::Symbol>();
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::FrameIterator, rq::FrameIterator,
                          std::ranges::subrange_kind::unsized>
    SymbolTable::getInclusiveFrameSubrange() {
  return std::ranges::subrange<rq::FrameIterator, rq::FrameIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::FrameIterator(this), rq::FrameIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstFrameIterator, rq::ConstFrameIterator,
                          std::ranges::subrange_kind::unsized>
    SymbolTable::getInclusiveFrameSubrange() const {
  return std::ranges::subrange<rq::ConstFrameIterator, rq::ConstFrameIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstFrameIterator(this), rq::ConstFrameIterator());
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

RQ_ALWAYS_INLINE Top::Top() : SymbolTable(rq::SymbolKind::TOP, nullptr) {}

[[nodiscard]] inline bool Top::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::TOP);
}

RQ_ALWAYS_INLINE LocalStatement::LocalStatement(
    rq::SymbolKind kind, rq::SymbolTable &containing_table,
    rq::Expression &expression, rq::ExpressionFlags flags, rq::Module &module)
    : SymbolTable(kind, &containing_table), _expression_ptr(&expression),
      _expression_flags(flags), _module_ptr(&module) {
  RQ_ASSERT(rq::getIsLocalStatement(kind), "not local statement");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
LocalStatement::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &LocalStatement::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] rq::ExpressionFlags LocalStatement::getExpressionFlags() const {
  return this->_expression_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
LocalStatement::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &LocalStatement::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] inline bool
LocalStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsLocalStatement(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedLocalStatement<KIND_PARAM>::DerivedLocalStatement(
    rq::SymbolTable &containing_table, rq::Expression &expression,
    rq::ExpressionFlags flags, rq::Module &module)
    : LocalStatement(KIND_PARAM, containing_table, expression, flags, module) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedLocalStatement<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE NamedTable::NamedTable(rq::SymbolKind kind,
                                        rq::SymbolTable &containing_table,
                                        llvm::StringRef name)
    : SymbolTable(kind, &containing_table), _name(name) {
  RQ_ASSERT(rq::getIsNamedTable(kind), "not named table");
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef NamedTable::getName() const {
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

RQ_ALWAYS_INLINE Namespace::Namespace(rq::SymbolTable &containing_table,
                                      llvm::StringRef name)
    : NamedTable(rq::SymbolKind::NAMESPACE, containing_table, name) {}

[[nodiscard]] inline bool Namespace::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::NAMESPACE);
}

RQ_ALWAYS_INLINE
GlobalDeclaration::GlobalDeclaration(
    rq::SymbolKind kind, rq::SymbolTable &containing_table,
    llvm::StringRef name, rq::SymbolTable &hosting_table,
    rq::Expression &expression, rq::Expression *name_expression_ptr,
    rq::ExpressionFlags flags, rq::Module &module)
    : NamedTable(kind, containing_table, name),
      _hosting_table_ptr(&hosting_table), _expression_ptr(&expression),
      _name_expression_ptr(name_expression_ptr), _flags(flags),
      _module_ptr(&module) {
  RQ_ASSERT(rq::getIsGlobalDeclaration(kind), "not global declaration");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool GlobalDeclaration::getIsEvaluated() const {
  return this->_is_evaluated;
}

RQ_ALWAYS_INLINE void GlobalDeclaration::setIsEvaluated() {
  RQ_ASSERT(this->_is_evaluated == false, "already evaluated");
  this->_is_evaluated = true;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
GlobalDeclaration::getContainingTable() const {
  return rq::dereferencePtr(this->getContainingTablePtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
GlobalDeclaration::getContainingTable() {
  return rq::dereferencePtr(this->getContainingTablePtr());
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
GlobalDeclaration::getHostingTable() const {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
GlobalDeclaration::getHostingTable() {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
GlobalDeclaration::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
GlobalDeclaration::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
GlobalDeclaration::getNameExpressionPtr() const {
  return this->_name_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
GlobalDeclaration::getNameExpressionPtr() {
  return this->_name_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
GlobalDeclaration::getExpressionFlags() const {
  return this->_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool GlobalDeclaration::getIsMember() const {
  const rq::SymbolKind containing_kind = this->getContainingTable().getKind();
  return containing_kind == rq::SymbolKind::CLASS_TYPE ||
         containing_kind == rq::SymbolKind::ENUMERATION_TYPE ||
         containing_kind == rq::SymbolKind::INTERFACE;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
GlobalDeclaration::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &GlobalDeclaration::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}

[[nodiscard]] inline bool
GlobalDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsGlobalDeclaration(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE Destructor::Destructor(rq::SymbolTable &containing_table,
                                        rq::SymbolTable &hosting_table,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags flags,
                                        rq::Module &module)
    : GlobalDeclaration(rq::SymbolKind::DESTRUCTOR, containing_table, {},
                        hosting_table, expression, nullptr, flags, module) {}

[[nodiscard]] inline bool Destructor::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::DESTRUCTOR);
}

RQ_ALWAYS_INLINE
Main::Main(rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
           rq::Expression &expression, rq::ExpressionFlags flags,
           rq::Module &module)
    : GlobalDeclaration(rq::SymbolKind::DESTRUCTOR, containing_table, {},
                        hosting_table, expression, nullptr, flags, module) {}

[[nodiscard]] inline bool Main::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::MAIN);
}

RQ_ALWAYS_INLINE
Instance::Instance(rq::SymbolKind kind, rq::SymbolTable &containing_table,
                   llvm::StringRef name, rq::SymbolTable &hosting_table,
                   rq::Expression &expression,
                   rq::Expression *name_expression_ptr,
                   rq::ExpressionFlags flags, rq::Module &module,
                   rq::Polymorph &polymorph, rq::Template *template_ptr,
                   rq::TemplateArgument *first_argument_ptr)
    : GlobalDeclaration(kind, containing_table, name, hosting_table, expression,
                        name_expression_ptr, flags, module),
      _polymorph_ptr(&polymorph), _template_ptr(template_ptr),
      _first_argument_ptr(first_argument_ptr) {
  RQ_ASSERT(rq::getIsInstance(kind), "not instance");
  RQ_ASSERT(containing_table.getIsFrame(), "not contained in frame");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph &
Instance::getPolymorph() const {
  return rq::dereferencePtr(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph &Instance::getPolymorph() {
  return rq::dereferencePtr(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *
Instance::getTemplatePtr() const {
  return this->_template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Template *Instance::getTemplatePtr() {
  return this->_template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateArgument *
Instance::getFirstTemplateArgumentPtr() const {
  return this->_first_argument_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateArgument *
Instance::getFirstTemplateArgumentPtr() {
  return this->_first_argument_ptr;
}

[[nodiscard]] inline bool Instance::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsInstance(static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE
ClassType::ClassType(rq::SymbolTable &containing_table, llvm::StringRef name,
                     rq::SymbolTable &hosting_table, rq::Expression &expression,
                     rq::Expression &name_expression, rq::ExpressionFlags flags,
                     rq::Module &module, rq::Polymorph &polymorph,
                     rq::Template *template_ptr,
                     rq::TemplateArgument *first_argument_ptr)
    : Instance(rq::SymbolKind::CLASS_TYPE, containing_table, name,
               hosting_table, expression, &name_expression, flags, module,
               polymorph, template_ptr, first_argument_ptr) {}

[[nodiscard]] inline bool ClassType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id ==
         rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::CLASS_TYPE);
}

RQ_ALWAYS_INLINE EnumerationType::EnumerationType(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, rq::Expression &expression,
    rq::Expression &name_expression, rq::ExpressionFlags flags,
    rq::Module &module, rq::Polymorph &polymorph, rq::Template *template_ptr,
    rq::TemplateArgument *first_argument_ptr)
    : Instance(rq::SymbolKind::ENUMERATION_TYPE, containing_table, name,
               hosting_table, expression, &name_expression, flags, module,
               polymorph, template_ptr, first_argument_ptr) {}

[[nodiscard]] inline bool
EnumerationType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::ENUMERATION_TYPE);
}

RQ_ALWAYS_INLINE
Interface::Interface(rq::SymbolTable &containing_table, llvm::StringRef name,
                     rq::SymbolTable &hosting_table, rq::Expression &expression,
                     rq::Expression &name_expression, rq::ExpressionFlags flags,
                     rq::Module &module, rq::Polymorph &polymorph,
                     rq::Template *template_ptr,
                     rq::TemplateArgument *first_argument_ptr)
    : Instance(rq::SymbolKind::INTERFACE, containing_table, name, hosting_table,
               expression, &name_expression, flags, module, polymorph,
               template_ptr, first_argument_ptr) {}

[[nodiscard]] inline bool Interface::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::INTERFACE);
}

RQ_ALWAYS_INLINE
Adapter::Adapter(rq::SymbolTable &containing_table, llvm::StringRef name,
                 rq::SymbolTable &hosting_table, rq::Expression &expression,
                 rq::Expression &name_expression, rq::ExpressionFlags flags,
                 rq::Module &module, rq::Polymorph &polymorph,
                 rq::Template *template_ptr,
                 rq::TemplateArgument *first_argument_ptr)
    : Instance(rq::SymbolKind::ADAPTER, containing_table, name, hosting_table,
               expression, &name_expression, flags, module, polymorph,
               template_ptr, first_argument_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface *
Adapter::getInterfacePtr() const {
  return this->_interface_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Interface *Adapter::getInterfacePtr() {
  return this->_interface_ptr;
}

[[nodiscard]] inline bool Adapter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::ADAPTER);
}

RQ_ALWAYS_INLINE
GlobalVariable::GlobalVariable(
    rq::SymbolKind kind, rq::SymbolTable &containing_table,
    llvm::StringRef name, rq::SymbolTable &hosting_table,
    rq::Expression &expression, rq::Expression &name_expression,
    rq::ExpressionFlags flags, rq::Module &module, rq::Polymorph &polymorph,
    rq::Template *template_ptr, rq::TemplateArgument *first_argument_ptr,
    rq::Expression *initial_value_expression_ptr)
    : Instance(kind, containing_table, name, hosting_table, expression,
               &name_expression, flags, module, polymorph, template_ptr,
               first_argument_ptr),
      _initial_value_expression_ptr(initial_value_expression_ptr) {
  RQ_ASSERT(rq::getIsGlobalVariable(kind), "not global variable");
}

RQ_ALWAYS_INLINE void GlobalVariable::setType(rq::SymbolConstant &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
GlobalVariable::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *
GlobalVariable::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
GlobalVariable::getInitialValueExpression() const {
  return rq::dereferencePtr(this->_initial_value_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
GlobalVariable::getInitialValueExpression() {
  return rq::dereferencePtr(this->_initial_value_expression_ptr);
}

[[nodiscard]] inline bool
GlobalVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsGlobalVariable(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

RQ_ALWAYS_INLINE GlobalDynamicVariable::GlobalDynamicVariable(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, rq::Expression &expression,
    rq::Expression &name_expression, rq::ExpressionFlags flags,
    rq::Module &module, rq::Polymorph &polymorph, rq::Template *template_ptr,
    rq::TemplateArgument *first_argument_ptr,
    rq::Expression &initial_value_expression)
    : GlobalVariable(rq::SymbolKind::GLOBAL_DYNAMIC_VARIABLE, containing_table,
                     name, hosting_table, expression, name_expression, flags,
                     module, polymorph, template_ptr, first_argument_ptr,
                     &initial_value_expression) {}

RQ_ALWAYS_INLINE void
GlobalDynamicVariable::setInitialValue(rq::Entity &entity) {
  rq::assignSingleValue(this->_initial_value_ptr, &entity);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
GlobalDynamicVariable::getInitialValuePtr() const {
  return this->_initial_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *
GlobalDynamicVariable::getInitialValuePtr() {
  return this->_initial_value_ptr;
}

[[nodiscard]] inline bool
GlobalDynamicVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::GLOBAL_DYNAMIC_VARIABLE);
}

RQ_ALWAYS_INLINE GlobalStaticVariable::GlobalStaticVariable(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, rq::Expression &expression,
    rq::Expression &name_expression, rq::ExpressionFlags flags,
    rq::Module &module, rq::Polymorph &polymorph, rq::Template *template_ptr,
    rq::TemplateArgument *first_argument_ptr,
    rq::Expression &initial_value_expression)
    : GlobalVariable(rq::SymbolKind::GLOBAL_STATIC_VARIABLE, containing_table,
                     name, hosting_table, expression, name_expression, flags,
                     module, polymorph, template_ptr, first_argument_ptr,
                     &initial_value_expression) {}

RQ_ALWAYS_INLINE void GlobalStaticVariable::setValue(rq::Entity &value) {
  rq::assignSingleValue(this->_value_ptr, &value);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
GlobalStaticVariable::getValuePtr() const {
  return this->_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *GlobalStaticVariable::getValuePtr() {
  return this->_value_ptr;
}

[[nodiscard]] inline bool
GlobalStaticVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET +
                   rq::getUnderlying(rq::SymbolKind::GLOBAL_STATIC_VARIABLE);
}

RQ_ALWAYS_INLINE
Ranger::Ranger(rq::SymbolKind kind, rq::SymbolTable &containing_table,
               rq::SymbolTable &hosting_table, rq::Expression &expression,
               rq::ExpressionFlags flags, rq::Module &module,
               rq::Polymorph &polymorph, rq::Template *template_ptr,
               rq::TemplateArgument *first_argument_ptr,
               rq::Expression &reciever_type_expression,
               rq::Expression &element_type_expression)
    : Instance(kind, containing_table, {}, hosting_table, expression, nullptr,
               flags, module, polymorph, template_ptr, first_argument_ptr),
      _reciever_type_expression_ptr(&reciever_type_expression),
      _element_type_expression_ptr(&element_type_expression) {
  RQ_ASSERT(rq::getIsRanger(kind), "not ranger");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Ranger::getRecieverTypeExpression() const {
  return rq::dereferencePtr(this->_reciever_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
Ranger::getRecieverTypeExpression() {
  return rq::dereferencePtr(this->_reciever_type_expression_ptr);
}

RQ_ALWAYS_INLINE void
Ranger::setRecieverType(rq::SymbolConstant &reciever_type) {
  rq::assignSingleValue(this->_reciever_type_ptr, &reciever_type);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
Ranger::getRecieverTypePtr() const {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *
Ranger::getRecieverTypePtr() {
  return this->_reciever_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Ranger::getElementTypeExpression() const {
  return rq::dereferencePtr(this->_element_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
Ranger::getElementTypeExpression() {
  return rq::dereferencePtr(this->_element_type_expression_ptr);
}

RQ_ALWAYS_INLINE void Ranger::setElementType(rq::SymbolConstant &element) {
  rq::assignSingleValue(this->_element_type_ptr, &element);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
Ranger::getElementTypePtr() const {
  return this->_element_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *Ranger::getElementTypePtr() {
  return this->_element_type_ptr;
}

[[nodiscard]] inline bool Ranger::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsRanger(static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedRanger<KIND_PARAM>::DerivedRanger(
    rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
    rq::Expression &expression, rq::ExpressionFlags flags, rq::Module &module,
    rq::Polymorph &polymorph, rq::Template *template_ptr,
    rq::TemplateArgument *first_argument_ptr,
    rq::Expression &reciever_type_expression,
    rq::Expression &element_type_expression)
    : Ranger(KIND_PARAM, containing_table, hosting_table, expression, flags,
             module, polymorph, template_ptr, first_argument_ptr,
             reciever_type_expression, element_type_expression) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedRanger<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE
Procedure::Procedure(rq::SymbolKind kind, rq::SymbolTable &containing_table,
                     llvm::StringRef name, rq::SymbolTable &hosting_table,
                     rq::Expression &expression,
                     rq::Expression &name_expression, rq::ExpressionFlags flags,
                     rq::Module &module, rq::Polymorph &polymorph,
                     rq::Template *template_ptr,
                     rq::TemplateArgument *first_argument_ptr)
    : Instance(kind, containing_table, name, hosting_table, expression,
               &name_expression, flags, module, polymorph, template_ptr,
               first_argument_ptr) {
  RQ_ASSERT(rq::getIsProcedure(kind), "not procedure");
}

RQ_ALWAYS_INLINE
void Procedure::setSignature(rq::Signature &signature) {
  rq::assignSingleValue(this->_signature_ptr, &signature);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature *
Procedure::getSignaturePtr() const {
  return this->_signature_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Signature *Procedure::getSignaturePtr() {
  return this->_signature_ptr;
}

[[nodiscard]] inline bool Procedure::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsProcedure(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedProcedure<KIND_PARAM>::DerivedProcedure(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, rq::Expression &expression,
    rq::Expression &name_expression, rq::ExpressionFlags flags,
    rq::Module &module, rq::Polymorph &polymorph, rq::Template *template_ptr,
    rq::TemplateArgument *first_argument_ptr)
    : Procedure(KIND_PARAM, containing_table, name, hosting_table, expression,
                name_expression, flags, module, polymorph, template_ptr,
                first_argument_ptr) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedProcedure<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE
Template::Template(rq::SymbolKind kind, rq::SymbolTable &containing_table,
                   llvm::StringRef name, rq::SymbolTable &hosting_table,
                   rq::Expression &expression, rq::Expression &name_expression,
                   rq::ExpressionFlags flags, rq::Module &module,
                   rq::Expression &layout_expression,
                   rq::Expression *constraint_expression_ptr,
                   rq::Expression *weight_expression_ptr, unsigned weight)
    : GlobalDeclaration(kind, containing_table, name, hosting_table, expression,
                        &name_expression, flags, module),
      _layout_expression_ptr(&layout_expression),
      _constraint_expression_ptr(constraint_expression_ptr),
      _weight_expression_ptr(weight_expression_ptr), _weight(weight) {
  RQ_ASSERT(rq::getIsTemplate(kind), "not template");
  RQ_ASSERT(containing_table.getIsFrame(), "not contained in frame");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Template::getLayoutExpression() const {
  return rq::dereferencePtr(this->_layout_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Template::getLayoutExpression() {
  return rq::dereferencePtr(this->_layout_expression_ptr);
}

RQ_ALWAYS_INLINE void Template::setLayout(rq::Layout &layout) {
  rq::assignSingleValue(this->_layout_ptr, &layout);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout *
Template::getLayoutPtr() const {
  return this->_layout_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Layout *Template::getLayoutPtr() {
  return this->_layout_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Template::getConstraintExpressionPtr() const {
  return this->_constraint_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Template::getConstraintExpressionPtr() {
  return this->_constraint_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Template::getWeightExpressionPtr() const {
  return this->_weight_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Template::getWeightExpressionPtr() {
  return this->_weight_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned Template::getWeight() const {
  return this->_weight;
}

[[nodiscard]] inline bool Template::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsTemplate(static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedTemplate<KIND_PARAM>::DerivedTemplate(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, rq::Expression &expression,
    rq::Expression &name_expression, rq::ExpressionFlags flags,
    rq::Module &module, rq::Expression &layout_expression,
    rq::Expression *constraint_expression_ptr,
    rq::Expression *weight_expression_ptr, unsigned weight)
    : Template(KIND_PARAM, containing_table, name, hosting_table, expression,
               name_expression, flags, module, layout_expression,
               constraint_expression_ptr, weight_expression_ptr, weight) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedTemplate<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE WeightLevel::WeightLevel(rq::SymbolKind kind, unsigned weight,
                                          rq::Polymorph &polymorph,
                                          rq::SymbolTable &containing_table)
    : Symbol(kind), _weight(weight), _polymorph_ptr(&polymorph),
      _containing_table_ptr(&containing_table) {
  RQ_ASSERT(rq::getIsWeightLevel(kind), "not weight level");
  RQ_ASSERT(containing_table.getIsFrame(), "not contained in frame");
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned WeightLevel::getWeight() const {
  return this->_weight;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph &
WeightLevel::getPolymorph() const {
  return rq::dereferencePtr(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph &WeightLevel::getPolymorph() {
  return rq::dereferencePtr(this->_polymorph_ptr);
}

[[nodiscard]] const rq::SymbolTable &WeightLevel::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] rq::SymbolTable &WeightLevel::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::Template>,
                          rq::NextIterator<rq::Template>,
                          std::ranges::subrange_kind::unsized>
    WeightLevel::getTemplateSubrange() {
  return std::ranges::subrange<rq::NextIterator<rq::Template>,
                               rq::NextIterator<rq::Template>,
                               std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::Template>(this->_first_ptr),
      rq::NextIterator<rq::Template>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstNextIterator<rq::Template>,
                          rq::ConstNextIterator<rq::Template>,
                          std::ranges::subrange_kind::unsized>
    WeightLevel::getTemplateSubrange() const {
  return std::ranges::subrange<rq::ConstNextIterator<rq::Template>,
                               rq::ConstNextIterator<rq::Template>,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::Template>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template>());
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedWeightLevel<KIND_PARAM>::DerivedWeightLevel(
    unsigned weight, rq::Polymorph &polymorph,
    rq::SymbolTable &containing_table)
    : WeightLevel(KIND_PARAM, weight, polymorph, containing_table) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedWeightLevel<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

RQ_ALWAYS_INLINE Polymorph::Polymorph(rq::SymbolKind kind, llvm::StringRef name,
                                      rq::SymbolTable &containing_table)
    : Symbol(kind), _name(name), _containing_table_ptr(&containing_table) {
  RQ_ASSERT(rq::getIsPolymorph(kind), "not polymorph");
  RQ_ASSERT(containing_table.getIsFrame(), "not contained in frame");
}

// inline void Polymorph::addTemplate(rq::BumpPtrAllocator &allocator,
//                                    rq::Template &template_) {
// rq::WeightLevel *prev_level_ptr = nullptr;
// for (rq::WeightLevel &level : this->getWeightSubrange()) {
//   if (level.getWeight() < template_.getWeight()) {
//     rq::WeightLevel &new_level = allocator.allocateValue<rq::WeightLevel>(
//         template_.getWeight(), *this);
//     new_level._first_ptr = &template_;
//     if (prev_level_ptr == nullptr) {
//       new_level._next_ptr = this->_highest_weight_ptr;
//       this->_highest_weight_ptr = &new_level;
//       return;
//     }
//     rq::WeightLevel &prev_level = rq::dereferencePtr(prev_level_ptr);
//     new_level._next_ptr = prev_level._next_ptr;
//     prev_level._next_ptr = &new_level;
//     return;
//   } else if (level.getWeight() == template_.getWeight()) {
//     template_._next_ptr = level._first_ptr;
//     level._first_ptr = &template_;
//     return;
//   }
//   prev_level_ptr = &level;
// }
// rq::WeightLevel &new_level =
//     allocator.allocateValue<rq::WeightLevel>(template_.getWeight());
// new_level._first_ptr = &template_;
// if (prev_level_ptr == nullptr) {
//   new_level._next_ptr = this->_highest_weight_ptr;
//   this->_highest_weight_ptr = &new_level;
//   return;
// }
// rq::WeightLevel &prev_level = rq::dereferencePtr(prev_level_ptr);
// new_level._next_ptr = prev_level._next_ptr;
// prev_level._next_ptr = &new_level;
//}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
Polymorph::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
Polymorph::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Polymorph::getHasSomeInstance() const {
  return this->_first_instance_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Polymorph::getHasMultipleInstance() const {
  if (this->_first_instance_ptr == nullptr) {
    return false;
  }
  rq::Instance &first_instance = rq::dereferencePtr(this->_first_instance_ptr);
  if (first_instance._next_ptr == nullptr) {
    return false;
  }
  return true;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Instance &
Polymorph::getOnlyInstance() const {
  RQ_ASSERT(!this->getHasMultipleInstance(), "has multiple");
  return rq::dereferencePtr(this->_first_instance_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Instance &Polymorph::getOnlyInstance() {
  RQ_ASSERT(!this->getHasMultipleInstance(), "has multiple");
  return rq::dereferencePtr(this->_first_instance_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::NextIterator<rq::WeightLevel>,
                          rq::NextIterator<rq::WeightLevel>,
                          std::ranges::subrange_kind::unsized>
    Polymorph::getWeightLevelSubrange() {
  return std::ranges::subrange<rq::NextIterator<rq::WeightLevel>,
                               rq::NextIterator<rq::WeightLevel>,
                               std::ranges::subrange_kind::unsized>(
      rq::NextIterator<rq::WeightLevel>(this->_highest_weight_ptr),
      rq::NextIterator<rq::WeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    std::ranges::subrange<rq::ConstNextIterator<rq::WeightLevel>,
                          rq::ConstNextIterator<rq::WeightLevel>,
                          std::ranges::subrange_kind::unsized>
    Polymorph::getWeightLevelSubrange() const {
  return std::ranges::subrange<rq::ConstNextIterator<rq::WeightLevel>,
                               rq::ConstNextIterator<rq::WeightLevel>,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstNextIterator<rq::WeightLevel>(this->_highest_weight_ptr),
      rq::ConstNextIterator<rq::WeightLevel>());
}

[[nodiscard]] inline bool Polymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  const rq::EntityId id = entity.getId();
  return rq::getIsPolymorph(
      static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET));
}

template <rq::SymbolKind KIND_PARAM>
RQ_ALWAYS_INLINE DerivedPolymorph<KIND_PARAM>::DerivedPolymorph(
    llvm::StringRef name, rq::SymbolTable &containing_table)
    : Polymorph(KIND_PARAM, name, containing_table) {}

template <rq::SymbolKind KIND_PARAM>
[[nodiscard]] inline bool
DerivedPolymorph<KIND_PARAM>::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id == rq::SYMBOL_OFFSET + rq::getUnderlying(KIND_PARAM);
}

} // namespace rq