#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/StringSaver.h>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ranges>
#include <utility>

namespace rq {

struct Context;

// NOTE: this is a massive tree of types that make use of LLVM RTTI
// The root of this tree is "Entity", and the tree contains all symbols,
// constant values, and symbolic instructions.
// https://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html

// NOTE: everything in this tree is meant to be allocated within a
// llvm::BumpPtrAllocator besides symbol table maps, which require special
// release function called on destruction.

// NOTE: all symbols and constants have unique instances. you can always test if
// two symbols/constants are exactly the same by comparing their pointers.

enum class Opcode : std::uint16_t {
  NONE = 0,

  // =====SYMBOLS=====

  // SIMPLE BUILTIN
  SY_RESULT,
  SY_OUT,
  SY_INTEGER_LITERAL,
  SY_FLOAT_LITERAL,
  SY_STRING_LITERAL,
  SY_CODEUNIT_LITERAL,
  SY_INFERENCE,
  SY_SYMBOL_REFLECTION,
  SY_TYPE,
  SY_EXPRESSION,
  SY_VOID,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_SIGNED_CONSTRAINT,
  SY_UNSIGNED_CONSTRAINT,
  SY_FLOAT_CONSTRAINT,
  SY_BINARY_CONSTRAINT,
  SY_BFLOAT_CONSTRAINT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_INTEGER_CONSTRAINT,
  SY_SIGNED_INTEGER_CONSTRAINT,
  SY_UNSIGNED_INTEGER_CONSTRAINT,
  SY_CODEUNIT_CONSTRAINT,
  SY_STRING_CONSTRAINT,
  SY_INTERPOLATED_STRING_CONSTRAINT,
  SY_CHAR,
  SY_ASCII,
  SY_UTF8,
  SY_EXPRESSION_ATTRIBUTE_CONSTRAINT,
  SY_LABELING,
  SY_VISIBILITY,
  SY_SCOPING,
  SY_AVAILABILITY,
  SY_ACCESSIBILITY,
  SY_PROPERTY_MUTABILITY,
  SY_EXPORTING,
  SY_RUNTIME,
  SY_CAPTURING,
  SY_EVALUATION_TIME,
  SY_PARENTABILITY,
  SY_PROPERTY_ASSOCIATION,
  SY_TANGIBILITY,
  SY_OVERRIDING,
  SY_INLINING,
  SY_MANGLING,
  SY_PACKING,
  SY_TEMPLATING,
  SY_LIKELYHOOD,
  SY_SUPPORT,
  SY_COPYABILITY,
  SY_ADDRESS_STABILITY,
  SY_CLEANUP,
  SY_RESULT_STATUS,
  SY_TYPE_ATTRIBUTE_CONSTRAINT,
  SY_MUTABILITY,
  SY_VOLATILITY,
  SY_DETERMINICITY,
  SY_ATOMICITY,
  SY_NULL_TERMINATION,

  // SCALED BUILTIN
  SY_SCALED_SIGNED_INTEGER,
  SY_SCALED_UNSIGNED_INTEGER,

  // UNARY SUBTYPE
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCED_COUNT_ARRAY,

  // COUNTED SUBTYPE
  SY_ARRAY,

  // PARAMETER LIST SUBTYPE
  SY_LAYOUT,
  SY_CLASS_LAYOUT,
  SY_TEMPLATE_LAYOUT,
  SY_SIGNATURE,

  // PARAMETER
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,

  // ARITHMETIC SEQUENCE
  SY_ARITHMETIC_INTERVAL,
  SY_FINITE_ARITHMETIC_PROGRESSION,
  SY_INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  SY_MODULE,
  SY_IMPORT,
  SY_CODE,
  SY_CATEGORY_DISCRIMINANT, // underlying type of category code
  SY_LABEL,
  SY_SYNONYM,
  SY_LOCAL,
  SY_STATIC,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE, // entry within a category referencing a code and
                           // maybe a value type

  // SYMBOL TABLE
  SY_TOP,
  SY_GLOBAL,
  SY_GLOBAL_STATIC,
  SY_SCOPE,
  SY_NAMESPACE,
  SY_CLASS,
  SY_ENUMERATION,
  SY_CATEGORY,

  // POLYMORPHS
  SY_RANGER_POLYMORPH,
  SY_PROCEDURE_POLYMORPH,
  SY_CLASS_POLYMORPH,
  SY_ENUMERATION_POLYMORPH,
  SY_CATEGORY_POLYMORPH,
  SY_GLOBAL_POLYMORPH,
  SY_GLOBAL_STATIC_POLYMORPH,

  // RANGERS
  SY_FORWARD_RANGER,
  SY_BACKWARD_RANGER,

  // PROCEDURE
  SY_ENTRY,
  SY_FUNCTION,
  SY_METHOD,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,

  // TEMPLATE
  SY_TEMPLATE_CLASS,
  SY_TEMPLATE_ENUMERATION,
  SY_TEMPLATE_CATEGORY,
  SY_TEMPLATE_GLOBAL,
  SY_TEMPLATE_GLOBAL_STATIC,
  SY_TEMPLATE_FORWARD_RANGER,
  SY_TEMPLATE_BACKWARD_RANGER,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,

  // SPECIALIZED
  SY_SPECIALIZED_CLASS,
  SY_SPECIALIZED_ENUMERATION,
  SY_SPECIALIZED_CATEGORY,
  SY_SPECIALIZED_GLOBAL,
  SY_SPECIALIZED_GLOBAL_STATIC,
  SY_SPECIALIZED_FORWARD_RANGER,
  SY_SPECIALIZED_BACKWARD_RANGER,
  SY_SPECIALIZED_FUNCTION,
  SY_SPECIALIZED_METHOD,
  SY_SPECIALIZED_EXTENSION_FUNCTION,
  SY_SPECIALIZED_EXTENSION_METHOD,

  // =====CONSTANTS=====

  CT_TYPE,
  CT_EXPRESSION,
  CT_BOOLEAN,
  CT_INTEGER,
  CT_FLOAT,
  CT_STRING,
  CT_ARRAY,
  CT_EXPRESSION_ATTRIBUTE,
  CT_TYPE_ATTRIBUTE,

  // =====INSTRUCTIONS=====

  // none | 0:statement 1:statement
  IN_EXECUTE,
  // 0 | 0:type 1:value
  IN_CAST,

  // boolean | 0:boolean 1:boolean
  IN_LOGICAL_AND,
  // boolean | 0:boolean 1:boolean
  IN_LOGICAL_OR,
  // boolean | 0:boolean
  IN_LOGICAL_COMPLEMENT,

  // boolean | 0:value 1:value
  IN_EQ,
  // boolean | 0:value 1:value
  IN_NE,
  // boolean | 0:numeric 1:numeric
  IN_GT,
  // boolean | 0:numeric 1:numeric
  IN_GE,
  // boolean | 0:numeric 1:numeric
  IN_LT,
  // boolean | 0:numeric 1:numeric
  IN_LE,

  // int | 0:int 1:int
  IN_AND,
  // int | 0:int 1:int
  IN_OR,
  // int | 0:int 1:int
  IN_XOR,
  // int | 0:int
  IN_COMPLEMENT,
  // int | 0:int 1:int
  IN_SHIFT_LEFT,
  // int | 0:int 1:int
  IN_SHIFT_RIGHT,
  // int | 0:int 1:int
  IN_ROTATE_LEFT,
  // int | 0:int 1:int
  IN_ROTATE_RIGHT,

  // numeric | 0:numeric 1:numeric
  IN_ADD,
  // numeric | 0:numeric 1:numeric
  IN_SUBTRACT,
  // numeric | 0:numeric 1:numeric
  IN_MULTIPLY,
  // numeric | 0:numeric 1:numeric
  IN_DIVIDE,
  // numeric | 0:numeric 1:numeric
  IN_MODULUS,
  // numeric | 0:numeric
  IN_NEGATE,

  // value | 0:pointer
  IN_CONTENT,
  // pointer | 0:value
  IN_ADDRESS,
  // value | 0:location
  IN_VALUE,
  // value | 0:location
  IN_REF,
  // value | 0:location 1:index
  IN_INDEX,
  // value | 0:value
  IN_DATA_COUNT,
  // none | 0:value 1:next arg (or last value)
  IN_ARG,
  // none | 0:procedure 1:arg0
  IN_CALL,
  // none | 0:location 1:location
  IN_MOVE,
  // none | 0:location 1:value
  IN_COPY,

  // none |
  IN_RET,
  // none | 0:boolean 1:fork
  IN_COND,
  // none | 0:if 1:else
  IN_FORK,

  // none |
  IN_DEBUG_BREAK,
  // none |
  IN_UNREACHABLE,
  // none | 0:boolean
  IN_ASSUME,

  LAST
};

static constexpr std::size_t OPCODE_COUNT =
    static_cast<std::size_t>(rq::Opcode::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode);

enum class OpcodeFlags : std::uint32_t {
  NONE = 0,

  // CATEGORIES
  // the first two bits used to determine category
  // the rest of the bits mean different things depending on the
  // category.
  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  NOT_INSTRUCTION_MASK = (rq::getBit(0) | rq::getBit(1)),

