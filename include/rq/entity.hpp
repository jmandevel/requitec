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

enum class EntityKind : std::uint16_t {
  NONE = 0,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same (including type attriubutes, etc) by comparing their
  // pointers.

  // TYPE DEFINITION
  SY_TYPE_DEFINITION,

  // SIMPLE BUILTIN
  SY_INFERENCE,
  SY_EXPRESSION,
  SY_ENTITY_KIND,
  SY_TYPE_TYPE,
  SY_GENERIC_SYMBOL,
  SY_VOID,
  SY_NULL,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_GENERIC_FLOAT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_GENERIC_BINARY,
  SY_GENERIC_BFLOAT,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_GENERIC_INTEGER,
  SY_GENERIC_SIGNED,
  SY_GENERIC_UNSIGNED,
  SY_GENERIC_CODEUNIT,
  SY_GENERIC_STRING,
  SY_ASCII,
  SY_UTF8,

  // SCALED BUILTIN
  SY_SCALED_SIGNED,
  SY_SCALED_UNSIGNED,

  // UNARY SUBTYPE
  SY_RANGE,
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCED_COUNT_ARRAY,

  // COUNTED SUBTYPE
  SY_ARRAY,

  // COMPOSITE SUBTYPE
  SY_LAYOUT,
  SY_SIGNATURE,

  // ARITHMETIC SEQUENCE
  SY_ARITHMETIC_INTERVAL,
  SY_FINITE_ARITHMETIC_PROGRESSION,
  SY_INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  SY_MODULE,
  SY_IMPORT,
  SY_EXTENSION,
  SY_CODE,
  SY_CATEGORY_DISCRIMINANT,

  // BINDING
  SY_DYNAMIC_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE,
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,
  SY_LABEL,

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
  SY_TEMPLATE_DYNAMIC_VARIABLE,
  SY_TEMPLATE_STATIC_VARIABLE,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,

  // PARTIAL SPECIALIZATION
  SY_PARTIAL_CLASS,
  SY_PARTIAL_ENUMERATION,
  SY_PARTIAL_DYNAMIC_VARIABLE,
  SY_PARTIAL_STATIC_VARIABLE,
  SY_PARTIAL_FUNCTION,
  SY_PARTIAL_METHOD,
  SY_PARTIAL_EXTENSION_FUNCTION,
  SY_PARTIAL_EXTENSION_METHOD,

  // =====CONSTANTS=====

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

  LAST
};

