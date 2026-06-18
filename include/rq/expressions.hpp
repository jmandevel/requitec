#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/entity.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstddef>
#include <cstdint>
#include <ranges>

namespace rq {

// Expression represents individual expressions in the AST. Each is identified
// by a keyword.
struct Expression;

static constexpr std::size_t KEYWORD_COUNT =
    static_cast<std::size_t>(rq::Keyword::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::NONE:
    return "none";

    // LITERALS
  case K::INTEGER_LITERAL:
    return "__integer_literal";
  case K::FLOAT_LITERAL:
    return "__float_literal";
  case K::STRING_LITERAL:
    return "__string_literal";
  case K::CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case K::IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case K::ERROR:
    return "__error";

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return "_unsituated_parenthesis_group";
  case K::UNSITUATED_EQUAL_OPERATOR:
    return "_unsituated_equal_operator";
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
    return "_unsituated_ascribe_expression";
  case K::UNSITUATED_ASCRIBE_TYPE:
    return "_unsituated_ascribe_type";

  // LOGICAL
  case K::LOGICAL_AND:
    return "_logical_and";
  case K::LOGICAL_OR:
    return "_logical_or";
  case K::LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case K::GREATER:
    return "_greater";
  case K::GREATER_EQUAL:
    return "_greater_equal";
  case K::LESS:
    return "_less";
  case K::LESS_EQUAL:
    return "_less_equal";
  case K::EQUAL:
    return "_equal";
  case K::NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case K::EXTEND:
    return "_extend";
  case K::INSTANTIATE_EXTENSION:
    return "_instantiate_extension";
  case K::BINDING:
    return "_binding";
  case K::ASCRIBE_TYPE:
    return "_ascribe_type";
  case K::ASCRIBE_EXPRESSION:
    return "_ascribe_expression";
  case K::ASCRIBE_RECIEVER:
    return "_ascribe_reciever";
  case K::INSTANTIATE_EXPRESSION_ATTRIBUTE:
    return "_instantiate_expression_attribute";
  case K::INSTANTIATE_TYPE_ATTRIBUTE:
    return "_instantiate_type_attribute";
  case K::IDENTIFY:
    return "identify";
  case K::IDENTIFY_OF:
    return "_identify_of";
  case K::FORK:
    return "_fork";

  // JUXTAPOSITIONAL
  case K::CONCATENATE:
    return "_concatenate";
  case K::APPEND:
    return "_append";

  // ARITHMETIC
  case K::ADD:
    return "_add";
  case K::SUBTRACT:
    return "_subtract";
  case K::MULTIPLY:
    return "_multiply";
  case K::DIVIDE:
    return "_divide";
  case K::MODULUS:
    return "_modulus";
  case K::NEGATE:
    return "_negate";

  // CASTS
  case K::AS:
    return "as";
  case K::AS_OF:
    return "_as_of";
  case K::OF:
    return "of";
  case K::OF_OF:
    return "_of_of";
  case K::CAST:
    return "cast";
  case K::CAST_OF:
    return "_cast_of";
  case K::BITWISE_CAST:
    return "bitwise_cast";
  case K::BITWISE_CAST_OF:
    return "_bitwise_cast_of";
  case K::PROCEDURE_CAST:
    return "procedure_cast";
  case K::PROCEDURE_CAST_OF:
    return "_procedure_cast_of";

  // BITWISE
  case K::BITWISE_OR:
    return "_bitwise_or";
  case K::BITWISE_AND:
    return "_bitwise_and";
  case K::BITWISE_XOR:
    return "_bitwise_xor";
  case K::BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case K::BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case K::BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case K::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case K::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case K::ASSIGN:
    return "_assign";
  case K::CONTENT:
    return "content";
  case K::CONTENT_OF:
    return "_content_of";
  case K::ADDRESS:
    return "address";
  case K::ADDRESS_OF:
    return "_address_of";
  case K::SLICE:
    return "slice";
  case K::SLICE_OF:
    return "_slice_of";
  case K::PROCEDURE_ADDRESS:
    return "procedure_address";
  case K::PROCEDURE_ADDRESS_OF:
    return "_procedure_address_of";
  case K::BORROW:
    return "borrow";
  case K::BORROW_OF:
    return "_borrow_of";
  case K::DATA_ADDRESS:
    return "data_address";
  case K::DATA_ADDRESS_OF:
    return "_data_address_of";
  case K::MOVE:
    return "move";
  case K::MOVE_OF:
    return "_move_of";
  case K::TAKE:
    return "take";
  case K::TAKE_OF:
    return "_take_of";
  case K::CALL:
    return "_call";
  case K::EMPLACE:
    return "emplace";
  case K::EMPLACE_OF:
    return "_emplace_of";
  case K::INVOKE:
    return "invoke";
  case K::INVOKE_OF:
    return "_invoke_of";
  case K::COMPOSE:
    return "compose";
  case K::COMPOSE_OF:
    return "_compose_of";
  case K::DECOMPOSE:
    return "decompose";
  case K::DECOMPOSE_OF:
    return "_decompose_of";
  case K::ADAPT:
    return "adapt";
  case K::ADAPT_OF:
    return "_adapt_of";
  case K::DESTROY:
    return "destroy";
  case K::DESTROY_OF:
    return "_destroy_of";
  case K::DROP:
    return "drop";
  case K::DROP_OF:
    return "_drop_of";
  case K::INPLACE_DESTROY:
    return "inplace_destroy";
  case K::INPLACE_DESTROY_OF:
    return "_inplace_destroy_of";
  case K::INPLACE_INITIALIZE:
    return "inplace_initialize";
  case K::INPLACE_INITIALIZE_OF:
    return "_inplace_initialize_of";

  // SUBTYPE
  case K::INSTANTIATE_ARRAY:
    return "_instantiate_array";
  case K::INSTANTIATE_REFERENCE:
    return "_instantiate_reference";
  case K::INSTANTIATE_POINTER:
    return "_instantiate_pointer";
  case K::INSTANTIATE_SLICE:
    return "_instantiate_slice";

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case K::NONPOSITIONAL_PARAMETERS_BEGIN:
    return "_nonpositional_parameters_begin";
  case K::LOCKED_PARAMETERS_BEGIN:
    return "_locked_parameters_begin";
  case K::NONAME:
    return "noname";

  // BRACES
  case K::INSTANTIATE_TUPLE:
    return "_instantiate_tuple";
  case K::INSTANTIATE_LAYOUT:
    return "_instantiate_layout";
  case K::INSTANTIATE_TEMPLATE:
    return "_instantiate_template";

  // PROCEDURES
  case K::NAMED_ARGUMENT:
    return "_named_argument";
  case K::INSTANTIATE_SIGNATURE:
    return "_instantiate_signature";
  case K::PLACEMENT:
    return "placement";
  case K::COMPOSITION:
    return "composition";
  case K::DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case K::MAIN:
    return "main";
  case K::FUNCTION:
    return "function";
  case K::IMPLEMENT_FUNCTION:
    return "implement_function";
  case K::USE_FUNCTION:
    return "use_function";

  // CONTROL FLOW
  case K::RETURN:
    return "return";
  case K::BREAK:
    return "break";
  case K::CONTINUE:
    return "continue";
  case K::FALLTHROUGH:
    return "fallthrough";
  case K::GOTO:
    return "goto";
  case K::RANGE_OVER:
    return "range_over";

  // DECLARED TYPES
  case K::CLASS:
    return "class";
  case K::ENUMERATION:
    return "enumeration";
  case K::INTERFACE:
    return "interface";
  case K::ADAPTER:
    return "adapter";

  // VALUES
  case K::ARRAY:
    return "array";
  case K::NULL_:
    return "null";
  case K::TRUE:
    return "true";
  case K::FALSE:
    return "false";
  case K::VALUE:
    return "value";
  case K::INDEX:
    return "index";
  case K::DISCRIMINANT:
    return "discriminant";
  case K::OUT:
    return "out";
  case K::THIS:
    return "this";
  case K::RESULT:
    return "result";
  case K::COMMAND_LINE_ARGUMENTS:
    return "command_line_arguments";
  case K::CALLSITE:
    return "callsite";

  // BUILTIN TYPES
  case K::INFERENCE:
    return "_inference";
  case K::EXPRESSION:
    return "expression";
  case K::VOID:
    return "void";
  case K::NO_RETURN:
    return "no_return";
  case K::BOOLEAN:
    return "boolean";
  case K::HALF:
    return "half";
  case K::SINGLE:
    return "single";
  case K::DOUBLE:
    return "double";
  case K::QUADRUPLE:
    return "quadruple";
  case K::BINARY16:
    return "binary16";
  case K::BINARY32:
    return "binary32";
  case K::BINARY64:
    return "binary64";
  case K::BINARY128:
    return "binary128";
  case K::BFLOAT16:
    return "bfloat16";
  case K::SIGNED_INTEGER:
    return "signed_integer";
  case K::UNSIGNED_INTEGER:
    return "unsigned_integer";
  case K::FAST_SIGNED_INTEGER:
    return "fast_signed_integer";
  case K::FAST_UNSIGNED_INTEGER:
    return "fast_unsigned_integer";
  case K::LEAST_SIGNED_INTEGER:
    return "least_signed_integer";
  case K::LEAST_UNSIGNED_INTEGER:
    return "least_unsigned_integer";
  case K::SIGNED_INDEX:
    return "signed_index";
  case K::UNSIGNED_INDEX:
    return "unsigned_index";
  case K::SIGNED_ADDRESS:
    return "signed_address";
  case K::UNSIGNED_ADDRESS:
    return "unsigned_address";
  case K::CHAR:
    return "char";
  case K::ASCII:
    return "ascii";
  case K::UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS_TYPE:
    return "variadic_arguments_type";
  case K::FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case K::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    return "_next_variadic_argument_of";
  case K::VARIADIC_ARGUMENTS:
    return "variadic_arguments";

  // SCOPES
  case K::IF:
    return "if";
  case K::ELSE_IF:
    return "else_if";
  case K::ELSE:
    return "else";
  case K::MATCH:
    return "match";
  case K::SWITCH:
    return "switch";
  case K::CASE:
    return "case";
  case K::DEFAULT:
    return "default";
  case K::FOR:
    return "for";
  case K::WHILE:
    return "while";
  case K::SPIN:
    return "spin";
  case K::WEAVE:
    return "weave";
  case K::SCOPE:
    return "scope";
  case K::BLOCK:
    return "block";

  // RANGES
  case K::ARITHMETIC_SEQUENCE:
    return "_arithmetic_sequence";
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return "_arithmetic_sequence_condition_less";
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return "_arithmetic_sequence_condition_greater";
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return "_arithmetic_sequence_condition_less_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return "_arithmetic_sequence_condition_greater_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return "_arithmetic_sequence_condition_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return "_arithmetic_sequence_condition_not_equal";
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return "_arithmetic_sequence_step_add";
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return "_arithmetic_sequence_step_subtract";
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return "_arithmetic_sequence_step_multiply";
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return "_arithmetic_sequence_step_divide";
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return "_arithmetic_sequence_step_modulus";

  // TABLE GRAPH
  case K::IMPORT:
    return "import";
  case K::NAMESPACE:
    return "namespace";
  case K::TOP:
    return "_top";
  case K::LABEL:
    return "label";

  // HINTS
  case K::DEBUG_BREAK:
    return "debug_break";
  case K::ABORT:
    return "abort";
  case K::ASSERT:
    return "assert";
  case K::UNREACHABLE:
    return "unreachable";
  case K::ASSUME:
    return "assume";

  // EXPRESSION ATTRIBUTES
  case K::NO_ANCHOR:
    return "no_anchor";
  case K::ANCHOR:
    return "anchor";
  case K::NO_OPAQUE:
    return "no_opaque";
  case K::OPAQUE:
    return "opaque";
  case K::NO_FLANK:
    return "no_flank";
  case K::FLANK:
    return "flank";
  case K::NO_GLOBAL:
    return "no_global";
  case K::GLOBAL:
    return "global";
  case K::NO_ACCESS:
    return "no_access";
  case K::EXPORT:
    return "export";
  case K::PUBLIC:
    return "public";
  case K::NO_PARTIAL_MUTATE:
    return "no_partial_mutate";
  case K::PARTIAL_MUTATE:
    return "partial_mutate";
  case K::NO_STATIC:
    return "no_static";
  case K::STATIC:
    return "static";
  case K::NO_CAPTURE:
    return "no_capture";
  case K::CAPTURE:
    return "capture";
  case K::NO_INLINE:
    return "no_inline";
  case K::INLINE:
    return "inline";
  case K::NO_MANGLE:
    return "no_mangle";
  case K::MANGLE:
    return "mangle";
  case K::NO_PACK:
    return "no_pack";
  case K::PACK:
    return "pack";
  case K::NO_BRANCH_TREND:
    return "no_branch_trend";
  case K::LIKELY:
    return "likely";
  case K::UNLIKELY:
    return "unlikely";
  case K::NO_SUPPORT_STATUS:
    return "no_support_status";
  case K::DEPRECIATE:
    return "depreciate";
  case K::EXPERIMENTAL:
    return "experimental";
  case K::NO_STABLE_ADDRESS:
    return "no_stable_address";
  case K::STABLE_ADDRESS:
    return "stable_address";
  case K::NO_VARIADIC:
    return "no_variadic";
  case K::VARIADIC:
    return "variadic";
  case K::NO_LOCATION:
    return "no_location";
  case K::LOCATION:
    return "location";
  case K::NO_TEMPLATE:
    return "no_template";
  case K::TEMPLATE:
    return "template";
  case K::NO_CONSTRAINT:
    return "no_constraint";
  case K::CONSTRAINT:
    return "constraint";
  case K::NO_WEIGHT:
    return "no_weight";
  case K::WEIGHT:
    return "weight";
  case K::NO_REQUIRE:
    return "no_require";
  case K::REQUIRE:
    return "require";
  case K::NO_ENSURE:
    return "no_ensure";
  case K::ENSURE:
    return "ensure";
  case K::NO_RANGER:
    return "no_ranger";
  case K::RANGER:
    return "ranger";

  // TYPE ATTRIBUTES
  case K::NO_VAR:
    return "no_var";
  case K::VAR:
    return "var";
  case K::PARTIAL_VAR:
    return "partial_var";
  case K::NO_VOLATILE:
    return "no_volatile";
  case K::VOLATILE:
    return "volatile";
  case K::NO_ATOMIC:
    return "no_atomic";
  case K::ATOMIC:
    return "atomic";
  case K::NO_NULL_TERMINATE:
    return "no_null_terminate";
  case K::NULL_TERMINATE:
    return "null_terminate";

  // EXPRESSION ATTRIBUTE TYPES
  case K::ANCHOR_ATTRIBUTE:
    return "anchor_attribute";
  case K::OPAQUE_ATTRIBUTE:
    return "opaque_attribute";
  case K::FLANK_ATTRIBUTE:
    return "flank_attribute";
  case K::GLOBAL_ATTRIBUTE:
    return "global_attribute";
  case K::ACCESS_ATTRIBUTE:
    return "access_attribute";
  case K::PARTIAL_MUTATE_ATTRIBUTE:
    return "partial_mutate_attribute";
  case K::STATIC_ATTRIBUTE:
    return "static_attribute";
  case K::CAPTURE_ATTRIBUTE:
    return "capture_attribute";
  case K::INLINE_ATTRIBUTE:
    return "inline_attribute";
  case K::MANGLE_ATTRIBUTE:
    return "mangle_attribute";
  case K::PACK_ATTRIBUTE:
    return "pack_attribute";
  case K::BRANCH_TREND_ATTRIBUTE:
    return "branch_trend_attribute";
  case K::SUPPORT_STATUS_ATTRIBUTE:
    return "support_status_attribute";
  case K::STABLE_ADDRESS_ATTRIBUTE:
    return "stable_address_attribute";
  case K::VARIADIC_ATTRIBUTE:
    return "variadic_attribute";
  case K::LOCATION_ATTRIBUTE:
    return "location_attribute";
  case K::TEMPLATE_ATTRIBUTE:
    return "template_attribute";
  case K::CONSTRAINT_ATTRIBUTE:
    return "constraint_attribute";
  case K::WEIGHT_ATTRIBUTE:
    return "weight_attribute";
  case K::REQUIRE_ATTRIBUTE:
    return "require_attribute";
  case K::ENSURE_ATTRIBUTE:
    return "ensure_attribute";
  case K::RANGER_ATTRIBUTE:
    return "ranger_attribute";

  // TYPE ATTRIBUTE TYPES
  case K::VAR_ATTRIBUTE:
    return "var_attribute";
  case K::VOLATILE_ATTRIBUTE:
    return "volatile_attribute";
  case K::ATOMIC_ATTRIBUTE:
    return "atomic_attribute";
  case K::NULL_TERMINATE_ATTRIBUTE:
    return "null_terminate_attribute";

  // REFLECTIONS
  case K::REFLECT:
    return "_reflect";
  case K::MEMBER_OF:
    return "_member_of";
  case K::IGNORE:
    return "ignore";
  case K::IGNORE_OF:
    return "_ignore_of";
  case K::BAKE:
    return "bake";
  case K::BAKE_OF:
    return "_bake_of";
  case K::BYTE_SIZE:
    return "byte_size";
  case K::BYTE_SIZE_OF:
    return "_byte_size_of";
  case K::BIT_DEPTH:
    return "bit_depth";
  case K::BIT_DEPTH_OF:
    return "_bit_depth_of";
  case K::ELEMENT_COUNT:
    return "element_count";
  case K::ELEMENT_COUNT_OF:
    return "_element_count_of";
  case K::SNIPPET:
    return "snippet";
  case K::SNIPPET_OF:
    return "_snippet_of";
  case K::NAME:
    return "name";
  case K::NAME_OF:
    return "_name_of";
  case K::LINE:
    return "line";
  case K::LINE_OF:
    return "_line_of";
  case K::COLUMN:
    return "column";
  case K::COLUMN_OF:
    return "_column_of";
  case K::IS:
    return "is";
  case K::IS_OF:
    return "_is_of";
  case K::HOLDS:
    return "holds";
  case K::HOLDS_OF:
    return "_holds_of";
  case K::TYPE:
    return "type";
  case K::TYPE_OF:
    return "_type_of";
  case K::SYMBOL:
    return "symbol";
  case K::SYMBOL_OF:
    return "_symbol_of";
  case K::HAS_MEMBER:
    return "has_member";
  case K::HAS_MEMBER_OF:
    return "_has_member_of";
  case K::HAS:
    return "has";
  case K::HAS_OF:
    return "_has_of";
  case K::GET:
    return "get";
  case K::GET_OF:
    return "_get_of";
  case K::SIGNATURE:
    return "signature";
  case K::SIGNATURE_OF:
    return "_signature_of";
  case K::SYNONYM:
    return "synonym";
  case K::SYNONYM_OF:
    return "_synonym_of";
  case K::CAPTURE_OF:
    return "_capture_of";
    return "_is_ok_of";
  case K::AS_EXTENSION:
    return "_as_extension";
  case K::AS_EXTENSION_OF:
    return "_as_extension_of";
    return "_reverse_of";
  case K::INCREMENT:
    return "increment";
  case K::INCREMENT_OF:
    return "_increment_of";
  case K::DECREMENT:
    return "decrement";
  case K::DECREMENT_OF:
    return "_decrement_of";
  case K::WHILST:
    return "whilst";
  case K::WHILST_OF:
    return "_whilst_of";
  case K::ELEMENT:
    return "element";
  case K::ELEMENT_OF:
    return "_element_of";
  case K::AT:
    return "at";
  case K::AT_OF:
    return "_at_of";
  case K::FORWARD:
    return "forward";
  case K::FORWARD_OF:
    return "_forward_of";
  case K::BACKWARD:
    return "backward";
  case K::BACKWARD_OF:
    return "_backward_of";
  case K::IS_TYPE:
    return "is_type";
  case K::IS_TYPE_OF:
    return "_is_type_of";
  case K::IS_RANGE_TYPE:
    return "is_range_type";
  case K::IS_RANGE_TYPE_OF:
    return "_is_range_type_of";
  case K::IS_PLACEMENT_TYPE:
    return "is_placement_type";
  case K::IS_PLACEMENT_TYPE_OF:
    return "_is_placement_type_of";
  case K::IS_SIGNED_TYPE:
    return "is_signed_type";
  case K::IS_SIGNED_TYPE_OF:
    return "_is_signed_type_of";
  case K::IS_UNSIGNED_TYPE:
    return "is_unsigned_type";
  case K::IS_UNSIGNED_TYPE_OF:
    return "_is_unsigned_type_of";
  case K::IS_INTEGER_TYPE:
    return "is_integer_type";
  case K::IS_INTEGER_TYPE_OF:
    return "_is_integer_type_of";
  case K::IS_FLOAT_TYPE:
    return "is_float_type";
  case K::IS_FLOAT_TYPE_OF:
    return "_is_float_type_of";
  case K::IS_BINARY_TYPE:
    return "is_binary_type";
  case K::IS_BINARY_TYPE_OF:
    return "_is_binary_type_of";
  case K::IS_BFLOAT_TYPE:
    return "is_bfloat_type";
  case K::IS_BFLOAT_TYPE_OF:
    return "_is_bfloat_type_of";
  case K::IS_STRING_TYPE:
    return "is_string_teyp";
  case K::IS_STRING_TYPE_OF:
    return "_is_string_type_of";
  case K::IS_CODEUNIT_TYPE:
    return "is_codeunit_type";
  case K::IS_CODEUNIT_TYPE_OF:
    return "_is_codeunit_type_of";
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE:
    return "is_expression_attribute_type";
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE_OF:
    return "_is_expression_attribute_type_of";
  case K::IS_TYPE_ATTRIBUTE_TYPE:
    return "is_type_attribute_type";
  case K::IS_TYPE_ATTRIBUTE_TYPE_OF:
    return "_is_type_attribute_type_of";

  case K::LAST:
    break;

    // NOTE: don't add default case so compiler will warn if cases are missing!
  }
  RQ_UNREACHABLE();
}

