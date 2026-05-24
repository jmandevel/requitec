#pragma once

#include <functional>

namespace rq {

[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OF = rq::OpcodeFlags;
  switch (opcode) {
  case O::NONE:
    return OF::NONE;

  // LITERALS
  case O::SY_INTEGER_LITERAL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL | OF::SY_IS_TYPE |
           OF::SY_IS_SIGNED_TYPE;
  case O::SY_FLOAT_LITERAL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL | OF::SY_IS_TYPE |
           OF::SY_IS_SIGNED_TYPE;
  case O::SY_STRING_LITERAL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL | OF::SY_IS_TYPE;
  case O::SY_CODEUNIT_LITERAL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL | OF::SY_IS_TYPE |
           OF::SY_IS_CODEUNIT_TYPE;

  // CONTEXTUAL VALUE
  case O::SY_OUT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_THIS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_RESULT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_VALUE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_INDEX:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_DISCRIMINANT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_COMMAND_LINE_ARGUMENTS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;
  case O::SY_CALLSITE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_VALUE;

  // CONTEXTUAL TYPE
  case O::SY_INFERENCE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_TYPE | OF::SY_IS_TYPE;
  case O::SY_VOID_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_TYPE | OF::SY_IS_TYPE;
  case O::SY_NO_RETURN_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL |
           OF::SY_CONTEXTUAL_TYPE | OF::SY_IS_TYPE;

  // EXPRESSION ATTRIBUTES
  case O::SY_ANCHOR_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_OPAQUE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_GLOBAL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_ACCESS_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_PARTIAL_MUTATE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_STATIC_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_CAPTURE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_INLINE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_MANGLE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_PACK_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_BRANCH_TREND_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_DEPRECIATE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_STABLE_ADDRESS_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_VARIADIC_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_LOCATION_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_TEMPLATE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_CONSTRAINT_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_WEIGHT_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;

  // TYPE ATTRIBUTES
  case O::SY_VAR_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_VOLATILE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_ATOMIC_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_NULL_TERMINATE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_REQUIRE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_ENSURE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;

  // REFLECTIVE
  case O::SY_SYMBOL_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_REFLECTIVE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_EXPRESSION_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_REFLECTIVE_TYPE |
           OF::SY_IS_TYPE;

  // PLATFORM PRIMITIVE
  case O::SY_BOOLEAN_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_HALF_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE;
  case O::SY_SINGLE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE;
  case O::SY_DOUBLE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE;
  case O::SY_QUADRUPLE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE;
  case O::SY_SIGNED_INTEGER_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_UNSIGNED_INTEGER_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_UNSIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_SIGNED_INDEX_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_UNSIGNED_INDEX_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_UNSIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_SIGNED_ADDRESS_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_UNSIGNED_ADDRESS_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_UNSIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;

  // STANDARD PRIMITIVE TYPE
  case O::SY_BINARY16_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE |
           OF::SY_IS_BINARY_TYPE;
  case O::SY_BINARY32_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE |
           OF::SY_IS_BINARY_TYPE;
  case O::SY_BINARY64_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE |
           OF::SY_IS_BINARY_TYPE;
  case O::SY_BINARY128_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE |
           OF::SY_IS_BINARY_TYPE;
  case O::SY_BFLOAT16_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_SIGNED_TYPE | OF::SY_IS_FLOAT_TYPE |
           OF::SY_IS_BFLOAT_TYPE;
  case O::SY_ASCII_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_CODEUNIT_TYPE;
  case O::SY_UTF8_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_CODEUNIT_TYPE;

  // VARIADIC ARGUMENTS
  case O::SY_VARIADIC_ARGUMENTS_TYPE_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_IS_TYPE;

  // SCALED PRIMITIVES
  case O::SY_SCALED_SIGNED_INTEGER_TYPE:
    return OF::SYMBOL | OF::SY_SCALED_PRIMITIVE_TYPE | OF::SY_IS_TYPE |
           OF::SY_IS_SIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;
  case O::SY_SCALED_UNSIGNED_INTEGER_TYPE:
    return OF::SYMBOL | OF::SY_SCALED_PRIMITIVE_TYPE | OF::SY_IS_TYPE |
           OF::SY_IS_UNSIGNED_TYPE | OF::SY_IS_INTEGER_TYPE;

  // ARRAY
  case O::SY_ARRAY_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_IS_TYPE;

  // UNCOUNTED SUBTYPES
  case O::SY_REFERENCE_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE |
           OF::SY_IS_TYPE;
  case O::SY_POINTER_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE |
           OF::SY_IS_TYPE;
  case O::SY_FAT_POINTER_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE |
           OF::SY_IS_TYPE;
  case O::SY_INFERENCE_COUNT_ARRAY_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE |
           OF::SY_IS_TYPE;

  // MODULES
  case O::SY_MODULE:
    return OF::SYMBOL;

  // IMPORTS
  case O::SY_IMPORT:
    return OF::SYMBOL | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // JUXTAPOSITIONAL LIST
  case O::SY_CONCATINATED_LIST_TYPE:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // ARITHMETIC SEQUENCES
  case O::SY_ARITHMETIC_INTERVAL:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_IS_TYPE;
  case O::SY_INFINITE_ARITHMETIC_SEQUENCE:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_IS_TYPE;
  case O::SY_FINITE_ARITHMETIC_SEQUENCE:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_IS_TYPE;

  // LOCAL DECLARATIONS
  case O::SY_LABEL:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION;
  case O::SY_ANCHOR:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION;

  // LOCAL VARIABLES
  case O::SY_LOCAL_DYNAMIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_LOCAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // SYMBOL PARAMETERS
  case O::SY_SIGNATURE_PARAMETER:
    return OF::SYMBOL | OF::SY_SYMBOL_PARAMETER;
  case O::SY_LAYOUT_PARAMETER:
    return OF::SYMBOL | OF::SY_SYMBOL_PARAMETER;

  // TYPE PARAMETERS
  case O::SY_TUPLE_PARAMETER:
    return OF::SYMBOL | OF::SY_TYPE_PARAMETER;
  case O::SY_PROCEDURE_PARAMETER:
    return OF::SYMBOL | OF::SY_TYPE_PARAMETER;

  // SYMBOL PARAMETER LISTS
  case O::SY_SIGNATURE:
    return OF::SYMBOL | OF::SY_SYMBOL_PARAMETER_LIST;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_SYMBOL_PARAMETER_LIST;

  // TYPE PARAMETER LISTS
  case O::SY_TUPLE_TYPE:
    return OF::SYMBOL | OF::SY_TYPE_PARAMETER_LIST | OF::SY_IS_TYPE;
  case O::SY_PROCEDURE_TYPE:
    return OF::SYMBOL | OF::SY_TYPE_PARAMETER_LIST | OF::SY_IS_TYPE;

    // PLACEMENTS
  case O::SY_PLACEMENT_TYPE:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // COMPOSITIONS
  case O::SY_COMPOSITION_TYPE:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // SYNONYMS
  case O::SY_SYNONYM_TYPE:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // SYMBOL TABLES
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE;

  // LOCAL STATEMENTS
  case O::SY_IF_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE_IF_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_MATCH_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SWITCH_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_CASE_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WITH_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_DEFAULT_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_FOR_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WHILE_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SPIN_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WEAVE_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SCOPE_STATEMENT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;

  // NAMED TABLES
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_NAMED_TABLE | OF::SY_SYMBOL_TYPE_TABLE;

  // GLOBAL DECLARATION
  case O::SY_CLASS_TYPE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_IS_TYPE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATION_TYPE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_IS_TYPE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE;
  case O::SY_INTERFACE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // GLOBAL VARIABLE
  case O::SY_GLOBAL_DYNAMIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_GLOBAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_GLOBAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_GLOBAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // RANGERS
  case O::SY_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_RANGER |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_RANGER |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;

  // DESTRUCTOR
  case O::SY_DESTRUCTOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;

  // PROCEDURES
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_PROCEDURE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_PROCEDURE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_PROCEDURE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_SYMBOL_TYPE_TABLE | OF::SY_PROCEDURE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;

  // TEMPLATES
  case O::SY_CLASS_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INTERFACE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_NAMED_TABLE |
           OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // POLYMORPHS
  case O::SY_RANGER_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_PROCEDURE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_CLASS_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_ENUMERATION_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_INTERFACE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;

  case O::CT_INTEGER:
    return OF::CONSTANT;
  case O::CT_FLOAT:
    return OF::CONSTANT;
  case O::CT_EXPRESSION:
    return OF::CONSTANT;
  case O::CT_SYMBOL:
    return OF::CONSTANT;
  case O::CT_BOOLEAN:
    return OF::CONSTANT;
  case O::CT_STRING:
    return OF::CONSTANT;
  case O::CT_ARRAY:
    return OF::CONSTANT;
  case O::CT_EXPRESSION_ATTRIBUTE:
    return OF::CONSTANT;
  case O::CT_TYPE_ATTRIBUTE:
    return OF::CONSTANT;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionFlags
getValidExpressionFlags(rq::Opcode opcode) {
  RQ_ASSERT(rq::getHasExpressionAttributes(opcode), "has no attributes");
  using O = rq::Opcode;
  using EF = rq::ExpressionFlags;
  switch (opcode) {
  case O::SY_IMPORT:
    return EF::EXPORT;
    return EF::PUBLIC | EF::PARTIAL_MUTATE | EF::LOCATION;
  case O::SY_SIGNATURE_PARAMETER:
    return EF::VARIADIC | EF::LOCATION;
  case O::SY_SIGNATURE:
    return EF::ENSURE | EF::REQUIRE;
  case O::SY_IF_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_ELSE_IF_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_ELSE_STATEMENT:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_MATCH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SWITCH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_CASE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WITH_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_DEFAULT_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_FOR_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WHILE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SPIN_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WEAVE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SCOPE_STATEMENT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_CLASS_TYPE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::PACK | EF::DEPRECIATE | EF::EXPERIMENTAL | EF::STABLE_ADDRESS;
  case O::SY_ENUMERATION_TYPE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_INTERFACE:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_GLOBAL_DYNAMIC_VARIABLE:
    return EF::OPAQUE | EF::GLOBAL | EF::EXPORT | EF::CAPTURE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_GLOBAL_STATIC_VARIABLE:
    return EF::GLOBAL | EF::EXPORT | EF::STATIC | EF::CAPTURE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_FORWARD_RANGER:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case O::SY_BACKWARD_RANGER:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case O::SY_DESTRUCTOR:
    return EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_ENTRY:
    return EF::CAPTURE | EF::MANGLE;
  case O::SY_FUNCTION:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_METHOD:
    return EF::OPAQUE | EF::PUBLIC | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_EXTENSION_METHOD:
    return EF::OPAQUE | EF::EXPORT | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_CLASS_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::PACK | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_ENUMERATION_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_INTERFACE_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_FUNCTION_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_METHOD_TEMPLATE:
    return EF::PUBLIC | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::CONSTANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasNone(flags, rq::OpcodeFlags::INSTRUCTION_NONE_MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasNone(flags, rq::OpcodeFlags::SY_SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONTEXTUAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttributeType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags,
                       rq::OpcodeFlags::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttributeType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE_ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_REFLECTIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledPrimitive(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUncountedSubtype(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNCOUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasSome(flags, rq::OpcodeFlags::SY_SYMBOL_PARAMETER |
                                   rq::OpcodeFlags::SY_TYPE_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolParameter(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameter(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasSome(flags, rq::OpcodeFlags::SY_SYMBOL_PARAMETER_LIST |
                                   rq::OpcodeFlags::SY_TYPE_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolParameterList(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameterList(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_TYPE_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclaration(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_RANGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_SIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_UNSIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_INTEGER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_BINARY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_BFLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionAttributes(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_EXPRESSION_ATTRIBUTES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_TABLE);
}

RQ_ALWAYS_INLINE Entity::Entity(rq::Opcode opcode) : _opcode(opcode) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::operator==(const Self &rhs) const {
  return this == &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::operator!=(const Self &rhs) const {
  return this != &rhs;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getOpcode() const {
  return this->_opcode;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags Entity::getOpcodeFlags() const {
  return rq::getFlags(this->getOpcode());
}

[[nodiscard]] inline bool Entity::classof(const rq::Entity *entity_ptr) {
  std::ignore = entity_ptr;
  return true;
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::Opcode opcode) : Entity(opcode) {
  RQ_ASSERT_SYMBOL(opcode);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsType() const {
  return rq::getIsType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSignedType() const {
  return rq::getIsSignedType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsignedType() const {
  return rq::getIsUnsignedType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsIntegerType() const {
  return rq::getIsIntegerType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFloatType() const {
  return rq::getIsFloatType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBinaryType() const {
  return rq::getIsBinaryType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBfloatType() const {
  return rq::getIsBfloatType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunitType() const {
  return rq::getIsCodeunitType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasExpressionAttributes() const {
  return rq::getHasExpressionAttributes(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLocalTable() const {
  return rq::getIsLocalTable(this->getOpcode());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbol(entity.getOpcode());
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::Opcode opcode)
    : Symbol(opcode) {
  RQ_ASSERT(rq::getIsSimpleSymbol(opcode), "not simple symbol");
}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSimpleSymbol(entity.getOpcode());
}

inline void SimpleSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileSimpleSymbol(id, this->getOpcode());
}

RQ_ALWAYS_INLINE void profileSimpleSymbol(llvm::FoldingSetNodeID &id,
                                          rq::Opcode opcode) {
  id.AddInteger(rq::getUnderlying(opcode));
}

RQ_ALWAYS_INLINE LiteralType::LiteralType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {
  RQ_ASSERT(rq::getIsLiteralType(opcode), "not literal type");
}

[[nodiscard]] inline bool LiteralType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLiteralType(entity.getOpcode());
}

RQ_ALWAYS_INLINE IntegerLiteralType::IntegerLiteralType()
    : LiteralType(rq::Opcode::SY_INTEGER_LITERAL_TYPE) {}

[[nodiscard]] inline bool
IntegerLiteralType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INTEGER_LITERAL_TYPE;
}

RQ_ALWAYS_INLINE FloatLiteralType::FloatLiteralType()
    : LiteralType(rq::Opcode::SY_FLOAT_LITERAL_TYPE) {}

[[nodiscard]] inline bool
FloatLiteralType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_FLOAT_LITERAL_TYPE;
}

RQ_ALWAYS_INLINE StringLiteralType::StringLiteralType()
    : LiteralType(rq::Opcode::SY_STRING_LITERAL_TYPE) {}

[[nodiscard]] inline bool
StringLiteralType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_STRING_LITERAL_TYPE;
}

RQ_ALWAYS_INLINE CodeunitLiteralType::CodeunitLiteralType()
    : LiteralType(rq::Opcode::SY_CODEUNIT_LITERAL_TYPE) {}

[[nodiscard]] inline bool
CodeunitLiteralType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CODEUNIT_LITERAL_TYPE;
}

RQ_ALWAYS_INLINE Contextual::Contextual(rq::Opcode opcode)
    : SimpleSymbol(opcode) {
  RQ_ASSERT(rq::getIsContextual(opcode), "not contextual");
}

[[nodiscard]] inline bool Contextual::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextual(entity.getOpcode());
}

RQ_ALWAYS_INLINE ContextualValue::ContextualValue(rq::Opcode opcode)
    : Contextual(opcode) {
  RQ_ASSERT(rq::getIsContextualValue(opcode), "not contextual value");
}

[[nodiscard]] inline bool
ContextualValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextualValue(entity.getOpcode());
}

RQ_ALWAYS_INLINE OutValue::OutValue() : ContextualValue(rq::Opcode::SY_OUT) {}

[[nodiscard]] inline bool OutValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_OUT;
}

RQ_ALWAYS_INLINE ThisValue::ThisValue()
    : ContextualValue(rq::Opcode::SY_THIS) {}

[[nodiscard]] inline bool ThisValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_THIS;
}

RQ_ALWAYS_INLINE ResultValue::ResultValue()
    : ContextualValue(rq::Opcode::SY_RESULT) {}

[[nodiscard]] inline bool ResultValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_RESULT;
}

RQ_ALWAYS_INLINE ValueValue::ValueValue()
    : ContextualValue(rq::Opcode::SY_VALUE) {}

[[nodiscard]] inline bool ValueValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VALUE;
}

RQ_ALWAYS_INLINE IndexValue::IndexValue()
    : ContextualValue(rq::Opcode::SY_INDEX) {}

[[nodiscard]] inline bool IndexValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INDEX;
}

RQ_ALWAYS_INLINE DiscriminantValue::DiscriminantValue()
    : ContextualValue(rq::Opcode::SY_DISCRIMINANT) {}

[[nodiscard]] inline bool
DiscriminantValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_DISCRIMINANT;
}

RQ_ALWAYS_INLINE CommandLineArgumentsValue::CommandLineArgumentsValue()
    : ContextualValue(rq::Opcode::SY_COMMAND_LINE_ARGUMENTS) {}

[[nodiscard]] inline bool
CommandLineArgumentsValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_OUT;
}

RQ_ALWAYS_INLINE CallsiteValue::CallsiteValue()
    : ContextualValue(rq::Opcode::SY_CALLSITE) {}

[[nodiscard]] inline bool CallsiteValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CALLSITE;
}

RQ_ALWAYS_INLINE ContextualType::ContextualType(rq::Opcode opcode)
    : Contextual(opcode) {
  RQ_ASSERT(rq::getIsContextualType(opcode), "not contextual type");
}

[[nodiscard]] inline bool
ContextualType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextualType(entity.getOpcode());
}

RQ_ALWAYS_INLINE VoidType::VoidType()
    : ContextualType(rq::Opcode::SY_VOID_TYPE) {}

[[nodiscard]] inline bool VoidType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VOID_TYPE;
}

RQ_ALWAYS_INLINE NoReturnType::NoReturnType()
    : ContextualType(rq::Opcode::SY_NO_RETURN_TYPE) {}

[[nodiscard]] inline bool NoReturnType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_NO_RETURN_TYPE;
}

RQ_ALWAYS_INLINE
ExpressionAttributeType::ExpressionAttributeType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {
  RQ_ASSERT(rq::getIsExpressionAttributeType(opcode),
            "not expression attribute type");
}

[[nodiscard]] inline bool
ExpressionAttributeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsExpressionAttributeType(entity.getOpcode());
}

RQ_ALWAYS_INLINE AnchorType::AnchorType()
    : ExpressionAttributeType(rq::Opcode::SY_ANCHOR_TYPE) {}

[[nodiscard]] inline bool AnchorType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ANCHOR_TYPE;
}

RQ_ALWAYS_INLINE OpaqueType::OpaqueType()
    : ExpressionAttributeType(rq::Opcode::SY_OPAQUE_TYPE) {}

[[nodiscard]] inline bool OpaqueType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_OPAQUE_TYPE;
}

RQ_ALWAYS_INLINE GlobalType::GlobalType()
    : ExpressionAttributeType(rq::Opcode::SY_GLOBAL_TYPE) {}

[[nodiscard]] inline bool GlobalType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_GLOBAL_TYPE;
}

RQ_ALWAYS_INLINE AccessType::AccessType()
    : ExpressionAttributeType(rq::Opcode::SY_ACCESS_TYPE) {}

[[nodiscard]] inline bool AccessType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ACCESS_TYPE;
}

RQ_ALWAYS_INLINE PartialMutateType::PartialMutateType()
    : ExpressionAttributeType(rq::Opcode::SY_PARTIAL_MUTATE_TYPE) {}

[[nodiscard]] inline bool
PartialMutateType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_PARTIAL_MUTATE_TYPE;
}

RQ_ALWAYS_INLINE StaticType::StaticType()
    : ExpressionAttributeType(rq::Opcode::SY_STATIC_TYPE) {}

[[nodiscard]] inline bool StaticType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_STATIC_TYPE;
}

RQ_ALWAYS_INLINE CaptureType::CaptureType()
    : ExpressionAttributeType(rq::Opcode::SY_CAPTURE_TYPE) {}

[[nodiscard]] inline bool CaptureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CAPTURE_TYPE;
}

RQ_ALWAYS_INLINE InlineType::InlineType()
    : ExpressionAttributeType(rq::Opcode::SY_INLINE_TYPE) {}

[[nodiscard]] inline bool InlineType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INLINE_TYPE;
}