  // SYMBOL FLAGS
  // these flags are valid only when working with symbols
  // SYMBOL INHERITING PROPERTIES - are implmented via inherited types that can
  // be used themselves. every symbol can have at most one of these.
  SY_SIMPLE_BUILTIN = rq::getBit(2),
  SY_SCALED_BUILTIN = rq::getBit(3),
  SY_UNARY_SUBTYPE = rq::getBit(4),
  SY_COUNTED_SUBTYPE = rq::getBit(5),
  SY_PARAMETER = rq::getBit(6),
  SY_PARAMETER_LIST_SUBTYPE = rq::getBit(7),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(8),
  SY_TABLE = rq::getBit(9),
  SY_POLYMORPH = rq::getBit(10),
  SY_RANGER = rq::getBit(11),
  SY_PROCEDURE = rq::getBit(12),
  SY_TEMPLATE = rq::getBit(13),
  SY_SPECIALIZED = rq::getBit(14),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(15),
  SY_TYPE = rq::getBit(16),
  SY_SUBTYPE = rq::getBit(17),
  SY_CONSTRAINT = rq::getBit(18),
  SY_EXPRESSION_ATTRIBUTE = rq::getBit(19),
  SY_TYPE_ATTRIBUTE = rq::getBit(20),
  SY_LITERAL = rq::getBit(21),
  SY_PLATFORM_CHANGING = rq::getBit(22),
  SY_INTEGER = rq::getBit(23),
  SY_FLOAT = rq::getBit(24),
  SY_BINARY = rq::getBit(25),
  SY_CODEUNIT = rq::getBit(26),
  SY_SIGNED = rq::getBit(25),
  SY_UNSIGNED = rq::getBit(27),
  SY_NO_ASCENDING = rq::getBit(28),
  SY_NO_ASCENDING_INTO_TOP = rq::getBit(29),

  // CONSTANT FLAGS
  // TODO

