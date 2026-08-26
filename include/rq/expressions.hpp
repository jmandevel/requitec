#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/entity.hpp>
#include <rq/iterators.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
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
    return "_none";

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
    return "_error";

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return "_unsituated_parenthesis_group";
  case K::UNSITUATED_EQUAL_OPERATOR:
    return "_unsituated_equal_operator";
  case K::UNSITUATED_ASCRIBE_LOW:
    return "_unsituated_ascribe_low";
  case K::UNSITUATED_ASCRIBE_HIGH:
    return "_unsituated_ascribe_high";
  case K::UNSITUATED_CHAIN:
    return "_unsituated_chain";
  case K::UNSITUATED_TRAIN:
    return "_unsituated_train";

  // LOGICAL
  case K::LOGICAL_AND:
    return "_logical_and";
  case K::LOGICAL_OR:
    return "_logical_or";
  case K::LOGICAL_COMPLEMENT:
    return "_logical_complement";
  case K::LOGICAL_AND_WITH_SHORTCIRCUIT:
    return "_logical_and_with_shortcircuit";
  case K::LOGICAL_OR_WITH_SHORTCIRCUIT:
    return "_logical_or_with_shortcircuit";

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
  case K::INSTANTIATE_CONFORMITY:
    return "_instantiate_conformity";
  case K::INSTANTIATE_ADAPTION:
    return "_instantiate_adaption";
  case K::BINDING:
    return "_binding";
  case K::UPBINDING:
    return "_upbinding";
  case K::ASCRIBE_HIGH:
    return "_ascribe_high";
  case K::ASCRIBE_LOW:
    return "_ascribe_low";
  case K::ASCRIBE_HIGH_RECIEVER:
    return "_ascribe_high_reciever";
  case K::INSTANTIATE_LOW_ATTRIBUTE:
    return "_instantiate_low_attribute";
  case K::INSTANTIATE_HIGH_ATTRIBUTE:
    return "instantiate_high_attribute";
  case K::IDENTIFY:
    return "identify";
  case K::IDENTIFY_OF:
    return "_identify_of";

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
  case K::SIGNATURE_CAST:
    return "signature_cast";
  case K::SIGNATURE_CAST_OF:
    return "_signature_cast_of";

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
  case K::FUNCTION_ADDRESS:
    return "function_address";
  case K::FUNCTION_ADDRESS_OF:
    return "_function_address_of";
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
  case K::REMOVE:
    return "remove";
  case K::REMOVE_OF:
    return "_remove_of";
  case K::INPLACE_DESTROY:
    return "inplace_destroy";
  case K::INPLACE_DESTROY_OF:
    return "_inplace_destroy_of";
  case K::INIT:
    return "init";
  case K::INIT_OF:
    return "_init_of";
  case K::INPLACE_INIT:
    return "inplace_init";
  case K::INPLACE_INIT_OF:
    return "_inplace_init_of";

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
  case K::NAMED_ELEMENT:
    return "_named_element";
  case K::INSTANTIATE_LAYOUT:
    return "_instantiate_layout";
  case K::SPECIALIZE:
    return "_specialize";

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
  case K::FUNCTION:
    return "function";
  case K::IMPLEMENT_FUNCTION:
    return "implement_function";
  case K::CONSTRUCTOR:
    return "constructor";
  case K::LAYOUT_CONSTRUCTOR:
    return "layout_constructor";

  // CONTROL FLOW
  case K::RETURN:
    return "return";
  case K::BREAK:
    return "break";
  case K::BREAK_OF:
    return "_break_of";
  case K::CONTINUE:
    return "continue";
  case K::CONTINUE_OF:
    return "_continue_of";

  // DECLARED TYPES
  case K::CLASS:
    return "class";
  case K::ENUM:
    return "enum";
  case K::INTERFACE:
    return "interface";
  case K::ADAPTER:
    return "adapter";

  // VALUES
  case K::ARRAY:
    return "array";
  case K::TRUE:
    return "true";
  case K::FALSE:
    return "false";
  case K::VALUE:
    return "value";
  case K::INDEX:
    return "index";
  case K::THIS:
    return "this";
  case K::RESULT:
    return "result";
  case K::CALLSITE:
    return "callsite";

  // BUILTIN TYPES
  case K::SYMBOL:
    return "symbol";
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
  case K::IF_CHAIN:
    return "_if_chain";
  case K::SWITCH_CHAIN:
    return "_switch_chain";
  case K::SPIN_CHAIN:
    return "_spin_chain";
  case K::IF:
    return "if";
  case K::ELSE_IF:
    return "else_if";
  case K::ELSE:
    return "else";
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
  case K::FOLD:
    return "fold";

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
  case K::C:
    return "c";
  case K::TOP:
    return "top";

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

  // LOW ATTRIBUTES
  case K::NO_ANCHOR:
    return "no_anchor";
  case K::ANCHOR:
    return "anchor";
  case K::NO_FLANK:
    return "no_flank";
  case K::FLANK:
    return "flank";
  case K::NO_OPAQUE:
    return "no_opaque";
  case K::OPAQUE:
    return "opaque";
  case K::NO_GLOBAL:
    return "no_global";
  case K::GLOBAL:
    return "global";
  case K::NO_PUBLIC:
    return "no_public";
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
  case K::NO_DELAY:
    return "no_delay";
  case K::DELAY:
    return "delay";
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
  case K::DEPRECIATED:
    return "depreciated";
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
  case K::NO_AUTO:
    return "no_auto";
  case K::AUTO:
    return "auto";
  case K::NO_VIRTUAL:
    return "no_virtual";
  case K::VIRTUAL:
    return "virtual";
  case K::NO_RANGER:
    return "no_ranger";
  case K::RANGER:
    return "ranger";
  case K::NO_REQUIRE:
    return "no_require";
  case K::REQUIRE:
    return "require";
  case K::NO_ENSURE:
    return "no_ensure";
  case K::ENSURE:
    return "ensure";

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
  case K::NO_GREATEST:
    return "no_greatest";
  case K::GREATEST:
    return "greatest";

  // LOW ATTRIBUTE TYPES
  case K::ANCHOR_ATTRIBUTE:
    return "anchor_attribute";
  case K::OPAQUE_ATTRIBUTE:
    return "opaque_attribute";
  case K::FLANK_ATTRIBUTE:
    return "flank_attribute";
  case K::GLOBAL_ATTRIBUTE:
    return "global_attribute";
  case K::PUBLIC_ATTRIBUTE:
    return "public_attribute";
  case K::PARTIAL_MUTATE_ATTRIBUTE:
    return "partial_mutate_attribute";
  case K::STATIC_ATTRIBUTE:
    return "static_attribute";
  case K::DELAY_ATTRIBUTE:
    return "delay_attribute";
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
  case K::AUTO_ATTRIBUTE:
    return "auto_attribute";
  case K::VIRTUAL_ATTRIBUTE:
    return "virtual_attribute";
  case K::RANGER_ATTRIBUTE:
    return "ranger_attribute";
  case K::REQUIRE_ATTRIBUTE:
    return "require_attribute";
  case K::ENSURE_ATTRIBUTE:
    return "ensure_attribute";

  // HIGH ATTRIBUTE TYPES
  case K::VAR_ATTRIBUTE:
    return "var_attribute";
  case K::VOLATILE_ATTRIBUTE:
    return "volatile_attribute";
  case K::ATOMIC_ATTRIBUTE:
    return "atomic_attribute";
  case K::NULL_TERMINATE_ATTRIBUTE:
    return "null_terminate_attribute";
  case K::GREATEST_ATTRIBUTE:
    return "greatest_attribute";

  // REFLECTIONS
  case K::MEMBER_OF:
    return "_member_of";
  case K::BAKE:
    return "bake";
  case K::BAKE_OF:
    return "_bake_of";
  case K::IGNORE:
    return "ignore";
  case K::IGNORE_OF:
    return "_ignore_of";
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
  case K::MAIN:
    return "main";
  case K::MAIN_OF:
    return "main_of";
  case K::DESTROY:
    return "destroy";
  case K::DESTROY_OF:
    return "_destroy_of";
  case K::FORWARD:
    return "forward";
  case K::FORWARD_OF:
    return "_forward_of";
  case K::BACKWARD:
    return "backward";
  case K::BACKWARD_OF:
    return "_backward_of";
  case K::UNDERLYING_VALUE:
    return "underlying_value";
  case K::UNDERLYING_VALUE_OF:
    return "_underlying_value_of";
  case K::UNDERLYING_TYPE:
    return "underlying_type";
  case K::UNDERLYING_TYPE_OF:
    return "_underlying_type_of";
  case K::REFLECT:
    return "reflect";
  case K::REFLECT_OF:
    return "_reflect_of";
  case K::POLYMORPH:
    return "polymorph";
  case K::POLYMORPH_OF:
    return "_polymorph_of";
  case K::OVERLOAD:
    return "overload";
  case K::OVERLOAD_OF:
    return "_overload_of";
  case K::OVERLOAD_RANGE:
    return "overload_range";
  case K::OVERLOAD_RANGE_OF:
    return "_overload_range_of";
  case K::SPECIALIZATION_RANGE:
    return "specialization_range";
  case K::SPECIALIZATION_RANGE_OF:
    return "_specialization_range_of";
  case K::WEIGHT_LEVEL:
    return "weight_level";
  case K::WEIGHT_LEVEL_OF:
    return "_weight_level_of";
  case K::WEIGHT_LEVEL_RANGE:
    return "weight_level_range";
  case K::WEIGHT_LEVEL_RANGE_OF:
    return "_weight_level_range_of";
  case K::WEIGHT_OF:
    return "_weight_of";
  case K::TEMPLATE_OF:
    return "_template_of";
  case K::TEMPLATE_RANGE:
    return "template_range";
  case K::TEMPLATE_RANGE_OF:
    return "_template_range_of";
  case K::CONSTRUCTOR_RANGE:
    return "constructor_range";
  case K::CONSTRUCTOR_RANGE_OF:
    return "_constructor_range_of";
  case K::RESOLVE_TEMPLATE:
    return "resolve_template";
  case K::RESOLVE_TEMPLATE_OF:
    return "_resolve_template_of";
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
    return "is_string_type";
  case K::IS_STRING_TYPE_OF:
    return "_is_string_type_of";
  case K::IS_CODEUNIT_TYPE:
    return "is_codeunit_type";
  case K::IS_CODEUNIT_TYPE_OF:
    return "_is_codeunit_type_of";
  case K::IS_LOW_ATTRIBUTE_TYPE:
    return "is_low_attribute_type";
  case K::IS_LOW_ATTRIBUTE_TYPE_OF:
    return "_is_low_attribute_type_of";
  case K::IS_HIGH_ATTRIBUTE_TYPE:
    return "is_high_attribute_type";
  case K::IS_HIGH_ATTRIBUTE_TYPE_OF:
    return "_is_high_attribute_type_of";

  case K::LAST:
    break;

    // NOTE: don't add default case so compiler will warn if cases are missing!
  }
  RQ_UNREACHABLE();
}

