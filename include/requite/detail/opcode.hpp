// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>
#include <magic_enum.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

namespace requite {

constexpr std::string_view getName(requite::Opcode opcode) {
  using namespace requite;
  using O = Opcode;

  switch (opcode) {
  case O::__NONE:
    return "__none";

  // HANDLES
  case O::__LOCAL_HANDLE:
    return "__local_handle";

  // LITERALS
  case O::__INTEGER_LITERAL:
    return "__integer_literal";
  case O::__FLOAT_LITERAL:
    return "__float_literal";
  case O::__STRING_LITERAL:
    return "__string_literal";
  case O::__CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case O::__IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case O::__ERROR:
    return "__error";

  // SITUATIONAL
  case O::_CLOVEN:
    return "_cloven";
  case O::_COLON:
    return "_colon";
  case O::_INFERENCE:
    return "_inference";

  // LOGICAL
  case O::_LOGICAL_AND:
    return "_logical_and";
  case O::_LOGICAL_OR:
    return "_logical_or";
  case O::_LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case O::_GREATER:
    return "_greater";
  case O::_GREATER_EQUAL:
    return "_greater_equal";
  case O::_LESS:
    return "_less";
  case O::_LESS_EQUAL:
    return "_less_equal";
  case O::_EQUAL:
    return "_equal";
  case O::_NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case O::_EXTEND:
    return "_extend";
  case O::_BINDING:
    return "_binding";
  case O::_ASCRIBE:
    return "_ascribe";
  case O::_CAST:
    return "_cast";
  case O::_IDENTIFY:
    return "_identify";

  // ARITHMETIC
  case O::_ADD:
    return "_add";
  case O::_SUBTRACT:
    return "_subtract";
  case O::_MULTIPLY:
    return "_multiply";
  case O::_DIVIDE:
    return "_divide";
  case O::_MODULUS:
    return "_modulus";
  case O::_NEGATE:
    return "_negate";

  // BITWISE
  case O::_BITWISE_CAST:
    return "_bitwise_cast";
  case O::_BITWISE_OR:
    return "_bitwise_or";
  case O::_BITWISE_AND:
    return "_bitwise_and";
  case O::_BITWISE_XOR:
    return "_bitwise_xor";
  case O::_BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case O::_BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case O::_BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case O::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case O::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case O::_CONCATINATE:
    return "_concatinate";
  case O::FROM_FRONT:
    return "from_front";
  case O::_FROM_FRONT_OF:
    return "_from_front_of";
  case O::FROM_BACK:
    return "from_back";
  case O::_FROM_BACK_OF:
    return "_from_back_of";
  case O::TRUNCATE_FRONT:
    return "truncate_front";
  case O::_TRUNCATE_FRONT_OF:
    return "_truncate_front_of";
  case O::TRUNCATE_BACK:
    return "truncate_back";
  case O::_TRUNCATE_BACK_OF:
    return "_truncate_back_of";
  case O::CONTENT:
    return "content";
  case O::_CONTENT_OF:
    return "_content_of";
  case O::ADDRESS:
    return "address";
  case O::_ADDRESS_OF:
    return "_address_of";
  case O::BORROW:
    return "borrow";
  case O::_BORROW_OF:
    return "_borrow_of";
  case O::_BORROW_OF_ASCRIBED:
    return "_borrow_of_ascribed";
  case O::STEAL:
    return "steal";
  case O::_STEAL_OF:
    return "_steal_of";
  case O::_STEAL_OF_ASCRIBED:
    return "_steal_of_ascribed";
  case O::VIEW:
    return "view";
  case O::_VIEW_OF:
    return "_view_of";
  case O::_VIEW_OF_ASCRIBED:
    return "_view_of_ascribed";
  case O::SLICE:
    return "slice";
  case O::_SLICE_OF:
    return "_slice_of";
  case O::_SLICE_OF_ASCRIBED:
    return "_slice_of_ascribed";

  // ASSIGNMENT
  case O::_ASSIGN:
    return "_assign";
  case O::_ASSIGN_ADD:
    return "_assign_add";
  case O::_ASSIGN_SUBTRACT:
    return "_assign_subtract";
  case O::_ASSIGN_MULTIPLY:
    return "_assign_multiply";
  case O::_ASSIGN_DIVIDE:
    return "_assign_divide";
  case O::_ASSIGN_MODULUS:
    return "_assign_modulus";

  // MOVE SEMANTICS
  case O::COPY:
    return "copy";
  case O::_COPY_OF:
    return "_copy_of";
  case O::MOVE:
    return "move";
  case O::_MOVE_OF:
    return "_move_of";
  case O::SWAP:
    return "swap";

  // SUBTYPE
  case O::_ARRAY:
    return "_array";
  case O::_REFERENCE:
    return "_reference";
  case O::_POINTER:
    return "_pointer";
  case O::_FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case O::MUTABLE:
    return "mutable";
  case O::CONSTANT:
    return "constant";
  case O::VOLATILE:
    return "volatile";
  case O::ATOMIC:
    return "atomic";
  case O::NULL_TERMINATED:
    return "null_terminated";
  case O::OWNING:
    return "owning";
  case O::MAY_DISCARD:
    return "may_discard";

  // PARAMETER RULES
  case O::_POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case O::_NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";

  // TRIPS
  case O::_TUPLE:
    return "_tuple";
  case O::_LAYOUT:
    return "_layout";
  case O::_NULL:
    return "_null";
  case O::_NULL_TYPE:
    return "_null_type";
  case O::_STRUCTURED_BINDING:
    return "_structured_binding";
  case O::_IGNORE:
    return "_ignore";
  case O::_SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case O::_CALL:
    return "_call";
  case O::_INDEX:
    return "_index";
  case O::_SIGNATURE:
    return "_signature";
  case O::DESTROY:
    return "destroy";
  case O::_DESTROY_VALUE:
    return "_destroy_value";
  case O::DROP:
    return "drop";
  case O::_DROP_VALUE:
    return "_drop_value";
  case O::ENTRY_POINT:
    return "entry_point";
  case O::FUNCTION:
    return "function";
  case O::CONSTRUCTOR:
    return "constructor";
  case O::DESTRUCTOR:
    return "destructor";
  case O::RANGER:
    return "ranger";
  case O::INDEXER:
    return "indexer";
  case O::_ANONYMOUS_FUNCTION:
    return "_anonymous_function";
  case O::CAPTURE:
    return "capture";

  // CONTROL FLOW
  case O::RETURN:
    return "return";
  case O::BREAK:
    return "break";
  case O::CONTINUE:
    return "continue";
  case O::FALLTHROUGH:
    return "fallthrough";
  case O::GOTO:
    return "goto";
  case O::EXIT:
    return "exit";
  case O::LOOP_BODY:
    return "loop_body";

  // SYMBOLS
  case O::OBJECT:
    return "object";
  case O::VARIANT:
    return "variant";
  case O::TABLE:
    return "table";
  case O::USE:
    return "use";

  // VALUES
  case O::TRUE:
    return "true";
  case O::FALSE:
    return "false";
  case O::INDETERMINATE:
    return "indeterminate";
  case O::THIS:
    return "this";
  case O::RESULT:
    return "result";
  case O::ARGUMENTS:
    return "arguments";
  case O::IN:
    return "in";
  case O::OUT:
    return "out";
  case O::ADDRESS_SIZE:
    return "address_size";
  case O::ADDRESS_DEPTH:
    return "address_depth";
  case O::BITS_PER_BYTE:
    return "bits_per_byte";

  // BUILTIN TYPES
  case O::VOID:
    return "void";
  case O::BOOLEAN:
    return "boolean";
  case O::WORD:
    return "word";
  case O::SIGNED:
    return "signed";
  case O::UNSIGNED:
    return "unsigned";
  case O::BFLOAT16:
    return "bfloat16";
  case O::BINARY16:
    return "binary16";
  case O::BINARY32:
    return "binary32";
  case O::BINARY64:
    return "binary64";
  case O::BINARY128:
    return "binary128";
  case O::UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case O::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case O::FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case O::_FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case O::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case O::_NEXT_VARIADIC_ARGUMENT_OF:
    return "_next_variadic_argument_of";

  // SCOPES
  case O::IF:
    return "if";
  case O::ELSE_IF:
    return "else_if";
  case O::ELSE:
    return "else";
  case O::SWITCH:
    return "switch";
  case O::MATCH:
    return "match";
  case O::CASE:
    return "case";
  case O::DEFAULT:
    return "default";
  case O::LOOP:
    return "loop";
  case O::SCOPE:
    return "scope";
  case O::BLOCK:
    return "block";
  case O::_INLINE_SCOPE:
    return "_inline_scope";
  case O::_INLINE_BLOCK:
    return "_inline_block";

  // RANGES
  case O::_LIMIT_RANGE_EQUAL:
    return "_limit_range_equal";
  case O::_LIMIT_RANGE_NOT_EQUAL:
    return "_limit_range_not_equal";
  case O::_LIMIT_RANGE_GREATER:
    return "_limit_range_greater";
  case O::_LIMIT_RANGE_GREATER_EQUAL:
    return "_limit_range_greater_equal";
  case O::_LIMIT_RANGE_LESS:
    return "_limit_range_less";
  case O::_LIMIT_RANGE_LESS_EQUAL:
    return "_limit_range_less_equal";
  case O::_LONG_RANGE:
    return "_long_range";
  case O::FOR:
    return "for";
  case O::DO:
    return "do";
  case O::WHILE:
    return "while";
  case O::UNTIL:
    return "until";
  case O::FOREVER:
    return "forever";
  case O::STEP:
    return "step";
  case O::WHEN:
    return "when";
  case O::_SHORT_RANGE:
    return "_short_range";
  case O::_SHORT_STEP_ADD:
    return "_short_step_add";
  case O::_SHORT_STEP_SUBTRACT:
    return "_short_step_subtract";
  case O::_SHORT_STEP_MULTIPLY:
    return "_short_step_multiply";
  case O::_SHORT_STEP_DIVIDE:
    return "_short_step_divide";
  case O::_SHORT_STEP_MODULUS:
    return "_short_step_modulus";
  case O::_SHORT_WHILE_LESS:
    return "_short_while_less";
  case O::_SHORT_WHILE_GREATER:
    return "_short_while_greater";
  case O::_SHORT_WHILE_LESS_EQUAL:
    return "_short_while_less_equal";
  case O::_SHORT_WHILE_GREATER_EQUAL:
    return "_short_while_greater_equal";
  case O::_SHORT_WHILE_EQUAL:
    return "_short_while_equal";
  case O::_SHORT_WHILE_NOT_EQUAL:
    return "_short_while_not_equal";

  // ACCESS MODIFIERS
  case O::PRIVATE:
    return "private";
  case O::PROTECTED:
    return "protected";
  case O::EXPORT:
    return "export";

  // SYMBOL GRAPH
  case O::IMPORT:
    return "import";
  case O::USE_TABLE:
    return "use_table";

  // SOURCES
  case O::_MODULE_ROOT:
    return "_module_root";

  // ERROR HANDLING AND DEBUGGING
  case O::ASSERT:
    return "assert";
  case O::UNREACHABLE:
    return "unreachable";

  // ATTRIBUTES
  case O::BAKE:
    return "bake";
  case O::MAY_PARENT:
    return "may_parent";
  case O::PARENT:
    return "parent";
  case O::POSITION:
    return "position";
  case O::INLINE:
    return "inline";
  case O::MANGLED_NAME:
    return "mangled_name";
  case O::_MANGLED_NAME_OF:
    return "_mangled_name_of";
  case O::PACK:
    return "pack";
  case O::USER:
    return "user";
  case O::LABEL:
    return "label";
  case O::TEMPLATE:
    return "template";

  // REFLECTIONS
  case O::_REFLECT:
    return "_reflect";
  case O::_MEMBER_OF:
    return "_member_of";
  case O::SIZE:
    return "size";
  case O::_SIZE_OF:
    return "_size_of";
  case O::DEPTH:
    return "depth";
  case O::_DEPTH_OF:
    return "_depth_of";
  case O::COUNT:
    return "count";
  case O::_COUNT_OF:
    return "_count_of";
  case O::LENGTH:
    return "length";
  case O::_LENGTH_OF:
    return "_length_of";
  case O::NAME:
    return "name";
  case O::_NAME_OF:
    return "_name_of";
  case O::LINE:
    return "line";
  case O::_LINE_OF:
    return "_line_of";
  case O::COLUMN:
    return "column";
  case O::_COLUMN_OF:
    return "_column_of";
  case O::IS:
    return "is";
  case O::_VALUE_IS:
    return "_value_is";
  case O::GET:
    return "get";
  case O::_GET_VALUE_OF_VARIANT:
    return "_get_value_of_variant";
  case O::TYPE:
    return "type";
  case O::_TYPE_OF:
    return "_type_of";
  case O::SYMBOL:
    return "symbol";
  case O::_SYMBOL_OF:
    return "_symbol_of";
  case O::DISCRIMINANT:
    return "discriminant";
  case O::_DISCRIMINANT_OF:
    return "_discriminant_of";

  case O::__LAST:
    return "__last";
  }

  return "_unknown";
}

namespace _opcode {
enum _OpcodeFlags : std::uint32_t {
  _NONE = 0,
  _CONVERGING = requite::getBit(31),
  _INTERMEDIATE = requite::getBit(30),
  _INTERNAL = requite::getBit(29),
  _THIS_CAN_HAVE_NO_SEMICOLON = requite::getBit(28),
  _HAS_SEMICOLON_SEPARATED_BRANCHES = requite::getBit(27),
  _FIRST_COMMA_BRANCH_CAN_BE_INFERENCE = requite::getBit(26),
  _LAST_COMMA_BRANCH_CAN_BE_INFERENCE = requite::getBit(25),
  _ALL_COMMA_BRANCHES_CAN_BE_INFERENCE = requite::getBit(24),
  _ROOT_STATEMENT = requite::getBit(23),
  _TOP_STATEMENT = requite::getBit(22),
  _TABLE_STATEMENT = requite::getBit(21),
  _OBJECT_STATEMENT = requite::getBit(20),
  _LOCAL_STATEMENT = requite::getBit(19),
  _VALUE = requite::getBit(18),
  _REFLECTION = requite::getBit(17),
  _ASCRIBED_REFLECTION = requite::getBit(16),
  _ARGUMENT = requite::getBit(15),
  _PARAMETER = requite::getBit(14),
  _BINDING = requite::getBit(13),
  _DESTINATION = requite::getBit(12),
  _ALTERNATIVE = requite::getBit(11),
  _NAME = requite::getBit(10),
  _PATH = requite::getBit(9),
  _ATTRIBUTE = requite::getBit(8),
  _LONG_RANGE_STAGE = requite::getBit(7),
  _SHORT_RANGE_STAGE = requite::getBit(6),
  _CASE = requite::getBit(5),
  _LAST_CASE = requite::getBit(4),
  _CAPTURE = requite::getBit(3),
  _STRING_LITERAL = requite::getBit(2),
  _COMMA_BRANCH_COUNT_MASK = 0x3,
  _ALL = _TOP_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
         _LOCAL_STATEMENT | _LOCAL_STATEMENT | _VALUE | _REFLECTION |
         _ARGUMENT | _PARAMETER | _BINDING | _DESTINATION | _ALTERNATIVE |
         _NAME | _PATH | _ATTRIBUTE | _LONG_RANGE_STAGE | _SHORT_RANGE_STAGE |
         _CASE | _LAST_CASE | _CAPTURE | _STRING_LITERAL
};
}

[[nodiscard]] constexpr requite::_opcode::_OpcodeFlags
operator|(requite::_opcode::_OpcodeFlags lhs,
          requite::_opcode::_OpcodeFlags rhs) {
  using Underlying = std::underlying_type_t<requite::_opcode::_OpcodeFlags>;
  return static_cast<requite::_opcode::_OpcodeFlags>(
      static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

[[nodiscard]] constexpr requite::_opcode::_OpcodeFlags
_getFlags(requite::Opcode opcode) {
  using namespace requite;
  using namespace requite::_opcode;
  using O = Opcode;

  switch (opcode) {
  case O::__NONE:
    return _NONE;

  // HANDLES
  case O::__LOCAL_HANDLE:
    return _INTERNAL | _VALUE | _ARGUMENT | _DESTINATION;

  // LITERALS
  case O::__INTEGER_LITERAL:
    return _INTERNAL | _VALUE | _ARGUMENT;
  case O::__FLOAT_LITERAL:
    return _INTERNAL | _VALUE | _ARGUMENT;
  case O::__STRING_LITERAL:
    return _INTERNAL | _VALUE | _ARGUMENT | _STRING_LITERAL;
  case O::__CODEUNIT_LITERAL:
    return _INTERNAL | _VALUE | _ARGUMENT;
  case O::__IDENTIFIER_LITERAL:
    return _INTERNAL | _VALUE | _DESTINATION | _REFLECTION | _ARGUMENT | _PARAMETER |
           _BINDING | _NAME | _PATH;

  // ERRORS
  case O::__ERROR:
    return _INTERNAL;

  // SITUATIONAL
  case O::_CLOVEN:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _DESTINATION | _NAME |
           _PATH | _LONG_RANGE_STAGE | _SHORT_RANGE_STAGE | _STRING_LITERAL;
  case O::_COLON:
    return _INTERMEDIATE | _VALUE | _DESTINATION | _ARGUMENT | _PARAMETER |
           _BINDING;
  case O::_INFERENCE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  // LOGICAL
  case O::_LOGICAL_AND:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LOGICAL_OR:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LOGICAL_COMPLEMENT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  // COMPARISON
  case O::_GREATER:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_GREATER_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LESS:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LESS_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_NOT_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  // APPLY
  case O::_EXTEND:
    return _INTERMEDIATE | _VALUE;
  case O::_BINDING:
    return _INTERMEDIATE | _DESTINATION | _PARAMETER | _ARGUMENT |
           _ALTERNATIVE;
  case O::_ASCRIBE:
    return _INTERMEDIATE | _ALL;
  case O::_CAST:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_IDENTIFY:
    return _INTERMEDIATE | _NAME | _PATH | _VALUE | _ARGUMENT;

  // ARITHMETIC
  case O::_ADD:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_SUBTRACT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_MULTIPLY:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_DIVIDE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_MODULUS:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_NEGATE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  // BITWISE
  case O::_BITWISE_CAST:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_OR:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_AND:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_XOR:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_COMPLEMENT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_SHIFT_LEFT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BITWISE_SHIFT_RIGHT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::BITWISE_ROTATE_LEFT:
    return _VALUE | _ARGUMENT;
  case O::BITWISE_ROTATE_RIGHT:
    return _VALUE | _ARGUMENT;

  // MEMORY
  case O::_CONCATINATE:
    return _INTERMEDIATE | _STRING_LITERAL | _VALUE | _ARGUMENT;
  case O::FROM_FRONT:
    return _REFLECTION;
  case O::_FROM_FRONT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::FROM_BACK:
    return _REFLECTION;
  case O::_FROM_BACK_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::TRUNCATE_FRONT:
    return _REFLECTION;
  case O::_TRUNCATE_FRONT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::TRUNCATE_BACK:
    return _REFLECTION;
  case O::_TRUNCATE_BACK_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::CONTENT:
    return _REFLECTION;
  case O::_CONTENT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::ADDRESS:
    return _REFLECTION;
  case O::_ADDRESS_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::BORROW:
    return _REFLECTION | _ASCRIBED_REFLECTION;
  case O::_BORROW_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_BORROW_OF_ASCRIBED:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::STEAL:
    return _REFLECTION | _ASCRIBED_REFLECTION;
  case O::_STEAL_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_STEAL_OF_ASCRIBED:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::VIEW:
    return _REFLECTION | _ASCRIBED_REFLECTION;
  case O::_VIEW_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_VIEW_OF_ASCRIBED:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::SLICE:
    return _REFLECTION | _ASCRIBED_REFLECTION;
  case O::_SLICE_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_SLICE_OF_ASCRIBED:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  // ASSIGNMENT
  case O::_ASSIGN:
    return _INTERMEDIATE | _LOCAL_STATEMENT | _ARGUMENT | _PARAMETER |
           _ALTERNATIVE;
  case O::_ASSIGN_ADD:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_ASSIGN_SUBTRACT:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_ASSIGN_MULTIPLY:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_ASSIGN_DIVIDE:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_ASSIGN_MODULUS:
    return _INTERMEDIATE | _LOCAL_STATEMENT;

  // MOVE SEMANTICS
  case O::COPY:
    return _REFLECTION;
  case O::_COPY_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::MOVE:
    return _REFLECTION;
  case O::_MOVE_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::SWAP:
    return _LOCAL_STATEMENT;

  // SUBTYPE
  case O::_ARRAY:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_REFERENCE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_POINTER:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_FAT_POINTER:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;

  // TYPE MODIFIER
  case O::MUTABLE:
    return _ATTRIBUTE;
  case O::CONSTANT:
    return _ATTRIBUTE;
  case O::VOLATILE:
    return _ATTRIBUTE;
  case O::ATOMIC:
    return _ATTRIBUTE;
  case O::NULL_TERMINATED:
    return _ATTRIBUTE;
  case O::OWNING:
    return _ATTRIBUTE;
  case O::MAY_DISCARD:
    return _ATTRIBUTE;

  // PARAMETER RULES
  case O::_POSITIONAL_PARAMETERS_END:
    return _INTERMEDIATE | _PARAMETER;
  case O::_NAMED_PARAMETERS_BEGIN:
    return _INTERMEDIATE | _PARAMETER;

  // TRIPS
  case O::_TUPLE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LAYOUT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_NULL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_NULL_TYPE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_STRUCTURED_BINDING:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_IGNORE:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::_SPECIALIZATION:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;

  // PROCEDURES
  case O::_CALL:
    return _INTERMEDIATE | _LOCAL_STATEMENT | _VALUE | _DESTINATION | _ARGUMENT;
  case O::_INDEX:
    return _INTERMEDIATE | _VALUE | _DESTINATION | _ARGUMENT;
  case O::_SIGNATURE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::DESTROY:
    return _REFLECTION;
  case O::_DESTROY_VALUE:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::DROP:
    return _REFLECTION;
  case O::_DROP_VALUE:
    return _INTERMEDIATE | _LOCAL_STATEMENT;
  case O::ENTRY_POINT:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _TOP_STATEMENT;
  case O::FUNCTION:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LAST_COMMA_BRANCH_CAN_BE_INFERENCE | _TOP_STATEMENT | _TABLE_STATEMENT |
           _OBJECT_STATEMENT | _LOCAL_STATEMENT | static_cast<_OpcodeFlags>(2);
  case O::CONSTRUCTOR:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _ALL_COMMA_BRANCHES_CAN_BE_INFERENCE | _OBJECT_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::DESTRUCTOR:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _OBJECT_STATEMENT;
  case O::RANGER:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _OBJECT_STATEMENT;
  case O::INDEXER:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LAST_COMMA_BRANCH_CAN_BE_INFERENCE | _OBJECT_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::_ANONYMOUS_FUNCTION:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _LAST_COMMA_BRANCH_CAN_BE_INFERENCE |
           _VALUE | _ARGUMENT | static_cast<_OpcodeFlags>(2);
  case O::CAPTURE:
    return _LONG_RANGE_STAGE | _CAPTURE;

  // CONTROL FLOW
  case O::RETURN:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::BREAK:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::CONTINUE:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::FALLTHROUGH:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::GOTO:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::EXIT:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::LOOP_BODY:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;

  // SYMBOLS
  case O::OBJECT:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LAST_COMMA_BRANCH_CAN_BE_INFERENCE | _TOP_STATEMENT | _TABLE_STATEMENT |
           static_cast<_OpcodeFlags>(2);
  case O::VARIANT:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LAST_COMMA_BRANCH_CAN_BE_INFERENCE | _TOP_STATEMENT | _TABLE_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::TABLE:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _TOP_STATEMENT | _TABLE_STATEMENT | static_cast<_OpcodeFlags>(1);
  case O::USE:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _TOP_STATEMENT | _TABLE_STATEMENT |
           _OBJECT_STATEMENT | _LOCAL_STATEMENT;

  // VALUES
  case O::TRUE:
    return _VALUE | _ARGUMENT;
  case O::FALSE:
    return _VALUE | _ARGUMENT;
  case O::INDETERMINATE:
    return _VALUE;
  case O::THIS:
    return _VALUE | _DESTINATION | _ARGUMENT;
  case O::RESULT:
    return _VALUE | _DESTINATION | _ARGUMENT;
  case O::ARGUMENTS:
    return _VALUE | _ARGUMENT;
  case O::IN:
    return _VALUE | _DESTINATION | _ARGUMENT;
  case O::OUT:
    return _VALUE | _DESTINATION | _ARGUMENT;
  case O::ADDRESS_SIZE:
    return _VALUE | _ARGUMENT;
  case O::ADDRESS_DEPTH:
    return _VALUE | _ARGUMENT;
  case O::BITS_PER_BYTE:
    return _VALUE | _ARGUMENT;

  // BUILTIN TYPES
  case O::VOID:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BOOLEAN:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::WORD:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::SIGNED:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::UNSIGNED:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BFLOAT16:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BINARY16:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BINARY32:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BINARY64:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::BINARY128:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::UTF8:
    return _VALUE | _ARGUMENT | _PARAMETER;

  // VARIADIC ARGUMENTS
  case O::VARIADIC_ARGUMENTS:
    return _VALUE | _ARGUMENT | _PARAMETER;
  case O::FIRST_VARIADIC_ARGUMENT:
    return _REFLECTION;
  case O::_FIRST_VARIADIC_ARGUMENT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;
  case O::NEXT_VARIADIC_ARGUMENT:
    return _REFLECTION;
  case O::_NEXT_VARIADIC_ARGUMENT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT | _PARAMETER;

  // SCOPES
  case O::IF:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::ELSE_IF:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::ELSE:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT;
  case O::SWITCH:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT |
           static_cast<_OpcodeFlags>(1);
  case O::MATCH:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT | _VALUE | static_cast<_OpcodeFlags>(1);
  case O::CASE:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT | static_cast<_OpcodeFlags>(1);
  case O::DEFAULT:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT;
  case O::LOOP:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _FIRST_COMMA_BRANCH_CAN_BE_INFERENCE | _LOCAL_STATEMENT |
           static_cast<_OpcodeFlags>(2);
  case O::SCOPE:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT | _VALUE;
  case O::BLOCK:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _THIS_CAN_HAVE_NO_SEMICOLON |
           _LOCAL_STATEMENT | _VALUE;
  case O::_INLINE_BLOCK:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _VALUE | _ARGUMENT | _PARAMETER;
  case O::_INLINE_SCOPE:
    return _HAS_SEMICOLON_SEPARATED_BRANCHES | _VALUE | _ARGUMENT | _PARAMETER;

  // RANGES
  case O::_LIMIT_RANGE_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LIMIT_RANGE_NOT_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LIMIT_RANGE_GREATER:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LIMIT_RANGE_GREATER_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LIMIT_RANGE_LESS:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LIMIT_RANGE_LESS_EQUAL:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_LONG_RANGE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::FOR:
    return _LONG_RANGE_STAGE;
  case O::DO:
    return _LONG_RANGE_STAGE;
  case O::WHILE:
    return _LONG_RANGE_STAGE;
  case O::UNTIL:
    return _LONG_RANGE_STAGE;
  case O::FOREVER:
    return _LONG_RANGE_STAGE;
  case O::STEP:
    return _LONG_RANGE_STAGE;
  case O::WHEN:
    return _LOCAL_STATEMENT | _LONG_RANGE_STAGE;
  case O::_SHORT_RANGE:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::_SHORT_STEP_ADD:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_STEP_SUBTRACT:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_STEP_MULTIPLY:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_STEP_DIVIDE:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_STEP_MODULUS:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_LESS:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_GREATER:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_LESS_EQUAL:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_GREATER_EQUAL:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_EQUAL:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;
  case O::_SHORT_WHILE_NOT_EQUAL:
    return _INTERMEDIATE | _SHORT_RANGE_STAGE;

  // ACCESS MODIFIERS
  case O::PRIVATE:
    return _ATTRIBUTE;
  case O::PROTECTED:
    return _ATTRIBUTE;
  case O::EXPORT:
    return _ATTRIBUTE;

  // SYMBOL GRAPH
  case O::IMPORT:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _TOP_STATEMENT;
  case O::USE_TABLE:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _TOP_STATEMENT | _TABLE_STATEMENT |
           _OBJECT_STATEMENT | _LOCAL_STATEMENT;

  // SOURCES
  case O::_MODULE_ROOT:
    return _INTERMEDIATE | _THIS_CAN_HAVE_NO_SEMICOLON |
           _HAS_SEMICOLON_SEPARATED_BRANCHES | _ROOT_STATEMENT;

  // ERROR HANDLING AND DEBUGGING
  case O::ASSERT:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;
  case O::UNREACHABLE:
    return _THIS_CAN_HAVE_NO_SEMICOLON | _LOCAL_STATEMENT;

  // ATTRIBUTES
  case O::BAKE:
    return _ATTRIBUTE;
  case O::MAY_PARENT:
    return _ATTRIBUTE;
  case O::PARENT:
    return _ATTRIBUTE;
  case O::POSITION:
    return _ATTRIBUTE;
  case O::INLINE:
    return _ATTRIBUTE;
  case O::MANGLED_NAME:
    return _REFLECTION | _ATTRIBUTE;
  case O::_MANGLED_NAME_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::PACK:
    return _ATTRIBUTE;
  case O::USER:
    return _ATTRIBUTE;
  case O::LABEL:
    return _ATTRIBUTE;
  case O::TEMPLATE:
    return _ATTRIBUTE;

  // REFLECTIONS
  case O::_REFLECT:
    return _INTERMEDIATE | _VALUE | _DESTINATION | _ARGUMENT | _PARAMETER |
           _PATH | _ASCRIBED_REFLECTION;
  case O::_MEMBER_OF:
    return _INTERMEDIATE | _VALUE | _DESTINATION | _ARGUMENT | _PATH;
  case O::SIZE:
    return _REFLECTION;
  case O::_SIZE_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::DEPTH:
    return _REFLECTION;
  case O::_DEPTH_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::COUNT:
    return _REFLECTION;
  case O::_COUNT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::LENGTH:
    return _REFLECTION;
  case O::_LENGTH_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::NAME:
    return _REFLECTION;
  case O::_NAME_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::LINE:
    return _REFLECTION;
  case O::_LINE_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::COLUMN:
    return _REFLECTION;
  case O::_COLUMN_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::IS:
    return _REFLECTION;
  case O::_VALUE_IS:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::GET:
    return _REFLECTION;
  case O::_GET_VALUE_OF_VARIANT:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::TYPE:
    return _VALUE | _ARGUMENT | _PARAMETER | _REFLECTION;
  case O::_TYPE_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::SYMBOL:
    return _REFLECTION;
  case O::_SYMBOL_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;
  case O::DISCRIMINANT:
    return _REFLECTION | _ATTRIBUTE;
  case O::_DISCRIMINANT_OF:
    return _INTERMEDIATE | _VALUE | _ARGUMENT;

  case O::__LAST:
    break;
  }

  return _NONE;
}

[[nodiscard]] constexpr bool
_getHasFlags(requite::Opcode opcode, requite::_opcode::_OpcodeFlags flags) {
  requite::_opcode::_OpcodeFlags opcode_flags = requite::_getFlags(opcode);
  const bool has_flags = (opcode_flags & flags) == flags;
  return has_flags;
}

[[nodiscard]] constexpr unsigned char _getMaskValue(requite::Opcode opcode) {
  const requite::_opcode::_OpcodeFlags flags = requite::_getFlags(opcode);
  const unsigned char value =
      (flags & requite::_opcode::_COMMA_BRANCH_COUNT_MASK);
  return value;
}

constexpr requite::Opcode getUniversalized(requite::Opcode opcode) {
  using namespace requite;
  using O = Opcode;
  switch (opcode) {
    case O::FROM_FRONT:
      return O::_FROM_FRONT_OF;
    case O::FROM_BACK:
      return O::_FROM_BACK_OF;
    case O::TRUNCATE_FRONT:
      return O::_TRUNCATE_FRONT_OF;
    case O::TRUNCATE_BACK:
      return O::_TRUNCATE_FRONT_OF;
    case O::CONTENT:
      return O::_CONTENT_OF;
    case O::ADDRESS:
      return O::_ADDRESS_OF;
    case O::BORROW:
      return O::_BORROW_OF;
    case O::STEAL:
      return O::_STEAL_OF;
    case O::SLICE:
      return O::_SLICE_OF;
    case O::VIEW:
      return O::_VIEW_OF;
    case O::COPY:
      return O::_COPY_OF;
    case O::MOVE:
      return O::_MOVE_OF;
    case O::FIRST_VARIADIC_ARGUMENT:
      return O::_FIRST_VARIADIC_ARGUMENT_OF;
    case O::NEXT_VARIADIC_ARGUMENT:
      return O::_NEXT_VARIADIC_ARGUMENT_OF;
    case O::MANGLED_NAME:
      return O::_MANGLED_NAME_OF;
    case O::SIZE:
      return O::_SIZE_OF;
    case O::DEPTH:
      return O::_DEPTH_OF;
    case O::COUNT:
      return O::_COUNT_OF;
    case O::LENGTH:
      return O::_LENGTH_OF;
    case O::NAME:
      return O::_NAME_OF;
    case O::LINE:
      return O::_LINE_OF;
    case O::COLUMN:
      return O::_COLUMN_OF;
    case O::IS:
      return O::_VALUE_IS;
    case O::GET:
      return O::_GET_VALUE_OF_VARIANT;
    case O::TYPE:
      return O::_TYPE_OF;
    case O::SYMBOL:
      return O::_SYMBOL_OF;
    case O::DISCRIMINANT:
      return O::_DISCRIMINANT_OF;
    default:
      break;
  }
  return O::__ERROR;
}


constexpr requite::Opcode getUniversalizedAscribed(requite::Opcode opcode) {
  using namespace requite;
  using O = Opcode;
  switch (opcode) {
    case O::BORROW:
      return O::_BORROW_OF_ASCRIBED;
    case O::STEAL:
      return O::_STEAL_OF_ASCRIBED;
    case O::VIEW:
      return O::_VIEW_OF_ASCRIBED;
    case O::SLICE:
      return O::_SLICE_OF_ASCRIBED;
    default:
      break;
  }
  return O::__ERROR;
}

constexpr bool getIsValid(requite::Opcode opcode) {
  return opcode != requite::Opcode::__NONE &&
         opcode != requite::Opcode::__ERROR &&
         static_cast<unsigned>(opcode) < requite::OPCODE_COUNT;
}

constexpr bool getIsInternal(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_INTERNAL);
  return has_flags;
}

constexpr bool getIsIntermediate(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_INTERMEDIATE);
  return has_flags;
}

constexpr bool getBranchCanHaveNoSemicolon(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_THIS_CAN_HAVE_NO_SEMICOLON);
  return has_flags;
}

