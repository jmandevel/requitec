#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/next_iterator.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/Hashing.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallVector.h>
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
  SY_ANCHOR_TYPE,
  SY_OPAQUE_TYPE,
  SY_FLANK_TYPE,
  SY_GLOBAL_TYPE,
  SY_ACCESS_TYPE,
  SY_PARTIAL_MUTATE_TYPE,
  SY_STATIC_TYPE,
  SY_CAPTURE_TYPE,
  SY_INLINE_TYPE,
  SY_MANGLE_TYPE,
  SY_PACK_TYPE,
  SY_BRANCH_TREND_TYPE,
  SY_DEPRECIATE_TYPE,
  SY_STABLE_ADDRESS_TYPE,
  SY_VARIADIC_TYPE,
  SY_LOCATION_TYPE,
  SY_TEMPLATE_TYPE,
  SY_CONSTRAINT_TYPE,
  SY_WEIGHT_TYPE,
  SY_REQUIRE_TYPE,
  SY_ENSURE_TYPE,

  // TYPE ATTRIBUTES
  SY_VAR_TYPE,
  SY_VOLATILE_TYPE,
  SY_ATOMIC_TYPE,
  SY_NULL_TERMINATE_TYPE,

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

  // STANDARD PRIMITIVE
  SY_BINARY16_TYPE,
  SY_BINARY32_TYPE,
  SY_BINARY64_TYPE,
  SY_BINARY128_TYPE,
  SY_BFLOAT16_TYPE,
  SY_ASCII_TYPE,
  SY_UTF8_TYPE,

  // VARIADIC ARGUMENTS
  SY_VARIADIC_ARGUMENTS_TYPE_TYPE,

  // SCALED PRIMITIVES
  SY_SCALED_SIGNED_INTEGER_TYPE,
  SY_SCALED_UNSIGNED_INTEGER_TYPE,

  // SUBTYPES
  SY_ARRAY_SUBTYPE,

  // UNCOUNTED SUBTYPES => SUBTYPES
  SY_REFERENCE_SUBTYPE,
  SY_POINTER_SUBTYPE,
  SY_FAT_POINTER_SUBTYPE,
  SY_INFERENCE_COUNT_ARRAY_SUBTYPE,

  // MODULES
  SY_MODULE,

  // IMPORTS
  SY_IMPORT,

  // JUXTAPOSITIONAL LIST
  SY_CONCATINATED_LIST_TYPE,

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

  // SYMBOL PARAMETERS
  SY_SIGNATURE_PARAMETER,
  SY_LAYOUT_PARAMETER,

  // TYPE PARAMETERS
  SY_TUPLE_PARAMETER,
  SY_PROCEDURE_PARAMETER,

  // SYMBOL PARAMETER LISTS
  SY_SIGNATURE,
  SY_LAYOUT,

  // TYPE PARAMETER LISTS
  SY_TUPLE_TYPE,
  SY_PROCEDURE_TYPE,

  // PLACEMENTS
  SY_PLACEMENT_TYPE,

  // COMPOSITIONS
  SY_COMPOSITION_TYPE,

  // SYNONYMS
  SY_SYNONYM_TYPE,

  // SYMBOL TABLES
  SY_TOP,

  // LOCAL STATEMENTS => symbol table
  SY_IF_STATEMENT,
  SY_ELSE_IF_STATEMENT,
  SY_ELSE_STATEMENT,
  SY_MATCH_STATEMENT,
  SY_SWITCH_STATEMENT,
  SY_CASE_STATEMENT,
  SY_WITH_STATEMENT,
  SY_DEFAULT_STATEMENT,
  SY_FOR_STATEMENT,
  SY_WHILE_STATEMENT,
  SY_SPIN_STATEMENT,
  SY_WEAVE_STATEMENT,
  SY_SCOPE_STATEMENT,

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
  SY_METHOD,
  SY_EXTENSION_METHOD,

  // TEMPLATES
  SY_CLASS_TEMPLATE,
  SY_ENUMERATION_TEMPLATE,
  SY_INTERFACE_TEMPLATE,
  SY_GLOBAL_STATIC_VARIABLE_TEMPLATE,
  SY_FORWARD_RANGER_TEMPLATE,
  SY_BACKWARD_RANGER_TEMPLATE,
  SY_FUNCTION_TEMPLATE,
  SY_METHOD_TEMPLATE,
  SY_EXTENSION_METHOD_TEMPLATE,

  // POLYMORPHS
  SY_RANGER_POLYMORPH,
  SY_PROCEDURE_POLYMORPH,
  SY_CLASS_POLYMORPH,
  SY_ENUMERATION_POLYMORPH,
  SY_INTERFACE_POLYMORPH,
  SY_GLOBAL_STATIC_VARIABLE_POLYMORPH,

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
  SY_CONTEXTUAL_VALUE = rq::getBit(5),
  SY_CONTEXTUAL_TYPE = rq::getBit(6),
  SY_EXPRESSION_TYPE_ATTRIBUTE_TYPE = rq::getBit(7),
  SY_TYPE_ATTRIBUTE_TYPE = rq::getBit(8),
  SY_REFLECTIVE_TYPE = rq::getBit(9),
  SY_PLATFORM_PRIMITIVE_TYPE = rq::getBit(10),
  SY_STANDARD_PRIMITIVE_TYPE = rq::getBit(11),
  SY_SCALED_PRIMITIVE_TYPE = rq::getBit(12),
  SY_SUBTYPE = rq::getBit(13),
  SY_UNCOUNTED_SUBTYPE = rq::getBit(14),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(15),
  SY_LOCAL_DECLARATION = rq::getBit(16),
  SY_LOCAL_VARIABLE = rq::getBit(17),
  SY_SYMBOL_PARAMETER = rq::getBit(18),
  SY_SYMBOL_PARAMETER_LIST = rq::getBit(19),
  SY_TYPE_PARAMETER = rq::getBit(20),
  SY_TYPE_PARAMETER_LIST = rq::getBit(21),
  SY_SYMBOL_TYPE_TABLE = rq::getBit(22),
  SY_LOCAL_STATEMENT = rq::getBit(23),
  SY_GLOBAL_DECLARATION = rq::getBit(24),
  SY_GLOBAL_VARIABLE = rq::getBit(25),
  SY_RANGER = rq::getBit(26),
  SY_PROCEDURE = rq::getBit(27),
  SY_TEMPLATE = rq::getBit(28),
  SY_POLYMORPH = rq::getBit(29),

  // SYMBOL DETAILS
  SY_IS_TYPE = rq::getBit(30),
  SY_IS_SIGNED_TYPE = rq::getBit(31),
  SY_IS_UNSIGNED_TYPE = rq::getBit(32),
  SY_IS_INTEGER_TYPE = rq::getBit(33),
  SY_IS_FLOAT_TYPE = rq::getBit(34),
  SY_IS_BINARY_TYPE = rq::getBit(35),
  SY_IS_BFLOAT_TYPE = rq::getBit(36),
  SY_IS_CODEUNIT_TYPE = rq::getBit(37),
  SY_HAS_EXPRESSION_ATTRIBUTES = rq::getBit(38),
  SY_LOCAL_TABLE = rq::getBit(39)
};

