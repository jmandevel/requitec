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
  SY_SYMBOL_CONSTRAINT,
  SY_TYPE_CONSTRAINT,
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

  // PROCEDURE
  SY_ENTRY,
  SY_FUNCTION,
  SY_METHOD,
  SY_RANGER,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,
  SY_EXTENSION_RANGER,

  // TEMPLATE
  SY_TEMPLATE_CLASS,
  SY_TEMPLATE_ENUMERATION,
  SY_TEMPLATE_CATEGORY,
  SY_TEMPLATE_GLOBAL,
  SY_TEMPLATE_GLOBAL_STATIC,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_RANGER,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,
  SY_TEMPLATE_EXTENSION_RANGER,

  // PARTIAL SPECIALIZATION
  SY_PARTIAL_CLASS,
  SY_PARTIAL_ENUMERATION,
  SY_PARTIAL_CATEGORY,
  SY_PARTIAL_GLOBAL,
  SY_PARTIAL_GLOBAL_STATIC,
  SY_PARTIAL_FUNCTION,
  SY_PARTIAL_METHOD,
  SY_PARTIAL_RANGER,
  SY_PARTIAL_EXTENSION_FUNCTION,
  SY_PARTIAL_EXTENSION_METHOD,
  SY_PARTIAL_EXTENSION_RANGER,

  // =====CONSTANTS=====

  CT_TYPE,
  CT_EXPRESSION,
  CT_BOOLEAN,
  CT_INTEGER,
  CT_FLOAT,
  CT_STRING,
  CT_ARRAY,

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
  IN_DEBUG_TRAP,
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
  SY_PROCEDURE = rq::getBit(10),
  SY_TEMPLATE = rq::getBit(11),
  SY_PARTIAL = rq::getBit(12),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(13),
  SY_TYPE = rq::getBit(14),
  SY_SUBTYPE = rq::getBit(15),
  SY_CONSTRAINT = rq::getBit(16),
  SY_LITERAL = rq::getBit(17),
  SY_PLATFORM_CHANGING = rq::getBit(18),
  SY_INTEGER = rq::getBit(19),
  SY_FLOAT = rq::getBit(20),
  SY_BINARY = rq::getBit(21),
  SY_CODEUNIT = rq::getBit(22),
  SY_SIGNED = rq::getBit(23),
  SY_UNSIGNED = rq::getBit(24),
  SY_TOP_OF_FRAME = rq::getBit(25),

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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint(rq::Opcode opcode);
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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame(rq::Opcode opcode);
[[nodiscard]] inline rq::Opcode getTemplate(rq::Opcode opcode);
[[nodiscard]] inline rq::Opcode getPartial(rq::Opcode opcode);
[[nodiscard]] inline rq::Opcode getFull(rq::Opcode opcode);
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
struct SymbolConstraint;
struct TypeConstraint;
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
struct Procedure;
struct Entry;
struct Function;
struct Method;
struct Ranger;
struct ExtensionFunction;
struct ExtensionMethod;
struct ExtensionRanger;
struct Template;
struct TemplateClass;
struct TemplateEnumeration;
struct TemplateCategory;
struct TemplateGlobal;
struct TemplateGlobalStatic;
struct TemplateFunction;
struct TemplateMethod;
struct TemplateRanger;
struct TemplateExtensionFunction;
struct TemplateExtensionMethod;
struct TemplateExtensionRanger;
struct Partial;
struct PartialClass;
struct PartialEnumeration;
struct PartialCategory;
struct PartialGlobal;
struct PartialGlobalStatic;
struct PartialFunction;
struct PartialMethod;
struct PartialRanger;
struct PartialExtensionFunction;
struct PartialExtensionMethod;
struct PartialExtensionRanger;
struct Constant;
struct TypeConstant;
struct ExpressionConstant;
struct BooleanConstant;
struct IntegerConstant;
struct FloatConstant;
struct StringConstant;
struct ArrayConstant;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocation() const;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStableAddress() const;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterListSubtype() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint() const;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getTemplateOpcode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getPartialOpcode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getFullOpcode() const;
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
  [[nodiscard]] inline bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::Inference> final : std::true_type {};

struct SymbolConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SymbolConstraint;

  inline explicit SymbolConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::SymbolConstraint> final : std::true_type {};