constexpr bool getHasSemicolonSeperatedBranches(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_HAS_SEMICOLON_SEPARATED_BRANCHES);
  return has_flags;
}

constexpr unsigned getCommaTerminatingBranchCount(requite::Opcode opcode) {
  REQUITE_ASSERT(requite::getHasSemicolonSeperatedBranches(opcode));
  const unsigned count = requite::_getMaskValue(opcode);
  return count;
}

constexpr bool getFirstCommaBranchCanBeTacit(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_FIRST_COMMA_BRANCH_CAN_BE_INFERENCE);
  return has_flags;
}

constexpr bool getLastCommaBranchCanBeTacit(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_LAST_COMMA_BRANCH_CAN_BE_INFERENCE);
  return has_flags;
}

constexpr bool getAllCommaBranchesCanBeTacit(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_ALL_COMMA_BRANCHES_CAN_BE_INFERENCE);
  return has_flags;
}

constexpr bool getCanHaveTacitCommaBranch(requite::Opcode opcode) {
  const bool has_flags = requite::_getHasFlags(
      opcode, requite::_opcode::_FIRST_COMMA_BRANCH_CAN_BE_INFERENCE |
                  requite::_opcode::_LAST_COMMA_BRANCH_CAN_BE_INFERENCE |
                  requite::_opcode::_ALL_COMMA_BRANCHES_CAN_BE_INFERENCE);
  return has_flags;
}

constexpr bool getIsConverging(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_CONVERGING);
  return has_flags;
}

constexpr bool getHasTextData(requite::Opcode opcode) {
  return opcode == requite::Opcode::__STRING_LITERAL ||
         opcode == requite::Opcode::__CODEUNIT_LITERAL ||
         opcode == requite::Opcode::__IDENTIFIER_LITERAL;
}

} // namespace requite