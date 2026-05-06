#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/Hashing.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/MemoryBufferRef.h>

#include <ranges>

namespace rq {

enum class Opcode {
  NONE,

  // LITERALS
  SY_INTEGER_LITERAL_TYPE,
  SY_FLOAT_LITERAL_TYPE,
  SY_STRING_LITERAL_TYPE,
  SY_CODEUNIT_LITERAL_TYPE,

  // CONTEXTUAL NAME
  SY_NO_NAME,

  // CONTEXTUAL VALUE
  SY_OUT,
  SY_THIS,
  SY_RESULT,
  SY_VALUE,
  SY_INDEX,
  SY_DISCRIMINANT,
  SY_COMMAND_LINE_ARGUMENTS,
  SY_CALLSITE,

  // CONTEXTUAL TYPE
  SY_INFERENCE_TYPE,
  SY_VOID_TYPE,
  SY_NO_RETURN_TYPE,

  // EXPRESSION ATTRIBUTES
  SY_ANCHORING_TYPE,
  SY_VISIBILITY_TYPE,
  SY_SCOPING_TYPE,
  SY_AVAILABILITY_TYPE,
  SY_PROPERTY_MUTABILITY_TYPE,
  SY_EXPORTING_TYPE,
  SY_GENERATION_TIME_TYPE,
  SY_CAPTURING_TYPE,
  SY_INLINING_TYPE,
  SY_MANGLING_TYPE,
  SY_PACKING_TYPE,
  SY_TEMPLATING_TYPE,
  SY_LIKELYHOOD_TYPE,
  SY_SUPPORT_TYPE,
  SY_ADDRESS_STABILITY_TYPE,
  SY_VARIADICNESS_TYPE,

  // TYPE ATTRIBUTES
  SY_VARIABILITY_TYPE,
  SY_VOLATILITY_TYPE,
  SY_ATOMICITY_TYPE,
  SY_NULL_TERMINATION_TYPE,
  SY_PRECONDITION_TYPE,
  SY_POSTCONDITION_TYPE,

  // REFLECTIVE
  SY_SYMBOL_TYPE,
  SY_EXPRESSION_TYPE,

  // PLATFORM PRIMITIVE
  SY_BOOLEAN_TYPE,
  SY_HALF_TYPE,
  SY_SINGLE_TYPE,
  SY_DOUBLE_TYPE,
  SY_QUADRUPLE_TYPE,
  SY_SIGNED_INTEGER_TYPE,
  SY_UNSIGNED_INTEGER_TYPE,
  SY_SIGNED_INDEX_TYPE,
  SY_UNSIGNED_INDEX_TYPE,
  SY_SIGNED_ADDRESS_TYPE,
  SY_UNSIGNED_ADDRESS_TYPE,
  SY_CHAR_TYPE,

  // STANDARD PRIMITIVE
  SY_BINARY16_TYPE,
  SY_BINARY32_TYPE,
  SY_BINARY64_TYPE,
  SY_BINARY128_TYPE,
  SY_BFLOAT16_TYPE,
  SY_ASCII_TYPE,
  SY_UTF8_TYPE,

  // VARIADIC ARGUMENTS
  SY_VARIADIC_ARGUMENTS_TYPE,

  // SCALED PRIMITIVES
  SY_SCALED_SIGNED_INTEGER_TYPE,
  SY_SCALED_UNSIGNED_INTEGER_TYPE,

  // SUBTYPES
  SY_ARRAY_SUBTYPE,

  // UNCOUNTED SUBTYPES => SUBTYPES
  SY_REFERENCE_SUBTYPE,
  SY_POINTER_SUBTYPE,
  SY_FAT_POINTER_SUBTYPE,
  SY_INFERENCE_TYPE_COUNT_ARRAY_SUBTYPE,

  // MODULES
  SY_MODULE,

  // IMPORTS
  SY_IMPORT,

  // CONCATENATED STRING
  SY_CONCATENATED_STRING_TYPE,

  // ARITHMETIC SEQUENCES
  SY_ARITHMETIC_INTERVAL,
  SY_INFINITE_ARITHMETIC_SEQUENCE,
  SY_FINITE_ARITHMETIC_SEQUENCE,

  // LOCAL DECLARATIONS
  SY_LABEL,
  SY_ANCHOR,

  // LOCAL VARIABLES => local declaration
  SY_LOCAL_DYNAMIC_VARIABLE,
  SY_LOCAL_STATIC_VARIABLE,

  // PARAMETERS => local variable
  SY_STATIC_PARAMETER,
  SY_DYNAMIC_PARAMETER,

  // PARAMETER LISTS
  SY_SIGNATURE,
  SY_LAYOUT,

  // PLACEMENTS
  SY_PLACEMENT,

  // COMPOSITIONS
  SY_COMPOSITION,

  // SYNONYMS
  SY_SYNONYM,

  // POLYMORPHS
  SY_RANGER_POLYMORPH,
  SY_PROCEDURE_POLYMORPH,
  SY_CLASS_POLYMORPH,
  SY_ENUMERATION_POLYMORPH,
  SY_INTERFACE_POLYMORPH,
  SY_GLOBAL_DYNAMIC_VARIABLE_POLYMORPH,
  SY_GLOBAL_STATIC_VARIABLE_POLYMORPH,

  // SYMBOL TABLES
  SY_TOP,

  // LOCAL TABLES => symbol table
  SY_IF,
  SY_ELSE_IF,
  SY_ELSE,
  SY_MATCH,
  SY_INLINE_MATCH,
  SY_SWITCH,
  SY_INLINE_SWITCH,
  SY_CASE,
  SY_WITH,
  SY_DEFAULT,
  SY_FOR,
  SY_WHILE,
  SY_SPIN,
  SY_WEAVE,
  SY_SCOPE,
  SY_INLINE_SCOPE,

  // GLOBAL DECLARATION => symbol table
  SY_NAMESPACE,
  SY_CLASS,
  SY_ENUMERATION,
  SY_ENUMERATOR,
  SY_INTERFACE,

  // GLOBAL VARIABLE => global declaration => symbol table
  SY_GLOBAL_DYNAMIC_VARIABLE,
  SY_GLOBAL_STATIC_VARIABLE,

  // RANGERS => global declaration => symbol table
  SY_FORWARD_RANGER,
  SY_BACKWARD_RANGER,

  // DESTRUCTOR => global declaration
  SY_DESTRUCTOR,

  // PROCEDURES => global declaration => symbol table
  SY_ENTRY,
  SY_FUNCTION,
  SY_MEMBER_FUNCTION,
  SY_METHOD,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,

  // TEMPLATES
  SY_CLASS_TEMPLATE,
  SY_ENUMERATION_TEMPLATE,
  SY_INTERFACE_TEMPLATE,
  SY_GLOBAL_DYNAMIC_VARIABLE_TEMPLATE,
  SY_GLOBAL_STATIC_VARIABLE_TEMPLATE,
  SY_FORWARD_RANGER_TEMPLATE,
  SY_BACKWARD_RANGER_TEMPLATE,
  SY_FUNCTION_TEMPLATE,
  SY_MEMBER_FUNCTION_TEMPLATE,
  SY_METHOD_TEMPLATE,
  SY_EXTENSION_FUNCTION_TEMPLATE,
  SY_EXTENSION_METHOD_TEMPLATE,