static constexpr std::size_t ENTITY_COUNT =
    static_cast<std::size_t>(rq::EntityKind::LAST) - 1;

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

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSimpleBuiltinSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledBuiltinSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsUnarySubtypeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCountedSubtypeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCompositeSubtypeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTableSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternativeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcreteSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtypeSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformChangingSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedSymbol(rq::EntityKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrameSymbol(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getTemplate(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getPartial(rq::EntityKind kind);
[[nodiscard]] inline rq::EntityKind getFullSpecialization(rq::EntityKind kind);

struct Entity {
  using Self = Entity;

  rq::EntityKind _kind;

  inline explicit Entity(rq::EntityKind kind);
  Entity(const Self &) = delete;
  Entity(Self &&) = delete;
  ~Entity() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getKindName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeDefinitionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltinSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferenceSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeTypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbolSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVoidSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoReturnSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariadicArgumentsSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBooleanSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericFloatSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHalfSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSingleSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDoubleSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsQuadrupleSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBinarySymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBfloatSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary16Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary32Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary64Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary128Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloat16Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericIntegerSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericUnsignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericCodeunitSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAsciiSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUtf8Symbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltinSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledSignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledUnsignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRangeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReferenceSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointerSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFatPointerSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferencedCountArraySymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArraySymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequenceSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticIntervalSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsFiniteArithmeticProgressionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsInfiniteArithmeticProgressionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModuleSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImportSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCategoryDiscriminantSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumeratorSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCategorySymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCategoryAlternativeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassParameterSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutParameterSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateParameterSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureParameterSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLabelSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumerationSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntrySymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRangerSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateClassSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateEnumerationSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateDynamicVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateStaticVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateExtensionFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateExtensionMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialClassSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialEnumerationSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialDynamicVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialStaticVariableSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsPartialExtensionFunctionSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialExtensionMethodSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArrayConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternativeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcreteSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtypeSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChangingSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrameSymbol() const;
};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTableSymbol;
  using reference = rq::SymbolTableSymbol &;
  using pointer = rq::SymbolTableSymbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTableSymbol *_symbol_table_ptr = nullptr;

  SymbolTableIterator() = default;
  inline explicit SymbolTableIterator(rq::SymbolTableSymbol *symbol_table_ptr);
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = const rq::SymbolTableSymbol;
  using reference = const rq::SymbolTableSymbol &;
  using pointer = const rq::SymbolTableSymbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTableSymbol *_symbol_table_ptr = nullptr;

  ConstSymbolTableIterator() = default;
  inline explicit ConstSymbolTableIterator(
      const rq::SymbolTableSymbol *symbol_table_ptr);
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

// ROOT WITH TYPE ATTRIBUTES
struct TypeDefinitionSymbol;

// SIMPLE BUILTIN
struct SimpleBuiltinSymbol;
struct InferenceSymbol;
struct ExpressionSymbol;
struct VoidSymbol;
struct NullSymbol;
struct NoReturnSymbol;
struct VariadicArgumentsSymbol;
struct BooleanSymbol;
struct GenericFloatSymbol;
struct HalfSymbol;
struct SingleSymbol;
struct DoubleSymbol;
struct QuadrupleSymbol;
struct GenericBinarySymbol;
struct GenericBfloatSymbol;
struct Binary16Symbol;
struct Binary32Symbol;
struct Binary64Symbol;
struct Binary128Symbol;
struct Bfloat16Symbol;
struct GenericIntegerSymbol;
struct GenericSignedSymbol;
struct GenericUnsignedSymbol;
struct GenericStringSymbol;
struct GenericCodeunitSymbol;
struct AsciiSymbol;
struct Utf8Symbol;

// SCALED BUILTIN
struct ScaledBuiltinSymbol;
struct ScaledSignedSymbol;
struct ScaledUnsignedSymbol;

// UNARY SUBTYPE
struct UnarySubtypeSymbol;
struct RangeSymbol;
struct ReferenceSymbol;
struct PointerSymbol;
struct FatPointerSymbol;
struct InferencedCountArraySymbol;

// COUNTED SUBTYPE
struct CountedSubtypeSymbol;
struct ArraySymbol;

// COMPOSITE SUBTYPE
struct LayoutSymbol;
struct SignatureSymbol;

// ARITHMETIC SEQUENCE
struct ArithmeticSequenceSymbol;
struct ArithmeticIntervalSymbol;
struct FiniteArithmeticProgressionSymbol;
struct InfiniteArithmeticProgressionSymbol;

// MISC
struct LabelSymbol;
struct ModuleSymbol;
struct ImportSymbol;
struct ExtensionSymbol;
struct CodeSymbol;
struct CategoryDiscriminantSymbol;

// BINDING
struct DynamicVariableSymbol;
struct StaticVariableSymbol;
struct EnumeratorSymbol;
struct CategoryAlternativeSymbol;
struct ClassParameterSymbol;
struct LayoutParameterSymbol;
struct TemplateParameterSymbol;
struct SignatureParameterSymbol;

// SYMBOL TABLES
struct SymbolTableSymbol;
struct TopSymbol;
struct ScopeSymbol;
struct TableSymbol;
struct ClassSymbol;
struct EnumerationSymbol;
struct CategorySymbol;

// PROCEDURE
struct ProcedureSymbol;
struct EntrySymbol;
struct FunctionSymbol;
struct MethodSymbol;
struct ExtensionFunctionSymbol;
struct ExtensionMethodSymbol;
struct RangerSymbol;

// TEMPLATE
struct TemplateSymbol;
struct TemplateClassSymbol;
struct TemplateEnumerationSymbol;
struct TemplateDynamicVariableSymbol;
struct TemplateStaticVariableSymbol;
struct TemplateFunctionSymbol;
struct TemplateMethodSymbol;
struct TemplateExtensionFunctionSymbol;
struct TemplateExtensionMethodSymbol;

// PARTIAL SPECIALIZATION
struct PartialSymbol;
struct PartialClassSymbol;
struct PartialEnumerationSymbol;
struct PartialDynamicVariableSymbol;
struct PartialStaticVariableSymbol;
struct PartialFunctionSymbol;
struct PartialMethodSymbol;
struct PartialExtensionFunctionSymbol;
struct PartialExtensionMethodSymbol;

// CONSTANTS
struct IntegerConstant;
struct FloatConstant;
struct StringConstant;
struct ArrayConstant;

// INSTRUCTION

struct Instruction;

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  inline explicit Symbol(rq::EntityKind kind);
  Symbol(const Self &) = delete;
  Symbol(Self &&) = delete;
  ~Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SimpleBuiltinSymbol : public rq::Symbol {
  using Self = rq::SimpleBuiltinSymbol;

  inline explicit SimpleBuiltinSymbol(rq::EntityKind kind);
  SimpleBuiltinSymbol(const Self &) = delete;
  SimpleBuiltinSymbol(Self &&) = delete;
  ~SimpleBuiltinSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

static constexpr unsigned MAX_SCALED_BUILTIN_SCALAR =
    std::numeric_limits<std::uint16_t>::max();

static constexpr unsigned MAX_SCALED_BUILTIN_UID =
    std::numeric_limits<std::uint16_t>::max();

enum class ScaledBuiltinFlags : std::uint8_t {
  NONE,
  BYTES = rq::getBit(0),
  INDEX = rq::getBit(1),
  ADDRESS = rq::getBit(2),
  BITS_NONE_MASK = BYTES | INDEX | ADDRESS,
  FASTEST = rq::getBit(3),
  LEAST = rq::getBit(4),
  EXACT_NONE_MASK = FASTEST | LEAST,
  LITTLE_ENDIAN_ = rq::getBit(5),
  BIG_ENDIAN_ = rq::getBit(6),
  PLATFORM_ENDIAN_NONE_MASK = LITTLE_ENDIAN_ | BIG_ENDIAN_,
  PLATFORM_SCALAR = rq::getBit(7)
};

template <> struct is_flags<rq::ScaledBuiltinFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBitsScalar(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBytesScalar(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasIndexScalar(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAddressScalar(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformEndian(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsBigEndian(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsLittleEndian(rq::ScaledBuiltinFlags flags);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPlatformScalar(rq::ScaledBuiltinFlags flags);
void RQ_ALWAYS_INLINE profileScaledIntegerSymbol(llvm::FoldingSetNodeID &id,
                                                 rq::EntityKind kind,
                                                 std::uint16_t scalar,
                                                 std::uint16_t uid,
                                                 rq::ScaledBuiltinFlags flags);
struct ScaledBuiltinSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledBuiltinSymbol;
  friend struct Context;

  std::uint16_t _scalar;
  std::uint16_t _uid;
  rq::ScaledBuiltinFlags _flags;

  inline ScaledBuiltinSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags);
  ScaledBuiltinSymbol(const Self &) = delete;
  ScaledBuiltinSymbol(Self &&) = delete;
  ~ScaledBuiltinSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getUid() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinFlags getFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBytesScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndexScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformEndian() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBigEndian() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLittleEndian() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPlatformScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSynonym() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};
struct ScaledIntegerSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::ScaledIntegerSymbol;

  inline ScaledIntegerSymbol(unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags);
  inline ScaledIntegerSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags);
  ScaledIntegerSymbol(const Self &) = delete;
  ScaledIntegerSymbol(Self &&) = delete;
  ~ScaledIntegerSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};
struct FloatSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::FloatSymbol;

  inline FloatSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags);
  inline FloatSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
              rq::ScaledBuiltinFlags flags);
  FloatSymbol(const Self &) = delete;
  FloatSymbol(Self &&) = delete;
  ~FloatSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};
void RQ_ALWAYS_INLINE profileUnarySubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                rq::EntityKind kind,
                                                const rq::Symbol &root);
struct UnarySubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtypeSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr{nullptr};

  inline UnarySubtypeSymbol(rq::EntityKind kind, rq::Symbol &root);
  UnarySubtypeSymbol(const Self &) = delete;
  UnarySubtypeSymbol(Self &&) = delete;
  ~UnarySubtypeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

void RQ_ALWAYS_INLINE profileCountedSubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                  rq::EntityKind kind,
                                                  const rq::Symbol &root,
                                                  unsigned count);
struct CountedSubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::CountedSubtypeSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr;
  std::size_t _count;

  inline CountedSubtypeSymbol(rq::EntityKind kind, rq::Symbol &root,
                              std::size_t count);
  CountedSubtypeSymbol(const Self &) = delete;
  CountedSubtypeSymbol(Self &&) = delete;
  ~CountedSubtypeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};
void RQ_ALWAYS_INLINE profileArithmeticSequenceSymbol(
    llvm::FoldingSetNodeID &id, const rq::Symbol &root,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step);
struct ArithmeticSequenceSymbol : public rq::Symbol,
                                  public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  inline ArithmeticSequenceSymbol(rq::EntityKind kind, rq::Symbol &root,
                                  rq::ArithmeticSequenceCondition condition,
                                  rq::ArithmeticSequenceStep step);
  ArithmeticSequenceSymbol(const Self &) = delete;
  ArithmeticSequenceSymbol(Self &&) = delete;
  ~ArithmeticSequenceSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

namespace detail {
struct HasLocationSymbol {
  using Self = rq::detail::HasLocationSymbol;

  rq::Expression *_expression_ptr{nullptr};

  inline explicit HasLocationSymbol(rq::Expression &expression);
  HasLocationSymbol() = default;
  HasLocationSymbol(const Self &) = delete;
  HasLocationSymbol(Self &&) = delete;
  ~HasLocationSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr);
};
struct ModuleMemberSymbol {
  using Self = rq::detail::ModuleMemberSymbol;

  rq::ModuleSymbol *_module_ptr;

  inline explicit ModuleMemberSymbol(rq::ModuleSymbol &module);
  ModuleMemberSymbol(const Self &) = delete;
  ModuleMemberSymbol(Self &&) = delete;
  ~ModuleMemberSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getModule();
};
struct SymbolTableMemberSymbol {
  using Self = rq::detail::SymbolTableMemberSymbol;

