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
  FUNCTION_ADDRESS,
  FUNCTION_ADDRESS_OF,
  BORROW,
  BORROW_OF,
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
  INSTANTIATE_ARRAY,
  INSTANTIATE_REFERENCE,
  INSTANTIATE_POINTER,
  INSTANTIATE_SLICE,
  INSTANTIATE_GREATEST,

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
  FUNCTION,
  IMPLEMENT_FUNCTION,
  CONSTRUCTOR,
  LAYOUT_CONSTRUCTOR,

  // CONTROL FLOW
  RETURN,
  BREAK,
  BREAK_OF,
  CONTINUE,
  CONTINUE_OF,

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
  // reference to extended value of function or extension_function.
  THIS,
  // value returned from a function.
  RESULT,
  // get information about location of function call
  CALLSITE,

  // BUILTIN TYPES
  SYMBOL,
  INFERENCE,
  EXPRESSION,
  VOID,
  NO_RETURN,
  BOOLEAN,
  HALF,
  SINGLE,
  DOUBLE,
  QUADRUPLE,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  BFLOAT16,
  SIGNED_INTEGER,
  UNSIGNED_INTEGER,
  FAST_SIGNED_INTEGER,
  FAST_UNSIGNED_INTEGER,
  LEAST_SIGNED_INTEGER,
  LEAST_UNSIGNED_INTEGER,
  SIGNED_INDEX,
  UNSIGNED_INDEX,
  SIGNED_ADDRESS,
  UNSIGNED_ADDRESS,
  CHAR,
  ASCII,
  UTF8,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS_TYPE,
  FIRST_VARIADIC_ARGUMENT,
  FIRST_VARIADIC_ARGUMENT_OF,
  NEXT_VARIADIC_ARGUMENT,
  NEXT_VARIADIC_ARGUMENT_OF,
  VARIADIC_ARGUMENTS,

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
  PARTIAL_MUTABLE,
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
  // var
  NO_VAR,
  PARTIAL_VAR,
  VAR,
  // volatile
  NO_VOLATILE,
  VOLATILE,
  // atomic
  NO_ATOMIC,
  ATOMIC,
  // null_terminate
  NO_NULL_TERMINATE,
  NULL_TERMINATE,

  // QUALIFIER TYPES
  VAR_QUALIFIER,            // no_var vs var vs partial_var
  VOLATILE_QUALIFIER,       // no_volatile vs volatile
  ATOMIC_QUALIFIER,         // no_atomic vs atomic
  NULL_TERMINATE_QUALIFIER, // no_null_terminate vs null_terminate

  // MODIFIER TYPES
  MODIFIER,

  // REFLECTIONS
  MEMBER_OF,
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
  POLYMORPH,
  POLYMORPH_OF,
  OVERLOAD_OF,
  OVERLOAD_RANGE,
  OVERLOAD_RANGE_OF,
  SPECIALIZATION_RANGE,
  SPECIALIZATION_RANGE_OF,
  WEIGHT_LEVEL,
  WEIGHT_LEVEL_OF,
  WEIGHT_LEVEL_RANGE,
  WEIGHT_LEVEL_RANGE_OF,
  WEIGHT_OF,
  TEMPLATE_OF,
  TEMPLATE_RANGE,
  TEMPLATE_RANGE_OF,
  CONSTRUCTOR_RANGE,
  CONSTRUCTOR_RANGE_OF,
  RESOLVE_TEMPLATE,
  RESOLVE_TEMPLATE_OF,
  RESOLVE_FUNCTION,
  RESOLVE_FUNCTION_OF,
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
  IS_QUALIFIER_TYPE,
  IS_QUALIFIER_TYPE_OF,

  LAST
};

enum class SymbolKind : rq::EntityId {
  NONE,

  // LITERALS
  INTEGER_LITERAL_TYPE,
  FLOAT_LITERAL_TYPE,
  STRING_LITERAL_TYPE,
  CODEUNIT_LITERAL_TYPE,

  // CONTEXTUAL VALUE
  UNKNOWN_VALUE,
  VALUE_VALUE,
  INDEX_VALUE,

  // CONTEXTUAL TYPE
  UNKNOWN_TYPE,
  INFERENCE_TYPE,
  VOID_TYPE,
  NO_RETURN_TYPE,

  // MODIFIER TYPES
  MODIFIER_TYPE,