  CT_INTEGER,
  CT_FLOAT,
  CT_EXPRESSION,
  CT_SYMBOL,
  CT_BOOLEAN,
  CT_STRING,
  CT_ARRAY,
  CT_EXPRESSION_ATTRIBUTE,
  CT_TYPE_ATTRIBUTE,

  // TODO instructions
};

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode);

enum class OpcodeFlags : std::uint64_t {
  NONE = 0,

  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  INSTRUCTION_NONE_MASK = SYMBOL | CONSTANT,

  // ==SYMBOL FLAGS==
  // SYMBOL CLASSIFICATION
  SY_SIMPLE_SYMBOL = rq::getBit(2),
  SY_LITERAL = rq::getBit(3),
  SY_CONTEXTUAL = rq::getBit(4),
  // SY_CONTEXTUAL_NAME
  SY_CONTEXTUAL_VALUE = rq::getBit(6),
  SY_CONTEXTUAL_TYPE = rq::getBit(7),
  SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE = rq::getBit(8),
  SY_TYPE_ATTRIBUTE_TYPE = rq::getBit(9),
  SY_REFLECTIVE_TYPE = rq::getBit(10),
  SY_PLATFORM_PRIMITIVE_TYPE = rq::getBit(11),
  SY_STANDARD_PRIMITIVE_TYPE = rq::getBit(12),
  SY_SCALED_PRIMITIVE_TYPE = rq::getBit(13),
  SY_SUBTYPE = rq::getBit(14),
  SY_UNCOUNTED_SUBTYPE = rq::getBit(14),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(15),
  SY_LOCAL_DECLARATION = rq::getBit(16),
  SY_LOCAL_VARIABLE = rq::getBit(17),
  SY_PARAMETER = rq::getBit(18),
  SY_PARAMETER_LIST = rq::getBit(19),
  SY_POLYMORPH = rq::getBit(20),
  SY_SYMBOL_TYPE_TABLE = rq::getBit(21),
  SY_LOCAL_TABLE = rq::getBit(22),
  SY_GLOBAL_DECLARATION = rq::getBit(23),
  SY_GLOBAL_VARIABLE = rq::getBit(24),
  SY_RANGER = rq::getBit(25),
  SY_PROCEDURE = rq::getBit(26),
  SY_TEMPLATE = rq::getBit(27),

  // SYMBOL DETAILS
  SY_IS_TYPE = rq::getBit(28),
  SY_IS_SIGNED_TYPE = rq::getBit(29),
  SY_IS_UNSIGNED_TYPE = rq::getBit(30),
  SY_IS_INTEGER_TYPE = rq::getBit(31),
  SY_IS_FLOAT_TYPE = rq::getBit(32),
  SY_IS_BINARY_TYPE = rq::getBit(33),
  SY_IS_BFLOAT_TYPE = rq::getBit(34),
  SY_IS_CODEUNIT_TYPE = rq::getBit(35),

  // EXPRESSION ATTRIBUTES
  // SY_HAS_ANCHORING, LOCAL_TABLE
  SY_HAS_VISIBILITY = rq::getBit(37),
  SY_HAS_SCOPE_LOCATION = rq::getBit(38),
  // SY_HAS_AVAILABILITY = SY_GLOBAL_VARIABLE | SY_LOCAL_DECLARATION
  SY_HAS_ACCESSIBILITY = rq::getBit(39),
  // SY_HAS_PROPERTY_MUTABILITY, DYNAMIC PARAMETER
  SY_HAS_EXPORTING = rq::getBit(40),
  // SY_HAS_GENERATION_TIME, SY_GLOBAL_VARIABLE | SY_LOCAL_VARIABLE |
  // SY_LOCAL_TABLE
  SY_HAS_CAPTURING = rq::getBit(41),
  // SY_HAS_INLINING, PROCEDURES
  SY_HAS_MANGLING = rq::getBit(42),
  // SY_HAS_PACKING, CLASS
  SY_HAS_TEMPLATING = rq::getBit(42),
  SY_HAS_LIKELYHOOD = rq::getBit(42),
  SY_HAS_SUPPORT = rq::getBit(42),
  // SY_HAS_ADDRESS_STABILITY, CLASS
  // SY_HAS_VARIADICNESS, DYNAMIC_PARAMETER
  // SY_HAS_CONTRAINT, TEMPLATES
  // SY_HAS_WEIGHTING, TEMPLATES

  // TYPE ATTRIBUTES
  SY_HAS_VARIABILITY = rq::getBit(43),
  SY_HAS_VOLATILITY = rq::getBit(44),
  SY_HAS_ATOMICITY = rq::getBit(45),
  SY_HAS_NULL_TERMINATE = rq::getBit(46)
  // SY_HAS_PRECONDITION, SIGNATURE
  // SY_HAS_POSTCONDITION, SIGNATURE
};

template <> struct is_flags<rq::OpcodeFlags> final : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getFlags(rq::Opcode opcode);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode);

#define RQ_ASSERT_SYMBOL(opcode)                                               \
  RQ_ASSERT(rq::getIsSymbol(opcode), "not symbol");
#define RQ_ASSERT_CONSTANT(opcode)                                             \
  RQ_ASSERT(rq::getIsConstant(opcode), "not constant");
