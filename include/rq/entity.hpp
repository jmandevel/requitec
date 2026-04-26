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

namespace rq {

enum class Opcode {
  NONE,

  // LITERALS
  SY_INTEGER_LITERAL,
  SY_FLOAT_LITERAL,
  SY_STRING_LITERAL,
  SY_CODEUNIT_LITERAL,

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
  SY_INFERENCE,
  SY_VOID,
  SY_NO_RETURN,

  // EXPRESSION ATTRIBUTES
  SY_LABELING,
  SY_VISIBILITY,
  SY_SCOPING,
  SY_AVAILABILITY,
  SY_PROPERTY_MUTABILITY,
  SY_EXPORTING,
  SY_GENERATION_TIME,
  SY_CAPTURING,
  SY_EVALUATION_TIME,
  SY_INLINING,
  SY_MANGLING,
  SY_PACKING,
  SY_TEMPLATING,
  SY_LIKELYHOOD,
  SY_SUPPORT,
  SY_COPYABILITY,
  SY_ADDRESS_STABILITY,
  SY_CLEANUP,
  SY_VARIADICNESS,

  // TYPE ATTRIBUTES
  SY_MUTABILITY,
  SY_VOLATILITY,
  SY_INITIALIZATION_REQUIREMENT,
  SY_ATOMICITY,
  SY_NULL_TERMINATION,
  SY_PRECONDITION,
  SY_POSTCONDITION,

  // REFLECTIVE
  SY_SYMBOL,
  SY_EXPRESSION,

  // UNSCALED PRIMITIVE
  SY_BOOLEAN,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_ASCII,
  SY_UTF8,
  SY_SIGNED_INTEGER,
  SY_UNSIGNED_INTEGER,
  SY_SIGNED_INDEX,
  SY_UNSIGNED_INDEX,
  SY_SIGNED_ADDRESS,
  SY_UNSIGNED_ADDRESS,

  // VARIADIC ARGUMENTS
  SY_VARIADIC_ARGUMENTS,

  // SCALED INTEGERS
  SY_SCALED_SIGNED_INTEGER,
  SY_SCALED_UNSIGNED_INTEGER,

  // UNCOUNTED SUBTYPES
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCE_COUNT_ARRAY,

  // ARRAY
  SY_ARRAY,

  // MODULES
  SY_MODULE,

  // IMPORTS
  SY_IMPORT,

  // CONCATENATED STRING
  SY_CONCATENATED_STRING,

  // ARITHMETIC SEQUENCES
  SY_ARITHMETIC_INTERVAL,
  SY_INFINITE_ARITHMETIC_SEQUENCE,
  SY_FINITE_ARITHMETIC_SEQUENCE,

  // LOCAL VARIABLES
  SY_LOCAL_DYNAMIC_VARIABLE,
  SY_LOCAL_STATIC_VARIABLE,

  // PARAMETERS => local variable
  SY_STATIC_PARAMETER,
  SY_DYNAMIC_PARAMETER,

  // PARAMETER LISTS
  SY_SIGNATURE,
  SY_LAYOUT,

  // LABELS
  SY_LABEL,

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
  SY_SCOPE,

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

  // PROCEDURES => global declaration => symbol table
  SY_ENTRY,
  SY_FUNCTION,
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
  SY_METHOD_TEMPLATE,
  SY_EXTENSION_FUNCTION_TEMPLATE,
  SY_EXTENSION_METHOD_TEMPLATE,

  CT_INTEGER,
  CT_FLOAT,
  CT_EXPRESSION,
  CT_SYMBOL,
  CT_BOOLEAN,
  CT_STRING,
  CT_ARRAY

  // TODO instructions
};

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode);

enum class OpcodeFlags : std::uint64_t {
  NONE = 0,

  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  // INSTRUCTION - not SYMBOL orB CONSTANT

