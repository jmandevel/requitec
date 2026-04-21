#pragma once

#include <rq/utility.hpp>
#include <rq/ast.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>

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

  // FUNDAMENTAL CONSTRAINTS
  SY_TYPE_CONSTRAINT,
  SY_RANGE_CONSTRAINT,
  SY_NUMERIC_CONSTRAINT,
  SY_SIGNED_CONSTRAINT,
  SY_UNSIGNED_CONSTRAINT,
  SY_INTEGER_CONSTRAINT,
  SY_SIGNED_INTEGER_CONSTRAINT,
  SY_UNSIGNED_INTEGER_CONSTRAINT,
  SY_FLOAT_CONSTRAINT,
  SY_BINARY_CONSTRAINT,
  SY_BFLOAT_CONSTRAINT,
  SY_STRING_CONSTRAINT,
  SY_CODEUNIT_CONSTRAINT,
  SY_EXPRESSION_ATTRIBUTE_CONSTRAINT,
  SY_TYPE_ATTRIBUTE_CONSTRAINT,

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
  SY_SIGNED_INDEX,
  SY_UNSIGNED_INDEX,
  SY_SIGNED_ADDRESS,
  SY_UNSIGNED_ADDRESS,

  // VARIADIC ARGUMENTS
  SY_VARIADIC_ARGUMENTS,

  // SCALED INTEGERS
  SY_SIGNED_INTEGER,
  SY_UNSIGNED_INTEGER,

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
  SY_SIGNATURE_PARAMETER,
  SY_CLASS_PARAMETER,
  SY_TEMPLATE_PARAMETER,

  // SIGNATURES
  SY_SIGNATURE,

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
  SY_ENUMERATOR_POLYMORPH,
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

  // SPECIALIZATIONS
  SY_CLASS_SPECIALIZATION,
  SY_ENUMERATION_SPECIALIZATION,
  SY_INTERFACE_SPECIALIZATION,
  SY_GLOBAL_DYNAMIC_VARIABLE_SPECIALIZATION,
  SY_GLOBAL_STATIC_VARIABLE_SPECIALIZATION,
  SY_FORWARD_RANGER_SPECIALIZATION,
  SY_BACKWARD_RANGER_SPECIALIZATION,
  SY_FUNCTION_SPECIALIZATION,
  SY_METHOD_SPECIALIZATION,
  SY_EXTENSION_FUNCTION_SPECIALIZATION,
  SY_EXTENSION_METHOD_SPECIALIZATION,

  CT_INTEGER,
  CT_FLOAT,
  CT_EXPRESSION,
  CT_TYPE,
  CT_BOOLEAN,
  CT_STRING,
  CT_ARRAY

  // TODO instructions
};

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode);

enum class OpcodeFlags {
  NONE = 0,

  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  // INSTRUCTION - not SYMBOL orB CONSTANT

  // SYMBOL FLAGS

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
      struct FundamentalConstraint;
        struct TypeConstraint;
        struct RangeConstraint;
        struct NumericConstraint;
        struct SignedConstraint;
        struct UnsignedConstraint;
        struct IntegerConstraint;
        struct SignedIntegerConstraint;
        struct UnsignedIntegerConstraint;
        struct FloatConstraint;
        struct BinaryConstraint;
        struct BfloatConstraint;
        struct StringConstraint;
        struct CodeunitConstraint;
        struct ExpressionAttributeConstraint;
        struct TypeAttributeConstraint;
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
          struct SignatureParameter;
          struct ClassParameter;
          struct TemplateParameter;
    struct Signature;
    struct Composition;
    struct Synonym;
    struct Polymorph;
      struct RangerPolymorph;
      struct ProcedurePolymorph;
      struct ClassPolymorph;
      struct EnumeratorPolymorph;
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
          struct ExttensionMethodTemplate;
        struct Specialization;
          struct ClassSpecialization;
          struct EnumerationSpecialization;
          struct InterfaceSpecialization;
          struct GlobalDynamicVariableSpecialization;
          struct GlobalStaticVariableSpecialization;
          struct ForwardRangerSpecialization;
          struct BackwardRangerSpecialization;
          struct FunctionSpecialization;
          struct MethodSpecialization;
          struct ExtensionFunctionSpecialization;
          struct ExtensionMethodSpecialization;
  struct Constant;
    struct IntegerConstant;
    struct FloatConstant;
    struct ExpressionConstant;
    struct TypeConstant;
    struct BooleanConstant;
    struct StringConstant;
    struct ArrayConstant;
  struct Instruction;
    struct NullaryInstruction;
    struct UnaryInstruction;
    struct BinaryInstruction;
// clang-format on

struct ParameterList final {
  using Self = rq::ParameterList;

