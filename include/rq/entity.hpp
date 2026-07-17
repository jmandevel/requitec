#pragma once

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
  UNSITUATED_ASCRIBE_LOW,
  UNSITUATED_ASCRIBE_HIGH,

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
  BINDING,
  ASCRIBE_HIGH,
  ASCRIBE_LOW,
  ASCRIBE_RECIEVER,
  INSTANTIATE_LOW_ATTRIBUTE,
  INSTANTIATE_HIGH_ATTRIBUTE,
  // turn a string into an identifier
  IDENTIFY,
  IDENTIFY_OF,
  FORK,

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
  PROCEDURE_CAST,
  PROCEDURE_CAST_OF,

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
  ADAPT,
  ADAPT_OF,
  DROP,
  DROP_OF,
  INPLACE_DESTROY,
  INPLACE_DESTROY_OF,
  INPLACE_INITIALIZE,
  INPLACE_INITIALIZE_OF,

  // SUBTYPE
  INSTANTIATE_ARRAY,
  INSTANTIATE_REFERENCE,
  INSTANTIATE_POINTER,
  INSTANTIATE_SLICE,

  // PARAMETER RULES
  POSITIONAL_PARAMETERS_END,
  NONPOSITIONAL_PARAMETERS_BEGIN,
  LOCKED_PARAMETERS_BEGIN,
  NONAME,

  // BRACES
  INSTANTIATE_TUPLE,
  INSTANTIATE_LAYOUT,
  INSTANTIATE_TEMPLATE,

  // PROCEDURES
  NAMED_ARGUMENT,
  INSTANTIATE_SIGNATURE,
  PLACEMENT,
  COMPOSITION,
  DEFAULT_VALUE_PARAMETER,
  FUNCTION,
  IMPLEMENT_FUNCTION,
  USE_FUNCTION,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  RANGE_OVER,

  // DECLARED TYPES
  CLASS,
  ENUMERATION,
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
  // vignette or reflected enumerator index.
  DISCRIMINANT,
  // value returned from a block.
  OUT,
  // reference to extended value of function or extension_function.
  THIS,
  // value returned from a function.
  RESULT,
  // get information about location of function call
  CALLSITE,

  // BUILTIN TYPES
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
  IF,
  ELSE_IF,
  ELSE,
  MATCH,
  SWITCH,
  CASE,
  DEFAULT,
  FOR,
  WHILE,
  SPIN,
  WEAVE,
  SCOPE,
  BLOCK,

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
  LABEL,

  // HINTS
  DEBUG_BREAK,
  ABORT,
  ASSERT,
  UNREACHABLE,
  ASSUME,

  // LOW ATTRIBUTES
  // anchor_attribute
  NO_ANCHOR,
  ANCHOR,
  //
  NO_OPAQUE,
  OPAQUE,
  // flank_attribute
  NO_FLANK,
  FLANK,
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
  // address_stability_attribute
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
  // ranger_attribute
  NO_RANGER,
  RANGER,

  // HIGH ATTRIBUTES
  // var_attribute
  NO_VAR,
  VAR,
  PARTIAL_VAR,
  // volatile_attribute
  NO_VOLATILE,
  VOLATILE,
  // atomic_attribute
  NO_ATOMIC,
  ATOMIC,
  // null_terminate_attribute
  NO_NULL_TERMINATE,
  NULL_TERMINATE,
  // require_attribute
  NO_REQUIRE,
  REQUIRE,
  // ensure_attribute
  NO_ENSURE,
  ENSURE,

  // LOW ATTRIBUTE TYPES
  ANCHOR_ATTRIBUTE,         // no_anchor vs anchor
  OPAQUE_ATTRIBUTE,         // no_opaque vs opaque
  FLANK_ATTRIBUTE,          // no_flank vs flank
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
  RANGER_ATTRIBUTE,         // no_ranger vs ranger

  // HIGH ATTRIBUTE TYPES
  VAR_ATTRIBUTE,            // no_var vs var vs partial_var
  VOLATILE_ATTRIBUTE,       // no_volatile vs volatile
  ATOMIC_ATTRIBUTE,         // no_atomic vs atomic
  NULL_TERMINATE_ATTRIBUTE, // no_null_terminate vs null_terminate
  REQUIRE_ATTRIBUTE,        // no_require vs require
  ENSURE_ATTRIBUTE,         // no_ensure vs ensure

  // REFLECTIONS
  REFLECT,
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
  SYMBOL,
  SYMBOL_OF,
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
  CAPTURE_OF,
  AS_EXTENSION,
  AS_EXTENSION_OF,
  INCREMENT,
  INCREMENT_OF,
  DECREMENT,
  DECREMENT_OF,
  WHILST,
  WHILST_OF,
  ELEMENT,
  ELEMENT_OF,
  AT,
  AT_OF,
  MAIN,
  MAIN_OF,
  DESTROY,
  DESTROY_OF,
  FORWARD,
  FORWARD_OF,
  BACKWARD,
  BACKWARD_OF,
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
  IS_LOW_ATTRIBUTE_TYPE,
  IS_LOW_ATTRIBUTE_TYPE_OF,
  IS_HIGH_ATTRIBUTE_TYPE,
  IS_HIGH_ATTRIBUTE_TYPE_OF,

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
  OUT_VALUE,
  THIS_VALUE,
  RESULT_VALUE,
  VALUE_VALUE,
  INDEX_VALUE,
  DISCRIMINANT_VALUE,
  CALLSITE_VALUE,

  // CONTEXTUAL TYPE
  INFERENCE_TYPE,
  VOID_TYPE,
  NO_RETURN_TYPE,

  // LOW ATTRIBUTES
  ANCHOR_ATTRIBUTE_TYPE,
  OPAQUE_ATTRIBUTE_TYPE,
  GLOBAL_ATTRIBUTE_TYPE,
  ACCESS_ATTRIBUTE_TYPE,
  PARTIAL_MUTATE_ATTRIBUTE_TYPE,
  STATIC_ATTRIBUTE_TYPE,
  CAPTURE_ATTRIBUTE_TYPE,
  INLINE_ATTRIBUTE_TYPE,
  MANGLE_ATTRIBUTE_TYPE,
  PACK_ATTRIBUTE_TYPE,
  BRANCH_TREND_ATTRIBUTE_TYPE,
  SUPPORT_STATUS_ATTRIBUTE_TYPE,
  STABLE_ADDRESS_ATTRIBUTE_TYPE,
  VARIADIC_ATTRIBUTE_TYPE,
  LOCATION_ATTRIBUTE_TYPE,
  TEMPLATE_ATTRIBUTE_TYPE,
  CONSTRAINT_ATTRIBUTE_TYPE,
  WEIGHT_ATTRIBUTE_TYPE,

  // HIGH ATTRIBUTES
  VAR_ATTRIBUTE_TYPE,
  VOLATILE_ATTRIBUTE_TYPE,
  ATOMIC_ATTRIBUTE_TYPE,
  NULL_TERMINATE_ATTRIBUTE_TYPE,
  REQUIRE_ATTRIBUTE_TYPE,
  ENSURE_ATTRIBUTE_TYPE,

  // REFLECTIVE
  SYMBOL_TYPE,
  EXPRESSION_TYPE,

  // PLATFORM PRIMITIVE
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

  // STANDARD PRIMITIVE
  BINARY16_TYPE,
  BINARY32_TYPE,
  BINARY64_TYPE,
  BINARY128_TYPE,
  BFLOAT16_TYPE,
  ASCII_TYPE,
  UTF8_TYPE,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS_TYPE,

  // SCALED PRIMITIVES
  SCALED_SIGNED_INTEGER_TYPE,
  SCALED_UNSIGNED_INTEGER_TYPE,

  // SUBTYPES
  ARRAY_SUBTYPE,

  // UNCOUNTED SUBTYPES => SUBTYPES
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

  // WEIGHTS
  CLASS_WEIGHT_LEVEL,
  ENUMERATION_WEIGHT_LEVEL,
  INTERFACE_WEIGHT_LEVEL,
  ADAPTER_WEIGHT_LEVEL,
  GLOBAL_DYNAMIC_VARIABLE_WEIGHT_LEVEL,
  GLOBAL_STATIC_VARIABLE_WEIGHT_LEVEL,
  FUNCTION_WEIGHT_LEVEL,

  // JUXTAPOSITIONAL LIST
  JUXTAPOSITIONAL_LIST_ITEM,
  JUXTAPOSITIONAL_LIST_TYPE,

  // ARITHMETIC SEQUENCES
  ARITHMETIC_INTERVAL_TYPE,
  INFINITE_ARITHMETIC_SEQUENCE_TYPE,
  FINITE_ARITHMETIC_SEQUENCE_TYPE,

  // LOCAL DECLARATIONS
  LABEL,
  ANCHOR,
  ENUMERATOR,

  // LOCAL VARIABLES => local declaration
  LOCAL_DYNAMIC_VARIABLE,
  LOCAL_STATIC_VARIABLE,
  TEMPLATE_ARGUMENT,
  PROCEDURE_ARGUMENT,

  // SYMBOL PARAMETERS
  SIGNATURE_PARAMETER,
  LAYOUT_PARAMETER,

  // TYPE PARAMETERS
  TUPLE_PARAMETER,
  PROCEDURE_PARAMETER,

  // SYMBOL PARAMETER LISTS
  SIGNATURE,
  LAYOUT,

  // TYPE PARAMETER LISTS
  TUPLE_TYPE,
  PROCEDURE_TYPE,

  // PLACEMENTS
  PLACEMENT_TYPE,

  // COMPOSITIONS
  COMPOSITION_COMPONENT,
  COMPOSITION_TYPE,

  // SYNONYMS
  SYNONYM_TYPE,

  // SYMBOL TABLES
  C,
  TOP,

  // LOCAL STATEMENTS => symbol table
  IF_STATEMENT,
  ELSE_IF_STATEMENT,
  ELSE_STATEMENT,
  MATCH_STATEMENT,
  SWITCH_STATEMENT,
  CASE_STATEMENT,
  WITH_STATEMENT,
  DEFAULT_STATEMENT,
  FOR_STATEMENT,
  WHILE_STATEMENT,
  SPIN_STATEMENT,
  WEAVE_STATEMENT,
  SCOPE_STATEMENT,

  // NAMED TABLE
  NAMESPACE,

  // INSTANCES => global declaration => named table => symbol table`
  CLASS_TYPE,
  ENUMERATION_TYPE,
  INTERFACE,
  ADAPTER,
  FUNCTION,

  // GLOBAL VARIABLE => instance => global declaration => named table => symbol
  // table
  GLOBAL_DYNAMIC_VARIABLE,
  GLOBAL_STATIC_VARIABLE,

  // TEMPLATES
  CLASS_TEMPLATE,
  ENUMERATION_TEMPLATE,
  INTERFACE_TEMPLATE,
  ADAPTER_TEMPLATE,
  GLOBAL_DYNAMIC_VARIABLE_TEMPLATE,
  GLOBAL_STATIC_VARIABLE_TEMPLATE,
  FUNCTION_TEMPLATE,

  // POLYMORPHS
  FUNCTION_POLYMORPH,
  CLASS_POLYMORPH,
  ENUMERATION_POLYMORPH,
  INTERFACE_POLYMORPH,
  ADAPTER_POLYMORPH,
  GLOBAL_DYNAMIC_VARIABLE_POLYMORPH,
  GLOBAL_STATIC_VARIABLE_POLYMORPH,

  LAST
};