  // ==SYMBOL FLAGS==
  // SYMBOL PARENTS
  SY_SIMPLE_SYMBOL = rq::getBit(2),
  SY_LITERAL = rq::getBit(3),
  SY_CONTEXTUAL = rq::getBit(4),
  SY_CONTEXTUAL_NAME = rq::getBit(5),
  SY_CONTEXTUAL_VALUE = rq::getBit(6),
  SY_CONTEXTUAL_TYPE = rq::getBit(7),
  SY_EXPRESSION_ATTRIBUTE_TYPE = rq::getBit(8),
  SY_TYPE_ATTRIBUTE_TYPE = rq::getBit(9),
  SY_REFLECTIVE_TYPE = rq::getBit(10),
  SY_UNSCALED_PRIMITIVE_TYPE = rq::getBit(11),
  SY_SCALED_INTEGER_TYPE = rq::getBit(12),
  SY_UNCOUNTED_SUBTYPE = rq::getBit(13),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(14),
  SY_LOCAL_DECLARATION = rq::getBit(15),
  SY_LOCAL_VARIABLE = rq::getBit(16),
  SY_PARAMETER = rq::getBit(18),
  SY_PARAMETER_LIST = rq::getBit(19),
  SY_POLYMORPH = rq::getBit(20),
  SY_SYMBOL_TABLE = rq::getBit(21),
  SY_GLOBAL_DECLARATION = rq::getBit(22),
  SY_GLOBAL_VARIABLE = rq::getBit(23),
  SY_RANGER = rq::getBit(24),
  SY_PROCEDURE = rq::getBit(25),
  SY_TEMPLATE = rq::getBit(26),

  // SYMBOL DETAILS
  SY_IS_TYPE = rq::getBit(27),
  SY_IS_SIGNED_TYPE = rq::getBit(28),
  SY_IS_UNSIGNED_TYPE = rq::getBit(29),
  SY_IS_INTEGER_TYPE = rq::getBit(30),
  SY_IS_FLOAT_TYPE = rq::getBit(31),
  SY_IS_BINARY_TYPE = rq::getBit(32),
  SY_IS_BFLOAT_TYPE = rq::getBit(33),
  SY_IS_STRING_TYPE = rq::getBit(34),
  SY_IS_CODEUNIT_TYPE = rq::getBit(35),
  SY_IS_EXPRESSION_ATTRIBUTE_TYPE = rq::getBit(36),
  SY_IS_TYPE_ATTRIBUTE_TYPE = rq::getBit(37),

  // CONSTANT FLAGS

  // INSTRUCTION FLAGS

};

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode);

#define RQ_ASSERT_SYMBOL(opcode)                                               \
  RQ_ASSERT(rq::getIsSymbol(opcode), "not symbol");
#define RQ_ASSERT_CONSTANT(opcode)                                             \
  RQ_ASSERT(rq::getIsConstant(opcode), "not constant");
#define RQ_ASSERT_INSTRUCTION(opcode)                                          \
  RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction");

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getFlags(rq::Opcode opcode);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttributeType(rq::Opcode opcode);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttributeType(rq::Opcode opcode);

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
        struct LabelingType;
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
        struct CopyabilityType;
        struct AddressStabilityType;
        struct CleanupType;
        struct VariadicnessType;
      struct TypeAttributeType;
        struct MutabilityType;
        struct VolatilityType;
        struct Initialization_RequirementType;
        struct AtomicityType;
        struct NullTerminationType;
        struct PreconditionType;
        struct PostconditionType;
      struct ReflectiveType;
        struct SymbolType;
        struct ExpressionType;
      struct UnscaledPrimitiveType;
        struct BooleanType;
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
        struct SignedIntegerType;
        struct UnsignedIntegerType;
        struct SignedIndexType;
        struct UnsignedIndexType;
        struct SignedAddressType;
        struct UnsignedAddressType;
      struct VariadicArgumentType;
    struct ScaledIntegerType;
      struct ScaledSignedIntegerType;
      struct ScaledUnsignedIntegerType;
    struct UncountedSubtype;
      struct ReferenceSubtype;
      struct PointerSubtype;
      struct FatPointerSubtype;
      struct InferenceCountArraySubtype;
    struct Array;
    struct Module;
    struct Import;
    struct ConcatenatedString;
    struct ArithmeticSequence;
      struct ArithmeticInterval;
      struct InfiniteArithmeticSequence;
      struct FiniteArithmeticSequence;
    struct LocalDeclaration;
      struct Label;
      struct LocalVariable;
        struct LocalDynamicVariable;
        struct LocalStaticVariable;
        struct Parameter;
          struct StaticParameter;
          struct DynamicParameter;
    struct ParameterList;
      struct Signature;
      struct Layout;
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
      struct Scope;
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
        struct Procedure;
          struct Entry;
          struct Function;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionAttributeType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttributeType() const;

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

