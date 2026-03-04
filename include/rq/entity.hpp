#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/StringSaver.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

struct Context;

// NOTE: this is a massive tree of types that make use of LLVM RTTI
// The root of this tree is "Entity", and the tree contains all symbols,
// constant values, and symbolic instructions.
// https://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html

// NOTE: everything in this tree is meant to be allocated within a
// llvm::BumpPtrAllocator. Nothing has a destructor. Make use of rq::BumpPtrList
// and rq::BumpPtrMap

// TODO: need to make rq::BumpPtrMap red black tree type for symbol tables that
// use llvm::BumpPtrAllocator allocated nodes

enum EntityKind : std::uint16_t {
  ENTITY_NONE = 0,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same (including type attriubutes, etc) by comparing their
  // pointers.

  // SIMPLE BUILTIN
  SY_GENERIC_SYMBOL,
  SY_GENERIC_TYPE,
  SY_INFERENCE,
  SY_EXPRESSION,
  SY_VOID,
  SY_NULL,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_GENERIC_SIGNED,
  SY_GENERIC_UNSIGNED,
  SY_GENERIC_FLOAT,
  SY_GENERIC_BINARY,
  SY_GENERIC_BFLOAT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_GENERIC_INTEGER,
  SY_GENERIC_SIGNED_INTEGER,
  SY_GENERIC_UNSIGNED_INTEGER,
  SY_GENERIC_CODEUNIT,
  SY_GENERIC_STRING,
  SY_ASCII,
  SY_UTF8,

  // SCALED BUILTIN
  SY_SIGNED_INTEGER,
  SY_UNSIGNED_INTEGER,

  // UNARY SUBTYPE
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCED_COUNT_ARRAY,

  // COUNTED SUBTYPE
  SY_ARRAY,

  // COMPOSITE SUBTYPE
  SY_LAYOUT,
  SY_SIGNATURE,
  SY_EXTENSION,

  // ARITHMETIC SEQUENCE
  SY_ARITHMETIC_INTERVAL,
  SY_FINITE_ARITHMETIC_PROGRESSION,
  SY_INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  SY_SYNONYM,
  SY_MODULE,
  SY_IMPORT,
  SY_CODE,
  SY_CATEGORY_DISCRIMINANT,
  SY_LABEL,

  // BINDING
  SY_DYNAMIC_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE,
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,

  // SYMBOL TABLE
  SY_TOP,
  SY_SCOPE,
  SY_TABLE,
  SY_CLASS,
  SY_ENUMERATION,
  SY_CATEGORY,

  // PROCEDURE
  SY_ENTRY,
  SY_FUNCTION,
  SY_METHOD,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,
  SY_RANGER,

  // TEMPLATE
  SY_TEMPLATE_CLASS,
  SY_TEMPLATE_ENUMERATION,
  SY_TEMPLATE_CATEGORY,
  SY_TEMPLATE_DYNAMIC_VARIABLE,
  SY_TEMPLATE_STATIC_VARIABLE,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,

  // PARTIAL SPECIALIZATION
  SY_PARTIAL_CLASS,
  SY_PARTIAL_ENUMERATION,
  SY_PARTIAL_CATEGORY,
  SY_PARTIAL_DYNAMIC_VARIABLE,
  SY_PARTIAL_STATIC_VARIABLE,
  SY_PARTIAL_FUNCTION,
  SY_PARTIAL_METHOD,
  SY_PARTIAL_EXTENSION_FUNCTION,
  SY_PARTIAL_EXTENSION_METHOD,

  // =====CONSTANTS=====

  CT_TYPE,
  CT_EXPRESSION,
  CT_BOOLEAN,
  CT_INTEGER,
  CT_FLOAT,
  CT_STRING,
  CT_ARRAY,

  // =====OPCODES=====

  OP_NONE,

  OP_LOGICAL_AND,
  OP_LOGICAL_OR,
  OP_LOGICAL_COMPLEMENT,

