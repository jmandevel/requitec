#pragma once

#include <rq/utility.hpp>

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
  SY_DETERMINICITY,
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
  // INSTRUCTION - ~(SYMBOL | CONSTANT)

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
        struct DeterminicityType;
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

struct ParameterList final {};

struct Entity {};

struct Symbol : public rq::Entity {};

struct SimpleSymbol : public rq::Symbol {};

struct Literal : public rq::SimpleSymbol {
  using Self = rq::Literal;
};

struct IntegerLiteral final : public rq::Literal {
  using Self = rq::IntegerLiteral;
};

struct FloatLiteral final : public rq::Literal {
  using Self = rq::FloatLiteral;
};

struct StringLiteral final : public rq::Literal {
  using Self = rq::StringLiteral;
};

struct CodeunitLiteral final : public rq::Literal {
  using Self = rq::CodeunitLiteral;
};

struct Contextual : public rq::SimpleSymbol {
  using Self = rq::Contextual;
};

struct ContextualName : public rq::Contextual {
  using Self = rq::ContextualName;
};

struct NoName final : public rq::ContextualName {
  using Self = rq::NoName;
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;
};

struct OutValue final : public rq::ContextualValue {
  using Self = rq::OutValue;
};

struct ThisValue final : public rq::ContextualValue {
  using Self = rq::ThisValue;
};

struct ResultValue final : public rq::ContextualValue {
  using Self = rq::ResultValue;
};

struct ValueValue final : public rq::ContextualValue {
  using Self = rq::ValueValue;
};

struct IndexValue final : public rq::ContextualValue {
  using Self = rq::IndexValue;
};

struct DiscriminantValue final : public rq::ContextualValue {
  using Self = rq::DiscriminantValue;
};

struct CommandLineArgumentsValue final : public rq::ContextualValue {
  using Self = rq::CommandLineArgumentsValue;
};

struct CallsiteValue final : public rq::ContextualValue {
  using Self = rq::CallsiteValue;
};

struct ContextualType : public rq::Contextual {
  using Self = rq::ContextualType;
};

struct InferenceType final : public rq::ContextualType {
  using Self = rq::InferenceType;
};

struct VoidType final : public rq::ContextualType {
  using Self = rq::VoidType;
};

struct NoReturnType final : public rq::ContextualType {
  using Self = rq::NoReturnType;
};

struct FundamentalConstraint : public rq::SimpleSymbol {
  using Self = rq::FundamentalConstraint;
};

struct TypeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::TypeConstraint;
};

struct RangeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::RangeConstraint;
};

struct NumericConstraint final : public rq::FundamentalConstraint {
  using Self = rq::NumericConstraint;
};

struct SignedConstraint final : public rq::FundamentalConstraint {
  using Self = rq::SignedConstraint;
};

struct UnsignedConstraint final : public rq::FundamentalConstraint {
  using Self = rq::UnsignedConstraint;
};

struct IntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::IntegerConstraint;
};

struct SignedIntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::SignedIntegerConstraint;
};

struct UnsignedIntegerConstraint final : public rq::FundamentalConstraint {
  using Self = rq::UnsignedIntegerConstraint;
};

struct FloatConstraint final : public rq::FundamentalConstraint {
  using Self = rq::FloatConstraint;
};

struct BinaryConstraint final : public rq::FundamentalConstraint {
  using Self = rq::BinaryConstraint;
};

struct BfloatConstraint final : public rq::FundamentalConstraint {
  using Self = rq::BfloatConstraint;
};

struct StringConstraint final : public rq::FundamentalConstraint {
  using Self = rq::StringConstraint;
};

struct CodeunitConstraint final : public rq::FundamentalConstraint {
  using Self = rq::CodeunitConstraint;
};

struct ExpressionAttributeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::ExpressionAttributeConstraint;
};

struct TypeAttributeConstraint final : public rq::FundamentalConstraint {
  using Self = rq::TypeAttributeConstraint;
};