struct LabelingType final : public rq::ExpressionAttributeType {
  using Self = rq::LabelingType;

  explicit RQ_ALWAYS_INLINE LabelingType();

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

struct CopyabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::CopyabilityType;

  explicit RQ_ALWAYS_INLINE CopyabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AddressStabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AddressStabilityType;

  explicit RQ_ALWAYS_INLINE AddressStabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct CleanupType final : public rq::ExpressionAttributeType {
  using Self = rq::CleanupType;

  explicit RQ_ALWAYS_INLINE CleanupType();

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

struct MutabilityType final : public rq::TypeAttributeType {
  using Self = rq::MutabilityType;

  explicit RQ_ALWAYS_INLINE MutabilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VolatilityType final : public rq::TypeAttributeType {
  using Self = rq::VolatilityType;

  explicit RQ_ALWAYS_INLINE VolatilityType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Initialization_RequirementType final : public rq::TypeAttributeType {
  using Self = rq::Initialization_RequirementType;

  explicit RQ_ALWAYS_INLINE Initialization_RequirementType();

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

struct UnscaledPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::UnscaledPrimitiveType;

  explicit RQ_ALWAYS_INLINE UnscaledPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BooleanType final : public rq::UnscaledPrimitiveType {
  using Self = rq::BooleanType;

  explicit RQ_ALWAYS_INLINE BooleanType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct HalfType final : public rq::UnscaledPrimitiveType {
  using Self = rq::HalfType;

  explicit RQ_ALWAYS_INLINE HalfType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SingleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SingleType;

  explicit RQ_ALWAYS_INLINE SingleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct DoubleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::DoubleType;

  explicit RQ_ALWAYS_INLINE DoubleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct QuadrupleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::QuadrupleType;

  explicit RQ_ALWAYS_INLINE QuadrupleType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary16Type;

  explicit RQ_ALWAYS_INLINE Binary16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary32Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary32Type;

  explicit RQ_ALWAYS_INLINE Binary32Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary64Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary64Type;

  explicit RQ_ALWAYS_INLINE Binary64Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Binary128Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary128Type;

  explicit RQ_ALWAYS_INLINE Binary128Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Bfloat16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Bfloat16Type;

  explicit RQ_ALWAYS_INLINE Bfloat16Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct AsciiType final : public rq::UnscaledPrimitiveType {
  using Self = rq::AsciiType;

  explicit RQ_ALWAYS_INLINE AsciiType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Utf8Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Utf8Type;

  explicit RQ_ALWAYS_INLINE Utf8Type();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedIntegerType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedIntegerType;

  explicit RQ_ALWAYS_INLINE SignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedIntegerType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE UnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedIndexType;

  explicit RQ_ALWAYS_INLINE SignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedIndexType;

  explicit RQ_ALWAYS_INLINE UnsignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedAddressType;

  explicit RQ_ALWAYS_INLINE SignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnsignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedAddressType;

  explicit RQ_ALWAYS_INLINE UnsignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct VariadicArgumentType final : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentType;

  explicit RQ_ALWAYS_INLINE VariadicArgumentType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

enum class ScaledIntegerKind { EXACT, FAST, LEAST };

struct ScaledIntegerType : public rq::Symbol {
  using Self = rq::ScaledIntegerType;

  rq::ScaledIntegerKind _kind;
  const rq::IntegerConstant *_scalar_ptr;
  std::uint64_t _synonym_id;

  explicit RQ_ALWAYS_INLINE ScaledIntegerType(rq::Opcode opcode,
                                              rq::ScaledIntegerKind kind,
                                              const rq::IntegerConstant &scalar,
                                              std::uint64_t synonym_id);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledIntegerKind getKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getScalar() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getSynonymId() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ScaledSignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledSignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledSignedIntegerType(rq::ScaledIntegerKind kind,
                          const rq::SymbolConstant &scalar,
                          std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ScaledUnsignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE
  ScaledUnsignedIntegerType(rq::ScaledIntegerKind kind,
                            const rq::SymbolConstant &scalar,
                            std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UncountedSubtype : public rq::SimpleSymbol {
  using Self = rq::UncountedSubtype;

  rq::SymbolConstant *_child_ptr;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::Opcode opcode,
                                             rq::SymbolConstant &child);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();

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

struct Array final : public rq::Symbol {
  using Self = rq::Array;

  rq::SymbolConstant *_child_ptr;
  const rq::IntegerConstant *_count_ptr;

  explicit RQ_ALWAYS_INLINE Array(rq::SymbolConstant &child,
                                  const rq::IntegerConstant &count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::IntegerConstant &getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

enum class ModuleKind : std::uint8_t { SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

struct ModuleFactory final {
  using Self = rq::ModuleFactory;

  rq::ModuleKind _module_kind;
  rq::Expression *_snippet_ptr;
  llvm::StringRef path;
  llvm::MemoryBufferRef buffer;
};

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  const rq::Expression *_snippet_ptr;
  llvm::StringRef path;
  llvm::MemoryBufferRef buffer;

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleFactory &factory);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getSnippet() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::MemoryBufferRef &getBuffer() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Import final : public rq::Symbol {
  using Self = rq::Import;

  rq::ExpressionFlags _expression_flags;
  rq::Module *_imported_ptr;

  explicit RQ_ALWAYS_INLINE Import(rq::ExpressionFlags flags,
                                   rq::Module &imported);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getFlags() const;
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
  const rq::Expression *_name_snippet_ptr;
  rq::SymbolTable *_containing_table_ptr;

  explicit RQ_ALWAYS_INLINE LocalDeclaration(rq::Opcode opcode,
                                             llvm::StringRef name,
                                             const rq::Expression &name_snippet,
                                             rq::SymbolTable &containing_table);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getNameSnippet() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Label final : public rq::LocalDeclaration {
  using Self = rq::Label;

  rq::Symbol *_target_symbol_ptr{nullptr};
  rq::Instruction *_target_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Label(llvm::StringRef name,
                                  const rq::Expression &name_snippet,
                                  rq::SymbolTable &containing_table);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTargetSymbol() const;
  RQ_ALWAYS_INLINE void setTargetSymbol(rq::Symbol &symbol);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getTargetSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getTargetSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTargetInstruction() const;
  RQ_ALWAYS_INLINE void setTargetInstruction(rq::Instruction &target);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &
  getTargetInstruction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getTargetInstruction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  rq::ExpressionFlags _expression_flags{};
  rq::SymbolConstant *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalVariable(rq::Opcode opcode,
                                          llvm::StringRef name,
                                          const rq::Expression &name_snippet,
                                          rq::SymbolTable &containing_table,
                                          rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const;
  RQ_ALWAYS_INLINE void setType(rq::SymbolConstant &type);
  RQ_ALWAYS_INLINE void replaceType(rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  explicit RQ_ALWAYS_INLINE
  LocalDynamicVariable(llvm::StringRef name, const rq::Expression &name_snippet,
                       rq::SymbolTable &containing_table,
                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;

  rq::SymbolicValue _value{};

  explicit RQ_ALWAYS_INLINE
  LocalStaticVariable(llvm::StringRef name, const rq::Expression &name_snippet,
                      rq::SymbolTable &containing_table,
                      rq::ExpressionFlags flags);

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
  const rq::Expression *_type_snippet_ptr{nullptr};
  const rq::Expression *_default_value_snippet_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Parameter(
      rq::Opcode opcode, llvm::StringRef name,
      const rq::Expression &name_snippet, rq::SymbolTable &containing_table,
      rq::ExpressionFlags expression_flags, rq::ParameterFlags parameter_flags,
      const rq::Expression &type_snippet,
      const rq::Expression &default_value_snippet);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamed() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsettable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getTypeSnippet() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getDefaultValueSnippet() const;

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

  ParameterListFactory() = default;

  inline void markNamedBegin();
  inline void markPositionalEnd();
  inline void markUnsettableBegin();
  inline void addParameter(rq::Parameter &parameter);
};

struct ParameterList : public rq::Symbol {
  using Self = rq::ParameterList;

  const rq::Expression *_snippet_ptr{nullptr};
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
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getUnsettableParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *
  getNamedParameter(llvm::StringRef name);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Signature final : public rq::ParameterList {
  using Self = rq::Signature;

  rq::SymbolConstant *_return_type_ptr{nullptr};
  rq::SymbolConstant *_reciever_type_ptr{nullptr};
  const rq::Expression *_precondition_snippet_ptr{nullptr};
  const rq::Expression *_postcondition_snippet_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Signature(rq::BumpPtrAllocator &allocator, rq::ParameterListFactory &factory,
            rq::SymbolConstant &return_type, rq::SymbolConstant &reciever_type,
            const rq::Expression &precondition_snippet,
            const rq::Expression &postcondition_snippet);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPreconditionSnippet() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getPostconditionSnippet() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Layout final : public rq::ParameterList {
  using Self = rq::Layout;

  explicit RQ_ALWAYS_INLINE Layout();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Composition final : public rq::SimpleSymbol {
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

struct TemplateWeightGroup final {
  using Self = rq::TemplateWeightGroup;

  rq::TemplateWeightGroup *_next_lower_ptr{nullptr};
  const rq::IntegerConstant *_weight{nullptr};
  rq::Template *_first_template{nullptr};
};

struct Polymorph : public rq::Symbol {
  using Self = rq::Polymorph;

  rq::TemplateWeightGroup *_highest_weight_group_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::Opcode opcode);

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
  rq::SymbolTable *_containing_table_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::Opcode opcode);

  RQ_ALWAYS_INLINE void release();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;

  explicit RQ_ALWAYS_INLINE Top();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Scope final : public rq::SymbolTable {
  using Self = rq::Scope;

  explicit RQ_ALWAYS_INLINE Scope();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct GlobalDeclaration : public rq::SymbolTable {
  using Self = rq::GlobalDeclaration;

  llvm::StringRef _name{};
  llvm::StringRef _mangled_name{};
  rq::SymbolTable *_hosting_table_ptr{nullptr};
  const rq::Expression *_snippet_ptr{nullptr};
  const rq::Expression *_name_snippet_ptr{nullptr};
  rq::ExpressionFlags _flags{};

  explicit RQ_ALWAYS_INLINE GlobalDeclaration(rq::Opcode opcode);

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

  const rq::Expression *_underlying_snippet_ptr{nullptr};
  rq::SymbolConstant *_underlying_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumeration();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Enumerator final : public rq::GlobalDeclaration {
  using Self = rq::Enumerator;

  const rq::Expression *_type_snippet_ptr{nullptr};
  const rq::Expression *_default_value_snippet_ptr{nullptr};
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
  rq::SymbolConstant *_reciever_ptr{nullptr};
  rq::SymbolConstant *_element_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Ranger(rq::Opcode opcode);

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

struct Procedure : public rq::GlobalDeclaration {
  using Self = rq::Procedure;

  rq::Signature *_signature_ptr{nullptr};
  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Procedure(rq::Opcode opcode);

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

  rq::Layout *_template_layout{nullptr};
  const rq::Expression *_constraint_snippet_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Template(rq::Opcode opcode);

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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;

  bool _is_platform_specific : 1;
  const llvm::APFloat _data;

  explicit RQ_ALWAYS_INLINE FloatConstant(const llvm::APFloat &data,
                                          bool is_platform_specific);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ExpressionConstant final : public rq::Constant {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_snippet_ptr;

  explicit RQ_ALWAYS_INLINE ExpressionConstant(const rq::Expression &data);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct SymbolConstant final : public rq::Constant {
  using Self = rq::SymbolConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  explicit RQ_ALWAYS_INLINE SymbolConstant(rq::Symbol &symbol,
                                           rq::TypeFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _is_platform_specific : 1;
  bool _data : 1;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data,
                                            bool is_platform_specific);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;

  bool _is_platform_specific : 1;
  llvm::StringRef _data;

  explicit RQ_ALWAYS_INLINE StringConstant(llvm::StringRef data,
                                           bool is_platform_specific);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  bool _is_platform_specific : 1;
  llvm::ArrayRef<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data,
                                          bool is_platform_specific);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression *_snippet_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE NullaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE UnaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity *_address0_ptr{nullptr};
  rq::Entity *_address1_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE BinaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity);
};

} // namespace rq

#include <rq/detail/entity.hpp>