RQ_ALWAYS_INLINE MangleType::MangleType()
    : ExpressionAttributeType(rq::Opcode::SY_MANGLE_TYPE) {}

[[nodiscard]] inline bool MangleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_MANGLE_TYPE;
}

RQ_ALWAYS_INLINE PackType::PackType()
    : ExpressionAttributeType(rq::Opcode::SY_PACK_TYPE) {}

[[nodiscard]] inline bool PackType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_PACK_TYPE;
}

RQ_ALWAYS_INLINE BranchTrendType::BranchTrendType()
    : ExpressionAttributeType(rq::Opcode::SY_BRANCH_TREND_TYPE) {}

[[nodiscard]] inline bool
BranchTrendType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BRANCH_TREND_TYPE;
}

RQ_ALWAYS_INLINE DepreciateType::DepreciateType()
    : ExpressionAttributeType(rq::Opcode::SY_DEPRECIATE_TYPE) {}

[[nodiscard]] inline bool
DepreciateType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_DEPRECIATE_TYPE;
}

RQ_ALWAYS_INLINE StableAddressType::StableAddressType()
    : ExpressionAttributeType(rq::Opcode::SY_STABLE_ADDRESS_TYPE) {}

[[nodiscard]] inline bool
StableAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_STABLE_ADDRESS_TYPE;
}