  OP_GREATER,
  OP_GREATER_EQUAL,
  OP_LESS,
  OP_LESS_EQUAL,
  OP_EQUAL,
  OP_NOT_EQUAL,

  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_MODULUS,
  OP_NEGATE,

  OP_CAST,

  OP_BITWISE_AND,
  OP_BITWISE_OR,
  OP_BITWISE_XOR,
  OP_BITWISE_COMPLEMENT,
  OP_BITWISE_SHIFT_LEFT,
  OP_BITWISE_SHIFT_RIGHT,
  OP_BITWISE_ROTATE_LEFT,
  OP_BITWISE_ROTATE_RIGHT,

  OP_CONTENT_OF,
  OP_ADDRESS_OF,
  OP_CALL,
  OP_DROP,
  OP_MOVE,
  OP_COPY,

  OP_RETURN,
  OP_GOTO,
  OP_CONDITION,

  OP_LOOP_SEQUENCE,
  OP_LOOP_ELEMENTS,
  OP_LOOP_RANGER,

  OP_DEBUG_TRAP,
  OP_UNREACHABLE,
  OP_ASSUME,

  ENTITY_LAST
};

static constexpr std::size_t ENTITY_COUNT =
    static_cast<std::size_t>(rq::EntityKind::ENTITY_LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::EntityKind kind);

enum class EntityFlags : std::uint32_t {
  NONE = 0,

  // CATEGORIES
  // the first two bits used to determine category
  // the rest of the bits mean different things depending on the
  // category.
  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  NOT_OPCODE_MASK = (rq::getBit(0) | rq::getBit(1)),

  // SYMBOL FLAGS
  // these flags are valid only when working with symbols
  // SYMBOL INHERITING PROPERTIES - are implmented via inherited types that can
  // be used themselves. every symbol can have at most one of these.
  SY_SIMPLE_BUILTIN = rq::getBit(2),
  SY_SCALED_BUILTIN = rq::getBit(3),
  SY_UNARY_SUBTYPE = rq::getBit(4),
  SY_COUNTED_SUBTYPE = rq::getBit(5),
  SY_COMPOSITE_SUBTYPE = rq::getBit(6),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(7),
  SY_SYMBOL_TABLE = rq::getBit(8),
  SY_PROCEDURE = rq::getBit(9),
  SY_TEMPLATE = rq::getBit(10),
  SY_PARTIAL = rq::getBit(11),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(12),
  SY_TYPE = rq::getBit(13),
  SY_GENERIC = rq::getBit(14),
  SY_CONCRETE = rq::getBit(15),
  SY_SUBTYPE = rq::getBit(16),
  SY_PLATFORM_CHANGING = rq::getBit(17),
  SY_INTEGER = rq::getBit(18),
  SY_FLOAT = rq::getBit(19),
  SY_CODEUNIT = rq::getBit(20),
  SY_SIGNED = rq::getBit(21),
  SY_UNSIGNED = rq::getBit(22),
  SY_TOP_OF_FRAME = rq::getBit(23)

  // CONSTANT FLAGS
  // TODO