  // QUALIFIER TYPES
  VAR_QUALIFIER_TYPE,
  VOLATILE_QUALIFIER_TYPE,
  ATOMIC_QUALIFIER_TYPE,
  NULL_TERMINATE_QUALIFIER_TYPE,

  // REFLECTIVE TYPES
  SYMBOL_TYPE,
  SYMBOL_RANGE_TYPE,
  EXPRESSION_TYPE,
  EXPRESSION_RANGE_TYPE,

  // PLATFORM PRIMITIVE TYPES
  BOOLEAN_TYPE,
  HALF_TYPE,
  SINGLE_TYPE,
  DOUBLE_TYPE,
  QUADRUPLE_TYPE,
  SIGNED_INTEGER_TYPE,
  UNSIGNED_INTEGER_TYPE,
  SIGNED_INDEX_TYPE,
  UNSIGNED_INDEX_TYPE,
  SIGNED_ADDRESS_TYPE,
  UNSIGNED_ADDRESS_TYPE,
  CHAR_TYPE,

  // FITTING PRIMITIVE TYPES
  FAST_SIGNED_INTEGER_TYPE,
  FAST_UNSIGNED_INTEGER_TYPE,
  LEAST_SIGNED_INTEGER_TYPE,
  LEAST_UNSIGNED_INTEGER_TYPE,

  // STANDARD PRIMITIVE TYPE
  BINARY16_TYPE,
  BINARY32_TYPE,
  BINARY64_TYPE,
  BINARY128_TYPE,
  BFLOAT16_TYPE,
  ASCII_TYPE,
  UTF8_TYPE,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS_TYPE,

  // SCALED PRIMITIVE TYPES
  SCALED_SIGNED_INTEGER_TYPE,
  SCALED_UNSIGNED_INTEGER_TYPE,

  // SUBTYPES
  ARRAY_SUBTYPE,
  GREATEST_SUBTYPE,
  REFERENCE_SUBTYPE,
  POINTER_SUBTYPE,
  SLICE_SUBTYPE,
  INFERENCE_COUNT_ARRAY_SUBTYPE,

  // MODULES
  MODULE,

  // IMPORTS
  IMPORT,

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
  FUNCTION_SPECIALIZATION_SET,

  // ARITHMETIC SEQUENCES
  ARITHMETIC_INTERVAL_TYPE,
  INFINITE_ARITHMETIC_SEQUENCE_TYPE,
  FINITE_ARITHMETIC_SEQUENCE_TYPE,

  // EAGER DECLARATIONS
  ANCHOR,
  ENUMERATOR,
  EAGER_VARIABLE,

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

  // SYMBOL TABLES
  C_TABLE,
  TOP_TABLE,

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

  // NAMED TABLE
  NAMESPACE,

  // OVERLOADS
  CLASS_OVERLOAD,
  ENUM_OVERLOAD,
  INTERFACE_OVERLOAD,
  ADAPTER_OVERLOAD,
  FUNCTION_OVERLOAD,
  LAZY_VARIABLE_OVERLOAD,

  // SPECIALIZATIONS
  CLASS_SPECIALIZATION,
  ENUM_SPECIALIZATION,
  INTERFACE_SPECIALIZATION,
  ADAPTER_SPECIALIZATION,
  FUNCTION_SPECIALIZATION,
  LAZY_VARIABLE_SPECIALIZATION,

  // TEMPLATES
  CLASS_TEMPLATE,
  ENUM_TEMPLATE,
  INTERFACE_TEMPLATE,
  ADAPTER_TEMPLATE,
  FUNCTION_TEMPLATE,
  LAZY_VARIABLE_TEMPLATE,

  // POLYMORPHS
  CLASS_POLYMORPH,
  ENUM_POLYMORPH,
  INTERFACE_POLYMORPH,
  ADAPTER_POLYMORPH,
  FUNCTION_POLYMORPH,
  LAZY_VARIABLE_POLYMORPH,

  // WEIGHT LEVELS
  CLASS_WEIGHT_LEVEL,
  ENUM_WEIGHT_LEVEL,
  INTERFACE_WEIGHT_LEVEL,
  ADAPTER_WEIGHT_LEVEL,
  FUNCTION_WEIGHT_LEVEL,
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

  // address0 = function
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

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpression() const {
    return this->_id < rq::getUnderlyingValue(rq::Keyword::LAST);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return this->_id >= rq::SYMBOL_OFFSET && this->_id < rq::CONSTANT_OFFSET;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const {
    return this->_id >= rq::OPCODE_OFFSET;
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