RQ_ALWAYS_INLINE VariadicType::VariadicType()
    : ExpressionAttributeType(rq::Opcode::SY_VARIADIC_TYPE) {}

[[nodiscard]] inline bool LocationType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_LOCATION_TYPE;
}

RQ_ALWAYS_INLINE LocationType::LocationType()
    : ExpressionAttributeType(rq::Opcode::SY_LOCATION_TYPE) {}

[[nodiscard]] inline bool VariadicType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VARIADIC_TYPE;
}

RQ_ALWAYS_INLINE TemplateType::TemplateType()
    : ExpressionAttributeType(rq::Opcode::SY_TEMPLATE_TYPE) {}

[[nodiscard]] inline bool TemplateType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_TEMPLATE_TYPE;
}

RQ_ALWAYS_INLINE ConstraintType::ConstraintType()
    : ExpressionAttributeType(rq::Opcode::SY_CONSTRAINT_TYPE) {}

[[nodiscard]] inline bool
ConstraintType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CONSTRAINT_TYPE;
}

RQ_ALWAYS_INLINE WeightType::WeightType()
    : ExpressionAttributeType(rq::Opcode::SY_WEIGHT_TYPE) {}

[[nodiscard]] inline bool WeightType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_WEIGHT_TYPE;
}

RQ_ALWAYS_INLINE RequireType::RequireType()
    : ExpressionAttributeType(rq::Opcode::SY_REQUIRE_TYPE) {}

[[nodiscard]] inline bool RequireType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_REQUIRE_TYPE;
}

RQ_ALWAYS_INLINE EnsureType::EnsureType()
    : ExpressionAttributeType(rq::Opcode::SY_ENSURE_TYPE) {}

[[nodiscard]] inline bool EnsureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ENSURE_TYPE;
}

RQ_ALWAYS_INLINE TypeAttributeType::TypeAttributeType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {}

[[nodiscard]] inline bool
TypeAttributeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsTypeAttributeType(entity.getOpcode());
}

RQ_ALWAYS_INLINE VarType::VarType()
    : TypeAttributeType(rq::Opcode::SY_VAR_TYPE) {}

[[nodiscard]] inline bool VarType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VAR_TYPE;
}

RQ_ALWAYS_INLINE VolatileType::VolatileType()
    : TypeAttributeType(rq::Opcode::SY_VOLATILE_TYPE) {}

[[nodiscard]] inline bool VolatileType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VOLATILE_TYPE;
}

RQ_ALWAYS_INLINE AtomicType::AtomicType()
    : TypeAttributeType(rq::Opcode::SY_ATOMIC_TYPE) {}

[[nodiscard]] inline bool AtomicType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ATOMIC_TYPE;
}

RQ_ALWAYS_INLINE NullTerminateType::NullTerminateType()
    : TypeAttributeType(rq::Opcode::SY_NULL_TERMINATE_TYPE) {}

[[nodiscard]] inline bool
NullTerminateType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_NULL_TERMINATE_TYPE;
}

RQ_ALWAYS_INLINE ReflectiveType::ReflectiveType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {}

[[nodiscard]] inline bool
ReflectiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsReflectiveType(entity.getOpcode());
}

RQ_ALWAYS_INLINE SymbolType::SymbolType()
    : ReflectiveType(rq::Opcode::SY_SYMBOL_TYPE) {}

[[nodiscard]] inline bool SymbolType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SYMBOL_TYPE;
}

RQ_ALWAYS_INLINE ExpressionType::ExpressionType()
    : ReflectiveType(rq::Opcode::SY_EXPRESSION_TYPE) {}

[[nodiscard]] inline bool
ExpressionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_EXPRESSION_TYPE;
}

RQ_ALWAYS_INLINE PlatformPrimitiveType::PlatformPrimitiveType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {
  RQ_ASSERT(rq::getIsPlatformPrimitiveType(opcode),
            "not platform primitive type");
}