enum class KeywordInfoFlags : std::uint32_t {
  NONE = 0,
  CONVERGING = rq::getBit(0),
  LITERAL = rq::getBit(1),
  UNQUOTED_LEFT = rq::getBit(2),
  UNQUOTED_RIGHT = rq::getBit(3),
  INTERNAL = rq::getBit(4),
  STARTING_CHAINLINK = rq::getBit(5),
  CONTINUING_CHAINLINK = rq::getBit(6),
  FINISHING_CHAINLINK = rq::getBit(7),
  IF_CHAINLINK = rq::getBit(8),
  SWITCH_CHAINLINK = rq::getBit(9),
  SPIN_CHAINLINK = rq::getBit(10),
  // TOP
  STATEMENT = rq::getBit(11),
  RVALUE = rq::getBit(12),
  LVALUE = rq::getBit(13),
  RAILCAR = rq::getBit(14),
  ARGUMENT = rq::getBit(15),
  PARAMETER = rq::getBit(16),
  TUPLE_ELEMENT = rq::getBit(17),
  BINDING = rq::getBit(18),
  NAME = rq::getBit(19),
  NAMESPACE = rq::getBit(20),
  ASCRIPTION = rq::getBit(21),
  LOW_ATTRIBUTE = rq::getBit(22),
  HIGH_ATTRIBUTE = rq::getBit(23),
  ARITHMETIC_SEQUENCE_STEP = rq::getBit(24),
  ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(25),
  ALL_SITUATIONS = STATEMENT | RVALUE | LVALUE | RAILCAR | ARGUMENT |
                   PARAMETER | BINDING | NAME | NAMESPACE | ASCRIPTION |
                   HIGH_ATTRIBUTE | LOW_ATTRIBUTE | ARITHMETIC_SEQUENCE_STEP |
                   ARITHMETIC_SEQUENCE_CONDITION,

};

RQ_DEFINE_FLAGS(rq::KeywordInfoFlags);