  // INSTRUCTION FLAGS
  // TODO
};
template <> struct is_flags<EntityFlags> : std::true_type {};
[[nodiscard]] inline rq::EntityFlags getFlags(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOpcode(rq::EntityKind kind);

#define RQ_ASSERT_SYMBOL(kind) RQ_ASSERT(rq::getIsSymbol((kind)), "not symbol")
#define RQ_ASSERT_CONSTANT(kind)                                               \
  RQ_ASSERT(rq::getIsConstant((kind)), "not constant")

#define RQ_ASSERT_OPCODE(opcode)                                               \
  RQ_ASSERT(rq::getIsOpcode(opcode), "not opcode")

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequence(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGeneric(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcrete(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getTemplate(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getPartial(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getFullSpecialization(rq::EntityKind kind);

struct Entity;
struct Symbol;
struct Type;
struct SimpleBuiltin;
struct GenericSymbol;
struct GenericType;
struct Inference;
struct Void;
struct Null;
struct NoReturn;
struct VariadicArguments;
struct Boolean;
struct GenericSigned;
struct GenericUnsigned;
struct GenericFloat;
struct GenericBinary;
struct GenericBFloat;
struct Half;
struct Single;
struct Double;
struct Quadruple;
struct Binary16;
struct Binary32;
struct Binary64;
struct Binary128;
struct BFloat16;
struct GenericInteger;
struct GenericSignedInteger;
struct GenericUnsignedInteger;
struct GenericCodeunit;
struct GenericString;
struct Ascii;
struct Utf8;
struct ScaledBuiltin;
struct SignedInteger;
struct UnsignedInteger;
struct UnarySubtype;
struct Reference;
struct Pointer;
struct FatPointer;
struct InferencedCountArray;
struct CountedSubtype;
struct Array;
struct CompositeSubtype;
struct Layout;
struct Signature;
struct Extension;
struct ArithmeticSequence;
struct ArithmeticInterval;
struct FiniteArithmeticProgression;
struct InfiniteArithmeticProgression;
struct Synonym;
struct Module;
struct Import;
struct Code;
struct CategoryDiscriminant;
struct Label;
struct DynamicVariable;
struct StaticVariable;
struct Enumerator;
struct CategoryAlternative;
struct ClassParameter;
struct LayoutParameter;
struct TemplateParameter;
struct SignatureParameter;
struct SymbolTable;
struct Top;
struct Scope;
struct Table;
struct Class;
struct Enumeration;
struct Category;
struct Entry;
struct Function;
struct Method;
struct ExtensionFunction;
struct ExtensionMethod;
struct Ranger;
struct Template;
struct TemplateClass;
struct TemplateEnumeration;
struct TemplateCategory;
struct TemplateDynamicVariable;
struct TemplateStaticVariable;
struct TemplateFunction;
struct TemplateMethod;
struct TemplateExtensionFunction;
struct TemplateExtensionMethod;
struct Partial;
struct PartialClass;
struct PartialEnumeration;
struct PartialCategory;
struct PartialDynamicVariable;
struct PartialStaticVariable;
struct PartialFunction;
struct PartialMethod;
struct PartialExtensionFunction;
struct PartialExtensionMethod;
struct Constant;
struct TypeConstant;
struct ExpressionConstant;
struct BooleanConstant;
struct IntegerConstant;
struct FloatConstant;
struct StringConstant;
struct ArrayConstant;
struct Instruction;

struct Entity {
  using Self = rq::Entity;

  rq::EntityKind kind;

  explicit Entity(rq::EntityKind k) : kind(k) {}
  Entity() = delete;
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  Entity &operator=(const Entity &) = delete;
  Entity &operator=(Entity &&) = delete;
  ~Entity() = default;

  bool operator==(const Entity *other) const { return this == other; }
  bool operator!=(const Entity *other) const { return this != other; }

  static bool classof(const Entity *entity) {
    std::ignore = entity;
    return true;
  }
};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit Symbol(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSymbol(entity->kind);
  }
};

struct Type : public rq::Entity {
  using Self = rq::Type;

  explicit Type(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsType(entity->kind);
  }
};

struct SimpleBuiltin : public rq::Entity {
  using Self = rq::SimpleBuiltin;

  explicit SimpleBuiltin(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSimpleBuiltin(entity->kind);
  }
};

struct GenericSymbol : public rq::Entity {
  using Self = rq::GenericSymbol;

  GenericSymbol() : Entity(rq::EntityKind::SY_GENERIC_SYMBOL) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_SYMBOL;
  }
};

struct GenericType : public rq::Entity {
  using Self = rq::GenericType;

  GenericType() : Entity(rq::EntityKind::SY_GENERIC_TYPE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_TYPE;
  }
};

struct Inference : public rq::Entity {
  using Self = rq::Inference;

  Inference() : Entity(rq::EntityKind::SY_INFERENCE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_INFERENCE;
  }
};

struct Void : public rq::Entity {
  using Self = rq::Void;

  Void() : Entity(rq::EntityKind::SY_VOID) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_VOID;
  }
};

struct Null : public rq::Entity {
  using Self = rq::Null;

  Null() : Entity(rq::EntityKind::SY_NULL) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_NULL;
  }
};

struct NoReturn : public rq::Entity {
  using Self = rq::NoReturn;

  NoReturn() : Entity(rq::EntityKind::SY_NO_RETURN) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_NO_RETURN;
  }
};

struct VariadicArguments : public rq::Entity {
  using Self = rq::VariadicArguments;

  VariadicArguments() : Entity(rq::EntityKind::SY_VARIADIC_ARGUMENTS) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_VARIADIC_ARGUMENTS;
  }
};