[[nodiscard]] inline bool
PlatformPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsPlatformPrimitiveType(entity.getOpcode());
}

RQ_ALWAYS_INLINE BooleanType::BooleanType()
    : PlatformPrimitiveType(rq::Opcode::SY_BOOLEAN_TYPE) {}

[[nodiscard]] inline bool BooleanType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BOOLEAN_TYPE;
}

RQ_ALWAYS_INLINE HalfType::HalfType()
    : PlatformPrimitiveType(rq::Opcode::SY_HALF_TYPE) {}

[[nodiscard]] inline bool HalfType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_HALF_TYPE;
}

RQ_ALWAYS_INLINE SingleType::SingleType()
    : PlatformPrimitiveType(rq::Opcode::SY_SINGLE_TYPE) {}

[[nodiscard]] inline bool SingleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SINGLE_TYPE;
}

RQ_ALWAYS_INLINE DoubleType::DoubleType()
    : PlatformPrimitiveType(rq::Opcode::SY_DOUBLE_TYPE) {}

[[nodiscard]] inline bool DoubleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_DOUBLE_TYPE;
}

RQ_ALWAYS_INLINE QuadrupleType::QuadrupleType()
    : PlatformPrimitiveType(rq::Opcode::SY_QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool QuadrupleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_QUADRUPLE_TYPE;
}

RQ_ALWAYS_INLINE SignedIntegerType::SignedIntegerType()
    : PlatformPrimitiveType(rq::Opcode::SY_QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool
SignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_QUADRUPLE_TYPE;
}

RQ_ALWAYS_INLINE UnsignedIntegerType::UnsignedIntegerType()
    : PlatformPrimitiveType(rq::Opcode::SY_QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool
UnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_QUADRUPLE_TYPE;
}

RQ_ALWAYS_INLINE SignedAddressType::SignedAddressType()
    : PlatformPrimitiveType(rq::Opcode::SY_QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool
SignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_QUADRUPLE_TYPE;
}

RQ_ALWAYS_INLINE UnsignedAddressType::UnsignedAddressType()
    : PlatformPrimitiveType(rq::Opcode::SY_UNSIGNED_ADDRESS_TYPE) {}

[[nodiscard]] inline bool
UnsignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_UNSIGNED_ADDRESS_TYPE;
}

RQ_ALWAYS_INLINE StandardPrimitiveType::StandardPrimitiveType(rq::Opcode opcode)
    : SimpleSymbol(opcode) {
  RQ_ASSERT(rq::getIsStandardPrimitiveType(opcode), "not standard primitive");
}

[[nodiscard]] inline bool
StandardPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsStandardPrimitiveType(entity.getOpcode());
}

RQ_ALWAYS_INLINE Binary16Type::Binary16Type()
    : StandardPrimitiveType(rq::Opcode::SY_BINARY16_TYPE) {}

[[nodiscard]] inline bool Binary16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BINARY16_TYPE;
}

RQ_ALWAYS_INLINE Binary32Type::Binary32Type()
    : StandardPrimitiveType(rq::Opcode::SY_BINARY32_TYPE) {}

[[nodiscard]] inline bool Binary32Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BINARY32_TYPE;
}

RQ_ALWAYS_INLINE Binary64Type::Binary64Type()
    : StandardPrimitiveType(rq::Opcode::SY_BINARY64_TYPE) {}

[[nodiscard]] inline bool Binary64Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BINARY64_TYPE;
}

RQ_ALWAYS_INLINE Binary128Type::Binary128Type()
    : StandardPrimitiveType(rq::Opcode::SY_BINARY128_TYPE) {}

[[nodiscard]] inline bool Binary128Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BINARY64_TYPE;
}

RQ_ALWAYS_INLINE Bfloat16Type::Bfloat16Type()
    : StandardPrimitiveType(rq::Opcode::SY_BFLOAT16_TYPE) {}

[[nodiscard]] inline bool Bfloat16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_BFLOAT16_TYPE;
}

RQ_ALWAYS_INLINE AsciiType::AsciiType()
    : StandardPrimitiveType(rq::Opcode::SY_ASCII_TYPE) {}

[[nodiscard]] inline bool AsciiType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ASCII_TYPE;
}

RQ_ALWAYS_INLINE Utf8Type::Utf8Type()
    : StandardPrimitiveType(rq::Opcode::SY_UTF8_TYPE) {}

[[nodiscard]] inline bool Utf8Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_UTF8_TYPE;
}

RQ_ALWAYS_INLINE VariadicArgumentsType::VariadicArgumentsType()
    : SimpleSymbol(rq::Opcode::SY_VARIADIC_ARGUMENTS_TYPE_TYPE) {}

[[nodiscard]] inline bool
VariadicArgumentsType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_VARIADIC_ARGUMENTS_TYPE_TYPE;
}

RQ_ALWAYS_INLINE
ScaledPrimitiveType::ScaledPrimitiveType(rq::Opcode opcode,
                                         rq::ScaledIntegerKind kind,
                                         const rq::IntegerConstant &scalar,
                                         std::uint64_t synonym_id)
    : Symbol(opcode), _kind(kind), _scalar_ptr(&scalar),
      _synonym_id(synonym_id) {
  RQ_ASSERT(rq::getIsScaledPrimitive(opcode), "not scaled primitive");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledIntegerKind
ScaledPrimitiveType::getKind() const {
  return this->_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &
ScaledPrimitiveType::getScalar() const {
  return rq::dereferencePtr(this->_scalar_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t
ScaledPrimitiveType::getSynonymTypeId() const {
  return this->_synonym_id;
}

[[nodiscard]] inline bool
ScaledPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsScaledPrimitive(entity.getOpcode());
}

inline void ScaledPrimitiveType::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileScaledPrimitiveType(id, this->getKind(), this->getScalar(),
                                 this->getSynonymTypeId());
}

RQ_ALWAYS_INLINE void profileScaledPrimitiveType(
    llvm::FoldingSetNodeID &id, rq::ScaledIntegerKind kind,
    const rq::IntegerConstant &scalar, std::uint64_t synonum_id) {
  id.AddInteger(rq::getUnderlying(kind));
  id.AddPointer(&scalar);
  id.AddInteger(synonum_id);
}

RQ_ALWAYS_INLINE
ScaledSignedIntegerType::ScaledSignedIntegerType(
    rq::ScaledIntegerKind kind, const rq::IntegerConstant &scalar,
    std::uint64_t synonym_id)
    : ScaledPrimitiveType(rq::Opcode::SY_SCALED_SIGNED_INTEGER_TYPE, kind,
                          scalar, synonym_id) {}

[[nodiscard]] inline bool
ScaledSignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SCALED_SIGNED_INTEGER_TYPE;
}

RQ_ALWAYS_INLINE
ScaledUnsignedIntegerType::ScaledUnsignedIntegerType(
    rq::ScaledIntegerKind kind, const rq::IntegerConstant &scalar,
    std::uint64_t synonym_id)
    : ScaledPrimitiveType(rq::Opcode::SY_SCALED_UNSIGNED_INTEGER_TYPE, kind,
                          scalar, synonym_id) {}

[[nodiscard]] inline bool
ScaledUnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SCALED_UNSIGNED_INTEGER_TYPE;
}

RQ_ALWAYS_INLINE Subtype::Subtype(rq::Opcode opcode, rq::SymbolConstant &child)
    : Symbol(opcode), _child_ptr(&child) {
  RQ_ASSERT(rq::getIsSubtype(opcode), "not subtype");
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
  return rq::getIsSubtype(entity.getOpcode());
}

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(rq::SymbolConstant &child,
                                            const rq::IntegerConstant &count)
    : Subtype(rq::Opcode::SY_ARRAY_SUBTYPE, child), _count_ptr(&count) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &
ArraySubtype::getCount() const {
  return rq::dereferencePtr(this->_count_ptr);
}

[[nodiscard]] inline bool ArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ARRAY_SUBTYPE;
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileArraySubtype(id, this->getChild(), this->getCount());
}

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &id,
                                          const rq::SymbolConstant &child,
                                          const rq::IntegerConstant &count) {
  id.AddPointer(&child);
  id.AddPointer(&count);
}

RQ_ALWAYS_INLINE UncountedSubtype::UncountedSubtype(rq::Opcode opcode,
                                                    rq::SymbolConstant &child)
    : Subtype(opcode, child) {
  RQ_ASSERT(rq::getIsUncountedSubtype(opcode), "not uncounted subtype");
}

[[nodiscard]] inline bool
UncountedSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsUncountedSubtype(entity.getOpcode());
}

