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
  case O::SY_INTEGER_LITERAL:
    return "sy_integer_literal";
  case O::SY_FLOAT_LITERAL:
    return "sy_float_literal";
  case O::SY_STRING_LITERAL:
    return "sy_string_literal";
  case O::SY_CODEUNIT_LITERAL:
    return "sy_codeunit_literal";
  case O::SY_INFERENCE:
    return "sy_inference";
  case O::SY_SYMBOL_REFLECTION:
    return "sy_symbol_reflection";
  case O::SY_TYPE:
    return "sy_type";
  case O::SY_EXPRESSION:
    return "sy_expression";
  case O::SY_VOID:
    return "sy_void";
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
    return "sy_bfloat16";
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
  case O::SY_INTERPOLATED_STRING_CONSTRAINT:
    return "sy_interpolated_string_constraint";
  case O::SY_CHAR:
    return "sy_char";
  case O::SY_ASCII:
    return "sy_ascii";
  case O::SY_UTF8:
    return "sy_utf8";
  case O::SY_EXPRESSION_ATTRIBUTE_CONSTRAINT:
    return "sy_expression_attribute_constraint";
  case O::SY_LABELING:
    return "sy_labeling";
  case O::SY_VISIBILITY:
    return "sy_visibility";
  case O::SY_SCOPING:
    return "sy_scoping";
  case O::SY_AVAILABILITY:
    return "sy_availability";
  case O::SY_ACCESSIBILITY:
    return "sy_accessibility";
  case O::SY_PROPERTY_MUTABILITY:
    return "sy_property_mutability";
  case O::SY_EXPORTING:
    return "sy_exporting";
  case O::SY_RUNTIME:
    return "sy_runtime";
  case O::SY_CAPTURING:
    return "sy_capturing";
  case O::SY_EVALUATION_TIME:
    return "sy_evaluation_time";
  case O::SY_PARENTABILITY:
    return "sy_parentability";
  case O::SY_PROPERTY_ASSOCIATION:
    return "sy_property_association";
  case O::SY_TANGIBILITY:
    return "sy_tangibility";
  case O::SY_OVERRIDING:
    return "sy_overriding";
  case O::SY_INLINING:
    return "sy_inlining";
  case O::SY_MANGLING:
    return "sy_mangling";
  case O::SY_PACKING:
    return "sy_packing";
  case O::SY_TEMPLATING:
    return "sy_templating";
  case O::SY_LIKELYHOOD:
    return "sy_likelyhood";
  case O::SY_SUPPORT:
    return "sy_support";
  case O::SY_COPYABILITY:
    return "sy_copyability";
  case O::SY_ADDRESS_STABILITY:
    return "sy_address_stability";
  case O::SY_CLEANUP:
    return "sy_cleanup";
  case O::SY_RESULT_STATUS:
    return "sy_result_status";
  case O::SY_TYPE_ATTRIBUTE_CONSTRAINT:
    return "sy_type_attribute_constraint";
  case O::SY_MUTABILITY:
    return "sy_mutability";
  case O::SY_VOLATILITY:
    return "sy_volatility";
  case O::SY_DETERMINICITY:
    return "sy_determinicity";
  case O::SY_ATOMICITY:
    return "sy_atomicity";
  case O::SY_NULL_TERMINATION:
    return "sy_null_termination";
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
  case O::SY_LOCAL:
    return "sy_local";
  case O::SY_STATIC:
    return "sy_static";
  case O::SY_ENUMERATOR:
    return "sy_enumerator";
  case O::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
  case O::SY_TOP:
    return "sy_top";
  case O::SY_GLOBAL:
    return "sy_global";
  case O::SY_GLOBAL_STATIC:
    return "sy_global_static";
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
  case O::SY_RANGER_POLYMORPH:
    return "sy_ranger_polymorph";
  case O::SY_PROCEDURE_POLYMORPH:
    return "sy_ranger_polymorph";
  case O::SY_CLASS_POLYMORPH:
    return "sy_class_polymorph";
  case O::SY_ENUMERATION_POLYMORPH:
    return "sy_enumeration_polymorph";
  case O::SY_CATEGORY_POLYMORPH:
    return "sy_category_polymorph";
  case O::SY_GLOBAL_POLYMORPH:
    return "sy_global_polymorph";
  case O::SY_GLOBAL_STATIC_POLYMORPH:
    return "sy_global_static_polymorph";
  case O::SY_FORWARD_RANGER:
    return "sy_forward_ranger";
  case O::SY_BACKWARD_RANGER:
    return "sy_backward_ranger";
  case O::SY_ENTRY:
    return "sy_entry";
  case O::SY_FUNCTION:
    return "sy_function";
  case O::SY_METHOD:
    return "sy_method";
  case O::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case O::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case O::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case O::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case O::SY_TEMPLATE_CATEGORY:
    return "sy_template_category";
  case O::SY_TEMPLATE_GLOBAL:
    return "sy_template_global";
  case O::SY_TEMPLATE_GLOBAL_STATIC:
    return "sy_template_global_static";
  case O::SY_TEMPLATE_FORWARD_RANGER:
    return "sy_template_forward_ranger";
  case O::SY_TEMPLATE_BACKWARD_RANGER:
    return "sy_template_backward_ranger";
  case O::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case O::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";
  case O::SY_SPECIALIZED_CLASS:
    return "sy_specialized_class";
  case O::SY_SPECIALIZED_ENUMERATION:
    return "sy_specialized_enumeration";
  case O::SY_SPECIALIZED_CATEGORY:
    return "sy_specialized_category";
  case O::SY_SPECIALIZED_GLOBAL:
    return "sy_specialized_global";
  case O::SY_SPECIALIZED_GLOBAL_STATIC:
    return "sy_specialized_global_static";
  case O::SY_SPECIALIZED_FORWARD_RANGER:
    return "sy_specialized_forward_ranger";
  case O::SY_SPECIALIZED_BACKWARD_RANGER:
    return "sy_specialized_backward_ranger";
  case O::SY_SPECIALIZED_FUNCTION:
    return "sy_specialized_function";
  case O::SY_SPECIALIZED_METHOD:
    return "sy_specialized_method";
  case O::SY_SPECIALIZED_EXTENSION_FUNCTION:
    return "sy_specialized_extension_function";
  case O::SY_SPECIALIZED_EXTENSION_METHOD:
    return "sy_specialized_extension_method";
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
  case O::CT_EXPRESSION_ATTRIBUTE:
    return "ct_expression_attribute";
  case O::CT_TYPE_ATTRIBUTE:
    return "ct_type_attribute";
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
  case O::IN_DEBUG_BREAK:
    return "in_debug_break";
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
  case O::SY_INTEGER_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_LITERAL;
  case O::SY_FLOAT_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_LITERAL;
  case O::SY_STRING_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_LITERAL;
  case O::SY_CODEUNIT_LITERAL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_LITERAL;
  case O::SY_INFERENCE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_SYMBOL_REFLECTION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_TYPE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_EXPRESSION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_VOID:
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
  case O::SY_INTERPOLATED_STRING_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_CHAR:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT |
           OF::SY_PLATFORM_CHANGING;
  case O::SY_ASCII:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_UTF8:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_EXPRESSION_ATTRIBUTE_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE | OF::SY_CONSTRAINT;
  case O::SY_LABELING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_VISIBILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_SCOPING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_AVAILABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_ACCESSIBILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_PROPERTY_MUTABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_EXPORTING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_RUNTIME:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_CAPTURING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_EVALUATION_TIME:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_PARENTABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_PROPERTY_ASSOCIATION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_TANGIBILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_OVERRIDING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_INLINING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_MANGLING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_PACKING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_TEMPLATING:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_LIKELYHOOD:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_SUPPORT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_COPYABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_ADDRESS_STABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_CLEANUP:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_RESULT_STATUS:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_EXPRESSION_ATTRIBUTE;
  case O::SY_TYPE_ATTRIBUTE_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE | OF::SY_CONSTRAINT;
  case O::SY_MUTABILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE;
  case O::SY_VOLATILITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE;
  case O::SY_DETERMINICITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE;
  case O::SY_ATOMICITY:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE;
  case O::SY_NULL_TERMINATION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_TYPE_ATTRIBUTE;
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
  case O::SY_LOCAL:
    return OF::SYMBOL;
  case O::SY_STATIC:
    return OF::SYMBOL;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_CATEGORY_ALTERNATIVE:
    return OF::SYMBOL;
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_NO_ASCENDING;
  case O::SY_GLOBAL:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_GLOBAL_STATIC:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_TABLE;
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_NO_ASCENDING;
  case O::SY_CLASS:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_TYPE | OF::SY_NO_ASCENDING |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_ENUMERATION:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_TYPE | OF::SY_NO_ASCENDING |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_CATEGORY:
    return OF::SYMBOL | OF::SY_TABLE | OF::SY_TYPE | OF::SY_NO_ASCENDING |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_RANGER_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_PROCEDURE_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_CLASS_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_ENUMERATION_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_CATEGORY_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_GLOBAL_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_GLOBAL_STATIC_POLYMORPH:
    return OF::SYMBOL | OF::SY_POLYMORPH;
  case O::SY_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_RANGER | OF::SY_NO_ASCENDING_INTO_TOP;
  case O::SY_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_RANGER | OF::SY_NO_ASCENDING_INTO_TOP;
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_NO_ASCENDING_INTO_TOP;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_NO_ASCENDING_INTO_TOP |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_NO_ASCENDING_INTO_TOP |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_NO_ASCENDING_INTO_TOP |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_NO_ASCENDING_INTO_TOP |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_TEMPLATE_CLASS:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_ENUMERATION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_CATEGORY:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_GLOBAL:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_GLOBAL_STATIC:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_TEMPLATE_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_SPECIALIZED_CLASS:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_ENUMERATION:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_CATEGORY:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_GLOBAL:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_GLOBAL_STATIC:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_FORWARD_RANGER:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_BACKWARD_RANGER:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_FUNCTION:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_METHOD:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
  case O::SY_SPECIALIZED_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_SPECIALIZED;
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
  case O::CT_EXPRESSION_ATTRIBUTE:
    return OF::CONSTANT;
  case O::CT_TYPE_ATTRIBUTE:
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
  case O::IN_DEBUG_BREAK:
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_RANGER) &&
         rq::getHasNone(flags, rq::OpcodeFlags::SY_TEMPLATE |
                                   rq::OpcodeFlags::SY_SPECIALIZED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE) &&
         rq::getHasNone(flags, rq::OpcodeFlags::SY_TEMPLATE |
                                   rq::OpcodeFlags::SY_SPECIALIZED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateRanger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_RANGER |
                                  rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateProcedure(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE |
                                  rq::OpcodeFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecialized(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SPECIALIZED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialRanger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SPECIALIZED |
                                  rq::OpcodeFlags::SY_RANGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialProcedure(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SPECIALIZED |
                                  rq::OpcodeFlags::SY_PROCEDURE);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttribute(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_EXPRESSION_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE_ATTRIBUTE);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscending(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_NO_ASCENDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscendingIntoTop(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_NO_ASCENDING_INTO_TOP);
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
InitialExpressionFlags::getHasTransparent() const {
  return rq::getHasTransparent(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasOpaque() const {
  return rq::getHasOpaque(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasInsideScope() const {
  return rq::getHasInsideScope(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasOutsideScope() const {
  return rq::getHasOutsideScope(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasGlobal() const {
  return rq::getHasGlobal(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasLocal() const {
  return rq::getHasLocal(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasPrivate() const {
  return rq::getHasPrivate(this->_expression_flags);
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
InitialExpressionFlags::getHasNoPartialMutate() const {
  return rq::getHasNoPartialMutate(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasPartialMutate() const {
  return rq::getHasPartialMutate(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoExport() const {
  return rq::getHasNoExport(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasExport() const {
  return rq::getHasExport(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasDynamic() const {
  return rq::getHasDynamic(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasStatic() const {
  return rq::getHasStatic(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoCapture() const {
  return rq::getHasNoCapture(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasCapture() const {
  return rq::getHasCapture(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialExpressionFlags::getHasLazy() const {
  return rq::getHasLazy(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasEager() const {
  return rq::getHasEager(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoParent() const {
  return rq::getHasNoParent(this->_expression_flags);
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
InitialExpressionFlags::getHasMixin() const {
  return rq::getHasMixin(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasTangible() const {
  return rq::getHasTangible(this->_expression_flags);
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
InitialExpressionFlags::getHasNoInline() const {
  return rq::getHasNoInline(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasInline() const {
  return rq::getHasInline(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasImplicitMangle() const {
  return rq::getHasImplicitMangle(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasExplicitMangle() const {
  return rq::getHasExplicitMangle(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoPack() const {
  return rq::getHasNoPack(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool InitialExpressionFlags::getHasPack() const {
  return rq::getHasPack(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasLabel() const {
  return rq::getHasLabel(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoTemplate() const {
  return rq::getHasNoTemplate(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasTemplate() const {
  return rq::getHasTemplate(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasSpecialize() const {
  return rq::getHasSpecialize(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasEquivocal() const {
  return rq::getHasEquivocal(this->_expression_flags);
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
InitialExpressionFlags::getHasSupported() const {
  return rq::getHasSupported(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasDepreciated() const {
  return rq::getHasDepreciated(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasExperimental() const {
  return rq::getHasExperimental(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNoCopy() const {
  return rq::getHasNoCopy(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasMayCopy() const {
  return rq::getHasMayCopy(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasUnstableAddress() const {
  return rq::getHasUnstableAddress(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasStableAddress() const {
  return rq::getHasStableAddress(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasExplicitDrop() const {
  return rq::getHasExplicitDrop(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasImplicitDrop() const {
  return rq::getHasImplicitDrop(this->_expression_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
InitialExpressionFlags::getHasNotOk() const {
  return rq::getHasNotOk(this->_expression_flags);
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

inline TableMember::TableMember(rq::Table &containing_table)
    : _containing_table_ptr(&containing_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool TableMember::getHasContainingTable() const {
  return this->_containing_table_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
TableMember::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table &TableMember::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

RQ_ALWAYS_INLINE
TableHosted::TableHosted(rq::Table &hosting_table)
    : _hosting_table_ptr(&hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
TableHosted::getHostingTable() const {
  return rq::dereferencePtr(this->_hosting_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table &TableHosted::getHostingTable() {
  return rq::dereferencePtr(this->_hosting_table_ptr);
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

[[nodiscard]] inline bool Entity::classof(const Entity *entity) {
  std::ignore = entity;
  return true;
}

RQ_ALWAYS_INLINE DeclarationInfo::DeclarationInfo(rq::Opcode opcode)
    : _opcode(opcode) {
  RQ_ASSERT(rq::getIsSymbol(opcode), "not symbol");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode DeclarationInfo::getOpcode() {
  return this->_opcode;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
DeclarationInfo::getOpcodeName() {
  return rq::getName(this->_opcode);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool DeclarationInfo::getHasName() const {
  return !this->_name.empty();
}

RQ_ALWAYS_INLINE void DeclarationInfo::setName(llvm::StringRef name) {
  RQ_ASSERT(!this->getHasName(), "already has name");
  this->_name = name;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
DeclarationInfo::getName() const {
  if (this->_name.empty()) {
    return "[no_name]";
  } else {
    return this->_name;
  }
}

[[nodiscard]] RQ_ALWAYS_INLINE bool DeclarationInfo::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

RQ_ALWAYS_INLINE void
DeclarationInfo::setExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
DeclarationInfo::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DeclarationInfo::getHasContainingTable() const {
  return this->_containing_table_ptr != nullptr;
}

RQ_ALWAYS_INLINE void DeclarationInfo::setContainingTable(rq::Table &table) {
  rq::assignSingleValue(this->_containing_table_ptr, &table);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
DeclarationInfo::getContainingTable() const {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table &
DeclarationInfo::getContainingTable() {
  return rq::dereferencePtr(this->_containing_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
DeclarationInfo::getHasContainingModule() const {
  return this->_containing_module_ptr != nullptr;
}

RQ_ALWAYS_INLINE void DeclarationInfo::setContainingModule(rq::Module &module) {
  rq::assignSingleValue(this->_containing_module_ptr, &module);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &
DeclarationInfo::getContainingModule() const {
  return rq::dereferencePtr(this->_containing_module_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module &
DeclarationInfo::getContainingModule() {
  return rq::dereferencePtr(this->_containing_module_ptr);
}

inline Symbol::Symbol(rq::Opcode opcode) : Entity(opcode) {}

[[nodiscard]] inline rq::DeclarationInfo Symbol::getDeclarationInfo() {
  using O = rq::Opcode;
  rq::DeclarationInfo info{this->getOpcode()};
  switch (this->getOpcode()) {
  case O::SY_IMPORT: {
    rq::Import &import = llvm::cast<rq::Import>(*this);
    info.setContainingModule(import.getContainingModule());
    info.setContainingTable(import.getContainingModule().getTop());
    info.setExpression(import.getExpression());
  } break;
  case O::SY_CODE: {
    rq::Code &code = llvm::cast<rq::Code>(*this);
    info.setContainingModule(code.getContainingModule());
    info.setContainingTable(code.getContainingTable());
    info.setName(code.getName());
    info.setExpression(code.getExpression());
  } break;
  case O::SY_LABEL: {
    rq::Label &label = llvm::cast<rq::Label>(*this);
    info.setContainingModule(label.getContainingModule());
    info.setContainingTable(label.getContainingTable());
    info.setName(label.getName());
    info.setExpression(label.getExpression());
  } break;
  case O::SY_LOCAL: {
    rq::Local &local = llvm::cast<rq::Local>(*this);
    info.setContainingModule(local.getContainingModule());
    info.setContainingTable(local.getContainingTable());
    info.setName(local.getName());
    info.setExpression(local.getExpression());
  } break;
  case O::SY_STATIC: {
    rq::Static &static_ = llvm::cast<rq::Static>(*this);
    info.setContainingModule(static_.getContainingModule());
    info.setContainingTable(static_.getContainingTable());
    info.setName(static_.getName());
    info.setExpression(static_.getExpression());
  } break;
  case O::SY_ENUMERATOR: {
    rq::Enumerator &enumerator = llvm::cast<rq::Enumerator>(*this);
    info.setContainingModule(enumerator.getContainingModule());
    info.setContainingTable(enumerator.getContainingTable());
    info.setName(enumerator.getName());
    info.setExpression(enumerator.getExpression());
  } break;
  case O::SY_CATEGORY_ALTERNATIVE: {
    rq::CategoryAlternative &alternative =
        llvm::cast<rq::CategoryAlternative>(*this);
    info.setContainingModule(alternative.getContainingModule());
    info.setContainingTable(alternative.getContainingTable());
    info.setName(alternative.getCode().getName());
    info.setExpression(alternative.getExpression());
  } break;
  case O::SY_GLOBAL: {
    rq::Global &global = llvm::cast<rq::Global>(*this);
    info.setContainingModule(global.getContainingModule());
    info.setContainingTable(global.getContainingTable());
    info.setName(global.getName());
    info.setExpression(global.getExpression());
  } break;
  case O::SY_GLOBAL_STATIC: {
    rq::GlobalStatic &global_static = llvm::cast<rq::GlobalStatic>(*this);
    info.setContainingModule(global_static.getContainingModule());
    info.setContainingTable(global_static.getContainingTable());
    info.setName(global_static.getName());
    info.setExpression(global_static.getExpression());
  } break;
  case O::SY_NAMESPACE: {
    rq::Namespace &namespace_ = llvm::cast<rq::Namespace>(*this);
    info.setContainingTable(namespace_.getContainingTable());
    info.setName(namespace_.getName());
  } break;
  case O::SY_CLASS: {
    rq::Class &class_ = llvm::cast<rq::Class>(*this);
    info.setContainingModule(class_.getContainingModule());
    info.setContainingTable(class_.getContainingTable());
    info.setName(class_.getName());
    info.setExpression(class_.getExpression());
  } break;
  case O::SY_ENUMERATION: {
    rq::Enumeration &enumeration = llvm::cast<rq::Enumeration>(*this);
    info.setContainingModule(enumeration.getContainingModule());
    info.setContainingTable(enumeration.getContainingTable());
    info.setName(enumeration.getName());
    info.setExpression(enumeration.getExpression());
  } break;
  case O::SY_CATEGORY: {
    rq::Category &category = llvm::cast<rq::Category>(*this);
    info.setContainingModule(category.getContainingModule());
    info.setContainingTable(category.getContainingTable());
    info.setName(category.getName());
    info.setExpression(category.getExpression());
  } break;
  case O::SY_ENTRY:
    [[fallthrough]];
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_EXTENSION_METHOD: {
    rq::Procedure &procedure = llvm::cast<rq::Procedure>(*this);
    info.setContainingModule(procedure.getContainingModule());
    info.setContainingTable(procedure.getContainingTable());
    info.setName(procedure.getName());
    info.setExpression(procedure.getExpression());
  } break;
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_STATIC:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD: {
    rq::Template &template_ = llvm::cast<rq::Template>(*this);
    info.setContainingModule(template_.getContainingModule());
    info.setContainingTable(template_.getContainingTable());
    info.setName(template_.getName());
    info.setExpression(template_.getExpression());
  } break;
  case O::SY_SPECIALIZED_CLASS:
    [[fallthrough]];
  case O::SY_SPECIALIZED_ENUMERATION:
    [[fallthrough]];
  case O::SY_SPECIALIZED_CATEGORY:
    [[fallthrough]];
  case O::SY_SPECIALIZED_GLOBAL:
    [[fallthrough]];
  case O::SY_SPECIALIZED_GLOBAL_STATIC:
    [[fallthrough]];
  case O::SY_SPECIALIZED_FUNCTION:
    [[fallthrough]];
  case O::SY_SPECIALIZED_METHOD:
    [[fallthrough]];
  case O::SY_SPECIALIZED_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_SPECIALIZED_EXTENSION_METHOD: {
    rq::Specialized &specialized = llvm::cast<rq::Specialized>(*this);
    info.setContainingModule(specialized.getContainingModule());
    info.setContainingTable(specialized.getContainingTable());
    info.setName(specialized.getName());
    info.setExpression(specialized.getExpression());
  } break;
  default:
    RQ_UNREACHABLE();
  }
  return rq::DeclarationInfo(info);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getHasTemplateAlternative() const {
  return rq::getHasTemplateAlternative(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsType() const {
  return rq::getIsType(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSubtype() const {
  return rq::getIsSubtype(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsParameter() const {
  return rq::getIsParameter(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsParameterListSubtype() const {
  return rq::getIsParameterListSubtype(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsConstraint() const {
  return rq::getIsConstraint(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsExpressionAttribute() const {
  return rq::getIsExpressionAttribute(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTypeAttribute() const {
  return rq::getIsConstraint(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPlatformChanging() const {
  if (rq::getIsScaledBuiltin(this->getOpcode())) {
    // TODO
  }
  return rq::getIsPlatformChanging(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNumeric() const {
  return rq::getIsNumeric(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsInteger() const {
  return rq::getIsInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFloat() const {
  return rq::getIsFloat(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBinary() const {
  return rq::getIsBinary(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunit() const {
  return rq::getIsCodeunit(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSigned() const {
  return rq::getIsSigned(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsigned() const {
  return rq::getIsUnsigned(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSignedInteger() const {
  return rq::getIsSignedInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsignedInteger() const {
  return rq::getIsUnsignedInteger(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNoAscending() const {
  return rq::getIsNoAscending(this->getOpcode());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNoAscendingIntoTop() const {
  return rq::getIsNoAscendingIntoTop(this->getOpcode());
}

[[nodiscard]] inline bool Symbol::classof(const Entity *entity) {
  return rq::getIsSymbol(rq::dereferencePtr(entity).getOpcode());
}

inline SimpleBuiltin::SimpleBuiltin(rq::Opcode opcode) : Symbol(opcode) {}

[[nodiscard]] inline bool SimpleBuiltin::classof(const Entity *entity) {
  return rq::getIsSimpleBuiltin(rq::dereferencePtr(entity).getOpcode());
}

inline Result::Result() : SimpleBuiltin(rq::Opcode::SY_RESULT) {}

[[nodiscard]] inline bool Result::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_RESULT;
}

inline Out::Out() : SimpleBuiltin(rq::Opcode::SY_OUT) {}

[[nodiscard]] inline bool Out::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_OUT;
}

inline IntegerLiteral::IntegerLiteral()
    : SimpleBuiltin(rq::Opcode::SY_INTEGER_LITERAL) {}

[[nodiscard]] inline bool IntegerLiteral::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_INTEGER_LITERAL;
}

inline FloatLiteral::FloatLiteral()
    : SimpleBuiltin(rq::Opcode::SY_FLOAT_LITERAL) {}

[[nodiscard]] inline bool FloatLiteral::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_FLOAT_LITERAL;
}

inline StringLiteral::StringLiteral()
    : SimpleBuiltin(rq::Opcode::SY_STRING_LITERAL) {}

[[nodiscard]] inline bool StringLiteral::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_STRING_LITERAL;
}

inline CodeunitLiteral::CodeunitLiteral()
    : SimpleBuiltin(rq::Opcode::SY_CODEUNIT_LITERAL) {}

[[nodiscard]] inline bool CodeunitLiteral::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_CODEUNIT_LITERAL;
}

inline Inference::Inference() : SimpleBuiltin(rq::Opcode::SY_INFERENCE) {}

[[nodiscard]] inline bool Inference::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_INFERENCE;
}

inline SymbolReflection::SymbolReflection()
    : SimpleBuiltin(rq::Opcode::SY_SYMBOL_REFLECTION) {}

[[nodiscard]] inline bool SymbolReflection::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SYMBOL_REFLECTION;
}

inline Type::Type() : SimpleBuiltin(rq::Opcode::SY_TYPE) {}

[[nodiscard]] inline bool Type::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_TYPE;
}

inline Void::Void() : SimpleBuiltin(rq::Opcode::SY_VOID) {}

[[nodiscard]] inline bool Void::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_VOID;
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

inline Quadruple::Quadruple() : SimpleBuiltin(rq::Opcode::SY_QUADRUPLE) {}

[[nodiscard]] inline bool Quadruple::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_QUADRUPLE;
}

inline Binary16::Binary16() : SimpleBuiltin(rq::Opcode::SY_BINARY16) {}

[[nodiscard]] inline bool Binary16::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY16;
}

inline Binary32::Binary32() : SimpleBuiltin(rq::Opcode::SY_BINARY32) {}

[[nodiscard]] inline bool Binary32::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY32;
}

inline Binary64::Binary64() : SimpleBuiltin(rq::Opcode::SY_BINARY64) {}

[[nodiscard]] inline bool Binary64::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY64;
}

inline Binary128::Binary128() : SimpleBuiltin(rq::Opcode::SY_BINARY128) {}

[[nodiscard]] inline bool Binary128::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY128;
}

inline Bfloat16::Bfloat16() : SimpleBuiltin(rq::Opcode::SY_BFLOAT16) {}

[[nodiscard]] inline bool Bfloat16::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BFLOAT16;
}

inline IntegerConstraint::IntegerConstraint()
    : SimpleBuiltin(rq::Opcode::SY_INTEGER_CONSTRAINT) {}

[[nodiscard]] inline bool IntegerConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_INTEGER_CONSTRAINT;
}

inline SignedIntegerConstraint::SignedIntegerConstraint()
    : SimpleBuiltin(rq::Opcode::SY_SIGNED_INTEGER_CONSTRAINT) {}

[[nodiscard]] inline bool
SignedIntegerConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SIGNED_INTEGER_CONSTRAINT;
}

inline UnsignedIntegerConstraint::UnsignedIntegerConstraint()
    : SimpleBuiltin(rq::Opcode::SY_UNSIGNED_INTEGER_CONSTRAINT) {}

[[nodiscard]] inline bool
UnsignedIntegerConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_UNSIGNED_INTEGER_CONSTRAINT;
}

inline CodeunitConstraint::CodeunitConstraint()
    : SimpleBuiltin(rq::Opcode::SY_CODEUNIT_CONSTRAINT) {}

[[nodiscard]] inline bool CodeunitConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_CODEUNIT_CONSTRAINT;
}

inline StringConstraint::StringConstraint()
    : SimpleBuiltin(rq::Opcode::SY_STRING_CONSTRAINT) {}

[[nodiscard]] inline bool StringConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_STRING_CONSTRAINT;
}

inline InterpolatedStringConstraint::InterpolatedStringConstraint()
    : SimpleBuiltin(rq::Opcode::SY_INTERPOLATED_STRING_CONSTRAINT) {}

[[nodiscard]] inline bool
InterpolatedStringConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_INTERPOLATED_STRING_CONSTRAINT;
}

inline Char::Char() : SimpleBuiltin(rq::Opcode::SY_CHAR) {}

[[nodiscard]] inline bool Char::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CHAR;
}

inline Ascii::Ascii() : SimpleBuiltin(rq::Opcode::SY_ASCII) {}

[[nodiscard]] inline bool Ascii::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ASCII;
}

inline Utf8::Utf8() : SimpleBuiltin(rq::Opcode::SY_UTF8) {}

[[nodiscard]] inline bool Utf8::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_UTF8;
}

inline ExpressionAttributeConstraint::ExpressionAttributeConstraint()
    : SimpleBuiltin(rq::Opcode::SY_EXPRESSION_ATTRIBUTE_CONSTRAINT) {}

[[nodiscard]] inline bool
ExpressionAttributeConstraint::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_EXPRESSION_ATTRIBUTE_CONSTRAINT;
}

inline Labeling::Labeling() : SimpleBuiltin(rq::Opcode::SY_LABELING) {}

[[nodiscard]] inline bool Labeling::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LABELING;
}

inline Visibility::Visibility() : SimpleBuiltin(rq::Opcode::SY_VISIBILITY) {}

[[nodiscard]] inline bool Visibility::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_VISIBILITY;
}

inline Scoping::Scoping() : SimpleBuiltin(rq::Opcode::SY_SCOPING) {}

[[nodiscard]] inline bool Scoping::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SCOPING;
}

inline Availability::Availability()
    : SimpleBuiltin(rq::Opcode::SY_AVAILABILITY) {}

[[nodiscard]] inline bool Availability::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_AVAILABILITY;
}

inline Accessibility::Accessibility()
    : SimpleBuiltin(rq::Opcode::SY_ACCESSIBILITY) {}

[[nodiscard]] inline bool Accessibility::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ACCESSIBILITY;
}

inline PropertyMutability::PropertyMutability()
    : SimpleBuiltin(rq::Opcode::SY_PROPERTY_MUTABILITY) {}

[[nodiscard]] inline bool PropertyMutability::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_PROPERTY_MUTABILITY;
}

inline Exporting::Exporting() : SimpleBuiltin(rq::Opcode::SY_EXPORTING) {}

[[nodiscard]] inline bool Exporting::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_EXPORTING;
}

inline Runtime::Runtime() : SimpleBuiltin(rq::Opcode::SY_RUNTIME) {}

[[nodiscard]] inline bool Runtime::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_RUNTIME;
}

inline Capturing::Capturing() : SimpleBuiltin(rq::Opcode::SY_CAPTURING) {}

[[nodiscard]] inline bool Capturing::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CAPTURING;
}

inline EvaluationTime::EvaluationTime()
    : SimpleBuiltin(rq::Opcode::SY_EVALUATION_TIME) {}

[[nodiscard]] inline bool EvaluationTime::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_EVALUATION_TIME;
}

inline Parentability::Parentability()
    : SimpleBuiltin(rq::Opcode::SY_PARENTABILITY) {}

[[nodiscard]] inline bool Parentability::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_PARENTABILITY;
}

inline PropertyAssociation::PropertyAssociation()
    : SimpleBuiltin(rq::Opcode::SY_PROPERTY_ASSOCIATION) {}

[[nodiscard]] inline bool PropertyAssociation::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_PROPERTY_ASSOCIATION;
}

inline Tangibility::Tangibility() : SimpleBuiltin(rq::Opcode::SY_TANGIBILITY) {}

[[nodiscard]] inline bool Tangibility::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_TANGIBILITY;
}

inline Overriding::Overriding() : SimpleBuiltin(rq::Opcode::SY_OVERRIDING) {}

[[nodiscard]] inline bool Overriding::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_OVERRIDING;
}

inline Inlining::Inlining() : SimpleBuiltin(rq::Opcode::SY_INLINING) {}

[[nodiscard]] inline bool Inlining::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_INLINING;
}

inline Mangling::Mangling() : SimpleBuiltin(rq::Opcode::SY_MANGLING) {}

[[nodiscard]] inline bool Mangling::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_MANGLING;
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
getHasPlatformScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::PLATFORM_SCALAR);
}

void RQ_ALWAYS_INLINE profileScaledBuiltin(llvm::FoldingSetNodeID &id,
                                           rq::Opcode opcode,
                                           std::uint16_t scalar,
                                           std::uint16_t uid,
                                           rq::ScaledBuiltinFlags flags) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddInteger(static_cast<unsigned>(scalar));
  id.AddInteger(static_cast<unsigned>(uid));
  id.AddInteger(static_cast<unsigned>(flags));
}

inline ScaledBuiltin::ScaledBuiltin(rq::Opcode opcode, std::uint16_t scalar,
                                    std::uint16_t uid,
                                    rq::ScaledBuiltinFlags flags)
    : Symbol(opcode), _scalar(scalar), _uid(uid), _flags(flags) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t ScaledBuiltin::getScalar() const {
  return this->_scalar;
}

[[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t ScaledBuiltin::getUid() const {
  return this->_uid;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinFlags
ScaledBuiltin::getFlags() const {
  return this->_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getHasBytesScalar() const {
  return rq::getHasBytesScalar(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getHasIndexScalar() const {
  return rq::getHasIndexScalar(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getHasAddressScalar() const {
  return rq::getHasAddressScalar(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getIsExact() const {
  return rq::getIsExact(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getIsFastest() const {
  return rq::getIsFastest(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getIsLeast() const {
  return rq::getIsLeast(this->_flags);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ScaledBuiltin::getIsSynonym() const {
  return this->_uid != 0;
}

[[nodiscard]] inline bool ScaledBuiltin::classof(const Entity *entity) {
  return rq::getIsScaledBuiltin(rq::dereferencePtr(entity).getOpcode());
}

inline void ScaledBuiltin::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileScaledBuiltin(id, this->_opcode, this->_scalar, this->_uid,
                           this->_flags);
}

inline ScaledSignedInteger::ScaledSignedInteger(std::uint16_t scalar,
                                                std::uint16_t uid,
                                                rq::ScaledBuiltinFlags flags)
    : ScaledBuiltin(rq::Opcode::SY_SCALED_SIGNED_INTEGER, scalar, uid, flags) {}

[[nodiscard]] inline bool ScaledSignedInteger::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SCALED_SIGNED_INTEGER;
}

inline ScaledUnsignedInteger::ScaledUnsignedInteger(
    std::uint16_t scalar, std::uint16_t uid, rq::ScaledBuiltinFlags flags)
    : ScaledBuiltin(rq::Opcode::SY_SCALED_UNSIGNED_INTEGER, scalar, uid,
                    flags) {}

[[nodiscard]] inline bool ScaledUnsignedInteger::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SCALED_UNSIGNED_INTEGER;
}

void RQ_ALWAYS_INLINE profileUnarySubtype(llvm::FoldingSetNodeID &id,
                                          rq::Opcode opcode,
                                          const rq::TypeConstant &descendent) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddPointer(&descendent);
}

inline UnarySubtype::UnarySubtype(rq::Opcode opcode,
                                  rq::TypeConstant &descendent)
    : Symbol(opcode), _descendent_ptr(&descendent) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
UnarySubtype::getDescendent() const {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &UnarySubtype::getDescendent() {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] inline bool UnarySubtype::classof(const Entity *entity) {
  return rq::getIsUnarySubtype(rq::dereferencePtr(entity).getOpcode());
}

inline void UnarySubtype::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileUnarySubtype(id, this->getOpcode(), this->getDescendent());
}

inline Reference::Reference(rq::TypeConstant &descendent)
    : UnarySubtype(rq::Opcode::SY_REFERENCE, descendent) {}

[[nodiscard]] inline bool Reference::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_REFERENCE;
}

inline Pointer::Pointer(rq::TypeConstant &descendent)
    : UnarySubtype(rq::Opcode::SY_POINTER, descendent) {}

[[nodiscard]] inline bool Pointer::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_POINTER;
}

inline FatPointer::FatPointer(rq::TypeConstant &descendent)
    : UnarySubtype(rq::Opcode::SY_FAT_POINTER, descendent) {}

[[nodiscard]] inline bool FatPointer::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_FAT_POINTER;
}

inline InferencedCountArray::InferencedCountArray(rq::TypeConstant &descendent)
    : UnarySubtype(rq::Opcode::SY_INFERENCED_COUNT_ARRAY, descendent) {}

[[nodiscard]] inline bool InferencedCountArray::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_INFERENCED_COUNT_ARRAY;
}

void RQ_ALWAYS_INLINE profileCountedSubtype(llvm::FoldingSetNodeID &id,
                                            rq::Opcode opcode,
                                            const rq::TypeConstant &descendent,
                                            unsigned count) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddPointer(&descendent);
  id.AddInteger(count);
}

inline CountedSubtype::CountedSubtype(rq::Opcode opcode,
                                      rq::TypeConstant &descendent,
                                      unsigned count)
    : Symbol(opcode), _descendent_ptr(&descendent), _count(count) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
CountedSubtype::getDescendent() const {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &
CountedSubtype::getDescendent() {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned CountedSubtype::getCount() const {
  return this->_count;
}

[[nodiscard]] inline bool CountedSubtype::classof(const Entity *entity) {
  return rq::getIsCountedSubtype(rq::dereferencePtr(entity).getOpcode());
}

void CountedSubtype::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileCountedSubtype(id, this->getOpcode(), this->getDescendent(),
                            this->getCount());
}

inline Array::Array(rq::TypeConstant &descendent, unsigned count)
    : CountedSubtype(rq::Opcode::SY_ARRAY, descendent, count) {}

[[nodiscard]] inline bool Array::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ARRAY;
}

inline ParameterListSubtype::ParameterListSubtype(
    rq::BumpPtrAllocator &allocator, unsigned map_bucket_count,
    rq::Opcode opcode, rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &hosting_table)
    : Symbol(opcode), InitialExpressionFlags(attributes),
      InitialModuleMember(module), TableHosted(hosting_table),
      _named_parameter_map(allocator.allocateAcquiredZeroedArray<rq::Parameter>(
          map_bucket_count)) {}

[[nodiscard]] inline bool ParameterListSubtype::classof(const Entity *entity) {
  return rq::getIsParameterListSubtype(rq::dereferencePtr(entity).getOpcode());
}

inline Layout::Layout(rq::BumpPtrAllocator &allocator,
                      unsigned parameter_bucket_count,
                      rq::ExpressionFlags attributes, rq::Module &module,
                      rq::Table &hosting_table)
    : ParameterListSubtype(allocator, parameter_bucket_count,
                           rq::Opcode::SY_LAYOUT, attributes, module,
                           hosting_table) {}
[[nodiscard]] inline bool Layout::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LAYOUT;
}

inline ClassLayout::ClassLayout(rq::BumpPtrAllocator &allocator,
                                unsigned parameter_bucket_count,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &hosting_table)
    : ParameterListSubtype(allocator, parameter_bucket_count,
                           rq::Opcode::SY_CLASS_LAYOUT, attributes, module,
                           hosting_table) {}

[[nodiscard]] inline bool ClassLayout::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CLASS_LAYOUT;
}

inline TemplateLayout::TemplateLayout(rq::BumpPtrAllocator &allocator,
                                      unsigned parameter_bucket_count,
                                      rq::Module &module,
                                      rq::Table &hosting_table)
    : ParameterListSubtype(allocator, parameter_bucket_count,
                           rq::Opcode::SY_TEMPLATE_LAYOUT, {}, module,
                           hosting_table) {}

[[nodiscard]] inline bool TemplateLayout::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_LAYOUT;
}

inline Signature::Signature(rq::BumpPtrAllocator &allocator,
                            unsigned parameter_bucket_count,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table)
    : ParameterListSubtype(allocator, parameter_bucket_count,
                           rq::Opcode::SY_SIGNATURE, attributes, module,
                           hosting_table) {}

RQ_ALWAYS_INLINE void Signature::setReturnType(rq::TypeConstant &type) {
  this->_return_type_ptr = &type;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Signature::getHasReturnType() const {
  return this->_return_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
Signature::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &Signature::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

RQ_ALWAYS_INLINE void Signature::setExtendedType(rq::TypeConstant &type) {
  this->_extended_type_ptr = &type;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Signature::getHasExtendedType() const {
  return this->_extended_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
Signature::getExtendedType() const {
  return rq::dereferencePtr(this->_extended_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &Signature::getExtendedType() {
  return rq::dereferencePtr(this->_extended_type_ptr);
}

[[nodiscard]] inline bool Signature::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SIGNATURE;
}

inline Parameter::Parameter(rq::Opcode opcode, llvm::StringRef name,
                            rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table)
    : Symbol(opcode), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableHosted(hosting_table), InitialMaybeNamed(name),
      _parameter_list_subtype_ptr(&list) {}

inline Parameter::Parameter(rq::Opcode opcode, rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table)
    : Symbol(opcode), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableHosted(hosting_table), InitialMaybeNamed(),
      _parameter_list_subtype_ptr(&list) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Parameter::getHasType() const {
  return this->_type_ptr != nullptr;
}

RQ_ALWAYS_INLINE void Parameter::setType(rq::TypeConstant &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
Parameter::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &Parameter::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ParameterListSubtype &
Parameter::getParameterListSubtype() const {
  return rq::dereferencePtr(this->_parameter_list_subtype_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterListSubtype &
Parameter::getParameterListSubtype() {
  return rq::dereferencePtr(this->_parameter_list_subtype_ptr);
}

[[nodiscard]] inline bool Parameter::classof(const Entity *entity) {
  return rq::getIsParameter(rq::dereferencePtr(entity).getOpcode());
}

inline ClassParameter::ClassParameter(llvm::StringRef name,
                                      rq::ClassLayout &layout,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_CLASS_PARAMETER, name, layout, expression,
                attributes, module, hosting_table) {}
inline ClassParameter::ClassParameter(rq::ClassLayout &layout,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_CLASS_PARAMETER, layout, expression, attributes,
                module, hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassLayout &
ClassParameter::getClassLayout() const {
  return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassLayout &
ClassParameter::getClassLayout() {
  return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
}

[[nodiscard]] inline bool ClassParameter::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_CLASS_PARAMETER;
}

inline LayoutParameter::LayoutParameter(llvm::StringRef name,
                                        rq::Layout &layout,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_LAYOUT_PARAMETER, name, layout, expression,
                attributes, module, hosting_table) {}
inline LayoutParameter::LayoutParameter(rq::Layout &layout,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_LAYOUT_PARAMETER, layout, expression, attributes,
                module, hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &
LayoutParameter::getLayout() const {
  return llvm::cast<rq::Layout>(this->getParameterListSubtype());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &LayoutParameter::getLayout() {
  return llvm::cast<rq::Layout>(this->getParameterListSubtype());
}

[[nodiscard]] inline bool LayoutParameter::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_LAYOUT_PARAMETER;
}

inline TemplateParameter::TemplateParameter(llvm::StringRef name,
                                            rq::TemplateLayout &template_layout,
                                            rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_TEMPLATE_PARAMETER, name, template_layout,
                expression, attributes, module, hosting_table) {}
inline TemplateParameter::TemplateParameter(rq::TemplateLayout &template_layout,
                                            rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_TEMPLATE_PARAMETER, template_layout, expression,
                attributes, module, hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &
TemplateParameter::getTemplate() const {
  return llvm::cast<rq::Template>(this->getParameterListSubtype());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Template &TemplateParameter::getTemplate() {
  return llvm::cast<rq::Template>(this->getParameterListSubtype());
}

[[nodiscard]] inline bool TemplateParameter::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_PARAMETER;
}

inline SignatureParameter::SignatureParameter(llvm::StringRef name,
                                              rq::Signature &signature,
                                              rq::Expression &expression,
                                              rq::ExpressionFlags attributes,
                                              rq::Module &module,
                                              rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_SIGNATURE_PARAMETER, name, signature, expression,
                attributes, module, hosting_table) {}

inline SignatureParameter::SignatureParameter(rq::Signature &signature,
                                              rq::Expression &expression,
                                              rq::ExpressionFlags attributes,
                                              rq::Module &module,
                                              rq::Table &hosting_table)
    : Parameter(rq::Opcode::SY_SIGNATURE_PARAMETER, signature, expression,
                attributes, module, hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &
SignatureParameter::getSignature() const {
  return llvm::cast<rq::Signature>(this->getParameterListSubtype());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Signature &
SignatureParameter::getSignature() {
  return llvm::cast<rq::Signature>(this->getParameterListSubtype());
}

[[nodiscard]] inline bool SignatureParameter::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_SIGNATURE_PARAMETER;
}

void RQ_ALWAYS_INLINE profileArithmeticSequence(
    llvm::FoldingSetNodeID &id, const rq::TypeConstant &descendent,
    rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  // no need to fold opcode
  id.AddPointer(&descendent);
  id.AddInteger(static_cast<unsigned>(condition));
  id.AddInteger(static_cast<unsigned>(step));
}

inline ArithmeticSequence::ArithmeticSequence(
    rq::Opcode opcode, rq::TypeConstant &descendent,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step)
    : Symbol(opcode), _descendent_ptr(&descendent), _condition(condition),
      _step(step) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
ArithmeticSequence::getDescendent() const {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &
ArithmeticSequence::getDescendent() {
  return rq::dereferencePtr(this->_descendent_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
ArithmeticSequence::getCondition() const {
  return this->_condition;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep
ArithmeticSequence::getStep() const {
  return this->_step;
}

[[nodiscard]] inline bool ArithmeticSequence::classof(const Entity *entity) {
  return rq::getIsArithmeticSequence(rq::dereferencePtr(entity).getOpcode());
}

void ArithmeticSequence::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileArithmeticSequence(id, this->getDescendent(), this->getCondition(),
                                this->getStep());
}

inline ArithmeticInterval::ArithmeticInterval(
    rq::TypeConstant &descendent, rq::ArithmeticSequenceCondition condition)
    : ArithmeticSequence(rq::Opcode::SY_ARITHMETIC_INTERVAL, descendent,
                         condition, rq::ArithmeticSequenceStep::NONE) {}

[[nodiscard]] inline bool ArithmeticInterval::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_ARITHMETIC_INTERVAL;
}

inline FiniteArithmeticProgression::FiniteArithmeticProgression(
    rq::TypeConstant &descendent, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step)
    : ArithmeticSequence(rq::Opcode::SY_FINITE_ARITHMETIC_PROGRESSION,
                         descendent, condition, step) {}

[[nodiscard]] inline bool
FiniteArithmeticProgression::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_FINITE_ARITHMETIC_PROGRESSION;
}

inline InfiniteArithmeticProgression::InfiniteArithmeticProgression(
    rq::TypeConstant &descendent, rq::ArithmeticSequenceStep step)
    : ArithmeticSequence(rq::Opcode::SY_INFINITE_ARITHMETIC_PROGRESSION,
                         descendent, rq::ArithmeticSequenceCondition::NONE,
                         step) {}

[[nodiscard]] inline bool
InfiniteArithmeticProgression::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_INFINITE_ARITHMETIC_PROGRESSION;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  using M = rq::ModuleKind;
  switch (kind) {
  case M::NONE:
    return "none";
  case M::SOURCE:
    return "source";
  case M::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

inline Module::Module(rq::ModuleKind kind, llvm::StringRef path,
                      llvm::MemoryBufferRef &&buffer, rq::Top &top)
    : Symbol(rq::Opcode::SY_MODULE), _module_kind(kind), _path(path),
      _buffer(buffer), _top_ptr(&top) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind Module::getModuleKind() const {
  return this->_module_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getSourceText() const {
  return this->_buffer.getBuffer();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Module::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Module::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Module::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

RQ_ALWAYS_INLINE void Module::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE void Module::setExpression(rq::Expression *expression) {
  rq::assignSingleValue(this->_expression_ptr, expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
Module::replaceExpression(rq::Expression &expression) {
  return rq::replaceValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Module::replaceExpressionPtr(rq::Expression &expression) {
  return rq::replaceValuPtr(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Top &Module::getTop() const {
  return rq::dereferencePtr(this->_top_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Top &Module::getTop() {
  return rq::dereferencePtr(this->_top_ptr);
}

[[nodiscard]] inline bool Module::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_MODULE;
}

inline Import::Import(const rq::Expression &expression,
                      rq::ExpressionFlags attributes, rq::Module &module)
    : Symbol(rq::Opcode::SY_IMPORT), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module) {}

[[nodiscard]] inline bool Import::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_IMPORT;
}

inline Code::Code(llvm::StringRef name, const rq::Expression &expression,
                  rq::ExpressionFlags attributes, rq::Module &module,
                  rq::Table &containing_table, rq::Table &hosting_table)
    : Symbol(rq::Opcode::SY_CODE), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] inline bool Code::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CODE;
}

inline CategoryDiscriminant::CategoryDiscriminant(rq::Category &category)
    : Symbol(rq::Opcode::SY_CATEGORY_DISCRIMINANT), _category_ptr(&category) {}

[[nodiscard]] inline bool CategoryDiscriminant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_CATEGORY_DISCRIMINANT;
}

inline Label::Label(llvm::StringRef name, rq::Expression &expression,
                    const rq::Expression &ascription, rq::Entity &subject,
                    rq::Module &module, rq::Table &containing_table)
    : Symbol(rq::Opcode::SY_LABEL), InitialExpression(expression),
      InitialModuleMember(module), TableMember(containing_table),
      InitialNamed(name), _ascription_ptr(&ascription), _subject_ptr(&subject) {
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Label::getAscription() const {
  return rq::dereferencePtr(this->_ascription_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &Label::getSubject() const {
  return rq::dereferencePtr(this->_subject_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &Label::getSubject() {
  return rq::dereferencePtr(this->_subject_ptr);
}

[[nodiscard]] inline bool Label::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LABEL;
}

inline Synonym::Synonym(rq::Symbol &original)
    : Symbol(rq::Opcode::SY_SYNONYM), _original_ptr(&original) {
  RQ_ASSERT(
      !llvm::isa<rq::ScaledBuiltin>(original),
      "must use internal uid to differentiate synonyms of scaled builtins");
  RQ_ASSERT(!llvm::isa<rq::Synonym>(original),
            "must not make synonym of synonym");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &Synonym::getOriginal() const {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &Synonym::getOriginal() {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] inline bool Synonym::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SYNONYM;
}

inline TableIterator::TableIterator(rq::Table *table_ptr)
    : _table_ptr(table_ptr) {}

RQ_ALWAYS_INLINE rq::TableIterator &TableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._containing_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::TableIterator TableIterator::operator++(int) {
  rq::TableIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TableIterator::operator==(const Self &it) const {
  return this->_table_ptr == it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TableIterator::operator!=(const Self &it) const {
  return this->_table_ptr != it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table &TableIterator::operator*() {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
TableIterator::operator*() const {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table *TableIterator::operator->() {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *
TableIterator::operator->() const {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TableIterator::getIsDone() const {
  return this->_table_ptr == nullptr;
}

inline ConstTableIterator::ConstTableIterator(const rq::Table *table_ptr)
    : _table_ptr(table_ptr) {}

RQ_ALWAYS_INLINE rq::ConstTableIterator &ConstTableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._containing_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstTableIterator ConstTableIterator::operator++(int) {
  rq::ConstTableIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstTableIterator::operator==(const Self &it) const {
  return this->_table_ptr == it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstTableIterator::operator!=(const Self &it) const {
  return this->_table_ptr != it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
ConstTableIterator::operator*() const {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *
ConstTableIterator::operator->() const {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ConstTableIterator::getIsDone() const {
  return this->_table_ptr == nullptr;
}

inline MemberTableIterator::MemberTableIterator(rq::Table *table_ptr)
    : _table_ptr(table_ptr) {}
RQ_ALWAYS_INLINE rq::MemberTableIterator &MemberTableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._next_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::MemberTableIterator MemberTableIterator::operator++(int) {
  rq::MemberTableIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
MemberTableIterator::operator==(const Self &it) const {
  return this->_table_ptr == it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
MemberTableIterator::operator!=(const Self &it) const {
  return this->_table_ptr != it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table &MemberTableIterator::operator*() {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
MemberTableIterator::operator*() const {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Table *MemberTableIterator::operator->() {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *
MemberTableIterator::operator->() const {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool MemberTableIterator::getIsDone() const {
  return this->_table_ptr == nullptr;
}

inline ConstMemberTableIterator::ConstMemberTableIterator(
    const rq::Table *table_ptr)
    : _table_ptr(table_ptr) {}

RQ_ALWAYS_INLINE rq::ConstMemberTableIterator &
ConstMemberTableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._next_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstMemberTableIterator
ConstMemberTableIterator::operator++(int) {
  rq::ConstMemberTableIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstMemberTableIterator::operator==(const Self &it) const {
  return this->_table_ptr == it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstMemberTableIterator::operator!=(const Self &it) const {
  return this->_table_ptr != it._table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &
ConstMemberTableIterator::operator*() const {
  return rq::dereferencePtr(this->_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *
ConstMemberTableIterator::operator->() const {
  return this->_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstMemberTableIterator::getIsDone() const {
  return this->_table_ptr == nullptr;
}

inline Table::Table(rq::Opcode opcode) : Symbol(opcode) {}

inline Table::Table(rq::Opcode opcode, rq::Table &containing_table)
    : Symbol(opcode), TableMember(containing_table) {}

inline void Table::release() {
  this->_named_symbols_map.clear();
  for (rq::Table &member : this->getMemberTableSubrange()) {
    member.release();
  }
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListRef<rq::Symbol>
Table::getUnamedSymbolsListRef() const {
  return this->_unamed_symbols_list;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
Table::getUnamedSymbolsListRef() {
  return this->_unamed_symbols_list;
}

inline void Table::_addMember(rq::Symbol &symbol) {
  if (llvm::isa<rq::Table>(symbol)) {
    rq::Table &table = llvm::cast<rq::Table>(symbol);
    table._next_table_ptr = this->_first_member_table_ptr;
    this->_first_member_table_ptr = &table;
  }
}

inline void Table::addNamedSymbol(rq::BumpPtrAllocator &allocator,
                                  llvm::StringRef name, rq::Symbol &symbol) {
  rq::BumpPtrList<rq::Symbol> &list = this->_named_symbols_map[name];
  list.insertFront(allocator, symbol);
  this->_addMember(symbol);
}

inline void Table::addUnamedSymbol(rq::BumpPtrAllocator &allocator,
                                   rq::Symbol &symbol) {
  this->_unamed_symbols_list.insertFront(allocator, symbol);
  this->_addMember(symbol);
}

[[nodiscard]] inline rq::ConstBumpPtrListRef<rq::Symbol>
Table::getNamedListRef(llvm::StringRef name) const {
  auto it = this->_named_symbols_map.find(name);
  if (it == this->_named_symbols_map.end()) {
    return rq::ConstBumpPtrListRef<rq::Symbol>();
  }
  return it->getSecond();
}

[[nodiscard]] inline rq::BumpPtrListRef<rq::Symbol>
Table::getNamedListRef(llvm::StringRef name) {
  auto it = this->_named_symbols_map.find(name);
  if (it == this->_named_symbols_map.end()) {
    return rq::BumpPtrListRef<rq::Symbol>();
  }
  return it->getSecond();
}

[[nodiscard]] inline auto Table::getNamedListsSubrange() const {
  auto begin_it = this->_named_symbols_map.begin();
  return std::ranges::subrange<decltype(begin_it), decltype(begin_it),
                               std::ranges::subrange_kind::unsized>(
      begin_it, this->_named_symbols_map.end());
}

[[nodiscard]] inline std::ranges::subrange<
    llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
    llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
    std::ranges::subrange_kind::unsized>
Table::getNamedListsSubrange() {
  return std::ranges::subrange<
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      std::ranges::subrange_kind::unsized>(this->_named_symbols_map.begin(),
                                           this->_named_symbols_map.end());
}

[[nodiscard]] inline std::ranges::subrange<rq::ConstTableIterator,
                                           rq::ConstTableIterator,
                                           std::ranges::subrange_kind::unsized>
Table::getInclusiveFrameSubrange() const {
  return std::ranges::subrange<rq::ConstTableIterator, rq::ConstTableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstTableIterator(this), rq::ConstTableIterator());
}

[[nodiscard]] inline std::ranges::subrange<rq::TableIterator, rq::TableIterator,
                                           std::ranges::subrange_kind::unsized>
Table::getInclusiveFrameSubrange() {
  return std::ranges::subrange<rq::TableIterator, rq::TableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::TableIterator(this), rq::TableIterator());
}

[[nodiscard]] inline std::ranges::subrange<rq::MemberTableIterator,
                                           rq::MemberTableIterator,
                                           std::ranges::subrange_kind::unsized>
Table::getMemberTableSubrange() {
  return std::ranges::subrange<rq::MemberTableIterator, rq::MemberTableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::MemberTableIterator(this->_first_member_table_ptr),
      rq::MemberTableIterator());
}

[[nodiscard]] inline std::ranges::subrange<rq::ConstMemberTableIterator,
                                           rq::ConstMemberTableIterator,
                                           std::ranges::subrange_kind::unsized>
Table::getMemberTableSubrange() const {
  return std::ranges::subrange<rq::ConstMemberTableIterator,
                               rq::ConstMemberTableIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstMemberTableIterator(this->_first_member_table_ptr),
      rq::ConstMemberTableIterator());
}

[[nodiscard]] inline bool Table::classof(const Entity *entity) {
  return rq::getIsTable(rq::dereferencePtr(entity).getOpcode());
}

inline Top::Top() : Table(rq::Opcode::SY_TOP) {}

[[nodiscard]] inline bool Top::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_TOP;
}

inline Global::Global(llvm::StringRef name, const rq::Expression &expression,
                      rq::ExpressionFlags attributes, rq::Module &module,
                      rq::Table &containing_table)
    : Table(rq::Opcode::SY_GLOBAL, containing_table),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), InitialNamed(name), _type_ptr(nullptr),
      _type_expression_ptr(nullptr), _value_expression_ptr(nullptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Global::getIsImplemented() const {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void Global::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Global::getHasType() const {
  return this->_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &Global::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &Global::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

RQ_ALWAYS_INLINE void
Global::setTypeExpression(const rq::Expression &expression) {
  this->_type_expression_ptr = &expression;
}

RQ_ALWAYS_INLINE void
Global::setValueExpression(const rq::Expression &expression) {
  this->_value_expression_ptr = &expression;
}

[[nodiscard]] inline bool Global::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_GLOBAL;
}

inline GlobalStatic::GlobalStatic(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::Table &containing_table)
    : Table(rq::Opcode::SY_GLOBAL_STATIC, containing_table),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), InitialNamed(name), _type_ptr(nullptr),
      _type_expression_ptr(nullptr), _value_expression_ptr(nullptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool GlobalStatic::getIsImplemented() const {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void GlobalStatic::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool GlobalStatic::getHasType() const {
  return this->_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &GlobalStatic::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
GlobalStatic::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

RQ_ALWAYS_INLINE void
GlobalStatic::setTypeExpression(const rq::Expression &expression) {
  this->_type_expression_ptr = &expression;
}

RQ_ALWAYS_INLINE void
GlobalStatic::setValueExpression(const rq::Expression &expression) {
  this->_value_expression_ptr = &expression;
}

[[nodiscard]] inline bool GlobalStatic::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_GLOBAL_STATIC;
}

inline Scope::Scope(rq::Expression &expression, rq::Module &module,
                    rq::Table &containing_table)
    : Table(rq::Opcode::SY_SCOPE, containing_table),
      InitialExpression(expression), InitialModuleMember(module) {}

[[nodiscard]] inline bool Scope::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SCOPE;
}

inline Namespace::Namespace(llvm::StringRef name,

                            rq::Table &containing_table)
    : Table(rq::Opcode::SY_NAMESPACE, containing_table), InitialNamed(name) {}

[[nodiscard]] inline bool Namespace::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NAMESPACE;
}

inline Class::Class(llvm::StringRef name, const rq::Expression &expression,
                    rq::ExpressionFlags attributes, rq::Module &module,
                    rq::Table &containing_table, rq::Table &hosting_table)
    : Table(rq::Opcode::SY_CLASS, containing_table),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Class::getIsImplemented() const {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void Class::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

inline void
Class::setClassLayoutExpression(const rq::Expression &layout_expression) {
  rq::assignSingleValue(this->_class_layout_expression_ptr, &layout_expression);
}

[[nodiscard]] inline bool Class::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CLASS;
}

inline Enumeration::Enumeration(llvm::StringRef name,
                                const rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table)
    : Table(rq::Opcode::SY_ENUMERATION, containing_table),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Enumeration::getIsImplemented() const {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void Enumeration::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

RQ_ALWAYS_INLINE void Enumeration::setUnderlyingTypeExpression(
    const rq::Expression &underlying_type_expression) {
  rq::assignSingleValue(this->_underlying_type_expression_ptr,
                        &underlying_type_expression);
}

[[nodiscard]] inline bool Enumeration::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENUMERATION;
}

inline Category::Category(llvm::StringRef name,
                          const rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::Table &containing_table, rq::Table &hosting_table)
    : Table(rq::Opcode::SY_CATEGORY, containing_table),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Category::getIsImplemented() const {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void Category::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

RQ_ALWAYS_INLINE void
Category::setDiscriminantTypeExpression(const rq::Expression &type_expression) {
  rq::assignSingleValue(this->_discriminant_type_expression_ptr,
                        &type_expression);
}

[[nodiscard]] inline bool Category::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CATEGORY;
}

inline TemplateIterator::TemplateIterator(rq::Template *template_ptr)
    : _template_ptr{template_ptr} {}

RQ_ALWAYS_INLINE rq::TemplateIterator &TemplateIterator::operator++() {
  this->_template_ptr =
      rq::dereferencePtr(this->_template_ptr)._next_template_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::TemplateIterator TemplateIterator::operator++(int) {
  rq::TemplateIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TemplateIterator::operator==(const Self &it) const {
  return this->_template_ptr == it._template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
TemplateIterator::operator!=(const Self &it) const {
  return this->_template_ptr != it._template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Template &TemplateIterator::operator*() {
  return rq::dereferencePtr(this->_template_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &
TemplateIterator::operator*() const {
  return rq::dereferencePtr(this->_template_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Template *TemplateIterator::operator->() {
  return this->_template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *
TemplateIterator::operator->() const {
  return this->_template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool TemplateIterator::getIsDone() const {
  return this->_template_ptr == nullptr;
}

inline ConstTemplateIterator::ConstTemplateIterator(
    const rq::Template *template_ptr)
    : _template_ptr(template_ptr) {}

RQ_ALWAYS_INLINE rq::ConstTemplateIterator &
ConstTemplateIterator::operator++() {
  this->_template_ptr =
      rq::dereferencePtr(this->_template_ptr)._next_template_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstTemplateIterator
ConstTemplateIterator::operator++(int) {
  rq::ConstTemplateIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstTemplateIterator::operator==(const Self &it) const {
  return this->_template_ptr == it._template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstTemplateIterator::operator!=(const Self &it) const {
  return this->_template_ptr != it._template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &
ConstTemplateIterator::operator*() const {
  return rq::dereferencePtr(this->_template_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *
ConstTemplateIterator::operator->() const {
  return this->_template_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ConstTemplateIterator::getIsDone() const {
  return this->_template_ptr == nullptr;
}

template <typename UnderlyingParam>
inline UnderlyingTemplateIterator<UnderlyingParam>::UnderlyingTemplateIterator(
    UnderlyingParam *underlying_ptr)
    : _underlying_ptr(underlying_ptr) {}

template <typename UnderlyingParam>
RQ_ALWAYS_INLINE rq::UnderlyingTemplateIterator<UnderlyingParam> &
UnderlyingTemplateIterator<UnderlyingParam>::operator++() {
  this->_underlying_ptr = std::bit_cast<UnderlyingParam *>(
      rq::dereferencePtr(this->_underlying_ptr)._next_template_ptr);
  return *this;
}

template <typename UnderlyingParam>
RQ_ALWAYS_INLINE UnderlyingTemplateIterator<UnderlyingParam>
UnderlyingTemplateIterator<UnderlyingParam>::operator++(int) {
  rq::UnderlyingTemplateIterator<UnderlyingParam> temp = *this;
  ++(*this);
  return temp;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
UnderlyingTemplateIterator<UnderlyingParam>::operator==(const Self &it) const {
  return this->_underlying_ptr == it._underlying_ptr;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
UnderlyingTemplateIterator<UnderlyingParam>::operator!=(const Self &it) const {
  return this->_underlying_ptr != it._underlying_ptr;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE UnderlyingParam &
UnderlyingTemplateIterator<UnderlyingParam>::operator*() {
  return llvm::cast<UnderlyingParam>(rq::dereferencePtr(this->_underlying_ptr));
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE const UnderlyingParam &
UnderlyingTemplateIterator<UnderlyingParam>::operator*() const {
  return llvm::cast<UnderlyingParam>(rq::dereferencePtr(this->_underlying_ptr));
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE UnderlyingParam *
UnderlyingTemplateIterator<UnderlyingParam>::operator->() {
  return std::bit_cast<UnderlyingParam *>(this->_underlying_ptr);
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE const UnderlyingParam *
UnderlyingTemplateIterator<UnderlyingParam>::operator->() const {
  return std::bit_cast<UnderlyingParam *>(this->_underlying_ptr);
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
UnderlyingTemplateIterator<UnderlyingParam>::getIsDone() const {
  return this->_underlying_ptr == nullptr;
}

template <typename UnderlyingParam>
inline ConstUnderlyingTemplateIterator<UnderlyingParam>::
    ConstUnderlyingTemplateIterator(const UnderlyingParam *underlying_ptr)
    : _underlying_ptr(underlying_ptr) {}

template <typename UnderlyingParam>
RQ_ALWAYS_INLINE ConstUnderlyingTemplateIterator<UnderlyingParam> &
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator++() {
  this->_underlying_ptr = rq::dereferencePtr(this->_underlying_ptr)._next_ptr;
  return *this;
}

template <typename UnderlyingParam>
RQ_ALWAYS_INLINE ConstUnderlyingTemplateIterator<UnderlyingParam>
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator++(int) {
  rq::ConstUnderlyingTemplateIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator==(
    const Self &it) const {
  return this->_underlying_ptr == it._underlying_ptr;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator!=(
    const Self &it) const {
  return this->_underlying_ptr != it._underlying_ptr;
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE const UnderlyingParam &
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator*() const {
  return llvm::cast<UnderlyingParam>(rq::dereferencePtr(this->_underlying_ptr));
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE const UnderlyingParam *
ConstUnderlyingTemplateIterator<UnderlyingParam>::operator->() const {
  return llvm::cast<UnderlyingParam>(this->_underlying_ptr);
}

template <typename UnderlyingParam>
[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstUnderlyingTemplateIterator<UnderlyingParam>::getIsDone() const {
  return this->_underlying_ptr == nullptr;
}

inline Polymorph::Polymorph(rq::Opcode opcode, rq::Table &containing_table)
    : Symbol(opcode), TableMember(containing_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Polymorph::getHasTemplate() const {
  return this->_first_template_ptr != nullptr;
}

[[nodiscard]] inline std::ranges::subrange<rq::TemplateIterator,
                                           rq::TemplateIterator,
                                           std::ranges::subrange_kind::unsized>
Polymorph::getTemplateSubrange() {
  return std::ranges::subrange<rq::TemplateIterator, rq::TemplateIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::TemplateIterator(this->_first_template_ptr), rq::TemplateIterator());
}

[[nodiscard]] inline std::ranges::subrange<rq::ConstTemplateIterator,
                                           rq::ConstTemplateIterator,
                                           std::ranges::subrange_kind::unsized>
Polymorph::getTemplateSubrange() const {
  return std::ranges::subrange<rq::ConstTemplateIterator,
                               rq::ConstTemplateIterator,
                               std::ranges::subrange_kind::unsized>(
      rq::ConstTemplateIterator(this->_first_template_ptr),
      rq::ConstTemplateIterator());
}

[[nodiscard]] inline bool Polymorph::classof(const Entity *entity) {
  return rq::getIsPolymorph(rq::dereferencePtr(entity).getOpcode());
}

inline RangerIterator::RangerIterator(rq::Ranger *ranger_ptr)
    : _ranger_ptr{ranger_ptr} {}

RQ_ALWAYS_INLINE rq::RangerIterator &RangerIterator::operator++() {
  this->_ranger_ptr = rq::dereferencePtr(this->_ranger_ptr)._next_ranger_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::RangerIterator RangerIterator::operator++(int) {
  rq::RangerIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
RangerIterator::operator==(const Self &it) const {
  return this->_ranger_ptr == it._ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
RangerIterator::operator!=(const Self &it) const {
  return this->_ranger_ptr != it._ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Ranger &RangerIterator::operator*() {
  return rq::dereferencePtr(this->_ranger_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger &
RangerIterator::operator*() const {
  return rq::dereferencePtr(this->_ranger_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Ranger *RangerIterator::operator->() {
  return this->_ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger *
RangerIterator::operator->() const {
  return this->_ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool RangerIterator::getIsDone() const {
  return this->_ranger_ptr == nullptr;
}

inline ConstRangerIterator::ConstRangerIterator(const rq::Ranger *ranger_ptr)
    : _ranger_ptr(ranger_ptr) {}

RQ_ALWAYS_INLINE rq::ConstRangerIterator &ConstRangerIterator::operator++() {
  this->_ranger_ptr = rq::dereferencePtr(this->_ranger_ptr)._next_ranger_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstRangerIterator ConstRangerIterator::operator++(int) {
  rq::ConstRangerIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstRangerIterator::operator==(const Self &it) const {
  return this->_ranger_ptr == it._ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstRangerIterator::operator!=(const Self &it) const {
  return this->_ranger_ptr != it._ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger &
ConstRangerIterator::operator*() const {
  return rq::dereferencePtr(this->_ranger_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger *
ConstRangerIterator::operator->() const {
  return this->_ranger_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool ConstRangerIterator::getIsDone() const {
  return this->_ranger_ptr == nullptr;
}

inline Local::Local(llvm::StringRef name, const rq::Expression &expression,
                    rq::ExpressionFlags attributes, rq::Module &module,
                    rq::Table &containing_table)
    : Symbol(rq::Opcode::SY_LOCAL), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Local::getIsIndeterminate() const {
  return this->_is_indeterminate;
}

RQ_ALWAYS_INLINE void Local::setNotIndeterminate() {
  RQ_ASSERT(this->getHasType(), "does not have type");
  // RQ_ASSERT(this->getType().getIsComplete(), "does not have complete
  // type");
  this->_is_indeterminate = false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Local::getHasType() const {
  return this->_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &Local::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

RQ_ALWAYS_INLINE void
Local::setTypeExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_type_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE void
Local::setValueExpression(const rq::Expression &expression_ptr) {
  rq::assignSingleValue(this->_value_expression_ptr, &expression_ptr);
}

[[nodiscard]] inline bool Local::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LOCAL;
}

inline Static::Static(llvm::StringRef name, const rq::Expression &expression,
                      rq::ExpressionFlags attributes, rq::Module &module,
                      rq::Table &containing_table)
    : Symbol(rq::Opcode::SY_STATIC), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Static::getIsIndeterminate() const {
  return this->_is_indeterminate;
}

RQ_ALWAYS_INLINE void Static::setNotIndeterminate() {
  RQ_ASSERT(this->getHasType(), "does not have type");
  // RQ_ASSERT(this->getType().getIsComplete(), "does not have complete
  // type");
  this->_is_indeterminate = false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Static::getHasType() const {
  return this->_type_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &Static::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

RQ_ALWAYS_INLINE void
Static::setTypeExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_type_expression_ptr, &expression);
}

RQ_ALWAYS_INLINE void
Static::setValueExpression(const rq::Expression &expression_ptr) {
  rq::assignSingleValue(this->_value_expression_ptr, &expression_ptr);
}

[[nodiscard]] inline bool Static::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_STATIC;
}

inline Enumerator::Enumerator(llvm::StringRef name, rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module, rq::Table &containing_table,
                              rq::Table &hosting_table)
    : Symbol(rq::Opcode::SY_ENUMERATOR), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Enumeration &
Enumerator::getEnumeration() const {
  return llvm::cast<rq::Enumeration>(this->getContainingTable());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Enumeration &Enumerator::getEnumeration() {
  return llvm::cast<rq::Enumeration>(this->getContainingTable());
}

[[nodiscard]] inline bool Enumerator::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENUMERATOR;
}

inline CategoryAlternative::CategoryAlternative(rq::Code &code,
                                                rq::Category &category,
                                                rq::Expression &expression,
                                                rq::ExpressionFlags attributes,
                                                rq::Module &module)
    : Symbol(rq::Opcode::SY_CATEGORY_ALTERNATIVE),
      InitialExpression(expression), InitialExpressionFlags(attributes),
      InitialModuleMember(module), TableMember(category), _code_ptr(&code) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Code &
CategoryAlternative::getCode() const {
  return rq::dereferencePtr(this->_code_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Code &CategoryAlternative::getCode() {
  return rq::dereferencePtr(this->_code_ptr);
}

[[nodiscard]] inline bool CategoryAlternative::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_CATEGORY_ALTERNATIVE;
}

inline Procedure::Procedure(rq::Opcode opcode, llvm::StringRef name,

                            const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &containing_table,
                            rq::Table &hosting_table)
    : Table(opcode, containing_table), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableHosted(hosting_table), InitialMaybeNamed(name) {}

inline Procedure::Procedure(rq::Opcode opcode, const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &containing_table,
                            rq::Table &hosting_table)
    : Table(opcode, containing_table), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableHosted(hosting_table) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool Procedure::getIsImplemented() {
  return this->_is_implemented;
}

RQ_ALWAYS_INLINE void Procedure::setIsImplemented() {
  RQ_ASSERT(!this->_is_implemented, "already implemented");
  this->_is_implemented = true;
}

RQ_ALWAYS_INLINE void
Procedure::setSignatureExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_signature_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Procedure::getHasSignature() const {
  return this->_signature_ptr != nullptr;
}

RQ_ALWAYS_INLINE void Procedure::setSignature(rq::TypeConstant &signature) {
  this->_signature_ptr = &signature;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
Procedure::getSignature() const {
  return rq::dereferencePtr(this->_signature_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &Procedure::getSignature() {
  return rq::dereferencePtr(this->_signature_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Procedure::getHasInstruction() const {
  return this->_instruction_ptr != nullptr;
}

RQ_ALWAYS_INLINE void
Procedure::setInstruction(rq::Instruction *instruction_ptr) {
  rq::assignSingleValue(this->_instruction_ptr, instruction_ptr);
}

RQ_ALWAYS_INLINE void Procedure::setInstruction(rq::Instruction &instruction) {
  rq::assignSingleValue(this->_instruction_ptr, &instruction);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
Procedure::getInstruction() const {
  return rq::dereferencePtr(this->_instruction_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &Procedure::getInstruction() {
  return rq::dereferencePtr(this->_instruction_ptr);
}

RQ_ALWAYS_INLINE bool Procedure::getHasSignatureExpression() const {
  return this->_signature_expression_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Procedure::getSignatureExpression() const {
  return rq::dereferencePtr(this->_signature_expression_ptr);
}

RQ_ALWAYS_INLINE bool Procedure::getHasBodyStartExpression() const {
  return this->_body_start_ptr != nullptr;
}

RQ_ALWAYS_INLINE void
Procedure::setBodyStartExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_body_start_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Procedure::getBodyStartExpression() const {
  return rq::dereferencePtr(this->_body_start_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Procedure::getHasMangledName() const {
  return !this->_mangled_name.empty();
}

RQ_ALWAYS_INLINE void Procedure::setMangledName(llvm::StringRef name) {
  RQ_ASSERT(!this->getHasMangledName(), "mangled name already set");
  this->_mangled_name = name;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
Procedure::getMangledName() const {
  return this->_mangled_name;
}

RQ_ALWAYS_INLINE void
Procedure::setLlvmFunctionPtr(llvm::Function *llvm_function_ptr) {
  rq::assignSingleValue(this->_llvm_function_ptr, llvm_function_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::Function *
Procedure::getLlvmFunctionPtr() const {
  return this->_llvm_function_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::Function *Procedure::getLlvmFunctionPtr() {
  return this->_llvm_function_ptr;
}

[[nodiscard]] inline bool Procedure::classof(const Entity *entity) {
  return rq::getIsProcedure(rq::dereferencePtr(entity).getOpcode());
}

inline Entry::Entry(const rq::Expression &expression,
                    rq::ExpressionFlags attributes, rq::Module &module,
                    rq::Table &containing_table, rq::Table &hosting_table)
    : Procedure(rq::Opcode::SY_ENTRY, expression, attributes, module,
                containing_table, hosting_table) {}

[[nodiscard]] inline bool Entry::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENTRY;
}

inline Function::Function(llvm::StringRef name,
                          const rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::Table &containing_table, rq::Table &hosting_table)
    : Procedure(rq::Opcode::SY_FUNCTION, name, expression, attributes, module,
                containing_table, hosting_table) {}

[[nodiscard]] inline bool Function::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_FUNCTION;
}

inline Method::Method(llvm::StringRef name, const rq::Expression &expression,
                      rq::ExpressionFlags attributes, rq::Module &module,
                      rq::Table &containing_table, rq::Table &hosting_table)
    : Procedure(rq::Opcode::SY_METHOD, name, expression, attributes, module,
                containing_table, hosting_table) {}

[[nodiscard]] inline bool Method::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_METHOD;
}

inline ExtensionFunction::ExtensionFunction(llvm::StringRef name,
                                            const rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::Table &containing_table,
                                            rq::Table &hosting_table)
    : Procedure(rq::Opcode::SY_EXTENSION_FUNCTION, name, expression, attributes,
                module, containing_table, hosting_table) {}

[[nodiscard]] inline bool ExtensionFunction::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_EXTENSION_FUNCTION;
}

inline ExtensionMethod::ExtensionMethod(llvm::StringRef name,
                                        const rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::Table &containing_table,
                                        rq::Table &hosting_table)
    : Procedure(rq::Opcode::SY_EXTENSION_METHOD, name, expression, attributes,
                module, containing_table, hosting_table) {}

[[nodiscard]] inline bool ExtensionMethod::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_EXTENSION_METHOD;
}

inline Template::Template(rq::Opcode opcode, llvm::StringRef name,
                          const rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::Table &containing_table, rq::Table &hosting_table,
                          rq::TemplateLayout &template_layout)
    : Symbol(opcode), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), TableHosted(hosting_table),
      InitialNamed(name), _template_layout_ptr(&template_layout) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateLayout &
Template::getTemplateLayout() const {
  return rq::dereferencePtr(this->_template_layout_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateLayout &
Template::getTemplateLayout() {
  return rq::dereferencePtr(this->_template_layout_ptr);
}

[[nodiscard]] inline bool Template::classof(const Entity *entity) {
  return rq::getIsTemplate(rq::dereferencePtr(entity).getOpcode());
}

inline TemplateClass::TemplateClass(llvm::StringRef name,
                                    const rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table,
                                    rq::TemplateLayout &template_layout)
    : Template(rq::Opcode::SY_TEMPLATE_CLASS, name, expression, attributes,
               module, containing_table, hosting_table, template_layout) {}

[[nodiscard]] inline bool TemplateClass::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_CLASS;
}

inline TemplateEnumeration::TemplateEnumeration(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : Template(rq::Opcode::SY_TEMPLATE_ENUMERATION, name, expression,
               attributes, module, containing_table, hosting_table,
               template_layout) {}

[[nodiscard]] inline bool TemplateEnumeration::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_ENUMERATION;
}

inline TemplateCategory::TemplateCategory(llvm::StringRef name,
                                          const rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::Table &containing_table,
                                          rq::Table &hosting_table,
                                          rq::TemplateLayout &template_layout)
    : Template(rq::Opcode::SY_TEMPLATE_CATEGORY, name, expression, attributes,
               module, containing_table, hosting_table, template_layout) {}

[[nodiscard]] inline bool TemplateCategory::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_CATEGORY;
}

inline TemplateGlobal::TemplateGlobal(llvm::StringRef name,
                                      const rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table,
                                      rq::TemplateLayout &template_layout)
    : Template(rq::Opcode::SY_TEMPLATE_GLOBAL, name, expression, attributes,
               module, containing_table, hosting_table, template_layout) {}

[[nodiscard]] inline bool TemplateGlobal::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_GLOBAL;
}

inline TemplateGlobalStatic::TemplateGlobalStatic(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : Template(rq::Opcode::SY_TEMPLATE_GLOBAL_STATIC, name, expression,
               attributes, module, containing_table, hosting_table,
               template_layout) {}

[[nodiscard]] inline bool TemplateGlobalStatic::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_GLOBAL_STATIC;
}

inline TemplateRanger::TemplateRanger(rq::Opcode opcode, llvm::StringRef name,
                                      const rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table,
                                      rq::TemplateLayout &template_layout)
    : Template(opcode, name, expression, attributes, module, containing_table,
               hosting_table, template_layout) {
  RQ_ASSERT(rq::getIsTemplateRanger(opcode), "not template ranger");
}

[[nodiscard]] inline bool TemplateRanger::classof(const Entity *entity) {
  return rq::getIsTemplateRanger(rq::dereferencePtr(entity).getOpcode());
}

inline TemplateForwardRanger::TemplateForwardRanger(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : TemplateRanger(rq::Opcode::SY_TEMPLATE_FORWARD_RANGER, name, expression,
                     attributes, module, containing_table, hosting_table,
                     template_layout) {}

[[nodiscard]] inline bool TemplateForwardRanger::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_FORWARD_RANGER;
}

inline TemplateBackwardRanger::TemplateBackwardRanger(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : TemplateRanger(rq::Opcode::SY_TEMPLATE_BACKWARD_RANGER, name, expression,
                     attributes, module, containing_table, hosting_table,
                     template_layout) {}

[[nodiscard]] inline bool
TemplateBackwardRanger::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_BACKWARD_RANGER;
}

inline TemplateProcedure::TemplateProcedure(
    rq::Opcode opcode, llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : Template(opcode, name, expression, attributes, module, containing_table,
               hosting_table, template_layout) {
  RQ_ASSERT(rq::getIsTemplateProcedure(opcode), "not template procedure");
}

[[nodiscard]] inline bool TemplateProcedure::classof(const Entity *entity) {
  return rq::getIsTemplateProcedure(rq::dereferencePtr(entity).getOpcode());
}

inline TemplateFunction::TemplateFunction(llvm::StringRef name,
                                          const rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::Table &containing_table,
                                          rq::Table &hosting_table,
                                          rq::TemplateLayout &template_layout)
    : TemplateProcedure(rq::Opcode::SY_TEMPLATE_FUNCTION, name, expression,
                        attributes, module, containing_table, hosting_table,
                        template_layout) {}

[[nodiscard]] inline bool TemplateFunction::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_FUNCTION;
}

inline TemplateMethod::TemplateMethod(llvm::StringRef name,
                                      const rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table,
                                      rq::TemplateLayout &template_layout)
    : TemplateProcedure(rq::Opcode::SY_TEMPLATE_METHOD, name, expression,
                        attributes, module, containing_table, hosting_table,
                        template_layout) {}

[[nodiscard]] inline bool TemplateMethod::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_METHOD;
}

inline TemplateExtensionFunction::TemplateExtensionFunction(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : TemplateProcedure(rq::Opcode::SY_TEMPLATE_EXTENSION_FUNCTION, name,
                        expression, attributes, module, containing_table,
                        hosting_table, template_layout) {}

[[nodiscard]] inline bool
TemplateExtensionFunction::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_EXTENSION_FUNCTION;
}

inline TemplateExtensionMethod::TemplateExtensionMethod(
    llvm::StringRef name, const rq::Expression &expression,
    rq::ExpressionFlags attributes, rq::Module &module,
    rq::Table &containing_table, rq::Table &hosting_table,
    rq::TemplateLayout &template_layout)
    : TemplateProcedure(rq::Opcode::SY_TEMPLATE_EXTENSION_METHOD, name,
                        expression, attributes, module, containing_table,
                        hosting_table, template_layout) {}

[[nodiscard]] inline bool
TemplateExtensionMethod::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::SY_TEMPLATE_EXTENSION_METHOD;
}

inline Specialized::Specialized(rq::Opcode opcode, llvm::StringRef name,
                                const rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table)
    : Symbol(opcode), InitialExpression(expression),
      InitialExpressionFlags(attributes), InitialModuleMember(module),
      TableMember(containing_table), TableHosted(hosting_table),
      InitialNamed(name) {}

[[nodiscard]] inline bool Specialized::classof(const Entity *entity) {
  return rq::getIsSpecialized(rq::dereferencePtr(entity).getOpcode());
}

inline Constant::Constant(rq::Opcode opcode) : Entity(opcode) {}

[[nodiscard]] inline bool Constant::classof(const Entity *entity) {
  return rq::getIsConstant(rq::dereferencePtr(entity).getOpcode());
}

void RQ_ALWAYS_INLINE profileTypeConstant(llvm::FoldingSetNodeID &id,
                                          const rq::Symbol &symbol,
                                          rq::TypeFlags attributes) {
  id.AddPointer(&symbol);
  id.AddInteger(static_cast<unsigned>(attributes));
}

inline TypeConstant::TypeConstant(rq::Symbol &symbol, rq::TypeFlags flags)
    : Constant(rq::Opcode::CT_TYPE), _symbol_ptr(&symbol), _type_flags(flags) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &
TypeConstant::getSymbol() const {
  return rq::dereferencePtr(this->_symbol_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &TypeConstant::getSymbol() {
  return rq::dereferencePtr(this->_symbol_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags
TypeConstant::getTypeFlags() const {
  return this->_type_flags;
}

[[nodiscard]] inline bool TypeConstant::getIsInferencing() const {
  const rq::Symbol &symbol = this->getSymbol();
  if (llvm::isa<rq::Inference>(symbol)) {
    return true;
  }
  if (llvm::isa<rq::InferencedCountArray>(symbol)) {
    return true;
  }
  if (llvm::isa<rq::UnarySubtype>(symbol)) {
    const rq::UnarySubtype &unary = llvm::cast<rq::UnarySubtype>(symbol);
    return unary.getDescendent().getIsInferencing();
  }
  if (llvm::isa<rq::CountedSubtype>(symbol)) {
    const rq::CountedSubtype &counted = llvm::cast<rq::CountedSubtype>(symbol);
    return counted.getDescendent().getIsInferencing();
  }
  return false;
}

[[nodiscard]] inline bool TypeConstant::getIsType() const {
  return this->getTypeFlags() == rq::TypeFlags::NONE &&
         llvm::isa<rq::Type>(this->getSymbol());
}

[[nodiscard]] inline bool TypeConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_TYPE;
}

inline void TypeConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileTypeConstant(id, this->getSymbol(), this->getTypeFlags());
}

void RQ_ALWAYS_INLINE profileExpressionConstant(
    llvm::FoldingSetNodeID &id, const rq::Expression &expression) {
  id.AddPointer(&expression);
}

inline ExpressionConstant::ExpressionConstant(const rq::Expression &expression)
    : Constant(rq::Opcode::CT_EXPRESSION), _expression_ptr(&expression) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
ExpressionConstant::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] inline bool ExpressionConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_EXPRESSION;
}

inline void ExpressionConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileExpressionConstant(id, this->getExpression());
}

inline BooleanConstant::BooleanConstant(bool value)
    : Constant(rq::Opcode::CT_BOOLEAN), _value(value) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool BooleanConstant::getValue() const {
  return this->_value;
}

[[nodiscard]] inline bool BooleanConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_BOOLEAN;
}

void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &int_) {
  int_.Profile(id);
}

inline IntegerConstant::IntegerConstant(const llvm::APInt &int_)
    : Constant(rq::Opcode::CT_INTEGER), _ap_int(int_) {}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &
IntegerConstant::getInt() const {
  return this->_ap_int;
}
[[nodiscard]] inline bool IntegerConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_INTEGER;
}

inline void IntegerConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileIntegerConstant(id, this->getInt());
}

void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &float_) {
  float_.Profile(id);
}

inline FloatConstant::FloatConstant(const llvm::APFloat &float_)
    : Constant(rq::Opcode::CT_FLOAT), _ap_float(float_) {}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &
FloatConstant::getFloat() const {
  return this->_ap_float;
}

[[nodiscard]] inline bool FloatConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_FLOAT;
}

inline void FloatConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileFloatConstant(id, this->getFloat());
}

void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef string) {
  id.AddPointer(string.data());
  id.AddInteger(string.size());
}

inline StringConstant::StringConstant(llvm::StringRef string)
    : Constant(rq::Opcode::CT_STRING), _string(string) {}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
StringConstant::getString() const {
  return this->_string;
}

[[nodiscard]] inline bool StringConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_STRING;
}

inline void StringConstant::Profile(llvm::FoldingSetNodeID &id) const {
  rq::profileStringConstant(id, this->getString());
}

inline ArrayConstant::ArrayConstant() : Constant(rq::Opcode::CT_ARRAY) {}

[[nodiscard]] inline bool ArrayConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_ARRAY;
}

inline ExpressionAttributeConstant::ExpressionAttributeConstant(
    rq::ExpressionAttribute expression_attribute)
    : Constant(rq::Opcode::CT_EXPRESSION_ATTRIBUTE),
      _expression_attribute(expression_attribute) {}

[[nodiscard]] rq::ExpressionAttribute
ExpressionAttributeConstant::getExpressionAttribute() const {
  return this->_expression_attribute;
}

[[nodiscard]] inline bool
ExpressionAttributeConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::CT_EXPRESSION_ATTRIBUTE;
}

inline TypeAttributeConstant::TypeAttributeConstant(
    rq::TypeAttribute type_attribute)
    : Constant(rq::Opcode::CT_TYPE_ATTRIBUTE), _type_attribute(type_attribute) {
}

[[nodiscard]] rq::TypeAttribute
TypeAttributeConstant::getTypeAttribute() const {
  return this->_type_attribute;
}

[[nodiscard]] inline bool TypeAttributeConstant::classof(const Entity *entity) {
  return rq::dereferencePtr(entity).getOpcode() ==
         rq::Opcode::CT_TYPE_ATTRIBUTE;
}

inline Instruction::Instruction(rq::Opcode opcode) : Entity(opcode) {
  RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction");
}

RQ_ALWAYS_INLINE void
Instruction::setExpression(const rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Instruction::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Instruction::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

inline NullaryInstruction::NullaryInstruction(rq::Opcode opcode)
    : Instruction(opcode) {
  RQ_ASSERT(rq::getIsNullaryInstruction(opcode), "not nullary instruction");
}

[[nodiscard]] inline bool NullaryInstruction::classof(const Entity *entity) {
  return rq::getIsNullaryInstruction(rq::dereferencePtr(entity).getOpcode());
}

inline UnaryInstruction::UnaryInstruction(rq::Opcode opcode)
    : Instruction(opcode) {
  RQ_ASSERT(rq::getIsUnaryInstruction(opcode), "not unary instruction");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool UnaryInstruction::getHasAddress0() const {
  return this->_address0_ptr != nullptr;
}

RQ_ALWAYS_INLINE void UnaryInstruction::setAddress0(rq::Entity &address0) {
  rq::assignSingleValue(this->_address0_ptr, &address0);
}

RQ_ALWAYS_INLINE void UnaryInstruction::setAddress0(rq::Entity *address0_ptr) {
  rq::assignSingleValue(this->_address0_ptr, address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
UnaryInstruction::replaceAddress0(rq::Entity &address0) {
  return rq::replaceValue(this->_address0_ptr, &address0);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
UnaryInstruction::replaceAddress0(rq::Entity *address0_ptr) {
  return rq::replaceValue(this->_address0_ptr, address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
UnaryInstruction::getAddress0() const {
  return rq::dereferencePtr(this->_address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &UnaryInstruction::getAddress0() {
  return rq::dereferencePtr(this->_address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &UnaryInstruction::popAddress0() {
  rq::Entity &address0 = this->getAddress0();
  this->_address0_ptr = nullptr;
  return address0;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *UnaryInstruction::popAddress0Ptr() {
  rq::Entity *address0_ptr = this->_address0_ptr;
  this->_address0_ptr = nullptr;
  return address0_ptr;
}

[[nodiscard]] inline bool UnaryInstruction::classof(const Entity *entity) {
  return rq::getIsUnaryInstruction(rq::dereferencePtr(entity).getOpcode());
}

inline BinaryInstruction::BinaryInstruction(rq::Opcode opcode)
    : Instruction(opcode) {
  RQ_ASSERT(rq::getIsBinaryInstruction(opcode), "not binary instruction");
}

[[nodiscard]] RQ_ALWAYS_INLINE bool BinaryInstruction::getHasAddress0() const {
  return this->_address0_ptr != nullptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool BinaryInstruction::getHasAddress1() const {
  return this->_address1_ptr != nullptr;
}

RQ_ALWAYS_INLINE void BinaryInstruction::setAddress0(rq::Entity &address0) {
  rq::assignSingleValue(this->_address0_ptr, &address0);
}

RQ_ALWAYS_INLINE void BinaryInstruction::setAddress0(rq::Entity *address0_ptr) {
  rq::assignSingleValue(this->_address0_ptr, address0_ptr);
}

RQ_ALWAYS_INLINE void BinaryInstruction::setAddress1(rq::Entity &address1) {
  rq::assignSingleValue(this->_address1_ptr, &address1);
}

RQ_ALWAYS_INLINE void BinaryInstruction::setAddress1(rq::Entity *address1_ptr) {
  rq::assignSingleValue(this->_address1_ptr, address1_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
BinaryInstruction::replaceAddress0(rq::Entity &address0) {
  return rq::replaceValue(this->_address0_ptr, &address0);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
BinaryInstruction::replaceAddress0(rq::Entity *address0_ptr) {
  return rq::replaceValue(this->_address0_ptr, address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
BinaryInstruction::replaceAddress1(rq::Entity &address1) {
  return rq::replaceValue(this->_address1_ptr, &address1);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
BinaryInstruction::replaceAddress1(rq::Entity *address1_ptr) {
  return rq::replaceValue(this->_address1_ptr, address1_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
BinaryInstruction::getAddress0() const {
  return rq::dereferencePtr(this->_address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &BinaryInstruction::getAddress0() {
  return rq::dereferencePtr(this->_address0_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
BinaryInstruction::getAddress1() const {
  return rq::dereferencePtr(this->_address1_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &BinaryInstruction::getAddress1() {
  return rq::dereferencePtr(this->_address1_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &BinaryInstruction::popAddress0() {
  rq::Entity &address0 = this->getAddress0();
  this->_address0_ptr = nullptr;
  return address0;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *BinaryInstruction::popAddress0Ptr() {
  rq::Entity *address0_ptr = this->_address0_ptr;
  this->_address0_ptr = nullptr;
  return address0_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &BinaryInstruction::popAddress1() {
  rq::Entity &address1 = this->getAddress1();
  this->_address1_ptr = nullptr;
  return address1;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *BinaryInstruction::popAddress1Ptr() {
  rq::Entity *address1_ptr = this->_address1_ptr;
  this->_address1_ptr = nullptr;
  return address1_ptr;
}

[[nodiscard]] inline bool BinaryInstruction::classof(const Entity *entity) {
  return rq::getIsBinaryInstruction(rq::dereferencePtr(entity).getOpcode());
}

} // namespace rq