enum class ConstantKind : rq::EntityId {
  NONE,

  WORD,
  ARRAY,
  DATA_ARRAY,
  SYMBOL,

  LAST
};

enum class Opcode : rq::EntityId {
  NONE,

  // address0 = source location (either rq::Expression or SOURCE_RANGE)
  // address1 = entity
  DEBUG_STEP,

  // address0 = start expression
  // address1 = end expression
  SOURCE_RANGE,

  // address0 = head
  // address1 = tail
  STATEMENT,

  // address0 = lvalue
  // address1 = rvalue
  ASSIGN,

  // address0 = true_path
  IF,
  ELSE_IF,
  ELSE,

  // LOGICAL
  // address0 = head
  // address1 = tail
  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_AND_WITH_SHORTCIRCUIT,
  LOGICAL_OR_WITH_SHORTCIRCUIT,

  // address0 = rvalue
  // address1 = none
  LOGICAL_COMPLEMENT,

  // address0 = rvalue0
  // address1 = rvalue1
  RVALUE_PAIR,

  // address0 = none
  // address1 = none
  RETURN,

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
  // address1 = none
  NEGATE,

  LAST
};

constexpr rq::EntityId KEYWORD_OFFSET = 0;

constexpr rq::EntityId SYMBOL_OFFSET = rq::getUnderlying(rq::Keyword::LAST);

constexpr rq::EntityId CONSTANT_OFFSET =
    rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::LAST);

constexpr rq::EntityId OPCODE_OFFSET =
    rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::LAST);

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
    return this->_id < rq::getUnderlying(rq::Keyword::LAST);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return this->_id >= rq::SYMBOL_OFFSET && this->_id < rq::CONSTANT_OFFSET;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const {
    return this->_id >= rq::OPCODE_OFFSET;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE auto getDottedSubrange(rq::Opcode opcode) {
    return std::ranges::subrange<rq::DottedInstructionIterator,
                                 rq::DottedInstructionIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::DottedInstructionIterator(this, opcode),
        rq::DottedInstructionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto
  getConstDottedSubrange(rq::Opcode opcode) const {
    return std::ranges::subrange<rq::ConstDottedInstructionIterator,
                                 rq::ConstDottedInstructionIterator,
                                 std::ranges::subrange_kind::unsized>(
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
  return llvm::hash_value(rq::getUnderlying(value));
}
} // namespace llvm