inline void UncountedSubtype::Profile(llvm::FoldingSetNodeID &id) const {
  return rq::profileUncountedSubtype(id, this->getChild());
}

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &id,
                                              const rq::SymbolConstant &child) {
  id.AddPointer(&child);
}

RQ_ALWAYS_INLINE ReferenceSubtype::ReferenceSubtype(rq::SymbolConstant &child)
    : UncountedSubtype(rq::Opcode::SY_REFERENCE_SUBTYPE, child) {}

[[nodiscard]] inline bool
ReferenceSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_REFERENCE_SUBTYPE;
}

RQ_ALWAYS_INLINE PointerSubtype::PointerSubtype(rq::SymbolConstant &child)
    : UncountedSubtype(rq::Opcode::SY_POINTER_SUBTYPE, child) {}

[[nodiscard]] inline bool
PointerSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_POINTER_SUBTYPE;
}

RQ_ALWAYS_INLINE FatPointerSubtype::FatPointerSubtype(rq::SymbolConstant &child)
    : UncountedSubtype(rq::Opcode::SY_FAT_POINTER_SUBTYPE, child) {}

[[nodiscard]] inline bool
FatPointerSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_FAT_POINTER_SUBTYPE;
}

RQ_ALWAYS_INLINE InferenceCountArraySubtype::InferenceCountArraySubtype(
    rq::SymbolConstant &child)
    : UncountedSubtype(rq::Opcode::SY_INFERENCE_COUNT_ARRAY_SUBTYPE, child) {}

[[nodiscard]] inline bool
InferenceCountArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INFERENCE_COUNT_ARRAY_SUBTYPE;
}

RQ_ALWAYS_INLINE ModuleFactory::ModuleFactory(rq::ModuleKind kind)
    : _module_kind(kind) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind ModuleFactory::getKind() const {
  return this->_module_kind;
}

void ModuleFactory::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
ModuleFactory::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
ModuleFactory::getExpressionPtr() {
  return this->_expression_ptr;
}

RQ_ALWAYS_INLINE void ModuleFactory::setPath(llvm::StringRef path) {
  RQ_ASSERT(this->_path.empty(), "path already set");
  this->_path = path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleFactory::getPath() const {
  RQ_ASSERT(this->_path.empty(), "path not set");
  return this->_path;
}

RQ_ALWAYS_INLINE void ModuleFactory::setBuffer(llvm::MemoryBufferRef &&buffer) {
  this->_buffer = std::move(buffer);
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &
ModuleFactory::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::MemoryBufferRef &
ModuleFactory::getBuffer() {
  return this->_buffer;
}

RQ_ALWAYS_INLINE Module::Module(rq::ModuleFactory &factory)
    : Symbol(rq::Opcode::SY_MODULE), _module_kind(factory.getKind()),
      _expression_ptr(factory.getExpressionPtr()), _path(factory.getPath()),
      _buffer(factory.getBuffer()) {
  RQ_ASSERT(factory.getExpressionPtr() != nullptr, "expression not set");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind Module::getModuleKind() const {
  return this->_module_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getSourceText() const {
  return this->_buffer.getBuffer();
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Module::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &
Module::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] inline bool Module::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_MODULE;
}

RQ_ALWAYS_INLINE Import::Import(rq::ExpressionFlags flags,
                                const rq::Expression &expression,
                                rq::Module &imported)
    : Symbol(rq::Opcode::SY_IMPORT), _expression_flags(flags),
      _expression_ptr(&expression), _imported_ptr(&imported) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
Import::getExpressionFlags() const {
  return this->_expression_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &Import::getModule() const {
  return rq::dereferencePtr(this->_imported_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &Import::getModule() {
  return rq::dereferencePtr(this->_imported_ptr);
}

[[nodiscard]] inline bool Import::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_IMPORT;
}

RQ_ALWAYS_INLINE
ConcatenatedList::ConcatenatedList(rq::ConcatenatedListItem &first_item)
    : Symbol(rq::Opcode::SY_CONCATINATED_LIST_TYPE),
      _first_item_ptr(&first_item) {}

[[nodiscard]] inline bool
ConcatenatedList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CONCATINATED_LIST_TYPE;
}

inline void ConcatenatedList::Profile(llvm::FoldingSetNodeID &id) const {
  const rq::ConcatenatedListItem &first_item =
      rq::dereferencePtr(this->_first_item_ptr);
  rq::profileConcatenatedList(id, first_item);
}

inline void
profileConcatenatedList(llvm::FoldingSetNodeID &id,
                        const rq::ConcatenatedListItem &first_item) {
  id.AddPointer(&first_item);
}

RQ_ALWAYS_INLINE ArithmeticSequence::ArithmeticSequence(
    rq::Opcode opcode, rq::SymbolConstant &child,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step)
    : Symbol(opcode), _child_ptr(&child), _condition(condition), _step(step) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
ArithmeticSequence::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
ArithmeticSequence::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
ArithmeticSequence::getCondition() const {
  return this->_condition;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep
ArithmeticSequence::getStep() const {
  return this->_step;
}

[[nodiscard]] inline bool
ArithmeticSequence::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsArithmeticSequence(entity.getOpcode());
}

inline void ArithmeticSequence::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileArithmeticSequence(id, this->getOpcode(), this->getChild(),
                                this->getCondition(), this->getStep());
}

RQ_ALWAYS_INLINE void
profileArithmeticSequence(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                          const rq::SymbolConstant &child,
                          rq::ArithmeticSequenceCondition condition,
                          rq::ArithmeticSequenceStep step) {
  id.AddInteger(rq::getUnderlying(opcode));
  id.AddPointer(&child);
  id.AddInteger(rq::getUnderlying(condition));
  id.AddInteger(rq::getUnderlying(step));
}

RQ_ALWAYS_INLINE
ArithmeticInterval::ArithmeticInterval(
    rq::SymbolConstant &child, rq::ArithmeticSequenceCondition condition)
    : ArithmeticSequence(rq::Opcode::SY_ARITHMETIC_INTERVAL, child, condition,
                         rq::ArithmeticSequenceStep::NONE) {
  RQ_ASSERT(condition != rq::ArithmeticSequenceCondition::NONE,
            "condition is none");
}

[[nodiscard]] inline bool
ArithmeticInterval::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ARITHMETIC_INTERVAL;
}

RQ_ALWAYS_INLINE
InfiniteArithmeticSequence::InfiniteArithmeticSequence(
    rq::SymbolConstant &child, rq::ArithmeticSequenceStep step)
    : ArithmeticSequence(rq::Opcode::SY_INFINITE_ARITHMETIC_SEQUENCE, child,
                         rq::ArithmeticSequenceCondition::NONE, step) {
  RQ_ASSERT(step != rq::ArithmeticSequenceStep::NONE, "step is none");
}

[[nodiscard]] inline bool
InfiniteArithmeticSequence::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INFINITE_ARITHMETIC_SEQUENCE;
}

RQ_ALWAYS_INLINE
FiniteArithmeticSequence::FiniteArithmeticSequence(
    rq::SymbolConstant &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step)
    : ArithmeticSequence(rq::Opcode::SY_INFINITE_ARITHMETIC_SEQUENCE, child,
                         condition, step) {
  RQ_ASSERT(condition != rq::ArithmeticSequenceCondition::NONE,
            "condition is none");
  RQ_ASSERT(step != rq::ArithmeticSequenceStep::NONE, "step is none");
}

[[nodiscard]] inline bool
FiniteArithmeticSequence::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_INFINITE_ARITHMETIC_SEQUENCE;
}

RQ_ALWAYS_INLINE
LocalDeclaration::LocalDeclaration(rq::Opcode opcode, llvm::StringRef name,
                                   const rq::Expression *name_expression_ptr,
                                   rq::SymbolTable &containing_table,
                                   rq::SymbolTable &hosting_table)
    : Symbol(opcode), _name(name), _name_expression_ptr(name_expression_ptr),
      _containing_table_ptr(&containing_table),
      _hosting_table_ptr(&hosting_table) {
  RQ_ASSERT(rq::getIsLocalDeclaration(opcode), "not local declaration");
  RQ_ASSERT(!name.empty() || name_expression_ptr != nullptr,
            "name must have expression if set");
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
LocalDeclaration::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
LocalDeclaration::getNameExpressionPtr() const {
  return this->_name_expression_ptr;
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

[[nodiscard]] inline bool
LocalDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLocalDeclaration(entity.getOpcode());
}

RQ_ALWAYS_INLINE
Label::Label(llvm::StringRef name, const rq::Expression &name_expression,
             rq::SymbolTable &containing_table, rq::Instruction &instruction)
    : LocalDeclaration(rq::Opcode::SY_LABEL, name, &name_expression,
                       containing_table, containing_table),
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
  return entity.getOpcode() == rq::Opcode::SY_LABEL;
}