struct ExpressionAttributeType : public rq::SimpleSymbol {
  using Self = rq::ExpressionAttributeType;
};

struct LabelingType final : public rq::ExpressionAttributeType {
  using Self = rq::LabelingType;
};

struct VisibilityType final : public rq::ExpressionAttributeType {
  using Self = rq::VisibilityType;
};

struct ScopingType final : public rq::ExpressionAttributeType {
  using Self = rq::ScopingType;
};

struct AvailabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AvailabilityType;
};

struct PropertyMutabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::PropertyMutabilityType;
};

struct ExportingType final : public rq::ExpressionAttributeType {
  using Self = rq::ExportingType;
};

struct GenerationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::GenerationTimeType;
};

struct CapturingType final : public rq::ExpressionAttributeType {
  using Self = rq::CapturingType;
};

struct EvaluationTimeType final : public rq::ExpressionAttributeType {
  using Self = rq::EvaluationTimeType;
};

struct InliningType final : public rq::ExpressionAttributeType {
  using Self = rq::InliningType;
};

struct ManglingType final : public rq::ExpressionAttributeType {
  using Self = rq::ManglingType;
};

struct PackingType final : public rq::ExpressionAttributeType {
  using Self = rq::PackingType;
};

struct TemplatingType final : public rq::ExpressionAttributeType {
  using Self = rq::TemplatingType;
};

struct LikelyhoodType final : public rq::ExpressionAttributeType {
  using Self = rq::LikelyhoodType;
};

struct SupportType final : public rq::ExpressionAttributeType {
  using Self = rq::SupportType;
};

struct CopyabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::CopyabilityType;
};

struct AddressStabilityType final : public rq::ExpressionAttributeType {
  using Self = rq::AddressStabilityType;
};

struct CleanupType final : public rq::ExpressionAttributeType {
  using Self = rq::CleanupType;
};

struct TypeAttributeType : public rq::SimpleSymbol {
  using Self = rq::TypeAttributeType;
};

struct MutabilityType final : public rq::TypeAttributeType {
  using Self = rq::MutabilityType;
};

struct VolatilityType final : public rq::TypeAttributeType {
  using Self = rq::VolatilityType;
};

struct DeterminicityType final : public rq::TypeAttributeType {
  using Self = rq::DeterminicityType;
};

struct AtomicityType final : public rq::TypeAttributeType {
  using Self = rq::AtomicityType;
};

struct NullTerminationType final : public rq::TypeAttributeType {
  using Self = rq::NullTerminationType;
};

struct PreconditionType final : public rq::TypeAttributeType {
  using Self = rq::PreconditionType;
};

struct PostconditionType final : public rq::TypeAttributeType {
  using Self = rq::PostconditionType;
};

struct ReflectiveType : public rq::SimpleSymbol {
  using Self = rq::ReflectiveType;
};

struct SymbolType final : public rq::ReflectiveType {
  using Self = rq::SymbolType;
};

struct ExpressionType final : public rq::ReflectiveType {
  using Self = rq::ExpressionType;
};

struct UnscaledPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::UnscaledPrimitiveType;
};

struct BooleanType final : public rq::UnscaledPrimitiveType {
  using Self = rq::BooleanType;
};

struct HalfType final : public rq::UnscaledPrimitiveType {
  using Self = rq::HalfType;
};

struct SingleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SingleType;
};

struct DoubleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::DoubleType;
};

struct QuadrupleType final : public rq::UnscaledPrimitiveType {
  using Self = rq::QuadrupleType;
};

struct Binary16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary16Type;
};

struct Binary32Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary32Type;
};

struct Binary64Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary64Type;
};

struct Binary128Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Binary128Type;
};

struct Bfloat16Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Bfloat16Type;
};

struct AsciiType final : public rq::UnscaledPrimitiveType {
  using Self = rq::AsciiType;
};

