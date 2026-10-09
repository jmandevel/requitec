#pragma once

#include <rq/iterators.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/Hashing.h>

#include <cstdint>

namespace rq {

using EntityId = std::uint32_t;

enum class Keyword : rq::EntityId {
  // this is the initial keyword set for expressions. it must be overwritten
  // later!
  NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  STRING_LITERAL,
  // a literal that represents a single text character.
  CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  IDENTIFIER_LITERAL,

  // ERRORS
  ERROR,

  // SITUATIONAL
  UNSITUATED_PARENTHESIS_GROUP,
  UNSITUATED_EQUAL_OPERATOR,
  UNSITUATED_ASCRIBE_MODIFIER,
  UNSITUATED_ASCRIBE_QUALIFIER,
  UNSITUATED_CHAIN,
  UNSITUATED_TRAIN,

  // LOGICAL
  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_COMPLEMENT,
  LOGICAL_AND_WITH_SHORTCIRCUIT,
  LOGICAL_OR_WITH_SHORTCIRCUIT,

  // COMPARISON
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  EQUAL,
  NOT_EQUAL,

  // APPLY
  EXTEND,
  INSTANTIATE_EXTENSION,
  INSTANTIATE_CONFORMITY,
  INSTANTIATE_ADAPTION,
  BINDING,
  UPBINDING,
  ASCRIBE_QUALIFIER,
  ASCRIBE_MODIFIER,
  ASCRIBE_RECIEVER_QUALIFIER,
  INSTANTIATE_MODIFIER,
  INSTANTIATE_QUALIFIER,
  // turn a string into an identifier
  IDENTIFY,
  IDENTIFY_OF,

  // JUXTAPOSITIONAL
  CONCATENATE,
  APPEND,

  // ARITHMETIC
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  NEGATE,

  // CASTS
  AS,
  AS_OF,
  OF,
  OF_OF,
  CAST,
  CAST_OF,
  BITWISE_CAST,
  BITWISE_CAST_OF,
  SIGNATURE_CAST,
  SIGNATURE_CAST_OF,

  // BITWISE
  BITWISE_OR,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_COMPLEMENT,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  // MEMORY
  ASSIGN,
  CONTENT,
  CONTENT_OF,
  ADDRESS,
  ADDRESS_OF,
  SLICE,
  SLICE_OF,
  PROCEDURE_ADDRESS,
  PROCEDURE_ADDRESS_OF,
  REF,
  REF_OF,
  DATA_ADDRESS,
  DATA_ADDRESS_OF,
  MOVE,
  MOVE_OF,
  TAKE,
  TAKE_OF,
  CALL,
  EMPLACE,
  EMPLACE_OF,
  INVOKE,
  INVOKE_OF,
  COMPOSE,
  COMPOSE_OF,
  DECOMPOSE,
  DECOMPOSE_OF,
  FORGET,
  FORGET_OF,
  INIT,
  INIT_OF,
  INPLACE_DESTROY,
  INPLACE_DESTROY_OF,
  INPLACE_INIT,
  INPLACE_INIT_OF,

  // SUBTYPE
  INSTANTIATE_SUBTYPE,
  ARRAY_SUBTYPE,
  REFERENCE_SUBTYPE,
  PTR_SUBTYPE,
  SLICE_SUBTYPE,
  SPLIT_SUBTYPE,

  // PARAMETER RULES
  POSITIONAL_PARAMETERS_END,
  NONPOSITIONAL_PARAMETERS_BEGIN,
  LOCKED_PARAMETERS_BEGIN,
  NONAME,

  // BRACES
  INSTANTIATE_TUPLE,
  NAMED_ELEMENT,
  INSTANTIATE_LAYOUT,
  SPECIALIZE,

  // PROCEDURES
  NAMED_ARGUMENT,
  INSTANTIATE_SIGNATURE,
  PLACEMENT,
  COMPOSITION,
  DEFAULT_VALUE_PARAMETER,
  PROCEDURE,
  IMPLEMENT_PROCEDURE,
  CONSTRUCTOR,
  LAYOUT_CONSTRUCTOR,

  // CONTROL FLOW
  RETURN,

  // DECLARED TYPES
  CLASS,
  ENUM,
  INTERFACE,
  ADAPTER,

  // VALUES
  ARRAY,
  TRUE,
  FALSE,
  // vignette value.
  VALUE,
  // vignette index.
  INDEX,
  // reference to reciever
  THIS,
  // get information about location of a procedure call
  CALLSITE,

  // BUILTIN TYPES
  SYMBOL,
  INFERENCE,
  EXPRESSION,
  VOID,
  NO_RETURN,
  BOOL,
  HALF,
  SINGLE,
  DOUBLE,
  QUADRUPLE,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  BFLOAT16,
  SINT,
  UINT,
  FSINT,
  FUINT,
  LSINT,
  LUINT,
  SSIZE,
  USIZE,
  SINDEX,
  UINDEX,
  CHAR,
  ASCII,
  UTF8,