RQ_ALWAYS_INLINE Anchor::Anchor(llvm::StringRef name,
                                const rq::Expression &name_expression,
                                rq::SymbolTable &containing_table,
                                rq::LocalStatement &local_table)
    : LocalDeclaration(rq::Opcode::SY_ANCHOR, name, &name_expression,
                       containing_table, containing_table),
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
  return entity.getOpcode() == rq::Opcode::SY_ANCHOR;
}

RQ_ALWAYS_INLINE
LocalVariable::LocalVariable(rq::Opcode opcode, llvm::StringRef name,
                             const rq::Expression *name_expression_ptr,
                             rq::SymbolTable &containing_table,
                             rq::SymbolTable &hosting_table,
                             rq::ExpressionFlags flags)
    : LocalDeclaration(opcode, name, name_expression_ptr, containing_table,
                       hosting_table),
      _expression_flags(flags) {
  RQ_ASSERT(rq::getIsLocalVariable(opcode), "not local variable");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
LocalVariable::getExpressionFlags() const {
  return this->_expression_flags;
}

RQ_ALWAYS_INLINE void LocalVariable::setType(rq::SymbolConstant &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

RQ_ALWAYS_INLINE void LocalVariable::replaceType(rq::SymbolConstant &type) {
  rq::replaceValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
LocalVariable::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *LocalVariable::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] inline bool LocalVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLocalVariable(entity.getOpcode());
}

RQ_ALWAYS_INLINE LocalDynamicVariable::LocalDynamicVariable(
    llvm::StringRef name, const rq::Expression &name_expression,
    rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
    rq::ExpressionFlags flags)
    : LocalVariable(rq::Opcode::SY_LOCAL_DYNAMIC_VARIABLE, name,
                    &name_expression, containing_table, hosting_table, flags) {}

[[nodiscard]] inline bool
LocalDynamicVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_LOCAL_DYNAMIC_VARIABLE;
}

RQ_ALWAYS_INLINE LocalStaticVariable::LocalStaticVariable(
    llvm::StringRef name, const rq::Expression &name_expression,
    rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
    rq::ExpressionFlags flags)
    : LocalVariable(rq::Opcode::SY_LOCAL_STATIC_VARIABLE, name,
                    &name_expression, containing_table, hosting_table, flags) {}

[[nodiscard]] const rq::SymbolicValue &LocalStaticVariable::getValue() const {
  return this->_value;
}

[[nodiscard]] rq::SymbolicValue &LocalStaticVariable::getValue() {
  return this->_value;
}

[[nodiscard]] inline bool
LocalStaticVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_LOCAL_STATIC_VARIABLE;
}

Parameter::Parameter(rq::Opcode opcode, rq::Parameter *next_ptr,
                     llvm::StringRef name, rq::SymbolConstant &type)
    : Symbol(opcode), _next_ptr(next_ptr), _name(name), _type_ptr(&type) {}

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
  return rq::getIsParameter(entity.getOpcode());
}

RQ_ALWAYS_INLINE
SymbolParameter::SymbolParameter(
    rq::Opcode opcode, rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
    bool is_positional, bool is_nonpositional, bool is_locked,
    const rq::Expression &expression, const rq::Expression &name_expression,
    const rq::Expression &type_expression,
    const rq::Expression *default_value_expression_ptr)
    : Parameter(opcode, next_ptr, name, type), _is_positional(is_positional),
      _is_nonpositional(is_nonpositional), _is_locked(is_locked),
      _expression_flags(expression_flags),
      _containing_table_ptr(&containing_table),
      _hosting_table_ptr(&hosting_table), _expression_ptr(&expression),
      _name_expression_ptr(&name_expression),
      _type_expression_ptr(&type_expression),
      _default_value_expression_ptr(default_value_expression_ptr) {
  RQ_ASSERT(rq::getIsSymbolParameter(opcode), "not symbol parameter");
}

[[nodiscard]] const rq::SymbolParameter *
SymbolParameter::getNextSymbolParameterPtr() const {
  return llvm::cast<rq::SymbolParameter>(this->getNextParameterPtr());
}

[[nodiscard]] rq::SymbolParameter *
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
SymbolParameter::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolParameter::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
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

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameter::getNameExpression() const {
  return rq::dereferencePtr(this->_name_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameter::getTypeExpression() const {
  return rq::dereferencePtr(this->_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
SymbolParameter::getDefaultValueExpressionPtr() const {
  return this->_default_value_expression_ptr;
}

[[nodiscard]] inline bool
SymbolParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbolParameter(entity.getOpcode());
}

RQ_ALWAYS_INLINE SignatureParameter::SignatureParameter(
    rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
    bool is_positional, bool is_nonpositional, bool is_locked,
    const rq::Expression &expression, const rq::Expression &name_expression,
    const rq::Expression &type_expression,
    const rq::Expression *default_value_expression_ptr)
    : SymbolParameter(rq::Opcode::SY_SIGNATURE_PARAMETER, next_ptr, name, type,
                      containing_table, hosting_table, expression_flags,
                      is_positional, is_nonpositional, is_locked, expression,
                      name_expression, type_expression,
                      default_value_expression_ptr) {}

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
  return entity.getOpcode() == rq::Opcode::SY_SIGNATURE_PARAMETER;
}

RQ_ALWAYS_INLINE LayoutParameter::LayoutParameter(
    rq::SymbolParameter *next_ptr, llvm::StringRef name,
    rq::SymbolConstant &type, rq::SymbolTable &containing_table,
    rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
    bool is_positional, bool is_nonpositional, bool is_locked,
    const rq::Expression &expression, const rq::Expression &name_expression,
    const rq::Expression &type_expression,
    const rq::Expression *default_value_expression_ptr)
    : SymbolParameter(rq::Opcode::SY_LAYOUT_PARAMETER, next_ptr, name, type,
                      containing_table, hosting_table, expression_flags,
                      is_positional, is_nonpositional, is_locked, expression,
                      name_expression, type_expression,
                      default_value_expression_ptr) {}

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
  return entity.getOpcode() == rq::Opcode::SY_LAYOUT_PARAMETER;
}

RQ_ALWAYS_INLINE
TypeParameter::TypeParameter(rq::Opcode opcode, rq::TypeParameter *next_ptr,
                             llvm::StringRef name, rq::SymbolConstant &type,
                             unsigned location, bool is_positional)
    : Parameter(opcode, next_ptr, name, type), _location(location),
      _is_positional(is_positional) {
  RQ_ASSERT(rq::getIsTypeParameter(opcode), "not type parameter");
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
  return rq::getIsTypeParameter(entity.getOpcode());
}

inline void TypeParameter::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileTypeParameter(id, this->getOpcode(), this->getName(),
                           this->getType(), this->getLocation(),
                           this->getIsPositional());
}

RQ_ALWAYS_INLINE void
profileTypeParameter(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                     llvm::StringRef name, const rq::SymbolConstant &type,
                     unsigned location, bool is_positional) {
  id.AddInteger(rq::getUnderlying(opcode));
  id.AddString(name);
  id.AddPointer(&type);
  id.AddInteger(location);
  id.AddBoolean(is_positional);
}

RQ_ALWAYS_INLINE ProcedureParameter::ProcedureParameter(
    rq::TypeParameter *next_ptr, llvm::StringRef name, rq::SymbolConstant &type,
    unsigned location)
    : TypeParameter(rq::Opcode::SY_PROCEDURE_PARAMETER, next_ptr, name, type,
                    location, name.empty()) {}

[[nodiscard]] inline bool
ProcedureParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_PROCEDURE_PARAMETER;
}

RQ_ALWAYS_INLINE
TupleParameter::TupleParameter(rq::TypeParameter *next_ptr,
                               llvm::StringRef name, rq::SymbolConstant &type,
                               unsigned location, bool is_positional)
    : TypeParameter(rq::Opcode::SY_TUPLE_PARAMETER, next_ptr, name, type,
                    location, is_positional) {}

[[nodiscard]] inline bool
TupleParameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_TUPLE_PARAMETER;
}

RQ_ALWAYS_INLINE
ParameterList::ParameterList(rq::Opcode opcode,
                             rq::Parameter *first_parameter_ptr,
                             unsigned parameter_count,
                             unsigned positional_parameter_count,
                             unsigned nonpositional_parameter_count)
    : Symbol(opcode), _first_parameter_ptr(first_parameter_ptr),
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
  return rq::getIsParameterList(entity.getOpcode());
}