struct Utf8Type final : public rq::UnscaledPrimitiveType {
  using Self = rq::Utf8Type;
};

struct SignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedIndexType;
};

struct UnsignedIndexType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedIndexType;
};

struct SignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::SignedAddressType;
};

struct UnsignedAddressType final : public rq::UnscaledPrimitiveType {
  using Self = rq::UnsignedAddressType;
};

struct VariadicArgumentType final : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentType;
};

struct ScaledIntegerType : public rq::Symbol {
  using Self = rq::ScaledIntegerType;
};

struct ScaledSignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledSignedIntegerType;
};

struct ScaledUnsignedIntegerType final : public rq::ScaledIntegerType {
  using Self = rq::ScaledUnsignedIntegerType;
};

struct UncountedSubtype : public rq::SimpleSymbol {
  using Self = rq::UncountedSubtype;
};

struct ReferenceSubtype final : public rq::UncountedSubtype {
  using Self = rq::ReferenceSubtype;
};

struct PointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::PointerSubtype;
};

struct FatPointerSubtype final : public rq::UncountedSubtype {
  using Self = rq::FatPointerSubtype;
};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {
  using Self = rq::InferenceCountArraySubtype;
};

struct Array final : public rq::SimpleSymbol {
  using Self = rq::Array;
};

struct Module final : public rq::SimpleSymbol {
  using Self = rq::Module;
};

struct Import final : public rq::SimpleSymbol {
  using Self = rq::Import;
};

struct ConcatenatedString final : public rq::SimpleSymbol {
  using Self = rq::ConcatenatedString;
};

struct ArithmeticSequence : public rq::SimpleSymbol {
  using Self = rq::ArithmeticSequence;
};

struct ArithmeticInterval final : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;
};

struct InfiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticSequence;
};

struct FiniteArithmeticSequence final : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticSequence;
};

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;
};

struct Label final : public rq::LocalDeclaration {
  using Self = rq::Label;
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;
};

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;
};

struct Parameter : public rq::LocalVariable {
  using Self = rq::Parameter;
};

struct SignatureParameter final : public rq::Parameter {
  using Self = rq::SignatureParameter;
};

struct ClassParameter final : public rq::Parameter {
  using Self = rq::ClassParameter;
};

struct TemplateParameter final : public rq::Parameter {
  using Self = rq::TemplateParameter;
};

struct Signature final : public rq::SimpleSymbol {
  using Self = rq::Signature;
};

struct Composition final : public rq::SimpleSymbol {
  using Self = rq::Composition;
};

struct Synonym final : public rq::SimpleSymbol {
  using Self = rq::Synonym;
};

struct Polymorph : public rq::SimpleSymbol {
  using Self = rq::Polymorph;
};

struct RangerPolymorph final : public rq::Polymorph {
  using Self = rq::RangerPolymorph;
};

struct ProcedurePolymorph final : public rq::Polymorph {
  using Self = rq::ProcedurePolymorph;
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;
};

struct EnumeratorPolymorph final : public rq::Polymorph {
  using Self = rq::EnumeratorPolymorph;
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;
};

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;
};

struct Scope final : public rq::SymbolTable {
  using Self = rq::Scope;
};

struct GlobalDeclaration : public rq::SymbolTable {
  using Self = rq::GlobalDeclaration;
};

struct Namespace final : public rq::GlobalDeclaration {
  using Self = rq::Namespace;
};

struct Class final : public rq::GlobalDeclaration {
  using Self = rq::Class;
};

struct Enumeration final : public rq::GlobalDeclaration {
  using Self = rq::Enumeration;
};

struct Interface final : public rq::GlobalDeclaration {
  using Self = rq::Interface;
};

struct GlobalVariable : public rq::GlobalDeclaration {
  using Self = rq::GlobalVariable;
};

struct GlobalDynamicVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalDynamicVariable;
};

struct GlobalStaticVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalStaticVariable;
};