  rq::SymbolTableSymbol *_containing_symbol_table_ptr{nullptr};

  SymbolTableMemberSymbol() = default;
  inline explicit SymbolTableMemberSymbol(
      rq::SymbolTableSymbol &containing_table);
  SymbolTableMemberSymbol(const Self &) = delete;
  SymbolTableMemberSymbol(Self &&) = delete;
  ~SymbolTableMemberSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingSymbolTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  getContainingSymbolTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &
  getContainingSymbolTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  getContainingSymbolTablePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *
  getContainingSymbolTablePtr();
};
struct HasAttributesSymbol {
  using Self = HasAttributesSymbol;

  rq::ExpressionAttributeFlags _attributes;

  inline explicit HasAttributesSymbol(rq::ExpressionAttributeFlags attributes);
  HasAttributesSymbol(const Self &) = delete;
  HasAttributesSymbol(Self &&) = delete;
  ~HasAttributesSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttributeFlags
  getExpressionAttributes() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOutside() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapture() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEager() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayParent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasParent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAbstract() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVirtual() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverride() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPosition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangle() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPack() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLabel() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikely() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnlikely() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDepreciated() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExport() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPublic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasProtected() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayCopy() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayMove() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOk() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::ExpressionAttribute attribute) const;
};
struct HasNameSymbol {
  using Self = rq::detail::HasNameSymbol;

  llvm::StringRef _name;

  inline explicit HasNameSymbol(llvm::StringRef name);
  HasNameSymbol(const Self &) = delete;
  HasNameSymbol(Self &&) = delete;
  ~HasNameSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
};
struct MaybeHasNameSymbol {
  using Self = rq::detail::MaybeHasNameSymbol;

  llvm::StringRef _name{};

  MaybeHasNameSymbol() = default;
  inline explicit MaybeHasNameSymbol(llvm::StringRef name);
  MaybeHasNameSymbol(const Self &) = delete;
  MaybeHasNameSymbol(Self &&) = delete;
  ~MaybeHasNameSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
};
struct HasBindingTypeSymbol {
  using Self = rq::detail::HasBindingTypeSymbol;

  rq::TypeDefinitionSymbol *_binding_type_ptr{nullptr};

  HasBindingTypeSymbol() = default;
  HasBindingTypeSymbol(const Self &) = delete;
  HasBindingTypeSymbol(Self &&) = delete;
  ~HasBindingTypeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBindingType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeDefinitionSymbol &
  getBindingType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeDefinitionSymbol &getBindingType();
  RQ_ALWAYS_INLINE void setBindingType(rq::TypeDefinitionSymbol &type);
};
struct HasImportModuleSymbol {
  using Self = rq::detail::HasImportModuleSymbol;

  rq::ModuleSymbol *_import_module_ptr{nullptr};

  HasImportModuleSymbol() = default;
  HasImportModuleSymbol(const Self &) = delete;
  HasImportModuleSymbol(Self &&) = delete;
  ~HasImportModuleSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasImportModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &
  getImportModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getImportModule();
  RQ_ALWAYS_INLINE void setImportModule(rq::ModuleSymbol &import_module);
};
} // namespace detail

struct SymbolTableSymbol : public rq::Symbol,
                           public rq::detail::SymbolTableMemberSymbol {
  using Self = rq::SymbolTableSymbol;

  llvm::SmallDenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_values{};
  rq::BumpPtrList<rq::Symbol> _unamed_values{};

  inline explicit SymbolTableSymbol(rq::EntityKind kind);
  inline SymbolTableSymbol(rq::EntityKind kind,
                    rq::SymbolTableSymbol &containing_table);
  SymbolTableSymbol(const Self &) = delete;
  SymbolTableSymbol(Self &&) = delete;
  ~SymbolTableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const;
  void tabulateNamedSymbol(rq::Context &context, llvm::StringRef name,
                           rq::Symbol &symbol);
  void tabulateUnamedSymbol(rq::Context &context, rq::Symbol &symbol);
  [[nodiscard]] inline rq::BumpPtrList<rq::Symbol>
  getNamedList(llvm::StringRef name);
  [[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol>
  getNamedList(llvm::StringRef name) const;
  [[nodiscard]] inline rq::BumpPtrList<rq::Symbol> getUnnamedList();
  [[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol> getUnnamedList() const;
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedListRange();
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedListRange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE auto getAscendingFrameRange();
  [[nodiscard]] RQ_ALWAYS_INLINE auto getAscendingFrameRange() const;
};

struct ProcedureSymbol : public rq::SymbolTableSymbol,
                         public rq::detail::HasLocationSymbol,
                         public rq::detail::ModuleMemberSymbol,
                         public rq::detail::HasAttributesSymbol {
  using Self = rq::ProcedureSymbol;

  rq::SignatureSymbol *_signature_ptr;

  inline ProcedureSymbol(rq::EntityKind kind, rq::Expression &expression,
                         rq::ModuleSymbol &module,
                         rq::SymbolTableSymbol &containing_table,
                         rq::ExpressionAttributeFlags attributes);
  ProcedureSymbol(const Self &) = delete;
  ProcedureSymbol(Self &&) = delete;
  ~ProcedureSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateSymbol : public rq::Symbol {
  using Self = rq::TemplateSymbol;

  rq::BumpPtrList<rq::TemplateParameterSymbol> _template_parameters;

  inline TemplateSymbol(
      rq::EntityKind kind,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateSymbol(const Self &) = delete;
  TemplateSymbol(Self &&) = delete;
  ~TemplateSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialSymbol : public rq::Symbol {
  using Self = rq::PartialSymbol;

  inline explicit PartialSymbol(rq::EntityKind kind);
  PartialSymbol(const Self &) = delete;
  PartialSymbol(Self &&) = delete;
  ~PartialSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace rq
namespace llvm {

// ROOT WITH TYPE ATTRIBUTES
template <> struct isa_impl<rq::TypeDefinitionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

// SIMPLE BUILTIN
template <> struct isa_impl<rq::SimpleBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::InferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::InferenceSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::ExpressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ExpressionSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::VoidSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::VoidSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::NullSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::NullSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::VariadicArgumentsSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::VariadicArgumentsSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::BooleanSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::BooleanSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericFloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::GenericFloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::HalfSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::HalfSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::SingleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::SingleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::DoubleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::DoubleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Binary16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Binary16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Binary32Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Binary32Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Binary64Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Binary64Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Binary128Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Binary128Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::GenericUnsignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::GenericCodeunitSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::GenericCodeunitSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::AsciiSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::AsciiSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

template <> struct isa_impl<rq::Utf8Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::Utf8Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val);
};

// SCALED BUILTIN
template <> struct isa_impl<rq::ScaledBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val);
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val);
};

// UNARY SUBTYPE
template <> struct isa_impl<rq::UnarySubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::RangeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::RangeSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val);
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val);
};

template <> struct isa_impl<rq::PointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val);
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val);
};

template <> struct isa_impl<rq::InferencedCountArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::InferencedCountArraySymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val);
};

// COUNTED SUBTYPE
template <> struct isa_impl<rq::CountedSubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ArraySymbol, rq::CountedSubtypeSymbol> {
  static inline bool doit(const rq::CountedSubtypeSymbol &val);
};

// COMPOSITE SUBTYPE
template <> struct isa_impl<rq::LayoutSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::SignatureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ExtensionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

