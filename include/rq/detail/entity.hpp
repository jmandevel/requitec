#pragma once

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode) {
  using O = rq::Opcode;
  switch (opcode) {
  case O::NONE:
    return "none";
  case O::SY_RESULT:
    return "sy_result";
  case O::SY_OUT:
    return "sy_out";
  case O::SY_INFERENCE:
    return "sy_inference";
  case O::SY_SYMBOL_CONSTRAINT:
    return "sy_symbol_constraint";
  case O::SY_TYPE_CONSTRAINT:
    return "sy_type_constraint";
  case O::SY_EXPRESSION:
    return "sy_expression";
  case O::SY_VOID:
    return "sy_void";
  case O::SY_NULL:
    return "sy_null";
  case O::SY_NO_RETURN:
    return "sy_no_return";
  case O::SY_VARIADIC_ARGUMENTS:
    return "sy_variadic_arguments";
  case O::SY_BOOLEAN:
    return "sy_boolean";
  case O::SY_SIGNED_CONSTRAINT:
    return "sy_signed_constraint";
  case O::SY_UNSIGNED_CONSTRAINT:
    return "sy_unsigned_constraint";
  case O::SY_FLOAT_CONSTRAINT:
    return "sy_float_constraint";
  case O::SY_BINARY_CONSTRAINT:
    return "sy_binary_constraint";
  case O::SY_BFLOAT_CONSTRAINT:
    return "sy_bfloat_constraint";
  case O::SY_HALF:
    return "sy_half";
  case O::SY_SINGLE:
    return "sy_single";
  case O::SY_DOUBLE:
    return "sy_double";
  case O::SY_QUADRUPLE:
    return "sy_quadruple";
  case O::SY_BINARY16:
    return "sy_binary16";
  case O::SY_BINARY32:
    return "sy_binary32";
  case O::SY_BINARY64:
    return "sy_binary64";
  case O::SY_BINARY128:
    return "sy_binary128";
  case O::SY_BFLOAT16:
    return "sy_Bfloat16";
  case O::SY_INTEGER_CONSTRAINT:
    return "sy_integer_constraint";
  case O::SY_SIGNED_INTEGER_CONSTRAINT:
    return "sy_signed_integer_constraint";
  case O::SY_UNSIGNED_INTEGER_CONSTRAINT:
    return "sy_unsigned_integer_constraint";
  case O::SY_CODEUNIT_CONSTRAINT:
    return "sy_codeunit_constraint";
  case O::SY_STRING_CONSTRAINT:
    return "sy_string_constraint";
  case O::SY_CHAR:
    return "sy_char";
  case O::SY_ASCII:
    return "sy_ascii";
  case O::SY_UTF8:
    return "sy_utf8";
  case O::SY_SCALED_SIGNED_INTEGER:
    return "sy_scaled_signed_integer";
  case O::SY_SCALED_UNSIGNED_INTEGER:
    return "sy_scaled_unsigned_integer";
  case O::SY_REFERENCE:
    return "sy_reference";
  case O::SY_POINTER:
    return "sy_pointer";
  case O::SY_FAT_POINTER:
    return "sy_fat_pointer";
  case O::SY_INFERENCED_COUNT_ARRAY:
    return "sy_inferenced_count_array";
  case O::SY_ARRAY:
    return "sy_array";
  case O::SY_LAYOUT:
    return "sy_layout";
  case O::SY_CLASS_LAYOUT:
    return "sy_class_layout";
  case O::SY_TEMPLATE_LAYOUT:
    return "sy_template_layout";
  case O::SY_SIGNATURE:
    return "sy_signature";
  case O::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case O::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case O::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case O::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
  case O::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case O::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case O::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";
  case O::SY_MODULE:
    return "sy_module";
  case O::SY_IMPORT:
    return "sy_import";
  case O::SY_CODE:
    return "sy_code";
  case O::SY_CATEGORY_DISCRIMINANT:
    return "sy_category_discriminant";
  case O::SY_LABEL:
    return "sy_label";
  case O::SY_SYNONYM:
    return "sy_synonym";
  case O::SY_TOP:
    return "sy_top";
  case O::SY_SCOPE:
    return "sy_scope";
  case O::SY_NAMESPACE:
    return "sy_namespace";
  case O::SY_CLASS:
    return "sy_class";
  case O::SY_ENUMERATION:
    return "sy_enumeration";
  case O::SY_CATEGORY:
    return "sy_category";
  case O::SY_GLOBAL_VARIABLE:
    return "sy_global_variable";
  case O::SY_LOCAL_VARIABLE:
    return "sy_local_variable";
  case O::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case O::SY_ENUMERATOR:
    return "sy_enumerator";
  case O::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
  case O::SY_ENTRY:
    return "sy_entry";
  case O::SY_FUNCTION:
    return "sy_function";
  case O::SY_METHOD:
    return "sy_method";
  case O::SY_RANGER:
    return "sy_ranger";
  case O::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case O::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case O::SY_EXTENSION_RANGER:
    return "sy_extension_ranger";
  case O::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case O::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case O::SY_TEMPLATE_CATEGORY:
    return "sy_template_category";
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    return "sy_template_global_variable";
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    return "sy_template_static_variable";
  case O::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case O::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case O::SY_TEMPLATE_RANGER:
    return "sy_template_ranger";
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    return "sy_template_extension_ranger";
  case O::SY_PARTIAL_CLASS:
    return "sy_partial_class";
  case O::SY_PARTIAL_ENUMERATION:
    return "sy_partial_enumeration";
  case O::SY_PARTIAL_CATEGORY:
    return "sy_partial_category";
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return "sy_partial_global_variable";
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return "sy_partial_static_variable";
  case O::SY_PARTIAL_FUNCTION:
    return "sy_partial_function";
  case O::SY_PARTIAL_METHOD:
    return "sy_partial_method";
  case O::SY_PARTIAL_RANGER:
    return "sy_partial_ranger";
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return "sy_partial_extension_function";
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return "sy_partial_extension_method";
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return "sy_partial_extension_ranger";
  case O::CT_TYPE:
    return "ct_type";
  case O::CT_EXPRESSION:
    return "ct_expression";
  case O::CT_BOOLEAN:
    return "ct_boolean";
  case O::CT_INTEGER:
    return "ct_integer";
  case O::CT_FLOAT:
    return "ct_float";
  case O::CT_STRING:
    return "ct_string";
  case O::CT_ARRAY:
    return "ct_array";
  case O::IN_EXECUTE:
    return "in_execute";
  case O::IN_CAST:
    return "in_cast";
  case O::IN_LOGICAL_AND:
    return "in_logical_and";
  case O::IN_LOGICAL_OR:
    return "in_logical_or";
  case O::IN_LOGICAL_COMPLEMENT:
    return "in_logical_complement";
  case O::IN_EQ:
    return "in_eq";
  case O::IN_NE:
    return "in_ne";
  case O::IN_GT:
    return "in_gt";
  case O::IN_GE:
    return "in_ge";
  case O::IN_LT:
    return "in_lt";
  case O::IN_LE:
    return "in_le";
  case O::IN_AND:
    return "in_and";
  case O::IN_OR:
    return "in_or";
  case O::IN_XOR:
    return "in_xor";
  case O::IN_COMPLEMENT:
    return "in_complement";
  case O::IN_SHIFT_LEFT:
    return "in_shift_left";
  case O::IN_SHIFT_RIGHT:
    return "in_shift_right";
  case O::IN_ROTATE_LEFT:
    return "in_rotate_left";
  case O::IN_ROTATE_RIGHT:
    return "in_rotate_right";
  case O::IN_ADD:
    return "in_add";
  case O::IN_SUBTRACT:
    return "in_subtract";
  case O::IN_MULTIPLY:
    return "in_multiply";
  case O::IN_DIVIDE:
    return "in_divide";
  case O::IN_MODULUS:
    return "in_modulus";
  case O::IN_NEGATE:
    return "in_negate";
  case O::IN_CONTENT:
    return "in_content";
  case O::IN_ADDRESS:
    return "in_address";
  case O::IN_VALUE:
    return "in_value";
  case O::IN_REF:
    return "in_ref";
  case O::IN_INDEX:
    return "in_index";
  case O::IN_DATA_COUNT:
    return "in_data_count";
  case O::IN_ARG:
    return "in_arg";
  case O::IN_CALL:
    return "in_call";
  case O::IN_MOVE:
    return "in_move";
  case O::IN_COPY:
    return "in_copy";
  case O::IN_RET:
    return "in_ret";
  case O::IN_COND:
    return "in_cond";
  case O::IN_FORK:
    return "in_fork";
  case O::IN_DEBUG_TRAP:
    return "in_debug_trap";
  case O::IN_UNREACHABLE:
    return "in_unreachable";
  case O::IN_ASSUME:
    return "in_assume";

  case O::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OF = rq::OpcodeFlags;
  switch (opcode) {
  case O::NONE:
    return OF::NONE;
  case O::SY_RESULT:
    return OF::SYMBOL;
  case O::SY_OUT:
    return OF::SYMBOL;
  case O::SY_INFERENCE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_SYMBOL_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_TYPE_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_EXPRESSION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_VOID:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_NULL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_NO_RETURN:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_VARIADIC_ARGUMENTS:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_BOOLEAN:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_SIGNED_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_SIGNED;
  case O::SY_UNSIGNED_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_FLOAT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_BINARY_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BFLOAT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_HALF:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_SINGLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_DOUBLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_QUADRUPLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_BINARY16:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY32:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY64:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY128:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BFLOAT16:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_SIGNED;
  case O::SY_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER;
  case O::SY_SIGNED_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER | OF::SY_SIGNED;
  case O::SY_UNSIGNED_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER | OF::SY_UNSIGNED;
  case O::SY_CODEUNIT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_CODEUNIT;
  case O::SY_STRING_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_CHAR:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT |
           OF::SY_PLATFORM_CHANGING;
  case O::SY_ASCII:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_UTF8:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_SCALED_SIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_BUILTIN | OF::SY_TYPE | OF::SY_INTEGER |
           OF::SY_SIGNED;
  case O::SY_SCALED_UNSIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_BUILTIN | OF::SY_TYPE | OF::SY_INTEGER |
           OF::SY_UNSIGNED;
  case O::SY_REFERENCE:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_POINTER:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_FAT_POINTER:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_INFERENCED_COUNT_ARRAY:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_ARRAY:
    return OF::SYMBOL | OF::SY_COUNTED_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_CLASS_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_TEMPLATE_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_SIGNATURE:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_CLASS_PARAMETER:
    return OF::SYMBOL;
  case O::SY_LAYOUT_PARAMETER:
    return OF::SYMBOL;
  case O::SY_TEMPLATE_PARAMETER:
    return OF::SYMBOL;
  case O::SY_SIGNATURE_PARAMETER:
    return OF::SYMBOL;
  case O::SY_ARITHMETIC_INTERVAL:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_FINITE_ARITHMETIC_PROGRESSION:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_MODULE:
    return OF::SYMBOL;
  case O::SY_IMPORT:
    return OF::SYMBOL;
  case O::SY_CODE:
    return OF::SYMBOL;
  case O::SY_CATEGORY_DISCRIMINANT:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_LABEL:
    return OF::SYMBOL;
  case O::SY_SYNONYM:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TIN_OF_FRAME;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE;
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TIN_OF_FRAME;
  case O::SY_CLASS:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_ENUMERATION:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_CATEGORY:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_DYNAMIC_VARIABLE |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_LOCAL_VARIABLE:
    return OF::SYMBOL | OF::SY_DYNAMIC_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_CATEGORY_ALTERNATIVE:
    return OF::SYMBOL;
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_RANGER:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_TEMPLATE_CLASS:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_ENUMERATION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_CATEGORY:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_RANGER:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_PARTIAL_CLASS:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_ENUMERATION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_CATEGORY:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_FUNCTION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_METHOD:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_RANGER:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::CT_TYPE:
    return OF::CONSTANT;
  case O::CT_EXPRESSION:
    return OF::CONSTANT;
  case O::CT_BOOLEAN:
    return OF::CONSTANT;
  case O::CT_INTEGER:
    return OF::CONSTANT;
  case O::CT_FLOAT:
    return OF::CONSTANT;
  case O::CT_STRING:
    return OF::CONSTANT;
  case O::CT_ARRAY:
    return OF::CONSTANT;
  case O::IN_EXECUTE:
    return OF::IN_BINARY;
  case O::IN_CAST:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_AND:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_OR:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_COMPLEMENT:
    return OF::IN_UNARY;
  case O::IN_EQ:
    return OF::IN_BINARY;
  case O::IN_NE:
    return OF::IN_BINARY;
  case O::IN_GT:
    return OF::IN_BINARY;
  case O::IN_GE:
    return OF::IN_BINARY;
  case O::IN_LT:
    return OF::IN_BINARY;
  case O::IN_LE:
    return OF::IN_BINARY;
  case O::IN_AND:
    return OF::IN_BINARY;
  case O::IN_OR:
    return OF::IN_BINARY;
  case O::IN_XOR:
    return OF::IN_BINARY;
  case O::IN_COMPLEMENT:
    return OF::IN_UNARY;
  case O::IN_SHIFT_LEFT:
    return OF::IN_BINARY;
  case O::IN_SHIFT_RIGHT:
    return OF::IN_BINARY;
  case O::IN_ROTATE_LEFT:
    return OF::IN_BINARY;
  case O::IN_ROTATE_RIGHT:
    return OF::IN_BINARY;
  case O::IN_ADD:
    return OF::IN_BINARY;
  case O::IN_SUBTRACT:
    return OF::IN_BINARY;
  case O::IN_MULTIPLY:
    return OF::IN_BINARY;
  case O::IN_DIVIDE:
    return OF::IN_BINARY;
  case O::IN_MODULUS:
    return OF::IN_BINARY;
  case O::IN_NEGATE:
    return OF::IN_UNARY;
  case O::IN_CONTENT:
    return OF::IN_UNARY;
  case O::IN_ADDRESS:
    return OF::IN_UNARY;
  case O::IN_VALUE:
    return OF::IN_UNARY;
  case O::IN_REF:
    return OF::IN_UNARY;
  case O::IN_INDEX:
    return OF::IN_BINARY;
  case O::IN_DATA_COUNT:
    return OF::IN_UNARY;
  case O::IN_ARG:
    return OF::IN_BINARY;
  case O::IN_CALL:
    return OF::IN_BINARY;
  case O::IN_MOVE:
    return OF::IN_BINARY;
  case O::IN_COPY:
    return OF::IN_BINARY;
  case O::IN_RET:
    return OF::IN_NULLARY;
  case O::IN_COND:
    return OF::IN_BINARY;
  case O::IN_FORK:
    return OF::IN_BINARY;
  case O::IN_DEBUG_TRAP:
    return OF::IN_NULLARY;
  case O::IN_UNREACHABLE:
    return OF::IN_NULLARY;
  case O::IN_ASSUME:
    return OF::IN_UNARY;
  case O::LAST:
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
  return rq::getHasNone(flags, rq::OpcodeFlags::NOT_INSTRUCTION_MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIMPLE_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SCALED_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNARY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_COUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterListSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER_LIST_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_DYNAMIC_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARTIAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_TEMPLATE_ALTERNATIVE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONSTRAINT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PLATFORM_CHANGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags,
                       rq::OpcodeFlags::SY_INTEGER | rq::OpcodeFlags::SY_FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_BINARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CODEUNIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNSIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIGNED |
                                  rq::OpcodeFlags::SY_INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNSIGNED |
                                  rq::OpcodeFlags::SY_INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TIN_OF_FRAME);
}

[[nodiscard]] inline rq::Opcode getTemplate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_TEMPLATE_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_TEMPLATE_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_TEMPLATE_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_TEMPLATE_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_TEMPLATE_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_TEMPLATE_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_TEMPLATE_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_TEMPLATE_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_TEMPLATE_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_TEMPLATE_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_TEMPLATE_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Opcode getPartial(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_PARTIAL_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_PARTIAL_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_PARTIAL_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_PARTIAL_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_PARTIAL_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_PARTIAL_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_PARTIAL_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_PARTIAL_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_PARTIAL_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_PARTIAL_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_PARTIAL_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Opcode getFull(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_NULLARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_UNARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_BINARY);
}

inline InitialExpression::InitialExpression(const rq::Expression &expression)
    : _expression_ptr(&expression) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
InitialExpression::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

inline LateExpression::LateExpression(const rq::Expression &expression)
    : InitialExpression(expression) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool LateExpression::getHasExpression() {
  return this->_expression_ptr != nullptr;
}

RQ_ALWAYS_INLINE void
LateExpression::setExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE void
LateExpression::setExpression(const rq::Expression *expression_ptr) {
  rq::assignSingleValue(this->_expression_ptr, expression_ptr);
}

inline ReplacableExpression::ReplacableExpression(
    const rq::Expression &expression)
    : LateExpression(expression) {}

RQ_ALWAYS_INLINE const rq::Expression &
ReplacableExpression::replaceExpression(const rq::Expression &expression) {
  return rq::replaceValue(this->_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE const rq::Expression &
ReplacableExpression::replaceExpression(const rq::Expression *expression) {
  return rq::replaceValue(this->_expression_ptr, expression);
}

RQ_ALWAYS_INLINE const rq::Expression *
ReplacableExpression::replaceExpressionPtr(const rq::Expression &expression) {
  return rq::replaceValuPtr(this->_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE const rq::Expression *
ReplacableExpression::replaceExpressionPtr(const rq::Expression *expression) {
  return rq::replaceValuPtr(this->_expression_ptr, expression);
}

inline InitialExpressionFlags::InitialExpressionFlags(rq::ExpressionFlags flags)
    : _expression_flags(flags) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
InitialExpressionFlags::getExpressionFlags() const {
  return this->_expression_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasOpaque() const {
  return rq::getHasOpaque(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasOutside() const {
  return rq::getHasOutside(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasStatic() const {
  return rq::getHasStatic(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasCapture() const {
  return rq::getHasCapture(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasEager() const {
  return rq::getHasEager(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasMayParent() const {
  return rq::getHasMayParent(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasParent() const {
  return rq::getHasParent(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasAbstract() const {
  return rq::getHasAbstract(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasVirtual() const {
  return rq::getHasVirtual(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasOverride() const {
  return rq::getHasOverride(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasLocation() const {
  return rq::getHasLocation(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasMangle() const {
  return rq::getHasMangle(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialExpressionFlags::getHasPack() const {
  return rq::getHasPack(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasLabel() const {
  return rq::getHasLabel(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasTemplate() const {
  return rq::getHasTemplate(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasLikely() const {
  return rq::getHasLikely(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasUnlikely() const {
  return rq::getHasUnlikely(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasDepreciated() const {
  return rq::getHasDepreciated(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasExport() const {
  return rq::getHasExport(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasPublic() const {
  return rq::getHasPublic(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasProtected() const {
  return rq::getHasProtected(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasMayCopy() const {
  return rq::getHasMayCopy(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasStableAddress() const {
  return rq::getHasStableAddress(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialExpressionFlags::getHasOk() const {
  return rq::getHasOk(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialExpressionFlags::getHasAttribute(
    rq::ExpressionAttribute attribute) const {
  return rq::getHasAttribute(this->_expression_flags, attribute);
}

inline InitialModuleMember::InitialModuleMember(rq::Module &module)
    : _containing_module_ptr(&module) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
InitialModuleMember::getContainingModule() const {
  return rq::dereferencePtr(this->_containing_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &
InitialModuleMember::getContainingModule() {
  return rq::dereferencePtr(this->_containing_module_ptr);
}

inline SymbolTableMember::SymbolTableMember(rq::SymbolTable &containing_table)
    : _containing_symbol_table_ptr(&containing_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableMember::getHasContainingSymbolTable() const {
  return this->_containing_symbol_table_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
SymbolTableMember::getContainingSymbolTable() const {
  return rq::dereferencePtr(this->_containing_symbol_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolTableMember::getContainingSymbolTable() {
  return rq::dereferencePtr(this->_containing_symbol_table_ptr);
}

RQ_ALWAYS_INLINE
SymbolTableHosted::SymbolTableHosted(rq::SymbolTable &hosting_table)
    : _hosting_symbol_table_ptr(&hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
SymbolTableHosted::getHostingSymbolTable() const {
  return rq::dereferencePtr(this->_hosting_symbol_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolTableHosted::getHostingSymbolTable() {
  return rq::dereferencePtr(this->_hosting_symbol_table_ptr);
}

inline InitialNamed::InitialNamed(llvm::StringRef name) : _name(name) {
  RQ_ASSERT(!name.empty(), "name is empty");
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef InitialNamed::getName() const {
  return this->_name;
}

inline InitialMaybeNamed::InitialMaybeNamed(llvm::StringRef name)
    : _name(name) {
  RQ_ASSERT(!name.empty(), "name is empty");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialMaybeNamed::getHasName() const {
  return !this->_name.empty();
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
InitialMaybeNamed::getName() const {
  return this->_name;
}

inline Entity::Entity(rq::Opcode opcode) : _opcode(opcode) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::operator==(const Entity &other) const {
  return this == &other;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Entity::operator!=(const Entity &other) const {
  return this != &other;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getOpcode() const {
  return this->_opcode;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getHasTemplateAlternative() const {
  return rq::getHasTemplateAlternative(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsType() const {
  return rq::getIsType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSubtype() const {
  return rq::getIsSubtype(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsParameter() const {
  return rq::getIsParameter(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsParameterListSubtype() const {
  return rq::getIsParameterListSubtype(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsConstraint() const {
  return rq::getIsConstraint(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsPlatformChanging() const {
  if (rq::getIsScaledBuiltin(this->getOpcode())) {
    // TODO
  }
  return rq::getIsPlatformChanging(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsNumeric() const {
  return rq::getIsNumeric(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsInteger() const {
  return rq::getIsInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsFloat() const {
  return rq::getIsFloat(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsBinary() const {
  return rq::getIsBinary(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsCodeunit() const {
  return rq::getIsCodeunit(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSigned() const {
  return rq::getIsSigned(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsUnsigned() const {
  return rq::getIsUnsigned(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsSignedInteger() const {
  return rq::getIsSignedInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsUnsignedInteger() const {
  return rq::getIsUnsignedInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Entity::getIsTopOfFrame() const {
  return rq::getIsTopOfFrame(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getTemplateKeyword() const {
  return rq::getTemplate(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getPartialKeyword() const {
  return rq::getPartial(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode Entity::getFullKeyword() const {
  return rq::getFull(this->getOpcode());
}

[[nodiscard]] inline bool Entity::classof(const Entity *entity) {
  std::ignore = entity;
  return true;
}

inline Symbol::Symbol(rq::Opcode opcode) : Entity(opcode) {}

[[nodiscard]] inline bool Symbol::classof(const Entity *entity) {
  return rq::getIsSymbol(rq::dereferencePtr(entity).getOpcode());
}

inline Result::Result() : Symbol(rq::Opcode::SY_RESULT) {}

[[nodiscard]] inline bool Result::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_RESULT;
}

inline Out::Out() : Symbol(rq::Opcode::SY_OUT) {}

[[nodiscard]] inline bool Out::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_OUT;
}

inline SimpleBuiltin::SimpleBuiltin(rq::Opcode opcode) : Symbol(opcode) {}

[[nodiscard]] inline bool SimpleBuiltin::classof(const Entity *entity) {
  return rq::getIsSimpleBuiltin(rq::dereferencePtr(entity).getOpcode());
}

inline Inference::Inference() : SimpleBuiltin(rq::Opcode::SY_INFERENCE) {}

[[nodiscard]] inline bool Inference::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_INFERENCE;
}

inline SymbolConstraint::SymbolConstraint()
    : SimpleBuiltin(rq::Opcode::SY_SYMBOL_CONSTRAINT) {}

[[nodiscard]] inline bool SymbolConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SYMBOL_CONSTRAINT;
}

inline TypeConstraint::TypeConstraint()
    : SimpleBuiltin(rq::Opcode::SY_TYPE_CONSTRAINT) {}

[[nodiscard]] inline bool TypeConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TYPE_CONSTRAINT;
}

inline Void::Void() : SimpleBuiltin(rq::Opcode::SY_VOID) {}

[[nodiscard]] inline bool Void::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_VOID;
}

inline Null::Null() : SimpleBuiltin(rq::Opcode::SY_NULL) {}

[[nodiscard]] inline bool Null::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NULL;
}

inline NoReturn::NoReturn() : SimpleBuiltin(rq::Opcode::SY_NO_RETURN) {}

[[nodiscard]] inline bool NoReturn::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NO_RETURN;
}

inline VariadicArguments::VariadicArguments()
    : SimpleBuiltin(rq::Opcode::SY_VARIADIC_ARGUMENTS) {}

[[nodiscard]] inline bool VariadicArguments::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_VARIADIC_ARGUMENTS;
}

inline Boolean::Boolean() : SimpleBuiltin(rq::Opcode::SY_BOOLEAN) {}

[[nodiscard]] inline bool Boolean::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BOOLEAN;
}

inline SignedConstraint::SignedConstraint()
    : SimpleBuiltin(rq::Opcode::SY_SIGNED_CONSTRAINT) {}

[[nodiscard]] inline bool SignedConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SIGNED_CONSTRAINT;
}

RQ_ALWAYS_INLINE rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  this->_table_ptr =
      rq::dereferencePtr(this->_table_ptr)._containing_symbol_table_ptr;
  return *this;
}

inline UnsignedConstraint::UnsignedConstraint()
    : SimpleBuiltin(rq::Opcode::SY_UNSIGNED_CONSTRAINT) {}

[[nodiscard]] inline bool UnsignedConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_UNSIGNED_CONSTRAINT;
}

inline FloatConstraint::FloatConstraint()
    : SimpleBuiltin(rq::Opcode::SY_FLOAT_CONSTRAINT) {}

[[nodiscard]] inline bool FloatConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_FLOAT_CONSTRAINT;
}

inline BinaryConstraint::BinaryConstraint()
    : SimpleBuiltin(rq::Opcode::SY_BINARY_CONSTRAINT) {}

[[nodiscard]] inline bool BinaryConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_BINARY_CONSTRAINT;
}

inline BfloatConstraint::BfloatConstraint()
    : SimpleBuiltin(rq::Opcode::SY_BFLOAT_CONSTRAINT) {}

[[nodiscard]] inline bool BfloatConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_BFLOAT_CONSTRAINT;
}

inline Half::Half() : SimpleBuiltin(rq::Opcode::SY_HALF) {}

[[nodiscard]] inline bool Half::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_HALF;
}

inline Single::Single() : SimpleBuiltin(rq::Opcode::SY_SINGLE) {}

[[nodiscard]] inline bool Single::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SINGLE;
}

inline Double::Double() : SimpleBuiltin(rq::Opcode::SY_DOUBLE) {}

[[nodiscard]] inline bool Double::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_DOUBLE;
}

} // namespace rq