  // DYNAMIC_VARIADIC ARGUMENTS
  DYNAMIC_VARIADIC_ARGUMENTS_TYPE,
  FIRST_DYNAMIC_VARIADIC_ARGUMENT,
  FIRST_DYNAMIC_VARIADIC_ARGUMENT_OF,
  NEXT_DYNAMIC_VARIADIC_ARGUMENT,
  NEXT_DYNAMIC_VARIADIC_ARGUMENT_OF,
  DYNAMIC_VARIADIC_ARGUMENTS,

  // SCOPES
  IF_CHAIN,
  SWITCH_CHAIN,
  SPIN_CHAIN,
  IF,
  ELSE_IF,
  ELSE,
  SWITCH,
  CASE,
  DEFAULT,
  FOR,
  WHILE,
  SPIN,
  WEAVE,
  SCOPE,
  FOLD,
  BREAK,
  BREAK_OF,
  CONTINUE,
  CONTINUE_OF,

  // RANGES
  ARITHMETIC_SEQUENCE,
  ARITHMETIC_SEQUENCE_CONDITION_LESS,
  ARITHMETIC_SEQUENCE_CONDITION_GREATER,
  ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL,
  ARITHMETIC_SEQUENCE_STEP_ADD,
  ARITHMETIC_SEQUENCE_STEP_SUBTRACT,
  ARITHMETIC_SEQUENCE_STEP_MULTIPLY,
  ARITHMETIC_SEQUENCE_STEP_DIVIDE,
  ARITHMETIC_SEQUENCE_STEP_MODULUS,

  // TABLE GRAPH
  IMPORT,
  NAMESPACE,
  C,
  TOP,

  // HINTS
  DEBUG_BREAK,
  ABORT,
  ASSERT,
  UNREACHABLE,
  ASSUME,

  // MODIFIERS
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
  // assignment_kind
  VARIABLE,
  ENUMERATOR,
  ALIAS,
  PARAMETER,
  PORTAL,
  // evaluation_time
  EAGER,
  LAZY,
  // execution_time (eager symbols and statements)
  POST,
  PRE,
  // tenure (lazy symbols)
  RUNTIME,
  GENTIME,
  ANYTIME,
  // initialization_time
  PRESET,
  SINGLETON,
  // static_closure
  CAPTURE,
  STATELESS,
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
  // dynamic_variadic
  INVARIADIC,
  DYNAMIC_VARIADIC,
  // offset
  BEST_LOCATION,
  LOCATION,
  // lazy_declaration_kind
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

  // QUALIFIERS
  // mut
  NO_MUT,
  PARTIAL_MUT,
  MUT,
  // volatile
  NO_VOLATILE,
  VOLATILE,
  // atomic
  NO_ATOMIC,
  ATOMIC,
  // null_terminate
  NO_NULL_TERMINATE,
  NULL_TERMINATE,
  // margin
  NO_MARGIN,
  MARGIN,

  // ATTRIBUTE TYPES
  MODIFIER,
  QUALIFIER,

  // REFLECTIONS
  MEMBER_OF,
  WITHOUT,
  WITHOUT_OF,
  BAKE,
  BAKE_OF,
  IGNORE,
  IGNORE_OF,
  BYTE_SIZE,
  BYTE_SIZE_OF,
  BIT_DEPTH,
  BIT_DEPTH_OF,
  ELEMENT_COUNT,
  ELEMENT_COUNT_OF,
  SNIPPET,
  SNIPPET_OF,
  NAME,
  NAME_OF,
  LINE,
  LINE_OF,
  COLUMN,
  COLUMN_OF,
  IS,
  IS_OF,
  HOLDS,
  HOLDS_OF,
  TYPE,
  TYPE_OF,
  HAS_MEMBER,
  HAS_MEMBER_OF,
  HAS,
  HAS_OF,
  GET,
  GET_OF,
  SIGNATURE,
  SIGNATURE_OF,
  // make a unique clone of a type that is not implicitly convertable
  // can use platform specific values for bit depth only if type is a synonym
  SYNONYM,
  SYNONYM_OF,
  AT,
  AT_OF,
  MAIN,
  MAIN_OF,
  DESTRUCTOR,
  DESTRUCTOR_OF,
  DESTROY,
  DESTROY_OF,
  UNDERLYING_VALUE,
  UNDERLYING_VALUE_OF,
  UNDERLYING_TYPE,
  UNDERLYING_TYPE_OF,
  REFLECT,
  REFLECT_OF,
  OVERLOAD_OF,
  OVERLOAD_RANGE,
  OVERLOAD_RANGE_OF,
  SPECIALIZATION_RANGE,
  SPECIALIZATION_RANGE_OF,
  WEIGHT_OF,
  TEMPLATE_OF,
  TEMPLATE_RANGE,
  TEMPLATE_RANGE_OF,
  CONSTRUCTOR_RANGE,
  CONSTRUCTOR_RANGE_OF,
  RESOLVE_TEMPLATE,
  RESOLVE_TEMPLATE_OF,
  RESOLVE_PROCEDURE,
  RESOLVE_PROCEDURE_OF,
  RESOLVE_ADAPTER,
  RESOLVE_ADAPTER_OF,
  IS_TYPE,
  IS_TYPE_OF,
  IS_RANGE_TYPE,
  IS_RANGE_TYPE_OF,
  IS_PLACEMENT_TYPE,
  IS_PLACEMENT_TYPE_OF,
  IS_SIGNED_TYPE,
  IS_SIGNED_TYPE_OF,
  IS_UNSIGNED_TYPE,
  IS_UNSIGNED_TYPE_OF,
  IS_INTEGER_TYPE,
  IS_INTEGER_TYPE_OF,
  IS_FLOAT_TYPE,
  IS_FLOAT_TYPE_OF,
  IS_BINARY_TYPE,
  IS_BINARY_TYPE_OF,
  IS_BFLOAT_TYPE,
  IS_BFLOAT_TYPE_OF,
  IS_STRING_TYPE,
  IS_STRING_TYPE_OF,
  IS_CODEUNIT_TYPE,
  IS_CODEUNIT_TYPE_OF,

