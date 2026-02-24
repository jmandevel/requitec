#pragma once

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::NONE:
    return "none";

  // =====SYMBOLS=====

  // TYPE DEFINITION
  case E::SY_TYPE_DEFINITION:
    return "sy_type_definition";

  // SIMPLE BUILTIN
  case E::SY_INFERENCE:
    return "sy_inference";
  case E::SY_EXPRESSION:
    return "sy_expression";
  case E::SY_ENTITY_KIND:
    return "sy_entity_kind";
  case E::SY_TYPE_TYPE:
    return "sy_type_type";
  case E::SY_GENERIC_SYMBOL:
    return "sy_generic_symbol";
  case E::SY_VOID:
    return "sy_void";
  case E::SY_NULL:
    return "sy_null";
  case E::SY_NO_RETURN:
    return "sy_no_return";
  case E::SY_VARIADIC_ARGUMENTS:
    return "sy_variadic_arguments";
  case E::SY_BOOLEAN:
    return "sy_boolean";
  case E::SY_GENERIC_FLOAT:
    return "sy_generic_float";
  case E::SY_HALF:
    return "sy_half";
  case E::SY_SINGLE:
    return "sy_single";
  case E::SY_DOUBLE:
    return "sy_double";
  case E::SY_QUADRUPLE:
    return "sy_quadruple";
  case E::SY_GENERIC_BINARY:
    return "sy_generic_binary";
  case E::SY_GENERIC_BFLOAT:
    return "sy_generic_float";
  case E::SY_BINARY16:
    return "sy_binary16";
  case E::SY_BINARY32:
    return "sy_binary32";
  case E::SY_BINARY64:
    return "sy_binary64";
  case E::SY_BINARY128:
    return "sy_binary128";
  case E::SY_BFLOAT16:
    return "sy_bfloat16";
  case E::SY_GENERIC_INTEGER:
    return "sy_generic_integer";
  case E::SY_GENERIC_SIGNED:
    return "sy_generic_signed";
  case E::SY_GENERIC_UNSIGNED:
    return "sy_generic_unsigned";
  case E::SY_GENERIC_CODEUNIT:
    return "sy_generic_codeunit";
  case E::SY_ASCII:
    return "sy_ascii";
  case E::SY_UTF8:
    return "sy_utf8";

  // SCALED BUILTIN
  case E::SY_SCALED_SIGNED:
    return "sy_scaled_signed";
  case E::SY_SCALED_UNSIGNED:
    return "sy_scaled_unsigned";

  // UNARY SUBTYPE
  case E::SY_RANGE:
    return "sy_range";
  case E::SY_REFERENCE:
    return "sy_reference";
  case E::SY_POINTER:
    return "sy_pointer";
  case E::SY_FAT_POINTER:
    return "sy_fat_pointer";
  case E::SY_INFERENCED_COUNT_ARRAY:
    return "sy_inferenced_count_array";

  // COUNTED SUBTYPE
  case E::SY_ARRAY:
    return "sy_array";

  // COMPOSITE SUBTYPE
  case E::SY_LAYOUT:
    return "sy_layout";
  case E::SY_SIGNATURE:
    return "sy_signature";

  // ARITHMETIC SEQUENCE
  case E::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";

  // MISC
  case E::SY_SYNONYM:
    return "sy_synonum";
  case E::SY_MODULE:
    return "sy_module";
  case E::SY_IMPORT:
    return "sy_import";
  case E::SY_EXTENSION:
    return "sy_extension";
  case E::SY_CODE:
    return "sy_code";
  case E::SY_CATEGORY_DISCRIMINANT:
    return "sy_category_discriminant";

  // BINDING
  case E::SY_DYNAMIC_VARIABLE:
    return "sy_dynamic_variable";
  case E::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case E::SY_ENUMERATOR:
    return "sy_enumerator";
  case E::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
  case E::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case E::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case E::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case E::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
  case E::SY_LABEL:
    return "sy_label";

  // SYMBOL TABLE
  case E::SY_TOP:
    return "sy_top";
  case E::SY_SCOPE:
    return "sy_scope";
  case E::SY_TABLE:
    return "sy_table";
  case E::SY_CLASS:
    return "sy_class";
  case E::SY_ENUMERATION:
    return "sy_enumeration";
  case E::SY_CATEGORY:
    return "sy_category";

  // PROCEDURE
  case E::SY_ENTRY:
    return "sy_entry";
  case E::SY_FUNCTION:
    return "sy_function";
  case E::SY_METHOD:
    return "sy_method";
  case E::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case E::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case E::SY_RANGER:
    return "sy_ranger";

  // TEMPLATE
  case E::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case E::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return "sy_template_dynamic_variable";
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return "sy_template_static_variable";
  case E::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case E::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";

  // PARTIAL SPECIALIZATION
  case E::SY_PARTIAL_CLASS:
    return "sy_partial_class";
  case E::SY_PARTIAL_ENUMERATION:
    return "sy_partial_enumeration";
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return "sy_partial_dynamic_variable";
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return "sy_partial_static_variable";
  case E::SY_PARTIAL_FUNCTION:
    return "sy_partial_function";
  case E::SY_PARTIAL_METHOD:
    return "sy_partial_method";
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return "sy_partial_extension_function";
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return "sy_partial_extension_method";

    // =====CONSTANTS=====

  case E::CT_INTEGER:
    return "ct_integer";
  case E::CT_FLOAT:
    return "ct_float";
  case E::CT_STRING:
    return "ct_string";
  case E::CT_ARRAY:
    return "ct_array";

  case E::OP_NONE:
    return "op_none";
  case E::OP_LOGICAL_AND:
    return "op_logical_and";
  case E::OP_LOGICAL_OR:
    return "op_logical_or";
  case E::OP_LOGICAL_COMPLEMENT:
    return "op_logical_complement";

  case E::OP_GREATER:
    return "op_greater";
  case E::OP_GREATER_EQUAL:
    return "op_greater_equal";
  case E::OP_LESS:
    return "op_less";
  case E::OP_LESS_EQUAL:
    return "op_less_equal";
  case E::OP_EQUAL:
    return "op_equal";
  case E::OP_NOT_EQUAL:
    return "op_not_equal";

  case E::OP_ADD:
    return "op_add";
  case E::OP_SUBTRACT:
    return "op_subtract";
  case E::OP_MULTIPLY:
    return "op_multiply";
  case E::OP_DIVIDE:
    return "op_divide";
  case E::OP_MODULUS:
    return "op_modulus";
  case E::OP_NEGATE:
    return "op_negate";

  case E::OP_CAST:
    return "op_cast";

  case E::OP_BITWISE_AND:
    return "op_bitwise_and";
  case E::OP_BITWISE_OR:
    return "op_bitwise_or";
  case E::OP_BITWISE_XOR:
    return "op_bitwise_xor";
  case E::OP_BITWISE_COMPLEMENT:
    return "op_bitwise_complement";
  case E::OP_BITWISE_SHIFT_LEFT:
    return "op_bitwise_shift_left";
  case E::OP_BITWISE_SHIFT_RIGHT:
    return "op_bitwise_shift_right";
  case E::OP_BITWISE_ROTATE_LEFT:
    return "op_bitwise_rotate_left";
  case E::OP_BITWISE_ROTATE_RIGHT:
    return "op_bitwise_rotate_right";

  case E::OP_CONTENT_OF:
    return "op_content_of";
  case E::OP_ADDRESS_OF:
    return "op_address_of";
  case E::OP_CALL:
    return "op_call";
  case E::OP_DROP:
    return "op_drop";
  case E::OP_MOVE:
    return "op_move";
  case E::OP_COPY:
    return "op_copy";

  case E::OP_RETURN:
    return "op_return";
  case E::OP_GOTO:
    return "op_goto";
  case E::OP_CONDITION:
    return "op_condition";

  case E::OP_LOOP_SEQUENCE:
    return "op_loop_sequence";
  case E::OP_LOOP_ELEMENTS:
    return "op_loop_elements";
  case E::OP_LOOP_RANGER:
    return "op_loop_ranger";

  case E::OP_DEBUG_TRAP:
    return "op_debug_trap";
  case E::OP_UNREACHABLE:
    return "op_unreachable";
  case E::OP_ASSUME:
    return "op_assume";

  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::EntityFlags getFlags(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  using EF = EntityFlags;
  switch (kind) {
  case E::NONE:
    return EF::NONE;

  // TYPE DEFINITION SYMBOL
  case E::SY_TYPE_DEFINITION:
    return EF::SYMBOL | EF::SY_TYPE_NODE;

  // SIMPLE BUILTIN SYMBOL
  case E::SY_INFERENCE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_GENERIC;
  case E::SY_EXPRESSION:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_GENERIC;
  case E::SY_ENTITY_KIND:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_GENERIC;
  case E::SY_TYPE_TYPE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE;
  case E::SY_GENERIC_SYMBOL:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_GENERIC;
  case E::SY_VOID:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_NULL:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_NO_RETURN:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_VARIADIC_ARGUMENTS:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_BOOLEAN:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_GENERIC_FLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_HALF:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_SINGLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_DOUBLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_QUADRUPLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_GENERIC_BINARY:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_GENERIC_BFLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_BINARY16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY32:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY64:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY128:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BFLOAT16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_GENERIC_INTEGER:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_SIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_UNSIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_CODEUNIT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CODEUNIT |
           EF::SY_GENERIC;
  case E::SY_ASCII:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CODEUNIT |
           EF::SY_CONCRETE;
  case E::SY_UTF8:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE_NODE | EF::SY_CODEUNIT |
           EF::SY_CONCRETE;

  // SCALED BUILTIN SYMBOL
  case E::SY_SCALED_SIGNED:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE_NODE | EF::SY_INTEGER |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING | EF::SY_SIGNED;
  case E::SY_SCALED_UNSIGNED:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE_NODE | EF::SY_INTEGER |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;

  // UNARY SUBTYPE SYMBOL
  case E::SY_RANGE:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_REFERENCE:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_FAT_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_INFERENCED_COUNT_ARRAY:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_GENERIC;

  // COUNTED SUBTYPE SYMBOL
  case E::SY_ARRAY:
    return EF::SYMBOL | EF::SY_COUNTED_SUBTYPE | EF::SY_TYPE_NODE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;

  // COMPOSITE SUBTYPE SYMBOL
  case E::SY_LAYOUT:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE_NODE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_SIGNATURE:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE_NODE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;

  // ARITHMETIC SEQUENCE SYMBOL
  case E::SY_ARITHMETIC_INTERVAL:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE_NODE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE_NODE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE_NODE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;

  // MISC SYMBOL
  case E::SY_SYNONYM:
    return EF::SYMBOL | EF::SY_TYPE_NODE | EF::SY_CONCRETE;
  case E::SY_MODULE:
    return EF::SYMBOL;
  case E::SY_IMPORT:
    return EF::SYMBOL;
  case E::SY_EXTENSION:
    return EF::SYMBOL | EF::SY_TYPE_NODE | EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_CODE:
    return EF::SYMBOL;
  case E::SY_CATEGORY_DISCRIMINANT:
    return EF::SYMBOL | EF::SY_TYPE_NODE | EF::SY_CONCRETE;

  // BINDING SYMBOL
  case E::SY_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENUMERATOR:
    return EF::SYMBOL;
  case E::SY_CATEGORY_ALTERNATIVE:
    return EF::SYMBOL;
  case E::SY_CLASS_PARAMETER:
    return EF::SYMBOL;
  case E::SY_LAYOUT_PARAMETER:
    return EF::SYMBOL;
  case E::SY_TEMPLATE_PARAMETER:
    return EF::SYMBOL;
  case E::SY_SIGNATURE_PARAMETER:
    return EF::SYMBOL;
  case E::SY_LABEL:
    return EF::SYMBOL;

  // SYMBOL TABLE SYMBOL
  case E::SY_TOP:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TOP_OF_FRAME;
  case E::SY_SCOPE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE;
  case E::SY_TABLE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TOP_OF_FRAME;
  case E::SY_CLASS:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME | EF::SY_TYPE_NODE;
  case E::SY_ENUMERATION:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME | EF::SY_TYPE_NODE;
  case E::SY_CATEGORY:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME | EF::SY_TYPE_NODE;

  // PROCEDURE SYMBOL
  case E::SY_ENTRY:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_TOP_OF_FRAME;
  case E::SY_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_TOP_OF_FRAME;

  // TEMPLATE SYMBOL
  case E::SY_TEMPLATE_CLASS:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_ENUMERATION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE;

  // PARTIAL SPECIALIZATION SYMBOL
  case E::SY_PARTIAL_CLASS:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_ENUMERATION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL;
  // CONSTANT
  case E::CT_INTEGER:
    return EF::CONSTANT;
  case E::CT_FLOAT:
    return EF::CONSTANT;
  case E::CT_STRING:
    return EF::CONSTANT;
  case E::CT_ARRAY:
    return EF::CONSTANT;
  default:
    break;
  }
  RQ_ASSERT(rq::getUnderlying(kind) < rq::getUnderlying(rq::EntityKind::LAST),
            "out of range");
  return EF::NONE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SYMBOL);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::CONSTANT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOpcode(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasNone(flags, rq::EntityFlags::NOT_OPCODE_MASK);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSimpleBuiltinSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIMPLE_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledBuiltinSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SCALED_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsUnarySubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNARY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCountedSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COUNTED_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCompositeSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COMPOSITE_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_ARITHMETIC_SEQUENCE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolTableSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SYMBOL_TABLE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PROCEDURE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TEMPLATE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PARTIAL);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternativeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_HAS_TEMPLATE_ALTERNATIVE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeNodeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TYPE_NODE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_GENERIC);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcreteSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CONCRETE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformChangingSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PLATFORM_CHANGING);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_FLOAT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CODEUNIT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNSIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrameSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TOP_OF_FRAME);
}
[[nodiscard]] inline rq::EntityKind getTemplate(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_PARTIAL_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  default:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] inline rq::EntityKind getPartial(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_PARTIAL_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  default:
    RQ_UNREACHABLE();
  }
}
[[nodiscard]] inline rq::EntityKind getFullSpecialization(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_METHOD:
    return E::SY_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_PARTIAL_CLASS:
    return E::SY_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  default:
    RQ_UNREACHABLE();
  }
}
Entity::Entity(rq::EntityKind kind) : _kind(kind) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind Entity::getKind() const {
  return this->_kind;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::operator==(const Self &rhs) const {
  return this == &rhs;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::operator!=(const Self &rhs) const {
  return this != &rhs;
}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Entity::getKindName() const {
  return rq::getName(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSymbol() const {
  return rq::getIsSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsConstant() const {
  return rq::getIsConstant(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsInstruction() const {
  return rq::getIsOpcode(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTypeSymbol() const {
  return this->_kind == rq::EntityKind::SY_TYPE_DEFINITION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSimpleBuiltinSymbol() const {
  return rq::getIsSimpleBuiltinSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsInferenceSymbol() const {
  return this->_kind == rq::EntityKind::SY_INFERENCE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsExpressionSymbol() const {
  return this->_kind == rq::EntityKind::SY_EXPRESSION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTypeTypeSymbol() const {
  return this->_kind == rq::EntityKind::SY_TYPE_TYPE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericSymbolSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_SYMBOL;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsVoidSymbol() const {
  return this->_kind == rq::EntityKind::SY_VOID;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsNullSymbol() const {
  return this->_kind == rq::EntityKind::SY_NULL;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsNoReturnSymbol() const {
  return this->_kind == rq::EntityKind::SY_NO_RETURN;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsVariadicArgumentsSymbol() const {
  return this->_kind == rq::EntityKind::SY_VARIADIC_ARGUMENTS;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBooleanSymbol() const {
  return this->_kind == rq::EntityKind::SY_BOOLEAN;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericFloatSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_FLOAT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsHalfSymbol() const {
  return this->_kind == rq::EntityKind::SY_HALF;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSingleSymbol() const {
  return this->_kind == rq::EntityKind::SY_SINGLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsDoubleSymbol() const {
  return this->_kind == rq::EntityKind::SY_DOUBLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsQuadrupleSymbol() const {
  return this->_kind == rq::EntityKind::SY_QUADRUPLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericBinarySymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_BINARY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericBfloatSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_BFLOAT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBinary16Symbol() const {
  return this->_kind == rq::EntityKind::SY_BINARY16;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBinary32Symbol() const {
  return this->_kind == rq::EntityKind::SY_BINARY32;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBinary64Symbol() const {
  return this->_kind == rq::EntityKind::SY_BINARY64;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBinary128Symbol() const {
  return this->_kind == rq::EntityKind::SY_BINARY128;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBfloat16Symbol() const {
  return this->_kind == rq::EntityKind::SY_BFLOAT16;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericIntegerSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_INTEGER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericSignedSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_SIGNED;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericUnsignedSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_UNSIGNED;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericCodeunitSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_CODEUNIT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericStringSymbol() const {
  return this->_kind == rq::EntityKind::SY_GENERIC_STRING;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsAsciiSymbol() const {
  return this->_kind == rq::EntityKind::SY_ASCII;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsUtf8Symbol() const {
  return this->_kind == rq::EntityKind::SY_UTF8;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsScaledBuiltinSymbol() const {
  return rq::getIsScaledBuiltinSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsScaledSignedSymbol() const {
  return this->_kind == rq::EntityKind::SY_SCALED_SIGNED;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsScaledUnsignedSymbol() const {
  return this->_kind == rq::EntityKind::SY_SCALED_UNSIGNED;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsUnarySubtypeSymbol() const {
  return rq::getIsUnarySubtypeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsRangeSymbol() const {
  return this->_kind == rq::EntityKind::SY_RANGE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsReferenceSymbol() const {
  return this->_kind == rq::EntityKind::SY_REFERENCE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPointerSymbol() const {
  return this->_kind == rq::EntityKind::SY_POINTER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsFatPointerSymbol() const {
  return this->_kind == rq::EntityKind::SY_FAT_POINTER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsInferencedCountArraySymbol() const {
  return this->_kind == rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsCountedSubtypeSymbol() const {
  return rq::getIsCountedSubtypeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsArraySymbol() const {
  return this->_kind == rq::EntityKind::SY_ARRAY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsCompositeSubtypeSymbol() const {
  return rq::getIsCompositeSubtypeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsLayoutSymbol() const {
  return this->_kind == rq::EntityKind::SY_LAYOUT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSignatureSymbol() const {
  return this->_kind == rq::EntityKind::SY_SIGNATURE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsArithmeticSequenceSymbol() const {
  return rq::getIsArithmeticSequenceSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsArithmeticIntervalSymbol() const {
  return this->_kind == rq::EntityKind::SY_ARITHMETIC_INTERVAL;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsFiniteArithmeticProgressionSymbol() const {
  return this->_kind == rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsInfiniteArithmeticProgressionSymbol() const {
  return this->_kind == rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSynonymSymbol() const {
  return this->_kind == rq::EntityKind::SY_SYNONYM;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsModuleSymbol() const {
  return this->_kind == rq::EntityKind::SY_MODULE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsImportSymbol() const {
  return this->_kind == rq::EntityKind::SY_IMPORT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsExtensionSymbol() const {
  return this->_kind == rq::EntityKind::SY_EXTENSION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsCodeSymbol() const {
  return this->_kind == rq::EntityKind::SY_CODE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsCategoryDiscriminantSymbol() const {
  return this->_kind == rq::EntityKind::SY_CATEGORY_DISCRIMINANT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsDynamicVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_DYNAMIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsStaticVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_STATIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsEnumeratorSymbol() const {
  return this->_kind == rq::EntityKind::SY_ENUMERATOR;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsCategoryAlternativeSymbol() const {
  return this->_kind == rq::EntityKind::SY_CATEGORY_ALTERNATIVE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsClassParameterSymbol() const {
  return this->_kind == rq::EntityKind::SY_CLASS_PARAMETER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsLayoutParameterSymbol() const {
  return this->_kind == rq::EntityKind::SY_LAYOUT_PARAMETER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateParameterSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_PARAMETER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsSignatureParameterSymbol() const {
  return this->_kind == rq::EntityKind::SY_SIGNATURE_PARAMETER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsLabelSymbol() const {
  return this->_kind == rq::EntityKind::SY_LABEL;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSymbolTableSymbol() const {
  return rq::getIsSymbolTableSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTopSymbol() const {
  return this->_kind == rq::EntityKind::SY_TOP;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsScopeSymbol() const {
  return this->_kind == rq::EntityKind::SY_SCOPE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTableSymbol() const {
  return this->_kind == rq::EntityKind::SY_TABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsClassSymbol() const {
  return this->_kind == rq::EntityKind::SY_CLASS;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsEnumerationSymbol() const {
  return this->_kind == rq::EntityKind::SY_ENUMERATION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsCategorySymbol() const {
  return this->_kind == rq::EntityKind::SY_CATEGORY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsProcedureSymbol() const {
  return rq::getIsProcedureSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsEntrySymbol() const {
  return this->_kind == rq::EntityKind::SY_ENTRY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsExtensionFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_EXTENSION_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsExtensionMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_EXTENSION_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsRangerSymbol() const {
  return this->_kind == rq::EntityKind::SY_RANGER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTemplateSymbol() const {
  return rq::getIsTemplateSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTemplateClassSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_CLASS;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateEnumerationSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_ENUMERATION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateDynamicVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateStaticVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTemplateMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateExtensionFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsTemplateExtensionMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPartialSymbol() const {
  return rq::getIsPartialSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPartialClassSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_CLASS;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPartialEnumerationSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_ENUMERATION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPartialDynamicVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPartialStaticVariableSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPartialFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPartialMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPartialExtensionFunctionSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPartialExtensionMethodSymbol() const {
  return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsIntegerConstant() const {
  return this->_kind == rq::EntityKind::CT_INTEGER;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsFloatConstant() const {
  return this->_kind == rq::EntityKind::CT_FLOAT;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsStringConstant() const {
  return this->_kind == rq::EntityKind::CT_STRING;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsArrayConstant() const {
  return this->_kind == rq::EntityKind::CT_ARRAY;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getHasTemplateAlternativeSymbol() const {
  return rq::getHasTemplateAlternativeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTypeNodeSymbol() const {
  return rq::getIsTypeNodeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsGenericSymbol() const {
  return rq::getIsGenericSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsConcreteSymbol() const {
  return rq::getIsConcreteSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSubtypeSymbol() const {
  return rq::getIsSubtypeSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::getIsPlatformChangingSymbol() const {
  return rq::getIsPlatformChangingSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsIntegerSymbol() const {
  return rq::getIsIntegerSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsFloatSymbol() const {
  return rq::getIsFloatSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsCodeunitSymbol() const {
  return rq::getIsCodeunitSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSignedSymbol() const {
  return rq::getIsSignedSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsUnsignedSymbol() const {
  return rq::getIsUnsignedSymbol(this->_kind);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTopOfFrameSymbol() const {
  return rq::getIsTopOfFrameSymbol(this->_kind);
}
SymbolTableIterator::SymbolTableIterator(
    rq::SymbolTableSymbol *symbol_table_ptr)
    : _symbol_table_ptr(symbol_table_ptr) {}
rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  if (this->_symbol_table_ptr == nullptr) {
    return *this;
  }
  rq::SymbolTableSymbol &table = rq::dereferencePtr(this->_symbol_table_ptr);
  if (table.getIsTopOfFrameSymbol()) {
    this->_symbol_table_ptr = nullptr;
    return *this;
  }
  this->_symbol_table_ptr = table.getContainingSymbolTablePtr();
  return *this;
}
rq::SymbolTableIterator SymbolTableIterator::operator++(int) { return ++*this; }
[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableIterator::operator==(const Self &it) const {
  return this->_symbol_table_ptr == it._symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableIterator::operator!=(const Self &it) const {
  return this->_symbol_table_ptr != it._symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &
SymbolTableIterator::operator*() {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
SymbolTableIterator::operator*() const {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *
SymbolTableIterator::operator->() {
  return this->_symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
SymbolTableIterator::operator->() const {
  return this->_symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool SymbolTableIterator::getIsDone() const {
  return this->_symbol_table_ptr == nullptr;
}
ConstSymbolTableIterator::ConstSymbolTableIterator(
    const rq::SymbolTableSymbol *symbol_table_ptr)
    : _symbol_table_ptr(symbol_table_ptr) {}
rq::ConstSymbolTableIterator &ConstSymbolTableIterator::operator++() {
  if (this->_symbol_table_ptr == nullptr) {
    return *this;
  }
  const rq::SymbolTableSymbol &table =
      rq::dereferencePtr(this->_symbol_table_ptr);
  if (table.getIsTopOfFrameSymbol()) {
    this->_symbol_table_ptr = nullptr;
    return *this;
  }
  this->_symbol_table_ptr = table.getContainingSymbolTablePtr();
  return *this;
}
rq::ConstSymbolTableIterator ConstSymbolTableIterator::operator++(int) {
  return ++*this;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::operator==(const Self &it) const {
  return this->_symbol_table_ptr == it._symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::operator!=(const Self &it) const {
  return this->_symbol_table_ptr != it._symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
ConstSymbolTableIterator::operator*() const {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
ConstSymbolTableIterator::operator->() const {
  return this->_symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::getIsDone() const {
  return this->_symbol_table_ptr == nullptr;
}
Symbol::Symbol(rq::EntityKind kind) : rq::Entity(kind) {
  RQ_ASSERT_SYMBOL(kind);
}
SimpleBuiltinSymbol::SimpleBuiltinSymbol(rq::EntityKind kind)
    : rq::Symbol(kind) {
  RQ_ASSERT(rq::getIsSimpleBuiltinSymbol(kind),
            "kind not builtin simple symbol");
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBitsScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags, rq::ScaledBuiltinFlags::BITS_NONE_MASK);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBytesScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::BYTES);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasIndexScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::INDEX);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAddressScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::ADDRESS);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags, rq::ScaledBuiltinFlags::EXACT_NONE_MASK);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::FASTEST);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::LEAST);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags,
                        rq::ScaledBuiltinFlags::PLATFORM_ENDIAN_NONE_MASK);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsBigEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::BIG_ENDIAN_);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsLittleEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::LITTLE_ENDIAN_);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPlatformScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::PLATFORM_SCALAR);
}
void RQ_ALWAYS_INLINE profileScaledIntegerSymbol(llvm::FoldingSetNodeID &id,
                                                 rq::EntityKind kind,
                                                 std::uint16_t scalar,
                                                 std::uint16_t uid,
                                                 rq::ScaledBuiltinFlags flags) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(static_cast<unsigned>(scalar));
  id.AddInteger(static_cast<unsigned>(uid));
  id.AddInteger(static_cast<unsigned>(flags));
}
ScaledBuiltinSymbol::ScaledBuiltinSymbol(rq::EntityKind kind, unsigned scalar,
                                         unsigned uid,
                                         rq::ScaledBuiltinFlags flags)
    : rq::Symbol(kind), _scalar(scalar), _uid(uid), _flags(flags) {
  RQ_ASSERT(rq::getIsScaledBuiltinSymbol(kind),
            "kind not scaled builtin symbol");
  RQ_ASSERT(scalar < rq::MAX_SCALED_BUILTIN_SCALAR, "scalar too large");
  RQ_ASSERT(uid < rq::MAX_SCALED_BUILTIN_UID, "uid too large");
}
[[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t
ScaledBuiltinSymbol::getScalar() const {
  return this->_scalar;
}
[[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t
ScaledBuiltinSymbol::getUid() const {
  return this->_uid;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinFlags
ScaledBuiltinSymbol::getFlags() const {
  return this->_flags;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getHasBytesScalar() const {
  return rq::getHasBytesScalar(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getHasIndexScalar() const {
  return rq::getHasIndexScalar(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getHasAddressScalar() const {
  return rq::getHasAddressScalar(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltinSymbol::getIsExact() const {
  return rq::getIsExact(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltinSymbol::getIsFastest() const {
  return rq::getIsFastest(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltinSymbol::getIsLeast() const {
  return rq::getIsLeast(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getIsPlatformEndian() const {
  return rq::getIsPlatformEndian(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getIsBigEndian() const {
  return rq::getIsBigEndian(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getIsLittleEndian() const {
  return rq::getIsLittleEndian(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
ScaledBuiltinSymbol::getHasPlatformScalar() const {
  return rq::getHasPlatformScalar(this->_flags);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltinSymbol::getIsSynonym() const {
  return this->_uid != 0;
}
void ScaledBuiltinSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileScaledIntegerSymbol(id, this->_kind, this->_scalar, this->_uid,
                                 this->_flags);
}
ScaledIntegerSymbol::ScaledIntegerSymbol(unsigned scalar, unsigned uid,
                                         rq::ScaledBuiltinFlags flags)
    : rq::ScaledBuiltinSymbol(rq::EntityKind::SY_GENERIC_INTEGER, scalar, uid,
                              flags) {}
ScaledIntegerSymbol::ScaledIntegerSymbol(rq::EntityKind kind, unsigned scalar,
                                         unsigned uid,
                                         rq::ScaledBuiltinFlags flags)
    : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}
FloatSymbol::FloatSymbol(unsigned scalar, unsigned uid,
                         rq::ScaledBuiltinFlags flags)
    : rq::ScaledBuiltinSymbol(rq::EntityKind::SY_GENERIC_FLOAT, scalar, uid,
                              flags) {}
FloatSymbol::FloatSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
                         rq::ScaledBuiltinFlags flags)
    : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}
void RQ_ALWAYS_INLINE profileUnarySubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                rq::EntityKind kind,
                                                const rq::Symbol &root) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&root);
}
inline UnarySubtypeSymbol::UnarySubtypeSymbol(rq::EntityKind kind,
                                              rq::Symbol &root)
    : rq::Symbol(kind), _root_ptr(&root) {
  RQ_ASSERT(rq::getIsUnarySubtypeSymbol(kind), "kind not unary subtype symbol");
  RQ_ASSERT(root.getIsTypeSymbol(), "not type");
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &UnarySubtypeSymbol::getRoot() {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
UnarySubtypeSymbol::getRoot() const {
  return rq::dereferencePtr(this->_root_ptr);
}
void UnarySubtypeSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileUnarySubtypeSymbol(id, this->getKind(), this->getRoot());
}
void RQ_ALWAYS_INLINE profileCountedSubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                  rq::EntityKind kind,
                                                  const rq::Symbol &root,
                                                  unsigned count) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&root);
  id.AddInteger(count);
}
CountedSubtypeSymbol::CountedSubtypeSymbol(rq::EntityKind kind,
                                           rq::Symbol &root, std::size_t count)
    : rq::Symbol(kind), _root_ptr(&root), _count(count) {
  RQ_ASSERT(rq::getIsCountedSubtypeSymbol(kind), "not counted subtype");
  RQ_ASSERT(root.getIsTypeSymbol(), "not type");
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &CountedSubtypeSymbol::getRoot() {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
CountedSubtypeSymbol::getRoot() const {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE std::size_t
CountedSubtypeSymbol::getCount() const {
  return this->_count;
}
void CountedSubtypeSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileCountedSubtypeSymbol(id, this->getKind(), this->getRoot(),
                                  this->getCount());
}
void RQ_ALWAYS_INLINE profileArithmeticSequenceSymbol(
    llvm::FoldingSetNodeID &id, const rq::Symbol &root,
    rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  // no need to fold kind
  id.AddPointer(&root);
  id.AddInteger(static_cast<unsigned>(condition));
  id.AddInteger(static_cast<unsigned>(step));
}
ArithmeticSequenceSymbol::ArithmeticSequenceSymbol(
    rq::EntityKind kind, rq::Symbol &root,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step)
    : rq::Symbol(kind), _root_ptr(&root), _condition(condition), _step(step) {
  RQ_ASSERT(root.getIsTypeSymbol(), "not type");
  RQ_ASSERT(rq::getIsArithmeticSequenceSymbol(kind),
            "kind not arithmetic sequence symbol");
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &ArithmeticSequenceSymbol::getRoot() {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
ArithmeticSequenceSymbol::getRoot() const {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
ArithmeticSequenceSymbol::getCondition() const {
  return this->_condition;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep
ArithmeticSequenceSymbol::getStep() const {
  return this->_step;
}
inline void
ArithmeticSequenceSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileArithmeticSequenceSymbol(id, this->getRoot(), this->getCondition(),
                                      this->getStep());
}
namespace detail {
HasLocationSymbol::HasLocationSymbol(rq::Expression &expression)
    : _expression_ptr(&expression) {}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasLocationSymbol::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
HasLocationSymbol::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
HasLocationSymbol::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}
RQ_ALWAYS_INLINE void
HasLocationSymbol::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}
RQ_ALWAYS_INLINE void
HasLocationSymbol::setExpression(rq::Expression *expression_ptr) {
  rq::assignSingleValue(this->_expression_ptr, expression_ptr);
}
ModuleMemberSymbol::ModuleMemberSymbol(rq::ModuleSymbol &module)
    : _module_ptr(&module) {}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &
ModuleMemberSymbol::getModule() const {
  return rq::dereferencePtr(this->_module_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &
ModuleMemberSymbol::getModule() {
  return rq::dereferencePtr(this->_module_ptr);
}
SymbolTableMemberSymbol::SymbolTableMemberSymbol(
    rq::SymbolTableSymbol &containing_table)
    : _containing_symbol_table_ptr(&containing_table) {}
[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableMemberSymbol::getHasContainingSymbolTable() const {
  return this->_containing_symbol_table_ptr != nullptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
SymbolTableMemberSymbol::getContainingSymbolTable() const {
  return rq::dereferencePtr(this->_containing_symbol_table_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &
SymbolTableMemberSymbol::getContainingSymbolTable() {
  return rq::dereferencePtr(this->_containing_symbol_table_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
SymbolTableMemberSymbol::getContainingSymbolTablePtr() const {
  return this->_containing_symbol_table_ptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *
SymbolTableMemberSymbol::getContainingSymbolTablePtr() {
  return this->_containing_symbol_table_ptr;
}
HasAttributesSymbol::HasAttributesSymbol(
    rq::ExpressionAttributeFlags attributes)
    : _attributes(attributes) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttributeFlags
HasAttributesSymbol::getExpressionAttributes() const {
  return this->_attributes;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasOpaque() const {
  return rq::getHasOpaque(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasOutside() const {
  return rq::getHasOutside(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasStatic() const {
  return rq::getHasStatic(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasCapture() const {
  return rq::getHasCapture(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasEager() const {
  return rq::getHasEager(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasMayParent() const {
  return rq::getHasMayParent(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasParent() const {
  return rq::getHasParent(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasAbstract() const {
  return rq::getHasAbstract(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasVirtual() const {
  return rq::getHasVirtual(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasOverride() const {
  return rq::getHasOverride(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasPosition() const {
  return rq::getHasPosition(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasMangle() const {
  return rq::getHasMangle(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasPack() const {
  return rq::getHasPack(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasLabel() const {
  return rq::getHasLabel(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasTemplate() const {
  return rq::getHasTemplate(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasLikely() const {
  return rq::getHasLikely(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasUnlikely() const {
  return rq::getHasUnlikely(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasDepreciated() const {
  return rq::getHasDepreciated(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasExport() const {
  return rq::getHasExport(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasPublic() const {
  return rq::getHasPublic(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasProtected() const {
  return rq::getHasProtected(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasMayCopy() const {
  return rq::getHasMayCopy(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasMayMove() const {
  return rq::getHasMayMove(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool HasAttributesSymbol::getHasOk() const {
  return rq::getHasOk(this->_attributes);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasAttributesSymbol::getHasAttribute(rq::ExpressionAttribute attribute) const {
  return rq::getHasAttribute(this->_attributes, attribute);
}
HasNameSymbol::HasNameSymbol(llvm::StringRef name) : _name(name) {
  RQ_ASSERT(!name.empty(), "empty name");
}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef HasNameSymbol::getName() const {
  return this->_name;
}
MaybeHasNameSymbol::MaybeHasNameSymbol(llvm::StringRef name) : _name(name) {
  RQ_ASSERT(!name.empty(), "empty name");
}
[[nodiscard]] RQ_ALWAYS_INLINE bool MaybeHasNameSymbol::getHasName() const {
  return !this->_name.empty();
}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
MaybeHasNameSymbol::getName() const {
  return this->_name;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasBindingTypeSymbol::getHasBindingType() const {
  return this->_binding_type_ptr != nullptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeSymbol &
HasBindingTypeSymbol::getBindingType() const {
  return rq::dereferencePtr(this->_binding_type_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &
HasBindingTypeSymbol::getBindingType() {
  return rq::dereferencePtr(this->_binding_type_ptr);
}
RQ_ALWAYS_INLINE void
HasBindingTypeSymbol::setBindingType(rq::TypeSymbol &type) {
  rq::assignSingleValue(this->_binding_type_ptr, &type);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
HasImportModuleSymbol::getHasImportModule() const {
  return this->_import_module_ptr != nullptr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &
HasImportModuleSymbol::getImportModule() const {
  return rq::dereferencePtr(this->_import_module_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &
HasImportModuleSymbol::getImportModule() {
  return rq::dereferencePtr(this->_import_module_ptr);
}
RQ_ALWAYS_INLINE void
HasImportModuleSymbol::setImportModule(rq::ModuleSymbol &import_module) {
  rq::assignSingleValue(this->_import_module_ptr, &import_module);
}
} // namespace detail

inline SymbolTableSymbol::SymbolTableSymbol(rq::EntityKind kind)
    : rq::Symbol(kind), rq::detail::SymbolTableMemberSymbol() {}
inline SymbolTableSymbol::SymbolTableSymbol(
    rq::EntityKind kind, rq::SymbolTableSymbol &containing_table)
    : rq::Symbol(kind), rq::detail::SymbolTableMemberSymbol(containing_table) {}
[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableSymbol::operator==(const Self &rhs) const {
  return this == &rhs;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableSymbol::operator!=(const Self &rhs) const {
  return this != &rhs;
}
[[nodiscard]] inline rq::BumpPtrList<rq::Symbol>
SymbolTableSymbol::getNamedList(llvm::StringRef name) {
  auto it = this->_named_values.find(name);
  if (it != this->_named_values.end()) {
    return it->second;
  }
  return rq::BumpPtrList<rq::Symbol>();
}
[[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol>
SymbolTableSymbol::getNamedList(llvm::StringRef name) const {
  auto it = this->_named_values.find(name);
  if (it != this->_named_values.end()) {
    return it->second;
  }
  return rq::ConstBumpPtrList<rq::Symbol>();
}

[[nodiscard]] inline rq::BumpPtrList<rq::Symbol>
SymbolTableSymbol::getUnnamedList() {
  return this->_unamed_values;
}
[[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol>
SymbolTableSymbol::getUnnamedList() const {
  return this->_unamed_values;
}
[[nodiscard]] RQ_ALWAYS_INLINE auto SymbolTableSymbol::getNamedListRange() {
  return std::ranges::subrange(this->_named_values.begin(),
                               this->_named_values.end());
}
[[nodiscard]] RQ_ALWAYS_INLINE auto
SymbolTableSymbol::getNamedListRange() const {
  return std::ranges::subrange(this->_named_values.begin(),
                               this->_named_values.end());
}
[[nodiscard]] RQ_ALWAYS_INLINE auto
SymbolTableSymbol::getAscendingFrameRange() {
  return std::ranges::subrange(
      rq::SymbolTableIterator(this->getContainingSymbolTablePtr()),
      rq::SymbolTableIterator());
}
[[nodiscard]] RQ_ALWAYS_INLINE auto
SymbolTableSymbol::getAscendingFrameRange() const {
  return std::ranges::subrange(
      rq::ConstSymbolTableIterator(this->getContainingSymbolTablePtr()),
      rq::ConstSymbolTableIterator());
}
inline ProcedureSymbol::ProcedureSymbol(rq::EntityKind kind,
                                        rq::Expression &expression,
                                        rq::ModuleSymbol &module,
                                        rq::SymbolTableSymbol &containing_table,
                                        rq::ExpressionAttributeFlags attributes)
    : rq::SymbolTableSymbol(kind, containing_table),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::HasAttributesSymbol(attributes), _signature_ptr(nullptr) {}
inline TemplateSymbol::TemplateSymbol(
    rq::EntityKind kind,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::Symbol(kind), _template_parameters(parameters) {}
PartialSymbol::PartialSymbol(rq::EntityKind kind) : rq::Symbol(kind) {}
inline TypeSymbol::TypeSymbol(rq::EntityKind kind, rq::Symbol &root,
                              rq::TypeAttributeFlags flags)
    : rq::Symbol(kind), _root_ptr(&root), _flags(flags) {}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &TypeSymbol::getRoot() const {
  return rq::dereferencePtr(this->_root_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttributeFlags
TypeSymbol::getFlags() const {
  return this->_flags;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasAttribute(rq::TypeAttribute attribute) const {
  return rq::getHasAttribute(this->_flags, attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasMutable(rq::TypeAttribute attribute) const {
  return rq::getHasMutable(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasConstant(rq::TypeAttribute attribute) const {
  return rq::getHasConstant(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasPartiallyMutable(rq::TypeAttribute attribute) const {
  return rq::getHasPartiallyMutable(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasVolatile(rq::TypeAttribute attribute) const {
  return rq::getHasVolatile(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasAtomic(rq::TypeAttribute attribute) const {
  return rq::getHasAtomic(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasNullTerminated(rq::TypeAttribute attribute) const {
  return rq::getHasNullTerminated(attribute);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
TypeSymbol::getHasMayDiscard(rq::TypeAttribute attribute) const {
  return rq::getHasMayDiscard(attribute);
}
inline void TypeSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  id.AddInteger(static_cast<unsigned>(this->_kind));
  id.AddPointer(this->_root_ptr);
  id.AddInteger(static_cast<unsigned>(this->_flags));
}
inline InferenceSymbol::InferenceSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_INFERENCE) {}
inline ExpressionSymbol::ExpressionSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_EXPRESSION) {}
VoidSymbol::VoidSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_VOID) {}
inline NullSymbol::NullSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_NULL) {}
inline NoReturnSymbol::NoReturnSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_NO_RETURN) {}
inline VariadicArgumentsSymbol::VariadicArgumentsSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_VARIADIC_ARGUMENTS) {}
inline BooleanSymbol::BooleanSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BOOLEAN) {}
inline GenericFloatSymbol::GenericFloatSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_FLOAT) {}
inline HalfSymbol::HalfSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_HALF) {}
inline SingleSymbol::SingleSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_SINGLE) {}
inline DoubleSymbol::DoubleSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_DOUBLE) {}
inline QuadrupleSymbol::QuadrupleSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_QUADRUPLE) {}
inline GenericBinarySymbol::GenericBinarySymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_BINARY) {}
inline GenericBfloatSymbol::GenericBfloatSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_BFLOAT) {}
inline Binary16Symbol::Binary16Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY16) {}
inline Binary32Symbol::Binary32Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY32) {}
inline Binary64Symbol::Binary64Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY64) {}
inline Binary128Symbol::Binary128Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY128) {}
inline Bfloat16Symbol::Bfloat16Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BFLOAT16) {}
inline GenericIntegerSymbol::GenericIntegerSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_INTEGER) {}
inline GenericSignedSymbol::GenericSignedSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_SIGNED) {}
inline GenericUnsignedSymbol::GenericUnsignedSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_UNSIGNED) {}
inline GenericStringSymbol::GenericStringSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_STRING) {}
inline GenericCodeunitSymbol::GenericCodeunitSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_CODEUNIT) {}
inline AsciiSymbol::AsciiSymbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_ASCII) {}
inline Utf8Symbol::Utf8Symbol()
    : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_UTF8) {}
inline ScaledUnsignedSymbol::ScaledUnsignedSymbol(unsigned scalar, unsigned uid,
                                                  rq::ScaledBuiltinFlags flags)
    : rq::ScaledIntegerSymbol(rq::EntityKind::SY_SCALED_UNSIGNED, scalar, uid,
                              flags) {}
inline ScaledSignedSymbol::ScaledSignedSymbol(unsigned scalar, unsigned uid,
                                              rq::ScaledBuiltinFlags flags)
    : rq::ScaledIntegerSymbol(rq::EntityKind::SY_SCALED_SIGNED, scalar, uid,
                              flags) {}
inline RangeSymbol::RangeSymbol(rq::Symbol &root)
    : rq::UnarySubtypeSymbol(rq::EntityKind::SY_RANGE, root) {}
inline ReferenceSymbol::ReferenceSymbol(rq::Symbol &root)
    : rq::UnarySubtypeSymbol(rq::EntityKind::SY_REFERENCE, root) {}
inline PointerSymbol::PointerSymbol(rq::Symbol &root)
    : rq::UnarySubtypeSymbol(rq::EntityKind::SY_POINTER, root) {}
inline FatPointerSymbol::FatPointerSymbol(rq::Symbol &root)
    : rq::UnarySubtypeSymbol(rq::EntityKind::SY_FAT_POINTER, root) {}
inline InferencedCountArraySymbol::InferencedCountArraySymbol(rq::Symbol &root)
    : rq::UnarySubtypeSymbol(rq::EntityKind::SY_INFERENCED_COUNT_ARRAY, root) {}
inline ArraySymbol::ArraySymbol(rq::Symbol &root, std::size_t count)
    : rq::CountedSubtypeSymbol(rq::EntityKind::SY_ARRAY, root, count) {}
inline LayoutSymbol::LayoutSymbol(
    rq::BumpPtrList<rq::LayoutParameterSymbol> properties)
    : rq::Symbol(rq::EntityKind::SY_LAYOUT), _properties(properties) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::LayoutParameterSymbol>
LayoutSymbol::getProperties() const {
  return this->_properties;
}
void LayoutSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  id.AddInteger(static_cast<unsigned>(this->_kind));
}
inline SignatureSymbol::SignatureSymbol(rq::Symbol &return_,
                                        rq::BumpPtrList<rq::Symbol> parameters)
    : rq::Symbol(rq::EntityKind::SY_SIGNATURE), _return_ptr(&return_),
      _parameters(parameters) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &SignatureSymbol::getReturn() {
  return rq::dereferencePtr(this->_return_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
SignatureSymbol::getReturn() const {
  return rq::dereferencePtr(this->_return_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Symbol>
SignatureSymbol::getParameters() const {
  return this->_parameters;
}
void SignatureSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  id.AddPointer(this->_return_ptr);
  id.AddInteger(static_cast<unsigned>(this->_kind));
}
inline ArithmeticIntervalSymbol::ArithmeticIntervalSymbol(
    rq::Symbol &root, rq::ArithmeticSequenceCondition condition)
    : rq::ArithmeticSequenceSymbol(rq::EntityKind::SY_ARITHMETIC_INTERVAL, root,
                                   condition,
                                   rq::ArithmeticSequenceStep::NONE) {}
inline FiniteArithmeticProgressionSymbol::FiniteArithmeticProgressionSymbol(
    rq::Symbol &root, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step)
    : rq::ArithmeticSequenceSymbol(
          rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION, root, condition,
          step) {}
inline InfiniteArithmeticProgressionSymbol::InfiniteArithmeticProgressionSymbol(
    rq::Symbol &root, rq::ArithmeticSequenceStep step)
    : rq::ArithmeticSequenceSymbol(
          rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION, root,
          rq::ArithmeticSequenceCondition::NONE, step) {}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  switch (kind) {
  case rq::ModuleKind::NONE:
    return "none";
  case rq::ModuleKind::SOURCE:
    return "source";
  case rq::ModuleKind::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}
inline SynonymSymbol::SynonymSymbol(rq::TypeSymbol &underlying,
                                    rq::Expression &location)
    : rq::Symbol(rq::EntityKind::SY_SYNONYM),
      rq::detail::HasLocationSymbol(location), _underlying_type(&underlying) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &
SynonymSymbol::getUnderlyingType() {
  return rq::dereferencePtr(this->_underlying_type);
}
inline ModuleSymbol::ModuleSymbol(rq::ModuleKind kind, llvm::StringRef path,
                                  llvm::MemoryBufferRef &&buffer)
    : rq::Symbol(rq::EntityKind::SY_MODULE), rq::detail::HasLocationSymbol(),
      _module_kind(kind), _llvm_buffer_ref(std::move(buffer)), _path(path) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind
ModuleSymbol::getModuleKind() const {
  return this->_module_kind;
}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleSymbol::getPath() const {
  return this->_path;
}
[[nodiscard]] RQ_ALWAYS_INLINE bool ModuleSymbol::getHasSourceText() const {
  return this->_llvm_buffer_ref.getBufferSize() != 0;
}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
ModuleSymbol::getSourceText() const {
  return this->_llvm_buffer_ref.getBuffer();
}
RQ_ALWAYS_INLINE void
ModuleSymbol::changeExpression(rq::Expression &expression) {
  RQ_ASSERT(this->_expression_ptr != nullptr, "no expression");
  this->_expression_ptr = &expression;
}
[[nodiscard]] inline rq::Expression &ModuleSymbol::popExpression() {
  rq::Expression &expression = rq::dereferencePtr(this->_expression_ptr);
  this->_expression_ptr = nullptr;
  return expression;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
ModuleSymbol::replaceExpression(rq::Expression &expression) {
  rq::Expression &replaced_expression = this->getExpression();
  this->_expression_ptr = &expression;
  return replaced_expression;
}
inline ImportSymbol::ImportSymbol(rq::Expression &expression,
                                  rq::ModuleSymbol &module,
                                  rq::SymbolTableSymbol &containing_table,
                                  rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_IMPORT),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes) {}
inline ExtensionSymbol::ExtensionSymbol(rq::SignatureSymbol &signature,
                                        rq::TypeSymbol &type)
    : rq::Symbol(rq::EntityKind::SY_EXTENSION), _signature_ptr(&signature),
      _type_ptr(&type) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureSymbol &
ExtensionSymbol::getSignature() {
  return rq::dereferencePtr(this->_signature_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureSymbol &
ExtensionSymbol::getSignature() const {
  return rq::dereferencePtr(this->_signature_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &ExtensionSymbol::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeSymbol &
ExtensionSymbol::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}
void ExtensionSymbol::Profile(llvm::FoldingSetNodeID &id) const {
  id.AddPointer(this->_signature_ptr);
  id.AddPointer(this->_type_ptr);
}
inline CodeSymbol::CodeSymbol(rq::Expression &expression, llvm::StringRef name,
                              rq::ModuleSymbol &module,
                              rq::ExpressionAttributeFlags attributes,
                              rq::SymbolTableSymbol &containing_table)
    : rq::Symbol(rq::EntityKind::SY_CODE),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::HasNameSymbol(name), rq::detail::ModuleMemberSymbol(module),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::SymbolTableMemberSymbol(containing_table) {}
inline CategorySymbol::CategorySymbol(rq::Expression &expression,
                                      llvm::StringRef name,
                                      rq::ModuleSymbol &module,
                                      rq::ExpressionAttributeFlags attributes,
                                      rq::SymbolTableSymbol &containing_table)
    : rq::SymbolTableSymbol(rq::EntityKind::SY_CATEGORY, containing_table),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::HasNameSymbol(name), rq::detail::ModuleMemberSymbol(module),
      rq::detail::HasAttributesSymbol(attributes) {}
inline TopSymbol::TopSymbol() : rq::SymbolTableSymbol(rq::EntityKind::SY_TOP) {}
inline ScopeSymbol::ScopeSymbol(rq::Expression &expression,
                                rq::ModuleSymbol &module,
                                rq::SymbolTableSymbol &containing_table)
    : rq::SymbolTableSymbol(rq::EntityKind::SY_SCOPE, containing_table),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module) {}
inline DynamicVariableSymbol::DynamicVariableSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_DYNAMIC_VARIABLE),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::HasNameSymbol(name) {}
inline StaticVariableSymbol::StaticVariableSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_STATIC_VARIABLE),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::HasNameSymbol(name) {}
inline EnumeratorSymbol::EnumeratorSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::EnumerationSymbol &enumeration, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_ENUMERATOR),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(
          llvm::cast<rq::SymbolTableSymbol>(enumeration)),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::HasNameSymbol(name) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumerationSymbol &
EnumeratorSymbol::getEnumeration() {
  rq::EnumerationSymbol &enumeration =
      llvm::cast<rq::EnumerationSymbol>(this->getContainingSymbolTable());
  return enumeration;
}
inline CategoryAlternativeSymbol::CategoryAlternativeSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::CategorySymbol &category, rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_ENUMERATOR),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(category),
      rq::detail::HasAttributesSymbol(attributes) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CategorySymbol &
CategoryAlternativeSymbol::getCategory() {
  rq::CategorySymbol &category =
      llvm::cast<rq::CategorySymbol>(this->getContainingSymbolTable());
  return category;
}
inline SignatureParameterSymbol::SignatureParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol() {}
inline SignatureParameterSymbol::SignatureParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol(name) {}
inline TemplateParameterSymbol::TemplateParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol() {}
inline TemplateParameterSymbol::TemplateParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol(name) {}
inline ClassParameterSymbol::ClassParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_CLASS_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes) {}
inline LayoutParameterSymbol::LayoutParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol() {}
inline LayoutParameterSymbol::LayoutParameterSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::MaybeHasNameSymbol(name) {}
inline LabelSymbol::LabelSymbol(rq::Expression &expression,
                                rq::ModuleSymbol &module,
                                rq::SymbolTableSymbol &containing_table,
                                llvm::StringRef name, rq::Expression &statement)
    : rq::Symbol(rq::EntityKind::SY_LABEL),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::SymbolTableMemberSymbol(containing_table),
      rq::detail::HasNameSymbol(name), _statement_ptr(&statement) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &LabelSymbol::getStatement() {
  return rq::dereferencePtr(this->_statement_ptr);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
LabelSymbol::getStatement() const {
  return rq::dereferencePtr(this->_statement_ptr);
}
RQ_ALWAYS_INLINE void LabelSymbol::setStatement(rq::Expression &statement) {
  rq::assignSingleValue(this->_statement_ptr, &statement);
}
inline TableSymbol::TableSymbol(llvm::StringRef name)
    : rq::SymbolTableSymbol(rq::EntityKind::SY_TABLE),
      rq::detail::HasNameSymbol(name) {}
inline ClassSymbol::ClassSymbol(rq::Expression &expression,
                                rq::ModuleSymbol &module,
                                rq::SymbolTableSymbol &containing_table,
                                llvm::StringRef name,
                                rq::ExpressionAttributeFlags attributes)
    : rq::SymbolTableSymbol(rq::EntityKind::SY_CLASS, containing_table),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::HasNameSymbol(name) {}
inline EnumerationSymbol::EnumerationSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::SymbolTableSymbol(rq::EntityKind::SY_ENUMERATION, containing_table),
      rq::detail::HasLocationSymbol(expression),
      rq::detail::ModuleMemberSymbol(module),
      rq::detail::HasAttributesSymbol(attributes),
      rq::detail::HasNameSymbol(name) {}
inline EntrySymbol::EntrySymbol(rq::Expression &expression,
                                rq::ModuleSymbol &module,
                                rq::SymbolTableSymbol &containing_table,
                                rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_ENTRY, expression, module,
                          containing_table, attributes) {}
inline FunctionSymbol::FunctionSymbol(rq::Expression &expression,
                                      rq::ModuleSymbol &module,
                                      rq::SymbolTableSymbol &containing_table,
                                      llvm::StringRef name,
                                      rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_FUNCTION, expression, module,
                          containing_table, attributes),
      rq::detail::HasNameSymbol(name) {}
inline MethodSymbol::MethodSymbol(rq::Expression &expression,
                                  rq::ModuleSymbol &module,
                                  rq::SymbolTableSymbol &containing_table,
                                  llvm::StringRef name,
                                  rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_METHOD, expression, module,
                          containing_table, attributes),
      rq::detail::HasNameSymbol(name) {}
inline ExtensionFunctionSymbol::ExtensionFunctionSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_FUNCTION, expression,
                          module, containing_table, attributes),
      rq::detail::HasNameSymbol(name) {}
inline ExtensionMethodSymbol::ExtensionMethodSymbol(
    rq::Expression &expression, rq::ModuleSymbol &module,
    rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
    rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_METHOD, expression,
                          module, containing_table, attributes),
      rq::detail::HasNameSymbol(name) {}
inline RangerSymbol::RangerSymbol(rq::Expression &expression,
                                  rq::ModuleSymbol &module,
                                  rq::SymbolTableSymbol &containing_table,
                                  rq::ExpressionAttributeFlags attributes)
    : rq::ProcedureSymbol(rq::EntityKind::SY_RANGER, expression, module,
                          containing_table, attributes) {}
inline TemplateClassSymbol::TemplateClassSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_CLASS, parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateEnumerationSymbol::TemplateEnumerationSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_ENUMERATION, parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateDynamicVariableSymbol::TemplateDynamicVariableSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE,
                         parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateStaticVariableSymbol::TemplateStaticVariableSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE,
                         parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateFunctionSymbol::TemplateFunctionSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_FUNCTION, parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateMethodSymbol::TemplateMethodSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_METHOD, parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateExtensionFunctionSymbol::TemplateExtensionFunctionSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION,
                         parameters),
      rq::detail::HasNameSymbol(name) {}
inline TemplateExtensionMethodSymbol::TemplateExtensionMethodSymbol(
    llvm::StringRef name,
    const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
    : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD,
                         parameters),
      rq::detail::HasNameSymbol(name) {}
inline PartialClassSymbol::PartialClassSymbol(llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_CLASS),
      rq::detail::HasNameSymbol(name) {}
inline PartialEnumerationSymbol::PartialEnumerationSymbol(llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_ENUMERATION),
      rq::detail::HasNameSymbol(name) {}
inline PartialDynamicVariableSymbol::PartialDynamicVariableSymbol(
    llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE),
      rq::detail::HasNameSymbol(name) {}
inline PartialStaticVariableSymbol::PartialStaticVariableSymbol(
    llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE),
      rq::detail::HasNameSymbol(name) {}
inline PartialFunctionSymbol::PartialFunctionSymbol(llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_FUNCTION),
      rq::detail::HasNameSymbol(name) {}
inline PartialExtensionFunctionSymbol::PartialExtensionFunctionSymbol(
    llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION),
      rq::detail::HasNameSymbol(name) {}
inline PartialExtensionMethodSymbol::PartialExtensionMethodSymbol(
    llvm::StringRef name)
    : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD),
      rq::detail::HasNameSymbol(name) {}
void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_INTEGER));
  llvm::SmallString<32> buf;
  value.toString(buf, 10, false);
  id.AddString(llvm::StringRef(buf));
}
void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_FLOAT));
  llvm::APInt bits = value.bitcastToAPInt();
  llvm::SmallString<32> buf;
  bits.toString(buf, 10, false);
  id.AddString(llvm::StringRef(buf));
}
void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_STRING));
  id.AddString(value);
}
void RQ_ALWAYS_INLINE profileArrayConstant(
    llvm::FoldingSetNodeID &id, const rq::BumpPtrList<rq::Entity *> &elements) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_ARRAY));
  for (rq::Entity *e : elements) {
    id.AddPointer(e);
  }
}
inline Constant::Constant(rq::EntityKind kind) : rq::Entity(kind) {
  RQ_ASSERT_CONSTANT(kind);
}
inline IntegerConstant::IntegerConstant(const llvm::APInt &value)
    : rq::Constant(rq::EntityKind::CT_INTEGER), _value(value) {}
[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &
IntegerConstant::getValue() const {
  return this->_value;
}
RQ_ALWAYS_INLINE void
IntegerConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileIntegerConstant(id, this->_value);
}
inline FloatConstant::FloatConstant(const llvm::APFloat &value)
    : rq::Constant(rq::EntityKind::CT_FLOAT), _value(value) {}
[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &
FloatConstant::getValue() const {
  return this->_value;
}
RQ_ALWAYS_INLINE void FloatConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileFloatConstant(id, this->_value);
}
inline StringConstant::StringConstant(llvm::StringRef value)
    : rq::Constant(rq::EntityKind::CT_STRING), _value(value) {}
[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
StringConstant::getValue() const {
  return this->_value;
}
RQ_ALWAYS_INLINE void
StringConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileStringConstant(id, this->_value);
}
inline ArrayConstant::ArrayConstant(rq::BumpPtrList<rq::Entity *> elements)
    : rq::Constant(rq::EntityKind::CT_ARRAY), _elements(elements) {}
[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Entity *>
ArrayConstant::getElements() const {
  return this->_elements;
}
RQ_ALWAYS_INLINE void ArrayConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileArrayConstant(id, this->_elements);
}
inline InstructionSlot::InstructionSlot(rq::Entity &entity) : _data{&entity} {}
inline InstructionSlot::InstructionSlot(rq::InstructionNode &node)
    : _data(&node) {}
inline InstructionSlot::InstructionSlot(rq::Instruction &instruction)
    : _data(&instruction) {}
[[nodiscard]] RQ_ALWAYS_INLINE bool InstructionSlot::getIsNone() const {
  return this->_data.isNull();
}
[[nodiscard]] RQ_ALWAYS_INLINE bool InstructionSlot::getIsEntity() const {
  return llvm::isa<rq::Entity *>(this->_data);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
InstructionSlot::getIsInstructionNode() const {
  return llvm::isa<rq::InstructionNode *>(this->_data);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool InstructionSlot::getIsInstruction() const {
  return llvm::isa<rq::Entity *>(this->_data) &&
         this->getEntity().getIsInstruction();
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &InstructionSlot::getEntity() {
  return rq::dereferencePtr(llvm::cast<rq::Entity *>(this->_data));
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
InstructionSlot::getEntity() const {
  return rq::dereferencePtr(llvm::cast<rq::Entity *>(this->_data));
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionNode &
InstructionSlot::getInstructionNode() {
  return rq::dereferencePtr(llvm::cast<rq::InstructionNode *>(this->_data));
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionNode &
InstructionSlot::getInstructionNode() const {
  return rq::dereferencePtr(llvm::cast<rq::InstructionNode *>(this->_data));
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &
InstructionSlot::getInstruction() {
  return llvm::cast<rq::Instruction>(
      rq::dereferencePtr(llvm::cast<rq::Entity *>(this->_data)));
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
InstructionSlot::getInstruction() const {
  return llvm::cast<rq::Instruction>(
      rq::dereferencePtr(llvm::cast<rq::Entity *>(this->_data)));
}
inline void InstructionSlot::clear() { this->_data = nullptr; }
[[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &InstructionNode::getCar() {
  return this->_car;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &
InstructionNode::getCar() const {
  return this->_car;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &InstructionNode::getCdr() {
  return this->_cdr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &
InstructionNode::getCdr() const {
  return this->_cdr;
}
inline void InstructionNode::clear() {
  this->_car.clear();
  this->_cdr.clear();
}
inline Instruction::Instruction() : rq::Entity(rq::EntityKind::OP_NONE) {}
RQ_ALWAYS_INLINE void Instruction::setOpcode(rq::EntityKind opcode) {
  RQ_ASSERT_OPCODE(opcode);
  RQ_ASSERT(this->getKind() == rq::EntityKind::OP_NONE, "opcode already set");
  this->_kind = opcode;
}
RQ_ALWAYS_INLINE void Instruction::changeOpcode(rq::EntityKind opcode) {
  RQ_ASSERT_OPCODE(opcode);
  RQ_ASSERT(this->getKind() != rq::EntityKind::OP_NONE,
            "opcode not already set");
  this->_kind = opcode;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &Instruction::getCdr() {
  return this->_cdr;
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &
Instruction::getCdr() const {
  return this->_cdr;
}
inline void Instruction::clear() {
  this->_kind = rq::EntityKind::OP_NONE;
  this->_cdr.clear();
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &Entity::getSymbol() {
  RQ_ASSERT(this->getIsTypeSymbol(), "not symbol");
  return llvm::cast<rq::Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &Entity::getTypeSymbol() {
  RQ_ASSERT(this->getIsTypeSymbol(), "not type symbol");
  return llvm::cast<rq::TypeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceSymbol &Entity::getInferenceSymbol() {
  RQ_ASSERT(this->getIsInferenceSymbol(), "not inference symbol");
  return llvm::cast<rq::InferenceSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionSymbol &Entity::getExpressionSymbol() {
  RQ_ASSERT(this->getIsExpressionSymbol(), "not expression symbol");
  return llvm::cast<rq::ExpressionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::VoidSymbol &Entity::getVoidSymbol() {
  RQ_ASSERT(this->getIsVoidSymbol(), "not void symbol");
  return llvm::cast<rq::VoidSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::NullSymbol &Entity::getNullSymbol() {
  RQ_ASSERT(this->getIsNullSymbol(), "not null symbol");
  return llvm::cast<rq::NullSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnSymbol &Entity::getNoReturnSymbol() {
  RQ_ASSERT(this->getIsNoReturnSymbol(), "not no return symbol");
  return llvm::cast<rq::NoReturnSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicArgumentsSymbol &Entity::getVariadicArgumentsSymbol() {
  RQ_ASSERT(this->getIsVariadicArgumentsSymbol(), "not variadic arguments symbol");
  return llvm::cast<rq::VariadicArgumentsSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanSymbol &Entity::getBooleanSymbol() {
  RQ_ASSERT(this->getIsBooleanSymbol(), "not boolean symbol");
  return llvm::cast<rq::BooleanSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericFloatSymbol &Entity::getGenericFloatSymbol() {
  RQ_ASSERT(this->getIsGenericFloatSymbol(), "not generic float symbol");
  return llvm::cast<rq::GenericFloatSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::HalfSymbol &Entity::getHalfSymbol() {
  RQ_ASSERT(this->getIsHalfSymbol(), "not half symbol");
  return llvm::cast<rq::HalfSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SingleSymbol &Entity::getSingleSymbol() {
  RQ_ASSERT(this->getIsSingleSymbol(), "not single symbol");
  return llvm::cast<rq::SingleSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::DoubleSymbol &Entity::getDoubleSymbol() {
  RQ_ASSERT(this->getIsDoubleSymbol(), "not double symbol");
  return llvm::cast<rq::DoubleSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::QuadrupleSymbol &Entity::getQuadrupleSymbol() {
  RQ_ASSERT(this->getIsQuadrupleSymbol(), "not quadruple symbol");
  return llvm::cast<rq::QuadrupleSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericBinarySymbol &Entity::getGenericBinarySymbol() {
  RQ_ASSERT(this->getIsGenericBinarySymbol(), "not generic binary symbol");
  return llvm::cast<rq::GenericBinarySymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericBfloatSymbol &Entity::getGenericBFloatSymbol() {
  RQ_ASSERT(this->getIsGenericBfloatSymbol(), "not generic bfloat symbol");
  return llvm::cast<rq::GenericBfloatSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Symbol &Entity::getBinary16Symbol() {
  RQ_ASSERT(this->getIsBinary16Symbol(), "not binary16 symbol");
  return llvm::cast<rq::Binary16Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Symbol &Entity::getBinary32Symbol() {
  RQ_ASSERT(this->getIsBinary32Symbol(), "not binary32 symbol");
  return llvm::cast<rq::Binary32Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Symbol &Entity::getBinary64Symbol() {
  RQ_ASSERT(this->getIsBinary64Symbol(), "not binary64 symbol");
  return llvm::cast<rq::Binary64Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Symbol &Entity::getBinary128Symbol() {
  RQ_ASSERT(this->getIsBinary128Symbol(), "not binary128 symbol");
  return llvm::cast<rq::Binary128Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Bfloat16Symbol &Entity::getBfloat16Symbol() {
  RQ_ASSERT(this->getIsBfloat16Symbol(), "not bfloat16 symbol");
  return llvm::cast<rq::Bfloat16Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericIntegerSymbol &Entity::getGenericIntegerSymbol() {
  RQ_ASSERT(this->getIsGenericIntegerSymbol(), "not generic integer symbol");
  return llvm::cast<rq::GenericIntegerSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericUnsignedSymbol &Entity::getGenericUnsignedSymbol() {
  RQ_ASSERT(this->getIsGenericUnsignedSymbol(), "not generic unsigned symbol");
  return llvm::cast<rq::GenericUnsignedSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericStringSymbol &Entity::getGenericStringSymbol() {
  RQ_ASSERT(this->getIsGenericStringSymbol(), "not generic string symbol");
  return llvm::cast<rq::GenericStringSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::GenericCodeunitSymbol &Entity::getGenericCodeunitSymbol() {
  RQ_ASSERT(this->getIsGenericCodeunitSymbol(), "not generic codeunit symbol");
  return llvm::cast<rq::GenericCodeunitSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::AsciiSymbol &Entity::getAsciiSymbol() {
  RQ_ASSERT(this->getIsAsciiSymbol(), "not ascii symbol");
  return llvm::cast<rq::AsciiSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Symbol &Entity::getUtf8Symbol() {
  RQ_ASSERT(this->getIsUtf8Symbol(), "not utf8 symbol");
  return llvm::cast<rq::Utf8Symbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinSymbol &Entity::getScaledBuiltinSymbol() {
  RQ_ASSERT(this->getIsScaledBuiltinSymbol(), "not scaled builtin symbol");
  return llvm::cast<rq::ScaledBuiltinSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledSignedSymbol &Entity::getScaledSignedSymbol() {
  RQ_ASSERT(this->getIsScaledSignedSymbol(), "not scaled signed symbol");
  return llvm::cast<rq::ScaledSignedSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledUnsignedSymbol &Entity::getScaledUnsignedSymbol() {
  RQ_ASSERT(this->getIsScaledUnsignedSymbol(), "not scaled unsigned symbol");
  return llvm::cast<rq::ScaledUnsignedSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::UnarySubtypeSymbol &Entity::getUnarySubtypeSymbol() {
  RQ_ASSERT(this->getIsUnarySubtypeSymbol(), "not unary subtype symbol");
  return llvm::cast<rq::UnarySubtypeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::RangeSymbol &Entity::getRangeSymbol() {
  RQ_ASSERT(this->getIsRangeSymbol(), "not range symbol");
  return llvm::cast<rq::RangeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ReferenceSymbol &Entity::getReferenceSymbol() {
  RQ_ASSERT(this->getIsReferenceSymbol(), "not reference symbol");
  return llvm::cast<rq::ReferenceSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSymbol &Entity::getPointerSymbol() {
  RQ_ASSERT(this->getIsPointerSymbol(), "not pointer symbol");
  return llvm::cast<rq::PointerSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::FatPointerSymbol &Entity::getFatPointerSymbol() {
  RQ_ASSERT(this->getIsFatPointerSymbol(), "not fat pointer symbol");
  return llvm::cast<rq::FatPointerSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArraySymbol &Entity::getInferencedCountArraySymbol() {
  RQ_ASSERT(this->getIsInferencedCountArraySymbol(), "not inferenced count array symbol");
  return llvm::cast<rq::InferencedCountArraySymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CountedSubtypeSymbol &Entity::getCountedSubtypeSymbol() {
  RQ_ASSERT(this->getIsCountedSubtypeSymbol(), "not counted subtype symbol");
  return llvm::cast<rq::CountedSubtypeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ArraySymbol &Entity::getArraySymbol() {
  RQ_ASSERT(this->getIsArraySymbol(), "not array symbol");
  return llvm::cast<rq::ArraySymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutSymbol &Entity::getLayoutSymbol() {
  RQ_ASSERT(this->getIsLayoutSymbol(), "not layout symbol");
  return llvm::cast<rq::LayoutSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureSymbol &Entity::getSignatureSymbol() {
  RQ_ASSERT(this->getIsSignatureSymbol(), "not signature symbol");
  return llvm::cast<rq::SignatureSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceSymbol &Entity::getArithmeticSequenceSymbol() {
  RQ_ASSERT(this->getIsArithmeticSequenceSymbol(), "not arithmetic sequence symbol");
  return llvm::cast<rq::ArithmeticSequenceSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &Entity::getFiniteArithmeticProgressionSymbol() {
  RQ_ASSERT(this->getIsFiniteArithmeticProgressionSymbol(), "not finite arithmetic progression symbol");
  return llvm::cast<rq::FiniteArithmeticProgressionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::InfiniteArithmeticProgressionSymbol &Entity::getInfiniteArithmeticProgressionSymbol() {
  RQ_ASSERT(this->getIsInfiniteArithmeticProgressionSymbol(), "not infinite arithmetic progression symbol");
  return llvm::cast<rq::InfiniteArithmeticProgressionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SynonymSymbol &Entity::getSynonymSymbol() {
  RQ_ASSERT(this->getIsSynonymSymbol(), "not synonym symbol");
  return llvm::cast<rq::SynonymSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::LabelSymbol &Entity::getLabelSymbol() {
  RQ_ASSERT(this->getIsLabelSymbol(), "not label symbol");
  return llvm::cast<rq::LabelSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &Entity::getModuleSymbol() {
  RQ_ASSERT(this->getIsModuleSymbol(), "not module symbol");
  return llvm::cast<rq::ModuleSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ImportSymbol &Entity::getImportSymbol() {
  RQ_ASSERT(this->getIsImportSymbol(), "not import symbol");
  return llvm::cast<rq::ImportSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ExtensionSymbol &Entity::getExtensionSymbol() {
  RQ_ASSERT(this->getIsExtensionSymbol(), "not extension symbol");
  return llvm::cast<rq::ExtensionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CodeSymbol &Entity::getCodeSymbol() {
  RQ_ASSERT(this->getIsCodeSymbol(), "not code symbol");
  return llvm::cast<rq::CodeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CategoryDiscriminantSymbol &Entity::getCategoryDiscriminantSymbol() {
  RQ_ASSERT(this->getIsCategoryDiscriminantSymbol(), "not category discriminant symbol");
  return llvm::cast<rq::CategoryDiscriminantSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::DynamicVariableSymbol &Entity::getDynamicVariableSymbol() {
  RQ_ASSERT(this->getIsDynamicVariableSymbol(), "not dynamic variable symbol");
  return llvm::cast<rq::DynamicVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::StaticVariableSymbol &Entity::getStaticVariableSymbol() {
  RQ_ASSERT(this->getIsStaticVariableSymbol(), "not static variable symbol");
  return llvm::cast<rq::StaticVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumeratorSymbol &Entity::getEnumeratorSymbol() {
  RQ_ASSERT(this->getIsEnumeratorSymbol(), "not enumerator symbol");
  return llvm::cast<rq::EnumeratorSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CategoryAlternativeSymbol &Entity::getCategoryAlternativeSymbol() {
  RQ_ASSERT(this->getIsCategoryAlternativeSymbol(), "not category alternative symbol");
  return llvm::cast<rq::CategoryAlternativeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassParameterSymbol &Entity::getClassParameterSymbol() {
  RQ_ASSERT(this->getIsClassParameterSymbol(), "not class parameter symbol");
  return llvm::cast<rq::ClassParameterSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameterSymbol &Entity::getLayoutParameterSymbol() {
  RQ_ASSERT(this->getIsLayoutParameterSymbol(), "not layout parameter symbol");
  return llvm::cast<rq::LayoutParameterSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateParameterSymbol &Entity::getTemplateParameterSymbol() {
  RQ_ASSERT(this->getIsTemplateParameterSymbol(), "not template parameter symbol");
  return llvm::cast<rq::TemplateParameterSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameterSymbol &Entity::getSignatureParameterSymbol() {
  RQ_ASSERT(this->getIsSignatureParameterSymbol(), "not signature parameter symbol");
  return llvm::cast<rq::SignatureParameterSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &Entity::getSymbolTableSymbol() {
  RQ_ASSERT(this->getIsSymbolTableSymbol(), "not symbol table symbol");
  return llvm::cast<rq::SymbolTableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TopSymbol &Entity::getTopSymbol() {
  RQ_ASSERT(this->getIsTopSymbol(), "not top symbol");
  return llvm::cast<rq::TopSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeSymbol &Entity::getScopeSymbol() {
  RQ_ASSERT(this->getIsScopeSymbol(), "not scope symbol");
  return llvm::cast<rq::ScopeSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TableSymbol &Entity::getTableSymbol() {
  RQ_ASSERT(this->getIsTableSymbol(), "not table symbol");
  return llvm::cast<rq::TableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassSymbol &Entity::getClassSymbol() {
  RQ_ASSERT(this->getIsClassSymbol(), "not class symbol");
  return llvm::cast<rq::ClassSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumerationSymbol &Entity::getEnumerationSymbol() {
  RQ_ASSERT(this->getIsEnumerationSymbol(), "not enumeration symbol");
  return llvm::cast<rq::EnumerationSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::CategorySymbol &Entity::getCategorySymbol() {
  RQ_ASSERT(this->getIsCategorySymbol(), "not category symbol");
  return llvm::cast<rq::CategorySymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureSymbol &Entity::getProcedureSymbol() {
  RQ_ASSERT(this->getIsProcedureSymbol(), "not procedure symbol");
  return llvm::cast<rq::ProcedureSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::EntrySymbol &Entity::getEntrySymbol() {
  RQ_ASSERT(this->getIsEntrySymbol(), "not entry symbol");
  return llvm::cast<rq::EntrySymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionSymbol &Entity::getFunctionSymbol() {
  RQ_ASSERT(this->getIsFunctionSymbol(), "not function symbol");
  return llvm::cast<rq::FunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::MethodSymbol &Entity::getMethodSymbol() {
  RQ_ASSERT(this->getIsMethodSymbol(), "not method symbol");
  return llvm::cast<rq::MethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ExtensionFunctionSymbol &Entity::getExtensionFunctionSymbol() {
  RQ_ASSERT(this->getIsExtensionFunctionSymbol(), "not extension function symbol");
  return llvm::cast<rq::ExtensionFunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ExtensionMethodSymbol &Entity::getExtensionMethodSymbol() {
  RQ_ASSERT(this->getIsExtensionMethodSymbol(), "not extension method symbol");
  return llvm::cast<rq::ExtensionMethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::RangerSymbol& Entity::getRangerSymbol() {
  RQ_ASSERT(this->getIsRangerSymbol(), "not ranger symbol");
  return llvm::cast<rq::RangerSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateSymbol &Entity::getTemplateSymbol() {
  RQ_ASSERT(this->getIsTemplateSymbol(), "not template symbol");
  return llvm::cast<rq::TemplateSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateClassSymbol &Entity::getTemplateClassSymbol() {
  RQ_ASSERT(this->getIsTemplateClassSymbol(), "not template class symbol");
  return llvm::cast<rq::TemplateClassSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateEnumerationSymbol &Entity::getTemplateEnumerationSymbol() {
  RQ_ASSERT(this->getIsTemplateEnumerationSymbol(), "not template enumeration symbol");
  return llvm::cast<rq::TemplateEnumerationSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateDynamicVariableSymbol &Entity::getTemplateDynamicVariableSymbol() {
  RQ_ASSERT(this->getIsTemplateDynamicVariableSymbol(), "not template dynamic variable symbol");
  return llvm::cast<rq::TemplateDynamicVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateStaticVariableSymbol &Entity::getTemplateStaticVariableSymbol() {
  RQ_ASSERT(this->getIsTemplateStaticVariableSymbol(), "not template static variable symbol");
  return llvm::cast<rq::TemplateStaticVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateFunctionSymbol &Entity::getTemplateFunctionSymbol() {
  RQ_ASSERT(this->getIsTemplateFunctionSymbol(), "not template function symbol");
  return llvm::cast<rq::TemplateFunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateMethodSymbol &Entity::getTemplateMethodSymbol() {
  RQ_ASSERT(this->getIsTemplateMethodSymbol(), "not template method symbol");
  return llvm::cast<rq::TemplateMethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateExtensionFunctionSymbol &Entity::getTemplateExtensionFunctionSymbol() {
  RQ_ASSERT(this->getIsTemplateExtensionFunctionSymbol(), "not template extension function symbol");
  return llvm::cast<rq::TemplateExtensionFunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateExtensionMethodSymbol &Entity::getTemplateExtensionMethodSymbol() {
  RQ_ASSERT(this->getIsTemplateExtensionMethodSymbol(), "not template extension method symbol");
  return llvm::cast<rq::TemplateExtensionMethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialSymbol &Entity::getPartialSymbol() {
  RQ_ASSERT(this->getIsPartialSymbol(), "not partial symbol");
  return llvm::cast<rq::PartialSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialClassSymbol &Entity::getPartialClassSymbol() {
  RQ_ASSERT(this->getIsPartialClassSymbol(), "not partial class symbol");
  return llvm::cast<rq::PartialClassSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialEnumerationSymbol &Entity::getPartialEnumerationSymbol() {
  RQ_ASSERT(this->getIsPartialEnumerationSymbol(), "not partial enumeration symbol");
  return llvm::cast<rq::PartialEnumerationSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialDynamicVariableSymbol &Entity::getPartialDynamicVariableSymbol() {
  RQ_ASSERT(this->getIsPartialDynamicVariableSymbol(), "not partial dynamic variable symbol");
  return llvm::cast<rq::PartialDynamicVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialStaticVariableSymbol &Entity::getPartialStaticVariableSymbol() {
  RQ_ASSERT(this->getIsPartialStaticVariableSymbol(), "not partial static variable symbol");
  return llvm::cast<rq::PartialStaticVariableSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialFunctionSymbol &Entity::getPartialFunctionSymbol() {
  RQ_ASSERT(this->getIsPartialFunctionSymbol(), "not partial function symbol");
  return llvm::cast<rq::PartialFunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialMethodSymbol &Entity::getPartialMethodSymbol() {
  RQ_ASSERT(this->getIsPartialMethodSymbol(), "not partial method symbol");
  return llvm::cast<rq::PartialMethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialExtensionFunctionSymbol &Entity::getPartialExtensionFunctionSymbol() {
  RQ_ASSERT(this->getIsPartialExtensionFunctionSymbol(), "not partial extension function symbol");
  return llvm::cast<rq::PartialExtensionFunctionSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::PartialExtensionMethodSymbol &Entity::getPartialExtensionMethodSymbol() {
  RQ_ASSERT(this->getIsPartialExtensionMethodSymbol(), "not partial extension method symbol");
  return llvm::cast<rq::PartialExtensionMethodSymbol>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Constant &Entity::getConstant() {
  RQ_ASSERT(this->getIsIntegerConstant(), "not constant");
  return llvm::cast<rq::Constant>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::IntegerConstant &Entity::getIntegerConstant() {
  RQ_ASSERT(this->getIsIntegerConstant(), "not integer constant");
  return llvm::cast<rq::IntegerConstant>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::FloatConstant &Entity::getFloatConstant() {
  RQ_ASSERT(this->getIsFloatConstant(), "not float constant");
  return llvm::cast<rq::FloatConstant>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::StringConstant &Entity::getStringConstant() {
  RQ_ASSERT(this->getIsStringConstant(), "not strinct constant");
  return llvm::cast<rq::StringConstant>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ArrayConstant &Entity::getArrayConstant() {
  RQ_ASSERT(this->getIsArrayConstant(), "not array constant");
  return llvm::cast<rq::ArrayConstant>(*this);
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &Entity::getInstruction() {
  RQ_ASSERT(this->getIsInstruction(), "not instruction");
  return llvm::cast<rq::Instruction>(*this);
}
} // namespace rq
