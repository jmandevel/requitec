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
  case O::SY_FLANK_TYPE:
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
    return OF::SYMBOL | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

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
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_LOCAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // PARAMETERS => local variable
  case O::SY_LAYOUT_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_SIGNATURE_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER | OF::SY_HAS_EXPRESSION_ATTRIBUTES;

  // PARAMETER LISTS
  case O::SY_SIGNATURE_TYPE:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST | OF::SY_IS_TYPE;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST;

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

  // LOCAL STATEMENTS
  case O::SY_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_MATCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SWITCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_CASE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WITH:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_DEFAULT:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_FOR:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WHILE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SPIN:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_WEAVE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TYPE_TABLE | OF::SY_LOCAL_STATEMENT |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;

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
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;

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
           OF::SY_RANGER | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;
  case O::SY_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_RANGER | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;

  // DESTRUCTOR =>  global declaration => symbol table
  case O::SY_DESTRUCTOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES | OF::SY_LOCAL_TABLE;

  // PROCEDURES => global declaration => symbol table
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TYPE_TABLE |
           OF::SY_PROCEDURE | OF::SY_HAS_EXPRESSION_ATTRIBUTES |
           OF::SY_LOCAL_TABLE;

  // TEMPLATES
  case O::SY_CLASS_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_ENUMERATION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_INTERFACE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE |
           OF::SY_HAS_EXPRESSION_ATTRIBUTES;

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
    return EF::FLANK | EF::EXPORT;
  case O::SY_LOCAL_DYNAMIC_VARIABLE:
    return EF::FLANK;
  case O::SY_LOCAL_STATIC_VARIABLE:
    return EF::FLANK | EF::STATIC;
  case O::SY_LAYOUT_PARAMETER:
    return EF::PUBLIC | EF::PARTIAL_MUTATE;
  case O::SY_SIGNATURE_PARAMETER:
    return EF::VARIADIC;
  case O::SY_IF:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_ELSE_IF:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_ELSE:
    return EF::ANCHOR | EF::STATIC | EF::LIKELY | EF::UNLIKELY;
  case O::SY_MATCH:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SWITCH:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_CASE:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WITH:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_DEFAULT:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_FOR:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WHILE:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SPIN:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_WEAVE:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_SCOPE:
    return EF::ANCHOR | EF::STATIC;
  case O::SY_CLASS:
    return EF::OPAQUE | EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::PACK | EF::DEPRECIATE | EF::EXPERIMENTAL | EF::STABLE_ADDRESS;
  case O::SY_ENUMERATION:
    return EF::OPAQUE | EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_INTERFACE:
    return EF::OPAQUE | EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::MANGLE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_GLOBAL_DYNAMIC_VARIABLE:
    return EF::OPAQUE | EF::FLANK | EF::GLOBAL | EF::EXPORT | EF::CAPTURE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_GLOBAL_STATIC_VARIABLE:
    return EF::FLANK | EF::GLOBAL | EF::EXPORT | EF::STATIC | EF::CAPTURE |
           EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_FORWARD_RANGER:
    return EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case O::SY_BACKWARD_RANGER:
    return EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL;
  case O::SY_DESTRUCTOR:
    return EF::FLANK | EF::CAPTURE | EF::DEPRECIATE | EF::EXPERIMENTAL;
  case O::SY_ENTRY:
    return EF::FLANK | EF::CAPTURE | EF::MANGLE;
  case O::SY_FUNCTION:
    return EF::OPAQUE | EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_METHOD:
    return EF::OPAQUE | EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_EXTENSION_METHOD:
    return EF::OPAQUE | EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::INLINE |
           EF::MANGLE;
  case O::SY_CLASS_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::PACK | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_ENUMERATION_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_INTERFACE_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_FUNCTION_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_METHOD_TEMPLATE:
    return EF::FLANK | EF::PUBLIC | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
           EF::EXPERIMENTAL | EF::TEMPLATE | EF::CONSTRAINT | EF::WEIGHT;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return EF::FLANK | EF::EXPORT | EF::CAPTURE | EF::INLINE | EF::DEPRECIATE |
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_LOCAL_STATEMENT);
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

inline void profileSimpleSymbol(llvm::FoldingSetNodeID &id, rq::Opcode opcode) {
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

RQ_ALWAYS_INLINE FlankType::FlankType()
    : ExpressionAttributeType(rq::Opcode::SY_FLANK_TYPE) {}

[[nodiscard]] inline bool FlankType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_FLANK_TYPE;
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

RQ_ALWAYS_INLINE RequireType::RequireType()
    : TypeAttributeType(rq::Opcode::SY_REQUIRE_TYPE) {}

[[nodiscard]] inline bool RequireType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_REQUIRE_TYPE;
}

RQ_ALWAYS_INLINE EnsureType::EnsureType()
    : TypeAttributeType(rq::Opcode::SY_ENSURE_TYPE) {}

[[nodiscard]] inline bool EnsureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getOpcode() == rq::Opcode::SY_ENSURE_TYPE;
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
  RQ_ASSERT(rq::getIsPlatformPrimitiveTypej(opcode),
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

} // namespace rq