  explicit RQ_ALWAYS_INLINE ParameterList();
};

struct Entity {
  using Self = rq::Entity;

  rq::Opcode _opcode;

  explicit RQ_ALWAYS_INLINE Entity(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SimpleSymbol : public rq::Symbol {
  using Self = rq::SimpleSymbol;

  explicit RQ_ALWAYS_INLINE SimpleSymbol(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Literal : public rq::SimpleSymbol {
  using Self = rq::Literal;

  explicit RQ_ALWAYS_INLINE Literal(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct IntegerLiteral final : public rq::Literal {
  using Self = rq::IntegerLiteral;

  explicit RQ_ALWAYS_INLINE IntegerLiteral();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FloatLiteral final : public rq::Literal {
  using Self = rq::FloatLiteral;

  explicit RQ_ALWAYS_INLINE FloatLiteral();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct StringLiteral final : public rq::Literal {
  using Self = rq::StringLiteral;

  explicit RQ_ALWAYS_INLINE StringLiteral();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CodeunitLiteral final : public rq::Literal {
  using Self = rq::CodeunitLiteral;

  explicit RQ_ALWAYS_INLINE CodeunitLiteral();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Contextual : public rq::SimpleSymbol {
  using Self = rq::Contextual;

  explicit RQ_ALWAYS_INLINE Contextual(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ContextualName : public rq::Contextual {
  using Self = rq::ContextualName;

  explicit RQ_ALWAYS_INLINE ContextualName(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct NoName final : public rq::ContextualName {
  using Self = rq::NoName;

  explicit RQ_ALWAYS_INLINE NoName();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;

  explicit RQ_ALWAYS_INLINE ContextualValue(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct OutValue final : public rq::ContextualValue {
  using Self = rq::OutValue;

  explicit RQ_ALWAYS_INLINE OutValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ThisValue final : public rq::ContextualValue {
  using Self = rq::ThisValue;

  explicit RQ_ALWAYS_INLINE ThisValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ResultValue final : public rq::ContextualValue {
  using Self = rq::ResultValue;

  explicit RQ_ALWAYS_INLINE ResultValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ValueValue final : public rq::ContextualValue {
  using Self = rq::ValueValue;

  explicit RQ_ALWAYS_INLINE ValueValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct IndexValue final : public rq::ContextualValue {
  using Self = rq::IndexValue;

  explicit RQ_ALWAYS_INLINE IndexValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct DiscriminantValue final : public rq::ContextualValue {
  using Self = rq::DiscriminantValue;

  explicit RQ_ALWAYS_INLINE DiscriminantValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CommandLineArgumentsValue final : public rq::ContextualValue {
  using Self = rq::CommandLineArgumentsValue;

  explicit RQ_ALWAYS_INLINE CommandLineArgumentsValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CallsiteValue final : public rq::ContextualValue {
  using Self = rq::CallsiteValue;

  explicit RQ_ALWAYS_INLINE CallsiteValue();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ContextualType : public rq::Contextual {
  using Self = rq::ContextualType;

  explicit RQ_ALWAYS_INLINE ContextualType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InferenceType final : public rq::ContextualType {
  using Self = rq::InferenceType;

  explicit RQ_ALWAYS_INLINE InferenceType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct VoidType final : public rq::ContextualType {
  using Self = rq::VoidType;

  explicit RQ_ALWAYS_INLINE VoidType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct NoReturnType final : public rq::ContextualType {
  using Self = rq::NoReturnType;

  explicit RQ_ALWAYS_INLINE NoReturnType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FundamentalConstraint : public rq::SimpleSymbol {
  using Self = rq::FundamentalConstraint;

  explicit RQ_ALWAYS_INLINE FundamentalConstraint(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TypeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::TypeConstraint;

  explicit RQ_ALWAYS_INLINE TypeConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct RangeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::RangeConstraint;

  explicit RQ_ALWAYS_INLINE RangeConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct NumericConstraint final : public rq::FundamentalConstraint {
  using Self = rq::NumericConstraint;

  explicit RQ_ALWAYS_INLINE NumericConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SignedConstraint final : public rq::FundamentalConstraint {
  using Self = rq::SignedConstraint;

  explicit RQ_ALWAYS_INLINE SignedConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnsignedConstraint final : public rq::FundamentalConstraint {
  using Self = rq::UnsignedConstraint;

  explicit RQ_ALWAYS_INLINE UnsignedConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct IntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::IntegerConstraint;

  explicit RQ_ALWAYS_INLINE IntegerConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SignedIntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::SignedIntegerConstraint;

  explicit RQ_ALWAYS_INLINE SignedIntegerConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnsignedIntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::UnsignedIntegerConstraint;

  explicit RQ_ALWAYS_INLINE UnsignedIntegerConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FloatConstraint final : public rq::FundamentalConstraint {
  using Self = rq::FloatConstraint;

  explicit RQ_ALWAYS_INLINE FloatConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BinaryConstraint final : public rq::FundamentalConstraint {
  using Self = rq::BinaryConstraint;

  explicit RQ_ALWAYS_INLINE BinaryConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BfloatConstraint final : public rq::FundamentalConstraint {
  using Self = rq::BfloatConstraint;

  explicit RQ_ALWAYS_INLINE BfloatConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct StringConstraint final : public rq::FundamentalConstraint {
  using Self = rq::StringConstraint;

  explicit RQ_ALWAYS_INLINE StringConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CodeunitConstraint final : public rq::FundamentalConstraint {
  using Self = rq::CodeunitConstraint;

  explicit RQ_ALWAYS_INLINE CodeunitConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExpressionAttributeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::ExpressionAttributeConstraint;

  explicit RQ_ALWAYS_INLINE ExpressionAttributeConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TypeAttributeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::TypeAttributeConstraint;

  explicit RQ_ALWAYS_INLINE TypeAttributeConstraint();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExpressionAttributeType : public rq::SimpleSymbol {
  using Self = rq::ExpressionAttributeType;

  explicit RQ_ALWAYS_INLINE ExpressionAttributeType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LabelingType final : public rq::ExpressionAttributeType {
  using Self = rq::LabelingType;

  explicit RQ_ALWAYS_INLINE LabelingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct VisibilityType final : public rq::ExpressionAttributeType {
  using Self = rq::VisibilityType;

  explicit RQ_ALWAYS_INLINE VisibilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ScopingType final : public rq::ExpressionAttributeType {
  using Self = rq::ScopingType;

  explicit RQ_ALWAYS_INLINE ScopingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct AvailabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AvailabilityType;

  explicit RQ_ALWAYS_INLINE AvailabilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct PropertyMutabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::PropertyMutabilityType;

  explicit RQ_ALWAYS_INLINE PropertyMutabilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExportingType final : public rq::ExpressionAttributeType {
  using Self = rq::ExportingType;

  explicit RQ_ALWAYS_INLINE ExportingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GenerationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::GenerationTimeType;

  explicit RQ_ALWAYS_INLINE GenerationTimeType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CapturingType final : public rq::ExpressionAttributeType {
  using Self = rq::CapturingType;

  explicit RQ_ALWAYS_INLINE CapturingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct EvaluationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::EvaluationTimeType;

  explicit RQ_ALWAYS_INLINE EvaluationTimeType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InliningType final : public rq::ExpressionAttributeType {
  using Self = rq::InliningType;

  explicit RQ_ALWAYS_INLINE InliningType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ManglingType final : public rq::ExpressionAttributeType {
  using Self = rq::ManglingType;

  explicit RQ_ALWAYS_INLINE ManglingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct PackingType final : public rq::ExpressionAttributeType {
  using Self = rq::PackingType;

  explicit RQ_ALWAYS_INLINE PackingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TemplatingType final : public rq::ExpressionAttributeType {
  using Self = rq::TemplatingType;

  explicit RQ_ALWAYS_INLINE TemplatingType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LikelyhoodType final : public rq::ExpressionAttributeType {
  using Self = rq::LikelyhoodType;

  explicit RQ_ALWAYS_INLINE LikelyhoodType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SupportType final : public rq::ExpressionAttributeType {
  using Self = rq::SupportType;

  explicit RQ_ALWAYS_INLINE SupportType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CopyabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::CopyabilityType;

  explicit RQ_ALWAYS_INLINE CopyabilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct AddressStabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AddressStabilityType;

  explicit RQ_ALWAYS_INLINE AddressStabilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct CleanupType final : public rq::ExpressionAttributeType {
  using Self = rq::CleanupType;

  explicit RQ_ALWAYS_INLINE CleanupType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TypeAttributeType : public rq::SimpleSymbol {
  using Self = rq::TypeAttributeType;

  explicit RQ_ALWAYS_INLINE TypeAttributeType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct MutabilityType final : public rq::TypeAttributeType {
  using Self = rq::MutabilityType;

  explicit RQ_ALWAYS_INLINE MutabilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct VolatilityType final : public rq::TypeAttributeType {
  using Self = rq::VolatilityType;

  explicit RQ_ALWAYS_INLINE VolatilityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Initialization_RequirementType final : public rq::TypeAttributeType {
  using Self = rq::Initialization_RequirementType;

  explicit RQ_ALWAYS_INLINE Initialization_RequirementType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct AtomicityType final : public rq::TypeAttributeType {
  using Self = rq::AtomicityType;

  explicit RQ_ALWAYS_INLINE AtomicityType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct NullTerminationType final : public rq::TypeAttributeType {
  using Self = rq::NullTerminationType;

  explicit RQ_ALWAYS_INLINE NullTerminationType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct PreconditionType final : public rq::TypeAttributeType {
  using Self = rq::PreconditionType;

  explicit RQ_ALWAYS_INLINE PreconditionType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct PostconditionType final : public rq::TypeAttributeType {
  using Self = rq::PostconditionType;

  explicit RQ_ALWAYS_INLINE PostconditionType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ReflectiveType : public rq::SimpleSymbol {
  using Self = rq::ReflectiveType;

  explicit RQ_ALWAYS_INLINE ReflectiveType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SymbolType final : public rq::ReflectiveType {
  using Self = rq::SymbolType;

  explicit RQ_ALWAYS_INLINE SymbolType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExpressionType final : public rq::ReflectiveType {
  using Self = rq::ExpressionType;

  explicit RQ_ALWAYS_INLINE ExpressionType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnscaledPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::UnscaledPrimitiveType;

  explicit RQ_ALWAYS_INLINE UnscaledPrimitiveType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BooleanType final : public rq::UnscaledPrimitiveType {
  using Self = rq::BooleanType;

  explicit RQ_ALWAYS_INLINE BooleanType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct HalfType final : public rq::UnscaledPrimitiveType {
  using Self = rq::HalfType;

  explicit RQ_ALWAYS_INLINE HalfType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SingleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SingleType;

  explicit RQ_ALWAYS_INLINE SingleType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct DoubleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::DoubleType;

  explicit RQ_ALWAYS_INLINE DoubleType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct QuadrupleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::QuadrupleType;

  explicit RQ_ALWAYS_INLINE QuadrupleType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Binary16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary16Type;

  explicit RQ_ALWAYS_INLINE Binary16Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Binary32Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary32Type;

  explicit RQ_ALWAYS_INLINE Binary32Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Binary64Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary64Type;

  explicit RQ_ALWAYS_INLINE Binary64Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Binary128Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary128Type;

  explicit RQ_ALWAYS_INLINE Binary128Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Bfloat16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Bfloat16Type;

  explicit RQ_ALWAYS_INLINE Bfloat16Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct AsciiType final : public rq::UnscaledPrimitiveType {
  using Self = rq::AsciiType;

  explicit RQ_ALWAYS_INLINE AsciiType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Utf8Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Utf8Type;

  explicit RQ_ALWAYS_INLINE Utf8Type();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedIndexType;

  explicit RQ_ALWAYS_INLINE SignedIndexType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnsignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedIndexType;

  explicit RQ_ALWAYS_INLINE UnsignedIndexType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedAddressType;

  explicit RQ_ALWAYS_INLINE SignedAddressType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnsignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedAddressType;

  explicit RQ_ALWAYS_INLINE UnsignedAddressType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct VariadicArgumentType final : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentType;

  explicit RQ_ALWAYS_INLINE VariadicArgumentType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

enum class ScaledIntegerFlags : std::uint32_t {
  NONE = 0,

  // EXACT = no fast or least
  FAST = rq::getBit(0),
  LEAST = rq::getBit(1)
};

struct ScaledIntegerType : public rq::Symbol {
  using Self = rq::ScaledIntegerType;

  explicit RQ_ALWAYS_INLINE ScaledIntegerType(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ScaledSignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledSignedIntegerType;

  explicit RQ_ALWAYS_INLINE ScaledSignedIntegerType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ScaledUnsignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE ScaledUnsignedIntegerType();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UncountedSubtype : public rq::SimpleSymbol {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ReferenceSubtype final : public rq::UncountedSubtype {
  using Self = rq::ReferenceSubtype;

  explicit RQ_ALWAYS_INLINE ReferenceSubtype();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct PointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::PointerSubtype;

  explicit RQ_ALWAYS_INLINE PointerSubtype();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FatPointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::FatPointerSubtype;

  explicit RQ_ALWAYS_INLINE FatPointerSubtype();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE InferenceCountArraySubtype();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Array final : public rq::SimpleSymbol {
  using Self = rq::Array;

  explicit RQ_ALWAYS_INLINE Array();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Module final : public rq::SimpleSymbol {
  using Self = rq::Module;

  explicit RQ_ALWAYS_INLINE Module();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Import final : public rq::SimpleSymbol {
  using Self = rq::Import;

  explicit RQ_ALWAYS_INLINE Import();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ConcatenatedString final : public rq::SimpleSymbol {
  using Self = rq::ConcatenatedString;

  explicit RQ_ALWAYS_INLINE ConcatenatedString();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ArithmeticSequence : public rq::SimpleSymbol {
  using Self = rq::ArithmeticSequence;

  explicit RQ_ALWAYS_INLINE ArithmeticSequence(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ArithmeticInterval final : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;

  explicit RQ_ALWAYS_INLINE ArithmeticInterval();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InfiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE InfiniteArithmeticSequence();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticSequence;

  explicit RQ_ALWAYS_INLINE FiniteArithmeticSequence();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;

  explicit RQ_ALWAYS_INLINE LocalDeclaration(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Label final : public rq::LocalDeclaration {
  using Self = rq::Label;

  explicit RQ_ALWAYS_INLINE Label();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  explicit RQ_ALWAYS_INLINE LocalVariable(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  explicit RQ_ALWAYS_INLINE LocalDynamicVariable();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;

  explicit RQ_ALWAYS_INLINE LocalStaticVariable();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Parameter : public rq::LocalVariable {
  using Self = rq::Parameter;

  explicit RQ_ALWAYS_INLINE Parameter(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SignatureParameter final : public rq::Parameter {
  using Self = rq::SignatureParameter;

  explicit RQ_ALWAYS_INLINE SignatureParameter();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ClassParameter final : public rq::Parameter {
  using Self = rq::ClassParameter;

  explicit RQ_ALWAYS_INLINE ClassParameter();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TemplateParameter final : public rq::Parameter {
  using Self = rq::TemplateParameter;

  explicit RQ_ALWAYS_INLINE TemplateParameter();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Signature final : public rq::SimpleSymbol {
  using Self = rq::Signature;

  explicit RQ_ALWAYS_INLINE Signature();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Composition final : public rq::SimpleSymbol {
  using Self = rq::Composition;

  explicit RQ_ALWAYS_INLINE Composition();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Synonym final : public rq::SimpleSymbol {
  using Self = rq::Synonym;

  explicit RQ_ALWAYS_INLINE Synonym();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Polymorph : public rq::SimpleSymbol {
  using Self = rq::Polymorph;

  explicit RQ_ALWAYS_INLINE Polymorph(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct RangerPolymorph final : public rq::Polymorph {
  using Self = rq::RangerPolymorph;

  explicit RQ_ALWAYS_INLINE RangerPolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ProcedurePolymorph final : public rq::Polymorph {
  using Self = rq::ProcedurePolymorph;

  explicit RQ_ALWAYS_INLINE ProcedurePolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct EnumeratorPolymorph final : public rq::Polymorph {
  using Self = rq::EnumeratorPolymorph;

  explicit RQ_ALWAYS_INLINE EnumeratorPolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariablePolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariablePolymorph();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;

  explicit RQ_ALWAYS_INLINE Top();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Scope final : public rq::SymbolTable {
  using Self = rq::Scope;

  explicit RQ_ALWAYS_INLINE Scope();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalDeclaration : public rq::SymbolTable {
  using Self = rq::GlobalDeclaration;

  explicit RQ_ALWAYS_INLINE GlobalDeclaration(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Namespace final : public rq::GlobalDeclaration {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Class final : public rq::GlobalDeclaration {
  using Self = rq::Class;

  explicit RQ_ALWAYS_INLINE Class();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Enumeration final : public rq::GlobalDeclaration {
  using Self = rq::Enumeration;

  explicit RQ_ALWAYS_INLINE Enumeration();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Interface final : public rq::GlobalDeclaration {
  using Self = rq::Interface;

  explicit RQ_ALWAYS_INLINE Interface();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalVariable : public rq::GlobalDeclaration {
  using Self = rq::GlobalVariable;

  explicit RQ_ALWAYS_INLINE GlobalVariable(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalDynamicVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalDynamicVariable;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariable();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalStaticVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalStaticVariable;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariable();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Ranger : public rq::GlobalDeclaration {
  using Self = rq::Ranger;

  explicit RQ_ALWAYS_INLINE Ranger(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;

  explicit RQ_ALWAYS_INLINE ForwardRanger();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;

  explicit RQ_ALWAYS_INLINE BackwardRanger();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Procedure : public rq::GlobalDeclaration {
  using Self = rq::Procedure;

  explicit RQ_ALWAYS_INLINE Procedure(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Entry final : public rq::Procedure {
  using Self = rq::Entry;

  explicit RQ_ALWAYS_INLINE Entry();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Function final : public rq::Procedure {
  using Self = rq::Function;

  explicit RQ_ALWAYS_INLINE Function();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Method final : public rq::Procedure {
  using Self = rq::Method;

  explicit RQ_ALWAYS_INLINE Method();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionFunction final : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  explicit RQ_ALWAYS_INLINE ExtensionFunction();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionMethod final : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  explicit RQ_ALWAYS_INLINE ExtensionMethod();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Template : public rq::SymbolTable {
  using Self = rq::Template;

  explicit RQ_ALWAYS_INLINE Template(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct EnumerationTemplate final : public rq::Template {
  using Self = rq::EnumerationTemplate;

  explicit RQ_ALWAYS_INLINE EnumerationTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ForwardRangerTemplate final : public rq::Template {
  using Self = rq::ForwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE ForwardRangerTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BackwardRangerTemplate final : public rq::Template {
  using Self = rq::BackwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE BackwardRangerTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct MethodTemplate final : public rq::Template {
  using Self = rq::MethodTemplate;

  explicit RQ_ALWAYS_INLINE MethodTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionFunctionTemplate final : public rq::Template {
  using Self = rq::ExtensionFunctionTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionFunctionTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionMethodTemplate final : public rq::Template {
  using Self = rq::ExtensionMethodTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionMethodTemplate();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Specialization : public rq::SymbolTable {
  using Self = rq::Specialization;

  explicit RQ_ALWAYS_INLINE Specialization(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ClassSpecialization final : public rq::Specialization {
  using Self = rq::ClassSpecialization;

  explicit RQ_ALWAYS_INLINE ClassSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct EnumerationSpecialization final : public rq::Specialization {
  using Self = rq::EnumerationSpecialization;

  explicit RQ_ALWAYS_INLINE EnumerationSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct InterfaceSpecialization final : public rq::Specialization {
  using Self = rq::InterfaceSpecialization;

  explicit RQ_ALWAYS_INLINE InterfaceSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalDynamicVariableSpecialization final : public rq::Specialization {
  using Self = rq::GlobalDynamicVariableSpecialization;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct GlobalStaticVariableSpecialization final : public rq::Specialization {
  using Self = rq::GlobalStaticVariableSpecialization;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ForwardRangerSpecialization final : public rq::Specialization {
  using Self = rq::ForwardRangerSpecialization;

  explicit RQ_ALWAYS_INLINE ForwardRangerSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BackwardRangerSpecialization final : public rq::Specialization {
  using Self = rq::BackwardRangerSpecialization;

  explicit RQ_ALWAYS_INLINE BackwardRangerSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FunctionSpecialization final : public rq::Specialization {
  using Self = rq::FunctionSpecialization;

  explicit RQ_ALWAYS_INLINE FunctionSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct MethodSpecialization final : public rq::Specialization {
  using Self = rq::MethodSpecialization;

  explicit RQ_ALWAYS_INLINE MethodSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionFunctionSpecialization final : public rq::Specialization {
  using Self = rq::ExtensionFunctionSpecialization;

  explicit RQ_ALWAYS_INLINE ExtensionFunctionSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExtensionMethodSpecialization final : public rq::Specialization {
  using Self = rq::ExtensionMethodSpecialization;

  explicit RQ_ALWAYS_INLINE ExtensionMethodSpecialization();

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit RQ_ALWAYS_INLINE Constant(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;

  const llvm::APInt _data;

  explicit RQ_ALWAYS_INLINE IntegerConstant(const llvm::APInt &data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;

  const llvm::APFloat _data;

  explicit RQ_ALWAYS_INLINE FloatConstant(const llvm::APFloat &data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ExpressionConstant final : public rq::Constant {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_expression_ptr;

  explicit RQ_ALWAYS_INLINE ExpressionConstant(const rq::Expression& data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct TypeConstant final : public rq::Constant {
  using Self = rq::TypeConstant;

  rq::Symbol* _symbol_ptr;
  rq::TypeFlags _type_flags;

  explicit RQ_ALWAYS_INLINE TypeConstant(rq::Symbol& symbol, rq::TypeFlags flags);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _data;

  explicit RQ_ALWAYS_INLINE BooleanConstant(bool data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;

  llvm::StringRef _data;

  explicit RQ_ALWAYS_INLINE StringConstant(llvm::StringRef data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;

  llvm::ArrayRef<rq::Constant> _data;

  explicit RQ_ALWAYS_INLINE ArrayConstant(llvm::ArrayRef<rq::Constant> data);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression* _expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Instruction(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  explicit RQ_ALWAYS_INLINE NullaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity* _address0_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE UnaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity* _address0_ptr{nullptr};
  rq::Entity* _address1_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE BinaryInstruction(rq::Opcode opcode);

  [[nodiscard]] static bool classof(const rq::Entity* entity);
};

} // namespace rq