struct Boolean : public rq::Entity {
  using Self = rq::Boolean;

  Boolean() : Entity(rq::EntityKind::SY_BOOLEAN) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BOOLEAN;
  }
};

struct GenericSigned : public rq::Entity {
  using Self = rq::GenericSigned;

  GenericSigned() : Entity(rq::EntityKind::SY_GENERIC_SIGNED) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_SIGNED;
  }
};

struct GenericUnsigned : public rq::Entity {
  using Self = rq::GenericUnsigned;

  GenericUnsigned() : Entity(rq::EntityKind::SY_GENERIC_UNSIGNED) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_UNSIGNED;
  }
};

struct GenericFloat : public rq::Entity {
  using Self = rq::GenericFloat;

  GenericFloat() : Entity(rq::EntityKind::SY_GENERIC_FLOAT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_FLOAT;
  }
};

struct GenericBinary : public rq::Entity {
  using Self = rq::GenericBinary;

  GenericBinary() : Entity(rq::EntityKind::SY_GENERIC_BINARY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_BINARY;
  }
};

struct GenericBFloat : public rq::Entity {
  using Self = rq::GenericBFloat;

  GenericBFloat() : Entity(rq::EntityKind::SY_GENERIC_BFLOAT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_BFLOAT;
  }
};

struct Half : public rq::Entity {
  using Self = rq::Half;

  Half() : Entity(rq::EntityKind::SY_HALF) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_HALF;
  }
};

struct Single : public rq::Entity {
  using Self = rq::Single;

  Single() : Entity(rq::EntityKind::SY_SINGLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SINGLE;
  }
};

struct Double : public rq::Entity {
  using Self = rq::Double;

  Double() : Entity(rq::EntityKind::SY_DOUBLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_DOUBLE;
  }
};

struct Quadruple : public rq::Entity {
  using Self = rq::Quadruple;

  Quadruple() : Entity(rq::EntityKind::SY_QUADRUPLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_QUADRUPLE;
  }
};

struct Binary16 : public rq::Entity {
  using Self = rq::Binary16;

  Binary16() : Entity(rq::EntityKind::SY_BINARY16) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BINARY16;
  }
};

struct Binary32 : public rq::Entity {
  using Self = rq::Binary32;

  Binary32() : Entity(rq::EntityKind::SY_BINARY32) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BINARY32;
  }
};

struct Binary64 : public rq::Entity {
  using Self = rq::Binary64;

  Binary64() : Entity(rq::EntityKind::SY_BINARY64) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BINARY64;
  }
};

struct Binary128 : public rq::Entity {
  using Self = rq::Binary128;

  Binary128() : Entity(rq::EntityKind::SY_BINARY128) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BINARY128;
  }
};

struct BFloat16 : public rq::Entity {
  using Self = rq::BFloat16;