enum class KeywordFlags : std::uint32_t {
  NONE = 0,
  CONVERGING = rq::getBit(0),
  LITERAL = rq::getBit(1),
  UNQUOTED_LEFT = rq::getBit(2),
  UNQUOTED_RIGHT = rq::getBit(3),
  INTERNAL = rq::getBit(4),
  UNIVERSALIZABLE = rq::getBit(5),
  STARTING_CHAINLINK = rq::getBit(6),
  CONTINUING_CHAINLINK = rq::getBit(7),
  FINISHING_CHAINLINK = rq::getBit(8),
  // TOP
  STATEMENT = rq::getBit(9),
  RVALUE = rq::getBit(10),
  LVALUE = rq::getBit(11),
  REFLECTION = rq::getBit(12),
  ARGUMENT = rq::getBit(13),
  PARAMETER = rq::getBit(14),
  BINDING = rq::getBit(15),
  NAME = rq::getBit(16),
  NAMESPACE = rq::getBit(17),
  ASCRIPTION = rq::getBit(18),
  EXPRESSION_ATTRIBUTE = rq::getBit(19),
  TYPE_ATTRIBUTE = rq::getBit(20),
  ARITHMETIC_SEQUENCE_STEP = rq::getBit(21),
  ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(22),
  ALL_SITUATIONS = STATEMENT | RVALUE | LVALUE | REFLECTION | ARGUMENT |
                   PARAMETER | BINDING | NAME | NAMESPACE | ASCRIPTION |
                   TYPE_ATTRIBUTE | EXPRESSION_ATTRIBUTE |
                   ARITHMETIC_SEQUENCE_STEP | ARITHMETIC_SEQUENCE_CONDITION,

};