[[nodiscard]] inline rq::KeywordInfoFlags getInfoFlags(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using KIF = KeywordInfoFlags;
  switch (keyword) {
  case K::NONE:
    return KIF::NONE;

  // LITERALS
  case K::INTEGER_LITERAL:
    return KIF::LITERAL | KIF::INTERNAL | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::FLOAT_LITERAL:
    return KIF::LITERAL | KIF::INTERNAL | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::STRING_LITERAL:
    return KIF::LITERAL | KIF::INTERNAL | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::CODEUNIT_LITERAL:
    return KIF::LITERAL | KIF::INTERNAL | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::IDENTIFIER_LITERAL:
    return KIF::LITERAL | KIF::INTERNAL | KIF::RVALUE | KIF::LVALUE |
           KIF::RAILCAR | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::NAME |
           KIF::NAMESPACE;

  // ERRORS
  case K::ERROR:
    return KIF::INTERNAL;

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    // NOTE: not allowed in STATEMENT and PARAMETER situations so that low
    // attributes do not get confused with _call (and also this would be weird).
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::LVALUE | KIF::NAME | KIF::NAMESPACE |
           KIF::ARITHMETIC_SEQUENCE_STEP | KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::UNSITUATED_EQUAL_OPERATOR:
    return KIF::STATEMENT | KIF::ARGUMENT | KIF::PARAMETER | KIF::TUPLE_ELEMENT;
  case K::UNSITUATED_ASCRIBE_LOW:
    return KIF::STATEMENT | KIF::RVALUE | KIF::PARAMETER | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT | KIF::ASCRIPTION;
  case K::UNSITUATED_ASCRIBE_HIGH:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::RAILCAR |
           KIF::ASCRIPTION;
  case K::UNSITUATED_CHAIN:
    return KIF::STATEMENT;
  case K::UNSITUATED_TRAIN:
    return KIF::CONVERGING | KIF::STATEMENT | KIF::RVALUE | KIF::LVALUE |
           KIF::RAILCAR | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::ARITHMETIC_SEQUENCE_STEP | KIF::ARITHMETIC_SEQUENCE_CONDITION |
           KIF::NAMESPACE;

  // LOGICAL
  case K::LOGICAL_AND:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOGICAL_OR:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOGICAL_COMPLEMENT:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOGICAL_AND_WITH_SHORTCIRCUIT:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOGICAL_OR_WITH_SHORTCIRCUIT:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // COMPARISON
  case K::GREATER:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::GREATER_EQUAL:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LESS:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LESS_EQUAL:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::EQUAL:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NOT_EQUAL:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // APPLY
  case K::EXTEND:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_EXTENSION:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_CONFORMITY:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_ADAPTION:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BINDING:
    return KIF::LVALUE | KIF::PARAMETER | KIF::ARGUMENT | KIF::BINDING |
           KIF::TUPLE_ELEMENT;
  case K::UPBINDING:
    return KIF::LVALUE;
  case K::ASCRIBE_HIGH:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::RAILCAR |
           KIF::ASCRIPTION;
  case K::ASCRIBE_LOW:
    return KIF::STATEMENT | KIF::PARAMETER;
  case K::ASCRIBE_HIGH_RECIEVER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::ASCRIPTION;
  case K::INSTANTIATE_LOW_ATTRIBUTE:
    return KIF::NONE; // LOW_ATTRIBUTE_INSTANTIATION
  case K::INSTANTIATE_HIGH_ATTRIBUTE:
    return KIF::NONE; // HIGH_ATTRIBUTE_INSTANTIATION
  case K::IDENTIFY:
    return KIF::RAILCAR | KIF::ASCRIPTION;
  case K::IDENTIFY_OF:
    return KIF::NAME | KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT | KIF::NAMESPACE;

  // JUXTAPOSITIONAL
  case K::CONCATENATE:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::APPEND:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // ARITHMETIC
  case K::ADD:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SUBTRACT:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MULTIPLY:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DIVIDE:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MODULUS:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NEGATE:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // CASTS
  case K::AS:
    return KIF::RAILCAR;
  case K::AS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::OF:
    return KIF::RAILCAR;
  case K::OF_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CAST:
    return KIF::RAILCAR;
  case K::CAST_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_CAST:
    return KIF::RAILCAR;
  case K::BITWISE_CAST_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SIGNATURE_CAST:
    return KIF::RAILCAR;
  case K::SIGNATURE_CAST_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // BITWISE
  case K::BITWISE_OR:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_AND:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_XOR:
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_COMPLEMENT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_SHIFT_LEFT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_SHIFT_RIGHT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // MEMORY
  case K::ASSIGN:
    return KIF::STATEMENT;
  case K::CONTENT:
    return KIF::RAILCAR;
  case K::CONTENT_OF:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ADDRESS:
    return KIF::RAILCAR;
  case K::ADDRESS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SLICE:
    return KIF::RAILCAR;
  case K::SLICE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FUNCTION_ADDRESS:
    return KIF::RAILCAR;
  case K::FUNCTION_ADDRESS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BORROW:
    return KIF::RAILCAR;
  case K::BORROW_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DATA_ADDRESS:
    return KIF::RAILCAR;
  case K::DATA_ADDRESS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MOVE:
    return KIF::RAILCAR;
  case K::MOVE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TAKE:
    return KIF::RAILCAR;
  case K::TAKE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CALL:
    return KIF::STATEMENT | KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::EMPLACE:
    return KIF::RAILCAR;
  case K::EMPLACE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INVOKE:
    return KIF::RAILCAR;
  case K::INVOKE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::COMPOSE:
    return KIF::RAILCAR;
  case K::COMPOSE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DECOMPOSE:
    return KIF::RAILCAR;
  case K::DECOMPOSE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::REMOVE:
    return KIF::RAILCAR;
  case K::REMOVE_OF:
    return KIF::STATEMENT;
  case K::INIT:
    return KIF::RAILCAR;
  case K::INIT_OF:
    return KIF::RVALUE | KIF::STATEMENT;
  case K::INPLACE_DESTROY:
    return KIF::RAILCAR;
  case K::INPLACE_DESTROY_OF:
    return KIF::STATEMENT;
  case K::INPLACE_INIT:
    return KIF::RAILCAR;
  case K::INPLACE_INIT_OF:
    return KIF::STATEMENT;

  // SUBTYPE
  case K::INSTANTIATE_ARRAY:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_REFERENCE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_POINTER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_SLICE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return KIF::PARAMETER;
  case K::NONPOSITIONAL_PARAMETERS_BEGIN:
    return KIF::PARAMETER;
  case K::LOCKED_PARAMETERS_BEGIN:
    return KIF::PARAMETER;
  case K::NONAME:
    return KIF::NAME | KIF::LVALUE;

  // BRACES
  case K::INSTANTIATE_TUPLE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NAMED_ELEMENT:
    return KIF::TUPLE_ELEMENT;
  case K::INSTANTIATE_LAYOUT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SPECIALIZE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::RAILCAR;

  // PROCEDURES
  case K::NAMED_ARGUMENT:
    return KIF::ARGUMENT;
  case K::INSTANTIATE_SIGNATURE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PLACEMENT:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::COMPOSITION:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DEFAULT_VALUE_PARAMETER:
    return KIF::PARAMETER;
  case K::FUNCTION:
    return KIF::STATEMENT;
  case K::IMPLEMENT_FUNCTION:
    return KIF::STATEMENT;
  case K::CONSTRUCTOR:
    return KIF::STATEMENT;
  case K::LAYOUT_CONSTRUCTOR:
    return KIF::STATEMENT | KIF::RAILCAR;

  // CONTROL FLOW
  case K::RETURN:
    return KIF::STATEMENT;
  case K::BREAK:
    return KIF::RAILCAR;
  case K::BREAK_OF:
    return KIF::STATEMENT;
  case K::CONTINUE:
    return KIF::RAILCAR;
  case K::CONTINUE_OF:
    return KIF::STATEMENT;

  // DECLARED TYPES
  case K::CLASS:
    return KIF::STATEMENT;
  case K::ENUM:
    return KIF::STATEMENT;
  case K::INTERFACE:
    return KIF::STATEMENT;
  case K::ADAPTER:
    return KIF::STATEMENT;

  // VALUES
  case K::ARRAY:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TRUE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FALSE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VALUE:
    return KIF::RVALUE;
  case K::INDEX:
    return KIF::RVALUE;
  case K::THIS:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RESULT:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CALLSITE:
    return KIF::RVALUE;

  // BUILTIN TYPES
  case K::SYMBOL:
    return KIF::RVALUE | KIF::TUPLE_ELEMENT;
  case K::INFERENCE:
    return KIF::RVALUE | KIF::ARGUMENT;
  case K::EXPRESSION:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VOID:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NO_RETURN:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BOOLEAN:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::HALF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SINGLE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DOUBLE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::QUADRUPLE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BINARY16:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BINARY32:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BINARY64:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BINARY128:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BFLOAT16:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNSIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FAST_SIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FAST_UNSIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LEAST_SIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LEAST_UNSIGNED_INTEGER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SIGNED_INDEX:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNSIGNED_INDEX:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SIGNED_ADDRESS:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNSIGNED_ADDRESS:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CHAR:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ASCII:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UTF8:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FIRST_VARIADIC_ARGUMENT:
    return KIF::RAILCAR;
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KIF::RAILCAR;
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VARIADIC_ARGUMENTS_TYPE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // SCOPES
  case K::IF_CHAIN:
    return KIF::STATEMENT;
  case K::SWITCH_CHAIN:
    return KIF::STATEMENT;
  case K::SPIN_CHAIN:
    return KIF::STATEMENT;
  case K::IF:
    return KIF::STATEMENT | KIF::STARTING_CHAINLINK | KIF::IF_CHAINLINK;
  case K::ELSE_IF:
    return KIF::CONTINUING_CHAINLINK | KIF::FINISHING_CHAINLINK |
           KIF::IF_CHAINLINK;
  case K::ELSE:
    return KIF::FINISHING_CHAINLINK | KIF::IF_CHAINLINK;
  case K::SWITCH:
    return KIF::STARTING_CHAINLINK | KIF::SWITCH_CHAINLINK;
  case K::CASE:
    return KIF::CONTINUING_CHAINLINK | KIF::FINISHING_CHAINLINK |
           KIF::SWITCH_CHAINLINK;
  case K::DEFAULT:
    return KIF::CONTINUING_CHAINLINK | KIF::FINISHING_CHAINLINK |
           KIF::SWITCH_CHAINLINK;
  case K::FOR:
    return KIF::STATEMENT;
  case K::WHILE:
    return KIF::STATEMENT;
  case K::SPIN:
    return KIF::STARTING_CHAINLINK | KIF::SPIN_CHAINLINK;
  case K::WEAVE:
    return KIF::CONTINUING_CHAINLINK | KIF::FINISHING_CHAINLINK |
           KIF::SPIN_CHAINLINK;
  case K::SCOPE:
    return KIF::STATEMENT;
  case K::FOLD:
    return KIF::STATEMENT;

  // RANGES
  case K::ARITHMETIC_SEQUENCE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return KIF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return KIF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return KIF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return KIF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return KIF::ARITHMETIC_SEQUENCE_STEP;

  // TABLE GRAPH
  case K::IMPORT:
    return KIF::STATEMENT;
  case K::NAMESPACE:
    return KIF::STATEMENT | KIF::ARGUMENT | KIF::RVALUE | KIF::TUPLE_ELEMENT;
  case K::C:
    return KIF::ARGUMENT | KIF::RVALUE | KIF::TUPLE_ELEMENT;
  case K::TOP:
    return KIF::ARGUMENT | KIF::RVALUE | KIF::TUPLE_ELEMENT; // TOP

  // HINTS
  case K::DEBUG_BREAK:
    return KIF::STATEMENT;
  case K::ABORT:
    return KIF::STATEMENT;
  case K::ASSERT:
    return KIF::STATEMENT;
  case K::UNREACHABLE:
    return KIF::STATEMENT;
  case K::ASSUME:
    return KIF::STATEMENT;

  // LOW ATTRIBUTES
  case K::NO_ANCHOR:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::ANCHOR:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_FLANK:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::FLANK:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_OPAQUE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::OPAQUE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_GLOBAL:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::GLOBAL:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_PUBLIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::PUBLIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_PARTIAL_MUTATE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::PARTIAL_MUTATE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_STATIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::STATIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_DELAY:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::DELAY:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_CAPTURE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::CAPTURE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR |
           KIF::TUPLE_ELEMENT;
  case K::NO_INLINE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::INLINE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_MANGLE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::MANGLE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_PACK:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::PACK:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_BRANCH_TREND:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::LIKELY:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::UNLIKELY:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_SUPPORT_STATUS:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::DEPRECIATED:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::EXPERIMENTAL:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_STABLE_ADDRESS:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::STABLE_ADDRESS:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_VARIADIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::VARIADIC:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_LOCATION:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::LOCATION:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_TEMPLATE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::TEMPLATE:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_CONSTRAINT:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::CONSTRAINT:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_WEIGHT:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::WEIGHT:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_AUTO:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::AUTO:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_VIRTUAL:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::VIRTUAL:
    return KIF::LOW_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_RANGER:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::RANGER:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_REQUIRE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::REQUIRE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_ENSURE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::ENSURE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;

  // TYPE ATTRIBUTES
  case K::NO_VAR:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::VAR:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::PARTIAL_VAR:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_VOLATILE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::VOLATILE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_ATOMIC:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::ATOMIC:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_NULL_TERMINATE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NULL_TERMINATE:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::NO_GREATEST:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;
  case K::GREATEST:
    return KIF::HIGH_ATTRIBUTE | KIF::RVALUE | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT;

  // LOW ATTRIBUTE TYPES
  case K::ANCHOR_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FLANK_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::OPAQUE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::GLOBAL_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PUBLIC_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PARTIAL_MUTATE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::STATIC_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DELAY_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CAPTURE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INLINE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MANGLE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PACK_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BRANCH_TREND_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SUPPORT_STATUS_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::STABLE_ADDRESS_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VARIADIC_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOCATION_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TEMPLATE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CONSTRAINT_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WEIGHT_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::AUTO_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VIRTUAL_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RANGER_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::REQUIRE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ENSURE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // HIGH ATTRIBUTE TYPES
  case K::VAR_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VOLATILE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ATOMIC_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NULL_TERMINATE_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::GREATEST_ATTRIBUTE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // REFLECTIONS
  case K::MEMBER_OF:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BAKE:
    return KIF::RAILCAR;
  case K::BAKE_OF:
    return KIF::RVALUE | KIF::TUPLE_ELEMENT;
  case K::IGNORE:
    return KIF::RAILCAR;
  case K::IGNORE_OF:
    return KIF::STATEMENT;
  case K::BYTE_SIZE:
    return KIF::RAILCAR;
  case K::BYTE_SIZE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BIT_DEPTH:
    return KIF::RAILCAR;
  case K::BIT_DEPTH_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ELEMENT_COUNT:
    return KIF::RAILCAR;
  case K::ELEMENT_COUNT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SNIPPET:
    return KIF::RAILCAR;
  case K::SNIPPET_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NAME:
    return KIF::RAILCAR;
  case K::NAME_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LINE:
    return KIF::RAILCAR;
  case K::LINE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::COLUMN:
    return KIF::RAILCAR;
  case K::COLUMN_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS:
    return KIF::RAILCAR;
  case K::IS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::HOLDS:
    return KIF::RAILCAR;
  case K::HOLDS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TYPE:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR;
  case K::TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::HAS_MEMBER:
    return KIF::RAILCAR;
  case K::HAS_MEMBER_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::HAS:
    return KIF::RAILCAR;
  case K::HAS_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::GET:
    return KIF::RAILCAR;
  case K::GET_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SIGNATURE:
    return KIF::RAILCAR;
  case K::SIGNATURE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SYNONYM:
    return KIF::RAILCAR;
  case K::SYNONYM_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INCREMENT:
    return KIF::RAILCAR;
  case K::INCREMENT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DECREMENT:
    return KIF::RAILCAR;
  case K::DECREMENT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WHILST:
    return KIF::RAILCAR;
  case K::WHILST_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ELEMENT:
    return KIF::RAILCAR;
  case K::ELEMENT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::AT:
    return KIF::RAILCAR;
  case K::AT_OF:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MAIN:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR;
  case K::MAIN_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DESTROY:
    return KIF::RAILCAR;
  case K::DESTROY_OF:
    return KIF::STATEMENT | KIF::TUPLE_ELEMENT;
  case K::FORWARD:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR;
  case K::FORWARD_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BACKWARD:
    return KIF::RAILCAR;
  case K::BACKWARD_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNDERLYING_VALUE:
    return KIF::RAILCAR;
  case K::UNDERLYING_VALUE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNDERLYING_TYPE:
    return KIF::RAILCAR;
  case K::UNDERLYING_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::REFLECT:
    return KIF::RAILCAR;
  case K::REFLECT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::POLYMORPH:
    return KIF::RAILCAR;
  case K::POLYMORPH_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::OVERLOAD:
    return KIF::RAILCAR;
  case K::OVERLOAD_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::OVERLOAD_RANGE:
    return KIF::RAILCAR;
  case K::OVERLOAD_RANGE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SPECIALIZATION_RANGE:
    return KIF::RAILCAR;
  case K::SPECIALIZATION_RANGE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WEIGHT_LEVEL:
    return KIF::RAILCAR;
  case K::WEIGHT_LEVEL_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WEIGHT_LEVEL_RANGE:
    return KIF::RAILCAR;
  case K::WEIGHT_LEVEL_RANGE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WEIGHT_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TEMPLATE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TEMPLATE_RANGE:
    return KIF::RAILCAR;
  case K::TEMPLATE_RANGE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CONSTRUCTOR_RANGE:
    return KIF::RAILCAR;
  case K::CONSTRUCTOR_RANGE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RESOLVE_TEMPLATE:
    return KIF::RAILCAR;
  case K::RESOLVE_TEMPLATE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RESOLVE_FUNCTION:
    return KIF::RAILCAR;
  case K::RESOLVE_FUNCTION_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RESOLVE_ADAPTER:
    return KIF::RAILCAR;
  case K::RESOLVE_ADAPTER_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_TYPE:
    return KIF::RAILCAR;
  case K::IS_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_RANGE_TYPE:
    return KIF::RAILCAR;
  case K::IS_RANGE_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_PLACEMENT_TYPE:
    return KIF::RAILCAR;
  case K::IS_PLACEMENT_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_SIGNED_TYPE:
    return KIF::RAILCAR;
  case K::IS_SIGNED_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_UNSIGNED_TYPE:
    return KIF::RAILCAR;
  case K::IS_UNSIGNED_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_INTEGER_TYPE:
    return KIF::RAILCAR;
  case K::IS_INTEGER_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_FLOAT_TYPE:
    return KIF::RAILCAR;
  case K::IS_FLOAT_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_BINARY_TYPE:
    return KIF::RAILCAR;
  case K::IS_BINARY_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_BFLOAT_TYPE:
    return KIF::RAILCAR;
  case K::IS_BFLOAT_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_STRING_TYPE:
    return KIF::RAILCAR;
  case K::IS_STRING_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_CODEUNIT_TYPE:
    return KIF::RAILCAR;
  case K::IS_CODEUNIT_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_LOW_ATTRIBUTE_TYPE:
    return KIF::RAILCAR;
  case K::IS_LOW_ATTRIBUTE_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::IS_HIGH_ATTRIBUTE_TYPE:
    return KIF::RAILCAR;
  case K::IS_HIGH_ATTRIBUTE_TYPE_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

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
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::LITERAL);
}