struct Ranger : public rq::GlobalDeclaration {
  using Self = rq::Ranger;
};

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;
};

struct Procedure : public rq::GlobalDeclaration {
  using Self = rq::Procedure;
};

struct Entry final : public rq::Procedure {
  using Self = rq::Entry;
};

struct Function final : public rq::Procedure {
  using Self = rq::Function;
};

struct Method final : public rq::Procedure {
  using Self = rq::Method;
};

struct ExtensionFunction final : public rq::Procedure {
  using Self = rq::ExtensionFunction;
};

struct ExtensionMethod final : public rq::Procedure {
  using Self = rq::ExtensionMethod;
};

struct Template : public rq::SymbolTable {
  using Self = rq::Template;
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;
};

struct EnumerationTemplate final : public rq::Template {
  using Self = rq::EnumerationTemplate;
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;
};

struct ForwardRangerTemplate final : public rq::Template {
  using Self = rq::ForwardRangerTemplate;
};

struct BackwardRangerTemplate final : public rq::Template {
  using Self = rq::BackwardRangerTemplate;
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;
};

struct MethodTemplate final : public rq::Template {
  using Self = rq::MethodTemplate;
};

struct ExtensionFunctionTemplate final : public rq::Template {
  using Self = rq::ExtensionFunctionTemplate;
};

struct ExtensionMethodTemplate final : public rq::Template {
  using Self = rq::ExtensionMethodTemplate;
};

struct Specialization : public rq::SymbolTable {
  using Self = rq::Specialization;
};

struct ClassSpecialization final : public rq::Specialization {
  using Self = rq::ClassSpecialization;
};

struct EnumerationSpecialization final : public rq::Specialization {
  using Self = rq::EnumerationSpecialization;
};

struct InterfaceSpecialization final : public rq::Specialization {
  using Self = rq::InterfaceSpecialization;
};

struct GlobalDynamicVariableSpecialization final : public rq::Specialization {
  using Self = rq::GlobalDynamicVariableSpecialization;
};

struct GlobalStaticVariableSpecialization final : public rq::Specialization {
  using Self = rq::GlobalStaticVariableSpecialization;
};

struct ForwardRangerSpecialization final : public rq::Specialization {
  using Self = rq::ForwardRangerSpecialization;
};

struct BackwardRangerSpecialization final : public rq::Specialization {
  using Self = rq::BackwardRangerSpecialization;
};

struct FunctionSpecialization final : public rq::Specialization {
  using Self = rq::FunctionSpecialization;
};

struct MethodSpecialization final : public rq::Specialization {
  using Self = rq::MethodSpecialization;
};

struct ExtensionFunctionSpecialization final : public rq::Specialization {
  using Self = rq::ExtensionFunctionSpecialization;
};

struct ExtensionMethodSpecialization final : public rq::Specialization {
  using Self = rq::ExtensionMethodSpecialization;
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;
};

struct IntegerConstant final : public rq::Constant {
  using Self = rq::IntegerConstant;
};

struct FloatConstant final : public rq::Constant {
  using Self = rq::FloatConstant;
};

struct ExpressionConstant final : public rq::Constant {
  using Self = rq::ExpressionConstant;
};

struct TypeConstant final : public rq::Constant {
  using Self = rq::TypeConstant;
};

struct BooleanConstant final : public rq::Constant {
  using Self = rq::BooleanConstant;
};

struct StringConstant final : public rq::Constant {
  using Self = rq::StringConstant;
};

struct ArrayConstant final : public rq::Constant {
  using Self = rq::ArrayConstant;
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;
};

struct NullaryInstruction final : public rq::Instruction {
  using Self = rq::NullaryInstruction;
};

struct UnaryInstruction final : public rq::Instruction {
  using Self = rq::UnaryInstruction;
};

struct BinaryInstruction final : public rq::Instruction {
  using Self = rq::BinaryInstruction;
};

} // namespace rq