template <> struct is_flags<rq::OpcodeFlags> final : std::true_type {};

[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode);

[[nodiscard]] inline rq::ExpressionFlags
getValidExpressionFlags(rq::Opcode opcode);

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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::Opcode opcode);
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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolParameter(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameter(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolParameterList(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameterList(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::Opcode opcode);
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
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionAttributes(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable(rq::Opcode opcode);

// clang-format off
struct Entity;
  struct Symbol;
    struct SimpleSymbol;
      struct LiteralType;
        struct IntegerLiteralType;
        struct FloatLiteralType;
        struct StringLiteralType;
        struct CodeunitLiteralType;
      struct Contextual;
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
        struct AnchorType;
        struct OpaquteType;
        struct FlankType;
        struct GlobalType;
        struct AccessType;
        struct PartialMutateType;
        struct StaticType;
        struct CaptureType;
        struct InlineType;
        struct MangleType;
        struct PackType;
        struct TemplateType;
        struct BranchTrendType;
        struct DepreciateType;
        struct StableAddressType;
        struct VariadicType;
        struct LocationType;
        struct RequireType;
        struct EnsureType;
      struct TypeAttributeType;
        struct VarType;
        struct VolatileType;
        struct AtomicType;
        struct NullTerminateType;
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
      struct VariadicArgumentsType;
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
    struct ConcatenatedList;
    struct ArithmeticSequenceType;
      struct ArithmeticIntervalType;
      struct InfiniteArithmeticSequenceType;
      struct FiniteArithmeticSequenceType;
    struct LocalDeclaration;
      struct Label;
      struct Anchor;
      struct LocalVariable;
        struct LocalDynamicVariable;
        struct LocalStaticVariable;
    struct Parameter;
      struct SymbolParameter;
        struct SignatureParameter;
        struct LayoutParameter;
      struct TypeParameter;
        struct ProcedureParameter;
        struct TupleParameter;
    struct ParameterList;
      struct SymbolParameterList;
        struct Signature;
        struct Layout;
      struct TypeParameterList;
        struct TupleType;
        struct ProcedureType;
    struct Placement;
    struct CompositionType;
    struct SynonymType;
    struct Polymorph;
      struct RangerPolymorph;
      struct ProcedurePolymorph;
      struct ClassPolymorph;
      struct EnumerationPolymorph;
      struct InterfacePolymorph;
      struct GlobalStaticVariablePolymorph;
    struct SymbolTable;
      struct Top;
      struct LocalStatement;
        struct IfStatement;
        struct ElseIfStatement;
        struct ElseStatement;
        struct MatchStatement;
        struct SwitchStatement;
        struct CaseStatement;
        struct WithStatement;
        struct DefaultStatement;
        struct ForStatement;
        struct WhileStatement;
        struct SpinStatement;
        struct WeaveStatement;
        struct ScopeStatement;
      struct GlobalDeclaration;
        struct Namespace;
        struct ClassType;
        struct EnumerationType;
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
          struct Method;
          struct ExtensionMethod;
        struct Template;
          struct ClassTemplate;
          struct EnumerationTemplate;
          struct InterfaceTemplate;
          struct GlobalStaticVariableTemplate;
          struct ForwardRangerTemplate;
          struct BackwardRangerTemplate;
          struct FunctionTemplate;
          struct MethodTemplate;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getOpcodeFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpressionAttributes() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SimpleSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SimpleSymbol;

  explicit RQ_ALWAYS_INLINE SimpleSymbol(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileSimpleSymbol(llvm::FoldingSetNodeID &id,
                                          rq::Opcode opcode);

struct LiteralType : public rq::SimpleSymbol {
  using Self = rq::LiteralType;

  explicit RQ_ALWAYS_INLINE LiteralType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IntegerLiteralType final : public rq::LiteralType {
  using Self = rq::IntegerLiteralType;

  explicit RQ_ALWAYS_INLINE IntegerLiteralType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FloatLiteralType final : public rq::LiteralType {
  using Self = rq::FloatLiteralType;

  explicit RQ_ALWAYS_INLINE FloatLiteralType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StringLiteralType final : public rq::LiteralType {
  using Self = rq::StringLiteralType;

  explicit RQ_ALWAYS_INLINE StringLiteralType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CodeunitLiteralType final : public rq::LiteralType {
  using Self = rq::CodeunitLiteralType;

  explicit RQ_ALWAYS_INLINE CodeunitLiteralType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Contextual : public rq::SimpleSymbol {
  using Self = rq::Contextual;

  explicit RQ_ALWAYS_INLINE Contextual(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;

  explicit RQ_ALWAYS_INLINE ContextualValue(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct OutValue final : public rq::ContextualValue {
  using Self = rq::OutValue;

  explicit RQ_ALWAYS_INLINE OutValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ThisValue final : public rq::ContextualValue {
  using Self = rq::ThisValue;

  explicit RQ_ALWAYS_INLINE ThisValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ResultValue final : public rq::ContextualValue {
  using Self = rq::ResultValue;

  explicit RQ_ALWAYS_INLINE ResultValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ValueValue final : public rq::ContextualValue {
  using Self = rq::ValueValue;

  explicit RQ_ALWAYS_INLINE ValueValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IndexValue final : public rq::ContextualValue {
  using Self = rq::IndexValue;

  explicit RQ_ALWAYS_INLINE IndexValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DiscriminantValue final : public rq::ContextualValue {
  using Self = rq::DiscriminantValue;

  explicit RQ_ALWAYS_INLINE DiscriminantValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CommandLineArgumentsValue final : public rq::ContextualValue {
  using Self = rq::CommandLineArgumentsValue;

  explicit RQ_ALWAYS_INLINE CommandLineArgumentsValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CallsiteValue final : public rq::ContextualValue {
  using Self = rq::CallsiteValue;

  explicit RQ_ALWAYS_INLINE CallsiteValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContextualType : public rq::Contextual {
  using Self = rq::ContextualType;

  explicit RQ_ALWAYS_INLINE ContextualType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InferenceType final : public rq::ContextualType {
  using Self = rq::InferenceType;

  explicit RQ_ALWAYS_INLINE InferenceType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VoidType final : public rq::ContextualType {
  using Self = rq::VoidType;

  explicit RQ_ALWAYS_INLINE VoidType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NoReturnType final : public rq::ContextualType {
  using Self = rq::NoReturnType;

  explicit RQ_ALWAYS_INLINE NoReturnType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExpressionAttributeType : public rq::SimpleSymbol {
  using Self = rq::ExpressionAttributeType;

  explicit RQ_ALWAYS_INLINE ExpressionAttributeType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AnchorType final : public rq::ExpressionAttributeType {
  using Self = rq::AnchorType;

  explicit RQ_ALWAYS_INLINE AnchorType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct OpaqueType final : public rq::ExpressionAttributeType {
  using Self = rq::OpaqueType;

  explicit RQ_ALWAYS_INLINE OpaqueType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FlankType final : public rq::ExpressionAttributeType {
  using Self = rq::FlankType;

  explicit RQ_ALWAYS_INLINE FlankType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalType final : public rq::ExpressionAttributeType {
  using Self = rq::GlobalType;

  explicit RQ_ALWAYS_INLINE GlobalType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AccessType final : public rq::ExpressionAttributeType {
  using Self = rq::AccessType;

  explicit RQ_ALWAYS_INLINE AccessType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PartialMutateType final : public rq::ExpressionAttributeType {
  using Self = rq::PartialMutateType;

  explicit RQ_ALWAYS_INLINE PartialMutateType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StaticType final : public rq::ExpressionAttributeType {
  using Self = rq::StaticType;

  explicit RQ_ALWAYS_INLINE StaticType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CaptureType final : public rq::ExpressionAttributeType {
  using Self = rq::CaptureType;

  explicit RQ_ALWAYS_INLINE CaptureType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InlineType final : public rq::ExpressionAttributeType {
  using Self = rq::InlineType;

  explicit RQ_ALWAYS_INLINE InlineType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MangleType final : public rq::ExpressionAttributeType {
  using Self = rq::MangleType;

  explicit RQ_ALWAYS_INLINE MangleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PackType final : public rq::ExpressionAttributeType {
  using Self = rq::PackType;

  explicit RQ_ALWAYS_INLINE PackType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BranchTrendType final : public rq::ExpressionAttributeType {
  using Self = rq::BranchTrendType;

  explicit RQ_ALWAYS_INLINE BranchTrendType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DepreciateType final : public rq::ExpressionAttributeType {
  using Self = rq::DepreciateType;

  explicit RQ_ALWAYS_INLINE DepreciateType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StableAddressType final : public rq::ExpressionAttributeType {
  using Self = rq::StableAddressType;

  explicit RQ_ALWAYS_INLINE StableAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VariadicType final : public rq::ExpressionAttributeType {
  using Self = rq::VariadicType;

  explicit RQ_ALWAYS_INLINE VariadicType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocationType final : public rq::ExpressionAttributeType {
  using Self = rq::LocationType;

  explicit RQ_ALWAYS_INLINE LocationType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TemplateType final : public rq::ExpressionAttributeType {
  using Self = rq::TemplateType;

  explicit RQ_ALWAYS_INLINE TemplateType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstraintType final : public rq::ExpressionAttributeType {
  using Self = rq::ConstraintType;

  explicit RQ_ALWAYS_INLINE ConstraintType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WeightType final : public rq::ExpressionAttributeType {
  using Self = rq::WeightType;

  explicit RQ_ALWAYS_INLINE WeightType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct RequireType final : public rq::ExpressionAttributeType {
  using Self = rq::RequireType;

  explicit RQ_ALWAYS_INLINE RequireType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnsureType final : public rq::ExpressionAttributeType {
  using Self = rq::EnsureType;

  explicit RQ_ALWAYS_INLINE EnsureType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TypeAttributeType : public rq::SimpleSymbol {
  using Self = rq::TypeAttributeType;

  explicit RQ_ALWAYS_INLINE TypeAttributeType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VarType final : public rq::TypeAttributeType {
  using Self = rq::VarType;

  explicit RQ_ALWAYS_INLINE VarType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VolatileType final : public rq::TypeAttributeType {
  using Self = rq::VolatileType;

  explicit RQ_ALWAYS_INLINE VolatileType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AtomicType : public rq::TypeAttributeType {
  using Self = rq::AtomicType;

  explicit RQ_ALWAYS_INLINE AtomicType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NullTerminateType final : public rq::TypeAttributeType {
  using Self = rq::NullTerminateType;

  explicit RQ_ALWAYS_INLINE NullTerminateType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ReflectiveType : public rq::SimpleSymbol {
  using Self = rq::ReflectiveType;

  explicit RQ_ALWAYS_INLINE ReflectiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolType final : public rq::ReflectiveType {
  using Self = rq::SymbolType;

  explicit RQ_ALWAYS_INLINE SymbolType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExpressionType final : public rq::ReflectiveType {
  using Self = rq::ExpressionType;

  explicit RQ_ALWAYS_INLINE ExpressionType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PlatformPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::PlatformPrimitiveType;

  explicit RQ_ALWAYS_INLINE PlatformPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BooleanType final : public rq::PlatformPrimitiveType {
  using Self = rq::BooleanType;

  explicit RQ_ALWAYS_INLINE BooleanType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct HalfType final : public rq::PlatformPrimitiveType {
  using Self = rq::HalfType;

  explicit RQ_ALWAYS_INLINE HalfType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SingleType final : public rq::PlatformPrimitiveType {
  using Self = rq::SingleType;

  explicit RQ_ALWAYS_INLINE SingleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DoubleType final : public rq::PlatformPrimitiveType {
  using Self = rq::DoubleType;

  explicit RQ_ALWAYS_INLINE DoubleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct QuadrupleType final : public rq::PlatformPrimitiveType {
  using Self = rq::QuadrupleType;

  explicit RQ_ALWAYS_INLINE QuadrupleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIntegerType;

  explicit RQ_ALWAYS_INLINE SignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE UnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIndexType;

  explicit RQ_ALWAYS_INLINE SignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIndexType;

  explicit RQ_ALWAYS_INLINE UnsignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedAddressType;

  explicit RQ_ALWAYS_INLINE SignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedAddressType;

  explicit RQ_ALWAYS_INLINE UnsignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StandardPrimitiveType : rq::SimpleSymbol {
  using Self = rq::StandardPrimitiveType;

  explicit RQ_ALWAYS_INLINE StandardPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Binary16Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary16Type;

  explicit RQ_ALWAYS_INLINE Binary16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Binary32Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary32Type;

  explicit RQ_ALWAYS_INLINE Binary32Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Binary64Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary64Type;

  explicit RQ_ALWAYS_INLINE Binary64Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Binary128Type final : public rq::StandardPrimitiveType {
  using Self = rq::Binary128Type;

  explicit RQ_ALWAYS_INLINE Binary128Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Bfloat16Type final : public rq::StandardPrimitiveType {
  using Self = rq::Bfloat16Type;

  explicit RQ_ALWAYS_INLINE Bfloat16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AsciiType final : public rq::StandardPrimitiveType {
  using Self = rq::AsciiType;

  explicit RQ_ALWAYS_INLINE AsciiType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Utf8Type final : public rq::StandardPrimitiveType {
  using Self = rq::Utf8Type;

  explicit RQ_ALWAYS_INLINE Utf8Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VariadicArgumentsType final : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentsType;

  explicit RQ_ALWAYS_INLINE VariadicArgumentsType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ScaledIntegerKind { EXACT, FAST, LEAST };

struct ScaledPrimitiveType : public rq::Symbol, public llvm::FoldingSetNode {
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
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getSynonymTypeId() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileScaledPrimitiveType(
    llvm::FoldingSetNodeID &id, rq::ScaledIntegerKind kind,
    const rq::IntegerConstant &scalar, std::uint64_t synonum_id);

struct ScaledSignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledSignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledSignedIntegerType(rq::ScaledIntegerKind kind,
                          const rq::IntegerConstant &scalar,
                          std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ScaledUnsignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledUnsignedIntegerType(rq::ScaledIntegerKind kind,
                            const rq::IntegerConstant &scalar,
                            std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Subtype : public rq::Symbol {
  using Self = rq::Subtype;

  rq::SymbolConstant *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::Opcode opcode,
                                    rq::SymbolConstant &child);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArraySubtype final : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::ArraySubtype;

  const rq::IntegerConstant *_count_ptr;

  explicit RQ_ALWAYS_INLINE ArraySubtype(rq::SymbolConstant &child,
                                         const rq::IntegerConstant &count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &id,
                                          const rq::SymbolConstant &child,
                                          const rq::IntegerConstant &count);

struct UncountedSubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::Opcode opcode,
                                             rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &id,
                                              const rq::SymbolConstant &child);

struct ReferenceSubtype final : public rq::UncountedSubtype {
  using Self = rq::ReferenceSubtype;

  explicit RQ_ALWAYS_INLINE ReferenceSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::PointerSubtype;

  explicit RQ_ALWAYS_INLINE PointerSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FatPointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::FatPointerSubtype;

  explicit RQ_ALWAYS_INLINE FatPointerSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE
  InferenceCountArraySubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ModuleKind : std::uint8_t { SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

struct ModuleFactory final {
  using Self = rq::ModuleFactory;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr{nullptr};
  llvm::StringRef _path{};
  llvm::MemoryBufferRef _buffer{};

  explicit RQ_ALWAYS_INLINE ModuleFactory(rq::ModuleKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getKind() const;
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
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleFactory &factory);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &getBuffer() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConcatenatedListItem final : public llvm::FoldingSetNode {
  using Self = rq::ConcatenatedListItem;
};

struct ConcatenatedList final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::ConcatenatedList;

  rq::ConcatenatedListItem *_first_item_ptr;

  explicit RQ_ALWAYS_INLINE
  ConcatenatedList(rq::ConcatenatedListItem &first_item);
  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileConcatenatedList(llvm::FoldingSetNodeID &id,
                        const rq::ConcatenatedListItem &first_item);

struct ArithmeticSequence : public rq::Symbol, llvm::FoldingSetNode {
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileArithmeticSequence(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                          const rq::SymbolConstant &child,
                          rq::ArithmeticSequenceCondition condition,
                          rq::ArithmeticSequenceStep step);

struct ArithmeticInterval final : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;

  explicit RQ_ALWAYS_INLINE
  ArithmeticInterval(rq::SymbolConstant &child,
                     rq::ArithmeticSequenceCondition condition);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InfiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE
  InfiniteArithmeticSequence(rq::SymbolConstant &child,
                             rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE
  FiniteArithmeticSequence(rq::SymbolConstant &child,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;

  llvm::StringRef _name;
  const rq::Expression *_name_expression_ptr;
  rq::SymbolTable *_containing_table_ptr;
  rq::SymbolTable *_hosting_table_ptr;

  explicit RQ_ALWAYS_INLINE
  LocalDeclaration(rq::Opcode opcode, llvm::StringRef name,
                   const rq::Expression *name_expression_ptr,
                   rq::SymbolTable &containing_table,
                   rq::SymbolTable &hosting_table);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNameExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getNameExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getHostingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHostingTable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Label final : public rq::LocalDeclaration {
  using Self = rq::Label;

  rq::Instruction *_target_instruction_ptr;

  explicit RQ_ALWAYS_INLINE Label(llvm::StringRef name,
                                  const rq::Expression &name_expression,
                                  rq::SymbolTable &containing_table,
                                  rq::Instruction &instruction);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
  getTargetInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getTargetInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Anchor final : public rq::LocalDeclaration {
  using Self = rq::Anchor;

  rq::LocalStatement *_local_table_ptr;

  explicit RQ_ALWAYS_INLINE Anchor(llvm::StringRef name,
                                   const rq::Expression &name_expression,
                                   rq::SymbolTable &containing_table,
                                   rq::LocalStatement &local_table);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LocalStatement &
  getLocalStatement() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LocalStatement &getLocalStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  rq::ExpressionFlags _expression_flags;
  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  LocalVariable(rq::Opcode opcode, llvm::StringRef name,
                const rq::Expression *name_expression_ptr,
                rq::SymbolTable &containing_table,
                rq::SymbolTable &hosting_table, rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  RQ_ALWAYS_INLINE void setType(rq::SymbolConstant &type);
  RQ_ALWAYS_INLINE void replaceType(rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  explicit RQ_ALWAYS_INLINE LocalDynamicVariable(
      llvm::StringRef name, const rq::Expression &name_expression,
      rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
      rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;

  rq::SymbolicValue _value{};

  explicit RQ_ALWAYS_INLINE LocalStaticVariable(
      llvm::StringRef name, const rq::Expression &name_expression,
      rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
      rq::ExpressionFlags flags);

  [[nodiscard]] const rq::SymbolicValue &getValue() const;
  [[nodiscard]] rq::SymbolicValue &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Parameter : public rq::Symbol {
  using Self = rq::Parameter;

  rq::Parameter *_next_ptr;
  llvm::StringRef _name;
  rq::SymbolConstant *_type_ptr;

  explicit Parameter(rq::Opcode opcode, rq::Parameter *next_ptr,
                     llvm::StringRef name, rq::SymbolConstant &type);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNextParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter &getNextParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter &getNextParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolParameter : public rq::Parameter {
  using Self = rq::SymbolParameter;

  bool _is_positional : 1;
  bool _is_nonpositional : 1;
  bool _is_locked : 1;
  rq::ExpressionFlags _expression_flags;
  rq::SymbolTable *_containing_table_ptr;
  rq::SymbolTable *_hosting_table_ptr;
  const rq::Expression *_expression_ptr;
  const rq::Expression *_name_expression_ptr;
  const rq::Expression *_type_expression_ptr;
  const rq::Expression *_default_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE SymbolParameter(
      rq::Opcode opcode, rq::SymbolParameter *next_ptr, llvm::StringRef name,
      rq::SymbolConstant &type, rq::SymbolTable &containing_table,
      rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
      bool is_positional, bool is_nonpositional, bool is_locked,
      const rq::Expression &expression, const rq::Expression &name_expression,
      const rq::Expression &type_expression,
      const rq::Expression *default_value_expression_ptr);

  [[nodiscard]] const rq::SymbolParameter &getNextSymbolParameter() const;
  [[nodiscard]] rq::SymbolParameter &getNextSymbolParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNonpositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocked() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getHostingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHostingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getNameExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getTypeExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDefaultValueExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getDefaultValueExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignatureParameter final : public rq::SymbolParameter {
  using Self = rq::SignatureParameter;

  explicit RQ_ALWAYS_INLINE SignatureParameter(
      rq::SymbolParameter *next_ptr, llvm::StringRef name,
      rq::SymbolConstant &type, rq::SymbolTable &containing_table,
      rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
      bool is_positional, bool is_nonpositional, bool is_locked,
      const rq::Expression &expression, const rq::Expression &name_expression,
      const rq::Expression &type_expression,
      const rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter &
  getNextSignatureParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter &
  getNextSignatureParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LayoutParameter final : public rq::SymbolParameter {
  using Self = rq::LayoutParameter;

  explicit RQ_ALWAYS_INLINE LayoutParameter(
      rq::SymbolParameter *next_ptr, llvm::StringRef name,
      rq::SymbolConstant &type, rq::SymbolTable &containing_table,
      rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
      bool is_positional, bool is_nonpositional, bool is_locked,
      const rq::Expression &expression, const rq::Expression &name_expression,
      const rq::Expression &type_expression,
      const rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter &
  getNextLayoutParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter &getNextLayoutParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TypeParameter : public rq::Parameter, public llvm::FoldingSetNode {
  using Self = rq::TypeParameter;

  unsigned _location;
  bool _is_positional : 1;

  explicit RQ_ALWAYS_INLINE
  TypeParameter(rq::Opcode opcode, rq::TypeParameter *next_ptr,
                llvm::StringRef name, rq::SymbolConstant &type,
                unsigned location, bool is_positional);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter &
  getNextTypeParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter &getNextTypeParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLocation() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNonpositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositionPassable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypePassable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamePassable() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileTypeParameter(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                     llvm::StringRef name, const rq::SymbolConstant &type,
                     unsigned location, bool is_positional);

struct ProcedureParameter final : public rq::TypeParameter {
  using Self = rq::TupleParameter;

  explicit RQ_ALWAYS_INLINE ProcedureParameter(rq::TypeParameter *next_ptr,
                                               llvm::StringRef name,
                                               rq::SymbolConstant &type,
                                               unsigned location);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TupleParameter final : public rq::TypeParameter {
  using Self = rq::TupleParameter;

  explicit RQ_ALWAYS_INLINE TupleParameter(rq::TypeParameter *next_ptr,
                                           llvm::StringRef name,
                                           rq::SymbolConstant &type,
                                           unsigned location,
                                           bool is_positional);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ParameterList : public rq::Symbol {
  using Self = rq::ParameterList;

  rq::Parameter *_first_parameter_ptr;
  unsigned _parameter_count;
  unsigned _positional_parameter_count;
  unsigned _nonpositional_parameter_count;

  explicit RQ_ALWAYS_INLINE
  ParameterList(rq::Opcode opcode, rq::Parameter *first_parameter_ptr,
                unsigned parameter_count, unsigned positional_parameter_count,
                unsigned nonpositional_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasFirstParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter &getFirstParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter &getFirstParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getPositionalParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned
  getNonpositionalParameterCount() const;
  [[nodiscard]] inline const rq::Parameter *
  getParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] inline rq::Parameter *
  getParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Parameter>,
                            rq::NextIterator<rq::Parameter>,
                            std::ranges::subrange_kind::unsized>
      getParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Parameter>,
                            rq::ConstNextIterator<rq::Parameter>,
                            std::ranges::subrange_kind::unsized>
      getParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolParameterInfo final {
  using Self = rq::SymbolParameterInfo;

  llvm::StringRef _name;
  rq::SymbolConstant *_type_ptr;
  const rq::Expression *_name_expression_ptr;
  rq::ExpressionFlags _expression_flags;
  bool _is_positional : 1;
  bool _is_nonpositional : 1;
  bool _is_locked : 1;
  const rq::Expression *_type_expression_ptr;
  const rq::Expression *_default_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  SymbolParameterInfo(llvm::StringRef name, rq::SymbolConstant &type,
                      const rq::Expression *name_expression_ptr,
                      rq::ExpressionFlags expression_flags, bool is_positional,
                      bool is_nonpositional, bool is_locked,
                      const rq::Expression &type_expression,
                      const rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getNameExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNonpositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocked() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getTypeExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDefaultValueExpressionPtr() const;
};

struct SymbolParameterListFactory final {
  using Self = SymbolParameterListFactory;

  rq::SymbolTable *_containing_table_ptr;
  rq::SymbolTable *_hosting_table_ptr;
  bool _is_positional : 1 {true};
  bool _is_nonpositional : 1 {false};
  bool _is_locked : 1 {false};
  llvm::SmallVector<rq::SymbolParameterInfo> _infos{};

  explicit SymbolParameterListFactory(rq::SymbolTable &containing_table,
                                      rq::SymbolTable &hosting_table);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHostingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNonpositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocked() const;
  RQ_ALWAYS_INLINE void markPositionalEnd();
  RQ_ALWAYS_INLINE void markNonpositionalBegin();
  RQ_ALWAYS_INLINE void markLockedBegin();
  RQ_ALWAYS_INLINE void
  addParameter(llvm::StringRef name, const rq::Expression *name_expression_ptr,
               rq::ExpressionFlags expression_flags,
               const rq::Expression &type_expression,
               const rq::Expression *default_value_expression_ptr);
};

struct SymbolParameterList : public rq::ParameterList {
  using Self = rq::SymbolParameterList;

  unsigned _locked_parameter_count;

  explicit RQ_ALWAYS_INLINE SymbolParameterList(
      rq::Opcode opcode, rq::SymbolParameter *first_parameter_ptr,
      unsigned parameter_count, unsigned positional_parameter_count,
      unsigned nonpositional_parameter_count, unsigned locked_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLockedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter &
  getFirstSymbolParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter &getFirstSymbolParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
  getSymbolParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
  getSymbolParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
      rq::NextIterator<rq::Parameter, rq::SymbolParameter>,
      std::ranges::subrange_kind::unsized>
  getSymbolParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::SymbolParameter>,
      std::ranges::subrange_kind::unsized>
  getSymbolParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Signature final : public rq::SymbolParameterList {
  using Self = rq::Signature;

  rq::SymbolConstant *_return_type_ptr;
  rq::SymbolConstant *_reciever_type_ptr;
  const rq::Expression *_precondition_expression_ptr;
  const rq::Expression *_postcondition_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  Signature(rq::SignatureParameter *first_parameter_ptr,
            unsigned parameter_count, unsigned positional_parameter_count,
            unsigned nonpositional_parameter_count,
            unsigned locked_parameter_count, rq::SymbolConstant &return_type,
            rq::SymbolConstant *reciever_type_ptr,
            const rq::Expression *precondition_expression_ptr,
            const rq::Expression *postcondition_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRecieverType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getRecieverType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getRecieverType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPreconditionExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPreconditionExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPostconditionExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPostconditionExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter &
  getFirstSignatureParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter &
  getFirstSignatureParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
  getSignatureParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
  getSignatureParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
      rq::NextIterator<rq::Parameter, rq::SignatureParameter>,
      std::ranges::subrange_kind::unsized>
  getSignatureParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::SignatureParameter>,
      std::ranges::subrange_kind::unsized>
  getSignatureParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Layout final : public rq::SymbolParameterList {
  using Self = rq::Layout;

  explicit RQ_ALWAYS_INLINE Layout(rq::SymbolParameter *first_parameter_ptr,
                                   unsigned parameter_count,
                                   unsigned positional_parameter_count,
                                   unsigned nonpositional_parameter_count,
                                   unsigned locked_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter &
  getFirstLayoutParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter &getFirstLayoutParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
  getLayoutParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
  getLayoutParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
      rq::NextIterator<rq::Parameter, rq::LayoutParameter>,
      std::ranges::subrange_kind::unsized>
  getLayoutParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::LayoutParameter>,
      std::ranges::subrange_kind::unsized>
  getLayoutParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TypeParameterList : public rq::ParameterList {
  using Self = rq::TypeParameterList;

  explicit RQ_ALWAYS_INLINE
  TypeParameterList(rq::Opcode opcode, rq::TypeParameter *first_parameter_ptr,
                    unsigned parameter_count,
                    unsigned positional_parameter_count,
                    unsigned nonpositional_parameter_count);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter &
  getFirstTypeParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter &getFirstTypeParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
  getTypeParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *
  getTypeParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::TypeParameter>,
                            rq::NextIterator<rq::Parameter, rq::TypeParameter>,
                            std::ranges::subrange_kind::unsized>
      getTypeParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::TypeParameter>,
      std::ranges::subrange_kind::unsized>
  getTypeParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedureType final : rq::TypeParameterList,
                             public llvm::FoldingSetNode {
  using Self = rq::ProcedureType;

  rq::SymbolConstant *_return_type_ptr;
  rq::SymbolConstant *_reciever_type_ptr;

  explicit RQ_ALWAYS_INLINE
  ProcedureType(rq::TypeParameter *first_parameter_ptr,
                unsigned parameter_count, unsigned positional_parameter_count,
                unsigned nonpositional_parameter_count,
                rq::SymbolConstant &return_type,
                rq::SymbolConstant *reciever_type_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter &
  getFirstProcedureParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter &
  getFirstProcedureParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter *
  getProcedureParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter *
  getProcedureParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
      rq::NextIterator<rq::Parameter, rq::ProcedureParameter>,
      std::ranges::subrange_kind::unsized>
  getProcedureParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::ProcedureParameter>,
      std::ranges::subrange_kind::unsized>
  getProcedureParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id, rq::Opcode opcode,
                      const rq::ProcedureParameter *first_parameter_ptr,
                      const rq::SymbolConstant &return_type,
                      const rq::SymbolConstant *reciever_type_ptr) const;
};

inline void
profileProcedureType(llvm::FoldingSetNodeID &id,
                     const rq::ProcedureParameter *first_parameter_ptr,
                     const rq::SymbolConstant &return_type,
                     const rq::SymbolConstant *reciever_type_ptr);

struct TupleType final : rq::TypeParameterList, public llvm::FoldingSetNode {
  using Self = rq::TupleType;

  unsigned _type_keyed_parameter_count;

  explicit RQ_ALWAYS_INLINE TupleType(rq::TypeParameter *first_parameter_ptr,
                                      unsigned parameter_count,
                                      unsigned positional_parameter_count,
                                      unsigned nonpositional_parameter_count,
                                      unsigned type_keyed_parameter_count);
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getNameKeyedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getTypeKeyedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TupleParameter &
  getFirstTupleParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter &getFirstTupleParameter();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TupleParameter *
  getTupleParameterPtrOfName(llvm::StringRef name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter *
  getTupleParameterPtrOfName(llvm::StringRef name);
  [[nodiscard]] inline const rq::TupleParameter *
  getTupleParameterPtrOfType(const rq::SymbolConstant &type) const;
  [[nodiscard]] inline rq::TupleParameter *
  getTupleParameterPtrOfType(const rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Parameter, rq::TupleParameter>,
                            rq::NextIterator<rq::Parameter, rq::TupleParameter>,
                            std::ranges::subrange_kind::unsized>
      getTupleParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
      rq::ConstNextIterator<rq::Parameter, rq::TupleParameter>,
      std::ranges::subrange_kind::unsized>
  getTupleParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &id,
                 const rq::TupleParameter *first_parameter_ptr);

struct Placement final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::Placement;

  rq::Procedure *_procedure_ptr;

  explicit RQ_ALWAYS_INLINE Placement(rq::Procedure &procedure);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &getProcedure() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &getProcedure();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &id,
                                       const rq::Procedure &procedure);

// TODO composition factory

struct CompositionComponent final : llvm::FoldingSetNode {
  using Self = rq::CompositionComponent;

  rq::Interface *_interface_ptr;
  rq::CompositionComponent *_next_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionComponent(rq::Interface &interface,
                       rq::CompositionComponent *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface &getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Interface &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNextComponent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent &
  getNextComponent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent &getNextComponent();

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

inline void
profileCompositionComponent(llvm::FoldingSetNodeID &id,
                            const rq::Interface &interface,
                            const rq::CompositionComponent *next_component_ptr);

struct CompositionType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::CompositionType;

  rq::CompositionComponent *_first_component_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionType(rq::CompositionComponent &first_component);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent &
  getFirstComponent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent &getFirstComponent();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::CompositionComponent>,
                            rq::NextIterator<rq::CompositionComponent>,
                            std::ranges::subrange_kind::unsized>
      getComponentSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::CompositionComponent>,
                            rq::ConstNextIterator<rq::CompositionComponent>,
                            std::ranges::subrange_kind::unsized>
      getComponentSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileComposition(llvm::FoldingSetNodeID &id,
                   const rq::CompositionComponent &first_component);

struct SynonymType final : public rq::Symbol {
  using Self = rq::SynonymType;

  rq::Symbol *_original_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE SynonymType(rq::Symbol &original);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getOriginal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getOriginal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;

  llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_member_map{};
  rq::BumpPtrList<rq::Symbol> _unamed_member_list{};
  rq::SymbolTable *_containing_table_ptr;

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::Opcode opcode,
                                        rq::SymbolTable *containing_table_ptr);

  inline void release();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  inline void addNamedMember(rq::BumpPtrAllocator &allocator,
                             llvm::StringRef name, rq::Symbol &symbol);
  RQ_ALWAYS_INLINE void addUnamedMember(rq::BumpPtrAllocator &allocator,
                                        rq::Symbol &symbol);
  [[nodiscard]] RQ_ALWAYS_INLINE const
      llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>> &
      getNamedMemberMap() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListRef<rq::Symbol>
  getUnamedMemberList() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
  getUnamedMemberList();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;

  explicit RQ_ALWAYS_INLINE Top();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalStatement : public rq::SymbolTable {
  using Self = rq::LocalStatement;

  rq::Expression *_expression_ptr;
  rq::ExpressionFlags _flags;

  explicit RQ_ALWAYS_INLINE LocalStatement(rq::Opcode opcode,
                                           rq::SymbolTable &containing_table,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IfStatement final : public rq::LocalStatement {
  using Self = rq::IfStatement;

  explicit RQ_ALWAYS_INLINE IfStatement(rq::SymbolTable &containing_table,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ElseIfStatement final : public rq::LocalStatement {
  using Self = rq::ElseIfStatement;

  explicit RQ_ALWAYS_INLINE ElseIfStatement(rq::SymbolTable &containing_table,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ElseStatement final : public rq::LocalStatement {
  using Self = rq::ElseStatement;

  explicit RQ_ALWAYS_INLINE ElseStatement(rq::SymbolTable &containing_table,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MatchStatement final : public rq::LocalStatement {
  using Self = rq::MatchStatement;

  explicit RQ_ALWAYS_INLINE MatchStatement(rq::SymbolTable &containing_table,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SwitchStatement final : public rq::LocalStatement {
  using Self = rq::SwitchStatement;

  explicit RQ_ALWAYS_INLINE SwitchStatement(rq::SymbolTable &containing_table,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CaseStatement final : public rq::LocalStatement {
  using Self = rq::CaseStatement;

  explicit RQ_ALWAYS_INLINE CaseStatement(rq::SymbolTable &containing_table,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WithStatement final : public rq::LocalStatement {
  using Self = rq::WithStatement;

  explicit RQ_ALWAYS_INLINE WithStatement(rq::SymbolTable &containing_table,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DefaultStatement final : public rq::LocalStatement {
  using Self = rq::DefaultStatement;

  explicit RQ_ALWAYS_INLINE DefaultStatement(rq::SymbolTable &containing_table,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ForStatement final : public rq::LocalStatement {
  using Self = rq::ForStatement;

  explicit RQ_ALWAYS_INLINE ForStatement(rq::SymbolTable &containing_table,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WhileStatement final : public rq::LocalStatement {
  using Self = rq::WhileStatement;

  explicit RQ_ALWAYS_INLINE WhileStatement(rq::SymbolTable &containing_table,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SpinStatement final : public rq::LocalStatement {
  using Self = rq::SpinStatement;

  explicit RQ_ALWAYS_INLINE SpinStatement(rq::SymbolTable &containing_table,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WeaveStatement final : public rq::LocalStatement {
  using Self = rq::WeaveStatement;

  explicit RQ_ALWAYS_INLINE WeaveStatement(rq::SymbolTable &containing_table,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ScopeStatement final : public rq::LocalStatement {
  using Self = rq::ScopeStatement;

  explicit RQ_ALWAYS_INLINE ScopeStatement(rq::SymbolTable &containing_table,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Namespace final : public rq::GlobalDeclaration {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Class final : public rq::GlobalDeclaration {
  using Self = rq::Class;

  rq::Layout *_class_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Class();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Enumeration final : public rq::GlobalDeclaration {
  using Self = rq::Enumeration;

  const rq::Expression *_underlying_expression_ptr{nullptr};
  rq::SymbolConstant *_underlying_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumeration();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Enumerator final : public rq::GlobalDeclaration {
  using Self = rq::Enumerator;

  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_default_value_expression_ptr{nullptr};
  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumerator();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Interface final : public rq::GlobalDeclaration {
  using Self = rq::Interface;

  explicit RQ_ALWAYS_INLINE Interface();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalVariable : public rq::GlobalDeclaration {
  using Self = rq::GlobalVariable;

  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalVariable(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalDynamicVariable;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalStaticVariable;

  rq::Constant *_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;

  explicit RQ_ALWAYS_INLINE ForwardRanger();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;

  explicit RQ_ALWAYS_INLINE BackwardRanger();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Destructor final : public rq::GlobalDeclaration {
  using Self = rq::Destructor;

  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Destructor();

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const;
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Entry final : public rq::Procedure {
  using Self = rq::Entry;

  explicit RQ_ALWAYS_INLINE Entry();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Function final : public rq::Procedure {
  using Self = rq::Function;

  explicit RQ_ALWAYS_INLINE Function();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MemberFunction final : public rq::Procedure {
  using Self = rq::MemberFunction;

  explicit RQ_ALWAYS_INLINE MemberFunction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Method final : public rq::Procedure {
  using Self = rq::Method;

  explicit RQ_ALWAYS_INLINE Method();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExtensionMethod final : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  explicit RQ_ALWAYS_INLINE ExtensionMethod();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Template : public rq::SymbolTable {
  using Self = rq::Template;

  rq::Template *_next_ptr{nullptr};
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationTemplate final : public rq::Template {
  using Self = rq::EnumerationTemplate;

  explicit RQ_ALWAYS_INLINE EnumerationTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ForwardRangerTemplate final : public rq::Template {
  using Self = rq::ForwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE ForwardRangerTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BackwardRangerTemplate final : public rq::Template {
  using Self = rq::BackwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE BackwardRangerTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MemberFunctionTemplate final : public rq::Template {
  using Self = rq::MemberFunctionTemplate;

  explicit RQ_ALWAYS_INLINE MemberFunctionTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MethodTemplate final : public rq::Template {
  using Self = rq::MethodTemplate;

  explicit RQ_ALWAYS_INLINE MethodTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExtensionMethodTemplate final : public rq::Template {
  using Self = rq::ExtensionMethodTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionMethodTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit RQ_ALWAYS_INLINE Constant(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Weight final {
  using Self = rq::Weight;

  rq::Weight *_next_ptr{nullptr};
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
      std::ranges::subrange<rq::NextIterator<rq::Template>,
                            rq::NextIterator<rq::Template>,
                            std::ranges::subrange_kind::unsized>
      getTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Template>,
                            rq::ConstNextIterator<rq::Template>,
                            std::ranges::subrange_kind::unsized>
      getTemplateSubrange() const;
};

struct Polymorph : public rq::Symbol {
  using Self = rq::Polymorph;

  rq::Weight *_highest_weight_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::Opcode opcode);
  void _addTemplate(rq::Template &template_);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Weight>,
                            rq::NextIterator<rq::Weight>,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Weight>,
                            rq::ConstNextIterator<rq::Weight>,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct RangerPolymorph final : public rq::Polymorph {
  using Self = rq::RangerPolymorph;

  rq::Ranger *_first_ranger_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE RangerPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedurePolymorph final : public rq::Polymorph {
  using Self = rq::ProcedurePolymorph;

  rq::Procedure *_first_procedure_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcedurePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  rq::BumpPtrList<rq::ClassType> _class_list{};

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationPolymorph final : public rq::Polymorph {
  using Self = rq::EnumerationPolymorph;

  rq::BumpPtrList<rq::EnumerationType> _enumerator_list{};

  explicit RQ_ALWAYS_INLINE EnumerationPolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  rq::BumpPtrList<rq::Interface> _interface_list{};

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;

  rq::BumpPtrList<rq::GlobalDynamicVariable> _global_dynamic_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariablePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;

  rq::BumpPtrList<rq::GlobalStaticVariable> _global_static_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariablePolymorph();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;

  bool _is_platform_specific : 1;
  const llvm::APInt _data;

  explicit RQ_ALWAYS_INLINE IntegerConstant(const llvm::APInt &data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APInt &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;

  bool _is_platform_specific : 1;
  const llvm::APFloat _data;

  explicit RQ_ALWAYS_INLINE FloatConstant(const llvm::APFloat &data,
                                          bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::APFloat &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExpressionConstant final : public rq::Constant {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_expression_ptr;

  explicit RQ_ALWAYS_INLINE ExpressionConstant(const rq::Expression &data);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _is_platform_specific : 1;
  bool _data : 1;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data,
                                            bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;

  bool _is_platform_specific : 1;
  llvm::StringRef _data;

  explicit RQ_ALWAYS_INLINE StringConstant(llvm::StringRef data,
                                           bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformSpecific() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  llvm::ArrayRef<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data,
                                          bool is_platform_specific);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::ArrayRef<rq::Constant> getData() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExpressionAttributeConstant final : public rq::Constant {
  using Self = rq::ExpressionAttributeConstant;

  rq::ExpressionAttribute _attribute;

  explicit RQ_ALWAYS_INLINE
  ExpressionAttributeConstant(rq::ExpressionAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TypeAttributeConstant final : public rq::Constant {
  using Self = rq::TypeAttributeConstant;

  rq::TypeAttribute _attribute;

  explicit RQ_ALWAYS_INLINE TypeAttributeConstant(rq::TypeAttribute attribute);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getAttribute() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression *_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode,
                                        const rq::Expression *expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression getExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE
  NullaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr;

  explicit RQ_ALWAYS_INLINE
  UnaryInstruction(rq::Opcode opcode, const rq::Expression *expression_ptr,
                   rq::Entity &address0);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/entity.hpp>