// ARITHMETIC SEQUENCE
template <> struct isa_impl<rq::ArithmeticSequenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ArithmeticIntervalSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::ArithmeticIntervalSymbol, rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val);
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val);
};

template <> struct isa_impl<rq::FiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::FiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val);
};

// MISC
template <> struct isa_impl<rq::LabelSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ModuleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ImportSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::CodeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::CategorySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::CategorySymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

template <> struct isa_impl<rq::CategoryDiscriminantSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

// BINDING
template <> struct isa_impl<rq::DynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::StaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::EnumeratorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::CategoryAlternativeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::SignatureParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ClassParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::LayoutParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

// SYMBOL TABLES
template <> struct isa_impl<rq::SymbolTableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TopSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TopSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

template <> struct isa_impl<rq::ScopeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ScopeSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

template <> struct isa_impl<rq::TableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TableSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

template <> struct isa_impl<rq::ClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ClassSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val);
};

// PROCEDURES
template <> struct isa_impl<rq::ProcedureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::EntrySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::EntrySymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

template <> struct isa_impl<rq::FunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::FunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

template <> struct isa_impl<rq::MethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::MethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

template <> struct isa_impl<rq::RangerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::RangerSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val);
};

// TEMPLATE
template <> struct isa_impl<rq::TemplateSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateClassSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::TemplateStaticVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

template <> struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val);
};

// PARTIAL SPECIALIZATION
template <> struct isa_impl<rq::PartialSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::PartialDynamicVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::PartialStaticVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::PartialExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val);
};

template <>
struct isa_impl<rq::PartialExtensionMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val);
};

template <> struct isa_impl<rq::IntegerConstant, rq::Entity> {
  static inline bool doit(const rq::Entity &val);
};

template <> struct isa_impl<rq::FloatConstant, rq::Entity> {
  static inline bool doit(const rq::Entity &val);
};

template <> struct isa_impl<rq::StringConstant, rq::Entity> {
  static inline bool doit(const rq::Entity &val);
};

template <> struct isa_impl<rq::ArrayConstant, rq::Entity> {
  static inline bool doit(const rq::Entity &val);
};

// INSTRUCTION

template <> struct isa_impl<rq::Instruction, rq::Entity> {
  static inline bool doit(const rq::Entity &val);
};

} // namespace llvm
namespace rq {

struct TypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::TypeSymbol;

  rq::Symbol *_root_ptr{nullptr};
  rq::TypeAttributeFlags _flags{};