template <> struct is_flags<rq::KeywordFlags> : std::true_type {};

[[nodiscard]] inline rq::KeywordFlags getFlags(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using KF = KeywordFlags;
  switch (keyword) {
  case K::NONE:
    return KF::NONE;

  // LITERALS
  case K::INTEGER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::FLOAT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::STRING_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::CODEUNIT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::IDENTIFIER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER | KF::NAME |
           KF::NAMESPACE;

  // ERRORS
  case K::ERROR:
    return KF::INTERNAL;

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT | KF::LVALUE | KF::NAME |
           KF::NAMESPACE | KF::ARITHMETIC_SEQUENCE_STEP |
           KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::UNSITUATED_EQUAL_OPERATOR:
    return KF::STATEMENT | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSITUATED_ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
    return KF::STATEMENT | KF::RVALUE | KF::PARAMETER | KF::ARGUMENT |
           KF::ASCRIPTION;

  // LOGICAL
  case K::LOGICAL_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LOGICAL_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LOGICAL_COMPLEMENT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // COMPARISON
  case K::GREATER:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::GREATER_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LESS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LESS_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::NOT_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // APPLY
  case K::EXTEND:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INSTANTIATE_EXTENSION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINDING:
    return KF::LVALUE | KF::PARAMETER | KF::ARGUMENT | KF::BINDING;
  case K::ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::ASCRIBE_EXPRESSION:
    return KF::STATEMENT | KF::PARAMETER | KF::ARGUMENT | KF::ASCRIPTION;
  case K::ASCRIBE_RECIEVER:
    return KF::RVALUE | KF::ARGUMENT | KF::ASCRIPTION;
  case K::INSTANTIATE_EXPRESSION_ATTRIBUTE:
    return KF::NONE; // EXPRESSION_ATTRIBUTE_INSTANTIATION
  case K::INSTANTIATE_TYPE_ATTRIBUTE:
    return KF::NONE; // TYPE_ATTRIBUTE_INSTANTIATION
  case K::IDENTIFY:
    return KF::REFLECTION | KF::ASCRIPTION;
  case K::IDENTIFY_OF:
    return KF::NAME | KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::NAMESPACE;
  case K::FORK:
    return KF::NONE;

  // JUXTAPOSITIONAL
  case K::CONCATENATE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::APPEND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // ARITHMETIC
  case K::ADD:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::SUBTRACT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::MULTIPLY:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::DIVIDE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::MODULUS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::NEGATE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // CASTS
  case K::AS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::AS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::OF:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::OF_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BITWISE_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::PROCEDURE_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::PROCEDURE_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // BITWISE
  case K::BITWISE_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_XOR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_COMPLEMENT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_SHIFT_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_SHIFT_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;

  // MEMORY
  case K::ASSIGN:
    return KF::STATEMENT;
  case K::CONTENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::CONTENT_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::ADDRESS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SLICE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SLICE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::PROCEDURE_ADDRESS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::PROCEDURE_ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BORROW:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BORROW_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DATA_ADDRESS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DATA_ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::MOVE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::MOVE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TAKE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::TAKE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CALL:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::EMPLACE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::EMPLACE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INVOKE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::INVOKE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COMPOSE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::COMPOSE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DECOMPOSE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DECOMPOSE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ADAPT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ADAPT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DROP:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DROP_OF:
    return KF::STATEMENT;
  case K::INPLACE_DESTROY:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::INPLACE_DESTROY_OF:
    return KF::STATEMENT;
  case K::INPLACE_INITIALIZE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::INPLACE_INITIALIZE_OF:
    return KF::STATEMENT;

  // SUBTYPE
  case K::INSTANTIATE_ARRAY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INSTANTIATE_REFERENCE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INSTANTIATE_POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INSTANTIATE_SLICE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return KF::PARAMETER;
  case K::NONPOSITIONAL_PARAMETERS_BEGIN:
    return KF::PARAMETER;
  case K::LOCKED_PARAMETERS_BEGIN:
    return KF::PARAMETER;
  case K::NONAME:
    return KF::NAME | KF::LVALUE;

  // BRACES
  case K::INSTANTIATE_TUPLE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INSTANTIATE_LAYOUT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INSTANTIATE_TEMPLATE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::NAMED_ARGUMENT:
    return KF::ARGUMENT;
  case K::INSTANTIATE_SIGNATURE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PLACEMENT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::COMPOSITION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DEFAULT_VALUE_PARAMETER:
    return KF::PARAMETER;
  case K::MAIN:
    return KF::STATEMENT;
  case K::FUNCTION:
    return KF::STATEMENT;
  case K::IMPLEMENT_FUNCTION:
    return KF::STATEMENT;
  case K::USE_FUNCTION:
    return KF::STATEMENT;

  // CONTROL FLOW
  case K::RETURN:
    return KF::STATEMENT;
  case K::BREAK:
    return KF::STATEMENT;
  case K::CONTINUE:
    return KF::STATEMENT;
  case K::FALLTHROUGH:
    return KF::STATEMENT;
  case K::GOTO:
    return KF::STATEMENT;
  case K::RANGE_OVER:
    return KF::STATEMENT;

  // DECLARED TYPES
  case K::CLASS:
    return KF::STATEMENT;
  case K::ENUMERATION:
    return KF::STATEMENT;
  case K::INTERFACE:
    return KF::STATEMENT;
  case K::ADAPTER:
    return KF::STATEMENT;

  // VALUES
  case K::ARRAY:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NULL_:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TRUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::FALSE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VALUE:
    return KF::RVALUE;
  case K::INDEX:
    return KF::RVALUE;
  case K::DISCRIMINANT:
    return KF::RVALUE;
  case K::OUT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::THIS:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::RESULT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::COMMAND_LINE_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CALLSITE:
    return KF::RVALUE;

  // BUILTIN TYPES
  case K::INFERENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPRESSION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::VOID:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NO_RETURN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BOOLEAN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::HALF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SINGLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DOUBLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::QUADRUPLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY32:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY64:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY128:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BFLOAT16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FAST_SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FAST_UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LEAST_SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LEAST_UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CHAR:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ASCII:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UTF8:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FIRST_VARIADIC_ARGUMENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VARIADIC_ARGUMENTS_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // SCOPES
  case K::IF:
    return KF::STATEMENT | KF::STARTING_CHAINLINK;
  case K::ELSE_IF:
    return KF::STATEMENT | KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK;
  case K::ELSE:
    return KF::STATEMENT | KF::FINISHING_CHAINLINK;
  case K::MATCH:
    return KF::STATEMENT | KF::RVALUE | KF::ARGUMENT;
  case K::SWITCH:
    return KF::STATEMENT | KF::RVALUE | KF::ARGUMENT;
  case K::CASE:
    return KF::STATEMENT | KF::STARTING_CHAINLINK | KF::CONTINUING_CHAINLINK |
           KF::FINISHING_CHAINLINK;
  case K::DEFAULT:
    return KF::STATEMENT | KF::STARTING_CHAINLINK | KF::CONTINUING_CHAINLINK |
           KF::FINISHING_CHAINLINK;
  case K::FOR:
    return KF::STATEMENT;
  case K::WHILE:
    return KF::STATEMENT;
  case K::SPIN:
    return KF::STATEMENT | KF::STARTING_CHAINLINK;
  case K::WEAVE:
    return KF::STATEMENT | KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK;
  case K::SCOPE:
    return KF::STATEMENT | KF::RVALUE | KF::ARGUMENT;
  case K::BLOCK:
    return KF::STATEMENT | KF::RVALUE | KF::ARGUMENT;

  // RANGES
  case K::ARITHMETIC_SEQUENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return KF::ARITHMETIC_SEQUENCE_STEP;

  // TABLE GRAPH
  case K::IMPORT:
    return KF::STATEMENT;
  case K::NAMESPACE:
    return KF::STATEMENT | KF::RVALUE;
  case K::TOP:
    return KF::NONE; // TOP
  case K::LABEL:
    return KF::STATEMENT;

  // HINTS
  case K::DEBUG_BREAK:
    return KF::STATEMENT;
  case K::ABORT:
    return KF::STATEMENT;
  case K::ASSERT:
    return KF::STATEMENT;
  case K::UNREACHABLE:
    return KF::STATEMENT;
  case K::ASSUME:
    return KF::STATEMENT;

  // EXPRESSION ATTRIBUTES
  case K::NO_ANCHOR:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ANCHOR:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_OPAQUE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::OPAQUE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_FLANK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::FLANK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_GLOBAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::GLOBAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ACCESS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPORT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PUBLIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_PARTIAL_MUTATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PARTIAL_MUTATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_STATIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::STATIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_CAPTURE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::CAPTURE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT |
           KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::NO_INLINE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::INLINE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_MANGLE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::MANGLE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_PACK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PACK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_BRANCH_TREND:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LIKELY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::UNLIKELY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_SUPPORT_STATUS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::DEPRECIATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPERIMENTAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_STABLE_ADDRESS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::STABLE_ADDRESS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_VARIADIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::VARIADIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_LOCATION:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LOCATION:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_TEMPLATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::TEMPLATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_CONSTRAINT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::CONSTRAINT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_WEIGHT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::WEIGHT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_REQUIRE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::REQUIRE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ENSURE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ENSURE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_RANGER:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::RANGER:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;

  // TYPE ATTRIBUTES
  case K::NO_VAR:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::VAR:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PARTIAL_VAR:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_VOLATILE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::VOLATILE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ATOMIC:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ATOMIC:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_NULL_TERMINATE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NULL_TERMINATE:
    return KF::TYPE_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;

  // EXPRESSION ATTRIBUTE TYPES
  case K::ANCHOR_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::OPAQUE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FLANK_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::GLOBAL_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ACCESS_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PARTIAL_MUTATE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::STATIC_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CAPTURE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INLINE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::MANGLE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PACK_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BRANCH_TREND_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SUPPORT_STATUS_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::STABLE_ADDRESS_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::VARIADIC_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LOCATION_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::TEMPLATE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CONSTRAINT_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::WEIGHT_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::REQUIRE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ENSURE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::RANGER_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // TYPE ATTRIBUTE TYPES
  case K::VAR_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::VOLATILE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ATOMIC_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NULL_TERMINATE_ATTRIBUTE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // REFLECTIONS
  case K::REFLECT:
    return KF::CONVERGING | KF::STATEMENT | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER |
           KF::ARITHMETIC_SEQUENCE_STEP | KF::ARITHMETIC_SEQUENCE_CONDITION |
           KF::NAMESPACE;
  case K::MEMBER_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BAKE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BAKE_OF:
    return KF::RVALUE;
  case K::IGNORE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IGNORE_OF:
    return KF::STATEMENT;
  case K::BYTE_SIZE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BYTE_SIZE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BIT_DEPTH:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BIT_DEPTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ELEMENT_COUNT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ELEMENT_COUNT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SNIPPET:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SNIPPET_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NAME:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LINE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::LINE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COLUMN:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::COLUMN_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HOLDS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HOLDS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::UNIVERSALIZABLE;
  case K::TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SYMBOL:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SYMBOL_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HAS_MEMBER:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HAS_MEMBER_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HAS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HAS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::GET:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::GET_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SIGNATURE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SIGNATURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SYNONYM:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SYNONYM_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CAPTURE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::AS_EXTENSION:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::AS_EXTENSION_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INCREMENT:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::INCREMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DECREMENT:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DECREMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::WHILST:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::WHILST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ELEMENT:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ELEMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::AT:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::AT_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::DESTROY:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DESTROY_OF:
    return KF::STATEMENT;
  case K::FORWARD:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
    return KF::RVALUE | KF::ARGUMENT;
  case K::FORWARD_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BACKWARD:
    return KF::NAME | KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BACKWARD_OF:
  case K::IS_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_RANGE_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_RANGE_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_PLACEMENT_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_PLACEMENT_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_SIGNED_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_SIGNED_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_UNSIGNED_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_UNSIGNED_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_INTEGER_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_INTEGER_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_FLOAT_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_FLOAT_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_BINARY_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_BINARY_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_BFLOAT_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_BFLOAT_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_STRING_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_STRING_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_CODEUNIT_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_CODEUNIT_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS_TYPE_ATTRIBUTE_TYPE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_TYPE_ATTRIBUTE_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;

  case K::LAST:
    break;

    // NOTE: don't add default case so compiler will warn if cases are missing!
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterMarkKeyword(rq::Keyword keyword) {
  return keyword == rq::Keyword::NONPOSITIONAL_PARAMETERS_BEGIN ||
         keyword == rq::Keyword::POSITIONAL_PARAMETERS_END ||
         keyword == rq::Keyword::LOCKED_PARAMETERS_BEGIN;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralKeyword(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LITERAL);
}

[[nodiscard]] inline rq::Keyword
getSituatedAscribeKeyword(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_EXPRESSION:
    return rq::Keyword::ASCRIBE_EXPRESSION;
  case rq::Keyword::UNSITUATED_ASCRIBE_TYPE:
    return rq::Keyword::ASCRIBE_TYPE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_LEFT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_RIGHT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONVERGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericLiteral(rq::Keyword keyword) {
  return keyword == rq::Keyword::INTEGER_LITERAL ||
         keyword == rq::Keyword::FLOAT_LITERAL;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUniversalizable(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::UNIVERSALIZABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::STARTING_CHAINLINK |
                                   rq::KeywordFlags::CONTINUING_CHAINLINK |
                                   rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TYPE_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::EXPRESSION_ATTRIBUTE);
}

enum class Situation : std::uint_fast8_t {
  NONE,
  TOP,
  STATEMENT,
  LVALUE,
  RVALUE,
  REFLECTION,
  ARGUMENT,
  PARAMETER,
  BINDING,
  NAME,
  NAMESPACE,
  ASCRIPTION,
  EXPRESSION_ATTRIBUTE_INSTANTIATION,
  TYPE_ATTRIBUTE_INSTANTIATION,
  ARITHMETIC_SEQUENCE_STAGE
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
getDescription(rq::Situation situation) {
  using namespace rq;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    return "no expression";
  case S::TOP:
    return "top expression";
  case S::STATEMENT:
    return "statement";
  case S::LVALUE:
    return "lvalue expression";
  case S::RVALUE:
    return "rvalue expression";
  case S::REFLECTION:
    return "reflection expression";
  case S::ARGUMENT:
    return "argument expression";
  case S::PARAMETER:
    return "parameter expression";
  case S::BINDING:
    return "binding expression";
  case S::NAME:
    return "name expression";
  case S::NAMESPACE:
    return "namespace expression";
  case S::ASCRIPTION:
    return "ascription expression";
  case S::EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return "expression attribute instantiation";
  case S::TYPE_ATTRIBUTE_INSTANTIATION:
    return "type attribute instantiation";
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return "sequence stage expression";
  }
  return "error expression";
}

[[nodiscard]] inline rq::Keyword getUniversalized(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  // APPLY
  case K::IDENTIFY:
    return K::IDENTIFY_OF;
  // CASTS
  case K::AS:
    return K::AS_OF;
  case K::OF:
    return K::OF_OF;
  case K::CAST:
    return K::CAST_OF;
  case K::BITWISE_CAST:
    return K::BITWISE_CAST_OF;
  case K::PROCEDURE_CAST:
    return K::PROCEDURE_CAST_OF;
  // MEMORY
  case K::CONTENT:
    return K::CONTENT_OF;
  case K::ADDRESS:
    return K::ADDRESS_OF;
  case K::SLICE:
    return K::SLICE_OF;
  case K::PROCEDURE_ADDRESS:
    return K::PROCEDURE_ADDRESS_OF;
  case K::BORROW:
    return K::BORROW_OF;
  case K::DATA_ADDRESS:
    return K::DATA_ADDRESS_OF;
  case K::AT:
    return K::AT_OF;
  case K::MOVE:
    return K::MOVE_OF;
  case K::TAKE:
    return K::TAKE_OF;
  case K::EMPLACE:
    return K::EMPLACE_OF;
  case K::INVOKE:
    return K::INVOKE_OF;
  case K::COMPOSE:
    return K::COMPOSE_OF;
  case K::DECOMPOSE:
    return K::DECOMPOSE_OF;
  case K::ADAPT:
    return K::ADAPT_OF;
  case K::DESTROY:
    return K::DESTROY_OF;
  case K::DROP:
    return K::DROP_OF;
  case K::INPLACE_DESTROY:
    return K::INPLACE_DESTROY_OF;
  case K::INPLACE_INITIALIZE:
    return K::INPLACE_INITIALIZE_OF;
  // VARIADIC ARGUMENTS
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::NEXT_VARIADIC_ARGUMENT_OF;
  // PROCEDURES
  case K::INCREMENT:
    return K::INCREMENT_OF;
  case K::DECREMENT:
    return K::DECREMENT_OF;
  case K::WHILST:
    return K::WHILST_OF;
  case K::ELEMENT:
    return K::ELEMENT_OF;
  // REFLECTIONS
  case K::BAKE:
    return K::BAKE_OF;
  case K::IGNORE:
    return K::IGNORE_OF;
  case K::BYTE_SIZE:
    return K::BYTE_SIZE_OF;
  case K::BIT_DEPTH:
    return K::BIT_DEPTH_OF;
  case K::ELEMENT_COUNT:
    return K::ELEMENT_COUNT_OF;
  case K::SNIPPET:
    return K::SNIPPET_OF;
  case K::NAME:
    return K::NAME_OF;
  case K::LINE:
    return K::LINE_OF;
  case K::COLUMN:
    return K::COLUMN_OF;
  case K::IS:
    return K::IS_TYPE;
  case K::HOLDS:
    return K::HOLDS_OF;
  case K::TYPE:
    return K::TYPE_OF;
  case K::SYMBOL:
    return K::SYMBOL_OF;
  case K::HAS_MEMBER:
    return K::HAS_MEMBER_OF;
  case K::HAS:
    return K::HAS_OF;
  case K::GET:
    return K::GET_OF;
  case K::SIGNATURE:
    return K::SIGNATURE_OF;
  case K::SYNONYM:
    return K::SYNONYM_OF;
  case K::CAPTURE:
    return K::CAPTURE_OF;
  case K::AS_EXTENSION:
    return K::AS_EXTENSION_OF;
  case K::IS_TYPE:
    return K::IS_TYPE_OF;
  case K::IS_RANGE_TYPE:
    return K::IS_RANGE_TYPE_OF;
  case K::IS_PLACEMENT_TYPE:
    return K::IS_PLACEMENT_TYPE_OF;
  case K::IS_SIGNED_TYPE:
    return K::IS_SIGNED_TYPE_OF;
  case K::IS_UNSIGNED_TYPE:
    return K::IS_UNSIGNED_TYPE_OF;
  case K::IS_INTEGER_TYPE:
    return K::IS_INTEGER_TYPE_OF;
  case K::IS_FLOAT_TYPE:
    return K::IS_FLOAT_TYPE_OF;
  case K::IS_BINARY_TYPE:
    return K::IS_BINARY_TYPE_OF;
  case K::IS_BFLOAT_TYPE:
    return K::IS_BFLOAT_TYPE_OF;
  case K::IS_STRING_TYPE:
    return K::IS_STRING_TYPE_OF;
  case K::IS_CODEUNIT_TYPE:
    return K::IS_CODEUNIT_TYPE_OF;
  case K::IS_EXPRESSION_ATTRIBUTE_TYPE:
    return K::IS_EXPRESSION_ATTRIBUTE_TYPE_OF;
  case K::IS_TYPE_ATTRIBUTE_TYPE:
    return K::IS_TYPE_ATTRIBUTE_TYPE_OF;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation
getAttributeInstantiationSituation(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_EXPRESSION:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_EXPRESSION:
    return rq::Situation::EXPRESSION_ATTRIBUTE_INSTANTIATION;
  case rq::Keyword::UNSITUATED_ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_RECIEVER:
    return rq::Situation::TYPE_ATTRIBUTE_INSTANTIATION;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTop(rq::Keyword keyword) {
  return keyword == rq::Keyword::TOP;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLvalue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRvalue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeReflection(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::REFLECTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::BINDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeNamespace(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NAMESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeExpressionAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_EXPRESSION_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeTypeAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_TYPE_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceTypeStage(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION |
                                   rq::KeywordFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceCondition(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStep(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] inline bool getCanBeSituation(rq::Keyword keyword,
                                            rq::Situation situation) {
  switch (situation) {
  case rq::Situation::NONE:
    return rq::getIsNone(keyword);
  case rq::Situation::TOP:
    return rq::getCanBeTop(keyword);
  case rq::Situation::STATEMENT:
    return rq::getCanBeStatement(keyword);
  case rq::Situation::LVALUE:
    return rq::getCanBeLvalue(keyword);
  case rq::Situation::RVALUE:
    return rq::getCanBeRvalue(keyword);
  case rq::Situation::REFLECTION:
    return rq::getCanBeReflection(keyword);
  case rq::Situation::ARGUMENT:
    return rq::getCanBeArgument(keyword);
  case rq::Situation::PARAMETER:
    return rq::getCanBeParameter(keyword);
  case rq::Situation::BINDING:
    return rq::getCanBeBinding(keyword);
  case rq::Situation::NAME:
    return rq::getCanBeName(keyword);
  case rq::Situation::NAMESPACE:
    return rq::getCanBeNamespace(keyword);
  case rq::Situation::ASCRIPTION:
    return rq::getCanBeAscription(keyword);
  case rq::Situation::TYPE_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeTypeAttributeInstantiation(keyword);
  case rq::Situation::EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeExpressionAttributeInstantiation(keyword);
  case rq::Situation::ARITHMETIC_SEQUENCE_STAGE:
    return rq::getCanBeArithmeticSequenceTypeStage(keyword);
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName(rq::Keyword keyword) {
  return keyword == rq::Keyword::IDENTIFY_OF ||
         keyword == rq::Keyword::IDENTIFIER_LITERAL;
}

enum class ChainKind : std::uint_fast8_t { NONE, UNKNOWN, IF, ARM };

[[nodiscard]] inline llvm::StringRef getDescription(rq::ChainKind chainKind) {
  using namespace rq;
  using CK = ChainKind;
  switch (chainKind) {
  case CK::NONE:
    return "no chain";
  case CK::UNKNOWN:
    return "unknown chain";
  case CK::IF:
    return "if chain";
  case CK::ARM:
    return "arm chain";
  }
  return "error chain";
}

enum class ExpressionAttribute : std::uint_fast8_t {
  NONE,
  // anchor_attribute
  NO_ANCHOR,
  ANCHOR,
  // opaque_attribute
  NO_OPAQUE,
  OPAQUE,
  // global_attribute
  NO_GLOBAL,
  GLOBAL,
  // access_attribute
  NO_ACCESS,
  EXPORT,
  PUBLIC,
  // partial_mutate_attribute
  NO_PARTIAL_MUTATE,
  PARTIAL_MUTATE,
  // static_attribute
  NO_STATIC,
  STATIC,
  // capture_attribute
  NO_CAPTURE,
  CAPTURE,
  // inline_attribute
  NO_INLINE,
  INLINE,
  // mangle_attribute
  NO_MANGLE,
  MANGLE,
  // pack_attribute
  NO_PACK,
  PACK,
  // branch_trend_attribute
  NO_BRANCH_TREND,
  LIKELY,
  UNLIKELY,
  // support_status_attribute
  NO_SUPPORT_STATUS,
  DEPRECIATE,
  EXPERIMENTAL,
  // stable_address_attribute
  NO_STABLE_ADDRESS,
  STABLE_ADDRESS,
  // variadic_attribute
  NO_VARIADIC,
  VARIADIC,
  // location_attribute
  NO_LOCATION,
  LOCATION,
  // template_attribute
  NO_TEMPLATE,
  TEMPLATE,
  // constraint_attribute
  NO_CONSTRAINT,
  CONSTRAINT,
  // weight_attribute
  NO_WEIGHT,
  WEIGHT,
  // require_attribute
  NO_REQUIRE,
  REQUIRE,
  // ensure_attribute
  NO_ENSURE,
  ENSURE,
  // ranger_attribute
  NO_RANGER,
  RANGER,

  LAST
};

[[nodiscard]] inline llvm::StringRef
getName(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using EA = ExpressionAttribute;
  switch (attribute) {
  case EA::NONE:
    return "none";
  case EA::NO_ANCHOR:
    return "no_anchor";
  case EA::ANCHOR:
    return "anchor";
  case EA::NO_OPAQUE:
    return "no_opaque";
  case EA::OPAQUE:
    return "opaque";
  case EA::NO_GLOBAL:
    return "no_global";
  case EA::GLOBAL:
    return "global";
  case EA::NO_ACCESS:
    return "no_access";
  case EA::EXPORT:
    return "export";
  case EA::PUBLIC:
    return "public";
  case EA::NO_PARTIAL_MUTATE:
    return "no_partial_mutate";
  case EA::PARTIAL_MUTATE:
    return "partial_mutate";
  case EA::NO_STATIC:
    return "no_static";
  case EA::STATIC:
    return "static";
  case EA::NO_CAPTURE:
    return "no_capture";
  case EA::CAPTURE:
    return "capture";
  case EA::NO_INLINE:
    return "no_inline";
  case EA::INLINE:
    return "inline";
  case EA::NO_MANGLE:
    return "no_mangle";
  case EA::MANGLE:
    return "mangle";
  case EA::NO_PACK:
    return "no_pack";
  case EA::PACK:
    return "pack";
  case EA::NO_BRANCH_TREND:
    return "no_branch_trend";
  case EA::LIKELY:
    return "likely";
  case EA::UNLIKELY:
    return "unlikely";
  case EA::NO_SUPPORT_STATUS:
    return "no_support_status";
  case EA::DEPRECIATE:
    return "depreciate";
  case EA::EXPERIMENTAL:
    return "experimental";
  case EA::NO_STABLE_ADDRESS:
    return "no_stable_address";
  case EA::STABLE_ADDRESS:
    return "stable_address";
  case EA::NO_VARIADIC:
    return "no_variadic";
  case EA::VARIADIC:
    return "variadic";
  case EA::NO_LOCATION:
    return "no_location";
  case EA::LOCATION:
    return "location";
  case EA::NO_TEMPLATE:
    return "no_template";
  case EA::TEMPLATE:
    return "template";
  case EA::NO_CONSTRAINT:
    return "no_constraint";
  case EA::CONSTRAINT:
    return "constraint";
  case EA::NO_WEIGHT:
    return "no_weight";
  case EA::WEIGHT:
    return "weight";
  case EA::NO_REQUIRE:
    return "no_require";
  case EA::REQUIRE:
    return "require";
  case EA::NO_ENSURE:
    return "no_ensure";
  case EA::ENSURE:
    return "ensure";
  case EA::NO_RANGER:
    return "no_ranger";
  case EA::RANGER:
    return "ranger";
  case EA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionAttribute
getExpressionAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using EA = ExpressionAttribute;
  switch (keyword) {
  case K::NO_ANCHOR:
    return EA::NO_ANCHOR;
  case K::ANCHOR:
    return EA::ANCHOR;
  case K::NO_OPAQUE:
    return EA::OPAQUE;
  case K::NO_GLOBAL:
    return EA::NO_GLOBAL;
  case K::GLOBAL:
    return EA::GLOBAL;
  case K::NO_ACCESS:
    return EA::NO_ACCESS;
  case K::EXPORT:
    return EA::EXPORT;
  case K::PUBLIC:
    return EA::PUBLIC;
  case K::NO_PARTIAL_MUTATE:
    return EA::NO_PARTIAL_MUTATE;
  case K::PARTIAL_MUTATE:
    return EA::PARTIAL_MUTATE;
  case K::NO_STATIC:
    return EA::NO_STATIC;
  case K::STATIC:
    return EA::STATIC;
  case K::NO_CAPTURE:
    return EA::NO_CAPTURE;
  case K::CAPTURE:
    return EA::CAPTURE;
  case K::NO_INLINE:
    return EA::NO_INLINE;
  case K::INLINE:
    return EA::INLINE;
  case K::NO_MANGLE:
    return EA::NO_MANGLE;
  case K::MANGLE:
    return EA::MANGLE;
  case K::NO_PACK:
    return EA::NO_PACK;
  case K::PACK:
    return EA::PACK;
  case K::NO_BRANCH_TREND:
    return EA::NO_BRANCH_TREND;
  case K::LIKELY:
    return EA::LIKELY;
  case K::UNLIKELY:
    return EA::UNLIKELY;
  case K::NO_SUPPORT_STATUS:
    return EA::NO_SUPPORT_STATUS;
  case K::DEPRECIATE:
    return EA::DEPRECIATE;
  case K::EXPERIMENTAL:
    return EA::EXPERIMENTAL;
  case K::NO_STABLE_ADDRESS:
    return EA::NO_STABLE_ADDRESS;
  case K::STABLE_ADDRESS:
    return EA::STABLE_ADDRESS;
  case K::NO_VARIADIC:
    return EA::NO_VARIADIC;
  case K::VARIADIC:
    return EA::VARIADIC;
  case K::NO_LOCATION:
    return EA::NO_LOCATION;
  case K::LOCATION:
    return EA::LOCATION;
  case K::NO_TEMPLATE:
    return EA::NO_TEMPLATE;
  case K::TEMPLATE:
    return EA::TEMPLATE;
  case K::NO_CONSTRAINT:
    return EA::NO_CONSTRAINT;
  case K::CONSTRAINT:
    return EA::CONSTRAINT;
  case K::NO_WEIGHT:
    return EA::NO_WEIGHT;
  case K::WEIGHT:
    return EA::WEIGHT;
  default:
    break;
  }
  return EA::NONE;
}

enum class ExpressionFlags : std::uint_fast32_t {
  NONE = 0,

  ANCHOR = rq::getBit(0),
  ANCHOR_MASK = ANCHOR,

  OPAQUE = rq::getBit(1),
  OPAQUE_MASK = OPAQUE,

  GLOBAL = rq::getBit(2),
  GLOBAL_MASK = GLOBAL,

  EXPORT = rq::getBit(3),
  PUBLIC = rq::getBit(4),
  ACCESS_MASK = EXPORT | PUBLIC,

  PARTIAL_MUTATE = rq::getBit(5),
  PARTIAL_MUTATE_MASK = PARTIAL_MUTATE,

  STATIC = rq::getBit(6),
  STATIC_MASK = STATIC,

  CAPTURE = rq::getBit(7),
  CAPTURE_MASK = CAPTURE,

  INLINE = rq::getBit(8),
  INLINE_MASK = INLINE,

  MANGLE = rq::getBit(9),
  MANGLE_MASK = MANGLE,

  PACK = rq::getBit(10),
  PACK_MASK = PACK,

  LIKELY = rq::getBit(11),
  UNLIKELY = rq::getBit(12),
  BRANCH_TREND_MASK = LIKELY | UNLIKELY,

  DEPRECIATE = rq::getBit(13),
  EXPERIMENTAL = rq::getBit(14),
  DEPRECIATE_MASK = DEPRECIATE | EXPERIMENTAL,

  STABLE_ADDRESS = rq::getBit(15),
  STABLE_ADDRESS_MASK = STABLE_ADDRESS,

  VARIADIC = rq::getBit(16),
  VARIADIC_MASK = VARIADIC,

  LOCATION = rq::getBit(17),
  LOCATION_MASK = LOCATION,

  TEMPLATE = rq::getBit(18),
  TEMPLATE_MASK = TEMPLATE,

  CONSTRAINT = rq::getBit(19),
  CONSTRAINT_MASK = CONSTRAINT,

  WEIGHT = rq::getBit(20),
  WEIGHT_MASK = WEIGHT,

  REQUIRE = rq::getBit(21),
  REQUIRE_MASK = REQUIRE,

  ENSURE = rq::getBit(22),
  ENSURE_MASK = ENSURE,

  RANGER = rq::getBit(23),
  RANGER_MASK = RANGER
};

template <> struct is_flags<rq::ExpressionFlags> : std::true_type {};

[[nodiscard]] inline rq::ExpressionFlags
getFlags(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using EA = ExpressionAttribute;
  using EF = ExpressionFlags;
  switch (attribute) {
  case EA::NONE:
    return EF::NONE;
  case EA::NO_ANCHOR:
    return EF::NONE;
  case EA::ANCHOR:
    return EF::ANCHOR;
  case EA::NO_OPAQUE:
    return EF::NONE;
  case EA::OPAQUE:
    return EF::OPAQUE;
  case EA::NO_GLOBAL:
    return EF::NONE;
  case EA::GLOBAL:
    return EF::GLOBAL;
  case EA::NO_ACCESS:
    return EF::NONE;
  case EA::EXPORT:
    return EF::EXPORT;
  case EA::PUBLIC:
    return EF::PUBLIC;
  case EA::NO_PARTIAL_MUTATE:
    return EF::NONE;
  case EA::PARTIAL_MUTATE:
    return EF::PARTIAL_MUTATE;
  case EA::NO_STATIC:
    return EF::NONE;
  case EA::STATIC:
    return EF::STATIC;
  case EA::NO_CAPTURE:
    return EF::NONE;
  case EA::CAPTURE:
    return EF::CAPTURE;
  case EA::NO_INLINE:
    return EF::NONE;
  case EA::INLINE:
    return EF::INLINE;
  case EA::NO_MANGLE:
    return EF::NONE;
  case EA::MANGLE:
    return EF::MANGLE;
  case EA::NO_PACK:
    return EF::NONE;
  case EA::PACK:
    return EF::PACK;
  case EA::NO_BRANCH_TREND:
    return EF::NONE;
  case EA::LIKELY:
    return EF::LIKELY;
  case EA::UNLIKELY:
    return EF::UNLIKELY;
  case EA::NO_SUPPORT_STATUS:
    return EF::NONE;
  case EA::DEPRECIATE:
    return EF::DEPRECIATE;
  case EA::EXPERIMENTAL:
    return EF::EXPERIMENTAL;
  case EA::NO_STABLE_ADDRESS:
    return EF::NONE;
  case EA::STABLE_ADDRESS:
    return EF::STABLE_ADDRESS;
  case EA::NO_VARIADIC:
    return EF::NONE;
  case EA::VARIADIC:
    return EF::VARIADIC;
  case EA::NO_LOCATION:
    return EF::NONE;
  case EA::LOCATION:
    return EF::LOCATION;
  case EA::NO_TEMPLATE:
    return EF::NONE;
  case EA::TEMPLATE:
    return EF::TEMPLATE;
  case EA::NO_CONSTRAINT:
    return EF::NONE;
  case EA::CONSTRAINT:
    return EF::CONSTRAINT;
  case EA::NO_WEIGHT:
    return EF::NONE;
  case EA::WEIGHT:
    return EF::WEIGHT;
  case EA::NO_REQUIRE:
    return EF::NONE;
  case EA::REQUIRE:
    return EF::REQUIRE;
  case EA::NO_ENSURE:
    return EF::NONE;
  case EA::ENSURE:
    return EF::ENSURE;
  case EA::NO_RANGER:
    return EF::NONE;
  case EA::RANGER:
    return EF::RANGER;
  case EA::LAST:
    break;
  }
  return EF::NONE;
}

enum class ExpressionAttributeKind : std::uint_fast8_t {
  NONE,
  ANCHOR_ATTRIBUTE,         // no_anchor vs anchor
  OPAQUE_ATTRIBUTE,         // no_opaque vs opaque
  GLOBAL_ATTRIBUTE,         // no_global vs global
  ACCESS_ATTRIBUTE,         // no_access vs export vs public
  PARTIAL_MUTATE_ATTRIBUTE, // no_partial_mutate vs partial_mutate
  STATIC_ATTRIBUTE,         // no_static vs static
  CAPTURE_ATTRIBUTE,        // no_capture vs capture
  INLINE_ATTRIBUTE,         // no_inline vs inline
  MANGLE_ATTRIBUTE,         // no_mangle vs mangle
  PACK_ATTRIBUTE,           // no_pack vs pack
  BRANCH_TREND_ATTRIBUTE,   // no_branch_trend vs likely vs unlikely
  SUPPORT_STATUS_ATTRIBUTE, // no_support_status vs depreciate vs experimental
  STABLE_ADDRESS_ATTRIBUTE, // no_stable_address vs stable_address
  VARIADIC_ATTRIBUTE,       // no_variadic vs variadic
  LOCATION_ATTRIBUTE,       // no_location vs location
  TEMPLATE_ATTRIBUTE,       // no_template vs template
  CONSTRAINT_ATTRIBUTE,     // no_constraint vs constraint
  WEIGHT_ATTRIBUTE,         // no_weight vs weight
  REQUIRE_ATTRIBUTE,        // no_require vs require
  ENSURE_ATTRIBUTE,         // no_ensure vs ensure
  RANGER_ATTRIBUTE          // no_ranger vs ranger
};

[[nodiscard]] inline llvm::StringRef getName(rq::ExpressionAttributeKind kind) {
  using EAK = rq::ExpressionAttributeKind;
  switch (kind) {
  case EAK::NONE:
    break;
  case EAK::ANCHOR_ATTRIBUTE:
    return "anchor_attribute";
  case EAK::OPAQUE_ATTRIBUTE:
    return "opaque_attribute";
  case EAK::GLOBAL_ATTRIBUTE:
    return "global_attribute";
  case EAK::ACCESS_ATTRIBUTE:
    return "access_attribute";
  case EAK::PARTIAL_MUTATE_ATTRIBUTE:
    return "partial_mutate_attribute";
  case EAK::STATIC_ATTRIBUTE:
    return "static_attribute";
  case EAK::CAPTURE_ATTRIBUTE:
    return "capture_attribute";
  case EAK::INLINE_ATTRIBUTE:
    return "inline_attribute";
  case EAK::MANGLE_ATTRIBUTE:
    return "mangle_attribute";
  case EAK::PACK_ATTRIBUTE:
    return "pack_attribute";
  case EAK::BRANCH_TREND_ATTRIBUTE:
    return "branch_trend_attribute";
  case EAK::SUPPORT_STATUS_ATTRIBUTE:
    return "support_status_attribute";
  case EAK::STABLE_ADDRESS_ATTRIBUTE:
    return "stable_address_attribute";
  case EAK::VARIADIC_ATTRIBUTE:
    return "variadic_attribute";
  case EAK::LOCATION_ATTRIBUTE:
    return "location_attribute";
  case EAK::TEMPLATE_ATTRIBUTE:
    return "template_attribute";
  case EAK::CONSTRAINT_ATTRIBUTE:
    return "constraint_attribute";
  case EAK::WEIGHT_ATTRIBUTE:
    return "weight_attribute";
  case EAK::REQUIRE_ATTRIBUTE:
    return "require_attribute";
  case EAK::ENSURE_ATTRIBUTE:
    return "ensure_attribute";
  case EAK::RANGER_ATTRIBUTE:
    return "ranger_attribute";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionAttributeKind
getKind(rq::ExpressionAttribute attribute) {
  using EA = rq::ExpressionAttribute;
  using EAK = rq::ExpressionAttributeKind;
  switch (attribute) {
  case EA::NONE:
    return EAK::NONE;
  case EA::NO_ANCHOR:
    [[fallthrough]];
  case EA::ANCHOR:
    return EAK::ANCHOR_ATTRIBUTE;
  case EA::NO_OPAQUE:
    [[fallthrough]];
  case EA::OPAQUE:
    return EAK::OPAQUE_ATTRIBUTE;
  case EA::NO_GLOBAL:
    [[fallthrough]];
  case EA::GLOBAL:
    return EAK::GLOBAL_ATTRIBUTE;
  case EA::NO_ACCESS:
    [[fallthrough]];
  case EA::EXPORT:
    [[fallthrough]];
  case EA::PUBLIC:
    return EAK::ACCESS_ATTRIBUTE;
  case EA::NO_PARTIAL_MUTATE:
    [[fallthrough]];
  case EA::PARTIAL_MUTATE:
    return EAK::PARTIAL_MUTATE_ATTRIBUTE;
  case EA::NO_STATIC:
    [[fallthrough]];
  case EA::STATIC:
    return EAK::STATIC_ATTRIBUTE;
  case EA::NO_CAPTURE:
    [[fallthrough]];
  case EA::CAPTURE:
    return EAK::CAPTURE_ATTRIBUTE;
  case EA::NO_INLINE:
    [[fallthrough]];
  case EA::INLINE:
    return EAK::INLINE_ATTRIBUTE;
  case EA::NO_MANGLE:
    [[fallthrough]];
  case EA::MANGLE:
    return EAK::MANGLE_ATTRIBUTE;
  case EA::NO_PACK:
    [[fallthrough]];
  case EA::PACK:
    return EAK::PACK_ATTRIBUTE;
  case EA::NO_BRANCH_TREND:
    [[fallthrough]];
  case EA::LIKELY:
    [[fallthrough]];
  case EA::UNLIKELY:
    return EAK::BRANCH_TREND_ATTRIBUTE;
  case EA::NO_SUPPORT_STATUS:
    [[fallthrough]];
  case EA::DEPRECIATE:
    [[fallthrough]];
  case EA::EXPERIMENTAL:
    return EAK::SUPPORT_STATUS_ATTRIBUTE;
  case EA::NO_STABLE_ADDRESS:
    [[fallthrough]];
  case EA::STABLE_ADDRESS:
    return EAK::STABLE_ADDRESS_ATTRIBUTE;
  case EA::NO_VARIADIC:
    [[fallthrough]];
  case EA::VARIADIC:
    return EAK::VARIADIC_ATTRIBUTE;
  case EA::NO_LOCATION:
    [[fallthrough]];
  case EA::LOCATION:
    return EAK::LOCATION_ATTRIBUTE;
  case EA::NO_TEMPLATE:
    [[fallthrough]];
  case EA::TEMPLATE:
    return EAK::TEMPLATE_ATTRIBUTE;
  case EA::NO_CONSTRAINT:
    [[fallthrough]];
  case EA::CONSTRAINT:
    return EAK::CONSTRAINT_ATTRIBUTE;
  case EA::NO_WEIGHT:
    [[fallthrough]];
  case EA::WEIGHT:
    return EAK::WEIGHT_ATTRIBUTE;
  case EA::NO_REQUIRE:
    [[fallthrough]];
  case EA::REQUIRE:
    return EAK::REQUIRE_ATTRIBUTE;
  case EA::NO_ENSURE:
    [[fallthrough]];
  case EA::ENSURE:
    return EAK::ENSURE_ATTRIBUTE;
  case EA::NO_RANGER:
    [[fallthrough]];
  case EA::RANGER:
    return EAK::RANGER_ATTRIBUTE;
  case EA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

struct ExpressionFlagsFactory final {
  using Self = rq::ExpressionFlagsFactory;
  using ExpressionList = llvm::SmallVector<const rq::Expression *, 1>;
  using PtrMap =
      llvm::SmallDenseMap<rq::ExpressionAttributeKind, ExpressionList>;

  rq::ExpressionFlags _flags{};
  PtrMap _ptr_map{};

  ExpressionFlagsFactory() = default;
  ExpressionFlagsFactory(const Self &) = delete;
  ExpressionFlagsFactory(Self &&) = delete;
  ~ExpressionFlagsFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  inline void addFlag(rq::ExpressionAttribute attribute,
                      const rq::Expression *expression_ptr) {
    const rq::ExpressionFlags flag = rq::getFlags(attribute);
    this->_flags |= flag;
    const rq::ExpressionAttributeKind kind = rq::getKind(attribute);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      ExpressionList list;
      list.push_back(expression_ptr);
      this->_ptr_map.emplace_or_assign(kind, std::move(list));
    } else {
      it->getSecond().push_back(expression_ptr);
    }
  }
};

enum class TypeAttribute : std::uint_fast8_t {
  NONE,
  NO_VAR,
  VAR,
  PARTIAL_VAR,
  NO_VOLATILE,
  VOLATILE,
  NO_ATOMIC,
  ATOMIC,
  NO_NULL_TERMINATE,
  NULL_TERMINATE
};

[[nodiscard]] inline llvm::StringRef getName(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  switch (attribute) {
  case TA::NONE:
    return "none";
  case TA::NO_VAR:
    return "no_var";
  case TA::VAR:
    return "var";
  case TA::PARTIAL_VAR:
    return "partial_var";
  case TA::NO_VOLATILE:
    return "no_volatile";
  case TA::VOLATILE:
    return "volatile";
  case TA::NO_ATOMIC:
    return "no_atomic";
  case TA::ATOMIC:
    return "atomic";
  case TA::NO_NULL_TERMINATE:
    return "no_null_terminate";
  case TA::NULL_TERMINATE:
    return "null_terminate";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::TypeAttribute getTypeAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using TA = TypeAttribute;
  switch (keyword) {
  case K::NO_VAR:
    return TA::NO_VAR;
  case K::VAR:
    return TA::VAR;
  case K::PARTIAL_VAR:
    return TA::PARTIAL_VAR;
  case K::NO_VOLATILE:
    return TA::NO_VOLATILE;
  case K::VOLATILE:
    return TA::VOLATILE;
  case K::NO_ATOMIC:
    return TA::NO_ATOMIC;
  case K::ATOMIC:
    return TA::ATOMIC;
  case K::NO_NULL_TERMINATE:
    return TA::NO_NULL_TERMINATE;
  case K::NULL_TERMINATE:
    return TA::NULL_TERMINATE;
  default:
    break;
  }
  return TA::NONE;
}

enum class TypeFlags : std::uint_fast8_t {
  NONE = 0,

  VAR = rq::getBit(0),
  PARTIAL_VAR = rq::getBit(1),
  VAR_MASK = VAR | PARTIAL_VAR,

  VOLATILE = rq::getBit(2),
  VOLATILE_MASK = VOLATILE,

  ATOMIC = rq::getBit(3),
  ATOMIC_MASK = ATOMIC,

  NULL_TERMINATE = rq::getBit(4),
  NULL_TERMINATE_MASK = NULL_TERMINATE,
};

template <> struct is_flags<TypeFlags> : std::true_type {};

[[nodiscard]] inline rq::TypeFlags getFlags(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  using TF = TypeFlags;
  switch (attribute) {
  case TA::NONE:
    return TF::NONE;
  case TA::NO_VAR:
    return TF::NONE;
  case TA::VAR:
    return TF::VAR;
  case TA::PARTIAL_VAR:
    return TF::PARTIAL_VAR;
  case TA::NO_VOLATILE:
    return TF::NONE;
  case TA::VOLATILE:
    return TF::VOLATILE;
  case TA::NO_ATOMIC:
    return TF::NONE;
  case TA::ATOMIC:
    return TF::ATOMIC;
  case TA::NO_NULL_TERMINATE:
    return TF::NONE;
  case TA::NULL_TERMINATE:
    return TF::NULL_TERMINATE;
  }
  RQ_UNREACHABLE();
}

enum class TypeAttributeKind : std::uint_fast8_t {
  NONE,
  VAR_ATTRIBUTE,
  VOLATILE_ATTRIBUTE,
  ATOMIC_ATTRIBUTE,
  NULL_TERMINATE_ATTRIBUTE
};

[[nodiscard]] inline llvm::StringRef getName(rq::TypeAttributeKind kind) {
  using TAK = rq::TypeAttributeKind;
  switch (kind) {
  case TAK::NONE:
    return "none";
  case TAK::VAR_ATTRIBUTE:
    return "var_attribute";
  case TAK::VOLATILE_ATTRIBUTE:
    return "volatile_attribute";
  case TAK::ATOMIC_ATTRIBUTE:
    return "atomic_attribute";
  case TAK::NULL_TERMINATE_ATTRIBUTE:
    return "null_terminate_attribute";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::TypeAttributeKind
getKind(rq::TypeAttribute attribute) {
  using TA = rq::TypeAttribute;
  using TAK = rq::TypeAttributeKind;
  switch (attribute) {
  case TA::NONE:
    return TAK::NONE;
  case TA::NO_VAR:
    [[fallthrough]];
  case TA::VAR:
    [[fallthrough]];
  case TA::PARTIAL_VAR:
    return TAK::VAR_ATTRIBUTE;
  case TA::NO_VOLATILE:
    [[fallthrough]];
  case TA::VOLATILE:
    return TAK::VOLATILE_ATTRIBUTE;
  case TA::NO_ATOMIC:
    [[fallthrough]];
  case TA::ATOMIC:
    return TAK::ATOMIC_ATTRIBUTE;
  case TA::NO_NULL_TERMINATE:
    [[fallthrough]];
  case TA::NULL_TERMINATE:
    return TAK::NULL_TERMINATE_ATTRIBUTE;
  }
  RQ_UNREACHABLE();
}

struct TypeFlagsFactory final {
  using Self = rq::TypeFlagsFactory;
  using ExpressionList = llvm::SmallVector<const rq::Expression *, 1>;
  using PtrMap = llvm::SmallDenseMap<rq::TypeAttributeKind, ExpressionList>;

  rq::TypeFlags _flags{};
  PtrMap _ptr_map{};

  TypeFlagsFactory() = default;
  TypeFlagsFactory(const Self &) = delete;
  TypeFlagsFactory(Self &&) = delete;
  ~TypeFlagsFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  inline void addFlag(rq::TypeAttribute attribute,
                      const rq::Expression *expression_ptr) {
    const rq::TypeFlags flag = rq::getFlags(attribute);
    this->_flags |= flag;
    const rq::TypeAttributeKind kind = rq::getKind(attribute);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      ExpressionList list;
      list.push_back(expression_ptr);
      this->_ptr_map.emplace_or_assign(kind, std::move(list));
    } else {
      it->getSecond().push_back(expression_ptr);
    }
  }
};

enum class ArithmeticSequenceStep : std::uint_fast8_t {
  NONE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::ArithmeticSequenceStep step) {
  using namespace rq;
  using ASS = ArithmeticSequenceStep;
  switch (step) {
  case ASS::NONE:
    return "no step";
  case ASS::ADD:
    return "add step";
  case ASS::SUBTRACT:
    return "subtract step";
  case ASS::MULTIPLY:
    return "multiply step";
  case ASS::DIVIDE:
    return "divide step";
  case ASS::MODULUS:
    return "modulus step";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ArithmeticSequenceStep
getArithmeticSequenceStep(rq::Keyword keyword) {
  using namespace rq;
  using ASS = ArithmeticSequenceStep;
  using K = Keyword;
  switch (keyword) {
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return ASS::ADD;
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return ASS::SUBTRACT;
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return ASS::MULTIPLY;
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return ASS::DIVIDE;
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return ASS::MODULUS;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class ArithmeticSequenceCondition : std::uint_fast8_t {
  NONE,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  EQUAL,
  NOT_EQUAL
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::ArithmeticSequenceCondition condition) {
  using namespace rq;
  using ASC = ArithmeticSequenceCondition;
  switch (condition) {
  case ASC::NONE:
    return "no condition";
  case ASC::LESS:
    return "less condition";
  case ASC::LESS_EQUAL:
    return "less equal condition";
  case ASC::GREATER:
    return "greater condition";
  case ASC::GREATER_EQUAL:
    return "greater equal condition";
  case ASC::EQUAL:
    return "equal condition";
  case ASC::NOT_EQUAL:
    return "not equal condition";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ArithmeticSequenceCondition
getArithmeticSequenceCondition(rq::Keyword keyword) {
  using namespace rq;
  using ASC = ArithmeticSequenceCondition;
  using K = Keyword;
  switch (keyword) {
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return ASC::LESS;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return ASC::LESS_EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return ASC::GREATER;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return ASC::GREATER_EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return ASC::EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return ASC::NOT_EQUAL;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

template <typename SourceAParam, typename SourceBParam>
[[nodiscard]] inline unsigned getSourceLengthBetween(const SourceAParam &first,
                                                     const SourceBParam &last) {
  RQ_ASSERT(first.getSourceTextPtr() != nullptr, "first must have source");
  RQ_ASSERT(last.getSourceTextPtr() != nullptr, "last must have source");
  RQ_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr(),
            "last source must start after first source");
  return static_cast<unsigned>(last.getSourceTextPtr() -
                               first.getSourceTextPtr()) +
         last.getSourceTextLength();
}

struct Expression;
struct Expression;

enum class ExpressionNextFlags : std::uint8_t {
  NONE = 0,
  // NOTE: a "chain-link" expression has an expression after it, but no
  // seperator between Used in things like if->else_if->else chains. only occurs
  // for expressions with certain keywords.
  CHAINLINK = rq::getBit(0),
  // NOTE: a "header" expression is one that terminates with a semicolon
  STATEMENT = rq::getBit(1)
};

template <> struct is_flags<ExpressionNextFlags> : std::true_type {};

enum class ExpressionSourceFlags : std::uint8_t {
  NONE = 0,
  // NOTE: an "inserted" node is one that was not present in the source
  // and was inserted into the AST by the compiler
  INSERTED = rq::getBit(0),
  // NOTE: this flag is set if there is an error with situating this node
  SITUATOR_ERROR = rq::getBit(1)
};

template <> struct is_flags<ExpressionSourceFlags> : std::true_type {};

struct ExpressionIterator final {
  using Self = rq::ExpressionIterator;
  using value_type = rq::Expression;
  using reference = rq::Expression &;
  using pointer = rq::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Expression *_expression_ptr = nullptr;

  ExpressionIterator() = default;
  explicit ExpressionIterator(rq::Expression *expression_ptr)
      : _expression_ptr(expression_ptr) {}
  ExpressionIterator(const Self &) = default;
  ExpressionIterator(Self &&) = default;
  ~ExpressionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_expression_ptr == it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_expression_ptr != it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &operator*() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &operator*() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *operator->() {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *operator->() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_expression_ptr == nullptr;
  }
};

struct ConstExpressionIterator final {
  using Self = rq::ConstExpressionIterator;
  using value_type = const rq::Expression;
  using reference = const rq::Expression &;
  using pointer = rq::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Expression *_expression_ptr = nullptr;

  ConstExpressionIterator() = default;
  explicit ConstExpressionIterator(const rq::Expression *expression_ptr)
      : _expression_ptr(expression_ptr) {}
  ConstExpressionIterator(const Self &) = default;
  ConstExpressionIterator(Self &&) = default;
  ~ConstExpressionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_expression_ptr == it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_expression_ptr != it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &operator*() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *operator->() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_expression_ptr == nullptr;
  }
};

struct Expression final : public rq::Entity {
  using Self = rq::Expression;

  rq::PtrWithFlags<rq::Expression, 2, rq::ExpressionNextFlags>
      _next_ptr_flags{};
  rq::Expression *_branch_ptr{nullptr};
  rq::PtrWithFlags<const char, 2, rq::ExpressionSourceFlags>
      _source_ptr_flags{};
  unsigned _source_text_length{0};

  explicit Expression() : Entity(rq::getUnderlying(rq::Keyword::NONE)) {}
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getKeyword() const {
    return static_cast<rq::Keyword>(this->getId());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return rq::getName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterMark() const {
    return rq::getIsParameterMarkKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return rq::getIsLiteralKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft() const {
    return rq::getHasUnquotedLeft(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight() const {
    return rq::getHasUnquotedRight(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging() const {
    return rq::getIsConverging(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal() const {
    return rq::getIsInternal(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericLiteral() const {
    return rq::getIsNumericLiteral(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getSituatedAscribe() const {
    return rq::getSituatedAscribeKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Situation getAttributeSituation() const {
    return rq::getAttributeInstantiationSituation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink() const {
    return rq::getCanBeChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStartingChainLink() const {
    return rq::getCanBeStartingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeContinuingChainLink() const {
    return rq::getCanBeContinuingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeFinishingChainLink() const {
    return rq::getCanBeFinishingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute() const {
    return rq::getIsTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionAttribute() const {
    return rq::getIsExpressionAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getUniversalized() const {
    return rq::getUniversalized(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUniversalizable() const {
    return rq::getIsUniversalizable(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone() const {
    return rq::getIsNone(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTop() const {
    return rq::getCanBeTop(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement() const {
    return rq::getCanBeStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLvalue() const {
    return rq::getCanBeLvalue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRvalue() const {
    return rq::getCanBeRvalue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeReflection() const {
    return rq::getCanBeReflection(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument() const {
    return rq::getCanBeArgument(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter() const {
    return rq::getCanBeParameter(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding() const {
    return rq::getCanBeBinding(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName() const {
    return rq::getCanBeName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeNamespace() const {
    return rq::getCanBeNamespace(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription() const {
    return rq::getCanBeAscription(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeExpressionAttributeInstantiation() const {
    return rq::getCanBeExpressionAttributeInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeTypeAttributeInstantiation() const {
    return rq::getCanBeTypeAttributeInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeArithmeticSequenceTypeStage() const {
    return rq::getCanBeArithmeticSequenceTypeStage(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeArithmeticSequenceCondition() const {
    return rq::getCanBeArithmeticSequenceCondition(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticSequenceStep() const {
    return rq::getCanBeArithmeticSequenceStep(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeSituation(rq::Situation situation) const {
    return rq::getCanBeSituation(this->getKeyword(), situation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName() const {
    return rq::getIsEvaluatableName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute
  getExpressionAttribute() const {
    return rq::getExpressionAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getTypeAttribute() const {
    return rq::getTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInserted() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceFlags::INSERTED);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSituatorError() const {
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceFlags::SITUATOR_ERROR);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStatement() const {
    return rq::getHasAll(this->_next_ptr_flags.getFlags(),
                         rq::ExpressionNextFlags::STATEMENT);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_source_ptr_flags.getPtr() != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return llvm::StringRef(this->_source_ptr_flags.getPtr(),
                           this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getSourceTextPtr() const {
    return this->_source_ptr_flags.getPtr();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getSourceTextLength() const {
    return this->_source_text_length;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceBegin() const {
    return llvm::SMLoc::getFromPointer(this->_source_ptr_flags.getPtr());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceEnd() const {
    return llvm::SMLoc::getFromPointer(this->_source_ptr_flags.getPtr() +
                                       this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMRange getLlvmSourceRange() const {
    return llvm::SMRange(this->getLlvmSourceBegin(), this->getLlvmSourceEnd());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsChainLink() const {
    return rq::getHasAll(this->_next_ptr_flags.getFlags(),
                         rq::ExpressionNextFlags::CHAINLINK);
  }
  RQ_ALWAYS_INLINE const char *getBeforeSourceTextPtr() const {
    return this->getSourceTextPtr();
  }
  RQ_ALWAYS_INLINE const char *getAfterSourceTextPtr() const {
    return this->getSourceTextPtr() + this->getSourceTextLength();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getEndSourceTextPtr() const {
    if (this->getSourceTextLength() == 0) {
      return this->getSourceTextPtr();
    }
    return this->getSourceTextPtr() + this->getSourceTextLength() - 1;
  }
  RQ_ALWAYS_INLINE void clear() {
    this->_id = rq::getUnderlying(rq::Keyword::NONE);
    this->_next_ptr_flags = {};
    this->_branch_ptr = nullptr;
    this->_source_ptr_flags = {};
    this->_source_text_length = 0;
  }
  RQ_ALWAYS_INLINE void setKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_id == rq::getUnderlying(rq::Keyword::NONE),
              "keyword must not already be set");
    this->_id = rq::getUnderlying(keyword);
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_id != rq::getUnderlying(rq::Keyword::NONE),
              "keyword must already be set");
    this->_id = rq::getUnderlying(keyword);
  }
  RQ_ALWAYS_INLINE void setIsInserted() {
    this->_source_ptr_flags.addFlags(rq::ExpressionSourceFlags::INSERTED);
  }
  RQ_ALWAYS_INLINE void setHasSituatorError() {
    this->_source_ptr_flags.addFlags(rq::ExpressionSourceFlags::SITUATOR_ERROR);
  }
  void RQ_ALWAYS_INLINE setIsStatement() {
    this->_next_ptr_flags.addFlags(rq::ExpressionNextFlags::STATEMENT);
  }
  void RQ_ALWAYS_INLINE setIsChainLink() {
    this->_next_ptr_flags.addFlags(rq::ExpressionNextFlags::CHAINLINK);
  }
  RQ_ALWAYS_INLINE void setSource(llvm::StringRef source) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    RQ_ASSERT(!source.empty(), "source text is empty");
    this->_source_ptr_flags.setPtr(source.data());
    this->_source_text_length = static_cast<unsigned>(source.size());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSource(const SourceParam &source) {
    this->setSource(source.getSourceText());
  }
  template <typename FirstSourceParam, typename LastSourceParam>
  inline void setSource(const FirstSourceParam &first,
                        const LastSourceParam &last) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    rq::assignSingleValue(this->_source_ptr_flags.getPtr(),
                          first.getSourceTextPtr());
    this->_source_text_length = rq::getSourceLengthBetween(first, last);
  }
  template <typename SourceParam>
  inline void extendSourceOver(const SourceParam &source) {
    RQ_ASSERT(this->getHasSourceText(), "expression source not set");
    if (source.getSourceTextPtr() >= this->_source_ptr_flags.getPtr()) {
      this->_source_text_length = rq::getSourceLengthBetween(*this, source);
    } else {
      this->_source_ptr_flags.setPtr(source.getSourceTextPtr());
      this->_source_text_length = rq::getSourceLengthBetween(source, *this);
    }
  }
  RQ_ALWAYS_INLINE void setSourceAt(const char *source_ptr) {
    rq::assignSingleValue(this->_source_ptr_flags.getPtr(), source_ptr);
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceBefore(const SourceParam &source) {
    this->setSourceAt(source.getBeforeSourceTextPtr());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceAfter(const SourceParam &source) {
    this->setSourceAt(source.getAfterSourceTextPtr());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceAtEnd(const SourceParam &source) {
    this->setSourceAt(source.getEndSourceTextPtr());
  }
  // NOTE: no getBranchCount and getNextCount because bad performance!
  RQ_ALWAYS_INLINE rq::Expression *getBranchPtr() {
    return static_cast<rq::Expression *>(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE const rq::Expression *getBranchPtr() const {
    return static_cast<rq::Expression *>(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE rq::Expression *getNextPtr() {
    return static_cast<rq::Expression *>(this->_next_ptr_flags.getPtr());
  }
  RQ_ALWAYS_INLINE const rq::Expression *getNextPtr() const {
    return static_cast<rq::Expression *>(this->_next_ptr_flags.getPtr());
  }
  RQ_ALWAYS_INLINE rq::Expression &getBranch() {
    return rq::dereferencePtr(this->getBranchPtr());
  }
  RQ_ALWAYS_INLINE const rq::Expression &getBranch() const {
    return rq::dereferencePtr(this->getBranchPtr());
  }
  RQ_ALWAYS_INLINE rq::Expression &getNext() {
    return rq::dereferencePtr(this->getNextPtr());
  }
  RQ_ALWAYS_INLINE const rq::Expression &getNext() const {
    return rq::dereferencePtr(this->getNextPtr());
  }
  // TODO: get rid of the getLastNext and getLastBranch functions to avoid
  // excessive pointer chasing
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastNext() {
    rq::Expression *expression_ptr = this;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastNext() const {
    const rq::Expression *expression_ptr = this;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastBranch() {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    rq::Expression *expression_ptr = this->getBranchPtr();
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastBranch() const {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    const rq::Expression *expression_ptr = this->getBranchPtr();
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression &branch) {
    rq::assignSingleValue(this->_branch_ptr, &branch);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression &next) {
    rq::assignSingleValue(this->_next_ptr_flags.getPtr(), &next);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression *branch_ptr) {
    rq::assignSingleValue(this->_branch_ptr, branch_ptr);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression *next_ptr) {
    rq::assignSingleValue(this->_next_ptr_flags.getPtr(), next_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceBranch(rq::Expression &branch) {
    return rq::replaceValue(this->_branch_ptr, &branch);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceNext(rq::Expression &next) {
    rq::Expression &replaced_next = this->getNext();
    this->_next_ptr_flags.setPtr(&next);
    return replaced_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceBranch(rq::Expression *branch_ptr) {
    return rq::replaceValue(this->_branch_ptr, branch_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceNext(rq::Expression *next_ptr) {
    rq::Expression &replaced_next = this->getNext();
    this->_next_ptr_flags.setPtr(next_ptr);
    return replaced_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceBranchPtr(rq::Expression &branch) {
    return rq::replaceValuePtr(this->_branch_ptr, &branch);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceNextPtr(rq::Expression &next) {
    rq::Expression *replaced_next_ptr = this->getNextPtr();
    this->_next_ptr_flags.setPtr(&next);
    return replaced_next_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceBranchPtr(rq::Expression *branch_ptr) {
    return rq::replaceValuePtr(this->_branch_ptr, branch_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceNextPtr(rq::Expression *next_ptr) {
    rq::Expression *replaced_next_ptr = this->getNextPtr();
    this->_next_ptr_flags.setPtr(next_ptr);
    return replaced_next_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBranch() const {
    return this->_branch_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNext() const {
    return this->_next_ptr_flags.getPtr() != nullptr;
  }
  [[nodiscard]] inline rq::Expression &popBranch() {
    return rq::popValue(this->_branch_ptr);
  }
  [[nodiscard]] inline rq::Expression *popBranchPtr() {
    return rq::popValuePtr(this->_branch_ptr);
  }
  [[nodiscard]] inline rq::Expression &popNext() {
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->getNextPtr();
    this->_next_ptr_flags.setPtr(nullptr);
    return rq::dereferencePtr(old_next_ptr);
  }
  [[nodiscard]] inline rq::Expression *popNextPtr() {
    rq::Expression *old_next_ptr = this->getNextPtr();
    this->_next_ptr_flags.setPtr(nullptr);
    return old_next_ptr;
  }
  [[nodiscard]] inline rq::Expression &mergeAndPopBranch() {
    rq::Expression &branch = this->popBranch();
    if (this->getHasNext()) {
      rq::Expression &branch_last_next = branch.getLastNext();
      branch_last_next.setNext(this->popNext());
    }
    this->clear();
    this->setKeyword(branch.getKeyword());
    if (branch.getHasBranch()) {
      this->setBranch(branch.popBranch());
    }
    if (branch.getHasNext()) {
      this->setNext(branch.popNext());
    }
    this->setSource(branch);
    branch.clear();
    return branch;
  }
  [[nodiscard]] inline rq::Expression &mergeAndPopNext() {
    rq::Expression &next = this->popNext();
    RQ_ASSERT(!this->getHasBranch(), "has branch");
    this->clear();
    this->setKeyword(next.getKeyword());
    if (next.getHasBranch()) {
      this->setBranch(next.popBranch());
    }
    if (next.getHasNext()) {
      this->setNext(next.popNext());
    }
    this->setSource(next);
    next.clear();
    return next;
  }
  [[nodiscard]] inline rq::Expression &getUnascribed() {
    if (this->getCanBeAscription()) {
      return this->getBranch().getUnascribed();
    }
    return *this;
  }
  [[nodiscard]] inline const rq::Expression &getUnascribed() const {
    if (this->getCanBeAscription()) {
      return this->getBranch().getUnascribed();
    }
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getInclusiveNextSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getInclusiveNextSubrange() const {
    return std::ranges::subrange(rq::ConstExpressionIterator(this),
                                 rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getNextSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this->getNextPtr()),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getNextSubrange() const {
    return std::ranges::subrange(
        rq::ConstExpressionIterator(this->getNextPtr()),
        rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getBranchSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this->getBranchPtr()),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getBranchSubrange() const {
    return std::ranges::subrange(
        rq::ConstExpressionIterator(this->getBranchPtr()),
        rq::ConstExpressionIterator());
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::KEYWORD_OFFSET || id < rq::SYMBOL_OFFSET;
  }
};

rq::ExpressionIterator &ExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
}

rq::ExpressionIterator ExpressionIterator::operator++(int) { return ++*this; }

rq::ConstExpressionIterator &ConstExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
}

rq::ConstExpressionIterator ConstExpressionIterator::operator++(int) {
  return ++*this;
}

} // namespace rq