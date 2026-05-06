#pragma once

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OF = rq::OpcodeFlags;
  switch (opcode) {
  case O::NONE:
    return OF::NONE;

  // LITERALS
  case O::SY_INTEGER_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
  case O::SY_FLOAT_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
  case O::SY_STRING_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
  case O::SY_CODEUNIT_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;

  // CONTEXTUAL NAME
  case O::SY_NO_NAME:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL;

  // CONTEXTUAL VALUE
  case O::SY_OUT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_THIS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_RESULT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_VALUE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_INDEX:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_DISCRIMINANT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_COMMAND_LINE_ARGUMENTS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
  case O::SY_CALLSITE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;

  // CONTEXTUAL TYPE
  case O::SY_INFERENCE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;
  case O::SY_VOID:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;
  case O::SY_NO_RETURN:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;

  // EXPRESSION ATTRIBUTES
  case O::SY_ANCHORING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_VISIBILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_SCOPING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_AVAILABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_PROPERTY_MUTABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_EXPORTING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_GENERATION_TIME:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_CAPTURING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_EVALUATION_TIME:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_INLINING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_MANGLING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_PACKING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_TEMPLATING:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_LIKELYHOOD:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_SUPPORT:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_ADDRESS_STABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;
  case O::SY_VARIADICNESS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_EXPRESSION_ATTRIBUTE_TYPE;

  // TYPE ATTRIBUTES
  case O::SY_VARIABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;
  case O::SY_VOLATILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;
  case O::SY_ATOMICITY:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;
  case O::SY_NULL_TERMINATION:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;
  case O::SY_PRECONDITION:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;
  case O::SY_POSTCONDITION:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_TYPE_ATTRIBUTE_TYPE;

  // REFLECTIVE
  case O::SY_SYMBOL:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_REFLECTIVE_TYPE;
  case O::SY_EXPRESSION:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_REFLECTIVE_TYPE;

  // PLATFORM PRIMITIVE
  case O::SY_BOOLEAN:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_HALF:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_SINGLE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_DOUBLE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_QUADRUPLE:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_SIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_UNSIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_SIGNED_INDEX:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_UNSIGNED_INDEX:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_SIGNED_ADDRESS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_UNSIGNED_ADDRESS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;
  case O::SY_CHAR:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_PLATFORM_PRIMITIVE_TYPE;

  // STANDARD PRIMITIVE TYPE
  case O::SY_BINARY16:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_BINARY32:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_BINARY64:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_BINARY128:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_BFLOAT16:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_ASCII:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;
  case O::SY_UTF8:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_STANDARD_PRIMITIVE_TYPE;

  // VARIADIC ARGUMENTS
  case O::SY_VARIADIC_ARGUMENTS:
    return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL;

  // SCALED PRIMITIVES
  case O::SY_SCALED_SIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_PRIMITIVE_TYPE;
  case O::SY_SCALED_UNSIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_PRIMITIVE_TYPE;

  // ARRAY
  case O::SY_ARRAY:
    return OF::SYMBOL | OF::SY_SUBTYPE;

  // UNCOUNTED SUBTYPES
  case O::SY_REFERENCE:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE;
  case O::SY_POINTER:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE;
  case O::SY_FAT_POINTER:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE;
  case O::SY_INFERENCE_COUNT_ARRAY:
    return OF::SYMBOL | OF::SY_SUBTYPE | OF::SY_UNCOUNTED_SUBTYPE;

  // MODULES
  case O::SY_MODULE:
    return OF::SYMBOL;

  // IMPORTS
  case O::SY_IMPORT:
    return OF::SYMBOL;

  // CONCATENATED STRING
  case O::SY_CONCATENATED_STRING:
    return OF::SYMBOL;

  // ARITHMETIC SEQUENCES
  case O::SY_ARITHMETIC_INTERVAL:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE;
  case O::SY_INFINITE_ARITHMETIC_SEQUENCE:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE;
  case O::SY_FINITE_ARITHMETIC_SEQUENCE:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE;

  // LOCAL DECLARATIONS
  case O::SY_LABEL:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION;
  case O::SY_ANCHOR:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION;

  // LOCAL VARIABLES
  case O::SY_LOCAL_DYNAMIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE;
  case O::SY_LOCAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE;

  // PARAMETERS => local variable
  case O::SY_STATIC_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER;
  case O::SY_DYNAMIC_PARAMETER:
    return OF::SYMBOL | OF::SY_LOCAL_DECLARATION | OF::SY_LOCAL_VARIABLE |
           OF::SY_PARAMETER;

  // PARAMETER LISTS
  case O::SY_SIGNATURE:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST;

  // PLACEMENTS
  case O::SY_PLACEMENT:
    return OF::SYMBOL;

  // COMPOSITIONS
  case O::SY_COMPOSITION:
    return OF::SYMBOL;

  // SYNONYMS
  case O::SY_SYNONYM:
    return OF::SYMBOL;

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
  case O::SY_GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_GLOBAL_STATIC_VARIABLE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;

  // SYMBOL TABLES
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE;

  // LOCAL TABLES
  case O::SY_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE_IF:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_ELSE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_MATCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_INLINE_MATCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_SWITCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_INLINE_SWITCH:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_CASE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_WITH:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_DEFAULT:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_FOR:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_WHILE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_SPIN:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_WEAVE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;
  case O::SY_INLINE_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_LOCAL_TABLE;

  // GLOBAL DECLARATION => symbol table
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;
  case O::SY_CLASS:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;
  case O::SY_ENUMERATION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;
  case O::SY_INTERFACE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;

  // GLOBAL VARIABLE => global declaration => symbol table
  case O::SY_GLOBAL_DYNAMIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_GLOBAL_VARIABLE;
  case O::SY_GLOBAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_GLOBAL_VARIABLE;

  // RANGERS => global declaration => symbol table
  case O::SY_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_RANGER;
  case O::SY_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_RANGER;

  // DESTRUCTOR =>  global declaration => symbol table
  case O::SY_DESTRUCTOR:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE;

  // PROCEDURES => global declaration => symbol table
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_PROCEDURE;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_PROCEDURE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_PROCEDURE;
  case O::SY_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_PROCEDURE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_SYMBOL_TABLE |
           OF::SY_PROCEDURE;

  // TEMPLATES
  case O::SY_CLASS_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_ENUMERATION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_INTERFACE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_FORWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_BACKWARD_RANGER_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_EXTENSION_FUNCTION_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;
  case O::SY_EXTENSION_METHOD_TEMPLATE:
    return OF::SYMBOL | OF::SY_GLOBAL_DECLARATION | OF::SY_TEMPLATE;

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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_STRING_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAnchoring(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasSome(flags, rq::OpcodeFlags::SY_LOCAL_DECLARATION |
                                   rq::OpcodeFlags::SY_GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVisibility(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_VISIBILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeLocation(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_SCOPE_LOCATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAvailability(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasSome(rq::OpcodeFlags::SY_LOCAL_DECLARATION |
                        rq::OpcodeFlags::SY_GLOBAL_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPropertyMutability(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  return opcode == rq::Opcode::SY_DYNAMIC_PARAMETER;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasExporting(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ASCRIBED_GLOBAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasGenerationTime(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ASCRIBED_GLOBAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapturing(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ASCRIBED_GLOBAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasEvaluationTime(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasSome(flags, rq::OpcodeFlags::SY_LOCAL_VARIABLE |
                                   rq::OpcodeFlags::SY_LOCAL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasInlining(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangling(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPacking(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  return opcode == rq::Opcode::CLASS;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplating(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ASCRIBED_GLOBAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikelyhood(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_LIKELYHOOD);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasSupport(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ASCRIBED_GLOBAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressStability(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  return opcode == rq::Opcode::CLASS;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariadicness(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  return opcode == rq::Opcode::SY_DYNAMIC_PARAMETER;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstraint(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasWeighting(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariability(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_VARIABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatility(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_VOLATILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomicity(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_ATOMICITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_NULL_TERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPrecondition(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPostcondition(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

RQ_ALWAYS_INLINE Entity::Entity(rq::Opcode opcode) : _opcode(opcode) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getOpcode() const {
  return this->_opcode;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getOpcodeFlags() const {
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

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsStringType() const {
  return rq::getIsStringType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunitType() const {
  return rq::getIsCodeunitType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasAnchoring() const {
  return rq::getHasAnchoring(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasVisibility() const {
  return rq::getHasVisibility(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasScopeLocation() const {
  return rq::getHasScopeLocation(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasAvailability() const {
  return rq::getHasAvailability(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasPropertyMutability() const {
  return rq::getHasPropertyMutability(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasExporting() const {
  return rq::getHasExporting(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasGenerationTime() const {
  return rq::getHasGenerationTime(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasCapturing() const {
  return rq::getHasCapturing(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasEvaluationTime() const {
  return rq::getHasEvaluationTime(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasInlining() const {
  return rq::getHasInlining(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasMangling() const {
  return rq::getHasMangling(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasPacking() const {
  return rq::getHasPacking(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasTemplating() const {
  return rq::getHasTemplating(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasLikelyhood() const {
  return rq::getHasLikely(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasSupport() const {
  return rq::getHasSupport(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasAddressStability() const {
  return rq::getHasAddressStability(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasVariadicness() const {
  return rq::getHasVariadicness(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasConstraint() const {
  return rq::getHasConstraint(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasWeighting() const {
  return rq::getHasWeighting(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasVariability() const {
  return rq::getHasVariability(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasVolatility() const {
  return rq::getHasVolatility(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasAtomicity() const {
  return rq::getHasAtomicity(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasNullTerminate() const {
  return rq::getHasNullTerminate(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasPrecondition() const {
  return rq::getHasPrecondition(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasPostcondition() const {
  return rq::getHasPostcondition(this->getOpcode());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSymbol(entity.getOpcode());
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::Opcode opcode)
    : Symbol(opcode) {
  RQ_ASSERT(rq::getIsSimpleSymbol(opcode),
            "not simple symbol");
}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSimpleSymbol(entity->getOpcode());
}

} // namespace rq