struct TypeConstraint final : public rq::SimpleBuiltin {
  using Self = rq::TypeConstraint;

  inline explicit TypeConstraint();
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_acquired<rq::TypeConstraint> final : std::true_type {};

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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType();
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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType();
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

struct Ranger : public rq::Procedure {
  using Self = rq::Ranger;

  inline explicit Ranger(llvm::StringRef name, const rq::Expression &expression,
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

struct TemplateFunction : public rq::Template {
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

struct TemplateMethod : public rq::Template {
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

struct TemplateRanger : public rq::Template {
  using Self = rq::TemplateRanger;

  inline explicit TemplateRanger(llvm::StringRef name,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::Table &containing_table,
                                 rq::Table &hosting_table,
                                 rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateExtensionFunction : public rq::Template {
  using Self = rq::TemplateExtensionFunction;

  inline explicit TemplateExtensionFunction(
      llvm::StringRef name, const rq::Expression &expression,
      rq::ExpressionFlags attributes, rq::Module &module,
      rq::Table &containing_table, rq::Table &hosting_table,
      rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct TemplateExtensionMethod : public rq::Template {
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

struct TemplateExtensionRanger : public rq::Template {
  using Self = rq::TemplateExtensionRanger;

  inline explicit TemplateExtensionRanger(llvm::StringRef name,
                                          const rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::Table &containing_table,
                                          rq::Table &hosting_table,
                                          rq::TemplateLayout &template_layout);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct Partial : public rq::Symbol,
                 public rq::InitialExpression,
                 public rq::InitialExpressionFlags,
                 public rq::InitialModuleMember,
                 public rq::TableMember,
                 public rq::TableHosted,
                 public rq::InitialNamed {
  using Self = rq::Partial;

  inline explicit Partial(rq::Opcode opcode, llvm::StringRef name,
                          const rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::Table &containing_table,
                          rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

template <> struct is_parent_only<rq::Partial> final : std::true_type {};

struct PartialClass : public rq::Partial {
  using Self = rq::PartialClass;

  inline explicit PartialClass(llvm::StringRef name, rq::Expression &expression,
                               rq::ExpressionFlags attributes,
                               rq::Module &module, rq::Table &containing_table,
                               rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialEnumeration : public rq::Partial {
  using Self = rq::PartialEnumeration;

  inline explicit PartialEnumeration(llvm::StringRef name,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::Table &containing_table,
                                     rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialCategory : public rq::Partial {
  using Self = rq::PartialCategory;

  inline explicit PartialCategory(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::Table &containing_table,
                                  rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialGlobal : public rq::Partial {
  using Self = rq::PartialGlobal;

  inline explicit PartialGlobal(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialGlobalStatic : public rq::Partial {
  using Self = rq::PartialGlobalStatic;

  inline explicit PartialGlobalStatic(llvm::StringRef name,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::Table &containing_table,
                                      rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialFunction : public rq::Partial {
  using Self = rq::PartialFunction;

  inline explicit PartialFunction(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::Table &containing_table,
                                  rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialMethod : public rq::Partial {
  using Self = rq::PartialMethod;

  inline explicit PartialMethod(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialRanger : public rq::Partial {
  using Self = rq::PartialRanger;

  inline explicit PartialRanger(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module, rq::Table &containing_table,
                                rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialExtensionFunction : public rq::Partial {
  using Self = rq::PartialExtensionFunction;

  inline explicit PartialExtensionFunction(llvm::StringRef name,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags attributes,
                                           rq::Module &module,
                                           rq::Table &containing_table,
                                           rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialExtensionMethod : public rq::Partial {
  using Self = rq::PartialExtensionMethod;

  inline explicit PartialExtensionMethod(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::Table &containing_table,
                                         rq::Table &hosting_table);
  [[nodiscard]] inline static bool classof(const Entity *entity);
};

struct PartialExtensionRanger : public rq::Partial {
  using Self = rq::PartialExtensionRanger;

  inline explicit PartialExtensionRanger(llvm::StringRef name,
                                         const rq::Expression &expression,
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstant() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPartiallyMutable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatile() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomic() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminated() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayDiscard() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndeterminate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRanging() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::TypeAttribute attribute) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutability() const;
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