  LAST
};

enum class SymbolKind : rq::EntityId {
  NONE,

  // LITERALS
  INTEGER_LITERAL_TYPE,
  FLOAT_LITERAL_TYPE,
  STRING_LITERAL_TYPE,
  CODEUNIT_LITERAL_TYPE,

  // FIGURATIVE
  UNKNOWN_TYPE,
  INFERENCE_TYPE,
  VOID_TYPE,
  NO_RETURN_TYPE,

  // ATTRIBUTE TYPES
  MODIFIER_TYPE,
  QUALIFIER_TYPE,

  // REFLECTION TYPES
  SYMBOL_TYPE,
  SYMBOL_RANGE_TYPE,
  EXPRESSION_TYPE,
  EXPRESSION_RANGE_TYPE,

  // PLATFORM FITTING TYPES
  FSINT,
  FUINT,
  LSINT,
  LUINT,

  // PLATFORM PRIMITIVE TYPES
  BOOLEAN_TYPE,
  HALF_TYPE,
  SINGLE_TYPE,
  DOUBLE_TYPE,
  QUADRUPLE_TYPE,
  SINT,
  UINT,
  SSIZE,
  USIZE,
  SINDEX,
  UINDEX,
  CHAR_TYPE,

  // STANDARD PRIMITIVE TYPE
  BINARY16_TYPE,
  BINARY32_TYPE,
  BINARY64_TYPE,
  BINARY128_TYPE,
  BFLOAT16_TYPE,
  ASCII_TYPE,
  UTF8_TYPE,

  // SCALED PRIMITIVE TYPES
  SCALED_SINT,
  SCALED_UINT,
  SCALED_FSINT,
  SCALED_FUINT,
  SCALED_LSINT,
  SCALED_LUINT,

  // DYNAMIC_VARIADIC ARGUMENTS
  DYNAMIC_VARIADIC_ARGUMENTS_TYPE,

  // SUBTYPES
  ARRAY_SUBTYPE,
  REF_SUBTYPE,
  PTR_SUBTYPE,
  SLICE_SUBTYPE,
  SPLIT_SUBTYPE,
  INFERENCE_COUNT_ARRAY_SUBTYPE,

  // MODULES
  MODULE,

  // IMPORTS
  IMPORT,

  // REALIZATION
  REALIZATION,

  // CONFORMITY
  CONFORMITY,

  // ADAPTION
  ADAPTION,

  // JUXT LIST
  JUXT_LIST_TYPE,
  JUXT_LIST_ITEM,

  // SPECIALIZATION SET ARGUMENT
  SPECIALIZATION_SET_ARGUMENT,

  // SPECIALIZATION SET
  SPECIALIATION_SET,
  ADAPTER_SPECIALIZATION_SET,
  PROCEDURE_SPECIALIZATION_SET,

  // ARITHMETIC SEQUENCES
  ARITHMETIC_INTERVAL_TYPE,
  INFINITE_ARITHMETIC_SEQUENCE_TYPE,
  FINITE_ARITHMETIC_SEQUENCE_TYPE,

  // EAGER DECLARATIONS
  ANCHOR,
  ENUMERATOR,
  DYNAMIC_EAGER_VARIABLE,
  STATIC_EAGER_VARIABLE,

  // PARAMETERS
  PARAMETER,

  // PARAMETER LISTS
  SIGNATURE_TYPE,
  LAYOUT_TYPE,

  // PLACEMENTS
  PLACEMENT_TYPE,

  // COMPOSITIONS
  COMPOSITION_COMPONENT,
  COMPOSITION_TYPE,

  // SYNONYMS
  SYNONYM_TYPE,

  // ROUTES
  ALIAS,
  PORTAL,

  // SYMBOL TABLES
  C_TABLE,
  NAMESPACE,

  // EAGER STATEMENTS
  IF_STATEMENT,
  ELSE_IF_STATEMENT,
  ELSE_STATEMENT,
  SWITCH_STATEMENT,
  CASE_STATEMENT,
  DEFAULT_STATEMENT,
  FOR_STATEMENT,
  WHILE_STATEMENT,
  SPIN_STATEMENT,
  WEAVE_STATEMENT,
  SCOPE_STATEMENT,

  // OVERLOADS
  CLASS_OVERLOAD,
  ENUM_OVERLOAD,
  INTERFACE_OVERLOAD,
  ADAPTER_OVERLOAD,
  PROCEDURE_OVERLOAD,
  LAZY_VARIABLE_OVERLOAD,