  inline TypeSymbol(rq::EntityKind kind, rq::Symbol &root,
                    rq::TypeAttributeFlags flags);
  TypeSymbol(const Self &) = delete;
  TypeSymbol(Self &&) = delete;
  ~TypeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttributeFlags getFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMutable(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasConstant(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasPartiallyMutable(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasVolatile(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAtomic(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasNullTerminated(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMayDiscard(rq::TypeAttribute attribute) const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

struct InferenceSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::InferenceSymbol;

  inline InferenceSymbol();
  InferenceSymbol(const Self &) = delete;
  InferenceSymbol(Self &&) = delete;
  ~InferenceSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExpressionSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::ExpressionSymbol;

  inline ExpressionSymbol();
  ExpressionSymbol(const Self &) = delete;
  ExpressionSymbol(Self &&) = delete;
  ~ExpressionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VoidSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VoidSymbol;

  inline VoidSymbol();
  VoidSymbol(const Self &) = delete;
  VoidSymbol(Self &&) = delete;
  ~VoidSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NullSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NullSymbol;

  inline NullSymbol();
  NullSymbol(const Self &) = delete;
  NullSymbol(Self &&) = delete;
  ~NullSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NoReturnSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NoReturnSymbol;

  inline NoReturnSymbol();
  NoReturnSymbol(const Self &) = delete;
  NoReturnSymbol(Self &&) = delete;
  ~NoReturnSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VariadicArgumentsSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VariadicArgumentsSymbol;

  inline VariadicArgumentsSymbol();
  VariadicArgumentsSymbol(const Self &) = delete;
  VariadicArgumentsSymbol(Self &&) = delete;
  ~VariadicArgumentsSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BooleanSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::BooleanSymbol;

  inline BooleanSymbol();
  BooleanSymbol(const Self &) = delete;
  BooleanSymbol(Self &&) = delete;
  ~BooleanSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericFloatSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericFloatSymbol;

  inline GenericFloatSymbol();
  GenericFloatSymbol(const Self &) = delete;
  GenericFloatSymbol(Self &&) = delete;
  ~GenericFloatSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct HalfSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::HalfSymbol;

  inline HalfSymbol();
  HalfSymbol(const Self &) = delete;
  HalfSymbol(Self &&) = delete;
  ~HalfSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SingleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::SingleSymbol;

  inline SingleSymbol();
  SingleSymbol(const Self &) = delete;
  SingleSymbol(Self &&) = delete;
  ~SingleSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DoubleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::DoubleSymbol;

  inline DoubleSymbol();
  DoubleSymbol(const Self &) = delete;
  DoubleSymbol(Self &&) = delete;
  ~DoubleSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct QuadrupleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::QuadrupleSymbol;

  inline QuadrupleSymbol();
  QuadrupleSymbol(const Self &) = delete;
  QuadrupleSymbol(Self &&) = delete;
  ~QuadrupleSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericBinarySymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericBinarySymbol;

  inline GenericBinarySymbol();
  GenericBinarySymbol(const Self &) = delete;
  GenericBinarySymbol(Self &&) = delete;
  ~GenericBinarySymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericBfloatSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericBfloatSymbol;

  inline GenericBfloatSymbol();
  GenericBfloatSymbol(const Self &) = delete;
  GenericBfloatSymbol(Self &&) = delete;
  ~GenericBfloatSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary16Symbol;

  inline Binary16Symbol();
  Binary16Symbol(const Self &) = delete;
  Binary16Symbol(Self &&) = delete;
  ~Binary16Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary32Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary32Symbol;

  inline Binary32Symbol();
  Binary32Symbol(const Self &) = delete;
  Binary32Symbol(Self &&) = delete;
  ~Binary32Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary64Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary64Symbol;

  inline Binary64Symbol();
  Binary64Symbol(const Self &) = delete;
  Binary64Symbol(Self &&) = delete;
  ~Binary64Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary128Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary128Symbol;

  inline Binary128Symbol();
  Binary128Symbol(const Self &) = delete;
  Binary128Symbol(Self &&) = delete;
  ~Binary128Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Bfloat16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Bfloat16Symbol;

  inline Bfloat16Symbol();
  Bfloat16Symbol(const Self &) = delete;
  Bfloat16Symbol(Self &&) = delete;
  ~Bfloat16Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericIntegerSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericIntegerSymbol;

  inline GenericIntegerSymbol();
  GenericIntegerSymbol(const Self &) = delete;
  GenericIntegerSymbol(Self &&) = delete;
  ~GenericIntegerSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericSignedSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericSignedSymbol;

  inline GenericSignedSymbol();
  GenericSignedSymbol(const Self &) = delete;
  GenericSignedSymbol(Self &&) = delete;
  ~GenericSignedSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericUnsignedSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericUnsignedSymbol;

  inline GenericUnsignedSymbol();
  GenericUnsignedSymbol(const Self &) = delete;
  GenericUnsignedSymbol(Self &&) = delete;
  ~GenericUnsignedSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericStringSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericStringSymbol;

  inline GenericStringSymbol();
  GenericStringSymbol(const Self &) = delete;
  GenericStringSymbol(Self &&) = delete;
  ~GenericStringSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericCodeunitSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericCodeunitSymbol;

  inline GenericCodeunitSymbol();
  GenericCodeunitSymbol(const Self &) = delete;
  GenericCodeunitSymbol(Self &&) = delete;
  ~GenericCodeunitSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct AsciiSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;

  inline AsciiSymbol();
  AsciiSymbol(const Self &) = delete;
  AsciiSymbol(Self &&) = delete;
  ~AsciiSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Utf8Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;
  friend struct Context;

private:
  inline Utf8Symbol();
  Utf8Symbol(const Self &) = delete;
  Utf8Symbol(Self &&) = delete;
  ~Utf8Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScaledUnsignedSymbol : public rq::ScaledIntegerSymbol {
  using Self = rq::ScaledUnsignedSymbol;
  friend struct Context;

private:
  inline ScaledUnsignedSymbol(unsigned scalar, unsigned uid,
                              rq::ScaledBuiltinFlags flags);
  ScaledUnsignedSymbol(const Self &) = delete;
  ScaledUnsignedSymbol(Self &&) = delete;
  ~ScaledUnsignedSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScaledSignedSymbol : public rq::ScaledIntegerSymbol {
  using Self = rq::ScaledSignedSymbol;
  friend struct Context;

private:
  inline ScaledSignedSymbol(unsigned scalar, unsigned uid,
                            rq::ScaledBuiltinFlags flags);
  ScaledSignedSymbol(const Self &) = delete;
  ScaledSignedSymbol(Self &&) = delete;
  ~ScaledSignedSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangeSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::RangeSymbol;
  friend struct Context;

private:
  inline RangeSymbol(rq::Symbol &root);
  RangeSymbol(const Self &) = delete;
  RangeSymbol(Self &&) = delete;
  ~RangeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ReferenceSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::ReferenceSymbol;
  friend struct Context;

private:
  inline ReferenceSymbol(rq::Symbol &root);
  ReferenceSymbol(const Self &) = delete;
  ReferenceSymbol(Self &&) = delete;
  ~ReferenceSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PointerSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::PointerSymbol;
  friend struct Context;

private:
  inline PointerSymbol(rq::Symbol &root);
  PointerSymbol(const Self &) = delete;
  PointerSymbol(Self &&) = delete;
  ~PointerSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FatPointerSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::FatPointerSymbol;
  friend struct Context;

private:
  inline FatPointerSymbol(rq::Symbol &root);
  FatPointerSymbol(const Self &) = delete;
  FatPointerSymbol(Self &&) = delete;
  ~FatPointerSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InferencedCountArraySymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::InferencedCountArraySymbol;
  friend struct Context;

private:
  inline InferencedCountArraySymbol(rq::Symbol &root);
  InferencedCountArraySymbol(const Self &) = delete;
  InferencedCountArraySymbol(Self &&) = delete;
  ~InferencedCountArraySymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ArraySymbol : public rq::CountedSubtypeSymbol {
  using Self = rq::ArraySymbol;
  friend struct Context;

private:
  inline ArraySymbol(rq::Symbol &root, std::size_t count);
  ArraySymbol(const Self &) = delete;
  ArraySymbol(Self &&) = delete;
  ~ArraySymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LayoutSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::LayoutSymbol;
  friend struct Context;

  rq::BumpPtrList<rq::LayoutParameterSymbol> _properties;

private:
  inline LayoutSymbol(rq::BumpPtrList<rq::LayoutParameterSymbol> properties);
  LayoutSymbol(const Self &) = delete;
  LayoutSymbol(Self &&) = delete;
  ~LayoutSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::LayoutParameterSymbol>
  getProperties() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

struct SignatureSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SignatureSymbol;
  friend struct Context;

  rq::Symbol *_return_ptr;
  rq::BumpPtrList<rq::Symbol> _parameters;

private:
  inline SignatureSymbol(rq::Symbol &return_,
                         rq::BumpPtrList<rq::Symbol> parameters);
  SignatureSymbol(const Self &) = delete;
  SignatureSymbol(Self &&) = delete;
  ~SignatureSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getReturn();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getReturn() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Symbol>
  getParameters() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

struct ArithmeticIntervalSymbol : public rq::ArithmeticSequenceSymbol {
  using Self = rq::ArithmeticIntervalSymbol;

  inline ArithmeticIntervalSymbol(rq::Symbol &root,
                                  rq::ArithmeticSequenceCondition condition);
  ArithmeticIntervalSymbol(const Self &) = delete;
  ArithmeticIntervalSymbol(Self &&) = delete;
  ~ArithmeticIntervalSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FiniteArithmeticProgressionSymbol : public rq::ArithmeticSequenceSymbol {
  using Self = rq::FiniteArithmeticProgressionSymbol;

  FiniteArithmeticProgressionSymbol(rq::Symbol &root,
                                    rq::ArithmeticSequenceCondition condition,
                                    rq::ArithmeticSequenceStep step);
  FiniteArithmeticProgressionSymbol(const Self &) = delete;
  FiniteArithmeticProgressionSymbol(Self &&) = delete;
  ~FiniteArithmeticProgressionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InfiniteArithmeticProgressionSymbol
    : public rq::ArithmeticSequenceSymbol {
  using Self = rq::FiniteArithmeticProgressionSymbol;

  inline InfiniteArithmeticProgressionSymbol(rq::Symbol &root,
                                             rq::ArithmeticSequenceStep step);
  InfiniteArithmeticProgressionSymbol(const Self &) = delete;
  InfiniteArithmeticProgressionSymbol(Self &&) = delete;
  ~InfiniteArithmeticProgressionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

struct ModuleSymbol final : public rq::Symbol,
                            public rq::detail::HasLocationSymbol {
  using Self = rq::ModuleSymbol;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;

  inline ModuleSymbol(rq::ModuleKind kind, llvm::StringRef path,
                      llvm::MemoryBufferRef &&buffer);
  ModuleSymbol(const Self &) = delete;
  ModuleSymbol(Self &&) = delete;
  ~ModuleSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const;
  RQ_ALWAYS_INLINE void changeExpression(rq::Expression &expression);
  [[nodiscard]] inline rq::Expression &popExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression);
};

struct ImportSymbol final : public rq::Symbol,
                            public rq::detail::HasLocationSymbol,
                            public rq::detail::ModuleMemberSymbol,
                            public rq::detail::SymbolTableMemberSymbol,
                            public rq::detail::HasAttributesSymbol,
                            public rq::detail::HasImportModuleSymbol {
  using Self = rq::ImportSymbol;

  inline ImportSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                      rq::SymbolTableSymbol &containing_table,
                      rq::ExpressionAttributeFlags attributes);
  ImportSymbol(const Self &) = delete;
  ImportSymbol(Self &&) = delete;
  ~ImportSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ExtensionSymbol;
  friend struct Context;

  rq::SignatureSymbol *_signature_ptr{nullptr};
  rq::TypeSymbol *_type_ptr{nullptr};

private:
  inline ExtensionSymbol(rq::SignatureSymbol &signature, rq::TypeSymbol &type);
  ExtensionSymbol(const Self &) = delete;
  ExtensionSymbol(Self &&) = delete;
  ~ExtensionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureSymbol &getSignature();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureSymbol &
  getSignature() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeSymbol &getType() const;
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

struct CodeSymbol : public rq::Symbol,
                    public rq::detail::HasLocationSymbol,
                    public rq::detail::HasNameSymbol,
                    public rq::detail::ModuleMemberSymbol,
                    public rq::detail::HasAttributesSymbol,
                    public rq::detail::SymbolTableMemberSymbol {
  using Self = rq::CodeSymbol;

  inline CodeSymbol(rq::Expression &expression, llvm::StringRef name,
                    rq::ModuleSymbol &module,
                    rq::ExpressionAttributeFlags attributes,
                    rq::SymbolTableSymbol &containing_table);
  CodeSymbol(const Self &) = delete;
  CodeSymbol(Self &&) = delete;
  ~CodeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct CategorySymbol : public rq::SymbolTableSymbol,
                        public rq::detail::HasLocationSymbol,
                        public rq::detail::HasNameSymbol,
                        public rq::detail::ModuleMemberSymbol,
                        public rq::detail::HasAttributesSymbol {
  using Self = rq::CategorySymbol;

  inline CategorySymbol(rq::Expression &expression, llvm::StringRef name,
                        rq::ModuleSymbol &module,
                        rq::ExpressionAttributeFlags attributes,
                        rq::SymbolTableSymbol &containing_table);
  CategorySymbol(const Self &) = delete;
  CategorySymbol(Self &&) = delete;
  ~CategorySymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TopSymbol : rq::SymbolTableSymbol {
  using Self = rq::TopSymbol;

  inline TopSymbol();
  TopSymbol(const Self &) = delete;
  TopSymbol(Self &&) = delete;
  ~TopSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScopeSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol {
  using Self = rq::ScopeSymbol;

  ScopeSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &containing_table);
  ScopeSymbol(const Self &) = delete;
  ScopeSymbol(Self &&) = delete;
  ~ScopeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DynamicVariableSymbol : public rq::Symbol,
                               public rq::detail::HasLocationSymbol,
                               public rq::detail::ModuleMemberSymbol,
                               public rq::detail::SymbolTableMemberSymbol,
                               public rq::detail::HasAttributesSymbol,
                               public rq::detail::HasNameSymbol,
                               public rq::detail::HasBindingTypeSymbol {
  using Self = rq::DynamicVariableSymbol;

  DynamicVariableSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes);
  DynamicVariableSymbol(const Self &) = delete;
  DynamicVariableSymbol(Self &&) = delete;
  ~DynamicVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct StaticVariableSymbol : public rq::Symbol,
                              public rq::detail::HasLocationSymbol,
                              public rq::detail::ModuleMemberSymbol,
                              public rq::detail::SymbolTableMemberSymbol,
                              public rq::detail::HasAttributesSymbol,
                              public rq::detail::HasNameSymbol,
                              public rq::detail::HasBindingTypeSymbol {
  using Self = rq::StaticVariableSymbol;

  rq::Gendex<rq::SymbolicValue> _symbolic_value{};

  inline StaticVariableSymbol(rq::Expression &expression,
                              rq::ModuleSymbol &module,
                              rq::SymbolTableSymbol &containing_table,
                              llvm::StringRef name,
                              rq::ExpressionAttributeFlags attributes);
  StaticVariableSymbol(const Self &) = delete;
  StaticVariableSymbol(Self &&) = delete;
  ~StaticVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumeratorSymbol : public rq::Symbol,
                          public rq::detail::HasLocationSymbol,
                          public rq::detail::ModuleMemberSymbol,
                          public rq::detail::SymbolTableMemberSymbol,
                          public rq::detail::HasAttributesSymbol,
                          public rq::detail::HasNameSymbol,
                          public rq::detail::HasBindingTypeSymbol {
  using Self = rq::EnumeratorSymbol;

  EnumeratorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                   rq::EnumerationSymbol &enumeration, llvm::StringRef name,
                   rq::ExpressionAttributeFlags attributes);
  EnumeratorSymbol(const Self &) = delete;
  EnumeratorSymbol(Self &&) = delete;
  ~EnumeratorSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumerationSymbol &getEnumeration();
};

struct CategoryAlternativeSymbol : public rq::Symbol,
                                   public rq::detail::HasLocationSymbol,
                                   public rq::detail::ModuleMemberSymbol,
                                   public rq::detail::SymbolTableMemberSymbol,
                                   public rq::detail::HasAttributesSymbol,
                                   public rq::detail::HasBindingTypeSymbol {
  using Self = rq::CategoryAlternativeSymbol;

  rq::CodeSymbol *_code_ptr;

  inline CategoryAlternativeSymbol(rq::Expression &expression,
                                   rq::ModuleSymbol &module,
                                   rq::CategorySymbol &category,
                                   rq::ExpressionAttributeFlags attributes);
  CategoryAlternativeSymbol(const Self &) = delete;
  CategoryAlternativeSymbol(Self &&) = delete;
  ~CategoryAlternativeSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CategorySymbol &getCategory();
};

struct SignatureParameterSymbol : public rq::Symbol,
                                  public rq::detail::HasLocationSymbol,
                                  public rq::detail::ModuleMemberSymbol,
                                  public rq::detail::SymbolTableMemberSymbol,
                                  public rq::detail::HasAttributesSymbol,
                                  public rq::detail::MaybeHasNameSymbol,
                                  public rq::detail::HasBindingTypeSymbol {
  using Self = rq::SignatureParameterSymbol;

  rq::SignatureSymbol *_signature_ptr;

  inline SignatureParameterSymbol(rq::Expression &expression,
                                  rq::ModuleSymbol &module,
                                  rq::SymbolTableSymbol &containing_table,
                                  rq::ExpressionAttributeFlags attributes);
  inline SignatureParameterSymbol(rq::Expression &expression,
                                  rq::ModuleSymbol &module,
                                  rq::SymbolTableSymbol &containing_table,
                                  llvm::StringRef name,
                                  rq::ExpressionAttributeFlags attributes);
  SignatureParameterSymbol(const Self &) = delete;
  SignatureParameterSymbol(Self &&) = delete;
  ~SignatureParameterSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateParameterSymbol : public rq::Symbol,
                                 public rq::detail::HasLocationSymbol,
                                 public rq::detail::ModuleMemberSymbol,
                                 public rq::detail::SymbolTableMemberSymbol,
                                 public rq::detail::HasAttributesSymbol,
                                 public rq::detail::MaybeHasNameSymbol,
                                 public rq::detail::HasBindingTypeSymbol {
  using Self = rq::TemplateParameterSymbol;

  rq::TemplateSymbol *_template_ptr;

  inline TemplateParameterSymbol(rq::Expression &expression,
                                 rq::ModuleSymbol &module,
                                 rq::SymbolTableSymbol &containing_table,
                                 rq::ExpressionAttributeFlags attributes);
  inline TemplateParameterSymbol(rq::Expression &expression,
                                 rq::ModuleSymbol &module,
                                 rq::SymbolTableSymbol &containing_table,
                                 llvm::StringRef name,
                                 rq::ExpressionAttributeFlags attributes);
  TemplateParameterSymbol(const Self &) = delete;
  TemplateParameterSymbol(Self &&) = delete;
  ~TemplateParameterSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassParameterSymbol : public rq::Symbol,
                              public rq::detail::HasLocationSymbol,
                              public rq::detail::ModuleMemberSymbol,
                              public rq::detail::SymbolTableMemberSymbol,
                              public rq::detail::HasAttributesSymbol,
                              public rq::detail::HasBindingTypeSymbol {
  using Self = rq::ClassParameterSymbol;

  ClassParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                       rq::SymbolTableSymbol &containing_table,
                       rq::ExpressionAttributeFlags attributes);
  ClassParameterSymbol(const Self &) = delete;
  ClassParameterSymbol(Self &&) = delete;
  ~ClassParameterSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LayoutParameterSymbol : public rq::Symbol,
                               public rq::detail::HasLocationSymbol,
                               public rq::detail::ModuleMemberSymbol,
                               public rq::detail::SymbolTableMemberSymbol,
                               public rq::detail::HasAttributesSymbol,
                               public rq::detail::MaybeHasNameSymbol,
                               public rq::detail::HasBindingTypeSymbol {
  using Self = rq::LayoutParameterSymbol;

  inline LayoutParameterSymbol(rq::Expression &expression,
                               rq::ModuleSymbol &module,
                               rq::SymbolTableSymbol &containing_table,
                               rq::ExpressionAttributeFlags attributes);
  inline LayoutParameterSymbol(rq::Expression &expression,
                               rq::ModuleSymbol &module,
                               rq::SymbolTableSymbol &containing_table,
                               llvm::StringRef name,
                               rq::ExpressionAttributeFlags attributes);
  LayoutParameterSymbol(const Self &) = delete;
  LayoutParameterSymbol(Self &&) = delete;
  ~LayoutParameterSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LabelSymbol : public rq::Symbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol,
                     public rq::detail::SymbolTableMemberSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::LabelSymbol;

  rq::Expression *_statement_ptr;

  inline LabelSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                     rq::SymbolTableSymbol &containing_table,
                     llvm::StringRef name, rq::Expression &statement);
  LabelSymbol(const Self &) = delete;
  LabelSymbol(Self &&) = delete;
  ~LabelSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getStatement();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getStatement() const;
  RQ_ALWAYS_INLINE void setStatement(rq::Expression &statement);
};

struct TableSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::TableSymbol;

  inline TableSymbol(llvm::StringRef name);
  TableSymbol(const Self &) = delete;
  TableSymbol(Self &&) = delete;
  ~TableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol,
                     public rq::detail::HasAttributesSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::ClassSymbol;

  rq::BumpPtrList<rq::ClassParameterSymbol> _class_parameters;

  ClassSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
              rq::ExpressionAttributeFlags attributes);
  ClassSymbol(const Self &) = delete;
  ClassSymbol(Self &&) = delete;
  ~ClassSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumerationSymbol : public rq::SymbolTableSymbol,
                           public rq::detail::HasLocationSymbol,
                           public rq::detail::ModuleMemberSymbol,
                           public rq::detail::HasAttributesSymbol,
                           public rq::detail::HasNameSymbol {
  using Self = rq::EnumerationSymbol;

  rq::BumpPtrList<rq::EnumeratorSymbol> _enumerators;

  EnumerationSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                    rq::SymbolTableSymbol &containing_table,
                    llvm::StringRef name,
                    rq::ExpressionAttributeFlags attributes);
  EnumerationSymbol(const Self &) = delete;
  EnumerationSymbol(Self &&) = delete;
  ~EnumerationSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EntrySymbol : public rq::ProcedureSymbol {
  using Self = rq::EntrySymbol;

  EntrySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &containing_table,
              rq::ExpressionAttributeFlags attributes);
  EntrySymbol(const Self &) = delete;
  EntrySymbol(Self &&) = delete;
  ~EntrySymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FunctionSymbol : public rq::ProcedureSymbol,
                        public rq::detail::HasNameSymbol {
  using Self = rq::FunctionSymbol;

  inline FunctionSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes);
  FunctionSymbol(const Self &) = delete;
  FunctionSymbol(Self &&) = delete;
  ~FunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct MethodSymbol : public rq::ProcedureSymbol,
                      public rq::detail::HasNameSymbol {
  using Self = rq::MethodSymbol;

  inline MethodSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                      rq::SymbolTableSymbol &containing_table,
                      llvm::StringRef name,
                      rq::ExpressionAttributeFlags attributes);
  MethodSymbol(const Self &) = delete;
  MethodSymbol(Self &&) = delete;
  ~MethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionFunctionSymbol : public rq::ProcedureSymbol,
                                 public rq::detail::HasNameSymbol {
  using Self = rq::ExtensionFunctionSymbol;

  ExtensionFunctionSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &containing_table,
                          llvm::StringRef name,
                          rq::ExpressionAttributeFlags attributes);
  ExtensionFunctionSymbol(const Self &) = delete;
  ExtensionFunctionSymbol(Self &&) = delete;
  ~ExtensionFunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionMethodSymbol : public rq::ProcedureSymbol,
                               public rq::detail::HasNameSymbol {
  using Self = rq::ExtensionMethodSymbol;

  ExtensionMethodSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes);
  ExtensionMethodSymbol(const Self &) = delete;
  ExtensionMethodSymbol(Self &&) = delete;
  ~ExtensionMethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangerSymbol : public rq::ProcedureSymbol {
  using Self = rq::RangerSymbol;

  RangerSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &containing_table,
               rq::ExpressionAttributeFlags attributes);
  RangerSymbol(const Self &) = delete;
  RangerSymbol(Self &&) = delete;
  ~RangerSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateClassSymbol : public rq::TemplateSymbol,
                             public rq::detail::HasNameSymbol {
  using Self = rq::TemplateClassSymbol;

  TemplateClassSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateClassSymbol(const Self &) = delete;
  TemplateClassSymbol(Self &&) = delete;
  ~TemplateClassSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateEnumerationSymbol : public rq::TemplateSymbol,
                                   public rq::detail::HasNameSymbol {
  using Self = rq::TemplateEnumerationSymbol;

  TemplateEnumerationSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateEnumerationSymbol(const Self &) = delete;
  TemplateEnumerationSymbol(Self &&) = delete;
  ~TemplateEnumerationSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateDynamicVariableSymbol : public rq::TemplateSymbol,
                                       public rq::detail::HasNameSymbol {
  using Self = rq::TemplateDynamicVariableSymbol;

  TemplateDynamicVariableSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateDynamicVariableSymbol(const Self &) = delete;
  TemplateDynamicVariableSymbol(Self &&) = delete;
  ~TemplateDynamicVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateStaticVariableSymbol : public rq::TemplateSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::TemplateStaticVariableSymbol;

  inline TemplateStaticVariableSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateStaticVariableSymbol(const Self &) = delete;
  TemplateStaticVariableSymbol(Self &&) = delete;
  ~TemplateStaticVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateFunctionSymbol : public rq::TemplateSymbol,
                                public rq::detail::HasNameSymbol {
  using Self = rq::TemplateFunctionSymbol;

  inline TemplateFunctionSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateFunctionSymbol(const Self &) = delete;
  TemplateFunctionSymbol(Self &&) = delete;
  ~TemplateFunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateMethodSymbol : public rq::TemplateSymbol,
                              public rq::detail::HasNameSymbol {
  using Self = rq::TemplateMethodSymbol;

  inline TemplateMethodSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateMethodSymbol(const Self &) = delete;
  TemplateMethodSymbol(Self &&) = delete;
  ~TemplateMethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionFunctionSymbol : public rq::TemplateSymbol,
                                         public rq::detail::HasNameSymbol {
  using Self = rq::TemplateExtensionFunctionSymbol;

  inline TemplateExtensionFunctionSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateExtensionFunctionSymbol(const Self &) = delete;
  TemplateExtensionFunctionSymbol(Self &&) = delete;
  ~TemplateExtensionFunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionMethodSymbol : public rq::TemplateSymbol,
                                       public rq::detail::HasNameSymbol {
  using Self = rq::TemplateExtensionMethodSymbol;

  inline TemplateExtensionMethodSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters);
  TemplateExtensionMethodSymbol(const Self &) = delete;
  TemplateExtensionMethodSymbol(Self &&) = delete;
  ~TemplateExtensionMethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};
struct PartialClassSymbol : public rq::PartialSymbol,
                            public rq::detail::HasNameSymbol {
  using Self = rq::PartialClassSymbol;

  inline explicit PartialClassSymbol(llvm::StringRef name);
  PartialClassSymbol(const Self &) = delete;
  PartialClassSymbol(Self &&) = delete;
  ~PartialClassSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialEnumerationSymbol : public rq::PartialSymbol,
                                  public rq::detail::HasNameSymbol {
  using Self = rq::PartialEnumerationSymbol;

  inline explicit PartialEnumerationSymbol(llvm::StringRef name);
  PartialEnumerationSymbol(const Self &) = delete;
  PartialEnumerationSymbol(Self &&) = delete;
  ~PartialEnumerationSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialDynamicVariableSymbol : public rq::PartialSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialDynamicVariableSymbol;

  inline explicit PartialDynamicVariableSymbol(llvm::StringRef name);
  PartialDynamicVariableSymbol(const Self &) = delete;
  PartialDynamicVariableSymbol(Self &&) = delete;
  ~PartialDynamicVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialStaticVariableSymbol : public rq::PartialSymbol,
                                     public rq::detail::HasNameSymbol {
  using Self = rq::PartialStaticVariableSymbol;

  inline explicit PartialStaticVariableSymbol(llvm::StringRef name);
  PartialStaticVariableSymbol(const Self &) = delete;
  PartialStaticVariableSymbol(Self &&) = delete;
  ~PartialStaticVariableSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialFunctionSymbol : public rq::PartialSymbol,
                               public rq::detail::HasNameSymbol {
  using Self = rq::PartialFunctionSymbol;

  inline explicit PartialFunctionSymbol(llvm::StringRef name);
  PartialFunctionSymbol(const Self &) = delete;
  PartialFunctionSymbol(Self &&) = delete;
  ~PartialFunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialMethodSymbol : public rq::PartialSymbol,
                             public rq::detail::HasNameSymbol {
  using Self = rq::PartialMethodSymbol;

  inline explicit PartialMethodSymbol(llvm::StringRef name);
  PartialMethodSymbol(const Self &) = delete;
  PartialMethodSymbol(Self &&) = delete;
  ~PartialMethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionFunctionSymbol : public rq::PartialSymbol,
                                        public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionFunctionSymbol;

  inline explicit PartialExtensionFunctionSymbol(llvm::StringRef name);
  PartialExtensionFunctionSymbol(const Self &) = delete;
  PartialExtensionFunctionSymbol(Self &&) = delete;
  ~PartialExtensionFunctionSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionMethodSymbol : public rq::PartialSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionMethodSymbol;

  inline explicit PartialExtensionMethodSymbol(llvm::StringRef name);
  PartialExtensionMethodSymbol(const Self &) = delete;
  PartialExtensionMethodSymbol(Self &&) = delete;
  ~PartialExtensionMethodSymbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};
void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &value);
void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &value);
void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef value);
void RQ_ALWAYS_INLINE profileArrayConstant(
    llvm::FoldingSetNodeID &id, const rq::BumpPtrList<rq::Entity *> &elements);
struct Constant : public rq::Entity {
  using Self = rq::Constant;

  inline explicit Constant(rq::EntityKind kind);
  Constant(const Self &) = delete;
  Constant(Self &&) = delete;
  ~Constant() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct IntegerConstant final : public rq::Constant,
                               public llvm::FoldingSetNode {
  using Self = rq::IntegerConstant;

  llvm::APInt _value;

  inline explicit IntegerConstant(const llvm::APInt &value);
  IntegerConstant(const Self &) = delete;
  IntegerConstant(Self &&) = delete;
  ~IntegerConstant() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getValue() const;
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct FloatConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::FloatConstant;

  llvm::APFloat _value;

  inline FloatConstant(const llvm::APFloat &value);
  FloatConstant(const Self &) = delete;
  FloatConstant(Self &&) = delete;
  ~FloatConstant() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getValue() const;
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct StringConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::StringConstant;

  llvm::StringRef _value;

  inline explicit StringConstant(llvm::StringRef value);
  StringConstant(const Self &) = delete;
  StringConstant(Self &&) = delete;
  ~StringConstant() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getValue() const;
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct ArrayConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ArrayConstant;

  rq::BumpPtrList<rq::Entity *> _elements;

  inline explicit ArrayConstant(rq::BumpPtrList<rq::Entity *> elements);
  ArrayConstant(const Self &) = delete;
  ArrayConstant(Self &&) = delete;
  ~ArrayConstant() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Entity *>
  getElements() const;
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

struct Instruction;
struct InstructionNode;

struct InstructionSlot final {
  using Self = InstructionSlot;

  llvm::PointerUnion<rq::Entity *, rq::InstructionNode *>
      _data{};

  InstructionSlot() = default;
  inline explicit InstructionSlot(rq::Entity &entity);
  inline explicit InstructionSlot(rq::InstructionNode &node);
  inline explicit InstructionSlot(rq::Instruction &instruction);
  ~InstructionSlot() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntity() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstructionNode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getEntity();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getEntity() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionNode &getInstructionNode();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionNode &
  getInstructionNode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  inline void clear();
};

struct InstructionNode final {
  using Self = rq::InstructionNode;

  rq::InstructionSlot _car{};
  rq::InstructionSlot _cdr{};

  InstructionNode() = default;
  InstructionNode(const Self &) = delete;
  InstructionNode(Self &&) = delete;
  ~InstructionNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &getCar();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &getCar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &getCdr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &getCdr() const;
  inline void clear();
};

struct Instruction final : public rq::Entity {
  using Self = rq::Instruction;

  rq::InstructionSlot _cdr;

  inline Instruction();
  Instruction(const Self &) = delete;
  Instruction(Self &&) = delete;
  ~Instruction() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  RQ_ALWAYS_INLINE void setOpcode(rq::EntityKind opcode);
  RQ_ALWAYS_INLINE void changeOpcode(rq::EntityKind opcode);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionSlot &getCdr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InstructionSlot &getCdr() const;
  inline void clear();
};

} // namespace rq

#include <rq/detail/entity.hpp.inl>