RQ_ALWAYS_INLINE
SymbolParameterInfo::SymbolParameterInfo(
    llvm::StringRef name, rq::SymbolConstant &type,
    const rq::Expression *name_expression_ptr,
    rq::ExpressionFlags expression_flags, bool is_positional,
    bool is_nonpositional, bool is_locked,
    const rq::Expression &type_expression,
    const rq::Expression *default_value_expression_ptr)
    : _name(name), _type_ptr(&type), _name_expression_ptr(name_expression_ptr),
      _expression_flags(expression_flags), _is_positional(is_positional),
      _is_nonpositional(is_nonpositional), _is_locked(is_locked),
      _type_expression_ptr(&type_expression),
      _default_value_expression_ptr(default_value_expression_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
SymbolParameterInfo::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
SymbolParameterInfo::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &
SymbolParameterInfo::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
SymbolParameterInfo::getNameExpressionPtr() const {
  return this->_name_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
SymbolParameterInfo::getExpressionFlags() const {
  return this->_expression_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameterInfo::getIsPositional() const {
  return this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameterInfo::getIsNonpositional() const {
  return this->_is_nonpositional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool SymbolParameterInfo::getIsLocked() const {
  return this->_is_locked;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
SymbolParameterInfo::getTypeExpression() const {
  return rq::dereferencePtr(this->_type_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
SymbolParameterInfo::getDefaultValueExpressionPtr() const {
  return this->_default_value_expression_ptr;
}

SymbolParameterListFactory::SymbolParameterListFactory(
    rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table)
    : _containing_table_ptr(&containing_table),
      _hosting_table_ptr(&hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolParameterListFactory::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolParameterListFactory::getHostingTable() {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameterListFactory::getIsPositional() const {
  return this->_is_positional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameterListFactory::getIsNonpositional() const {
  return this->_is_nonpositional;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolParameterListFactory::getIsLocked() const {
  return this->_is_locked;
}

RQ_ALWAYS_INLINE void SymbolParameterListFactory::markPositionalEnd() {
  RQ_ASSERT(this->_is_positional, "already not positional");
  this->_is_positional = false;
}

RQ_ALWAYS_INLINE void SymbolParameterListFactory::markNonpositionalBegin() {
  RQ_ASSERT(!this->_is_nonpositional, "already nonpositional");
  this->_is_positional = false;
}

RQ_ALWAYS_INLINE void SymbolParameterListFactory::markLockedBegin() {
  RQ_ASSERT(!this->_is_positional, "locking positional parameter");
  RQ_ASSERT(!this->_is_locked, "already locked");
  this->_is_positional = false;
}

RQ_ALWAYS_INLINE void SymbolParameterListFactory::addParameter(
    llvm::StringRef name, const rq::Expression *name_expression_ptr,
    rq::ExpressionFlags expression_flags, const rq::Expression &type_expression,
    const rq::Expression *default_value_expression_ptr) {
  this->_infos.emplace_back(name, name_expression_ptr, expression_flags,
                            this->getIsPositional(), this->getIsNonpositional(),
                            this->getIsLocked(), type_expression,
                            default_value_expression_ptr);
}

RQ_ALWAYS_INLINE SymbolParameterList::SymbolParameterList(
    rq::Opcode opcode, rq::SymbolParameter *first_parameter_ptr,
    unsigned parameter_count, unsigned positional_parameter_count,
    unsigned nonpositional_parameter_count, unsigned locked_parameter_count)
    : ParameterList(opcode, first_parameter_ptr, parameter_count,
                    positional_parameter_count, nonpositional_parameter_count),
      _locked_parameter_count(locked_parameter_count) {
  RQ_ASSERT(rq::getIsSymbolParameterList(opcode), "not symbol parameter list");
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
  return rq::getIsSymbolParameterList(entity.getOpcode());
}

RQ_ALWAYS_INLINE Signature::Signature(
    rq::SignatureParameter *first_parameter_ptr, unsigned parameter_count,
    unsigned positional_parameter_count, unsigned nonpositional_parameter_count,
    unsigned locked_parameter_count, rq::SymbolConstant &return_type,
    rq::SymbolConstant *reciever_type_ptr,
    const rq::Expression *precondition_expression_ptr,
    const rq::Expression *postcondition_expression_ptr)
    : SymbolParameterList(rq::Opcode::SY_SIGNATURE, first_parameter_ptr,
                          parameter_count, positional_parameter_count,
                          nonpositional_parameter_count,
                          locked_parameter_count),
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

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Signature::getPostconditionExpressionPtr() const {
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
  return entity.getOpcode() == rq::Opcode::SY_SIGNATURE;
}

RQ_ALWAYS_INLINE Layout::Layout(rq::SymbolParameter *first_parameter_ptr,
                                unsigned parameter_count,
                                unsigned positional_parameter_count,
                                unsigned nonpositional_parameter_count,
                                unsigned locked_parameter_count)
    : SymbolParameterList(rq::Opcode::SY_LAYOUT, first_parameter_ptr,
                          parameter_count, positional_parameter_count,
                          nonpositional_parameter_count,
                          locked_parameter_count) {}

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
  return entity.getOpcode() == rq::Opcode::SY_LAYOUT;
}

RQ_ALWAYS_INLINE
TypeParameterList::TypeParameterList(rq::Opcode opcode,
                                     rq::TypeParameter *first_parameter_ptr,
                                     unsigned parameter_count,
                                     unsigned positional_parameter_count,
                                     unsigned nonpositional_parameter_count)
    : ParameterList(opcode, first_parameter_ptr, parameter_count,
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
  return rq::getIsTypeParameterList(entity.getOpcode());
}

RQ_ALWAYS_INLINE
ProcedureType::ProcedureType(rq::TypeParameter *first_parameter_ptr,
                             unsigned parameter_count,
                             unsigned positional_parameter_count,
                             unsigned nonpositional_parameter_count,
                             rq::SymbolConstant &return_type,
                             rq::SymbolConstant *reciever_type_ptr)
    : TypeParameterList(rq::Opcode::SY_PROCEDURE_TYPE, first_parameter_ptr,
                        parameter_count, positional_parameter_count,
                        nonpositional_parameter_count),
      _return_type_ptr(&return_type), _reciever_type_ptr(reciever_type_ptr) {}

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
  return entity.getOpcode() == rq::Opcode::SY_PROCEDURE_TYPE;
}

inline void
ProcedureType::Profile(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                       const rq::ProcedureParameter *first_parameter_ptr,
                       const rq::SymbolConstant &return_type,
                       const rq::SymbolConstant *reciever_type_ptr) const {
  id.AddInteger(rq::getUnderlying(opcode));
  id.AddPointer(first_parameter_ptr);
  id.AddPointer(&return_type);
  id.AddPointer(reciever_type_ptr);
}

inline void
profileProcedureType(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                     const rq::ProcedureParameter *first_parameter_ptr,
                     const rq::SymbolConstant &return_type,
                     const rq::SymbolConstant *reciever_type_ptr) {
  id.AddInteger(rq::getUnderlying(opcode));
  id.AddPointer(first_parameter_ptr);
  id.AddPointer(&return_type);
  id.AddPointer(reciever_type_ptr);
}

RQ_ALWAYS_INLINE TupleType::TupleType(rq::TypeParameter *first_parameter_ptr,
                                      unsigned parameter_count,
                                      unsigned positional_parameter_count,
                                      unsigned nonpositional_parameter_count,
                                      unsigned type_keyed_parameter_count)
    : TypeParameterList(rq::Opcode::SY_TUPLE_TYPE, first_parameter_ptr,
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
  return entity.getOpcode() == rq::Opcode::SY_TUPLE_TYPE;
}

inline void TupleType::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileTupleType(
      id, llvm::cast<rq::TupleParameter>(this->_first_parameter_ptr));
}

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &id,
                 const rq::TupleParameter *first_parameter_ptr) {
  id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE Placement::Placement(rq::Procedure &procedure)
    : Symbol(rq::Opcode::SY_PLACEMENT_TYPE), _procedure_ptr(&procedure) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &
Placement::getProcedure() const {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &Placement::getProcedure() {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] inline bool Placement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_PLACEMENT_TYPE;
}

inline void Placement::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profilePlacement(id, this->getProcedure());
}

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &id,
                                       const rq::Procedure &procedure) {
  id.AddPointer(&procedure);
}

RQ_ALWAYS_INLINE
CompositionComponent::CompositionComponent(rq::Interface &interface,
                                           rq::CompositionComponent *next_ptr)
    : _interface_ptr(&interface), _next_ptr(next_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface &
CompositionComponent::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Interface &
CompositionComponent::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent *
CompositionComponent::getNextComponentPtr() const {
  return this->_next_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent *
CompositionComponent::getNextComponentPtr() {
  return this->_next_ptr;
}

inline void CompositionComponent::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileCompositionComponent(id, this->getInterface(), this->_next_ptr);
}

RQ_ALWAYS_INLINE void profileCompositionComponent(
    llvm::FoldingSetNodeID &id, const rq::Interface &interface,
    const rq::CompositionComponent *next_component_ptr) {
  id.AddPointer(&interface);
  id.AddPointer(next_component_ptr);
}

RQ_ALWAYS_INLINE
CompositionType::CompositionType(rq::CompositionComponent &first_component)
    : Symbol(rq::Opcode::SY_COMPOSITION_TYPE),
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
  return entity.getOpcode() == rq::Opcode::SY_COMPOSITION_TYPE;
}

inline void CompositionType::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileComposition(id, this->getFirstComponent());
}

inline void
profileComposition(llvm::FoldingSetNodeID &id,
                   const rq::CompositionComponent &first_component) {
  id.AddPointer(&first_component);
}

RQ_ALWAYS_INLINE SynonymType::SynonymType(rq::Symbol &original)
    : Symbol(rq::Opcode::SY_SYNONYM_TYPE), _original_ptr(&original) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
SynonymType::getOriginal() const {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &SynonymType::getOriginal() {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] inline bool SynonymType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SYNONYM_TYPE;
}

RQ_ALWAYS_INLINE SymbolTable::SymbolTable(rq::Opcode opcode,
                                          rq::SymbolTable *containing_table_ptr)
    : Symbol(opcode), _containing_table_ptr(containing_table_ptr) {
  RQ_ASSERT(rq::getIsSymbolTable(opcode), "not symbol table");
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

[[nodiscard]] inline bool SymbolTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbolTable(entity.getOpcode());
}

RQ_ALWAYS_INLINE Top::Top() : SymbolTable(rq::Opcode::SY_TOP, nullptr) {}

[[nodiscard]] inline bool Top::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_TOP;
}

RQ_ALWAYS_INLINE LocalStatement::LocalStatement(
    rq::Opcode opcode, rq::SymbolTable &containing_table,
    rq::Expression &expression, rq::ExpressionFlags flags)
    : SymbolTable(opcode, &containing_table), _expression_ptr(&expression),
      _flags(flags) {
  RQ_ASSERT(rq::getIsLocalStatement(opcode), "not local statement");
}

[[nodiscard]] inline bool
LocalStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLocalStatement(entity.getOpcode());
}

RQ_ALWAYS_INLINE IfStatement::IfStatement(rq::SymbolTable &containing_table,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_IF_STATEMENT, containing_table, expression,
                     flags) {}

[[nodiscard]] inline bool IfStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_IF_STATEMENT;
}

RQ_ALWAYS_INLINE
ElseIfStatement::ElseIfStatement(rq::SymbolTable &containing_table,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_ELSE_IF_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
ElseIfStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ELSE_IF_STATEMENT;
}

RQ_ALWAYS_INLINE
ElseStatement::ElseStatement(rq::SymbolTable &containing_table,
                             rq::Expression &expression,
                             rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_ELSE_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool ElseStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ELSE_STATEMENT;
}

RQ_ALWAYS_INLINE
MatchStatement::MatchStatement(rq::SymbolTable &containing_table,
                               rq::Expression &expression,
                               rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_MATCH_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
MatchStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_MATCH_STATEMENT;
}

RQ_ALWAYS_INLINE
SwitchStatement::SwitchStatement(rq::SymbolTable &containing_table,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_SWITCH_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
SwitchStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SWITCH_STATEMENT;
}

RQ_ALWAYS_INLINE
CaseStatement::CaseStatement(rq::SymbolTable &containing_table,
                             rq::Expression &expression,
                             rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_CASE_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool CaseStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CASE_STATEMENT;
}

RQ_ALWAYS_INLINE
WithStatement::WithStatement(rq::SymbolTable &containing_table,
                             rq::Expression &expression,
                             rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_WITH_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool WithStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_WITH_STATEMENT;
}

RQ_ALWAYS_INLINE
DefaultStatement::DefaultStatement(rq::SymbolTable &containing_table,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_DEFAULT_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
DefaultStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_DEFAULT_STATEMENT;
}

RQ_ALWAYS_INLINE
ForStatement::ForStatement(rq::SymbolTable &containing_table,
                           rq::Expression &expression,
                           rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_FOR_STATEMENT, containing_table, expression,
                     flags) {}

[[nodiscard]] inline bool ForStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_FOR_STATEMENT;
}

RQ_ALWAYS_INLINE
WhileStatement::WhileStatement(rq::SymbolTable &containing_table,
                               rq::Expression &expression,
                               rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_WHILE_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
WhileStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_WHILE_STATEMENT;
}

RQ_ALWAYS_INLINE
SpinStatement::SpinStatement(rq::SymbolTable &containing_table,
                             rq::Expression &expression,
                             rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_SPIN_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool SpinStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SPIN_STATEMENT;
}

RQ_ALWAYS_INLINE
WeaveStatement::WeaveStatement(rq::SymbolTable &containing_table,
                               rq::Expression &expression,
                               rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_WEAVE_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
WeaveStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_WEAVE_STATEMENT;
}

RQ_ALWAYS_INLINE
ScopeStatement::ScopeStatement(rq::SymbolTable &containing_table,
                               rq::Expression &expression,
                               rq::ExpressionFlags flags)
    : LocalStatement(rq::Opcode::SY_SCOPE_STATEMENT, containing_table,
                     expression, flags) {}

[[nodiscard]] inline bool
ScopeStatement::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_SCOPE_STATEMENT;
}

RQ_ALWAYS_INLINE NamedTable::NamedTable(rq::Opcode opcode,
                                        rq::SymbolTable &containing_table,
                                        llvm::StringRef name)
    : SymbolTable(opcode, &containing_table), _name(name) {
  RQ_ASSERT(rq::getIsNamedTable(opcode), "not named table");
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
  return rq::getIsNamedTable(entity.getOpcode());
}

RQ_ALWAYS_INLINE
GlobalDeclaration::GlobalDeclaration(
    rq::Opcode opcode, rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, const rq::Expression &expression,
    const rq::Expression *name_expression_ptr, rq::ExpressionFlags flags)
    : NamedTable(opcode, containing_table, name),
      _hosting_table_ptr(&hosting_table), _expression_ptr(&expression),
      _name_expression_ptr(name_expression_ptr), _flags(flags) {
  RQ_ASSERT(rq::getIsGlobalDeclaration(opcode), "not global declaration");
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

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
GlobalDeclaration::getNameExpressionPtr() const {
  return this->_name_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
GlobalDeclaration::getExpressionFlags() {
  return this->_flags;
}

[[nodiscard]] inline bool
GlobalDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsGlobalDeclaration(entity.getOpcode());
}

RQ_ALWAYS_INLINE
ClassType::ClassType(rq::SymbolTable &containing_table, llvm::StringRef name,
                     rq::SymbolTable &hosting_table,
                     const rq::Expression &expression,
                     const rq::Expression &name_expression,
                     rq::ExpressionFlags flags)
    : GlobalDeclaration(rq::Opcode::SY_CLASS_TYPE, containing_table, name,
                        hosting_table, expression, &name_expression, flags) {}

[[nodiscard]] inline bool ClassType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_CLASS_TYPE;
}

RQ_ALWAYS_INLINE EnumerationType::EnumerationType(
    rq::SymbolTable &containing_table, llvm::StringRef name,
    rq::SymbolTable &hosting_table, const rq::Expression &expression,
    const rq::Expression &name_expression, rq::ExpressionFlags flags)
    : GlobalDeclaration(rq::Opcode::SY_ENUMERATION_TYPE, containing_table, name,
                        hosting_table, expression, &name_expression, flags) {}

[[nodiscard]] inline bool
EnumerationType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ENSURE_TYPE;
}

RQ_ALWAYS_INLINE
 Enumerator::Enumerator(rq::SymbolTable &containing_table, llvm::StringRef name,
             rq::SymbolTable &hosting_table, const rq::Expression &expression,
             const rq::Expression &name_expression, rq::ExpressionFlags flags) : GlobalDeclaration(rq::Opcode::SY_ENUMERATOR, )

 [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

} // namespace rq