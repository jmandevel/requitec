#pragma once

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getFlags(rq::Opcode opcode) {
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

  // CONTEXTUAL NAME
  case O::SY_NO_NAME:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL;

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
  case O::SY_ANCHORING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_VISIBILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_SCOPING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_AVAILABILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_PROPERTY_MUTABILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_EXPORTING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_GENERATION_TIME_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_CAPTURING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_INLINING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_MANGLING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_PACKING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_TEMPLATING_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_LIKELYHOOD_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_SUPPORT_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_ADDRESS_STABILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;
  case O::SY_VARIADICNESS_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL |
           OF::SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE | OF::SY_IS_TYPE;

  // TYPE ATTRIBUTES
  case O::SY_VARIABILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_VOLATILITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_ATOMICITY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_NULL_TERMINATION_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_PRECONDITION_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE |
           OF::SY_IS_TYPE;
  case O::SY_POSTCONDITION_TYPE:
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
  case O::SY_CHAR_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE |
           OF::SY_IS_TYPE | OF::SY_IS_CODEUNIT_TYPE;

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
  case O::SY_VARIADIC_ARGUMENTS_TYPE:
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
  case O::SY_INFERENCE_TYPE_COUNT_ARRAY_SUBTYPE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE |
           OF::SY_IS_TYPE;

  // MODULES
  case O::SY_MODULE:
    return OF::SYMBOL;

  // IMPORTS
  case O::SY_IMPORT:
    return OF::SYMBOL;

  // TUPLE TYPE
  case O::SY_TUPLE_TYPE:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // JUXTAPOSITIONAL LIST
  case O::SY_JUXTAPOSITIONAL_LIST_TYPE:
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
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_LOCAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // PARAMETERS => local variable
  case O::SY_STATIC_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_DYNAMIC_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // PARAMETER LISTS
  case O::SY_SIGNATURE:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST | OF::SY_IS_TYPE;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST | OF::SY_IS_TYPE;

  // PLACEMENTS
  case O::SY_PLACEMENT:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // COMPOSITIONS
  case O::SY_COMPOSITION:
    return OF::SYMBOL | OF::SY_IS_TYPE;

  // SYNONYMS
  case O::SY_SYNONYM:
    return OF::SYMBOL | OF::SY_IS_TYPE;

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

  // SYMBOL TABLES
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE;

  // LOCAL TABLES
  case O::SY_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ELSE_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ELSE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_MATCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INLINE_MATCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_SWITCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INLINE_SWITCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_CASE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_WITH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_DEFAULT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FOR:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_WHILE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_SPIN:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_WEAVE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INLINE_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // GLOBAL DECLARATION => symbol table
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE;
  case O::SY_CLASS:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_IS_TYPE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_IS_TYPE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE;
  case O::SY_INTERFACE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // GLOBAL VARIABLE => global declaration => symbol table
  case O::SY_GLOBAL_DYNAMIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_GLOBAL_VARIABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_GLOBAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_GLOBAL_VARIABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // RANGERS => global declaration => symbol table
  case O::SY_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_RANGER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_RANGER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // DESTRUCTOR =>  global declaration => symbol table
  case O::SY_DESTRUCTOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // PROCEDURES => global declaration => symbol table
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // TEMPLATES
  case O::SY_CLASS_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INTERFACE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONTEXTUAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualName(rq::Opcode opcode) {
  return opcode == rq::Opcode::SY_NO_NAME;
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
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_TYPE_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_TABLE);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpressionAttributes(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_EXPRESSION_ATTRIBUTES
}

RQ_ALWAYS_INLINE Entity::Entity(rq::Opcode opcode) : _opcode(opcode) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getOpcode() const {
  return this->_opcode;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags Entity::getOpcodeFlags() const {
  return rq::getFlags(this->getOpcode());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity) {
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

} // namespace rq