#define RQ_ASSERT_INSTRUCTION(opcode)                                          \
  RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction");

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualName(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttributeType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttributeType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledPrimitive(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUncountedSubtype(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclaration(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::Opcode opcode);

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAnchoring(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVisibility(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeLocation(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAvailability(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPropertyMutability(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasExporting(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasGenerationTime(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapturing(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasInlining(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangling(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPacking(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplating(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikelyhood(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasSupport(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressStability(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariadicness(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstraint(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasWeighting(rq::Opcode opcode);

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariability(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatility(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomicity(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminate(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPrecondition(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPostcondition(rq::Opcode opcode);

// clang-format off
struct Entity;
  struct Symbol;
    struct SimpleSymbol;
      struct Literal;
        struct IntegerLiteral;
        struct FloatLiteral;
        struct StringLiteral;
        struct CodeunitLiteral;
      struct Contextual;
        struct ContextualName;
          struct NoName;
        struct ContextualValue;
          struct OutValue;
          struct ThisValue;
          struct ResultValue;
          struct ValueValue;
          struct IndexValue;
          struct DiscriminantValue;
          struct CommandLineArgumentsValue;
          struct CallsiteValue;
        struct ContextualType;
          struct InferenceType;
          struct VoidType;
          struct NoReturnType;
      struct ExpressionAttributeType;
        struct AnchoringType;
        struct VisibilityType;
        struct ScopingType;
        struct AvailabilityType;
        struct PropertyMutabilityType;
        struct ExportingType;
        struct GenerationTimeType;
        struct CapturingType;
        struct EvaluationTimeType;
        struct InliningType;
        struct ManglingType;
        struct PackingType;
        struct TemplatingType;
        struct LikelyhoodType;
        struct SupportType;
        struct AddressStabilityType;
        struct VariadicnessType;
      struct TypeAttributeType;
        struct VariabilityType;
        struct VolatilityType;
        struct AtomicityType;
        struct NullTerminationType;
        struct PreconditionType;
        struct PostconditionType;
      struct ReflectiveType;
        struct SymbolType;
        struct ExpressionType;
      struct PlatformPrimitiveType;
        struct BooleanType;
        struct SignedIntegerType;
        struct UnsignedIntegerType;
        struct SignedIndexType;
        struct UnsignedIndexType;
        struct SignedAddressType;
        struct UnsignedAddressType;
        struct CharType;
      struct StandardPrimitiveType;
        struct HalfType;
        struct SingleType;
        struct DoubleType;
        struct QuadrupleType;
        struct Binary16Type;
        struct Binary32Type;
        struct Binary64Type;
        struct Binary128Type;
        struct Bfloat16Type;
        struct AsciiType;
        struct Utf8Type;
      struct VariadicArgumentType;
    struct ScaledPrimitiveType;
      struct ScaledSignedIntegerType;
      struct ScaledUnsignedIntegerType;
    struct Subtype;
      struct ArraySubtype;
      struct UncountedSubtype;
        struct ReferenceSubtype;
        struct PointerSubtype;
        struct FatPointerSubtype;
        struct InferenceCountArraySubtype;
    struct Module;
    struct Import;
    struct ConcatenatedString;
    struct ArithmeticSequence;
      struct ArithmeticInterval;
      struct InfiniteArithmeticSequence;
      struct FiniteArithmeticSequence;
    struct LocalDeclaration;
      struct Label;
      struct Anchor;
      struct LocalVariable;
        struct LocalDynamicVariable;
        struct LocalStaticVariable;
        struct Parameter;
          struct StaticParameter;
          struct DynamicParameter;
    struct ParameterList;
      struct Signature;
      struct Layout;
    struct Placement;
    struct Composition;
    struct Synonym;
    struct Polymorph;
      struct RangerPolymorph;
      struct ProcedurePolymorph;
      struct ClassPolymorph;
      struct EnumerationPolymorph;
      struct InterfacePolymorph;
      struct GlobalDynamicVariablePolymorph;
      struct GlobalStaticVariablePolymorph;
    struct SymbolTable;
      struct Top;
      struct LocalTable;
        struct IfTable;
        struct ElseIfTable;
        struct ElseTable;
        struct MatchTable;
        struct InlineMatchTable;
        struct SwitchTable;
        struct InlineSwitchTable;
        struct CaseTable;
        struct WithTable;
        struct DefaultTable;
        struct ForTable;
        struct WhileTable;
        struct SpinTable;
        struct WeaveTable;
        struct ScopeTable;
        struct InlineScopeTable;
      struct GlobalDeclaration;
        struct Namespace;
        struct Class;
        struct Enumeration;
        struct Enumerator;
        struct Interface;
        struct GlobalVariable;
          struct GlobalDynamicVariable;
          struct GlobalStaticVariable;
        struct Ranger;
          struct ForwardRanger;
          struct BackwardRanger;
        struct Destructor;
        struct Procedure;
          struct Entry;
          struct Function;
          struct MemberFunction;
          struct Method;
          struct ExtensionFunction;
          struct ExtensionMethod;
        struct Template;
          struct ClassTemplate;
          struct EnumerationTemplate;
          struct InterfaceTemplate;
          struct GlobalDynamicVariableTemplate;
          struct GlobalStaticVariableTemplate;
          struct ForwardRangerTemplate;
          struct BackwardRangerTemplate;
          struct FunctionTemplate;
          struct MemberFunctionTemplate;
          struct MethodTemplate;
          struct ExtensionFunctionTemplate;
          struct ExtensionMethodTemplate;
  struct Constant;
    struct IntegerConstant;
    struct FloatConstant;
    struct ExpressionConstant;
    struct SymbolConstant;
    struct BooleanConstant;
    struct StringConstant;
    struct ArrayConstant;
    struct ExpressionAttributeConstant;
    struct TypeAttributeConstant;
  struct Instruction;
    struct NullaryInstruction;
    struct UnaryInstruction;
    struct BinaryInstruction;
// clang-format on

struct Entity {
  using Self = rq::Entity;

  rq::Opcode _opcode;

  explicit RQ_ALWAYS_INLINE Entity(rq::Opcode opcode);
  Entity(const Self &) = delete;
  Entity(Self &&) = delete;
  ~Entity() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getOpcodeFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::Opcode opcode);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType() const;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAnchoring() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVisibility() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeLocation() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAvailability() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPropertyMutability() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExporting() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGenerationTime() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapturing() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInlining() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangling() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPacking() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplating() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikelyhood() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSupport() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressStability() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariadicness() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstraint() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasWeighting() const;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVariability() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatility() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomicity() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPrecondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPostcondition() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SimpleSymbol : public rq::Symbol {
  using Self = rq::SimpleSymbol;

  explicit RQ_ALWAYS_INLINE SimpleSymbol(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Literal : public rq::SimpleSymbol {
  using Self = rq::Literal;

  explicit RQ_ALWAYS_INLINE Literal(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct IntegerLiteral final : public rq::Literal {
  using Self = rq::IntegerLiteral;

  explicit RQ_ALWAYS_INLINE IntegerLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FloatLiteral final : public rq::Literal {
  using Self = rq::FloatLiteral;

  explicit RQ_ALWAYS_INLINE FloatLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct StringLiteral final : public rq::Literal {
  using Self = rq::StringLiteral;

  explicit RQ_ALWAYS_INLINE StringLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CodeunitLiteral final : public rq::Literal {
  using Self = rq::CodeunitLiteral;

  explicit RQ_ALWAYS_INLINE CodeunitLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Contextual : public rq::SimpleSymbol {
  using Self = rq::Contextual;

  explicit RQ_ALWAYS_INLINE Contextual(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ContextualName : public rq::Contextual {
  using Self = rq::ContextualName;

  explicit RQ_ALWAYS_INLINE ContextualName(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct NoName final : public rq::ContextualName {
  using Self = rq::NoName;

  explicit RQ_ALWAYS_INLINE NoName();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;

  explicit RQ_ALWAYS_INLINE ContextualValue(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct OutValue final : public rq::ContextualValue {
  using Self = rq::OutValue;

  explicit RQ_ALWAYS_INLINE OutValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ThisValue final : public rq::ContextualValue {
  using Self = rq::ThisValue;

  explicit RQ_ALWAYS_INLINE ThisValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ResultValue final : public rq::ContextualValue {
  using Self = rq::ResultValue;

  explicit RQ_ALWAYS_INLINE ResultValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ValueValue final : public rq::ContextualValue {
  using Self = rq::ValueValue;

  explicit RQ_ALWAYS_INLINE ValueValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct IndexValue final : public rq::ContextualValue {
  using Self = rq::IndexValue;

  explicit RQ_ALWAYS_INLINE IndexValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct DiscriminantValue final : public rq::ContextualValue {
  using Self = rq::DiscriminantValue;

  explicit RQ_ALWAYS_INLINE DiscriminantValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CommandLineArgumentsValue final : public rq::ContextualValue {
  using Self = rq::CommandLineArgumentsValue;

  explicit RQ_ALWAYS_INLINE CommandLineArgumentsValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CallsiteValue final : public rq::ContextualValue {
  using Self = rq::CallsiteValue;

  explicit RQ_ALWAYS_INLINE CallsiteValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ContextualType : public rq::Contextual {
  using Self = rq::ContextualType;

  explicit RQ_ALWAYS_INLINE ContextualType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InferenceType final : public rq::ContextualType {
  using Self = rq::InferenceType;

  explicit RQ_ALWAYS_INLINE InferenceType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VoidType final : public rq::ContextualType {
  using Self = rq::VoidType;

  explicit RQ_ALWAYS_INLINE VoidType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct NoReturnType final : public rq::ContextualType {
  using Self = rq::NoReturnType;

  explicit RQ_ALWAYS_INLINE NoReturnType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExpressionAttributeType : public rq::SimpleSymbol {
  using Self = rq::ExpressionAttributeType;

  explicit RQ_ALWAYS_INLINE ExpressionAttributeType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AnchoringType final : public rq::ExpressionAttributeType {
  using Self = rq::AnchoringType;

  explicit RQ_ALWAYS_INLINE AnchoringType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VisibilityType final : public rq::ExpressionAttributeType {
  using Self = rq::VisibilityType;

  explicit RQ_ALWAYS_INLINE VisibilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ScopingType final : public rq::ExpressionAttributeType {
  using Self = rq::ScopingType;

  explicit RQ_ALWAYS_INLINE ScopingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AvailabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AvailabilityType;

  explicit RQ_ALWAYS_INLINE AvailabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PropertyMutabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::PropertyMutabilityType;

  explicit RQ_ALWAYS_INLINE PropertyMutabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExportingType final : public rq::ExpressionAttributeType {
  using Self = rq::ExportingType;

  explicit RQ_ALWAYS_INLINE ExportingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GenerationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::GenerationTimeType;

  explicit RQ_ALWAYS_INLINE GenerationTimeType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CapturingType final : public rq::ExpressionAttributeType {
  using Self = rq::CapturingType;

  explicit RQ_ALWAYS_INLINE CapturingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct EvaluationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::EvaluationTimeType;

  explicit RQ_ALWAYS_INLINE EvaluationTimeType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InliningType final : public rq::ExpressionAttributeType {
  using Self = rq::InliningType;

  explicit RQ_ALWAYS_INLINE InliningType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ManglingType final : public rq::ExpressionAttributeType {
  using Self = rq::ManglingType;

  explicit RQ_ALWAYS_INLINE ManglingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PackingType final : public rq::ExpressionAttributeType {
  using Self = rq::PackingType;

  explicit RQ_ALWAYS_INLINE PackingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct TemplatingType final : public rq::ExpressionAttributeType {
  using Self = rq::TemplatingType;

  explicit RQ_ALWAYS_INLINE TemplatingType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LikelyhoodType final : public rq::ExpressionAttributeType {
  using Self = rq::LikelyhoodType;

  explicit RQ_ALWAYS_INLINE LikelyhoodType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SupportType final : public rq::ExpressionAttributeType {
  using Self = rq::SupportType;

  explicit RQ_ALWAYS_INLINE SupportType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AddressStabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AddressStabilityType;

  explicit RQ_ALWAYS_INLINE AddressStabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VariadicnessType final : public rq::ExpressionAttributeType {
  using Self = rq::VariadicnessType;

  explicit RQ_ALWAYS_INLINE VariadicnessType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct TypeAttributeType : public rq::SimpleSymbol {
  using Self = rq::TypeAttributeType;

  explicit RQ_ALWAYS_INLINE TypeAttributeType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VariabilityType final : public rq::TypeAttributeType {
  using Self = rq::VariabilityType;

  explicit RQ_ALWAYS_INLINE VariabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VolatilityType final : public rq::TypeAttributeType {
  using Self = rq::VolatilityType;

  explicit RQ_ALWAYS_INLINE VolatilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AtomicityType final : public rq::TypeAttributeType {
  using Self = rq::AtomicityType;

  explicit RQ_ALWAYS_INLINE AtomicityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct NullTerminationType final : public rq::TypeAttributeType {
  using Self = rq::NullTerminationType;

  explicit RQ_ALWAYS_INLINE NullTerminationType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PreconditionType final : public rq::TypeAttributeType {
  using Self = rq::PreconditionType;

  explicit RQ_ALWAYS_INLINE PreconditionType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PostconditionType final : public rq::TypeAttributeType {
  using Self = rq::PostconditionType;

  explicit RQ_ALWAYS_INLINE PostconditionType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ReflectiveType : public rq::SimpleSymbol {
  using Self = rq::ReflectiveType;

  explicit RQ_ALWAYS_INLINE ReflectiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SymbolType final : public rq::ReflectiveType {
  using Self = rq::SymbolType;

  explicit RQ_ALWAYS_INLINE SymbolType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExpressionType final : public rq::ReflectiveType {
  using Self = rq::ExpressionType;

  explicit RQ_ALWAYS_INLINE ExpressionType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PlatformPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::PlatformPrimitiveType;

  explicit RQ_ALWAYS_INLINE PlatformPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BooleanType final : public rq::PlatformPrimitiveType {
  using Self = rq::BooleanType;

  explicit RQ_ALWAYS_INLINE BooleanType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct HalfType final : public rq::PlatformPrimitiveType {
  using Self = rq::HalfType;

  explicit RQ_ALWAYS_INLINE HalfType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SingleType final : public rq::PlatformPrimitiveType {
  using Self = rq::SingleType;

  explicit RQ_ALWAYS_INLINE SingleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct DoubleType final : public rq::PlatformPrimitiveType {
  using Self = rq::DoubleType;

  explicit RQ_ALWAYS_INLINE DoubleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct QuadrupleType final : public rq::PlatformPrimitiveType {
  using Self = rq::QuadrupleType;

  explicit RQ_ALWAYS_INLINE QuadrupleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIntegerType;

  explicit RQ_ALWAYS_INLINE SignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE UnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIndexType;

  explicit RQ_ALWAYS_INLINE SignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIndexType;

  explicit RQ_ALWAYS_INLINE UnsignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedAddressType;

  explicit RQ_ALWAYS_INLINE SignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedAddressType;

  explicit RQ_ALWAYS_INLINE UnsignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CharType final : rq::PlatformPrimitiveType {
  using Self = rq::CharType;

  explicit RQ_ALWAYS_INLINE CharType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct StandardPrimitiveType : rq::SimpleSymbol {
  using Self = rq::StandardPrimitiveType;

  explicit RQ_ALWAYS_INLINE StandardPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary16Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary16Type;

  explicit RQ_ALWAYS_INLINE Binary16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary32Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary32Type;

  explicit RQ_ALWAYS_INLINE Binary32Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary64Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary64Type;

  explicit RQ_ALWAYS_INLINE Binary64Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary128Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary128Type;

  explicit RQ_ALWAYS_INLINE Binary128Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Bfloat16Type final : public rq::StandardPrimitiveType {
  using Self = rq::Bfloat16Type;

  explicit RQ_ALWAYS_INLINE Bfloat16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AsciiType final : public rq::StandardPrimitiveType {
  using Self = rq::AsciiType;

  explicit RQ_ALWAYS_INLINE AsciiType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Utf8Type final : public rq::StandardPrimitiveType {
  using Self = rq::Utf8Type;

  explicit RQ_ALWAYS_INLINE Utf8Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VariadicArgumentType final : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentType;

  explicit RQ_ALWAYS_INLINE VariadicArgumentType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

enum class ScaledIntegerKind { EXACT, FAST, LEAST };

struct ScaledPrimitiveType : public rq::Symbol {
  using Self = rq::ScaledPrimitiveType;

  rq::ScaledIntegerKind _kind;
  const rq::IntegerConstant *_scalar_ptr;
  std::uint64_t _synonym_id;

  explicit RQ_ALWAYS_INLINE
  ScaledPrimitiveType(rq::Opcode opcode, rq::ScaledIntegerKind kind,
                      const rq::IntegerConstant &scalar,
                      std::uint64_t synonym_id);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledIntegerKind getKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getSynonymId() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ScaledSignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledSignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledSignedIntegerType(rq::ScaledIntegerKind kind,
                          const rq::SymbolConstant &scalar,
                          std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ScaledUnsignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledUnsignedIntegerType(rq::ScaledIntegerKind kind,
                            const rq::SymbolConstant &scalar,
                            std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Subtype : public rq::SimpleSymbol {
  using Self = rq::Subtype;

  rq::SymbolConstant *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::Opcode opcode,
                                    rq::SymbolConstant &child);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ArraySubtype final : public rq::Subtype {
  using Self = rq::ArraySubtype;

  const rq::IntegerConstant *_count_ptr;

  explicit RQ_ALWAYS_INLINE ArraySubtype(rq::SymbolConstant &child,
                                         const rq::IntegerConstant &count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UncountedSubtype : public rq::Subtype {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::Opcode opcode,
                                             rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ReferenceSubtype final : public rq::UncountedSubtype {
  using Self = rq::ReferenceSubtype;

  explicit RQ_ALWAYS_INLINE ReferenceSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct PointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::PointerSubtype;

  explicit RQ_ALWAYS_INLINE PointerSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FatPointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::FatPointerSubtype;

  explicit RQ_ALWAYS_INLINE FatPointerSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE
  InferenceCountArraySubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

enum class ModuleKind : std::uint8_t { SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

struct ModuleFactory final {
  using Self = rq::ModuleFactory;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;

  explicit RQ_ALWAYS_INLINE ModuleFactory(rq::ModuleKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  RQ_ALWAYS_INLINE void setPath(llvm::StringRef path);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  RQ_ALWAYS_INLINE void setBuffer(llvm::MemoryBufferRef &&buffer);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::MemoryBufferRef &getBuffer();
};

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  const rq::Expression *_expression_ptr;
  llvm::StringRef path;
  llvm::MemoryBufferRef buffer;

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleFactory &factory);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &getBuffer() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Import final : public rq::Symbol {
  using Self = rq::Import;

  rq::ExpressionFlags _expression_flags;
  const rq::Expression *_expression_ptr;
  rq::Module *_imported_ptr;

  explicit RQ_ALWAYS_INLINE Import(rq::ExpressionFlags flags,
                                   const rq::Expression &expression,
                                   rq::Module &imported);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ConcatenatedString final : public rq::Symbol {
  using Self = rq::ConcatenatedString;

  llvm::ArrayRef<rq::SymbolConstant *> _children_ptrs;

  explicit RQ_ALWAYS_INLINE
  ConcatenatedString(llvm::ArrayRef<rq::SymbolConstant *> children_ptrs);

  [[nodiscard]] llvm::ArrayRef<const rq::SymbolConstant *> getChildren() const;
  [[nodiscard]] llvm::ArrayRef<rq::SymbolConstant *> getChildren();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ArithmeticSequence : public rq::Symbol {
  using Self = rq::ArithmeticSequence;

  rq::SymbolConstant *_child_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  explicit RQ_ALWAYS_INLINE
  ArithmeticSequence(rq::Opcode opcode, rq::SymbolConstant &child,
                     rq::ArithmeticSequenceCondition condition,
                     rq::ArithmeticSequenceStep step);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ArithmeticInterval final : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;

  explicit RQ_ALWAYS_INLINE
  ArithmeticInterval(rq::SymbolConstant &child,
                     rq::ArithmeticSequenceCondition condition);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InfiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE
  InfiniteArithmeticSequence(rq::SymbolConstant &child,
                             rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE
  FiniteArithmeticSequence(rq::SymbolConstant &child,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;

  llvm::StringRef _name;
  const rq::Expression *_name_expression_ptr;
  rq::SymbolTable *_containing_table_ptr;

  explicit RQ_ALWAYS_INLINE
  LocalDeclaration(rq::Opcode opcode, llvm::StringRef name,
                   const rq::Expression &name_expression,
                   rq::SymbolTable &containing_table);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getNameExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Label final : public rq::LocalDeclaration {
  using Self = rq::Label;

  rq::Instruction *_target_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Label(llvm::StringRef name,
                                  const rq::Expression &name_expression,
                                  rq::SymbolTable &containing_table,
                                  rq::Instruction &instruction);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
  getTargetInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getTargetInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Anchor final : public rq::LocalDeclaration {
  using Self = rq::Anchor;

  rq::LocalTable *_local_table_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Anchor(llvm::StringRef name,
                                   const rq::Expression &name_expression,
                                   rq::SymbolTable &containing_table,
                                   rq::LocalTable &local_table);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LocalTable &getLocalTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocalTable &getLocalTable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  rq::ExpressionFlags _expression_flags{};
  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalVariable(rq::Opcode opcode,
                                          llvm::StringRef name,
                                          const rq::Expression &name_expression,
                                          rq::SymbolTable &containing_table,
                                          rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  RQ_ALWAYS_INLINE void setType(rq::SymbolConstant &type);
  RQ_ALWAYS_INLINE void replaceType(rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  explicit RQ_ALWAYS_INLINE LocalDynamicVariable(
      llvm::StringRef name, const rq::Expression &name_expression,
      rq::SymbolTable &containing_table, rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;

  rq::SymbolicValue _value{};

  explicit RQ_ALWAYS_INLINE LocalStaticVariable(
      llvm::StringRef name, const rq::Expression &name_expression,
      rq::SymbolTable &containing_table, rq::ExpressionFlags flags);

  [[nodiscard]] const rq::SymbolicValue &getValue() const;
  [[nodiscard]] rq::SymbolicValue &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

enum class ParameterFlags : std::uint8_t {
  NONE = 0,

  POSITIONAL = rq::getBit(0),
  NAMED = rq::getBit(1),
  // UNSETABLE - not POSITIONAL and NAMED
};

template <> struct is_flags<rq::ParameterFlags> final : std::true_type {};

struct Parameter : public rq::LocalVariable {
  using Self = rq::Parameter;

  rq::Parameter *_next_parameter_ptr{nullptr};
  rq::ParameterFlags _parameter_flags{};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_default_value_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Parameter(
      rq::Opcode opcode, llvm::StringRef name,
      const rq::Expression &name_expression, rq::SymbolTable &containing_table,
      rq::ExpressionFlags expression_flags, rq::ParameterFlags parameter_flags,
      const rq::Expression &type_expression,
      const rq::Expression &default_value_expression);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamed() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocked() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getTypeExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getDefaultValueExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct StaticParameter final : public rq::Parameter {
  using Self = rq::StaticParameter;

  explicit RQ_ALWAYS_INLINE StaticParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct DynamicParameter final : public rq::Parameter {
  using Self = rq::DynamicParameter;

  explicit RQ_ALWAYS_INLINE DynamicParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ParameterListFactory final {
  using Self = ParameterListFactory;

  rq::ParameterFlags _state{rq::ParameterFlags::POSITIONAL};
  unsigned _parameter_count{0};
  unsigned _positional_pass_parameter_count{0};
  unsigned _named_pass_parameter_count{0};
  unsigned _unsettble_parameter_count{0};
  rq::Parameter *_first_parameter_ptr{nullptr};
  rq::Parameter *_last_parameter_ptr{nullptr};

  ParameterListFactory() = default;

  [[nodiscard]] inline bool markNamedBegin();
  [[nodiscard]] inline bool markPositionalEnd();
  [[nodiscard]] inline bool markLockedBegin();
  inline void addParameter(rq::Parameter &parameter);
};

struct ParameterIterator final {
  using Self = rq::ParameterIterator;
  using value_type = rq::Parameter;
  using reference = rq::Parameter &;
  using pointer = rq::Parameter *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Parameter *_parameter_ptr{nullptr};

  ParameterIterator() = default;
  explicit ParameterIterator(rq::Parameter *parameter_ptr);
  ParameterIterator(const Self &) = default;
  ParameterIterator(Self &&) = default;
  ~ParameterIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstParameterIterator final {
  using Self = rq::ConstParameterIterator;
  using value_type = const rq::Parameter;
  using reference = const rq::Parameter &;
  using pointer = rq::Parameter *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Parameter *_parameter_ptr{nullptr};

  ConstParameterIterator() = default;
  explicit ConstParameterIterator(const rq::Parameter *parameter_ptr);
  ConstParameterIterator(const Self &) = default;
  ConstParameterIterator(Self &&) = default;
  ~ConstParameterIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ParameterList : public rq::Symbol {
  using Self = rq::ParameterList;

  const rq::Expression *_expression_ptr{nullptr};
  unsigned _parameter_count{0};
  unsigned _positional_pass_parameter_count{0};
  unsigned _named_pass_parameter_count{0};
  unsigned _unsettble_parameter_count{0};
  rq::Parameter *_first_parameter_ptr{nullptr};
  llvm::ArrayRef<rq::Parameter *> _named_parameter_hash_table{};

  explicit inline ParameterList(rq::Opcode opcode,
                                rq::BumpPtrAllocator &allocator,
                                rq::ParameterListFactory &factory);

  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned
  getPositionalPassParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getNamedPassParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLockedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *
  getNamedParameter(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ParameterIterator, rq::ParameterIterator,
                            std::ranges::subrange_kind::unsized>
      getParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstParameterIterator,
                            rq::ConstParameterIterator,
                            std::ranges::subrange_kind::unsized>
      getParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Signature final : public rq::ParameterList {
  using Self = rq::Signature;

  rq::SymbolConstant *_return_type_ptr{nullptr};
  rq::SymbolConstant *_reciever_type_ptr{nullptr};
  const rq::Expression *_precondition_expression_ptr{nullptr};
  const rq::Expression *_postcondition_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Signature(rq::BumpPtrAllocator &allocator, rq::ParameterListFactory &factory,
            rq::SymbolConstant &return_type, rq::SymbolConstant &reciever_type,
            const rq::Expression &precondition_expression,
            const rq::Expression &postcondition_expression);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPreconditionExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPostconditionExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Layout final : public rq::ParameterList {
  using Self = rq::Layout;

  explicit RQ_ALWAYS_INLINE Layout();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Placement final : public rq::Symbol {
  using Self = rq::Placement;

  rq::Procedure *_procedure_ptr;

  explicit RQ_ALWAYS_INLINE Placement(rq::Procedure &procedure);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &getProcedure() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &getProcedure();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Composition final : public rq::Symbol {
  using Self = rq::Composition;

  rq::BumpPtrList<rq::Interface> _interface_list{};

  explicit RQ_ALWAYS_INLINE
  Composition(llvm::ArrayRef<rq::Interface *> interface_ptrs);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Synonym final : public rq::SimpleSymbol {
  using Self = rq::Synonym;

  rq::Symbol *_original_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Synonym(rq::Symbol &original);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getOriginal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getOriginal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
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
  explicit TemplateIterator(rq::Template *template_ptr);
  TemplateIterator(const Self &) = default;
  TemplateIterator(Self &&) = default;
  ~TemplateIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
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
  using value_type = const rq::Template;
  using reference = const rq::Template &;
  using pointer = rq::Template *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Template *_template_ptr{nullptr};

  ConstTemplateIterator() = default;
  explicit ConstTemplateIterator(const rq::Template *template_ptr);
  ConstTemplateIterator(const Self &) = default;
  ConstTemplateIterator(Self &&) = default;
  ~ConstTemplateIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct Weight final {
  using Self = rq::Weight;

  rq::Weight *_next_lower_ptr{nullptr};
  const rq::IntegerConstant *_weight{nullptr};
  rq::Template *_first_template{nullptr};

  explicit Weight() = default;
  Weight(const Self &) = delete;
  Weight(Self &&) = delete;
  ~Weight() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getWeight() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::TemplateIterator, rq::TemplateIterator,
                            std::ranges::subrange_kind::unsized>
      getTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstTemplateIterator,
                            rq::ConstTemplateIterator,
                            std::ranges::subrange_kind::unsized>
      getTemplateSubrange() const;
};

struct WeightIterator final {
  using Self = rq::WeightIterator;
  using value_type = rq::Weight;
  using reference = rq::Weight &;
  using pointer = rq::Weight *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Template *_template_ptr{nullptr};

  WeightIterator() = default;
  explicit WeightIterator(rq::Template *template_ptr);
  WeightIterator(const Self &) = default;
  WeightIterator(Self &&) = default;
  ~WeightIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Weight &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Weight &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Weight *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Weight *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstWeightIterator final {
  using Self = rq::ConstWeightIterator;
  using value_type = const rq::Weight;
  using reference = const rq::Weight &;
  using pointer = rq::Weight *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Template *_template_ptr{nullptr};

  ConstWeightIterator() = default;
  explicit ConstWeightIterator(const rq::Template *template_ptr);
  ConstWeightIterator(const Self &) = default;
  ConstWeightIterator(Self &&) = default;
  ~ConstWeightIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Weight &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Weight *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct Polymorph : public rq::Symbol {
  using Self = rq::Polymorph;

  rq::Weight *_highest_weight_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::Opcode opcode);
  void _addTemplate(rq::Template &template_);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::WeightIterator, rq::WeightIterator,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstWeightIterator, rq::ConstWeightIterator,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct RangerPolymorph final : public rq::Polymorph {
  using Self = rq::RangerPolymorph;

  rq::Ranger *_first_ranger_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE RangerPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ProcedurePolymorph final : public rq::Polymorph {
  using Self = rq::ProcedurePolymorph;

  rq::Procedure *_first_procedure_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcedurePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  rq::BumpPtrList<rq::Class> _class_list{};

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct EnumerationPolymorph final : public rq::Polymorph {
  using Self = rq::EnumerationPolymorph;

  rq::BumpPtrList<rq::Enumeration> _enumerator_list{};

  explicit RQ_ALWAYS_INLINE EnumerationPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  rq::BumpPtrList<rq::Interface> _interface_list{};

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;

  rq::BumpPtrList<rq::GlobalDynamicVariable> _global_dynamic_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariablePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;

  rq::BumpPtrList<rq::GlobalStaticVariable> _global_static_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariablePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;

  llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_member_map{};
  rq::BumpPtrList<rq::Symbol> _unamed_member_list{};
  rq::SymbolTable *_containing_table_ptr;

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::Opcode opcode,
                                        rq::SymbolTable *containing_table_ptr);

  RQ_ALWAYS_INLINE void release();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  inline void addNamedMember(llvm::StringRef name, rq::Symbol &symbol);
  RQ_ALWAYS_INLINE void addUnamedMember(rq::BumpPtrAllocator &allocator,
                                        rq::Symbol &symbol);
  [[nodiscard]] RQ_ALWAYS_INLINE const
      llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>> &
      getNamedMemberMap() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstBumpPtrListIterator<rq::Symbol>,
                            rq::ConstBumpPtrListIterator<rq::Symbol>,
                            std::ranges::subrange_kind::unsized>
      getUnamedSymbolsRange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::Symbol>,
                            rq::BumpPtrListIterator<rq::Symbol>,
                            std::ranges::subrange_kind::unsized>
      getUnamedSymbolsRange();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;

  explicit RQ_ALWAYS_INLINE Top();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalTable : public rq::SymbolTable {
  using Self = rq::LocalTable;

  rq::Expression *_expression_ptr;
  rq::ExpressionFlags _flags;

  explicit RQ_ALWAYS_INLINE LocalTable(rq::Opcode opcode,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct IfTable final : public rq::LocalTable {
  using Self = rq::IfTable;

  explicit RQ_ALWAYS_INLINE IfTable(rq::Expression &expression,
                                    rq::ExpressionFlags flags)
};

struct ElseIfTable final : public rq::LocalTable {
  using Self = rq::ElseIfTable;

  explicit RQ_ALWAYS_INLINE ElseIfTable(rq::Expression &expression,
                                        rq::ExpressionFlags flags)
};

struct ElseTable final : public rq::LocalTable {
  using Self = rq::ElseTable;

  explicit RQ_ALWAYS_INLINE ElseTable(rq::Expression &expression,
                                      rq::ExpressionFlags flags)
};

struct MatchTable final : public rq::LocalTable {
  using Self = rq::MatchTable;

  explicit RQ_ALWAYS_INLINE MatchTable(rq::Expression &expression,
                                       rq::ExpressionFlags flags)
};

struct InlineMatchTable final : public rq::LocalTable {
  using Self = rq::InlineMatchTable;

  explicit RQ_ALWAYS_INLINE InlineMatchTable(rq::Expression &expression,
                                             rq::ExpressionFlags flags)
};

struct SwitchTable final : public rq::LocalTable {
  using Self = rq::SwitchTable;

  explicit RQ_ALWAYS_INLINE SwitchTable(rq::Expression &expression,
                                        rq::ExpressionFlags flags)
};

struct InlineSwitchTable final : public rq::LocalTable {
  using Self = rq::InlineSwitchTable;

  explicit RQ_ALWAYS_INLINE InlineSwitchTable(rq::Expression &expression,
                                              rq::ExpressionFlags flags)
};

struct CaseTable final : public rq::LocalTable {
  using Self = rq::CaseTable;

  explicit RQ_ALWAYS_INLINE CaseTable(rq::Expression &expression,
                                      rq::ExpressionFlags flags)
};

struct WithTable final : public rq::LocalTable {
  using Self = rq::WithTable;

  explicit RQ_ALWAYS_INLINE WithTable(rq::Expression &expression,
                                      rq::ExpressionFlags flags)
};

struct DefaultTable final : public rq::LocalTable {
  using Self = rq::DefaultTable;

  explicit RQ_ALWAYS_INLINE DefaultTable(rq::Expression &expression,
                                         rq::ExpressionFlags flags)
};

struct ForTable final : public rq::LocalTable {
  using Self = rq::ForTable;

  explicit RQ_ALWAYS_INLINE ForTable(rq::Expression &expression,
                                     rq::ExpressionFlags flags)
};

struct WhileTable final : public rq::LocalTable {
  using Self = rq::WhileTable;

  explicit RQ_ALWAYS_INLINE WhileTable(rq::Expression &expression,
                                       rq::ExpressionFlags flags)
};

struct SpinTable final : public rq::LocalTable {
  using Self = rq::SpinTable;

  explicit RQ_ALWAYS_INLINE SpinTable(rq::Expression &expression,
                                      rq::ExpressionFlags flags)
};

struct WeaveTable final : public rq::LocalTable {
  using Self = rq::WeaveTable;

  explicit RQ_ALWAYS_INLINE WeaveTable(rq::Expression &expression,
                                       rq::ExpressionFlags flags)
};

struct ScopeTable final : public rq::LocalTable {
  using Self = rq::ScopeTable;

  explicit RQ_ALWAYS_INLINE ScopeTable(rq::Expression &expression,
                                       rq::ExpressionFlags flags)
};

struct InlineScopeTable final : public rq::LocalTable {
  using Self = rq::InlineScopeTable;

  explicit RQ_ALWAYS_INLINE InlineScopeTable(rq::Expression &expression,
                                             rq::SymbolTable &containing_table);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalDeclaration : public rq::SymbolTable {
  using Self = rq::GlobalDeclaration;

  llvm::StringRef _name;
  llvm::StringRef _mangled_name{};
  rq::SymbolTable *_hosting_table_ptr;
  const rq::Expression *_expression_ptr;
  const rq::Expression *_name_expression_ptr;
  rq::ExpressionFlags _flags;

  explicit RQ_ALWAYS_INLINE
  GlobalDeclaration(rq::Opcode opcode, rq::SymbolTable &containing_table,
                    llvm::StringRef name, rq::SymbolTable &hosting_table,
                    const rq::Expression *expression_ptr,
                    const rq::Expression *name_expression_ptr,
                    rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangledName() const;
  RQ_ALWAYS_INLINE void setMangledName(llvm::StringRef mangled_name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getHostingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHostingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNameExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getNameExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Namespace final : public rq::GlobalDeclaration {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Class final : public rq::GlobalDeclaration {
  using Self = rq::Class;

  rq::Layout *_class_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Class();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Enumeration final : public rq::GlobalDeclaration {
  using Self = rq::Enumeration;

  const rq::Expression *_underlying_expression_ptr{nullptr};
  rq::SymbolConstant *_underlying_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumeration();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Enumerator final : public rq::GlobalDeclaration {
  using Self = rq::Enumerator;

  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_default_value_expression_ptr{nullptr};
  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumerator();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Interface final : public rq::GlobalDeclaration {
  using Self = rq::Interface;

  explicit RQ_ALWAYS_INLINE Interface();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalVariable : public rq::GlobalDeclaration {
  using Self = rq::GlobalVariable;

  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalVariable(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalDynamicVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalDynamicVariable;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalStaticVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalStaticVariable;

  rq::Constant *_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Ranger : public rq::GlobalDeclaration {
  using Self = rq::Ranger;

  rq::Instruction *_instruction_ptr{nullptr};
  const rq::Expression *_reciever_expression_ptr;
  rq::SymbolConstant *_reciever_ptr;
  const rq::Expression *_element_expression_ptr;
  rq::SymbolConstant *_element_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Ranger(rq::Opcode opcode,
                                   const rq::Expression &reciever_expression,
                                   const rq::Expression &element_expression);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const;
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;

  explicit RQ_ALWAYS_INLINE ForwardRanger();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;

  explicit RQ_ALWAYS_INLINE BackwardRanger();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Destructor final : public rq::GlobalDeclaration {
  using Self = rq::Destructor;

  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Destructor();

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const;
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Procedure : public rq::GlobalDeclaration {
  using Self = rq::Procedure;

  rq::Signature *_signature_ptr{nullptr};
  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Procedure(rq::Opcode opcode);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSignature() const;
  RQ_ALWAYS_INLINE void setSignature(rq::Signature &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &getSignature() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature &getSignature();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const;
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Entry final : public rq::Procedure {
  using Self = rq::Entry;

  explicit RQ_ALWAYS_INLINE Entry();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Function final : public rq::Procedure {
  using Self = rq::Function;

  explicit RQ_ALWAYS_INLINE Function();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct MemberFunction final : public rq::Procedure {
  using Self = rq::MemberFunction;

  explicit RQ_ALWAYS_INLINE MemberFunction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Method final : public rq::Procedure {
  using Self = rq::Method;

  explicit RQ_ALWAYS_INLINE Method();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExtensionFunction final : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  rq::SymbolConstant *_extended_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ExtensionFunction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExtensionMethod final : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  explicit RQ_ALWAYS_INLINE ExtensionMethod();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Template : public rq::SymbolTable {
  using Self = rq::Template;

  rq::Template *_next_template_ptr{nullptr};
  rq::Layout *_template_layout{nullptr};
  const rq::Expression *_constraint_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Template(rq::Opcode opcode);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLayout() const;
  RQ_ALWAYS_INLINE void setLayout(rq::Layout &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getLayout() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getLayout();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstraintExpression() const;
  RQ_ALWAYS_INLINE void
  setConstraintExpression(const rq::Expression &expression);
  [[nodiscard]] const rq::Expression &getConstraintExpression() const;
  [[nodiscard]] rq::Expression &getConstraintExpression();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct EnumerationTemplate final : public rq::Template {
  using Self = rq::EnumerationTemplate;

  explicit RQ_ALWAYS_INLINE EnumerationTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ForwardRangerTemplate final : public rq::Template {
  using Self = rq::ForwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE ForwardRangerTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BackwardRangerTemplate final : public rq::Template {
  using Self = rq::BackwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE BackwardRangerTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct MemberFunctionTemplate final : public rq::Template {
  using Self = rq::MemberFunctionTemplate;

  explicit RQ_ALWAYS_INLINE MemberFunctionTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct MethodTemplate final : public rq::Template {
  using Self = rq::MethodTemplate;

  explicit RQ_ALWAYS_INLINE MethodTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExtensionFunctionTemplate final : public rq::Template {
  using Self = rq::ExtensionFunctionTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionFunctionTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExtensionMethodTemplate final : public rq::Template {
  using Self = rq::ExtensionMethodTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionMethodTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit RQ_ALWAYS_INLINE Constant(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;

  bool _is_platform_specific : 1;
  const llvm::APInt _data;

  explicit RQ_ALWAYS_INLINE IntegerConstant(const llvm::APInt &data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;

  bool _is_platform_specific : 1;
  const llvm::APFloat _data;

  explicit RQ_ALWAYS_INLINE FloatConstant(const llvm::APFloat &data,
                                          bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExpressionConstant final : public rq::Constant {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_expression_ptr;

  explicit RQ_ALWAYS_INLINE ExpressionConstant(const rq::Expression &data);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SymbolConstant final : public rq::Constant {
  using Self = rq::SymbolConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  explicit RQ_ALWAYS_INLINE SymbolConstant(rq::Symbol &symbol,
                                           rq::TypeFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _is_platform_specific : 1;
  bool _data : 1;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;

  bool _is_platform_specific : 1;
  llvm::StringRef _data;

  explicit RQ_ALWAYS_INLINE StringConstant(llvm::StringRef data,
                                           bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  llvm::ArrayRef<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data,
                                          bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant> getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExpressionAttributeConstant final : public rq::Constant {
  using Self = rq::ExpressionAttributeConstant;

  rq::ExpressionAttribute _attribute;

  explicit RQ_ALWAYS_INLINE ArrayConstant(rq::ExpressionAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct TypeAttributeConstant final : public rq::Constant {
  using Self = rq::TypeAttributeConstant;

  rq::TypeAttribute _attribute;

  explicit RQ_ALWAYS_INLINE ArrayConstant(rq::TypeAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression *_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        const rq::Expression *expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression getExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE
  NullaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr;

  explicit RQ_ALWAYS_INLINE
  UnaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr,
                   rq::Entity &address0);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity *_address0_ptr;
  rq::Entity *_address1_ptr;

  explicit RQ_ALWAYS_INLINE BinaryInstruction(rq::Opcode opcode,
                                              rq::Entity &address0,
                                              rq::Entity &address1);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress1() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress1();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

} // namespace rq

#include <rq/detail/entity.hpp>