  // INSTRUCTION FLAGS
  IN_NULLARY = rq::getBit(2),
  IN_UNARY = rq::getBit(3),
  IN_BINARY = rq::getBit(4),
};
template <> struct is_flags<OpcodeFlags> : std::true_type {};
[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode);

#define RQ_ASSERT_SYMBOL(opcode)                                               \
  RQ_ASSERT(rq::getIsSymbol((opcode)), "not symbol")
#define RQ_ASSERT_CONSTANT(opcode)                                             \
  RQ_ASSERT(rq::getIsConstant((opcode)), "not constant")

#define RQ_ASSERT_INSTRUCTION(opcode)                                          \
  RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction")

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterListSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTable(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateRanger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateProcedure(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecialized(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecializedRanger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSpecializedProcedure(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionAttribute(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute(rq::Opcode opcode);
// NOTE: SCALED_SIGNED_INTEGER and SCALED_UNSIGNED_INTEGER is platform changing
// only if depth is not exact that is checked in member function of Entity, not
// here.
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscending(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscendingIntoTop(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullaryInstruction(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnaryInstruction(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryInstruction(rq::Opcode opcode);

struct Entity;
struct Symbol;
struct SimpleBuiltin;
struct Result;
struct Out;
struct IntegerLiteral;
struct FloatLiteral;
struct StringLiteral;
struct CodeunitLiteral;
struct Inference;
struct SymbolReflection;
struct Type;
struct Void;
struct NoReturn;
struct VariadicArguments;
struct Boolean;
struct SignedConstraint;
struct UnsignedConstraint;
struct FloatConstraint;
struct BinaryConstraint;
struct BfloatConstraint;
struct Half;
struct Single;
struct Double;
struct Quadruple;
struct Binary16;
struct Binary32;
struct Binary64;
struct Binary128;
struct Bfloat16;
struct IntegerConstraint;
struct SignedIntegerConstraint;
struct UnsignedIntegerConstraint;
struct CodeunitConstraint;
struct StringConstraint;
struct InterpolatedStringConstraint;
struct Char;
struct Ascii;
struct Utf8;
struct ExpressionAttributeConstraint;
struct Labeling;
struct Visibility;
struct Scoping;
struct Availability;
struct Accessibility;
struct PropertyMutability;
struct Exporting;
struct Runtime;
struct Capturing;
struct EvaluationTime;
struct Parentability;
struct PropertyAssociation;
struct Tangibility;
struct Overriding;
struct Inlining;
struct Mangling;
struct Packing;
struct Templating;
struct Likelyhood;
struct Support;
struct Copyability;
struct Cleanup;
struct ResultStatus;
struct TypeAttributeConstraint;
struct Mutability;
struct Volatility;
struct Determinicity;
struct Atomicity;
struct NullTermination;
struct ScaledBuiltin;
struct ScaledSignedInteger;
struct ScaledUnsignedInteger;
struct UnarySubtype;
struct Reference;
struct Pointer;
struct FatPointer;
struct InferencedCountArray;
struct CountedSubtype;
struct Array;
struct Parameter;
struct ClassParameter;
struct LayoutParameter;
struct TemplateParameter;
struct SignatureParameter;
struct ParameterListSubtype;
struct Layout;
struct ClassLayout;
struct TemplateLayout;
struct Signature;
struct ArithmeticSequence;
struct ArithmeticInterval;
struct FiniteArithmeticProgression;
struct InfiniteArithmeticProgression;
struct Module;
struct Import;
struct Code;
struct CategoryDiscriminant;
struct Label;
struct Synonym;
struct Local;
struct Static;
struct Enumerator;
struct CategoryAlternative;
struct Table;
struct Top;
struct Global;
struct GlobalStatic;
struct Scope;
struct Namespace;
struct Class;
struct Enumeration;
struct Category;
struct Polymorph;
struct RangerPolymorph;
struct ProcedurePolymorph;
struct ClassPolymorph;
struct EnumerationPolymorph;
struct CategoryPolymorph;
struct GlobalPolymorph;
struct GlobalStaticPolymorph;
struct Ranger;
struct ForwardRanger;
struct BackwardRanger;
struct Procedure;
struct Entry;
struct Function;
struct Method;
struct ExtensionFunction;
struct ExtensionMethod;
struct ExtensionRanger;
struct Template;
struct TemplateClass;
struct TemplateEnumeration;
struct TemplateCategory;
struct TemplateGlobal;
struct TemplateGlobalStatic;
struct TemplateRanger;
struct TemplateForwardRanger;
struct TemplateBackwardRanger;
struct TemplateProcedure;
struct TemplateFunction;
struct TemplateMethod;
struct TemplateExtensionFunction;
struct TemplateExtensionMethod;
struct Specialized;
struct SpecializedClass;
struct SpecializedEnumeration;
struct SpecializedCategory;
struct SpecializedGlobal;
struct SpecializedGlobalStatic;
struct SpecializedRanger;
struct SpecializedForwardRanger;
struct SpecializedBackwardRanger;
struct SpecializedProcedure;
struct SpecializedFunction;
struct SpecializedMethod;
struct SpecializedExtensionFunction;
struct SpecializedExtensionMethod;
struct Constant;
struct TypeConstant;
struct ExpressionConstant;
struct BooleanConstant;
struct IntegerConstant;
struct FloatConstant;
struct StringConstant;
struct ArrayConstant;
struct ExpressionAttributeConstant;
struct TypeAttributeConstant;
struct Instruction;
struct UnaryInstruction;
struct BinaryInstruction;
struct TernaryInstruction;

struct InitialExpression {
  using Self = rq::InitialExpression;

  const rq::Expression *_expression_ptr{nullptr};

  InitialExpression() = default;
  inline explicit InitialExpression(const rq::Expression &expression);
  InitialExpression(const Self &) = delete;
  InitialExpression(Self &&) = delete;
  ~InitialExpression() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
};

struct LateExpression : public rq::InitialExpression {
  using Self = rq::LateExpression;

  LateExpression() = default;
  inline explicit LateExpression(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression();
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression *expression_ptr);
};
struct ReplacableExpression : public rq::LateExpression {
  using Self = rq::ReplacableExpression;

  ReplacableExpression() = default;
  inline explicit ReplacableExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE const rq::Expression &
  replaceExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE const rq::Expression &
  replaceExpression(const rq::Expression *expression);
  RQ_ALWAYS_INLINE const rq::Expression *
  replaceExpressionPtr(const rq::Expression &expression);
  RQ_ALWAYS_INLINE const rq::Expression *
  replaceExpressionPtr(const rq::Expression *expression);
};
struct InitialExpressionFlags {
  using Self = InitialExpressionFlags;

  rq::ExpressionFlags _expression_flags;

  inline explicit InitialExpressionFlags(rq::ExpressionFlags flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTransparent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInsideScope() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOutsideScope() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPrivate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPublic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasProtected() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoPartialMutate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPartialMutate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoExport() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExport() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDynamic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoCapture() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapture() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLazy() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEager() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoParent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayParent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasParent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMixin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTangible() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAbstract() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVirtual() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverride() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoInline() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInline() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasImplicitMangle() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExplicitMangle() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoPack() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPack() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLabel() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoTemplate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSpecialize() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEquivocal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikely() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnlikely() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSupported() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDepreciated() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExperimental() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoCopy() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayCopy() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnstableAddress() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStableAddress() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExplicitDrop() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasImplicitDrop() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNotOk() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOk() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::ExpressionAttribute attribute) const;
};
struct InitialModuleMember {
  using Self = rq::InitialModuleMember;

  rq::Module *_containing_module_ptr;

  inline explicit InitialModuleMember(rq::Module &module);
  InitialModuleMember(const Self &) = delete;
  InitialModuleMember(Self &&) = delete;
  ~InitialModuleMember() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getContainingModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getContainingModule();
};
struct TableMember {
  using Self = rq::TableMember;

  rq::Table *_containing_table_ptr{nullptr};

  inline explicit TableMember() = default;
  inline explicit TableMember(rq::Table &containing_table);
  TableMember(const Self &) = delete;
  TableMember(Self &&) = delete;
  ~TableMember() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getContainingTable();
};
struct TableHosted {
  using Self = rq::TableHosted;

  rq::Table *_hosting_table_ptr{nullptr};

  inline explicit TableHosted(rq::Table &hosting_table);
  TableHosted(const Self &) = delete;
  TableHosted(Self &&) = delete;
  ~TableHosted() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getHostingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getHostingTable();
};
struct InitialNamed {
  using Self = rq::InitialNamed;

  llvm::StringRef _name;

  inline InitialNamed(llvm::StringRef name);
  InitialNamed(const Self &) = delete;
  InitialNamed(Self &&) = delete;
  ~InitialNamed() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
};
struct InitialMaybeNamed {
  using Self = rq::InitialMaybeNamed;

  llvm::StringRef _name{};

  InitialMaybeNamed() = default;
  inline explicit InitialMaybeNamed(llvm::StringRef name);
  InitialMaybeNamed(const Self &) = delete;
  InitialMaybeNamed(Self &&) = delete;
  ~InitialMaybeNamed() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
};
struct Entity {
  using Self = rq::Entity;

  rq::Opcode _opcode;

  inline explicit Entity(rq::Opcode opcode);
  Entity() = delete;
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  Entity &operator=(const Entity &) = delete;
  Entity &operator=(Entity &&) = delete;
  ~Entity() = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Entity &other) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Entity &other) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Entity> final : std::true_type {};

struct DeclarationInfo {
  using Self = rq::DeclarationInfo;

  rq::Opcode _opcode;
  llvm::StringRef _name{};
  const rq::Expression *_expression_ptr{nullptr};
  rq::Table *_containing_table_ptr{nullptr};
  rq::Module *_containing_module_ptr{nullptr};

  RQ_ALWAYS_INLINE explicit DeclarationInfo(rq::Opcode opcode);
  explicit DeclarationInfo(const Self &) = default;
  explicit DeclarationInfo(Self &&) = default;
  ~DeclarationInfo() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode();
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getOpcodeName();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const;
  RQ_ALWAYS_INLINE void setName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingTable() const;
  RQ_ALWAYS_INLINE void setContainingTable(rq::Table &table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &getContainingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingModule() const;
  RQ_ALWAYS_INLINE void setContainingModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getContainingModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getContainingModule();
};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  inline explicit Symbol(rq::Opcode opcode);
  [[nodiscard]] inline rq::DeclarationInfo getDeclarationInfo();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterListSubtype() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionAttribute() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscending() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoAscendingIntoTop() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Symbol> final : std::true_type {};

struct SimpleBuiltin : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit SimpleBuiltin(rq::Opcode opcode);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::SimpleBuiltin> final : std::true_type {};

struct Result : public rq::SimpleBuiltin {
  using Self = rq::Result;

  inline explicit Result();
  [[nodiscard]] inline bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Result> final : std::true_type {};

struct Out : public rq::SimpleBuiltin {
  using Self = rq::Out;

  inline explicit Out();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Out> final : std::true_type {};

struct IntegerLiteral : public rq::SimpleBuiltin {
  using Self = rq::IntegerLiteral;

  inline explicit IntegerLiteral();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::IntegerLiteral> final : std::true_type {};

struct FloatLiteral : public rq::SimpleBuiltin {
  using Self = rq::FloatLiteral;

  inline explicit FloatLiteral();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::FloatLiteral> final : std::true_type {};

struct StringLiteral : public rq::SimpleBuiltin {
  using Self = rq::StringLiteral;

  inline explicit StringLiteral();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::StringLiteral> final : std::true_type {};

struct CodeunitLiteral : public rq::SimpleBuiltin {
  using Self = rq::CodeunitLiteral;

  inline explicit CodeunitLiteral();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::CodeunitLiteral> final : std::true_type {};

struct Inference final : public rq::SimpleBuiltin {
  using Self = rq::Inference;

  inline explicit Inference();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Inference> final : std::true_type {};

struct SymbolReflection final : public rq::SimpleBuiltin {
  using Self = rq::SymbolReflection;

  inline explicit SymbolReflection();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::SymbolReflection> final : std::true_type {};

struct Type final : public rq::SimpleBuiltin {
  using Self = rq::Type;

  inline explicit Type();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Type> final : std::true_type {};

struct Void final : public rq::SimpleBuiltin {
  using Self = rq::Void;

  inline explicit Void();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Void> final : std::true_type {};

struct NoReturn final : public rq::SimpleBuiltin {
  using Self = rq::NoReturn;

  inline explicit NoReturn();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::NoReturn> final : std::true_type {};

struct VariadicArguments final : public rq::SimpleBuiltin {
  using Self = rq::VariadicArguments;

  inline explicit VariadicArguments();
  [[nodiscard]] inline bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::VariadicArguments> final : std::true_type {};

struct Boolean final : public rq::SimpleBuiltin {
  using Self = rq::Boolean;

  inline explicit Boolean();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Boolean> final : std::true_type {};

struct SignedConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SignedConstraint;

  inline explicit SignedConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::SignedConstraint> final : std::true_type {};

struct UnsignedConstraint final : public rq::SimpleBuiltin {
  using Self = rq::UnsignedConstraint;

  inline explicit UnsignedConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::UnsignedConstraint> final : std::true_type {};

struct FloatConstraint final : public rq::SimpleBuiltin {
  using Self = rq::FloatConstraint;

  inline explicit FloatConstraint();
  [[nodiscard]] inline bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::FloatConstraint> final : std::true_type {};

struct BinaryConstraint final : public rq::SimpleBuiltin {
  using Self = rq::BinaryConstraint;

  inline explicit BinaryConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::BinaryConstraint> final : std::true_type {};

struct BfloatConstraint final : public rq::SimpleBuiltin {
  using Self = rq::BfloatConstraint;

  inline explicit BfloatConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::BfloatConstraint> final : std::true_type {};

struct Half final : public rq::SimpleBuiltin {
  using Self = rq::Half;

  inline explicit Half();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Half> final : std::true_type {};

struct Single final : public rq::SimpleBuiltin {
  using Self = rq::Single;

  inline explicit Single();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Single> final : std::true_type {};

struct Double final : public rq::SimpleBuiltin {
  using Self = rq::Double;

  inline explicit Double();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Double> final : std::true_type {};

struct Quadruple final : public rq::SimpleBuiltin {
  using Self = rq::Quadruple;

  inline explicit Quadruple();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Quadruple> final : std::true_type {};

struct Binary16 final : public rq::SimpleBuiltin {
  using Self = rq::Binary16;

  inline explicit Binary16();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Binary16> final : std::true_type {};

struct Binary32 final : public rq::SimpleBuiltin {
  using Self = rq::Binary32;

  inline explicit Binary32();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Binary32> final : std::true_type {};

struct Binary64 final : public rq::SimpleBuiltin {
  using Self = rq::Binary64;

  inline explicit Binary64();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Binary64> final : std::true_type {};

struct Binary128 final : public rq::SimpleBuiltin {
  using Self = rq::Binary128;

  inline explicit Binary128();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Binary128> final : std::true_type {};

struct Bfloat16 final : public rq::SimpleBuiltin {
  using Self = rq::Bfloat16;

  inline explicit Bfloat16();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Bfloat16> final : std::true_type {};

struct IntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::IntegerConstraint;

  inline explicit IntegerConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::IntegerConstraint> final : std::true_type {};

struct SignedIntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SignedIntegerConstraint;

  inline explicit SignedIntegerConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::SignedIntegerConstraint> final : std::true_type {};

struct UnsignedIntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::UnsignedIntegerConstraint;

  inline explicit UnsignedIntegerConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::UnsignedIntegerConstraint> final : std::true_type {};

struct CodeunitConstraint final : public rq::SimpleBuiltin {
  using Self = rq::CodeunitConstraint;

  inline explicit CodeunitConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::CodeunitConstraint> final : std::true_type {};

struct StringConstraint final : public rq::SimpleBuiltin {
  using Self = rq::StringConstraint;

  inline explicit StringConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::StringConstraint> final : std::true_type {};

struct InterpolatedStringConstraint final : public rq::SimpleBuiltin {
  using Self = rq::InterpolatedStringConstraint;

  inline explicit InterpolatedStringConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::InterpolatedStringConstraint> final : std::true_type {};

struct Char final : public rq::SimpleBuiltin {
  using Self = rq::Char;

  inline explicit Char();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Char> final : std::true_type {};

struct Ascii final : public rq::SimpleBuiltin {
  using Self = rq::Ascii;

  inline explicit Ascii();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Ascii> final : std::true_type {};

struct Utf8 final : public rq::SimpleBuiltin {
  using Self = rq::Utf8;

  inline explicit Utf8();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Utf8> final : std::true_type {};

struct ExpressionAttributeConstraint final : public rq::SimpleBuiltin {
  using Self = rq::ExpressionAttributeConstraint;

  inline explicit ExpressionAttributeConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::ExpressionAttributeConstraint> final : std::true_type {};

struct Labeling final : public rq::SimpleBuiltin {
  using Self = rq::Labeling;

  inline explicit Labeling();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Labeling> final : std::true_type {};

struct Visibility final : public rq::SimpleBuiltin {
  using Self = rq::Visibility;

  inline explicit Visibility();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Visibility> final : std::true_type {};

struct Scoping final : public rq::SimpleBuiltin {
  using Self = rq::Scoping;

  inline explicit Scoping();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Scoping> final : std::true_type {};

struct Availability final : public rq::SimpleBuiltin {
  using Self = rq::Availability;

  inline explicit Availability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Availability> final : std::true_type {};

struct Accessibility final : public rq::SimpleBuiltin {
  using Self = rq::Accessibility;

  inline explicit Accessibility();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Accessibility> final : std::true_type {};

struct PropertyMutability final : public rq::SimpleBuiltin {
  using Self = rq::PropertyMutability;

  inline explicit PropertyMutability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::PropertyMutability> final : std::true_type {};

struct Exporting final : public rq::SimpleBuiltin {
  using Self = rq::Exporting;

  inline explicit Exporting();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Exporting> final : std::true_type {};

struct Runtime final : public rq::SimpleBuiltin {
  using Self = rq::Runtime;

  inline explicit Runtime();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Runtime> final : std::true_type {};

struct Capturing final : public rq::SimpleBuiltin {
  using Self = rq::Capturing;

  inline explicit Capturing();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Capturing> final : std::true_type {};

struct EvaluationTime final : public rq::SimpleBuiltin {
  using Self = rq::EvaluationTime;

  inline explicit EvaluationTime();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::EvaluationTime> final : std::true_type {};

struct Parentability final : public rq::SimpleBuiltin {
  using Self = rq::Parentability;

  inline explicit Parentability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Parentability> final : std::true_type {};

struct PropertyAssociation final : public rq::SimpleBuiltin {
  using Self = rq::PropertyAssociation;

  inline explicit PropertyAssociation();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::PropertyAssociation> final : std::true_type {};

struct Tangibility final : public rq::SimpleBuiltin {
  using Self = rq::Tangibility;

  inline explicit Tangibility();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Tangibility> final : std::true_type {};

struct Overriding final : public rq::SimpleBuiltin {
  using Self = rq::Overriding;

  inline explicit Overriding();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Overriding> final : std::true_type {};

struct Inlining final : public rq::SimpleBuiltin {
  using Self = rq::Inlining;

  inline explicit Inlining();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Inlining> final : std::true_type {};

struct Mangling final : public rq::SimpleBuiltin {
  using Self = rq::Mangling;

  inline explicit Mangling();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Mangling> final : std::true_type {};

struct Packing final : public rq::SimpleBuiltin {
  using Self = rq::Packing;

  inline explicit Packing();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Packing> final : std::true_type {};

struct Templating final : public rq::SimpleBuiltin {
  using Self = rq::Templating;

  inline explicit Templating();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Templating> final : std::true_type {};

struct Likelyhood final : public rq::SimpleBuiltin {
  using Self = rq::Likelyhood;

  inline explicit Likelyhood();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Likelyhood> final : std::true_type {};

struct Support final : public rq::SimpleBuiltin {
  using Self = rq::Support;

  inline explicit Support();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Support> final : std::true_type {};

struct Copyability final : public rq::SimpleBuiltin {
  using Self = rq::Copyability;

  inline explicit Copyability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Copyability> final : std::true_type {};

struct AddressStability final : public rq::SimpleBuiltin {
  using Self = rq::AddressStability;

  inline explicit AddressStability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::AddressStability> final : std::true_type {};

struct Cleanup final : public rq::SimpleBuiltin {
  using Self = rq::Cleanup;

  inline explicit Cleanup();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Cleanup> final : std::true_type {};

struct ResultStatus final : public rq::SimpleBuiltin {
  using Self = rq::ResultStatus;

  inline explicit ResultStatus();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::ResultStatus> final : std::true_type {};

struct TypeAttributeConstraint final : public rq::SimpleBuiltin {
  using Self = rq::TypeAttributeConstraint;

  inline explicit TypeAttributeConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::TypeAttributeConstraint> final : std::true_type {};

struct Mutability final : public rq::SimpleBuiltin {
  using Self = rq::Mutability;

  inline explicit Mutability();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Mutability> final : std::true_type {};

struct Volatility final : public rq::SimpleBuiltin {
  using Self = rq::Volatility;

  inline explicit Volatility();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Volatility> final : std::true_type {};

struct Determinicity final : public rq::SimpleBuiltin {
  using Self = rq::Determinicity;

  inline explicit Determinicity();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Determinicity> final : std::true_type {};

struct Atomicity final : public rq::SimpleBuiltin {
  using Self = rq::Atomicity;

  inline explicit Atomicity();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Atomicity> final : std::true_type {};

struct NullTermination final : public rq::SimpleBuiltin {
  using Self = rq::NullTermination;

  inline explicit NullTermination();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::NullTermination> final : std::true_type {};

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
getHasPlatformScalar(rq::ScaledBuiltinFlags flags);
void RQ_ALWAYS_INLINE profileScaledBuiltin(llvm::FoldingSetNodeID &id,
                                           rq::Opcode opcode,
                                           std::uint16_t scalar,
                                           std::uint16_t uid,
                                           rq::ScaledBuiltinFlags flags);
struct ScaledBuiltin : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledBuiltin;

  std::uint16_t _scalar;
  std::uint16_t _uid;
  rq::ScaledBuiltinFlags _flags;

  inline explicit ScaledBuiltin(rq::Opcode opcode, std::uint16_t scalar,
                                std::uint16_t uid,
                                rq::ScaledBuiltinFlags flags);
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getUid() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinFlags getFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBytesScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndexScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSynonym() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::ScaledBuiltin> final : std::true_type {};

struct ScaledSignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledSignedInteger;

  inline explicit ScaledSignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                      rq::ScaledBuiltinFlags flags);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::ScaledSignedInteger> final : std::true_type {};

struct ScaledUnsignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledUnsignedInteger;

  inline explicit ScaledUnsignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                        rq::ScaledBuiltinFlags flags);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::ScaledUnsignedInteger> final : std::true_type {};

void RQ_ALWAYS_INLINE profileUnarySubtype(llvm::FoldingSetNodeID &id,
                                          rq::Opcode opcode,
                                          const rq::TypeConstant &descendent);

struct UnarySubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtype;

  rq::TypeConstant *_descendent_ptr;

  inline explicit UnarySubtype(rq::Opcode opcode, rq::TypeConstant &descendent);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent();
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::UnarySubtype> final : std::true_type {};

struct Reference final : public rq::UnarySubtype {
  using Self = rq::Reference;

  inline explicit Reference(rq::TypeConstant &descendent);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Reference> final : std::true_type {};

struct Pointer final : public rq::UnarySubtype {
  using Self = rq::Pointer;

  inline explicit Pointer(rq::TypeConstant &descendent);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Pointer> final : std::true_type {};

struct FatPointer final : public rq::UnarySubtype {
  using Self = rq::FatPointer;

  inline explicit FatPointer(rq::TypeConstant &descendent);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::FatPointer> final : std::true_type {};

struct InferencedCountArray final : public rq::UnarySubtype {
  using Self = rq::InferencedCountArray;

  inline explicit InferencedCountArray(rq::TypeConstant &descendent);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::InferencedCountArray> final : std::true_type {};

void RQ_ALWAYS_INLINE profileCountedSubtype(llvm::FoldingSetNodeID &id,
                                            rq::Opcode opcode,
                                            const rq::TypeConstant &descendent,
                                            unsigned count);

struct CountedSubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::CountedSubtype;

  rq::TypeConstant *_descendent_ptr;
  unsigned _count;

  inline explicit CountedSubtype(rq::Opcode opcode,
                                 rq::TypeConstant &descendent, unsigned count);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getCount() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::CountedSubtype> final : std::true_type {};

struct Array final : public rq::CountedSubtype {
  using Self = rq::Array;

  inline explicit Array(rq::TypeConstant &descendent, unsigned count);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Array> final : std::true_type {};

struct ParameterListSubtype : public rq::Symbol,
                              public rq::InitialExpressionFlags,
                              public rq::InitialModuleMember,
                              public rq::TableHosted {
  using Self = rq::ParameterListSubtype;

  llvm::ArrayRef<rq::Parameter> _named_parameter_map{};
  rq::Parameter *_positional_parameter_list_begin_ptr{nullptr};
  unsigned _parameter_count{};

  inline explicit ParameterListSubtype(rq::BumpPtrAllocator &allocator,
                                       unsigned map_bucket_count,
                                       rq::Opcode opcode,
                                       rq::ExpressionFlags attributes,
                                       rq::Module &module,
                                       rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_parent_only<rq::ParameterListSubtype> final : std::true_type {};

struct Layout final : public rq::ParameterListSubtype {
  using Self = rq::Layout;

  inline explicit Layout(rq::BumpPtrAllocator &allocator,
                         unsigned parameter_bucket_count,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Layout> final : std::true_type {};

struct ClassLayout final : public rq::ParameterListSubtype {
  using Self = rq::ClassLayout;

  inline explicit ClassLayout(rq::BumpPtrAllocator &allocator,
                              unsigned parameter_bucket_count,
                              rq::ExpressionFlags attributes,
                              rq::Module &module, rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::ClassLayout> final : std::true_type {};

struct TemplateLayout final : public rq::ParameterListSubtype {
  using Self = rq::TemplateLayout;

  inline explicit TemplateLayout(rq::BumpPtrAllocator &allocator,
                                 unsigned parameter_bucket_count,
                                 rq::Module &module, rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::TemplateLayout> final : std::true_type {};

struct Signature final : public rq::ParameterListSubtype {
  using Self = rq::Signature;

  rq::TypeConstant *_return_type_ptr{nullptr};
  rq::TypeConstant *_extended_type_ptr{nullptr};

  inline explicit Signature(rq::BumpPtrAllocator &allocator,
                            unsigned parameter_bucket_count,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table);
  RQ_ALWAYS_INLINE void setReturnType(rq::TypeConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getReturnType();
  RQ_ALWAYS_INLINE void setExtendedType(rq::TypeConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExtendedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
  getExtendedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getExtendedType();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Parameter : public rq::Symbol,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::TableHosted,
                   public rq::InitialMaybeNamed {
  using Self = rq::Parameter;

  std::size_t _hash{0};
  rq::TypeConstant *_type_ptr{nullptr};
  rq::ParameterListSubtype *_parameter_list_subtype_ptr;
  Self *_left_ptr{nullptr};
  Self *_right_ptr{nullptr};
  Self *_next_ptr{nullptr};

  inline explicit Parameter(rq::Opcode opcode, llvm::StringRef name,
                            rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table);
  inline explicit Parameter(rq::Opcode opcode, rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  RQ_ALWAYS_INLINE void setType(rq::TypeConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ParameterListSubtype &
  getParameterListSubtype() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterListSubtype &
  getParameterListSubtype();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Parameter> final : std::true_type {};

// NOTE: rq::Parameter is not is_parent_only so it can be allocated for
// rq::ParameterListSubtype map. as a consequence of this, no child types of
// rq::Parameter can have properties beyond what is inherited from Parameter.
// also, all child types must only inherit rq::Parameter and nothing else.

struct ClassParameter : public rq::Parameter {
  using Self = rq::ClassParameter;

  inline explicit ClassParameter(llvm::StringRef name, rq::ClassLayout &layout,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module, rq::Table &hosting_table);
  inline explicit ClassParameter(rq::ClassLayout &layout,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module, rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassLayout &getClassLayout() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassLayout &getClassLayout();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::ClassParameter> final : std::true_type {};

struct LayoutParameter : public rq::Parameter {
  using Self = rq::LayoutParameter;

  inline explicit LayoutParameter(llvm::StringRef name, rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module, rq::Table &hosting_table);
  inline explicit LayoutParameter(rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module, rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getLayout() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getLayout();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::LayoutParameter> final : std::true_type {};

struct TemplateParameter : public rq::Parameter {
  using Self = rq::TemplateParameter;

  inline explicit TemplateParameter(llvm::StringRef name,
                                    rq::TemplateLayout &template_layout,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &hosting_table);
  inline explicit TemplateParameter(rq::TemplateLayout &template_layout,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &getTemplate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template &getTemplate();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::TemplateParameter> final : std::true_type {};

struct SignatureParameter : public rq::Parameter {
  using Self = rq::SignatureParameter;

  inline explicit SignatureParameter(llvm::StringRef name,
                                     rq::Signature &signature,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::Table &hosting_table);
  inline explicit SignatureParameter(rq::Signature &signature,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &getSignature() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature &getSignature();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::SignatureParameter> final : std::true_type {};

void RQ_ALWAYS_INLINE profileArithmeticSequence(
    llvm::FoldingSetNodeID &id, const rq::TypeConstant &descendent,
    rq::ArithmeticSequenceCondition condition, rq::ArithmeticSequenceStep step);

struct ArithmeticSequence : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequence;

  rq::TypeConstant *_descendent_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  inline explicit ArithmeticSequence(rq::Opcode opcode,
                                     rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition,
                                     rq::ArithmeticSequenceStep step);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <>
struct is_acquired<rq::ArithmeticSequence> final : std::true_type {};

struct ArithmeticInterval : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;

  inline explicit ArithmeticInterval(rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::ArithmeticInterval> final : std::true_type {};

struct FiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticProgression;

  inline explicit FiniteArithmeticProgression(
      rq::TypeConstant &descendent, rq::ArithmeticSequenceCondition condition,
      rq::ArithmeticSequenceStep step);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::FiniteArithmeticProgression> final : std::true_type {};

struct InfiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticProgression;

  inline explicit InfiniteArithmeticProgression(
      rq::TypeConstant &descendent, rq::ArithmeticSequenceStep step);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::InfiniteArithmeticProgression> final : std::true_type {};

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModuleKind _module_kind;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;
  rq::Top *_top_ptr{nullptr};

  inline explicit Module(rq::ModuleKind kind, llvm::StringRef path,
                         llvm::MemoryBufferRef &&buffer, rq::Top &top);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceExpressionPtr(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Top &getTop() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Top &getTop();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Module> final : std::true_type {};

struct Import final : public rq::Symbol,
                      public rq::InitialExpression,
                      public rq::InitialExpressionFlags,
                      public rq::InitialModuleMember {
  using Self = rq::Import;

  inline explicit Import(const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Code : public rq::Symbol,
              public rq::InitialExpression,
              public rq::InitialExpressionFlags,
              public rq::InitialModuleMember,
              public rq::TableMember,
              public rq::TableHosted,
              public rq::InitialNamed {
  using Self = rq::Code;

  inline explicit Code(llvm::StringRef name, const rq::Expression &expression,
                       rq::ExpressionFlags attributes, rq::Module &module,
                       rq::Table &containing_table, rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct CategoryDiscriminant : public rq::Symbol {
  using Self = rq::CategoryDiscriminant;

  rq::Category *_category_ptr;

  inline explicit CategoryDiscriminant(rq::Category &category);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::CategoryDiscriminant> final : std::true_type {};

struct Label : public rq::Symbol,
               public rq::InitialExpression,
               public rq::InitialModuleMember,
               public rq::TableMember,
               public rq::InitialNamed {
  using Self = rq::Label;

  const rq::Expression *_ascription_ptr;
  rq::Entity *_subject_ptr;

  inline explicit Label(llvm::StringRef name, rq::Expression &expression,
                        const rq::Expression &ascription, rq::Entity &subject,
                        rq::Module &module, rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getAscription() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getSubject() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getSubject();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Synonym : public rq::Symbol {
  using Self = rq::Synonym;

  rq::Symbol *_original_ptr;

  inline explicit Synonym(rq::Symbol &original);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getOriginal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getOriginal();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Synonym> final : std::true_type {};

template <>
struct is_acquired<rq::CategoryAlternative> final : std::true_type {};

struct Local : public rq::Symbol,
               public rq::InitialExpression,
               public rq::InitialExpressionFlags,
               public rq::InitialModuleMember,
               public rq::TableMember,
               public rq::InitialNamed {
  using Self = rq::Local;

  bool _is_indeterminate : 1 {true};
  rq::TypeConstant *_type_ptr{nullptr};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit Local(llvm::StringRef name, const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module,
                        rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIndeterminate() const;
  RQ_ALWAYS_INLINE void setNotIndeterminate();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const;
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE void
  setValueExpression(const rq::Expression &expression_ptr);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Static : public rq::Symbol,
                public rq::InitialExpression,
                public rq::InitialExpressionFlags,
                public rq::InitialModuleMember,
                public rq::TableMember,
                public rq::InitialNamed {
  using Self = rq::Static;

  bool _is_indeterminate : 1 {true};
  rq::TypeConstant *_type_ptr{nullptr};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit Static(llvm::StringRef name, const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIndeterminate() const;
  RQ_ALWAYS_INLINE void setNotIndeterminate();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const;
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE void
  setValueExpression(const rq::Expression &expression_ptr);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Enumerator : public rq::Symbol,
                    public rq::InitialExpression,
                    public rq::InitialExpressionFlags,
                    public rq::InitialModuleMember,
                    public rq::TableMember,
                    public rq::TableHosted,
                    public rq::InitialNamed {
  using Self = rq::Enumerator;

  inline explicit Enumerator(llvm::StringRef name, rq::Expression &expression,
                             rq::ExpressionFlags attributes, rq::Module &module,
                             rq::Table &containing_table,
                             rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Enumeration &getEnumeration() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Enumeration &getEnumeration();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Enumerator> final : std::true_type {};

struct CategoryAlternative : public rq::Symbol,
                             public rq::InitialExpression,
                             public rq::InitialExpressionFlags,
                             public rq::InitialModuleMember,
                             public rq::TableMember {
  using Self = rq::CategoryAlternative;

  rq::Code *_code_ptr;

  inline explicit CategoryAlternative(rq::Code &code, rq::Category &category,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Code &getCode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Code &getCode();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TableIterator final {
  using Self = rq::TableIterator;
  using value_type = rq::Table;
  using reference = rq::Table &;
  using pointer = rq::Table *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Table *_table_ptr{nullptr};

  TableIterator() = default;
  inline explicit TableIterator(rq::Table *table_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstTableIterator final {
  using Self = rq::ConstTableIterator;
  using value_type = rq::Table;
  using reference = const rq::Table &;
  using pointer = const rq::Table *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Table *_table_ptr = nullptr;

  ConstTableIterator() = default;
  inline explicit ConstTableIterator(const rq::Table *table_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct MemberTableIterator final {
  using Self = rq::MemberTableIterator;
  using value_type = rq::Table;
  using reference = rq::Table &;
  using pointer = rq::Table *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Table *_table_ptr{nullptr};

  MemberTableIterator() = default;
  inline MemberTableIterator(rq::Table *table_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Table *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstMemberTableIterator final {
  using Self = rq::ConstMemberTableIterator;
  using value_type = rq::Table;
  using reference = const rq::Table &;
  using pointer = const rq::Table *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Table *_table_ptr = nullptr;

  ConstMemberTableIterator() = default;
  inline explicit ConstMemberTableIterator(const rq::Table *table_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Table *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct Table : public rq::Symbol, public rq::TableMember {
  using Self = rq::Table;

  rq::Table *_next_table_ptr{nullptr};
  rq::Table *_first_member_table_ptr{nullptr};
  llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_symbols_map{};
  rq::BumpPtrList<rq::Symbol> _unamed_symbols_list{};

  inline explicit Table(rq::Opcode opcode);
  inline explicit Table(rq::Opcode opcode, rq::Table &containing_table);
  inline void release();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListRef<rq::Symbol>
  getUnamedSymbolsListRef() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
  getUnamedSymbolsListRef();
  inline void _addMember(rq::Symbol &symbol);
  inline void addNamedSymbol(rq::BumpPtrAllocator &allocator,
                             llvm::StringRef name, rq::Symbol &symbol);
  inline void addUnamedSymbol(rq::BumpPtrAllocator &allocator,
                              rq::Symbol &symbol);
  [[nodiscard]] inline rq::ConstBumpPtrListRef<rq::Symbol>
  getNamedListRef(llvm::StringRef name) const;
  [[nodiscard]] inline rq::BumpPtrListRef<rq::Symbol>
  getNamedListRef(llvm::StringRef name);
  [[nodiscard]] inline auto getNamedListsSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      std::ranges::subrange_kind::unsized>
  getNamedListsSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstTableIterator, rq::ConstTableIterator,
      std::ranges::subrange_kind::unsized>
  getInclusiveFrameSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::TableIterator, rq::TableIterator, std::ranges::subrange_kind::unsized>
  getInclusiveFrameSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::MemberTableIterator, rq::MemberTableIterator,
      std::ranges::subrange_kind::unsized>
  getMemberTableSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstMemberTableIterator, rq::ConstMemberTableIterator,
      std::ranges::subrange_kind::unsized>
  getMemberTableSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Table> final : std::true_type {};

struct Top : public rq::Table {
  using Self = rq::Top;

  inline explicit Top();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Global : public rq::Table,
                public rq::InitialExpression,
                public rq::InitialExpressionFlags,
                public rq::InitialModuleMember,
                public rq::InitialNamed {
  using Self = rq::Global;

  bool _is_implemented : 1 {false};
  rq::TypeConstant *_type_ptr{nullptr};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit Global(llvm::StringRef name, const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const;
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE void
  setValueExpression(const rq::Expression &expression_ptr);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct GlobalStatic : public rq::Table,
                      public rq::InitialExpression,
                      public rq::InitialExpressionFlags,
                      public rq::InitialModuleMember,
                      public rq::InitialNamed {
  using Self = rq::Global;

  bool _is_implemented : 1 {false};
  rq::TypeConstant *_type_ptr{nullptr};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit GlobalStatic(llvm::StringRef name,
                               const rq::Expression &expression,
                               rq::ExpressionFlags attributes,
                               rq::Module &module, rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const;
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression &expression);
  RQ_ALWAYS_INLINE void
  setValueExpression(const rq::Expression &expression_ptr);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Scope : public rq::Table,
               rq::InitialExpression,
               rq::InitialModuleMember {
  using Self = rq::Scope;

  inline explicit Scope(rq::Expression &expression, rq::Module &module,
                        rq::Table &containing_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Namespace : public rq::Table, rq::InitialNamed {
  using Self = rq::Namespace;

  inline explicit Namespace(llvm::StringRef name, rq::Table &containing_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Class : public rq::Table,
               public rq::InitialExpression,
               public rq::InitialExpressionFlags,
               public rq::InitialModuleMember,
               public rq::TableHosted,
               public rq::InitialNamed {
  using Self = rq::Class;

  bool _is_implemented : 1 {false};
  const rq::Expression *_class_layout_expression_ptr{nullptr};
  rq::ClassLayout *_class_layout_ptr{nullptr};

  inline explicit Class(llvm::StringRef name, const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module,
                        rq::Table &containing_table, rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  inline void setClassLayoutExpression(const rq::Expression &layout_expression);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Enumeration : public rq::Table,
                     public rq::InitialExpression,
                     public rq::InitialExpressionFlags,
                     public rq::InitialModuleMember,
                     public rq::TableHosted,
                     public rq::InitialNamed {
  using Self = rq::Enumeration;

  bool _is_implemented : 1 {false};
  const rq::Expression *_underlying_type_expression_ptr{nullptr};

  inline explicit Enumeration(llvm::StringRef name,
                              const rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module, rq::Table &containing_table,
                              rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  RQ_ALWAYS_INLINE void
  setUnderlyingTypeExpression(const rq::Expression &underlying_type_expression);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Category : public rq::Table,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::TableHosted,
                  public rq::InitialNamed {
  using Self = rq::Category;

  bool _is_implemented : 1 {false};
  const rq::Expression *_discriminant_type_expression_ptr{nullptr};

  inline explicit Category(llvm::StringRef name,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::Table &containing_table,
                           rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  RQ_ALWAYS_INLINE void
  setDiscriminantTypeExpression(const rq::Expression &type_expression);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateIterator final {
  using Self = rq::TemplateIterator;
  using value_type = rq::Template;
  using reference = rq::Template &;
  using pointer = rq::Template *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Template *_template_ptr{nullptr};

  TemplateIterator() = default;
  inline explicit TemplateIterator(rq::Template *template_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstTemplateIterator final {
  using Self = rq::ConstTemplateIterator;
  using value_type = rq::Template;
  using reference = const rq::Template &;
  using pointer = const rq::Template *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Template *_template_ptr{nullptr};

  ConstTemplateIterator() = default;
  inline explicit ConstTemplateIterator(const rq::Template *template_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

template <typename UnderlyingParam> struct UnderlyingTemplateIterator final {
  using Underlying = UnderlyingParam;
  using Self = rq::UnderlyingTemplateIterator<Underlying>;
  using value_type = Underlying;
  using reference = Underlying &;
  using pointer = Underlying *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  static_assert(!std::is_const_v<Underlying>);
  static_assert(!std::is_volatile_v<Underlying>);

  Underlying *_underlying_ptr{nullptr};

  UnderlyingTemplateIterator() = default;
  inline explicit UnderlyingTemplateIterator(Underlying *underlying_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE Underlying &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const Underlying &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE Underlying *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const Underlying *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

template <typename UnderlyingParam>
struct ConstUnderlyingTemplateIterator final {
  using Underlying = UnderlyingParam;
  using Self = rq::ConstUnderlyingTemplateIterator<Underlying>;
  using value_type = rq::Template;
  using reference = const rq::Template &;
  using pointer = const rq::Template *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  static_assert(!std::is_const_v<Underlying>);
  static_assert(!std::is_volatile_v<Underlying>);

  const Underlying *_underlying_ptr{nullptr};

  ConstUnderlyingTemplateIterator() = default;
  inline explicit ConstUnderlyingTemplateIterator(
      const Underlying *underlying_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const Underlying &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const Underlying *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct Polymorph : public rq::Symbol, public rq::TableMember {
  using Self = rq::Polymorph;

  rq::Template *_first_template_ptr{nullptr};

  explicit inline Polymorph(rq::Opcode opcode, rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::TemplateIterator, rq::TemplateIterator,
      std::ranges::subrange_kind::unsized>
  getTemplateSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstTemplateIterator, rq::ConstTemplateIterator,
      std::ranges::subrange_kind::unsized>
  getTemplateSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct RangerIterator final {
  using Self = rq::RangerIterator;
  using value_type = rq::Ranger;
  using reference = rq::Ranger &;
  using pointer = rq::Ranger *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Ranger *_ranger_ptr{nullptr};

  RangerIterator() = default;
  inline explicit RangerIterator(rq::Ranger *ranger_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Ranger &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Ranger *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstRangerIterator final {
  using Self = rq::ConstRangerIterator;
  using value_type = rq::Ranger;
  using reference = const rq::Ranger &;
  using pointer = const rq::Ranger *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Ranger *_ranger_ptr{nullptr};

  ConstRangerIterator() = default;
  inline explicit ConstRangerIterator(const rq::Ranger *ranger_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Ranger *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct RangerPolymorph : public rq::Polymorph {
  using Self = rq::RangerPolymorph;

  rq::Ranger *_first_ranger_ptr{nullptr};

  explicit inline RangerPolymorph(rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRanger() const;
  RQ_ALWAYS_INLINE void addRanger(rq::Ranger &ranger);
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateRanger>,
      rq::UnderlyingTemplateIterator<rq::TemplateRanger>,
      std::ranges::subrange_kind::unsized>
  getTemplateRangerSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateRanger>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateRanger>,
      std::ranges::subrange_kind::unsized>
  getTemplateRangerSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::RangerIterator, rq::RangerIterator,
      std::ranges::subrange_kind::unsized>
  getRangerSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstRangerIterator, rq::ConstRangerIterator,
      std::ranges::subrange_kind::unsized>
  getRangerSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ProcedureIterator final {
  using Self = rq::ProcedureIterator;
  using value_type = rq::Procedure;
  using reference = rq::Procedure &;
  using pointer = rq::Procedure *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Procedure *_procedure_ptr{nullptr};

  ProcedureIterator() = default;
  inline explicit ProcedureIterator(rq::Procedure *procedure_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstProcedureIterator final {
  using Self = rq::ConstProcedureIterator;
  using value_type = rq::Procedure;
  using reference = const rq::Procedure &;
  using pointer = const rq::Procedure *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Procedure *_procedure_ptr{nullptr};

  ConstProcedureIterator() = default;
  inline explicit ConstProcedureIterator(const rq::Table *table_ptr);
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ProcedurePolymorph : public rq::Polymorph, rq::InitialNamed {
  using Self = rq::ProcedurePolymorph;

  rq::Procedure *_first_procedure_ptr{nullptr};

  explicit inline ProcedurePolymorph(llvm::StringRef name,
                                     rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasProcedure() const;
  RQ_ALWAYS_INLINE void addProcedure(rq::Procedure &procedure);
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateProcedure>,
      rq::UnderlyingTemplateIterator<rq::TemplateProcedure>,
      std::ranges::subrange_kind::unsized>
  getTemplateProcedureSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateProcedure>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateProcedure>,
      std::ranges::subrange_kind::unsized>
  getTemplateProcedureSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::ProcedureIterator, rq::ProcedureIterator,
      std::ranges::subrange_kind::unsized>
  getProcedureSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstProcedureIterator, rq::ConstProcedureIterator,
      std::ranges::subrange_kind::unsized>
  getProcedureSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ClassPolymorph : public rq::Polymorph, public rq::InitialNamed {
  using Self = rq::ClassPolymorph;

  rq::BumpPtrList<rq::Class> _class_list{};

  explicit inline ClassPolymorph(llvm::StringRef name,
                                 rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasClass() const;
  RQ_ALWAYS_INLINE void addClass(rq::BumpPtrAllocator &allocator,
                                 rq::Class &class_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Class &getClass() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Class &getClass();
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateClass>,
      rq::UnderlyingTemplateIterator<rq::TemplateClass>,
      std::ranges::subrange_kind::unsized>
  getTemplateClassSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateClass>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateClass>,
      std::ranges::subrange_kind::unsized>
  getTemplateClassSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::BumpPtrListIterator<rq::Class>, rq::BumpPtrListIterator<rq::Class>,
      std::ranges::subrange_kind::unsized>
  getClassSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::Class>,
      rq::ConstBumpPtrListIterator<rq::Class>,
      std::ranges::subrange_kind::unsized>
  getClassSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct EnumerationPolymorph : public rq::Polymorph, public rq::InitialNamed {
  using Self = rq::EnumerationPolymorph;

  rq::BumpPtrList<rq::Enumeration> _enumeration_list{};

  explicit inline EnumerationPolymorph(llvm::StringRef name,
                                       rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEnumeration() const;
  RQ_ALWAYS_INLINE void addEnumeration(rq::BumpPtrAllocator &allocator,
                                       rq::Enumeration &enumeration);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Enumeration &getEnumeration() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Enumeration &getEnumeration();
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateEnumeration>,
      rq::UnderlyingTemplateIterator<rq::TemplateEnumeration>,
      std::ranges::subrange_kind::unsized>
  getTemplateEnumerationSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateEnumeration>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateEnumeration>,
      std::ranges::subrange_kind::unsized>
  getTemplateEnumerationSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::BumpPtrListIterator<rq::Enumeration>,
      rq::BumpPtrListIterator<rq::Enumeration>,
      std::ranges::subrange_kind::unsized>
  getEnumerationSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::Enumeration>,
      rq::ConstBumpPtrListIterator<rq::Enumeration>,
      std::ranges::subrange_kind::unsized>
  getEnumerationSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct CategoryPolymorph : public rq::Polymorph, public rq::InitialNamed {
  using Self = rq::CategoryPolymorph;

  rq::BumpPtrList<rq::Category> _category_list{};

  explicit inline CategoryPolymorph(llvm::StringRef name,
                                    rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCategory() const;
  RQ_ALWAYS_INLINE void addCategory(rq::BumpPtrAllocator &allocator,
                                    rq::Category &category);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Category &getCategory() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Category &getCategory();
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateCategory>,
      rq::UnderlyingTemplateIterator<rq::TemplateCategory>,
      std::ranges::subrange_kind::unsized>
  getTemplateCategorySubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateCategory>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateCategory>,
      std::ranges::subrange_kind::unsized>
  getTemplateCategorySubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::BumpPtrListIterator<rq::Category>,
      rq::BumpPtrListIterator<rq::Category>,
      std::ranges::subrange_kind::unsized>
  getCategorySubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::Category>,
      rq::ConstBumpPtrListIterator<rq::Category>,
      std::ranges::subrange_kind::unsized>
  getCategorySubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct GlobalPolymorph : public rq::Polymorph, public rq::InitialNamed {
  using Self = rq::GlobalPolymorph;

  rq::BumpPtrList<rq::Global> _global_list{};

  explicit inline GlobalPolymorph(llvm::StringRef name,
                                  rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobal() const;
  RQ_ALWAYS_INLINE void addGlobal(rq::BumpPtrAllocator &allocator,
                                  rq::Global &global);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Global &getGlobal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Global &getGlobal();
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateGlobal>,
      rq::UnderlyingTemplateIterator<rq::TemplateGlobal>,
      std::ranges::subrange_kind::unsized>
  getTemplateGlobalSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateGlobal>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateGlobal>,
      std::ranges::subrange_kind::unsized>
  getTemplateGlobalSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::BumpPtrListIterator<rq::Global>, rq::BumpPtrListIterator<rq::Global>,
      std::ranges::subrange_kind::unsized>
  getGlobalSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::Global>,
      rq::ConstBumpPtrListIterator<rq::Global>,
      std::ranges::subrange_kind::unsized>
  getGlobalSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct GlobalStaticPolymorph : public rq::Polymorph, public rq::InitialNamed {
  using Self = rq::GlobalStaticPolymorph;

  rq::BumpPtrList<rq::GlobalStatic> _global_static_list{};

  explicit inline GlobalStaticPolymorph(llvm::StringRef name,
                                        rq::Table &containing_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobal() const;
  RQ_ALWAYS_INLINE void addGlobalStatic(rq::BumpPtrAllocator &allocator,
                                        rq::GlobalStatic &global);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStatic &
  getGlobalStatic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStatic &getGlobalStatic();
  [[nodiscard]] inline std::ranges::subrange<
      rq::UnderlyingTemplateIterator<rq::TemplateGlobalStatic>,
      rq::UnderlyingTemplateIterator<rq::TemplateGlobalStatic>,
      std::ranges::subrange_kind::unsized>
  getTemplateGlobalStaticSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstUnderlyingTemplateIterator<rq::TemplateGlobalStatic>,
      rq::ConstUnderlyingTemplateIterator<rq::TemplateGlobalStatic>,
      std::ranges::subrange_kind::unsized>
  getTemplateGlobalStaticSubrange() const;
  [[nodiscard]] inline std::ranges::subrange<
      rq::BumpPtrListIterator<rq::GlobalStatic>,
      rq::BumpPtrListIterator<rq::GlobalStatic>,
      std::ranges::subrange_kind::unsized>
  getGlobalStaticSubrange();
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::GlobalStatic>,
      rq::ConstBumpPtrListIterator<rq::GlobalStatic>,
      std::ranges::subrange_kind::unsized>
  getGlobalStaticSubrange() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Ranger : public rq::Table,
                public rq::InitialExpression,
                public rq::InitialExpressionFlags,
                public rq::InitialModuleMember {
  using Self = rq::Ranger;

  bool _is_implemented : 1 {false};
  rq::Ranger *_next_ranger_ptr{nullptr};
  rq::Instruction *_instruction_ptr{nullptr};

  inline explicit Ranger(rq::Opcode opcode, const rq::Expression &expression,
                         rq::ExpressionFlags flags, rq::Module &module,
                         rq::Table &containing_table, rq::Table &hosting_table);
  [[nodiscard]] bool getIsImplemented() const;
  RQ_ALWAYS_INLINE void setIsImplemented();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;

  inline explicit ForwardRanger(const rq::Expression &expression,
                                rq::ExpressionFlags flags, rq::Module &module,
                                rq::Table &containing_table,
                                rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;

  inline explicit BackwardRanger(const rq::Expression &expression,
                                 rq::ExpressionFlags flags, rq::Module &module,
                                 rq::Table &containing_table,
                                 rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Procedure : public rq::Table,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::TableHosted,
                   public rq::InitialMaybeNamed {
  using Self = rq::Procedure;

  bool _is_implemented : 1 {false};
  rq::TypeConstant *_signature_ptr{nullptr};
  const rq::Expression *_signature_expression_ptr{nullptr};
  const rq::Expression *_body_start_ptr{nullptr};
  rq::Instruction *_instruction_ptr{nullptr};
  llvm::StringRef _mangled_name{};
  llvm::Function *_llvm_function_ptr{nullptr};

  inline explicit Procedure(rq::Opcode opcode, llvm::StringRef name,
                            const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &containing_table,
                            rq::Table &hosting_table);
  inline explicit Procedure(rq::Opcode opcode, const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::Table &containing_table,
                            rq::Table &hosting_table);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented();
  RQ_ALWAYS_INLINE void setIsImplemented();
  RQ_ALWAYS_INLINE void
  setSignatureExpression(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSignature() const;
  RQ_ALWAYS_INLINE void setSignature(rq::TypeConstant &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getSignature() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getSignature();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const;
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction *instruction_ptr);
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();
  RQ_ALWAYS_INLINE bool getHasSignatureExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getSignatureExpression() const;
  RQ_ALWAYS_INLINE bool getHasBodyStartExpression() const;
  RQ_ALWAYS_INLINE void
  setBodyStartExpression(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getBodyStartExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangledName() const;
  RQ_ALWAYS_INLINE void setMangledName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;
  RQ_ALWAYS_INLINE void setLlvmFunctionPtr(llvm::Function *llvm_function_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Function *
  getLlvmFunctionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Function *getLlvmFunctionPtr();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Procedure> final : std::true_type {};

struct Entry : public rq::Procedure {
  using Self = rq::Entry;

  inline explicit Entry(const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module,
                        rq::Table &containing_table, rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Function : public rq::Procedure {
  using Self = rq::Function;

  inline explicit Function(llvm::StringRef name,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::Table &containing_table,
                           rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Method : public rq::Procedure {
  using Self = rq::Method;

  inline explicit Method(llvm::StringRef name, const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::Table &containing_table, rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ExtensionFunction : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  inline explicit ExtensionFunction(llvm::StringRef name,
                                    const rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ExtensionMethod : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  inline explicit ExtensionMethod(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::Table &containing_table,
                                  rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct ExtensionRanger : public rq::Procedure {
  using Self = rq::ExtensionRanger;

  inline explicit ExtensionRanger(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::Table &containing_table,
                                  rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Template : public rq::Symbol,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::TableMember,
                  public rq::TableHosted,
                  public rq::InitialNamed {
  using Self = rq::Template;

  rq::Template *_next_template_ptr{nullptr};
  rq::TemplateLayout *_template_layout_ptr;

  inline explicit Template(rq::Opcode opcode, llvm::StringRef name,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::Table &containing_table,
                           rq::Table &hosting_table,
                           rq::TemplateLayout &template_layout);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateLayout &
  getTemplateLayout() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateLayout &getTemplateLayout();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Template> final : std::true_type {};

struct TemplateClass : public rq::Template {
  using Self = rq::TemplateClass;

  inline explicit TemplateClass(llvm::StringRef name,
                                const rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table,
                                rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateEnumeration : public rq::Template {
  using Self = rq::TemplateEnumeration;

  inline explicit TemplateEnumeration(llvm::StringRef name,
                                      const rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table,
                                      rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateCategory : public rq::Template {
  using Self = rq::TemplateCategory;

  inline explicit TemplateCategory(llvm::StringRef name,
                                   const rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::Table &containing_table,
                                   rq::Table &hosting_table,
                                   rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateGlobal : public rq::Template {
  using Self = rq::TemplateGlobal;

  inline explicit TemplateGlobal(llvm::StringRef name,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::Table &containing_table,
                                 rq::Table &hosting_table,
                                 rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateGlobalStatic : public rq::Template {
  using Self = rq::TemplateGlobalStatic;

  inline explicit TemplateGlobalStatic(llvm::StringRef name,
                                       const rq::Expression &expression,
                                       rq::ExpressionFlags attributes,
                                       rq::Module &module,
                                       rq::Table &containing_table,
                                       rq::Table &hosting_table,
                                       rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateRanger : public rq::Template {
  using Self = rq::TemplateForwardRanger;

  inline explicit TemplateRanger(rq::Opcode opcode, llvm::StringRef name,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::Table &containing_table,
                                 rq::Table &hosting_table,
                                 rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateForwardRanger : public rq::TemplateRanger {
  using Self = rq::TemplateForwardRanger;

  inline explicit TemplateForwardRanger(llvm::StringRef name,
                                        const rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::Table &containing_table,
                                        rq::Table &hosting_table,
                                        rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateBackwardRanger : public rq::TemplateRanger {
  using Self = rq::TemplateBackwardRanger;

  inline explicit TemplateBackwardRanger(llvm::StringRef name,
                                         const rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::Table &containing_table,
                                         rq::Table &hosting_table,
                                         rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateProcedure : public rq::Template {
  using Self = rq::TemplateProcedure;

  inline explicit TemplateProcedure(rq::Opcode opcode, llvm::StringRef name,
                                    const rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table,
                                    rq::TemplateLayout &template_layout);
  [[nodiscard]] inline bool classof(const Entity *entity);
};

struct TemplateFunction : public rq::TemplateProcedure {
  using Self = rq::TemplateFunction;

  inline explicit TemplateFunction(llvm::StringRef name,
                                   const rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::Table &containing_table,
                                   rq::Table &hosting_table,
                                   rq::TemplateLayout &template_layout);
  [[nodiscard]] inline bool classof(const Entity *entity);
};

struct TemplateMethod : public rq::TemplateProcedure {
  using Self = rq::TemplateMethod;

  inline explicit TemplateMethod(llvm::StringRef name,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::Table &containing_table,
                                 rq::Table &hosting_table,
                                 rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateExtensionFunction : public rq::TemplateProcedure {
  using Self = rq::TemplateExtensionFunction;

  inline explicit TemplateExtensionFunction(
      llvm::StringRef name, const rq::Expression &expression,
      rq::ExpressionFlags attributes, rq::Module &module,
      rq::Table &containing_table, rq::Table &hosting_table,
      rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateExtensionMethod : public rq::TemplateProcedure {
  using Self = rq::TemplateExtensionMethod;

  inline explicit TemplateExtensionMethod(llvm::StringRef name,
                                          const rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::Table &containing_table,
                                          rq::Table &hosting_table,
                                          rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Specialized : public rq::Symbol,
                     public rq::InitialExpression,
                     public rq::InitialExpressionFlags,
                     public rq::InitialModuleMember,
                     public rq::TableMember,
                     public rq::TableHosted,
                     public rq::InitialNamed {
  using Self = rq::Specialized;

  inline explicit Specialized(rq::Opcode opcode, llvm::StringRef name,
                              const rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module, rq::Table &containing_table,
                              rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Specialized> final : std::true_type {};

struct SpecializedClass : public rq::Specialized {
  using Self = rq::SpecializedClass;

  inline explicit SpecializedClass(llvm::StringRef name,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::Table &containing_table,
                                   rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedEnumeration : public rq::Specialized {
  using Self = rq::SpecializedEnumeration;

  inline explicit SpecializedEnumeration(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::Table &containing_table,
                                         rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedCategory : public rq::Specialized {
  using Self = rq::SpecializedCategory;

  inline explicit SpecializedCategory(llvm::StringRef name,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedGlobal : public rq::Specialized {
  using Self = rq::SpecializedGlobal;

  inline explicit SpecializedGlobal(llvm::StringRef name,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedGlobalStatic : public rq::Specialized {
  using Self = rq::SpecializedGlobalStatic;

  inline explicit SpecializedGlobalStatic(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::Table &containing_table,
                                          rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedRanger : public rq::Specialized {
  using Self = rq::SpecializedRanger;

  inline explicit SpecializedRanger(rq::Opcode opcode, llvm::StringRef name,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedForwardRanger : public rq::SpecializedRanger {
  using Self = rq::SpecializedForwardRanger;

  inline explicit SpecializedForwardRanger(llvm::StringRef name,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags attributes,
                                           rq::Module &module,
                                           rq::Table &containing_table,
                                           rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedBackwardRanger : public rq::SpecializedRanger {
  using Self = rq::SpecializedBackwardRanger;

  inline explicit SpecializedBackwardRanger(llvm::StringRef name,
                                            rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::Table &containing_table,
                                            rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedProcedure : public rq::Specialized {
  using Self = rq::SpecializedProcedure;

  inline explicit SpecializedProcedure(rq::Opcode opcode, llvm::StringRef name,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags attributes,
                                       rq::Module &module,
                                       rq::Table &containing_table,
                                       rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedFunction : public rq::SpecializedProcedure {
  using Self = rq::SpecializedFunction;

  inline explicit SpecializedFunction(llvm::StringRef name,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedMethod : public rq::SpecializedProcedure {
  using Self = rq::SpecializedMethod;

  inline explicit SpecializedMethod(llvm::StringRef name,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::Table &containing_table,
                                    rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedExtensionFunction : public rq::SpecializedProcedure {
  using Self = rq::SpecializedExtensionFunction;

  inline explicit SpecializedExtensionFunction(llvm::StringRef name,
                                               rq::Expression &expression,
                                               rq::ExpressionFlags attributes,
                                               rq::Module &module,
                                               rq::Table &containing_table,
                                               rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct SpecializedExtensionMethod : public rq::SpecializedProcedure {
  using Self = rq::SpecializedExtensionMethod;

  inline explicit SpecializedExtensionMethod(llvm::StringRef name,
                                             rq::Expression &expression,
                                             rq::ExpressionFlags attributes,
                                             rq::Module &module,
                                             rq::Table &containing_table,
                                             rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  inline explicit Constant(rq::Opcode opcode);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Constant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileTypeConstant(llvm::FoldingSetNodeID &id,
                                          const rq::Symbol &symbol,
                                          rq::TypeFlags attributes);

struct TypeConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::TypeConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  inline explicit TypeConstant(rq::Symbol &symbol, rq::TypeFlags flags);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getTypeFlags() const;
  // TODO
  [[nodiscard]] inline bool getIsInferencing() const;
  [[nodiscard]] inline bool getIsType() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::TypeConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileExpressionConstant(
    llvm::FoldingSetNodeID &id, const rq::Expression &expression);

struct ExpressionConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_expression_ptr;

  inline explicit ExpressionConstant(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <>
struct is_acquired<rq::ExpressionConstant> final : std::true_type {};

struct BooleanConstant : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _value;

  inline explicit BooleanConstant(bool value);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getValue() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::BooleanConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &int_);

struct IntegerConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::IntegerConstant;

  llvm::APInt _ap_int;

  inline explicit IntegerConstant(const llvm::APInt &int_);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getInt() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::IntegerConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &float_);

struct FloatConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::FloatConstant;

  llvm::APFloat _ap_float;

  inline explicit FloatConstant(const llvm::APFloat &float_);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getFloat() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::FloatConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef string);

struct StringConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::StringConstant;

  llvm::StringRef _string;

  inline explicit StringConstant(llvm::StringRef string);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getString() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

template <> struct is_acquired<rq::StringConstant> final : std::true_type {};

struct ArrayConstant : public rq::Constant {
  using Self = rq::ArrayConstant;

  inline explicit ArrayConstant();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::ArrayConstant> final : std::true_type {};

struct ExpressionAttributeConstant : public rq::Constant {
  using Self = rq::ExpressionAttributeConstant;

  rq::ExpressionAttribute _expression_attribute;

  inline explicit ExpressionAttributeConstant(
      rq::ExpressionAttribute expression_attribute);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute
  getExpressionAttribute() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::ExpressionAttributeConstant> final : std::true_type {};

struct TypeAttributeConstant : public rq::Constant {
  using Self = rq::TypeAttributeConstant;

  rq::TypeAttribute _type_attribute;

  inline explicit TypeAttributeConstant(rq::TypeAttribute type_attribute);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getTypeAttribute() const;
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::TypeAttributeConstant> final : std::true_type {};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression *_expression_ptr{nullptr};

  inline explicit Instruction(rq::Opcode opcode);
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
};

template <> struct is_parent_only<rq::Instruction> final : std::true_type {};

struct NullaryInstruction : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  inline explicit NullaryInstruction(rq::Opcode opcode);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <>
struct is_acquired<rq::NullaryInstruction> final : std::true_type {};

struct UnaryInstruction : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr{nullptr};

  inline explicit UnaryInstruction(rq::Opcode opcode);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress0() const;
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity &address0);
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity *address0_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity &address0);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity *address0_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress0Ptr();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::UnaryInstruction> final : std::true_type {};

struct BinaryInstruction : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity *_address0_ptr{nullptr};
  rq::Entity *_address1_ptr{nullptr};

  inline explicit BinaryInstruction(rq::Opcode opcode);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress1() const;
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity &address0);
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity *address0_ptr);
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity &address1);
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity *address1_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity &address0);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity *address0_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity &address1);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity *address1_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress1() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress1();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress0Ptr();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress1();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress1Ptr();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::BinaryInstruction> final : std::true_type {};

} // namespace rq

#include <rq/detail/entity.hpp>