  BFloat16() : Entity(rq::EntityKind::SY_BFLOAT16) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_BFLOAT16;
  }
};

struct GenericInteger : public rq::Entity {
  using Self = rq::GenericInteger;

  GenericInteger() : Entity(rq::EntityKind::SY_GENERIC_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_INTEGER;
  }
};

struct GenericSignedInteger : public rq::Entity {
  using Self = rq::GenericSignedInteger;

  GenericSignedInteger() : Entity(rq::EntityKind::SY_GENERIC_SIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_SIGNED_INTEGER;
  }
};

struct GenericUnsignedInteger : public rq::Entity {
  using Self = rq::GenericUnsignedInteger;

  GenericUnsignedInteger()
      : Entity(rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER;
  }
};

struct GenericCodeunit : public rq::Entity {
  using Self = rq::GenericCodeunit;

  GenericCodeunit() : Entity(rq::EntityKind::SY_GENERIC_CODEUNIT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_CODEUNIT;
  }
};

struct GenericString : public rq::Entity {
  using Self = rq::GenericString;

  GenericString() : Entity(rq::EntityKind::SY_GENERIC_STRING) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_GENERIC_STRING;
  }
};

struct Ascii : public rq::Entity {
  using Self = rq::Ascii;

  Ascii() : Entity(rq::EntityKind::SY_ASCII) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ASCII;
  }
};

struct Utf8 : public rq::Entity {
  using Self = rq::Utf8;

  Utf8() : Entity(rq::EntityKind::SY_UTF8) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_UTF8;
  }
};

struct ScaledBuiltin : public rq::Entity {
  using Self = rq::ScaledBuiltin;

  explicit ScaledBuiltin(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsScaledBuiltin(entity->kind);
  }
};

struct SignedInteger : public rq::Entity {
  using Self = rq::SignedInteger;

  SignedInteger() : Entity(rq::EntityKind::SY_SIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SIGNED_INTEGER;
  }
};

struct UnsignedInteger : public rq::Entity {
  using Self = rq::UnsignedInteger;

  UnsignedInteger() : Entity(rq::EntityKind::SY_UNSIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_UNSIGNED_INTEGER;
  }
};

struct UnarySubtype : public rq::Entity {
  using Self = rq::UnarySubtype;

  explicit UnarySubtype(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsUnarySubtype(entity->kind);
  }
};

struct Reference : public rq::Entity {
  using Self = rq::Reference;

  Reference() : Entity(rq::EntityKind::SY_REFERENCE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_REFERENCE;
  }
};

struct Pointer : public rq::Entity {
  using Self = rq::Pointer;

  Pointer() : Entity(rq::EntityKind::SY_POINTER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_POINTER;
  }
};

struct FatPointer : public rq::Entity {
  using Self = rq::FatPointer;

  FatPointer() : Entity(rq::EntityKind::SY_FAT_POINTER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_FAT_POINTER;
  }
};

struct InferencedCountArray : public rq::Entity {
  using Self = rq::InferencedCountArray;

  InferencedCountArray() : Entity(rq::EntityKind::SY_INFERENCED_COUNT_ARRAY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
  }
};

struct CountedSubtype : public rq::Entity {
  using Self = rq::CountedSubtype;

  explicit CountedSubtype(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsCountedSubtype(entity->kind);
  }
};

struct Array : public rq::Entity {
  using Self = rq::Array;

  Array() : Entity(rq::EntityKind::SY_ARRAY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ARRAY;
  }
};

struct CompositeSubtype : public rq::Entity {
  using Self = rq::CompositeSubtype;

  explicit CompositeSubtype(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsCompositeSubtype(entity->kind);
  }
};

struct Layout : public rq::Entity {
  using Self = rq::Layout;

  Layout() : Entity(rq::EntityKind::SY_LAYOUT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_LAYOUT;
  }
};

struct Signature : public rq::Entity {
  using Self = rq::Signature;