[[nodiscard]] inline rq::Keyword
getSituatedAscribeKeyword(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_LOW:
    return rq::Keyword::ASCRIBE_LOW;
  case rq::Keyword::UNSITUATED_ASCRIBE_HIGH:
    return rq::Keyword::ASCRIBE_HIGH;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::UNQUOTED_LEFT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::UNQUOTED_RIGHT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::CONVERGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericLiteral(rq::Keyword keyword) {
  return keyword == rq::Keyword::INTEGER_LITERAL ||
         keyword == rq::Keyword::FLOAT_LITERAL;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordInfoFlags::STARTING_CHAINLINK |
                                   rq::KeywordInfoFlags::CONTINUING_CHAINLINK |
                                   rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingChainLink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingChainLink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingChainLink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsHighAttribute(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::HIGH_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLowAttribute(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::LOW_ATTRIBUTE);
}

enum class Situation : std::uint_fast8_t {
  NONE,
  TOP,
  STATEMENT,
  LVALUE,
  RVALUE,
  RAILCAR,
  ARGUMENT,
  PARAMETER,
  TUPLE_ELEMENT,
  BINDING,
  NAME,
  NAMESPACE,
  ASCRIPTION,
  LOW_ATTRIBUTE_INSTANTIATION,
  HIGH_ATTRIBUTE_INSTANTIATION,
  ARITHMETIC_SEQUENCE_STAGE,
  STARTING_CHAINLINK,
  CONTINUING_CHAINLINK,
  FINISHING_CHAINLINK,
  IF_CHAINLINK,
  SWITCH_CHAINLINK,
  SPIN_CHAINLINK,
  STARTING_IF_CHAINLINK,
  CONTINUING_IF_CHAINLINK,
  FINISHING_IF_CHAINLINK,
  STARTING_SWITCH_CHAINLINK,
  CONTINUING_SWITCH_CHAINLINK,
  FINISHING_SWITCH_CHAINLINK,
  STARTING_SPIN_CHAINLINK,
  CONTINUING_SPIN_CHAINLINK,
  FINISHING_SPIN_CHAINLINK
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
  case S::RAILCAR:
    return "railcar expression";
  case S::ARGUMENT:
    return "argument expression";
  case S::PARAMETER:
    return "parameter expression";
  case S::TUPLE_ELEMENT:
    return "tuple element expression";
  case S::BINDING:
    return "binding expression";
  case S::NAME:
    return "name expression";
  case S::NAMESPACE:
    return "namespace expression";
  case S::ASCRIPTION:
    return "ascription expression";
  case S::LOW_ATTRIBUTE_INSTANTIATION:
    return "low attribute instantiation";
  case S::HIGH_ATTRIBUTE_INSTANTIATION:
    return "high attribute instantiation";
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return "sequence stage expression";
  case S::STARTING_CHAINLINK:
    return "starting chainlink";
  case S::CONTINUING_CHAINLINK:
    return "continuing chainlink";
  case S::FINISHING_CHAINLINK:
    return "finishing chainlink";
  case S::IF_CHAINLINK:
    return "if chainlink";
  case S::SWITCH_CHAINLINK:
    return "switch chainlink";
  case S::SPIN_CHAINLINK:
    return "spin chainlink";
  case S::STARTING_IF_CHAINLINK:
    return "starting if chainlink";
  case S::CONTINUING_IF_CHAINLINK:
    return "continuing if chainlink";
  case S::FINISHING_IF_CHAINLINK:
    return "finishing if chainlink";
  case S::STARTING_SWITCH_CHAINLINK:
    return "starting swtich chainlink";
  case S::CONTINUING_SWITCH_CHAINLINK:
    return "continuing switch chainlink";
  case S::FINISHING_SWITCH_CHAINLINK:
    return "finishing switch chainlink";
  case S::STARTING_SPIN_CHAINLINK:
    return "starting spin chainlink";
  case S::CONTINUING_SPIN_CHAINLINK:
    return "continuing spin chainlink";
  case S::FINISHING_SPIN_CHAINLINK:
    return "finishing spin chainlink";
  }
  return "error expression";
}

[[nodiscard]] inline rq::Keyword getUniversalized(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::IDENTIFY:
    return K::IDENTIFY_OF;
  case K::AS:
    return K::AS_OF;
  case K::OF:
    return K::OF_OF;
  case K::CAST:
    return K::CAST_OF;
  case K::BITWISE_CAST:
    return K::BITWISE_CAST_OF;
  case K::SIGNATURE_CAST:
    return K::SIGNATURE_CAST_OF;
  case K::CONTENT:
    return K::CONTENT_OF;
  case K::ADDRESS:
    return K::ADDRESS_OF;
  case K::FUNCTION_ADDRESS:
    return K::FUNCTION_ADDRESS_OF;
  case K::DATA_ADDRESS:
    return K::DATA_ADDRESS_OF;
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
  case K::REMOVE:
    return K::REMOVE_OF;
  case K::INIT:
    return K::INIT_OF;
  case K::INPLACE_DESTROY:
    return K::INPLACE_DESTROY_OF;
  case K::INPLACE_INIT:
    return K::INPLACE_INIT_OF;
  case K::BREAK:
    return K::BREAK_OF;
  case K::CONTINUE:
    return K::CONTINUE_OF;
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::NEXT_VARIADIC_ARGUMENT_OF;
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
    return K::IS_OF;
  case K::HOLDS:
    return K::HOLDS_OF;
  case K::TYPE:
    return K::TYPE_OF;
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
  case K::INCREMENT:
    return K::INCREMENT_OF;
  case K::DECREMENT:
    return K::DECREMENT_OF;
  case K::WHILST:
    return K::WHILST_OF;
  case K::ELEMENT:
    return K::ELEMENT_OF;
  case K::AT:
    return K::AT_OF;
  case K::MAIN:
    return K::MAIN_OF;
  case K::DESTROY:
    return K::DESTROY_OF;
  case K::FORWARD:
    return K::FORWARD_OF;
  case K::BACKWARD:
    return K::BACKWARD_OF;
  case K::UNDERLYING_VALUE:
    return K::UNDERLYING_VALUE_OF;
  case K::UNDERLYING_TYPE:
    return K::UNDERLYING_TYPE_OF;
  case K::REFLECT:
    return K::REFLECT_OF;
  case K::POLYMORPH:
    return K::POLYMORPH_OF;
  case K::OVERLOAD:
    return K::OVERLOAD_OF;
  case K::OVERLOAD_RANGE:
    return K::OVERLOAD_RANGE_OF;
  case K::SPECIALIZATION_RANGE:
    return K::SPECIALIZATION_RANGE_OF;
  case K::WEIGHT_LEVEL:
    return K::WEIGHT_LEVEL_OF;
  case K::WEIGHT_LEVEL_RANGE:
    return K::WEIGHT_LEVEL_RANGE_OF;
  case K::WEIGHT:
    return K::WEIGHT_OF;
  case K::TEMPLATE:
    return K::TEMPLATE_OF;
  case K::TEMPLATE_RANGE:
    return K::TEMPLATE_RANGE_OF;
  case K::CONSTRUCTOR_RANGE:
    return K::CONSTRUCTOR_RANGE_OF;
  case K::RESOLVE_TEMPLATE:
    return K::RESOLVE_TEMPLATE_OF;
  case K::RESOLVE_FUNCTION:
    return K::RESOLVE_FUNCTION_OF;
  case K::RESOLVE_ADAPTER:
    return K::RESOLVE_ADAPTER_OF;
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
  case K::IS_LOW_ATTRIBUTE_TYPE:
    return K::IS_LOW_ATTRIBUTE_TYPE_OF;
  case K::IS_HIGH_ATTRIBUTE_TYPE:
    return K::IS_LOW_ATTRIBUTE_TYPE_OF;
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
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLvalue(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRvalue(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRailcar(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::RAILCAR);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTupleElement(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::TUPLE_ELEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::BINDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeNamespace(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::NAMESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeLowAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_LOW_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeHighAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_HIGH_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStage(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasSome(flags,
                        rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_CONDITION |
                            rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceCondition(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags,
                       rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_CONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStep(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeIfChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::IF_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeSwitchChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SWITCH_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSpinChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SPIN_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingIfChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::IF_CHAINLINK |
                                  rq::KeywordInfoFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingIfChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::IF_CHAINLINK |
                                  rq::KeywordInfoFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingIfChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::IF_CHAINLINK |
                                  rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingSwitchChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SWITCH_CHAINLINK |
                                  rq::KeywordInfoFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingSwitchChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SWITCH_CHAINLINK |
                                  rq::KeywordInfoFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingSwitchChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SWITCH_CHAINLINK |
                                  rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingSpinChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SPIN_CHAINLINK |
                                  rq::KeywordInfoFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingSpinChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SPIN_CHAINLINK |
                                  rq::KeywordInfoFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingSpinChainlink(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::SPIN_CHAINLINK |
                                  rq::KeywordInfoFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsChainlinkKind(rq::Situation situation) {
  using S = rq::Situation;
  return situation == S::IF_CHAINLINK || situation == S::SWITCH_CHAINLINK ||
         situation == S::SPIN_CHAINLINK;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsChainlinkPosition(rq::Situation situation) {
  using S = rq::Situation;
  return situation == S::STARTING_CHAINLINK ||
         situation == S::CONTINUING_CHAINLINK ||
         situation == S::FINISHING_CHAINLINK;
}

[[nodiscard]] inline bool getCanBeSituation(rq::Keyword keyword,
                                            rq::Situation situation) {
  using S = rq::Situation;
  switch (situation) {
  case S::NONE:
    return rq::getIsNone(keyword);
  case S::TOP:
    return rq::getCanBeTop(keyword);
  case S::STATEMENT:
    return rq::getCanBeStatement(keyword);
  case S::LVALUE:
    return rq::getCanBeLvalue(keyword);
  case S::RVALUE:
    return rq::getCanBeRvalue(keyword);
  case S::RAILCAR:
    return rq::getCanBeRailcar(keyword);
  case S::ARGUMENT:
    return rq::getCanBeArgument(keyword);
  case S::PARAMETER:
    return rq::getCanBeParameter(keyword);
  case S::TUPLE_ELEMENT:
    return rq::getCanBeTupleElement(keyword);
  case S::BINDING:
    return rq::getCanBeBinding(keyword);
  case S::NAME:
    return rq::getCanBeName(keyword);
  case S::NAMESPACE:
    return rq::getCanBeNamespace(keyword);
  case S::ASCRIPTION:
    return rq::getCanBeAscription(keyword);
  case S::LOW_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeLowAttributeInstantiation(keyword);
  case S::HIGH_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeHighAttributeInstantiation(keyword);
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return rq::getCanBeArithmeticSequenceStage(keyword);
  case S::STARTING_CHAINLINK:
    return rq::getCanBeStartingChainlink(keyword);
  case S::CONTINUING_CHAINLINK:
    return rq::getCanBeContinuingChainLink(keyword);
  case S::FINISHING_CHAINLINK:
    return rq::getCanBeFinishingChainLink(keyword);
  case S::IF_CHAINLINK:
    return rq::getCanBeIfChainlink(keyword);
  case S::SWITCH_CHAINLINK:
    return rq::getCanBeSwitchChainlink(keyword);
  case S::SPIN_CHAINLINK:
    return rq::getCanBeSpinChainlink(keyword);
  case S::STARTING_IF_CHAINLINK:
    return rq::getCanBeStartingIfChainlink(keyword);
  case S::CONTINUING_IF_CHAINLINK:
    return rq::getCanBeContinuingIfChainlink(keyword);
  case S::FINISHING_IF_CHAINLINK:
    return rq::getCanBeFinishingIfChainlink(keyword);
  case S::STARTING_SWITCH_CHAINLINK:
    return rq::getCanBeStartingSwitchChainlink(keyword);
  case S::CONTINUING_SWITCH_CHAINLINK:
    return rq::getCanBeContinuingSwitchChainlink(keyword);
  case S::FINISHING_SWITCH_CHAINLINK:
    return rq::getCanBeFinishingSwitchChainlink(keyword);
  case S::STARTING_SPIN_CHAINLINK:
    return rq::getCanBeStartingSpinChainlink(keyword);
  case S::CONTINUING_SPIN_CHAINLINK:
    return rq::getCanBeContinuingSpinChainlink(keyword);
  case S::FINISHING_SPIN_CHAINLINK:
    return rq::getCanBeFinishingSpinChainlink(keyword);
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName(rq::Keyword keyword) {
  return keyword == rq::Keyword::IDENTIFY_OF ||
         keyword == rq::Keyword::IDENTIFIER_LITERAL;
}

enum class LowAttribute : std::uint_fast8_t {
  NONE,
  // anchor_attribute
  NO_ANCHOR,
  ANCHOR,
  // flank_attribute
  NO_FLANK,
  FLANK,
  // opaque_attribute
  NO_OPAQUE,
  OPAQUE,
  // global_attribute
  NO_GLOBAL,
  GLOBAL,
  // public_attribute
  NO_PUBLIC,
  PUBLIC,
  // partial_mutate_attribute
  NO_PARTIAL_MUTATE,
  PARTIAL_MUTATE,
  // static_attribute
  NO_STATIC,
  STATIC,
  // delay_attribute
  NO_DELAY,
  DELAY,
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
  DEPRECIATED,
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
  // auto_attribute
  NO_AUTO,
  AUTO,
  // virtual_attribute
  NO_VIRTUAL,
  VIRTUAL,
  // ranger_attribute
  NO_RANGER,
  RANGER,
  // require_attribute
  NO_REQUIRE,
  REQUIRE,
  // ensure_attribute
  NO_ENSURE,
  ENSURE,

  LAST
};

[[nodiscard]] inline llvm::StringRef getName(rq::LowAttribute attribute) {
  using namespace rq;
  using LA = LowAttribute;
  switch (attribute) {
  case LA::NONE:
    return "none";
  case LA::NO_ANCHOR:
    return "no_anchor";
  case LA::ANCHOR:
    return "anchor";
  case LA::NO_FLANK:
    return "no_flank";
  case LA::FLANK:
    return "flank";
  case LA::NO_OPAQUE:
    return "no_opaque";
  case LA::OPAQUE:
    return "opaque";
  case LA::NO_GLOBAL:
    return "no_global";
  case LA::GLOBAL:
    return "global";
  case LA::NO_PUBLIC:
    return "no_public";
  case LA::PUBLIC:
    return "public";
  case LA::NO_PARTIAL_MUTATE:
    return "no_partial_mutate";
  case LA::PARTIAL_MUTATE:
    return "partial_mutate";
  case LA::NO_STATIC:
    return "no_static";
  case LA::STATIC:
    return "static";
  case LA::NO_DELAY:
    return "no_delay";
  case LA::DELAY:
    return "delay";
  case LA::NO_CAPTURE:
    return "no_capture";
  case LA::CAPTURE:
    return "capture";
  case LA::NO_INLINE:
    return "no_inline";
  case LA::INLINE:
    return "inline";
  case LA::NO_MANGLE:
    return "no_mangle";
  case LA::MANGLE:
    return "mangle";
  case LA::NO_PACK:
    return "no_pack";
  case LA::PACK:
    return "pack";
  case LA::NO_BRANCH_TREND:
    return "no_branch_trend";
  case LA::LIKELY:
    return "likely";
  case LA::UNLIKELY:
    return "unlikely";
  case LA::NO_SUPPORT_STATUS:
    return "no_support_status";
  case LA::DEPRECIATED:
    return "depreciated";
  case LA::EXPERIMENTAL:
    return "experimental";
  case LA::NO_STABLE_ADDRESS:
    return "no_stable_address";
  case LA::STABLE_ADDRESS:
    return "stable_address";
  case LA::NO_VARIADIC:
    return "no_variadic";
  case LA::VARIADIC:
    return "variadic";
  case LA::NO_LOCATION:
    return "no_location";
  case LA::LOCATION:
    return "location";
  case LA::NO_TEMPLATE:
    return "no_template";
  case LA::TEMPLATE:
    return "template";
  case LA::NO_CONSTRAINT:
    return "no_constraint";
  case LA::CONSTRAINT:
    return "constraint";
  case LA::NO_WEIGHT:
    return "no_weight";
  case LA::WEIGHT:
    return "weight";
  case LA::NO_AUTO:
    return "no_auto";
  case LA::AUTO:
    return "auto";
  case LA::NO_VIRTUAL:
    return "no_virtual";
  case LA::VIRTUAL:
    return "virtual";
  case LA::NO_RANGER:
    return "no_ranger";
  case LA::RANGER:
    return "ranger";
  case LA::NO_REQUIRE:
    return "no_require";
  case LA::REQUIRE:
    return "require";
  case LA::NO_ENSURE:
    return "no_ensure";
  case LA::ENSURE:
    return "ensure";
  case LA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::LowAttribute getLowAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using LA = LowAttribute;
  switch (keyword) {
  case K::NO_ANCHOR:
    return LA::NO_ANCHOR;
  case K::ANCHOR:
    return LA::ANCHOR;
  case K::NO_FLANK:
    return LA::NO_FLANK;
  case K::FLANK:
    return LA::FLANK;
  case K::NO_OPAQUE:
    return LA::NO_OPAQUE;
  case K::OPAQUE:
    return LA::OPAQUE;
  case K::NO_GLOBAL:
    return LA::NO_GLOBAL;
  case K::GLOBAL:
    return LA::GLOBAL;
  case K::NO_PUBLIC:
    return LA::NO_PUBLIC;
  case K::PUBLIC:
    return LA::PUBLIC;
  case K::NO_PARTIAL_MUTATE:
    return LA::NO_PARTIAL_MUTATE;
  case K::PARTIAL_MUTATE:
    return LA::PARTIAL_MUTATE;
  case K::NO_STATIC:
    return LA::NO_STATIC;
  case K::STATIC:
    return LA::STATIC;
  case K::NO_DELAY:
    return LA::NO_DELAY;
  case K::DELAY:
    return LA::DELAY;
  case K::NO_CAPTURE:
    return LA::NO_CAPTURE;
  case K::CAPTURE:
    return LA::CAPTURE;
  case K::NO_INLINE:
    return LA::NO_INLINE;
  case K::INLINE:
    return LA::INLINE;
  case K::NO_MANGLE:
    return LA::NO_MANGLE;
  case K::MANGLE:
    return LA::MANGLE;
  case K::NO_PACK:
    return LA::NO_PACK;
  case K::PACK:
    return LA::PACK;
  case K::NO_BRANCH_TREND:
    return LA::NO_BRANCH_TREND;
  case K::LIKELY:
    return LA::LIKELY;
  case K::UNLIKELY:
    return LA::UNLIKELY;
  case K::NO_SUPPORT_STATUS:
    return LA::NO_SUPPORT_STATUS;
  case K::DEPRECIATED:
    return LA::DEPRECIATED;
  case K::EXPERIMENTAL:
    return LA::EXPERIMENTAL;
  case K::NO_STABLE_ADDRESS:
    return LA::NO_STABLE_ADDRESS;
  case K::STABLE_ADDRESS:
    return LA::STABLE_ADDRESS;
  case K::NO_VARIADIC:
    return LA::NO_VARIADIC;
  case K::VARIADIC:
    return LA::VARIADIC;
  case K::NO_LOCATION:
    return LA::NO_LOCATION;
  case K::LOCATION:
    return LA::LOCATION;
  case K::NO_TEMPLATE:
    return LA::NO_TEMPLATE;
  case K::TEMPLATE:
    return LA::TEMPLATE;
  case K::NO_CONSTRAINT:
    return LA::NO_CONSTRAINT;
  case K::CONSTRAINT:
    return LA::CONSTRAINT;
  case K::NO_WEIGHT:
    return LA::NO_WEIGHT;
  case K::WEIGHT:
    return LA::WEIGHT;
  case K::NO_AUTO:
    return LA::NO_AUTO;
  case K::AUTO:
    return LA::AUTO;
  case K::NO_VIRTUAL:
    return LA::NO_VIRTUAL;
  case K::VIRTUAL:
    return LA::VIRTUAL;
  case K::NO_RANGER:
    return LA::NO_RANGER;
  case K::RANGER:
    return LA::RANGER;
  case K::NO_ENSURE:
    return LA::NO_ENSURE;
  case K::ENSURE:
    return LA::ENSURE;
  case K::NO_REQUIRE:
    return LA::NO_REQUIRE;
  case K::REQUIRE:
    return LA::REQUIRE;
  default:
    break;
  }
  return LA::NONE;
}

enum class LowFuseFlags : std::uint_fast32_t {
  NONE = 0,

  ANCHOR = rq::getBit(0),
  ANCHOR_MASK = ANCHOR,

  FLANK = rq::getBit(1),
  FLANK_MASK = FLANK,

  OPAQUE = rq::getBit(2),
  OPAQUE_MASK = OPAQUE,

  GLOBAL = rq::getBit(3),
  GLOBAL_MASK = GLOBAL,

  PUBLIC = rq::getBit(4),
  PUBLIC_MASK = PUBLIC,

  PARTIAL_MUTATE = rq::getBit(5),
  PARTIAL_MUTATE_MASK = PARTIAL_MUTATE,

  STATIC = rq::getBit(6),
  STATIC_MASK = STATIC,

  DELAY = rq::getBit(7),
  DELAY_MASK = DELAY,

  CAPTURE = rq::getBit(8),
  CAPTURE_MASK = CAPTURE,

  INLINE = rq::getBit(9),
  INLINE_MASK = INLINE,

  MANGLE = rq::getBit(10),
  MANGLE_MASK = MANGLE,

  PACK = rq::getBit(11),
  PACK_MASK = PACK,

  LIKELY = rq::getBit(12),
  UNLIKELY = rq::getBit(13),
  BRANCH_TREND_MASK = LIKELY | UNLIKELY,

  DEPRECIATED = rq::getBit(14),
  EXPERIMENTAL = rq::getBit(15),
  SUPPORT_STATUS_MASK = DEPRECIATED | EXPERIMENTAL,

  STABLE_ADDRESS = rq::getBit(16),
  STABLE_ADDRESS_MASK = STABLE_ADDRESS,

  VARIADIC = rq::getBit(17),
  VARIADIC_MASK = VARIADIC,

  LOCATION = rq::getBit(18),
  LOCATION_MASK = LOCATION,

  TEMPLATE = rq::getBit(19),
  TEMPLATE_MASK = TEMPLATE,

  CONSTRAINT = rq::getBit(20),
  CONSTRAINT_MASK = CONSTRAINT,

  WEIGHT = rq::getBit(21),
  WEIGHT_MASK = WEIGHT,

  AUTO = rq::getBit(22),
  AUTO_MASK = AUTO,

  VIRTUAL = rq::getBit(23),
  VIRTUAL_MASK = VIRTUAL,

  RANGER = rq::getBit(24),
  RANGER_MASK = RANGER,

  REQUIRE = rq::getBit(25),
  REQUIRE_MASK = REQUIRE,

  ENSURE = rq::getBit(26),
  ENSURE_MASK = ENSURE,

  ALL_MASK = ANCHOR | FLANK | OPAQUE | PUBLIC | PARTIAL_MUTATE | STATIC |
             DELAY | CAPTURE | INLINE | PACK | LIKELY | UNLIKELY | DEPRECIATED |
             EXPERIMENTAL | STABLE_ADDRESS | VARIADIC | LOCATION | TEMPLATE |
             CONSTRAINT | WEIGHT | AUTO | VIRTUAL | RANGER | REQUIRE | ENSURE
};

RQ_DEFINE_FLAGS(rq::LowFuseFlags);

[[nodiscard]] inline rq::LowFuseFlags getFuseFlags(rq::LowAttribute attribute) {
  using namespace rq;
  using LA = LowAttribute;
  using LFF = LowFuseFlags;
  switch (attribute) {
  case LA::NONE:
    return LFF::NONE;
  case LA::NO_ANCHOR:
    return LFF::NONE;
  case LA::ANCHOR:
    return LFF::ANCHOR;
  case LA::NO_FLANK:
    return LFF::NONE;
  case LA::FLANK:
    return LFF::FLANK;
  case LA::NO_OPAQUE:
    return LFF::NONE;
  case LA::OPAQUE:
    return LFF::OPAQUE;
  case LA::NO_GLOBAL:
    return LFF::NONE;
  case LA::GLOBAL:
    return LFF::GLOBAL;
  case LA::NO_PUBLIC:
    return LFF::NONE;
  case LA::PUBLIC:
    return LFF::PUBLIC;
  case LA::NO_PARTIAL_MUTATE:
    return LFF::NONE;
  case LA::PARTIAL_MUTATE:
    return LFF::PARTIAL_MUTATE;
  case LA::NO_STATIC:
    return LFF::NONE;
  case LA::STATIC:
    return LFF::STATIC;
  case LA::NO_DELAY:
    return LFF::NONE;
  case LA::DELAY:
    return LFF::DELAY;
  case LA::NO_CAPTURE:
    return LFF::NONE;
  case LA::CAPTURE:
    return LFF::CAPTURE;
  case LA::NO_INLINE:
    return LFF::NONE;
  case LA::INLINE:
    return LFF::INLINE;
  case LA::NO_MANGLE:
    return LFF::NONE;
  case LA::MANGLE:
    return LFF::MANGLE;
  case LA::NO_PACK:
    return LFF::NONE;
  case LA::PACK:
    return LFF::PACK;
  case LA::NO_BRANCH_TREND:
    return LFF::NONE;
  case LA::LIKELY:
    return LFF::LIKELY;
  case LA::UNLIKELY:
    return LFF::UNLIKELY;
  case LA::NO_SUPPORT_STATUS:
    return LFF::NONE;
  case LA::DEPRECIATED:
    return LFF::DEPRECIATED;
  case LA::EXPERIMENTAL:
    return LFF::EXPERIMENTAL;
  case LA::NO_STABLE_ADDRESS:
    return LFF::NONE;
  case LA::STABLE_ADDRESS:
    return LFF::STABLE_ADDRESS;
  case LA::NO_VARIADIC:
    return LFF::NONE;
  case LA::VARIADIC:
    return LFF::VARIADIC;
  case LA::NO_LOCATION:
    return LFF::NONE;
  case LA::LOCATION:
    return LFF::LOCATION;
  case LA::NO_TEMPLATE:
    return LFF::NONE;
  case LA::TEMPLATE:
    return LFF::TEMPLATE;
  case LA::NO_CONSTRAINT:
    return LFF::NONE;
  case LA::CONSTRAINT:
    return LFF::CONSTRAINT;
  case LA::NO_WEIGHT:
    return LFF::NONE;
  case LA::WEIGHT:
    return LFF::WEIGHT;
  case LA::NO_AUTO:
    return LFF::NONE;
  case LA::AUTO:
    return LFF::AUTO;
  case LA::NO_VIRTUAL:
    return LFF::NONE;
  case LA::VIRTUAL:
    return LFF::VIRTUAL;
  case LA::NO_RANGER:
    return LFF::NONE;
  case LA::RANGER:
    return LFF::RANGER;
  case LA::NO_REQUIRE:
    return LFF::NONE;
  case LA::REQUIRE:
    return LFF::REQUIRE;
  case LA::NO_ENSURE:
    return LFF::NONE;
  case LA::ENSURE:
    return LFF::ENSURE;
  case LA::LAST:
    break;
  }
  return LFF::NONE;
}

enum class LowInfoFlags : std::uint_fast8_t {
  NONE = 0,

  DEFAULT_OF_KIND,
  MUST_HAVE_ATTACHMENT,
  MUST_NOT_HAVE_ATTACHMENT
};

RQ_DEFINE_FLAGS(rq::LowInfoFlags);

[[nodiscard]] RQ_ALWAYS_INLINE rq::LowInfoFlags
getInfoFlags(rq::LowAttribute attribute) {
  using LA = rq::LowAttribute;
  using LIF = rq::LowInfoFlags;
  switch (attribute) {
  case LA::NONE:
    return LIF::NONE;
  case LA::NO_ANCHOR:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::ANCHOR:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_FLANK:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::FLANK:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_OPAQUE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::OPAQUE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_GLOBAL:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::GLOBAL:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_PUBLIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::PUBLIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_PARTIAL_MUTATE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::PARTIAL_MUTATE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_STATIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::STATIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_DELAY:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::DELAY:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_CAPTURE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::CAPTURE:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_INLINE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::INLINE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_MANGLE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::MANGLE:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_PACK:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::PACK:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_BRANCH_TREND:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::LIKELY:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::UNLIKELY:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_SUPPORT_STATUS:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::DEPRECIATED:
    return LIF::NONE;
  case LA::EXPERIMENTAL:
    return LIF::NONE;
  case LA::NO_STABLE_ADDRESS:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::STABLE_ADDRESS:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_VARIADIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::VARIADIC:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_LOCATION:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::LOCATION:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_TEMPLATE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::TEMPLATE:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_CONSTRAINT:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::CONSTRAINT:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_WEIGHT:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::WEIGHT:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_AUTO:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::AUTO:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_VIRTUAL:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::VIRTUAL:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_RANGER:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::RANGER:
    return LIF::MUST_NOT_HAVE_ATTACHMENT;
  case LA::NO_REQUIRE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::REQUIRE:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::NO_ENSURE:
    return LIF::MUST_NOT_HAVE_ATTACHMENT | LIF::DEFAULT_OF_KIND;
  case LA::ENSURE:
    return LIF::MUST_HAVE_ATTACHMENT;
  case LA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsDefaultOfKind(rq::LowAttribute attribute) {
  rq::LowInfoFlags flags = rq::getInfoFlags(attribute);
  return rq::getHasAll(flags, rq::LowInfoFlags::DEFAULT_OF_KIND);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMustHaveAttachment(rq::LowAttribute attribute) {
  rq::LowInfoFlags flags = rq::getInfoFlags(attribute);
  return rq::getHasAll(flags, rq::LowInfoFlags::MUST_HAVE_ATTACHMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMustNotHaveAttachment(rq::LowAttribute attribute) {
  rq::LowInfoFlags flags = rq::getInfoFlags(attribute);
  return rq::getHasAll(flags, rq::LowInfoFlags::MUST_NOT_HAVE_ATTACHMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMayOrMayNotHaveAttachment(rq::LowAttribute attribute) {
  rq::LowInfoFlags flags = rq::getInfoFlags(attribute);
  return rq::getHasNone(flags, rq::LowInfoFlags::MUST_HAVE_ATTACHMENT |
                                   rq::LowInfoFlags::MUST_NOT_HAVE_ATTACHMENT);
}

enum class LowAttributeKind : std::uint_fast8_t {
  NONE,
  ANCHOR,         // no_anchor vs anchor
  FLANK,          // no_flank vs flank
  OPAQUE,         // no_opaque vs opaque
  GLOBAL,         // no_global vs global
  PUBLIC,         // no_public vs public
  PARTIAL_MUTATE, // no_partial_mutate vs partial_mutate
  STATIC,         // no_static vs static
  DELAY,          // no_delay vs delay
  CAPTURE,        // no_capture vs capture
  INLINE,         // no_inline vs inline
  MANGLE,         // no_mangle vs mangle
  PACK,           // no_pack vs pack
  BRANCH_TREND,   // no_branch_trend vs likely vs unlikely
  SUPPORT_STATUS, // no_support_status vs depreciate vs experimental
  STABLE_ADDRESS, // no_stable_address vs stable_address
  VARIADIC,       // no_variadic vs variadic
  LOCATION,       // no_location vs location
  TEMPLATE,       // no_template vs template
  CONSTRAINT,     // no_constraint vs constraint
  WEIGHT,         // no_weight vs weight
  AUTO,           // no_auto vs auto
  VIRTUAL,        // no_virtual vs virtual
  RANGER,         // no_ranger vs ranger
  REQUIRE,        // no_require vs require
  ENSURE          // no_ensure vs ensure
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::LowAttributeKind kind) {
  using LAK = rq::LowAttributeKind;
  switch (kind) {
  case LAK::NONE:
    break;
  case LAK::ANCHOR:
    return "anchor attribute";
  case LAK::FLANK:
    return "flank attribute";
  case LAK::OPAQUE:
    return "opaque attribute";
  case LAK::GLOBAL:
    return "global attribute";
  case LAK::PUBLIC:
    return "public attribute";
  case LAK::PARTIAL_MUTATE:
    return "partial mutate attribute";
  case LAK::STATIC:
    return "static attribute";
  case LAK::DELAY:
    return "delay attribute";
  case LAK::CAPTURE:
    return "capture attribute";
  case LAK::INLINE:
    return "inline attribute";
  case LAK::MANGLE:
    return "mangle attribute";
  case LAK::PACK:
    return "pack attribute";
  case LAK::BRANCH_TREND:
    return "branch trend attribute";
  case LAK::SUPPORT_STATUS:
    return "support status attribute";
  case LAK::STABLE_ADDRESS:
    return "stable address attribute";
  case LAK::VARIADIC:
    return "variadic attribute";
  case LAK::LOCATION:
    return "location attribute";
  case LAK::TEMPLATE:
    return "template attribute";
  case LAK::CONSTRAINT:
    return "constraint attribute";
  case LAK::WEIGHT:
    return "weight attribute";
  case LAK::AUTO:
    return "auto attribute";
  case LAK::VIRTUAL:
    return "virtual attribute";
  case LAK::RANGER:
    return "ranger attribute";
  case LAK::REQUIRE:
    return "require attribute";
  case LAK::ENSURE:
    return "ensure attribute";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::LowAttributeKind getKind(rq::LowAttribute attribute) {
  using LA = rq::LowAttribute;
  using LAK = rq::LowAttributeKind;
  switch (attribute) {
  case LA::NONE:
    return LAK::NONE;
  case LA::NO_ANCHOR:
    [[fallthrough]];
  case LA::ANCHOR:
    return LAK::ANCHOR;
  case LA::NO_FLANK:
    [[fallthrough]];
  case LA::FLANK:
    return LAK::FLANK;
  case LA::NO_OPAQUE:
    [[fallthrough]];
  case LA::OPAQUE:
    return LAK::OPAQUE;
  case LA::NO_GLOBAL:
    [[fallthrough]];
  case LA::GLOBAL:
    return LAK::GLOBAL;
  case LA::NO_PUBLIC:
    [[fallthrough]];
  case LA::PUBLIC:
    return LAK::PUBLIC;
  case LA::NO_PARTIAL_MUTATE:
    [[fallthrough]];
  case LA::PARTIAL_MUTATE:
    return LAK::PARTIAL_MUTATE;
  case LA::NO_STATIC:
    [[fallthrough]];
  case LA::STATIC:
    return LAK::STATIC;
  case LA::NO_DELAY:
    [[fallthrough]];
  case LA::DELAY:
    return LAK::DELAY;
  case LA::NO_CAPTURE:
    [[fallthrough]];
  case LA::CAPTURE:
    return LAK::CAPTURE;
  case LA::NO_INLINE:
    [[fallthrough]];
  case LA::INLINE:
    return LAK::INLINE;
  case LA::NO_MANGLE:
    [[fallthrough]];
  case LA::MANGLE:
    return LAK::MANGLE;
  case LA::NO_PACK:
    [[fallthrough]];
  case LA::PACK:
    return LAK::PACK;
  case LA::NO_BRANCH_TREND:
    [[fallthrough]];
  case LA::LIKELY:
    [[fallthrough]];
  case LA::UNLIKELY:
    return LAK::BRANCH_TREND;
  case LA::NO_SUPPORT_STATUS:
    [[fallthrough]];
  case LA::DEPRECIATED:
    [[fallthrough]];
  case LA::EXPERIMENTAL:
    return LAK::SUPPORT_STATUS;
  case LA::NO_STABLE_ADDRESS:
    [[fallthrough]];
  case LA::STABLE_ADDRESS:
    return LAK::STABLE_ADDRESS;
  case LA::NO_VARIADIC:
    [[fallthrough]];
  case LA::VARIADIC:
    return LAK::VARIADIC;
  case LA::NO_LOCATION:
    [[fallthrough]];
  case LA::LOCATION:
    return LAK::LOCATION;
  case LA::NO_TEMPLATE:
    [[fallthrough]];
  case LA::TEMPLATE:
    return LAK::TEMPLATE;
  case LA::NO_CONSTRAINT:
    [[fallthrough]];
  case LA::CONSTRAINT:
    return LAK::CONSTRAINT;
  case LA::NO_WEIGHT:
    [[fallthrough]];
  case LA::WEIGHT:
    return LAK::WEIGHT;
  case LA::NO_AUTO:
    [[fallthrough]];
  case LA::AUTO:
    return LAK::AUTO;
  case LA::NO_VIRTUAL:
    [[fallthrough]];
  case LA::VIRTUAL:
    return LAK::VIRTUAL;
  case LA::NO_RANGER:
    [[fallthrough]];
  case LA::RANGER:
    return LAK::RANGER;
  case LA::NO_REQUIRE:
    [[fallthrough]];
  case LA::REQUIRE:
    return LAK::REQUIRE;
  case LA::NO_ENSURE:
    [[fallthrough]];
  case LA::ENSURE:
    return LAK::ENSURE;
  case LA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

struct LowExpressionPair final {
  using Self = rq::LowExpressionPair;

  rq::Expression *_instantiation_ex_ptr{nullptr};
  rq::Expression *_attachment_ex_ptr{nullptr};

  explicit LowExpressionPair() = default;
  explicit LowExpressionPair(rq::Expression &instantiation_ex,
                             rq::Expression *attachment_ex_ptr)
      : _instantiation_ex_ptr(&instantiation_ex),
        _attachment_ex_ptr(attachment_ex_ptr) {}

  [[nodiscard]] bool getIsEmpty() const {
    return this->_instantiation_ex_ptr == nullptr;
  }

  [[nodiscard]] const rq::Expression &getInstantiationEx() const {
    return rq::dereferencePtr(this->_instantiation_ex_ptr);
  }

  [[nodiscard]] rq::Expression &getInstantiationEx() {
    return rq::dereferencePtr(this->_instantiation_ex_ptr);
  }

  [[nodiscard]] const rq::Expression *getAttachmentExPtr() const {
    return this->_attachment_ex_ptr;
  }

  [[nodiscard]] rq::Expression *getAttachmentExPtr() {
    return this->_attachment_ex_ptr;
  }
};

struct LowFactory final {
  using Self = rq::LowFactory;
  using PtrMap =
      llvm::SmallDenseMap<rq::LowAttributeKind, rq::LowExpressionPair>;

  rq::LowFuseFlags _flags{};
  PtrMap _ptr_map{};

  LowFactory() = default;
  LowFactory(const Self &) = delete;
  LowFactory(Self &&) = delete;
  ~LowFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_flags == rq::LowFuseFlags::NONE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getFuseFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAll(rq::LowFuseFlags flags) const {
    return rq::getHasAll(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSome(rq::LowFuseFlags flags) const {
    return rq::getHasSome(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNone(rq::LowFuseFlags flags) const {
    return rq::getHasNone(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowExpressionPair
  getExpressionPair(rq::LowAttributeKind kind) const {
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      return rq::LowExpressionPair();
    }
    return it->getSecond();
  }
  [[nodiscard]] inline bool addFlag(rq::LowAttribute attribute,
                                    rq::Expression &instantiation_ex,
                                    rq::Expression *attachment_ex_ptr) {
    const rq::LowFuseFlags flag = rq::getFuseFlags(attribute);
    this->_flags |= flag;
    const rq::LowAttributeKind kind = rq::getKind(attribute);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      this->_ptr_map.emplace_or_assign(
          kind, rq::LowExpressionPair(instantiation_ex, attachment_ex_ptr));
    } else {
      return false;
    }
    return true;
  }
};

enum class HighAttribute : std::uint_fast8_t {
  NONE,
  NO_VAR,
  VAR,
  PARTIAL_VAR,
  NO_VOLATILE,
  VOLATILE,
  NO_ATOMIC,
  ATOMIC,
  NO_NULL_TERMINATE,
  NULL_TERMINATE,
  NO_GREATEST,
  GREATEST
};

[[nodiscard]] inline llvm::StringRef getName(rq::HighAttribute attribute) {
  using namespace rq;
  using HA = HighAttribute;
  switch (attribute) {
  case HA::NONE:
    return "none";
  case HA::NO_VAR:
    return "no_var";
  case HA::VAR:
    return "var";
  case HA::PARTIAL_VAR:
    return "partial_var";
  case HA::NO_VOLATILE:
    return "no_volatile";
  case HA::VOLATILE:
    return "volatile";
  case HA::NO_ATOMIC:
    return "no_atomic";
  case HA::ATOMIC:
    return "atomic";
  case HA::NO_NULL_TERMINATE:
    return "no_null_terminate";
  case HA::NULL_TERMINATE:
    return "null_terminate";
  case HA::NO_GREATEST:
    return "no_greatest";
  case HA::GREATEST:
    return "greatest";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::HighAttribute getHighAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using HA = HighAttribute;
  switch (keyword) {
  case K::NO_VAR:
    return HA::NO_VAR;
  case K::VAR:
    return HA::VAR;
  case K::PARTIAL_VAR:
    return HA::PARTIAL_VAR;
  case K::NO_VOLATILE:
    return HA::NO_VOLATILE;
  case K::VOLATILE:
    return HA::VOLATILE;
  case K::NO_ATOMIC:
    return HA::NO_ATOMIC;
  case K::ATOMIC:
    return HA::ATOMIC;
  case K::NO_NULL_TERMINATE:
    return HA::NO_NULL_TERMINATE;
  case K::NULL_TERMINATE:
    return HA::NULL_TERMINATE;
  case K::NO_GREATEST:
    return HA::NO_GREATEST;
  case K::GREATEST:
    return HA::GREATEST;
  default:
    break;
  }
  return HA::NONE;
}

enum class HighFuseFlags : std::uint_fast8_t {
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

  GREATEST = rq::getBit(5),
  GREATEST_MASK = GREATEST
};

RQ_DEFINE_FLAGS(rq::HighFuseFlags);

[[nodiscard]] inline rq::HighFuseFlags
getFuseFlags(rq::HighAttribute attribute) {
  using namespace rq;
  using HA = HighAttribute;
  using HFF = HighFuseFlags;
  switch (attribute) {
  case HA::NONE:
    return HFF::NONE;
  case HA::NO_VAR:
    return HFF::NONE;
  case HA::VAR:
    return HFF::VAR;
  case HA::PARTIAL_VAR:
    return HFF::PARTIAL_VAR;
  case HA::NO_VOLATILE:
    return HFF::NONE;
  case HA::VOLATILE:
    return HFF::VOLATILE;
  case HA::NO_ATOMIC:
    return HFF::NONE;
  case HA::ATOMIC:
    return HFF::ATOMIC;
  case HA::NO_NULL_TERMINATE:
    return HFF::NONE;
  case HA::NULL_TERMINATE:
    return HFF::NULL_TERMINATE;
  case HA::NO_GREATEST:
    return HFF::NONE;
  case HA::GREATEST:
    return HFF::GREATEST;
  }
  RQ_UNREACHABLE();
}

enum class HighAttributeKind : std::uint_fast8_t {
  NONE,
  VAR,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATE,
  GREATEST
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::HighAttributeKind kind) {
  using HAK = rq::HighAttributeKind;
  switch (kind) {
  case HAK::NONE:
    return "none";
  case HAK::VAR:
    return "var attribute";
  case HAK::VOLATILE:
    return "volatile attribute";
  case HAK::ATOMIC:
    return "atomic attribute";
  case HAK::NULL_TERMINATE:
    return "null terminate attribute";
  case HAK::GREATEST:
    return "greatest";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::HighAttributeKind
getKind(rq::HighAttribute attribute) {
  using HA = rq::HighAttribute;
  using HAK = rq::HighAttributeKind;
  switch (attribute) {
  case HA::NONE:
    return HAK::NONE;
  case HA::NO_VAR:
    [[fallthrough]];
  case HA::VAR:
    [[fallthrough]];
  case HA::PARTIAL_VAR:
    return HAK::VAR;
  case HA::NO_VOLATILE:
    [[fallthrough]];
  case HA::VOLATILE:
    return HAK::VOLATILE;
  case HA::NO_ATOMIC:
    [[fallthrough]];
  case HA::ATOMIC:
    return HAK::ATOMIC;
  case HA::NO_NULL_TERMINATE:
    [[fallthrough]];
  case HA::NULL_TERMINATE:
    return HAK::NULL_TERMINATE;
  case HA::NO_GREATEST:
    [[fallthrough]];
  case HA::GREATEST:
    return HAK::GREATEST;
  }
  RQ_UNREACHABLE();
}

struct HighFactory final {
  using Self = rq::HighFactory;
  using PtrMap = llvm::SmallDenseMap<rq::HighAttributeKind, rq::Expression *>;

  rq::HighFuseFlags _flags{};
  PtrMap _ptr_map{};

  HighFactory() = default;
  HighFactory(const Self &) = delete;
  HighFactory(Self &&) = delete;
  ~HighFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::HighFuseFlags getFuseFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  getExpressionPtr(rq::HighAttributeKind kind) {
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      return nullptr;
    }
    return it->getSecond();
  }
  [[nodiscard]] inline bool addFlag(rq::HighAttribute attribute,
                                    rq::Expression *expression_ptr) {
    const rq::HighFuseFlags flag = rq::getFuseFlags(attribute);
    this->_flags |= flag;
    const rq::HighAttributeKind kind = rq::getKind(attribute);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      this->_ptr_map.emplace_or_assign(kind, expression_ptr);
    } else {
      return false;
    }
    return true;
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

enum class Chain : std::uint_fast8_t { NONE, IF, SWITCH, SPIN };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getDescription(rq::Chain chain) {
  using C = rq::Chain;
  switch (chain) {
  case C::NONE:
    return "no chain";
  case C::IF:
    return "if chain";
  case C::SWITCH:
    return "switch case chain";
  case C::SPIN:
    return "spin weave chain";
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

enum class ExpressionNextInfoFlags : std::uint8_t {
  NONE = 0,
  // NOTE: a "statement" expression is one that terminates with a semicolon
  STATEMENT = rq::getBit(0)
};

RQ_DEFINE_FLAGS(rq::ExpressionNextInfoFlags);

enum class ExpressionSourceInfoFlags : std::uint8_t {
  NONE = 0,
  // NOTE: an "inserted" node is one that was not present in the source
  // and was inserted into the AST by the compiler
  INSERTED = rq::getBit(0),
  // NOTE: this flag is set if there is an error with situating this node
  SITUATOR_ERROR = rq::getBit(1)
};

RQ_DEFINE_FLAGS(rq::ExpressionSourceInfoFlags);

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

  rq::PtrWithFlags<rq::Expression, 2, rq::ExpressionNextInfoFlags>
      _next_ptr_flags{};
  rq::Expression *_branch_ptr{nullptr};
  rq::PtrWithFlags<const char, 2, rq::ExpressionSourceInfoFlags>
      _source_ptr_flags{};
  unsigned _source_text_length{0};

  explicit Expression() : Entity(rq::getUnderlyingValue(rq::Keyword::NONE)) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHighAttribute() const {
    return rq::getIsHighAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLowAttribute() const {
    return rq::getIsLowAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getUniversalized() const {
    return rq::getUniversalized(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRailcar() const {
    return rq::getCanBeRailcar(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument() const {
    return rq::getCanBeArgument(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter() const {
    return rq::getCanBeParameter(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTupleElement() const {
    return rq::getCanBeTupleElement(this->getKeyword());
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
  getCanBeLowAttributeInstantiation() const {
    return rq::getCanBeLowAttributeInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeHighAttributeInstantiation() const {
    return rq::getCanBeHighAttributeInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticSequenceStage() const {
    return rq::getCanBeArithmeticSequenceStage(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowAttribute getLowAttribute() const {
    return rq::getLowAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::HighAttribute getHighAttribute() const {
    return rq::getHighAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInserted() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceInfoFlags::INSERTED);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSituatorError() const {
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceInfoFlags::SITUATOR_ERROR);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStatement() const {
    return rq::getHasAll(this->_next_ptr_flags.getFlags(),
                         rq::ExpressionNextInfoFlags::STATEMENT);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTag() const {
    return rq::getHasNone(this->_next_ptr_flags.getFlags(),
                          rq::ExpressionNextInfoFlags::STATEMENT);
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
  RQ_ALWAYS_INLINE void clearData() {
    this->_id = rq::getUnderlyingValue(rq::Keyword::NONE);
    this->_next_ptr_flags = {};
    this->_branch_ptr = nullptr;
    this->_source_ptr_flags = {};
    this->_source_text_length = 0;
  }
  RQ_ALWAYS_INLINE void setKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_id == rq::getUnderlyingValue(rq::Keyword::NONE),
              "keyword must not already be set");
#if !defined(_NDEBUG)
    this->_debug_keyword = keyword;
#endif
    this->_id = rq::getUnderlyingValue(keyword);
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_id != rq::getUnderlyingValue(rq::Keyword::NONE),
              "keyword must already be set");
#if !defined(_NDEBUG)
    this->_debug_keyword = keyword;
#endif
    this->_id = rq::getUnderlyingValue(keyword);
  }
  RQ_ALWAYS_INLINE void setIsInserted() {
    this->_source_ptr_flags.addFlags(rq::ExpressionSourceInfoFlags::INSERTED);
  }
  RQ_ALWAYS_INLINE void setHasSituatorError() {
    this->_source_ptr_flags.addFlags(
        rq::ExpressionSourceInfoFlags::SITUATOR_ERROR);
  }
  void RQ_ALWAYS_INLINE setIsStatement() {
    this->_next_ptr_flags.addFlags(rq::ExpressionNextInfoFlags::STATEMENT);
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
    const bool was_statement = this->getIsStatement();
    this->clearData();
    this->setKeyword(branch.getKeyword());
    if (branch.getHasBranch()) {
      this->setBranch(branch.popBranch());
    }
    if (branch.getHasNext()) {
      this->setNext(branch.popNext());
    }
    this->setSource(branch);
    if (was_statement) {
      this->setIsStatement();
    }
    branch.clearData();
    return branch;
  }
  [[nodiscard]] inline rq::Expression &mergeAndPopNext() {
    rq::Expression &next = this->popNext();
    RQ_ASSERT(!this->getHasBranch(), "has branch");
    const bool was_statement = this->getIsStatement();
    this->clearData();
    this->setKeyword(next.getKeyword());
    if (next.getHasBranch()) {
      this->setBranch(next.popBranch());
    }
    if (next.getHasNext()) {
      this->setNext(next.popNext());
    }
    this->setSource(next);
    if (was_statement) {
      this->setIsStatement();
    }
    next.clearData();
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ExpressionIterator>
  getInclusiveNextSubrange() {
    return rq::Subrange<rq::ExpressionIterator>(rq::ExpressionIterator(this),
                                                rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstExpressionIterator>
  getInclusiveNextSubrange() const {
    return rq::Subrange<rq::ConstExpressionIterator>(
        rq::ConstExpressionIterator(this), rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ExpressionIterator>
  getNextSubrange() {
    return rq::Subrange(rq::ExpressionIterator(this->getNextPtr()),
                        rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstExpressionIterator>
  getNextSubrange() const {
    return rq::Subrange<rq::ConstExpressionIterator>(
        rq::ConstExpressionIterator(this->getNextPtr()),
        rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ExpressionIterator>
  getBranchSubrange() {
    return rq::Subrange<rq::ExpressionIterator>(
        rq::ExpressionIterator(this->getBranchPtr()), rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstExpressionIterator>
  getBranchSubrange() const {
    return rq::Subrange<rq::ConstExpressionIterator>(
        rq::ConstExpressionIterator(this->getBranchPtr()),
        rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstExpressionIterator>
  getConstBranchSubrange() const {
    return rq::Subrange<rq::ConstExpressionIterator>(
        rq::ConstExpressionIterator(this->getBranchPtr()),
        rq::ConstExpressionIterator());
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
    const rq::EntityId id = entity.getId();
    return id >= rq::KEYWORD_OFFSET && id < rq::SYMBOL_OFFSET;
  }

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const {
    inout_id.AddInteger(rq::getUnderlyingValue(this->getKeyword()));
    if (this->getIsStatement()) {
      inout_id.AddInteger(1);
    } else {
      inout_id.AddInteger(0);
    }
    if (this->getHasBranch()) {
      inout_id.Add(this->getBranch());
    } else {
      inout_id.AddInteger(0);
    }
    if (this->getHasNext()) {
      inout_id.Add(this->getNext());
    } else {
      inout_id.AddInteger(0);
    }
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