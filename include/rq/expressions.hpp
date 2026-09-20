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
  case K::UNSITUATED_ASCRIBE_MODIFIER:
    return "_unsituated_ascribe_modifier";
  case K::UNSITUATED_ASCRIBE_QUALIFIER:
    return "_unsituated_ascribe_qualifier";
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
  case K::ASCRIBE_QUALIFIER:
    return "_ascribe_qualifier";
  case K::ASCRIBE_MODIFIER:
    return "_ascribe_modifier";
  case K::ASCRIBE_RECIEVER_QUALIFIER:
    return "_ascribe_reciever_qualifier";
  case K::INSTANTIATE_MODIFIER:
    return "_instantiate_modifier";
  case K::INSTANTIATE_QUALIFIER:
    return "instantiate_qualifier";
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
  case K::FORGET:
    return "forget";
  case K::FORGET_OF:
    return "_forget_of";
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
  case K::INSTANTIATE_GREATEST:
    return "_instantiate_greatest";

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
  case K::STELLARSCOPE:
    return "_stellarscope";
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

  // MODIFIERS
  case K::NO_MODIFIER:
    return "no_modifier";
  case K::ANCHOR:
    return "anchor";
  case K::RESIDENT:
    return "resident";
  case K::FLANK:
    return "flank";
  case K::TRANSPARENT:
    return "transparent";
  case K::OPAQUE:
    return "opaque";
  case K::PRIVATE:
    return "private";
  case K::PUBLIC:
    return "public";
  case K::EXPORT:
    return "export";
  case K::MUTABLE:
    return "mutable";
  case K::PARTIALLY_MUTABLE:
    return "partially_mutable";
  case K::CONSTANT:
    return "constant";
  case K::DYNAMIC:
    return "dynamic";
  case K::STATIC:
    return "static";
  case K::RUNTIME:
    return "runtime";
  case K::COMPILE_TIME:
    return "compile_time";
  case K::HYBRID:
    return "hybrid";
  case K::EAGER:
    return "eager";
  case K::LAZY:
    return "lazy";
  case K::PREDEFINED:
    return "predefined";
  case K::SINGLETON:
    return "singleton";
  case K::CAPTURE:
    return "capture";
  case K::LINKED:
    return "linked";
  case K::INLINE:
    return "inline";
  case K::STANDARD_MANGLE:
    return "standard_mangle";
  case K::MANGLE:
    return "mangle";
  case K::PAD:
    return "pad";
  case K::PACK:
    return "pack";
  case K::EQUIVOCAL:
    return "equivocal";
  case K::LIKELY:
    return "likely";
  case K::UNLIKELY:
    return "unlikely";
  case K::SUPPORTED:
    return "supported";
  case K::DEPRECIATED:
    return "depreciated";
  case K::EXPERIMENTAL:
    return "experimental";
  case K::UNSTABLE_ADDRESS:
    return "unstable_address";
  case K::STABLE_ADDRESS:
    return "stable_address";
  case K::INVARIADIC:
    return "invariadic";
  case K::VARIADIC:
    return "variadic";
  case K::BEST_LOCATION:
    return "best_location";
  case K::LOCATION:
    return "location";
  case K::TEMPLATE:
    return "template";
  case K::OVERLOAD:
    return "overload";
  case K::CONSTRAINT:
    return "constraint";
  case K::DEFAULT_WEIGHT:
    return "default_weight";
  case K::WEIGHT:
    return "weight";
  case K::MANUAL:
    return "manual";
  case K::AUTO:
    return "auto";
  case K::DIRECT:
    return "direct";
  case K::VIRTUAL:
    return "virtual";
  case K::RANGER:
    return "ranger";
  case K::REQUIRE:
    return "require";
  case K::ENSURE:
    return "ensure";

  // TYPE MODIFIERS
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

  // MODIFIER TYPES
  case K::MODIFIER:
    return "modifier";

  // QUALIFIER TYPES
  case K::VAR_QUALIFIER:
    return "var_qualifier";
  case K::VOLATILE_QUALIFIER:
    return "volatile_qualifier";
  case K::ATOMIC_QUALIFIER:
    return "atomic_qualifier";
  case K::NULL_TERMINATE_QUALIFIER:
    return "null_terminate_qualifier";

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
  case K::AT:
    return "at";
  case K::AT_OF:
    return "_at_of";
  case K::MAIN:
    return "main";
  case K::MAIN_OF:
    return "main_of";
  case K::DESTRUCTOR:
    return "destructor";
  case K::DESTRUCTOR_OF:
    return "_destructor_of";
  case K::DESTROY:
    return "destroy";
  case K::DESTROY_OF:
    return "_destroy_of";
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
  case K::RESOLVE_FUNCTION:
    return "resolve_function";
  case K::RESOLVE_FUNCTION_OF:
    return "_resolve_function_of";
  case K::RESOLVE_ADAPTER:
    return "resolve_adapter";
  case K::RESOLVE_ADAPTER_OF:
    return "_resolve_adapter_of";
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
  case K::IS_QUALIFIER_TYPE:
    return "is_qualifier_type";
  case K::IS_QUALIFIER_TYPE_OF:
    return "_is_qualifier_type_of";

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
  MODIFIER = rq::getBit(22),
  QUALIFIER = rq::getBit(23),
  ARITHMETIC_SEQUENCE_STEP = rq::getBit(24),
  ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(25),
  ALL_SITUATIONS = STATEMENT | RVALUE | LVALUE | RAILCAR | ARGUMENT |
      PARAMETER | BINDING | NAME | NAMESPACE | ASCRIPTION | MODIFIER |
      QUALIFIER | ARITHMETIC_SEQUENCE_STEP | ARITHMETIC_SEQUENCE_CONDITION,

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
    // modifiers do not get confused with _call (and also this would be weird).
    return KIF::CONVERGING | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::LVALUE | KIF::NAME | KIF::NAMESPACE |
           KIF::ARITHMETIC_SEQUENCE_STEP | KIF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::UNSITUATED_EQUAL_OPERATOR:
    return KIF::STATEMENT | KIF::ARGUMENT | KIF::PARAMETER | KIF::TUPLE_ELEMENT;
  case K::UNSITUATED_ASCRIBE_MODIFIER:
    return KIF::STATEMENT | KIF::RVALUE | KIF::PARAMETER | KIF::ARGUMENT |
           KIF::TUPLE_ELEMENT | KIF::ASCRIPTION;
  case K::UNSITUATED_ASCRIBE_QUALIFIER:
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
  case K::ASCRIBE_QUALIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::RAILCAR |
           KIF::ASCRIPTION;
  case K::ASCRIBE_MODIFIER:
    return KIF::STATEMENT | KIF::PARAMETER;
  case K::ASCRIBE_RECIEVER_QUALIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT | KIF::ASCRIPTION;
  case K::INSTANTIATE_MODIFIER:
    return KIF::NONE; // MODIFIER_INSTANTIATION
  case K::INSTANTIATE_QUALIFIER:
    return KIF::NONE; // QUALIFIER_INSTANTIATION
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
  case K::FORGET:
    return KIF::RAILCAR;
  case K::FORGET_OF:
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
  case K::INSTANTIATE_GREATEST:
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
  case K::STELLARSCOPE:
    return KIF::ARGUMENT | KIF::RVALUE | KIF::TUPLE_ELEMENT;
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

  // MODIFIERS
  case K::NO_MODIFIER:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ANCHOR:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RESIDENT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::FLANK:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TRANSPARENT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::OPAQUE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PRIVATE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PUBLIC:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::EXPORT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MUTABLE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PARTIALLY_MUTABLE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CONSTANT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DYNAMIC:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::STATIC:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RUNTIME:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::COMPILE_TIME:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::HYBRID:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::EAGER:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LAZY:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PREDEFINED:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SINGLETON:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::CAPTURE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LINKED:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INLINE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::STANDARD_MANGLE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MANGLE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PAD:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PACK:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::EQUIVOCAL:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LIKELY:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNLIKELY:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::SUPPORTED:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DEPRECIATED:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::EXPERIMENTAL:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::UNSTABLE_ADDRESS:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::STABLE_ADDRESS:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::INVARIADIC:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VARIADIC:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::BEST_LOCATION:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::LOCATION:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::TEMPLATE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::RAILCAR;
  case K::OVERLOAD:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::RAILCAR;
  case K::CONSTRAINT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DEFAULT_WEIGHT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::WEIGHT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT |
           KIF::RAILCAR;
  case K::MANUAL:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::AUTO:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DIRECT:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VIRTUAL:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::RANGER:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::REQUIRE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ENSURE:
    return KIF::MODIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // QUALIFIERS
  case K::NO_VAR:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VAR:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::PARTIAL_VAR:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NO_VOLATILE:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VOLATILE:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NO_ATOMIC:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ATOMIC:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NO_NULL_TERMINATE:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NULL_TERMINATE:
    return KIF::QUALIFIER | KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // MODIFIER TYPES
  case K::MODIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;

  // QUALIFIER TYPES
  case K::VAR_QUALIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::VOLATILE_QUALIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::ATOMIC_QUALIFIER:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::NULL_TERMINATE_QUALIFIER:
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
  case K::AT:
    return KIF::RAILCAR;
  case K::AT_OF:
    return KIF::RVALUE | KIF::LVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::MAIN:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR | KIF::STATEMENT;
  case K::MAIN_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DESTRUCTOR:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::RAILCAR | KIF::STATEMENT;
  case K::DESTRUCTOR_OF:
    return KIF::RVALUE | KIF::ARGUMENT | KIF::TUPLE_ELEMENT;
  case K::DESTROY:
    return KIF::RAILCAR;
  case K::DESTROY_OF:
    return KIF::STATEMENT | KIF::TUPLE_ELEMENT;
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
  case K::IS_QUALIFIER_TYPE:
    return KIF::RAILCAR;
  case K::IS_QUALIFIER_TYPE_OF:
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
  case rq::Keyword::UNSITUATED_ASCRIBE_MODIFIER:
    return rq::Keyword::ASCRIBE_MODIFIER;
  case rq::Keyword::UNSITUATED_ASCRIBE_QUALIFIER:
    return rq::Keyword::ASCRIBE_QUALIFIER;
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsQualifier(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::QUALIFIER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsModifier(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordInfoFlags::MODIFIER);
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
  MODIFIER_INSTANTIATION,
  QUALIFIER_INSTANTIATION,
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
  case S::MODIFIER_INSTANTIATION:
    return "modifier instantiation";
  case S::QUALIFIER_INSTANTIATION:
    return "qualifier instantiation";
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
  case K::FORGET:
    return K::FORGET_OF;
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
  case K::AT:
    return K::AT_OF;
  case K::MAIN:
    return K::MAIN_OF;
  case K::DESTRUCTOR:
    return K::DESTRUCTOR_OF;
  case K::DESTROY:
    return K::DESTROY_OF;
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
  case K::IS_QUALIFIER_TYPE:
    return K::IS_QUALIFIER_TYPE_OF;
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
getCanBeModifierInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_MODIFIER;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeQualifierInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_QUALIFIER;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStage(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasSome(flags,
                        rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_CONDITION |
                            rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticCondition(rq::Keyword keyword) {
  const rq::KeywordInfoFlags flags = rq::getInfoFlags(keyword);
  return rq::getHasAll(flags,
                       rq::KeywordInfoFlags::ARITHMETIC_SEQUENCE_CONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticStep(rq::Keyword keyword) {
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
  case S::MODIFIER_INSTANTIATION:
    return rq::getCanBeModifierInstantiation(keyword);
  case S::QUALIFIER_INSTANTIATION:
    return rq::getCanBeQualifierInstantiation(keyword);
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

enum class Modifier : std::uint_fast8_t {
  NO_MODIFIER,
  // anchor
  ANCHOR,
  // container
  RESIDENT,
  FLANK,
  // visibility
  TRANSPARENT,
  OPAQUE,
  // access
  PRIVATE,
  PUBLIC,
  EXPORT,
  // mutability
  MUTABLE,
  PARTIALLY_MUTABLE,
  CONSTANT,
  // generation_time
  DYNAMIC,
  STATIC,
  // execution_time
  RUNTIME,
  COMPILE_TIME,
  HYBRID,
  // evaluation_time
  EAGER,
  LAZY,
  // initialization_time
  PREDEFINED,
  SINGLETON,
  // capture
  CAPTURE,
  // linkage
  LINKED,
  INLINE,
  // mangle
  STANDARD_MANGLE,
  MANGLE,
  // pack
  PAD,
  PACK,
  // branch_trend
  EQUIVOCAL,
  LIKELY,
  UNLIKELY,
  // support_notice
  SUPPORTED,
  DEPRECIATED,
  EXPERIMENTAL,
  // address_stability
  UNSTABLE_ADDRESS,
  STABLE_ADDRESS,
  // variadic
  INVARIADIC,
  VARIADIC,
  // offset
  BEST_LOCATION,
  LOCATION,
  // declaration
  TEMPLATE,
  OVERLOAD,
  // constraint
  CONSTRAINT,
  // weight
  DEFAULT_WEIGHT,
  WEIGHT,
  // deduction
  MANUAL,
  AUTO,
  // virtuality
  DIRECT,
  VIRTUAL,
  // ranger
  RANGER,
  // require
  REQUIRE,
  // ensure
  ENSURE,
  LAST
};

[[nodiscard]] inline llvm::StringRef getName(rq::Modifier modifier) {
  using namespace rq;
  using M = Modifier;
  switch (modifier) {
  case M::NO_MODIFIER:
    return "NO_MODIFIER";
  case M::ANCHOR:
    return "ANCHOR";
  case M::RESIDENT:
    return "RESIDENT";
  case M::FLANK:
    return "FLANK";
  case M::TRANSPARENT:
    return "TRANSPARENT";
  case M::OPAQUE:
    return "OPAQUE";
  case M::PRIVATE:
    return "PRIVATE";
  case M::PUBLIC:
    return "PUBLIC";
  case M::EXPORT:
    return "EXPORT";
  case M::MUTABLE:
    return "MUTABLE";
  case M::PARTIALLY_MUTABLE:
    return "PARTIALLY_MUTABLE";
  case M::CONSTANT:
    return "CONSTANT";
  case M::DYNAMIC:
    return "DYNAMIC";
  case M::STATIC:
    return "STATIC";
  case M::RUNTIME:
    return "RUNTIME";
  case M::COMPILE_TIME:
    return "COMPILE_TIME";
  case M::HYBRID:
    return "hybrid";
  case M::EAGER:
    return "EAGER";
  case M::LAZY:
    return "LAZY";
  case M::PREDEFINED:
    return "PREDEFINED";
  case M::SINGLETON:
    return "SINGLETON";
  case M::CAPTURE:
    return "CAPTURE";
  case M::LINKED:
    return "LINKED";
  case M::INLINE:
    return "INLINE";
  case M::STANDARD_MANGLE:
    return "STANDARD_MANGLE";
  case M::MANGLE:
    return "MANGLE";
  case M::PAD:
    return "PAD";
  case M::PACK:
    return "PACK";
  case M::EQUIVOCAL:
    return "EQUIVOCAL";
  case M::LIKELY:
    return "LIKELY";
  case M::UNLIKELY:
    return "UNLIKELY";
  case M::SUPPORTED:
    return "SUPPORTED";
  case M::DEPRECIATED:
    return "DEPRECIATED";
  case M::EXPERIMENTAL:
    return "EXPERIMENTAL";
  case M::UNSTABLE_ADDRESS:
    return "UNSTABLE_ADDRESS";
  case M::STABLE_ADDRESS:
    return "STABLE_ADDRESS";
  case M::INVARIADIC:
    return "INVARIADIC";
  case M::VARIADIC:
    return "VARIADIC";
  case M::BEST_LOCATION:
    return "BEST_LOCATION";
  case M::LOCATION:
    return "LOCATION";
  case M::TEMPLATE:
    return "TEMPLATE";
  case M::OVERLOAD:
    return "OVERLOAD";
  case M::CONSTRAINT:
    return "CONSTRAINT";
  case M::DEFAULT_WEIGHT:
    return "DEFAULT_WEIGHT";
  case M::WEIGHT:
    return "WEIGHT";
  case M::MANUAL:
    return "MANUAL";
  case M::AUTO:
    return "AUTO";
  case M::DIRECT:
    return "DIRECT";
  case M::VIRTUAL:
    return "VIRTUAL";
  case M::RANGER:
    return "RANGER";
  case M::REQUIRE:
    return "REQUIRE";
  case M::ENSURE:
    return "ENSURE";
  case M::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Modifier getModifier(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using M = Modifier;
  switch (keyword) {
  case K::NO_MODIFIER:
    return M::NO_MODIFIER;
  case K::ANCHOR:
    return M::ANCHOR;
  case K::RESIDENT:
    return M::RESIDENT;
  case K::FLANK:
    return M::FLANK;
  case K::TRANSPARENT:
    return M::TRANSPARENT;
  case K::OPAQUE:
    return M::OPAQUE;
  case K::PRIVATE:
    return M::PRIVATE;
  case K::PUBLIC:
    return M::PUBLIC;
  case K::EXPORT:
    return M::EXPORT;
  case K::MUTABLE:
    return M::MUTABLE;
  case K::PARTIALLY_MUTABLE:
    return M::PARTIALLY_MUTABLE;
  case K::CONSTANT:
    return M::CONSTANT;
  case K::DYNAMIC:
    return M::DYNAMIC;
  case K::STATIC:
    return M::STATIC;
  case K::RUNTIME:
    return M::RUNTIME;
  case K::COMPILE_TIME:
    return M::COMPILE_TIME;
  case K::HYBRID:
    return M::HYBRID;
  case K::EAGER:
    return M::EAGER;
  case K::LAZY:
    return M::LAZY;
  case K::PREDEFINED:
    return M::PREDEFINED;
  case K::SINGLETON:
    return M::SINGLETON;
  case K::CAPTURE:
    return M::CAPTURE;
  case K::LINKED:
    return M::LINKED;
  case K::INLINE:
    return M::INLINE;
  case K::STANDARD_MANGLE:
    return M::STANDARD_MANGLE;
  case K::MANGLE:
    return M::MANGLE;
  case K::PAD:
    return M::PAD;
  case K::PACK:
    return M::PACK;
  case K::EQUIVOCAL:
    return M::EQUIVOCAL;
  case K::LIKELY:
    return M::LIKELY;
  case K::UNLIKELY:
    return M::UNLIKELY;
  case K::SUPPORTED:
    return M::SUPPORTED;
  case K::DEPRECIATED:
    return M::DEPRECIATED;
  case K::EXPERIMENTAL:
    return M::EXPERIMENTAL;
  case K::UNSTABLE_ADDRESS:
    return M::UNSTABLE_ADDRESS;
  case K::STABLE_ADDRESS:
    return M::STABLE_ADDRESS;
  case K::INVARIADIC:
    return M::INVARIADIC;
  case K::VARIADIC:
    return M::VARIADIC;
  case K::BEST_LOCATION:
    return M::BEST_LOCATION;
  case K::LOCATION:
    return M::LOCATION;
  case K::TEMPLATE:
    return M::TEMPLATE;
  case K::OVERLOAD:
    return M::OVERLOAD;
  case K::CONSTRAINT:
    return M::CONSTRAINT;
  case K::DEFAULT_WEIGHT:
    return M::DEFAULT_WEIGHT;
  case K::WEIGHT:
    return M::WEIGHT;
  case K::MANUAL:
    return M::MANUAL;
  case K::AUTO:
    return M::AUTO;
  case K::DIRECT:
    return M::DIRECT;
  case K::VIRTUAL:
    return M::VIRTUAL;
  case K::RANGER:
    return M::RANGER;
  case K::REQUIRE:
    return M::REQUIRE;
  case K::ENSURE:
    return M::ENSURE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class ModifierFuseFlags : std::uint_fast32_t {
  NONE = 0,

  NO_MODIFIER = 0,

  ANCHOR = rq::getBit(0),
  ANCHOR_MODIFIER_MASK = ANCHOR,

  RESIDENT = rq::getBit(1),
  FLANK = rq::getBit(2),
  CONTAINER_MODIFIER_MASK = RESIDENT | FLANK,

  TRANSPARENT = rq::getBit(3),
  OPAQUE = rq::getBit(4),
  VISIBILITY_MODIFIER_MASK = TRANSPARENT | OPAQUE,

  PRIVATE = rq::getBit(5),
  PUBLIC = rq::getBit(6),
  EXPORT = rq::getBit(7),
  ACCESS_MODIFIER_MASK = PRIVATE | PUBLIC | EXPORT,

  MUTABLE = rq::getBit(8),
  PARTIALLY_MUTABLE = rq::getBit(9),
  CONSTANT = rq::getBit(10),
  MUTATE_MODIFIER_MASK = MUTABLE | PARTIALLY_MUTABLE | CONSTANT,

  DYNAMIC = rq::getBit(11),
  STATIC = rq::getBit(12),
  GENERATION_TIME_MASK = DYNAMIC | STATIC,

  RUNTIME = rq::getBit(13),
  COMPILE_TIME = rq::getBit(14),
  HYBRID = rq::getBit(15),
  EXECUTION_TIME_MASK = RUNTIME | COMPILE_TIME | HYBRID,

  EAGER = rq::getBit(15),
  LAZY = rq::getBit(16),
  EVALUATION_TIME_MASK = EAGER | LAZY,

  PREDEFINED = rq::getBit(17),
  SINGLETON = rq::getBit(18),
  INITIALIZATION_TIME_MASK = PREDEFINED | SINGLETON,

  CAPTURE = rq::getBit(19),
  CAPTURE_MODIFIER_MASK = CAPTURE,

  LINKED = rq::getBit(20),
  INLINE = rq::getBit(21),
  LINKAGE_MODIFIER_MASK = LINKED | INLINE,

  STANDARD_MANGLE = rq::getBit(22),
  MANGLE = rq::getBit(23),
  MANGLE_MODIFIER_MASK = STANDARD_MANGLE | MANGLE,

  PAD = rq::getBit(24),
  PACK = rq::getBit(25),
  PACK_MODIFIER_MASK = PAD | PACK,

  EQUIVOCAL = rq::getBit(26),
  LIKELY = rq::getBit(27),
  UNLIKELY = rq::getBit(28),
  BRANCH_TREND_MODIFIER_MASK = EQUIVOCAL | LIKELY | UNLIKELY,

  SUPPORTED = rq::getBit(29),
  DEPRECIATED = rq::getBit(30),
  EXPERIMENTAL = rq::getBit(31),
  SUPPORT_NOTICE_MODIFIER_MASK = SUPPORTED | DEPRECIATED | EXPERIMENTAL,

  UNSTABLE_ADDRESS = rq::getBit(32),
  STABLE_ADDRESS = rq::getBit(33),
  ADDRESS_STABILITY_MODIFIER_MASK = UNSTABLE_ADDRESS | STABLE_ADDRESS,

  INVARIADIC = rq::getBit(34),
  VARIADIC = rq::getBit(35),
  VARIADIC_MODIFIER_MASK,

  BEST_LOCATION = rq::getBit(36),
  LOCATION = rq::getBit(37),
  OFFSET_MODIFIER_MASK = BEST_LOCATION | LOCATION,

  TEMPLATE = rq::getBit(38),
  OVERLOAD = rq::getBit(39),
  LAZY_DECLARATION_KIND_MODIFIER_MASK = TEMPLATE | OVERLOAD,

  CONSTRAINT = rq::getBit(40),
  CONSTRAINT_MODIFIER_MASK = CONSTRAINT,

  DEFAULT_WEIGHT = rq::getBit(41),
  WEIGHT = rq::getBit(42),
  WEIGHT_MODIFIER_MASK = DEFAULT_WEIGHT | WEIGHT,

  MANUAL = rq::getBit(43),
  AUTO = rq::getBit(44),
  DEDUCTION_MODIFIER_MASK = MANUAL | AUTO,

  DIRECT = rq::getBit(45),
  VIRTUAL = rq::getBit(46),
  VIRTUALITY_MODIFIER_MASK = DIRECT | VIRTUAL,

  RANGER = rq::getBit(47),
  RANGER_MODIFIER_MASK = RANGER,

  REQUIRE = rq::getBit(48),
  REQUIRE_MODIFIER_MASK = REQUIRE,

  ENSURE = rq::getBit(49),
  ENSURE_MODIFIER_MASK = ENSURE
};

RQ_DEFINE_FLAGS(rq::ModifierFuseFlags);

[[nodiscard]] inline rq::ModifierFuseFlags getFuseFlags(rq::Modifier modifier) {
  using namespace rq;
  using M = Modifier;
  using MFF = ModifierFuseFlags;
  switch (modifier) {
  case M::NO_MODIFIER:
    return MFF::NO_MODIFIER;
  case M::ANCHOR:
    return MFF::ANCHOR;
  case M::RESIDENT:
    return MFF::RESIDENT;
  case M::FLANK:
    return MFF::FLANK;
  case M::TRANSPARENT:
    return MFF::TRANSPARENT;
  case M::OPAQUE:
    return MFF::OPAQUE;
  case M::PRIVATE:
    return MFF::PRIVATE;
  case M::PUBLIC:
    return MFF::PUBLIC;
  case M::EXPORT:
    return MFF::EXPORT;
  case M::MUTABLE:
    return MFF::MUTABLE;
  case M::PARTIALLY_MUTABLE:
    return MFF::PARTIALLY_MUTABLE;
  case M::CONSTANT:
    return MFF::CONSTANT;
  case M::DYNAMIC:
    return MFF::DYNAMIC;
  case M::STATIC:
    return MFF::STATIC;
  case M::RUNTIME:
    return MFF::RUNTIME;
  case M::COMPILE_TIME:
    return MFF::COMPILE_TIME;
  case M::HYBRID:
    return MFF::HYBRID;
  case M::EAGER:
    return MFF::EAGER;
  case M::LAZY:
    return MFF::LAZY;
  case M::PREDEFINED:
    return MFF::PREDEFINED;
  case M::SINGLETON:
    return MFF::SINGLETON;
  case M::CAPTURE:
    return MFF::CAPTURE;
  case M::LINKED:
    return MFF::LINKED;
  case M::INLINE:
    return MFF::INLINE;
  case M::STANDARD_MANGLE:
    return MFF::STANDARD_MANGLE;
  case M::MANGLE:
    return MFF::MANGLE;
  case M::PAD:
    return MFF::PAD;
  case M::PACK:
    return MFF::PACK;
  case M::EQUIVOCAL:
    return MFF::EQUIVOCAL;
  case M::LIKELY:
    return MFF::LIKELY;
  case M::UNLIKELY:
    return MFF::UNLIKELY;
  case M::SUPPORTED:
    return MFF::SUPPORTED;
  case M::DEPRECIATED:
    return MFF::DEPRECIATED;
  case M::EXPERIMENTAL:
    return MFF::EXPERIMENTAL;
  case M::UNSTABLE_ADDRESS:
    return MFF::UNSTABLE_ADDRESS;
  case M::STABLE_ADDRESS:
    return MFF::STABLE_ADDRESS;
  case M::INVARIADIC:
    return MFF::INVARIADIC;
  case M::VARIADIC:
    return MFF::VARIADIC;
  case M::BEST_LOCATION:
    return MFF::BEST_LOCATION;
  case M::LOCATION:
    return MFF::LOCATION;
  case M::TEMPLATE:
    return MFF::TEMPLATE;
  case M::OVERLOAD:
    return MFF::OVERLOAD;
  case M::CONSTRAINT:
    return MFF::CONSTRAINT;
  case M::DEFAULT_WEIGHT:
    return MFF::DEFAULT_WEIGHT;
  case M::WEIGHT:
    return MFF::WEIGHT;
  case M::MANUAL:
    return MFF::MANUAL;
  case M::AUTO:
    return MFF::AUTO;
  case M::DIRECT:
    return MFF::DIRECT;
  case M::VIRTUAL:
    return MFF::VIRTUAL;
  case M::RANGER:
    return MFF::RANGER;
  case M::REQUIRE:
    return MFF::REQUIRE;
  case M::ENSURE:
    return MFF::ENSURE;
  case M::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

enum class ModifierInfoFlags : std::uint_fast8_t {
  NONE = 0,

  ATTACHMENT = rq::getBit(0),
  NO_ATTACHMENT = rq::getBit(1)
};

RQ_DEFINE_FLAGS(rq::ModifierInfoFlags);

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierInfoFlags
getInfoFlags(rq::Modifier modifier) {
  using M = rq::Modifier;
  using MIF = rq::ModifierInfoFlags;
  switch (modifier) {
  case M::NO_MODIFIER:
    return MIF::NO_ATTACHMENT;
  case M::ANCHOR:
    return MIF::ATTACHMENT;
  case M::RESIDENT:
    return MIF::NO_ATTACHMENT;
  case M::FLANK:
    return MIF::ATTACHMENT;
  case M::TRANSPARENT:
    return MIF::NO_ATTACHMENT;
  case M::OPAQUE:
    return MIF::NO_ATTACHMENT;
  case M::PRIVATE:
    return MIF::NO_ATTACHMENT;
  case M::PUBLIC:
    return MIF::NO_ATTACHMENT;
  case M::EXPORT:
    return MIF::NO_ATTACHMENT;
  case M::MUTABLE:
    return MIF::NO_ATTACHMENT;
  case M::PARTIALLY_MUTABLE:
    return MIF::NO_ATTACHMENT;
  case M::CONSTANT:
    return MIF::NO_ATTACHMENT;
  case M::DYNAMIC:
    return MIF::NO_ATTACHMENT;
  case M::STATIC:
    return MIF::NO_ATTACHMENT;
  case M::RUNTIME:
    return MIF::NO_ATTACHMENT;
  case M::COMPILE_TIME:
    return MIF::NO_ATTACHMENT;
  case M::HYBRID:
    return MIF::NO_ATTACHMENT;
  case M::EAGER:
    return MIF::NO_ATTACHMENT;
  case M::LAZY:
    return MIF::NO_ATTACHMENT;
  case M::PREDEFINED:
    return MIF::NO_ATTACHMENT;
  case M::SINGLETON:
    return MIF::NO_ATTACHMENT;
  case M::CAPTURE:
    return MIF::ATTACHMENT;
  case M::LINKED:
    return MIF::NO_ATTACHMENT;
  case M::INLINE:
    return MIF::NO_ATTACHMENT;
  case M::STANDARD_MANGLE:
    return MIF::NO_ATTACHMENT;
  case M::MANGLE:
    return MIF::ATTACHMENT;
  case M::PAD:
    return MIF::NO_ATTACHMENT;
  case M::PACK:
    return MIF::NO_ATTACHMENT;
  case M::EQUIVOCAL:
    return MIF::NO_ATTACHMENT;
  case M::LIKELY:
    return MIF::NO_ATTACHMENT;
  case M::UNLIKELY:
    return MIF::NO_ATTACHMENT;
  case M::SUPPORTED:
    return MIF::NO_ATTACHMENT;
  case M::DEPRECIATED:
    return MIF::ATTACHMENT;
  case M::EXPERIMENTAL:
    return MIF::ATTACHMENT;
  case M::UNSTABLE_ADDRESS:
    return MIF::NO_ATTACHMENT;
  case M::STABLE_ADDRESS:
    return MIF::NO_ATTACHMENT;
  case M::INVARIADIC:
    return MIF::NO_ATTACHMENT;
  case M::VARIADIC:
    return MIF::NO_ATTACHMENT;
  case M::BEST_LOCATION:
    return MIF::NO_ATTACHMENT;
  case M::LOCATION:
    return MIF::ATTACHMENT;
  case M::TEMPLATE:
    return MIF::ATTACHMENT;
  case M::OVERLOAD:
    return MIF::NO_ATTACHMENT;
  case M::CONSTRAINT:
    return MIF::ATTACHMENT;
  case M::DEFAULT_WEIGHT:
    return MIF::NO_ATTACHMENT;
  case M::WEIGHT:
    return MIF::ATTACHMENT;
  case M::MANUAL:
    return MIF::NO_ATTACHMENT;
  case M::AUTO:
    return MIF::NO_ATTACHMENT;
  case M::DIRECT:
    return MIF::NO_ATTACHMENT;
  case M::VIRTUAL:
    return MIF::NO_ATTACHMENT;
  case M::RANGER:
    return MIF::NO_ATTACHMENT;
  case M::REQUIRE:
    return MIF::NO_ATTACHMENT;
  case M::ENSURE:
    return MIF::NO_ATTACHMENT;
  case M::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMustHaveAttachment(rq::Modifier modifier) {
  rq::ModifierInfoFlags flags = rq::getInfoFlags(modifier);
  return rq::getHasAll(flags, rq::ModifierInfoFlags::ATTACHMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMustNotHaveAttachment(rq::Modifier modifier) {
  rq::ModifierInfoFlags flags = rq::getInfoFlags(modifier);
  return rq::getHasAll(flags, rq::ModifierInfoFlags::NO_ATTACHMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getMayOrMayNotHaveAttachment(rq::Modifier modifier) {
  rq::ModifierInfoFlags flags = rq::getInfoFlags(modifier);
  return rq::getHasNone(flags, rq::ModifierInfoFlags::ATTACHMENT |
                                   rq::ModifierInfoFlags::NO_ATTACHMENT);
}

enum class ModifierKind : std::uint_fast8_t {
  NONE,
  ANCHOR,
  CONTAINER,
  VISIBIITY,
  ACCESS,
  MUTABILITY,
  GENERATION_TIME,
  EXECUTION_TIME,
  EVALUATION_TIME,
  INITIALIZATION_TIME,
  CAPTURE,
  LINKAGE,
  MANGLE,
  PACK,
  BRANCH_TREND,
  SUPPORT_NOTICE,
  ADDRESS_STABILITY,
  VARIADIC,
  OFFSET,
  LAZY_DECLARATION_KIND,
  CONSTRAINT,
  WEIGHT,
  DEDUCTION,
  VIRTUALITY,
  RANGER,
  REQUIRE,
  ENSURE,
  LAST
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::ModifierKind kind) {
  using MK = rq::ModifierKind;
  switch (kind) {
  case MK::NONE:
    return "no modifier";
  case MK::ANCHOR:
    return "anchor modifier";
  case MK::CONTAINER:
    return "container modifier";
  case MK::VISIBIITY:
    return "visibility modifier";
  case MK::ACCESS:
    return "access modifier";
  case MK::MUTABILITY:
    return "mutability modifier";
  case MK::GENERATION_TIME:
    return "generation time modifier";
  case MK::EXECUTION_TIME:
    return "execution time modifier";
  case MK::EVALUATION_TIME:
    return "evaluation time modifier";
  case MK::INITIALIZATION_TIME:
    return "initialization time modifier";
  case MK::CAPTURE:
    return "capture modifier";
  case MK::LINKAGE:
    return "linkage modifier";
  case MK::MANGLE:
    return "mangle modifier";
  case MK::PACK:
    return "pack modifier";
  case MK::BRANCH_TREND:
    return "branch trend modifier";
  case MK::SUPPORT_NOTICE:
    return "support modifier";
  case MK::ADDRESS_STABILITY:
    return "address stability modifier";
  case MK::VARIADIC:
    return "variadic modifier";
  case MK::OFFSET:
    return "offset modifier";
  case MK::LAZY_DECLARATION_KIND:
    return "lazy declaration kind modifier";
  case MK::CONSTRAINT:
    return "constraint modifier";
  case MK::WEIGHT:
    return "weight modifier";
  case MK::DEDUCTION:
    return "deduction modifier";
  case MK::VIRTUALITY:
    return "virtuality modifier";
  case MK::RANGER:
    return "ranger modifier";
  case MK::REQUIRE:
    return "require modifier";
  case MK::ENSURE:
    return "ensure modifier";
  case MK::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ModifierFuseFlags getMask(rq::ModifierKind kind) {
  using MK = rq::ModifierKind;
  using MFF = rq::ModifierFuseFlags;
  switch (kind) {
  case MK::NONE:
    return MFF::NO_MODIFIER;
  case MK::ANCHOR:
    return MFF::ANCHOR_MODIFIER_MASK;
  case MK::CONTAINER:
    return MFF::CONTAINER_MODIFIER_MASK;
  case MK::VISIBIITY:
    return MFF::VISIBILITY_MODIFIER_MASK;
  case MK::ACCESS:
    return MFF::ACCESS_MODIFIER_MASK;
  case MK::MUTABILITY:
    return MFF::MUTATE_MODIFIER_MASK;
  case MK::GENERATION_TIME:
    return MFF::GENERATION_TIME_MASK;
  case MK::EXECUTION_TIME:
    return MFF::EXECUTION_TIME_MASK;
  case MK::EVALUATION_TIME:
    return MFF::EVALUATION_TIME_MASK;
  case MK::INITIALIZATION_TIME:
    return MFF::INITIALIZATION_TIME_MASK;
  case MK::CAPTURE:
    return MFF::CAPTURE_MODIFIER_MASK;
  case MK::LINKAGE:
    return MFF::LINKAGE_MODIFIER_MASK;
  case MK::MANGLE:
    return MFF::MANGLE_MODIFIER_MASK;
  case MK::PACK:
    return MFF::PACK_MODIFIER_MASK;
  case MK::BRANCH_TREND:
    return MFF::BRANCH_TREND_MODIFIER_MASK;
  case MK::SUPPORT_NOTICE:
    return MFF::SUPPORT_NOTICE_MODIFIER_MASK;
  case MK::ADDRESS_STABILITY:
    return MFF::ADDRESS_STABILITY_MODIFIER_MASK;
  case MK::VARIADIC:
    return MFF::VARIADIC_MODIFIER_MASK;
  case MK::OFFSET:
    return MFF::OFFSET_MODIFIER_MASK;
  case MK::LAZY_DECLARATION_KIND:
    return MFF::LAZY_DECLARATION_KIND_MODIFIER_MASK;
  case MK::CONSTRAINT:
    return MFF::CONSTRAINT_MODIFIER_MASK;
  case MK::WEIGHT:
    return MFF::WEIGHT_MODIFIER_MASK;
  case MK::DEDUCTION:
    return MFF::DEDUCTION_MODIFIER_MASK;
  case MK::VIRTUALITY:
    return MFF::VIRTUALITY_MODIFIER_MASK;
  case MK::RANGER:
    return MFF::RANGER_MODIFIER_MASK;
  case MK::REQUIRE:
    return MFF::REQUIRE_MODIFIER_MASK;
  case MK::ENSURE:
    return MFF::ENSURE_MODIFIER_MASK;
  case MK::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ModifierKind getKind(rq::Modifier modifier) {
  using M = rq::Modifier;
  using MK = rq::ModifierKind;
  switch (modifier) {
  case M::NO_MODIFIER:
    return MK::NONE;
  case M::ANCHOR:
    return MK::ANCHOR;
  case M::RESIDENT:
    [[fallthrough]];
  case M::FLANK:
    return MK::CONTAINER;
  case M::TRANSPARENT:
    [[fallthrough]];
  case M::OPAQUE:
    return MK::VISIBIITY;
  case M::PRIVATE:
    [[fallthrough]];
  case M::PUBLIC:
    [[fallthrough]];
  case M::EXPORT:
    return MK::ACCESS;
  case M::MUTABLE:
    [[fallthrough]];
  case M::PARTIALLY_MUTABLE:
    [[fallthrough]];
  case M::CONSTANT:
    return MK::MUTABILITY;
  case M::DYNAMIC:
    [[fallthrough]];
  case M::STATIC:
    return MK::GENERATION_TIME;
  case M::RUNTIME:
    [[fallthrough]];
  case M::COMPILE_TIME:
    [[fallthrough]];
  case M::HYBRID:
    return MK::EXECUTION_TIME;
  case M::EAGER:
    [[fallthrough]];
  case M::LAZY:
    return MK::EVALUATION_TIME;
  case M::PREDEFINED:
    [[fallthrough]];
  case M::SINGLETON:
    return MK::INITIALIZATION_TIME;
  case M::CAPTURE:
    return MK::CAPTURE;
  case M::LINKED:
    [[fallthrough]];
  case M::INLINE:
    return MK::LINKAGE;
  case M::STANDARD_MANGLE:
    [[fallthrough]];
  case M::MANGLE:
    return MK::MANGLE;
  case M::PAD:
    [[fallthrough]];
  case M::PACK:
    return MK::PACK;
  case M::EQUIVOCAL:
    [[fallthrough]];
  case M::LIKELY:
    [[fallthrough]];
  case M::UNLIKELY:
    return MK::BRANCH_TREND;
  case M::SUPPORTED:
    [[fallthrough]];
  case M::DEPRECIATED:
    [[fallthrough]];
  case M::EXPERIMENTAL:
    return MK::SUPPORT_NOTICE;
  case M::UNSTABLE_ADDRESS:
    [[fallthrough]];
  case M::STABLE_ADDRESS:
    return MK::ADDRESS_STABILITY;
  case M::INVARIADIC:
    [[fallthrough]];
  case M::VARIADIC:
    return MK::VARIADIC;
  case M::BEST_LOCATION:
    [[fallthrough]];
  case M::LOCATION:
    return MK::OFFSET;
  case M::TEMPLATE:
    [[fallthrough]];
  case M::OVERLOAD:
    return MK::LAZY_DECLARATION_KIND;
  case M::CONSTRAINT:
    return MK::CONSTRAINT;
  case M::DEFAULT_WEIGHT:
    [[fallthrough]];
  case M::WEIGHT:
    return MK::WEIGHT;
  case M::MANUAL:
    [[fallthrough]];
  case M::AUTO:
    return MK::DEDUCTION;
  case M::DIRECT:
    [[fallthrough]];
  case M::VIRTUAL:
    return MK::VIRTUALITY;
  case M::RANGER:
    return MK::RANGER;
  case M::REQUIRE:
    return MK::REQUIRE;
  case M::ENSURE:
    return MK::ENSURE;
  case M::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

struct ModifierExpressionPair final {
  using Self = rq::ModifierExpressionPair;

  rq::Expression *_instantiation_ex_ptr{nullptr};
  rq::Expression *_attachment_ex_ptr{nullptr};

  explicit ModifierExpressionPair() = default;
  explicit ModifierExpressionPair(rq::Expression &instantiation_ex,
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

struct ModifierFactory final {
  using Self = rq::ModifierFactory;
  using PtrMap =
      llvm::SmallDenseMap<rq::ModifierKind, rq::ModifierExpressionPair>;

  rq::ModifierFuseFlags _flags{};
  PtrMap _ptr_map{};

  ModifierFactory() = default;
  ModifierFactory(const Self &) = delete;
  ModifierFactory(Self &&) = delete;
  ~ModifierFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_flags == rq::ModifierFuseFlags::NONE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags getFuseFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAll(rq::ModifierFuseFlags flags) const {
    return rq::getHasAll(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasSome(rq::ModifierFuseFlags flags) const {
    return rq::getHasSome(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasNone(rq::ModifierFuseFlags flags) const {
    return rq::getHasNone(this->_flags, flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierExpressionPair
  getExpressionPair(rq::ModifierKind kind) const {
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      return rq::ModifierExpressionPair();
    }
    return it->getSecond();
  }
  [[nodiscard]] inline bool addFlag(rq::Modifier modifier,
                                    rq::Expression &instantiation_ex,
                                    rq::Expression *attachment_ex_ptr) {
    const rq::ModifierFuseFlags flag = rq::getFuseFlags(modifier);
    this->_flags |= flag;
    const rq::ModifierKind kind = rq::getKind(modifier);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      this->_ptr_map.emplace_or_assign(
          kind,
          rq::ModifierExpressionPair(instantiation_ex, attachment_ex_ptr));
    } else {
      return false;
    }
    return true;
  }
};

enum class Qualifier : std::uint_fast8_t {
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

[[nodiscard]] inline llvm::StringRef getName(rq::Qualifier modifier) {
  using namespace rq;
  using HA = Qualifier;
  switch (modifier) {
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

[[nodiscard]] inline rq::Qualifier getQualifier(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using HA = Qualifier;
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
  default:
    break;
  }
  return HA::NONE;
}

enum class QualifierFuseFlags : std::uint_fast8_t {
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

RQ_DEFINE_FLAGS(rq::QualifierFuseFlags);

[[nodiscard]] inline rq::QualifierFuseFlags
getFuseFlags(rq::Qualifier modifier) {
  using namespace rq;
  using Q = Qualifier;
  using QFF = QualifierFuseFlags;
  switch (modifier) {
  case Q::NONE:
    return QFF::NONE;
  case Q::NO_VAR:
    return QFF::NONE;
  case Q::VAR:
    return QFF::VAR;
  case Q::PARTIAL_VAR:
    return QFF::PARTIAL_VAR;
  case Q::NO_VOLATILE:
    return QFF::NONE;
  case Q::VOLATILE:
    return QFF::VOLATILE;
  case Q::NO_ATOMIC:
    return QFF::NONE;
  case Q::ATOMIC:
    return QFF::ATOMIC;
  case Q::NO_NULL_TERMINATE:
    return QFF::NONE;
  case Q::NULL_TERMINATE:
    return QFF::NULL_TERMINATE;
  case Q::NO_GREATEST:
    return QFF::NONE;
  case Q::GREATEST:
    return QFF::GREATEST;
  }
  RQ_UNREACHABLE();
}

enum class QualifierKind : std::uint_fast8_t {
  NONE,
  VAR,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATE,
  GREATEST
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::QualifierKind kind) {
  using HAK = rq::QualifierKind;
  switch (kind) {
  case HAK::NONE:
    return "none";
  case HAK::VAR:
    return "var modifier";
  case HAK::VOLATILE:
    return "volatile modifier";
  case HAK::ATOMIC:
    return "atomic modifier";
  case HAK::NULL_TERMINATE:
    return "null terminate modifier";
  case HAK::GREATEST:
    return "greatest";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::QualifierKind getKind(rq::Qualifier modifier) {
  using HA = rq::Qualifier;
  using HAK = rq::QualifierKind;
  switch (modifier) {
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

struct QualifierFactory final {
  using Self = rq::QualifierFactory;
  using PtrMap = llvm::SmallDenseMap<rq::QualifierKind, rq::Expression *>;

  rq::QualifierFuseFlags _flags{};
  PtrMap _ptr_map{};

  QualifierFactory() = default;
  QualifierFactory(const Self &) = delete;
  QualifierFactory(Self &&) = delete;
  ~QualifierFactory() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifierFuseFlags getFuseFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const PtrMap &getPtrMap() const {
    return this->_ptr_map;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  getExpressionPtr(rq::QualifierKind kind) {
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      return nullptr;
    }
    return it->getSecond();
  }
  [[nodiscard]] inline bool addFlag(rq::Qualifier modifier,
                                    rq::Expression *expression_ptr) {
    const rq::QualifierFuseFlags flag = rq::getFuseFlags(modifier);
    this->_flags |= flag;
    const rq::QualifierKind kind = rq::getKind(modifier);
    auto it = this->_ptr_map.find(kind);
    if (it == this->_ptr_map.end()) {
      this->_ptr_map.emplace_or_assign(kind, expression_ptr);
    } else {
      return false;
    }
    return true;
  }
};

enum class ArithmeticStep : std::uint_fast8_t {
  NONE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::ArithmeticStep step) {
  using namespace rq;
  using ASS = ArithmeticStep;
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

[[nodiscard]] inline rq::ArithmeticStep getArithmeticStep(rq::Keyword keyword) {
  using namespace rq;
  using ASS = ArithmeticStep;
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

enum class ArithmeticCondition : std::uint_fast8_t {
  NONE,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  EQUAL,
  NOT_EQUAL
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::ArithmeticCondition condition) {
  using namespace rq;
  using ASC = ArithmeticCondition;
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

[[nodiscard]] inline rq::ArithmeticCondition
getArithmeticCondition(rq::Keyword keyword) {
  using namespace rq;
  using ASC = ArithmeticCondition;
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
  using pointer = const rq::Expression *;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsQualifier() const {
    return rq::getIsQualifier(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModifier() const {
    return rq::getIsModifier(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeModifierInstantiation() const {
    return rq::getCanBeModifierInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeQualifierInstantiation() const {
    return rq::getCanBeQualifierInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticSequenceStage() const {
    return rq::getCanBeArithmeticSequenceStage(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticCondition() const {
    return rq::getCanBeArithmeticCondition(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticStep() const {
    return rq::getCanBeArithmeticStep(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeSituation(rq::Situation situation) const {
    return rq::getCanBeSituation(this->getKeyword(), situation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName() const {
    return rq::getIsEvaluatableName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Modifier getModifier() const {
    return rq::getModifier(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Qualifier getQualifier() const {
    return rq::getQualifier(this->getKeyword());
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
    if (this->getIsLiteral()) {
      inout_id.AddString(this->getSourceText());
    } else {
      inout_id.AddString("");
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