  Signature() : Entity(rq::EntityKind::SY_SIGNATURE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SIGNATURE;
  }
};

struct Extension : public rq::Entity {
  using Self = rq::Extension;

  Extension() : Entity(rq::EntityKind::SY_EXTENSION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_EXTENSION;
  }
};

struct ArithmeticSequence : public rq::Entity {
  using Self = rq::ArithmeticSequence;

  explicit ArithmeticSequence(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsArithmeticSequence(entity->kind);
  }
};

struct ArithmeticInterval : public rq::Entity {
  using Self = rq::ArithmeticInterval;

  ArithmeticInterval() : Entity(rq::EntityKind::SY_ARITHMETIC_INTERVAL) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ARITHMETIC_INTERVAL;
  }
};

struct FiniteArithmeticProgression : public rq::Entity {
  using Self = rq::FiniteArithmeticProgression;

  FiniteArithmeticProgression()
      : Entity(rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
};

struct InfiniteArithmeticProgression : public rq::Entity {
  using Self = rq::InfiniteArithmeticProgression;

  InfiniteArithmeticProgression()
      : Entity(rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
};

struct Synonym : public rq::Entity {
  using Self = rq::Synonym;

  Synonym() : Entity(rq::EntityKind::SY_SYNONYM) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SYNONYM;
  }
};

struct Module : public rq::Entity {
  using Self = rq::Module;

  Module() : Entity(rq::EntityKind::SY_MODULE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_MODULE;
  }
};

struct Import : public rq::Entity {
  using Self = rq::Import;

  Import() : Entity(rq::EntityKind::SY_IMPORT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_IMPORT;
  }
};

struct Code : public rq::Entity {
  using Self = rq::Code;

  Code() : Entity(rq::EntityKind::SY_CODE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CODE;
  }
};

struct CategoryDiscriminant : public rq::Entity {
  using Self = rq::CategoryDiscriminant;

  CategoryDiscriminant() : Entity(rq::EntityKind::SY_CATEGORY_DISCRIMINANT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CATEGORY_DISCRIMINANT;
  }
};

struct Label : public rq::Entity {
  using Self = rq::Label;

  Label() : Entity(rq::EntityKind::SY_LABEL) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_LABEL;
  }
};

struct DynamicVariable : public rq::Entity {
  using Self = rq::DynamicVariable;

  DynamicVariable() : Entity(rq::EntityKind::SY_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_DYNAMIC_VARIABLE;
  }
};

struct StaticVariable : public rq::Entity {
  using Self = rq::StaticVariable;

  StaticVariable() : Entity(rq::EntityKind::SY_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_STATIC_VARIABLE;
  }
};

struct Enumerator : public rq::Entity {
  using Self = rq::Enumerator;

  Enumerator() : Entity(rq::EntityKind::SY_ENUMERATOR) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ENUMERATOR;
  }
};

struct CategoryAlternative : public rq::Entity {
  using Self = rq::CategoryAlternative;

  CategoryAlternative() : Entity(rq::EntityKind::SY_CATEGORY_ALTERNATIVE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CATEGORY_ALTERNATIVE;
  }
};

struct ClassParameter : public rq::Entity {
  using Self = rq::ClassParameter;

  ClassParameter() : Entity(rq::EntityKind::SY_CLASS_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CLASS_PARAMETER;
  }
};

struct LayoutParameter : public rq::Entity {
  using Self = rq::LayoutParameter;

  LayoutParameter() : Entity(rq::EntityKind::SY_LAYOUT_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_LAYOUT_PARAMETER;
  }
};

struct TemplateParameter : public rq::Entity {
  using Self = rq::TemplateParameter;

  TemplateParameter() : Entity(rq::EntityKind::SY_TEMPLATE_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_PARAMETER;
  }
};

struct SignatureParameter : public rq::Entity {
  using Self = rq::SignatureParameter;