  // SPECIALIZATIONS
  CLASS_SPECIALIZATION,
  ENUM_SPECIALIZATION,
  INTERFACE_SPECIALIZATION,
  ADAPTER_SPECIALIZATION,
  PROCEDURE_SPECIALIZATION,
  LAZY_VARIABLE_SPECIALIZATION,

  // TEMPLATES
  CLASS_TEMPLATE,
  ENUM_TEMPLATE,
  INTERFACE_TEMPLATE,
  ADAPTER_TEMPLATE,
  PROCEDURE_TEMPLATE,
  LAZY_VARIABLE_TEMPLATE,

  // POLYMORPHS
  CLASS_POLYMORPH,
  ENUM_POLYMORPH,
  INTERFACE_POLYMORPH,
  ADAPTER_POLYMORPH,
  PROCEDURE_POLYMORPH,
  LAZY_VARIABLE_POLYMORPH,

  // WEIGHT LEVELS
  CLASS_WEIGHT_LEVEL,
  ENUM_WEIGHT_LEVEL,
  INTERFACE_WEIGHT_LEVEL,
  ADAPTER_WEIGHT_LEVEL,
  PROCEDURE_WEIGHT_LEVEL,
  LAZY_VARIABLE_WEIGHT_LEVEL,

  LAST
};

enum class ConstantKind : rq::EntityId {
  NONE,

  WORD,
  ARRAY,
  DATA_ARRAY,
  SYMBOL,
  ENTITY_RANGE,
  ARITHMETIC_SEQUENCE,

  LAST
};

enum class Opcode : rq::EntityId {
  NONE,

  // address0 = head
  // address1 = tail
  STATEMENT,

  // address0 = lvalue
  // address1 = rvalue
  ASSIGN,

  // address0 = variable
  REF,

  // address0 = condition rvalue
  // address1 = false target block
  CONDITIONAL_JUMP,
  LIKELY_CONDITIONAL_JUMP,
  UNLIKELY_CONDITIONAL_JUMP,

  // address0 = block
  JUMP,

  // LOGICAL
  // address0 = head
  // address1 = tail
  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_AND_WITH_SHORTCIRCUIT,
  LOGICAL_OR_WITH_SHORTCIRCUIT,

  // address0 = rvalue
  LOGICAL_COMPLEMENT,

  // address0 = rvalue0
  // address1 = rvalue1
  RVALUE_PAIR,

  RETURN,

  // address0 = proc
  // address1 = push_args
  CALL,

  // COMPARISON
  // address0 = operand type
  // address1 = rvalue pair
  LESS,
  GREATER,
  LESS_EQUAL,
  GREATER_EQUAL,
  EQUAL,
  NOT_EQUAL,

  // ARITHMETIC
  // address0 = head
  // address1 = tail
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,

  // address0 = rvalue
  NEGATE,

  LAST
};

constexpr rq::EntityId KEYWORD_OFFSET = 0;

constexpr rq::EntityId SYMBOL_OFFSET =
    rq::getUnderlyingValue(rq::Keyword::LAST);

[[nodiscard]] constexpr RQ_ALWAYS_INLINE rq::EntityId
getSymbolId(rq::SymbolKind kind) {
  return rq::SYMBOL_OFFSET + rq::getUnderlyingValue(kind);
}

constexpr rq::EntityId CONSTANT_OFFSET =
    rq::SYMBOL_OFFSET + rq::getUnderlyingValue(rq::SymbolKind::LAST);

constexpr rq::EntityId OPCODE_OFFSET =
    rq::CONSTANT_OFFSET + rq::getUnderlyingValue(rq::ConstantKind::LAST);

constexpr rq::EntityId CFG_BLOCK_ID =
    OPCODE_OFFSET + rq::getUnderlyingValue(rq::Opcode::LAST);

[[nodiscard]] constexpr RQ_ALWAYS_INLINE rq::EntityId
getId(rq::Keyword keyword) {
  return static_cast<rq::EntityId>(keyword) + rq::KEYWORD_OFFSET;
}

[[nodiscard]] constexpr RQ_ALWAYS_INLINE rq::EntityId
getId(rq::SymbolKind kind) {
  return static_cast<rq::EntityId>(kind) + rq::SYMBOL_OFFSET;
}

[[nodiscard]] constexpr RQ_ALWAYS_INLINE rq::EntityId
getId(rq::ConstantKind kind) {
  return static_cast<rq::EntityId>(kind) + rq::CONSTANT_OFFSET;
}

[[nodiscard]] constexpr RQ_ALWAYS_INLINE rq::EntityId getId(rq::Opcode kind) {
  return static_cast<rq::EntityId>(kind) + rq::OPCODE_OFFSET;
}

struct Entity;

struct DottedInstructionIterator final {
  using Self = rq::DottedInstructionIterator;
  using value_type = rq::Entity;
  using reference = rq::Entity &;
  using pointer = rq::Entity *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Entity *_entity_ptr{nullptr};
  rq::Opcode _opcode{rq::Opcode::NONE};

