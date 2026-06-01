#pragma once

#include <rq/utility.hpp>

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
  UNSITUATED_ASCRIBE_EXPRESSION,
  UNSITUATED_ASCRIBE_TYPE,

  // LOGICAL
  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_COMPLEMENT,

  // COMPARISON
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  EQUAL,
  NOT_EQUAL,

  // APPLY
  EXTEND,
  INITIALIZE_RECIEVER,
  BINDING,
  ASCRIBE_TYPE,
  ASCRIBE_EXPRESSION,
  ASCRIBE_ROOT_OF_VALUE,
  INSTANTIATE_EXPRESSION_ATTRIBUTE,
  INSTANTIATE_TYPE_ATTRIBUTE,
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
  AT,
  AT_OF,
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
  DESTROY,
  DESTROY_OF,
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
  FORWARD_RANGER,
  BACKWARD_RANGER,
  DESTRUCTOR,
  MAIN,
  FUNCTION,
  METHOD,
  EXTENSION_METHOD,
  IMPLEMENT_FUNCTION,
  IMPLEMENT_METHOD,
  IMPLEMENT_EXTENSION_METHOD,
  USE_FUNCTION,
  USE_METHOD,

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
  IMPLEMENT_INTERFACE,

  // VALUES
  ARRAY,
  NULL_,
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
  // reference to extended value of method or extension_method.
  THIS,
  // value returned from a function.
  RESULT,
  // retrieve command line arguments within main.
  COMMAND_LINE_ARGUMENTS,
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
  WITH,
  DEFAULT,
  FOR,
  WHILE,
  SPIN,
  WEAVE,
  SCOPE,
  BLOCK,

  // RANGES
  RANGE,
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
  TOP,
  LABEL,

  // HINTS
  DEBUG_BREAK,
  ABORT,
  ASSERT,
  UNREACHABLE,
  ASSUME,

  // EXPRESSION ATTRIBUTES
  // anchor_type
  NO_ANCHOR,
  ANCHOR,
  // opaque_type
  NO_OPAQUE,
  OPAQUE,
  // flank_type
  NO_FLANK,
  FLANK,
  // global_type
  NO_GLOBAL,
  GLOBAL,
  // access_type
  NO_ACCESS,
  EXPORT,
  PUBLIC,
  // partial_mutate_type
  NO_PARTIAL_MUTATE,
  PARTIAL_MUTATE,
  // static_type
  NO_STATIC,
  STATIC,
  // capture_type
  NO_CAPTURE,
  CAPTURE,
  // inline_type
  NO_INLINE,
  INLINE,
  // mangle_type
  NO_MANGLE,
  MANGLE,
  // pack_type
  NO_PACK,
  PACK,
  // branch_trend_type
  NO_BRANCH_TREND,
  LIKELY,
  UNLIKELY,
  // depreciate_type
  NO_DEPRECIATE,
  DEPRECIATE,
  EXPERIMENTAL,
  // address_stability_type
  NO_STABLE_ADDRESS,
  STABLE_ADDRESS,
  // variadic_type
  NO_VARIADIC,
  VARIADIC,
  // location_type
  NO_LOCATION,
  LOCATION,
  // template_type
  NO_TEMPLATE,
  TEMPLATE,
  // constraint_type
  NO_CONSTRAINT,
  CONSTRAINT,
  // weight_type
  NO_WEIGHT,
  WEIGHT,
  // require_type
  NO_REQUIRE,
  REQUIRE,
  // ensure_type
  NO_ENSURE,
  ENSURE,

  // TYPE ATTRIBUTES
  // var_type
  NO_VAR,
  VAR,
  PARTIAL_VAR,
  // volatile_type
  NO_VOLATILE,
  VOLATILE,
  // atomic_type
  NO_ATOMIC,
  ATOMIC,
  // null_terminate_type
  NO_NULL_TERMINATE,
  NULL_TERMINATE,

  // EXPRESSION ATTRIBUTE TYPES
  ANCHOR_TYPE,         // no_anchor vs anchor
  OPAQUE_TYPE,         // no_opaque vs opaque
  FLANK_TYPE,          // no_flank vs flank
  GLOBAL_TYPE,         // no_global vs global
  ACCESS_TYPE,         // no_access vs export vs public
  PARTIAL_MUTATE_TYPE, // no_partial_mutate vs partial_mutate
  STATIC_TYPE,         // no_static vs static
  CAPTURE_TYPE,        // no_capture vs capture
  INLINE_TYPE,         // no_inline vs inline
  MANGLE_TYPE,         // no_mangle vs mangle
  PACK_TYPE,           // no_pack vs pack
  BRANCH_TREND_TYPE,   // no_branch_trend vs likely vs unlikely
  DEPRECIATE_TYPE,     // no_depreciate vs depreciate vs experimental
  STABLE_ADDRESS_TYPE, // no_stable_address vs stable_address
  VARIADIC_TYPE,       // no_variadic vs variadic
  LOCATION_TYPE,       // no_location vs location
  TEMPLATE_TYPE,       // no_template vs template
  CONSTRAINT_TYPE,     // no_constraint vs constraint
  WEIGHT_TYPE,         // no_weight vs weight
  REQUIRE_TYPE,        // no_require vs require
  ENSURE_TYPE,         // no_ensure vs ensure

  // TYPE ATTRIBUTE TYPES
  VAR_TYPE,            // no_var vs var vs partial_var
  VOLATILE_TYPE,       // no_volatile vs volatile
  ATOMIC_TYPE,         // no_atomic vs atomic
  NULL_TERMINATE_TYPE, // no_null_terminate vs null_terminate

  // REFLECTIONS
  REFLECT,
  MEMBER_OF,
  MEMBER_OF_TOP,
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
  REVERSE,
  REVERSE_OF,
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
  IS_EXPRESSION_ATTRIBUTE_TYPE,
  IS_EXPRESSION_ATTRIBUTE_TYPE_OF,
  IS_TYPE_ATTRIBUTE_TYPE,
  IS_TYPE_ATTRIBUTE_TYPE_OF,

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
  OUT,
  THIS,
  RESULT,
  VALUE,
  INDEX,
  DISCRIMINANT,
  COMMAND_LINE_ARGUMENTS,
  CALLSITE,

  // CONTEXTUAL TYPE
  INFERENCE_TYPE,
  VOID_TYPE,
  NO_RETURN_TYPE,

  // EXPRESSION ATTRIBUTES
  ANCHOR_TYPE,
  OPAQUE_TYPE,
  GLOBAL_TYPE,
  ACCESS_TYPE,
  PARTIAL_MUTATE_TYPE,
  STATIC_TYPE,
  CAPTURE_TYPE,
  INLINE_TYPE,
  MANGLE_TYPE,
  PACK_TYPE,
  BRANCH_TREND_TYPE,
  DEPRECIATE_TYPE,
  STABLE_ADDRESS_TYPE,
  VARIADIC_TYPE,
  LOCATION_TYPE,
  TEMPLATE_TYPE,
  CONSTRAINT_TYPE,
  WEIGHT_TYPE,
  REQUIRE_TYPE,
  ENSURE_TYPE,

  // TYPE ATTRIBUTES
  VAR_TYPE,
  VOLATILE_TYPE,
  ATOMIC_TYPE,
  NULL_TERMINATE_TYPE,

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

  // JUXTAPOSITIONAL LIST
  JUXTAPOSITIONAL_LIST_TYPE,

  // ARITHMETIC SEQUENCES
  ARITHMETIC_INTERVAL,
  INFINITE_ARITHMETIC_SEQUENCE,
  FINITE_ARITHMETIC_SEQUENCE,

  // LOCAL DECLARATIONS
  LABEL,
  ANCHOR,

  // LOCAL VARIABLES => local declaration
  LOCAL_DYNAMIC_VARIABLE,
  LOCAL_STATIC_VARIABLE,
  ENUMERATOR,

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
  COMPOSITION_TYPE,

  // SYNONYMS
  SYNONYM_TYPE,

  // SYMBOL TABLES
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

  // GLOBAL DECLARATION => named table
  CLASS_TYPE,
  ENUMERATION_TYPE,
  INTERFACE,

  // GLOBAL VARIABLE => global declaration => named table => symbol table
  GLOBAL_DYNAMIC_VARIABLE,
  GLOBAL_STATIC_VARIABLE,

  // CALLABLE => global declaration => named table => symbol table
  DESTRUCTOR,
  MAIN,

  // RANGERS => callable => global declaration => named table => symbol table
  FORWARD_RANGER,
  BACKWARD_RANGER,

  // PROCEDURES => callatble => global declaration => symbol table
  FUNCTION,
  METHOD,
  EXTENSION_METHOD,

  // TEMPLATES
  CLASS_TEMPLATE,
  ENUMERATION_TEMPLATE,
  INTERFACE_TEMPLATE,
  GLOBAL_DYNAMIC_VARIABLE_TEMPLATE,
  GLOBAL_STATIC_VARIABLE_TEMPLATE,
  FORWARD_RANGER_TEMPLATE,
  BACKWARD_RANGER_TEMPLATE,
  FUNCTION_TEMPLATE,
  METHOD_TEMPLATE,
  EXTENSION_METHOD_TEMPLATE,

  // POLYMORPHS
  FORWARD_RANGER_POLYMORPH,
  BACKWARD_RANGER_POLYMORPH,
  FUNCTION_POLYMORPH,
  METHOD_POLYMORPH,
  EXTENSION_METHOD_POLYMORPH,
  CLASS_POLYMORPH,
  ENUMERATION_POLYMORPH,
  INTERFACE_POLYMORPH,
  GLOBAL_DYNAMIC_VARIABLE_POLYMORPH,
  GLOBAL_STATIC_VARIABLE_POLYMORPH,

  LAST
};

enum class ConstantKind : rq::EntityId {
    NONE,

    SYMBOL,
    WORD,
    ARRAY,

    LAST
};

enum class Opcode : rq::EntityId {
    NONE,

    // TODO

    LAST
};

constexpr rq::EntityId KEYWORD_OFFSET = 0;

constexpr rq::EntityId SYMBOL_OFFSET = rq::getUnderlying(rq::Keyword::LAST);

constexpr rq::EntityId CONSTANT_OFFSET =  rq::SYMBOL_OFFSET + rq::getUnderlying(rq::SymbolKind::LAST);

constexpr rq::EntityId OPCODE_OFFSET = rq::CONSTANT_OFFSET + rq::getUnderlying(rq::ConstantKind::LAST);

struct Entity {
    using Self = rq::Entity;

    rq::EntityId _id;

    explicit RQ_ALWAYS_INLINE Entity(rq::EntityId id) : _id(id) {}
    Entity(const Self&) = delete;
    Entity(Self&&) = delete;
    ~Entity() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;

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

      [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr) {
        RQ_ASSERT(entity_ptr != nullptr, "nullptr entity");
        return true;
      }

};

}