  SignatureParameter() : Entity(rq::EntityKind::SY_SIGNATURE_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SIGNATURE_PARAMETER;
  }
};

struct SymbolTable : public rq::Entity {
  using Self = rq::SymbolTable;

  explicit SymbolTable(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSymbolTable(entity->kind);
  }
};

struct Top : public rq::Entity {
  using Self = rq::Top;

  Top() : Entity(rq::EntityKind::SY_TOP) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TOP;
  }
};

struct Scope : public rq::Entity {
  using Self = rq::Scope;

  Scope() : Entity(rq::EntityKind::SY_SCOPE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_SCOPE;
  }
};

struct Table : public rq::Entity {
  using Self = rq::Table;

  Table() : Entity(rq::EntityKind::SY_TABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TABLE;
  }
};

struct Class : public rq::Entity {
  using Self = rq::Class;

  Class() : Entity(rq::EntityKind::SY_CLASS) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CLASS;
  }
};

struct Enumeration : public rq::Entity {
  using Self = rq::Enumeration;

  Enumeration() : Entity(rq::EntityKind::SY_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ENUMERATION;
  }
};

struct Category : public rq::Entity {
  using Self = rq::Category;

  Category() : Entity(rq::EntityKind::SY_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_CATEGORY;
  }
};

struct Entry : public rq::Entity {
  using Self = rq::Entry;

  Entry() : Entity(rq::EntityKind::SY_ENTRY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_ENTRY;
  }
};

struct Function : public rq::Entity {
  using Self = rq::Function;

  Function() : Entity(rq::EntityKind::SY_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_FUNCTION;
  }
};

struct Method : public rq::Entity {
  using Self = rq::Method;

  Method() : Entity(rq::EntityKind::SY_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_METHOD;
  }
};

struct ExtensionFunction : public rq::Entity {
  using Self = rq::ExtensionFunction;

  ExtensionFunction() : Entity(rq::EntityKind::SY_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_EXTENSION_FUNCTION;
  }
};

struct ExtensionMethod : public rq::Entity {
  using Self = rq::ExtensionMethod;

  ExtensionMethod() : Entity(rq::EntityKind::SY_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_EXTENSION_METHOD;
  }
};

struct Ranger : public rq::Entity {
  using Self = rq::Ranger;

  Ranger() : Entity(rq::EntityKind::SY_RANGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_RANGER;
  }
};

struct Template : public rq::Entity {
  using Self = rq::Template;

  explicit Template(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsTemplate(entity->kind);
  }
};

struct TemplateClass : public rq::Entity {
  using Self = rq::TemplateClass;

  TemplateClass() : Entity(rq::EntityKind::SY_TEMPLATE_CLASS) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_CLASS;
  }
};

struct TemplateEnumeration : public rq::Entity {
  using Self = rq::TemplateEnumeration;

  TemplateEnumeration() : Entity(rq::EntityKind::SY_TEMPLATE_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_ENUMERATION;
  }
};

struct TemplateCategory : public rq::Entity {
  using Self = rq::TemplateCategory;

  TemplateCategory() : Entity(rq::EntityKind::SY_TEMPLATE_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_CATEGORY;
  }
};

struct TemplateDynamicVariable : public rq::Entity {
  using Self = rq::TemplateDynamicVariable;

  TemplateDynamicVariable()
      : Entity(rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
  }
};

struct TemplateStaticVariable : public rq::Entity {
  using Self = rq::TemplateStaticVariable;

  TemplateStaticVariable()
      : Entity(rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
  }
};

struct TemplateFunction : public rq::Entity {
  using Self = rq::TemplateFunction;

  TemplateFunction() : Entity(rq::EntityKind::SY_TEMPLATE_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_FUNCTION;
  }
};

struct TemplateMethod : public rq::Entity {
  using Self = rq::TemplateMethod;

  TemplateMethod() : Entity(rq::EntityKind::SY_TEMPLATE_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_METHOD;
  }
};