  DottedInstructionIterator() = default;
  explicit DottedInstructionIterator(rq::Entity *entity_ptr, rq::Opcode opcode);
  DottedInstructionIterator(const Self &) = default;
  DottedInstructionIterator(Self &&) = default;
  ~DottedInstructionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE rq::DottedInstructionIterator &operator++();
  RQ_ALWAYS_INLINE rq::DottedInstructionIterator operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstDottedInstructionIterator final {
  using Self = rq::ConstDottedInstructionIterator;
  using value_type = const rq::Entity;
  using reference = const rq::Entity &;
  using pointer = rq::Entity *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Entity *_entity_ptr = nullptr;
  rq::Opcode _opcode{rq::Opcode::NONE};

  ConstDottedInstructionIterator() = default;
  explicit ConstDottedInstructionIterator(const rq::Entity *entity_ptr,
                                          rq::Opcode opcode);
  ConstDottedInstructionIterator(const Self &) = default;
  ConstDottedInstructionIterator(Self &&) = default;
  ~ConstDottedInstructionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE rq::ConstDottedInstructionIterator &operator++();
  RQ_ALWAYS_INLINE rq::ConstDottedInstructionIterator operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

enum class SymbolInfoFlags : std::uint64_t {
  NONE = 0,

  // SYMBOL CLASSIFICATION
  SIMPLE_SYMBOL = rq::getBit(0),
  LITERAL = rq::getBit(1),
  FIGURATIVE = rq::getBit(2),
  ATTRIBUTE_TYPE = rq::getBit(3),
  REFLECTION_TYPE = rq::getBit(4),
  FITTING_PRIMITIVE_TYPE = rq::getBit(5),
  PLATFORM_PRIMITIVE_TYPE = rq::getBit(6),
  STANDARD_PRIMITIVE_TYPE = rq::getBit(7),
  SCALED_PRIMITIVE_TYPE = rq::getBit(8),
  SUBTYPE = rq::getBit(9),
  SIMPLE_SUBTYPE = rq::getBit(10),
  ARITHMETIC_SEQUENCE_TYPE = rq::getBit(11),
  SPECIALIZATION_SET = rq::getBit(12),
  PARAMETER_LIST = rq::getBit(13),
  TABLE_MEMBER = rq::getBit(14),
  EAGER_DECLARATION = rq::getBit(15),
  EAGER_VARIABLE = rq::getBit(16),
  ROUTE = rq::getBit(17),
  POLYMORPH = rq::getBit(18),
  WEIGHT_LEVEL = rq::getBit(19),
  TEMPLATE = rq::getBit(20),
  SYMBOL_TABLE = rq::getBit(21),
  EAGER_SCOPE = rq::getBit(22),
  NAMED_TABLE = rq::getBit(23),
  LAZY_DECLARATION = rq::getBit(24),
  CLASS_IMPLEMENTATION = rq::getBit(25),
  ENUM_IMPLEMENTATION = rq::getBit(26),
  INTERFACE_IMPLEMENTATION = rq::getBit(27),
  ADAPTER_IMPLEMENTATION = rq::getBit(28),
  PROCEDURE_IMPLEMENTATION = rq::getBit(29),
  LAZY_VARIABLE_IMPLEMENTATION = rq::getBit(30),

  // SYMBOL DETAIL
  IS_RUNTIME_TYPE = rq::getBit(48),
  IS_GENTIME_TYPE = rq::getBit(49),
  IS_CONTEXTUAL = rq::getBit(50),
  IS_INTEGER = rq::getBit(51),
  IS_FAST = rq::getBit(52),
  IS_LEAST = rq::getBit(53),
  IS_SIZE = rq::getBit(54),
  IS_INDEX = rq::getBit(55),
  IS_FLOAT = rq::getBit(56),
  IS_BINARY = rq::getBit(57),
  IS_BFLOAT = rq::getBit(58),
  IS_CODEUNIT = rq::getBit(59),
  IS_SIGNED = rq::getBit(60),
  IS_UNSIGNED = rq::getBit(61),
  IS_FRAME_SCOPE = rq::getBit(62),
  IS_OBJECT_SCOPE = rq::getBit(63)
};

RQ_DEFINE_FLAGS(rq::SymbolInfoFlags);

[[nodiscard]] inline rq::SymbolInfoFlags getInfoFlags(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  using SIF = rq::SymbolInfoFlags;
  switch (kind) {
  case S::NONE:
    return SIF::NONE;

  // LITERALS
  case S::INTEGER_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_GENTIME_TYPE;
  case S::FLOAT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_GENTIME_TYPE;
  case S::STRING_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_GENTIME_TYPE;
  case S::CODEUNIT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_GENTIME_TYPE;

  // FIGURATIVE TYPE
  case S::UNKNOWN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FIGURATIVE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_CONTEXTUAL;
  case S::INFERENCE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FIGURATIVE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_CONTEXTUAL;
  case S::VOID_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FIGURATIVE | SIF::IS_RUNTIME_TYPE;
  case S::NO_RETURN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FIGURATIVE | SIF::IS_RUNTIME_TYPE;

  // ATTRIBUTE TYPES
  case S::MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_GENTIME_TYPE;
  case S::QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_GENTIME_TYPE;

  // REFLECTION TYPES
  case S::SYMBOL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::SYMBOL_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::EXPRESSION_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::EXPRESSION_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE |
           SIF::IS_GENTIME_TYPE;

  // PLATFORM FITTING TYPES
  case S::FSINT:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_CONTEXTUAL | SIF::IS_INTEGER | SIF::IS_FAST | SIF::IS_SIGNED;
  case S::FUINT:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_CONTEXTUAL | SIF::IS_INTEGER | SIF::IS_FAST |
           SIF::IS_UNSIGNED;
  case S::LSINT:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_CONTEXTUAL | SIF::IS_INTEGER | SIF::IS_LEAST |
           SIF::IS_SIGNED;
  case S::LUINT:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_CONTEXTUAL | SIF::IS_INTEGER | SIF::IS_LEAST |
           SIF::IS_UNSIGNED;

  // PLATFORM PRIMITIVE TYPES
  case S::BOOLEAN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE;
  case S::HALF_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_SIGNED;
  case S::SINGLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_SIGNED;
  case S::DOUBLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_SIGNED;
  case S::QUADRUPLE_TYPE:
    return SIF::SIMPLE_SUBTYPE | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_SIGNED;
  case S::SINT:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_SIGNED;
  case S::UINT:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_UNSIGNED;
  case S::SSIZE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_SIZE | SIF::IS_SIGNED;
  case S::USIZE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_SIZE | SIF::IS_UNSIGNED;
  case S::SINDEX:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_INDEX | SIF::IS_SIGNED;
  case S::UINDEX:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER |
           SIF::IS_INDEX | SIF::IS_UNSIGNED;
  case S::CHAR_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_CODEUNIT;

  // STANDARD PRIMITIVE TYPE
  case S::BINARY16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_BINARY | SIF::IS_SIGNED;
  case S::BINARY32_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_BINARY | SIF::IS_SIGNED;
  case S::BINARY64_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_BINARY | SIF::IS_SIGNED;
  case S::BINARY128_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_BINARY | SIF::IS_SIGNED;
  case S::BFLOAT16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_FLOAT |
           SIF::IS_BFLOAT | SIF::IS_SIGNED;
  case S::ASCII_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_CODEUNIT;
  case S::UTF8_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE | SIF::IS_CODEUNIT;

  // SCALED PRIMITIVE TYPES
  case S::SCALED_SINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_SIGNED;
  case S::SCALED_UINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_UNSIGNED;
  case S::SCALED_FSINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_FAST |
           SIF::IS_SIGNED;
  case S::SCALED_FUINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_FAST |
           SIF::IS_UNSIGNED;
  case S::SCALED_LSINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_LEAST |
           SIF::IS_SIGNED;
  case S::SCALED_LUINT:
    return SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_INTEGER | SIF::IS_LEAST |
           SIF::IS_UNSIGNED;

  // DYNAMIC_VARIADIC ARGUMENTS
  case S::DYNAMIC_VARIADIC_ARGUMENTS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;

  // SUBTYPES
  case S::ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE;
  case S::REF_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::PTR_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::SLICE_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::SPLIT_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;

  // MODULES
  case S::MODULE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;

  // IMPORTS
  case S::IMPORT:
    return SIF::NONE;

  // REALIZATION
  case S::REALIZATION:
    return SIF::NONE;

  // CONFORMITY
  case S::CONFORMITY:
    return SIF::NONE;

  // ADAPTION
  case S::ADAPTION:
    return SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE;

  // JUXT LIST
  case S::JUXT_LIST_TYPE:
    return SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE;
  case S::JUXT_LIST_ITEM:
    return SIF::NONE;

  // SPECIALIZATION SET ARGUMENT
  case S::SPECIALIZATION_SET_ARGUMENT:
    return SIF::NONE;

  // SPECIALIZATION SET
  case S::SPECIALIATION_SET:
    return SIF::NONE;
  case S::ADAPTER_SPECIALIZATION_SET:
    return SIF::NONE;
  case S::PROCEDURE_SPECIALIZATION_SET:
    return SIF::NONE;

  // ARITHMETIC SEQUENCES
  case S::ARITHMETIC_INTERVAL_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE;

  // EAGER DECLARATIONS
  case S::ANCHOR:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION;
  case S::ENUMERATOR:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION;
  case S::DYNAMIC_EAGER_VARIABLE:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION | SIF::EAGER_VARIABLE;
  case S::STATIC_EAGER_VARIABLE:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION | SIF::EAGER_VARIABLE;

  // PARAMETERS
  case S::PARAMETER:
    return SIF::NONE;

  // PARAMETER LISTS
  case S::SIGNATURE_TYPE:
    return SIF::PARAMETER_LIST;
  case S::LAYOUT_TYPE:
    return SIF::PARAMETER_LIST;

  // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return SIF::NONE;

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return SIF::NONE;
  case S::COMPOSITION_TYPE:
    return SIF::NONE;

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return SIF::NONE;

  // ROUTES
  case S::ALIAS:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION | SIF::ROUTE;
  case S::PORTAL:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION | SIF::ROUTE;

  // SYMBOL TABLES
  case S::C_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;
  case S::NAMESPACE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;

  // EAGER STATEMENTS
  case S::IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::ELSE_IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::ELSE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::SWITCH_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::CASE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::DEFAULT_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::FOR_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::WHILE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::SPIN_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::WEAVE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;
  case S::SCOPE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_DECLARATION;

  // OVERLOADS
  case S::CLASS_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::CLASS_IMPLEMENTATION |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ENUM_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::ENUM_IMPLEMENTATION |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::INTERFACE_IMPLEMENTATION |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::ADAPTER_IMPLEMENTATION |
           SIF::IS_OBJECT_SCOPE;
  case S::PROCEDURE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::PROCEDURE_IMPLEMENTATION |
           SIF::IS_FRAME_SCOPE;
  case S::LAZY_VARIABLE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::LAZY_VARIABLE_IMPLEMENTATION;

  // SPECIALIZATIONS
  case S::CLASS_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::CLASS_IMPLEMENTATION |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ENUM_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::ENUM_IMPLEMENTATION |
           SIF::ENUM_IMPLEMENTATION | SIF::IS_RUNTIME_TYPE |
           SIF::IS_GENTIME_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::INTERFACE_IMPLEMENTATION |
           SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::ADAPTER_IMPLEMENTATION |
           SIF::IS_OBJECT_SCOPE;
  case S::PROCEDURE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::PROCEDURE_IMPLEMENTATION |
           SIF::IS_FRAME_SCOPE;
  case S::LAZY_VARIABLE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::LAZY_VARIABLE_IMPLEMENTATION;

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::ENUM_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::INTERFACE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::ADAPTER_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::PROCEDURE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::LAZY_VARIABLE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;

  // POLYMORPHS
  case S::CLASS_POLYMORPH:
    return SIF::NONE;
  case S::ENUM_POLYMORPH:
    return SIF::NONE;
  case S::INTERFACE_POLYMORPH:
    return SIF::NONE;
  case S::ADAPTER_POLYMORPH:
    return SIF::NONE;
  case S::PROCEDURE_POLYMORPH:
    return SIF::NONE;
  case S::LAZY_VARIABLE_POLYMORPH:
    return SIF::NONE;

  // WEIGHT LEVELS
  case S::CLASS_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::ENUM_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::INTERFACE_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::ADAPTER_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::PROCEDURE_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::LAZY_VARIABLE_WEIGHT_LEVEL:
    return SIF::NONE;
  case S::LAST:
    break;

    // do not add default case so there is compiler warning if a case is
    // missing.
  }
  return SIF::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectionType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::REFLECTION_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFigurative(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::FIGURATIVE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsAttributeType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasSome(
      flags, SIF::FITTING_PRIMITIVE_TYPE | SIF::PLATFORM_PRIMITIVE_TYPE |
                 SIF::STANDARD_PRIMITIVE_TYPE | SIF::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSubtype(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::SIMPLE_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::ARITHMETIC_SEQUENCE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSpecializationSet(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::SPECIALIZATION_SET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::TABLE_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerDeclaration(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::EAGER_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerVariable(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::EAGER_VARIABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoute(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::ROUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerScope(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::EAGER_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLazyDeclarataion(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::LAZY_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasSome(
      flags, SIF::CLASS_IMPLEMENTATION | SIF::ENUM_IMPLEMENTATION |
                 SIF::INTERFACE_IMPLEMENTATION | SIF::ADAPTER_IMPLEMENTATION |
                 SIF::PROCEDURE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsClassImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::CLASS_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsEnumImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::ENUM_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsInterfaceImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::INTERFACE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsAdapterImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::ADAPTER_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsProcedureImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::PROCEDURE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsLazyVariableImplementation(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::LAZY_VARIABLE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags,
                        SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRuntimeType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_RUNTIME_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompileTimeType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_GENTIME_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsAnytimeType(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_RUNTIME_TYPE | SIF::IS_GENTIME_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_CONTEXTUAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, SIF::IS_INTEGER | SIF::IS_FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastInt(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_FAST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeastInt(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_LEAST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSizeInt(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_SIZE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIndexInt(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_INDEX);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_BINARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBFloat(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_BFLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_CODEUNIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_SIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_UNSIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_FRAME_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope(rq::SymbolKind kind) {
  using SIF = rq::SymbolInfoFlags;
  SIF flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, SIF::IS_OBJECT_SCOPE);
}

struct Entity {
  using Self = rq::Entity;

  rq::EntityId _id;
#if !defined(_NDEBUG)
  rq::Keyword _debug_keyword{rq::Keyword::NONE};
  rq::SymbolKind _debug_symbol_kind{rq::SymbolKind::NONE};
  rq::ConstantKind _debug_constant_kind{rq::ConstantKind::NONE};
  rq::Opcode _debug_opcode{rq::Opcode::NONE};
#endif

  explicit RQ_ALWAYS_INLINE Entity(rq::EntityId id) : _id(id) {
#if !defined(_NDEBUG)
    if (id < rq::SYMBOL_OFFSET) {
      this->_debug_keyword = static_cast<rq::Keyword>(id);
    } else if (id < rq::CONSTANT_OFFSET) {
      this->_debug_symbol_kind =
          static_cast<rq::SymbolKind>(id - rq::SYMBOL_OFFSET);
    } else if (id < rq::OPCODE_OFFSET) {
      this->_debug_constant_kind =
          static_cast<rq::ConstantKind>(id - rq::CONSTANT_OFFSET);
    } else {
      this->_debug_opcode = static_cast<rq::Opcode>(id - rq::OPCODE_OFFSET);
    }
#endif
  }
  Entity(const Self &) = delete;
  Entity(Self &&) = delete;
  ~Entity() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityId getId() const {
    return this->_id;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getUnsafeKeyword() const {
    return static_cast<rq::Keyword>(this->_id);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind getUnsafeSymbolKind() const {
    return static_cast<rq::SymbolKind>(this->_id + rq::SYMBOL_OFFSET);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getUnsafeOpcode() const {
    return static_cast<rq::Opcode>(this->_id + rq::OPCODE_OFFSET);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpression() const {
    return this->_id < rq::getUnderlyingValue(rq::Keyword::LAST);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return this->_id >= rq::SYMBOL_OFFSET && this->_id < rq::CONSTANT_OFFSET;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const {
    return this->_id >= rq::OPCODE_OFFSET && this->_id < rq::CFG_BLOCK_ID;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCfgBlock() const {
    return this->_id == rq::CFG_BLOCK_ID;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolInfoFlags
  getSymbolInfoFlags() const {
    return rq::getInfoFlags(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol() const {
    return rq::getIsSimpleSymbol(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType() const {
    return rq::getIsLiteralType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectionType() const {
    return rq::getIsReflectionType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFigurative() const {
    return rq::getIsFigurative(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAttributeType() const {
    return rq::getIsAttributeType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType() const {
    return rq::getIsPrimitiveType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStandardPrimitiveType() const {
    return rq::getIsStandardPrimitiveType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformPrimitiveType() const {
    return rq::getIsPlatformPrimitiveType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const {
    return rq::getIsSubtype(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSubtype() const {
    return rq::getIsSimpleSubtype(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequenceType() const {
    return rq::getIsArithmeticSequenceType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecializationSet() const {
    return rq::getIsSpecializationSet(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList() const {
    return rq::getIsParameterList(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember() const {
    return rq::getIsTableMember(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerDeclaration() const {
    return rq::getIsEagerDeclaration(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerVariable() const {
    return rq::getIsEagerVariable(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoute() const {
    return rq::getIsRoute(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph() const {
    return rq::getIsPolymorph(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel() const {
    return rq::getIsWeightLevel(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate() const {
    return rq::getIsTemplate(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable() const {
    return rq::getIsSymbolTable(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerScope() const {
    return rq::getIsEagerScope(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable() const {
    return rq::getIsNamedTable(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLazyDeclarataion() const {
    return rq::getIsLazyDeclarataion(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation() const {
    return rq::getIsImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassImplementation() const {
    return rq::getIsClassImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumImplementation() const {
    return rq::getIsEnumImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInterfaceImplementation() const {
    return rq::getIsInterfaceImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAdapterImplementation() const {
    return rq::getIsAdapterImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureImplementation() const {
    return rq::getIsProcedureImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLazyVariableImplementation() const {
    return rq::getIsLazyVariableImplementation(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return rq::getIsType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRuntimeType() const {
    return rq::getIsRuntimeType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompileTimeType() const {
    return rq::getIsCompileTimeType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAnytimeType() const {
    return rq::getIsAnytimeType(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual() const {
    return rq::getIsContextual(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsInteger(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastInt() const {
    return rq::getIsFastInt(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeastInt() const {
    return rq::getIsLeastInt(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSizeInt() const {
    return rq::getIsSizeInt(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIndexInt() const {
    return rq::getIsIndexInt(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary() const {
    return rq::getIsBinary(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBFloat() const {
    return rq::getIsBFloat(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunit(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return rq::getIsSigned(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return rq::getIsUnsigned(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope() const {
    return rq::getIsFrameScope(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope() const {
    return rq::getIsObjectScope(this->getUnsafeSymbolKind());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::DottedInstructionIterator>
  getDottedSubrange(rq::Opcode opcode) {
    return rq::Subrange<rq::DottedInstructionIterator>(
        rq::DottedInstructionIterator(this, opcode),
        rq::DottedInstructionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::Subrange<rq::ConstDottedInstructionIterator>
      getDottedSubrange(rq::Opcode opcode) const {
    return rq::Subrange<rq::ConstDottedInstructionIterator>(
        rq::ConstDottedInstructionIterator(this, opcode),
        rq::ConstDottedInstructionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::Subrange<rq::ConstDottedInstructionIterator>
      getConstDottedSubrange(rq::Opcode opcode) const {
    return rq::Subrange<rq::ConstDottedInstructionIterator>(
        rq::ConstDottedInstructionIterator(this, opcode),
        rq::ConstDottedInstructionIterator());
  }

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
    RQ_ASSERT(entity_ptr != nullptr, "nullptr entity");
    return true;
  }
};

} // namespace rq

namespace llvm {
RQ_ALWAYS_INLINE llvm::hash_code hash_value(const rq::Keyword &value) {
  return llvm::hash_value(rq::getUnderlyingValue(value));
}
} // namespace llvm