struct TemplateExtensionFunction : public rq::Entity {
  using Self = rq::TemplateExtensionFunction;

  TemplateExtensionFunction()
      : Entity(rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
};

struct TemplateExtensionMethod : public rq::Entity {
  using Self = rq::TemplateExtensionMethod;

  TemplateExtensionMethod()
      : Entity(rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
  }
};

struct Partial : public rq::Entity {
  using Self = rq::Partial;

  explicit Partial(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsPartial(entity->kind);
  }
};

struct PartialClass : public rq::Entity {
  using Self = rq::PartialClass;

  PartialClass() : Entity(rq::EntityKind::SY_PARTIAL_CLASS) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_CLASS;
  }
};

struct PartialEnumeration : public rq::Entity {
  using Self = rq::PartialEnumeration;

  PartialEnumeration() : Entity(rq::EntityKind::SY_PARTIAL_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_ENUMERATION;
  }
};

struct PartialCategory : public rq::Entity {
  using Self = rq::PartialCategory;

  PartialCategory() : Entity(rq::EntityKind::SY_PARTIAL_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_CATEGORY;
  }
};

struct PartialDynamicVariable : public rq::Entity {
  using Self = rq::PartialDynamicVariable;

  PartialDynamicVariable()
      : Entity(rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
  }
};

struct PartialStaticVariable : public rq::Entity {
  using Self = rq::PartialStaticVariable;

  PartialStaticVariable()
      : Entity(rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
  }
};

struct PartialFunction : public rq::Entity {
  using Self = rq::PartialFunction;

  PartialFunction() : Entity(rq::EntityKind::SY_PARTIAL_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_FUNCTION;
  }
};

struct PartialMethod : public rq::Entity {
  using Self = rq::PartialMethod;

  PartialMethod() : Entity(rq::EntityKind::SY_PARTIAL_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct PartialExtensionFunction : public rq::Entity {
  using Self = rq::PartialExtensionFunction;

  PartialExtensionFunction()
      : Entity(rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
  }
};

struct PartialExtensionMethod : public rq::Entity {
  using Self = rq::PartialExtensionMethod;

  PartialExtensionMethod()
      : Entity(rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
  }
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit Constant(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsConstant(entity->kind);
  }
};

struct TypeConstant : public rq::Entity {
  using Self = rq::TypeConstant;

  TypeConstant() : Entity(rq::EntityKind::CT_TYPE) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_TYPE;
  }
};

struct ExpressionConstant : public rq::Entity {
  using Self = rq::ExpressionConstant;

  ExpressionConstant() : Entity(rq::EntityKind::CT_EXPRESSION) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_EXPRESSION;
  }
};

struct BooleanConstant : public rq::Entity {
  using Self = rq::BooleanConstant;

  BooleanConstant() : Entity(rq::EntityKind::CT_BOOLEAN) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_BOOLEAN;
  }
};

struct IntegerConstant : public rq::Entity {
  using Self = rq::IntegerConstant;

  IntegerConstant() : Entity(rq::EntityKind::CT_INTEGER) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_INTEGER;
  }
};

struct FloatConstant : public rq::Entity {
  using Self = rq::FloatConstant;

  FloatConstant() : Entity(rq::EntityKind::CT_FLOAT) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_FLOAT;
  }
};

struct StringConstant : public rq::Entity {
  using Self = rq::StringConstant;

  StringConstant() : Entity(rq::EntityKind::CT_STRING) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_STRING;
  }
};

struct ArrayConstant : public rq::Entity {
  using Self = rq::ArrayConstant;

  ArrayConstant() : Entity(rq::EntityKind::CT_ARRAY) {}

  static bool classof(const Entity *entity) {
    return entity->kind == rq::EntityKind::CT_ARRAY;
  }
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  explicit Instruction(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsOpcode(entity->kind);
  }
};

} // namespace rq