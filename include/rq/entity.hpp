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

struct ParameterList final {

};

struct Entity {

};

struct Symbol : public rq::Entity {

};

struct SimpleSymbol : public rq::Symbol {

};

struct Literal : public rq::SimpleSymbol {

};

struct IntegerLiteral final : public rq::Literal {

};

struct FloatLiteral final : public rq::Literal {

};

struct StringLiteral final : public rq::Literal {

};

struct CodeunitLiteral final : public rq::Literal {

};

struct Contextual : public rq::SimpleSymbol {

};

struct ContextualName : public rq::Contextual {

};

struct NoName final : public rq::ContextualName {

};

struct ContextualValue : public rq::Contextual {

};

struct OutValue final : public rq::ContextualValue {

};

struct ThisValue final : public rq::ContextualValue {

};

struct ResultValue final : public rq::ContextualValue {

};

struct ValueValue final : public rq::ContextualValue {

};

struct IndexValue final : public rq::ContextualValue {

};

struct DiscriminantValue final : public rq::ContextualValue {

};

struct CommandLineArgumentsValue final : public rq::ContextualValue {

};

struct CallsiteValue final : public rq::ContextualValue {

};

struct ContextualType : public rq::Contextual {

};

struct InferenceType final : public rq::ContextualType {

};

struct VoidType final : public rq::ContextualType {

};

struct NoReturnType final : public rq::ContextualType {

};

struct FundamentalConstraint : public rq::SimpleSymbol {

};

struct TypeConstraint final : public rq::FundamentalConstraint {

};

struct RangeConstraint final : public rq::FundamentalConstraint {

};

struct NumericConstraint final : public rq::FundamentalConstraint {

};

struct SignedConstraint final : public rq::FundamentalConstraint {

};

struct UnsignedConstraint final : public rq::FundamentalConstraint {

};

struct IntegerConstraint final : public rq::FundamentalConstraint {

};

struct SignedIntegerConstraint final : public rq::FundamentalConstraint {

};

struct UnsignedIntegerConstraint final : public rq::FundamentalConstraint {

};

struct FloatConstraint final : public rq::FundamentalConstraint {

};

struct BinaryConstraint final : public rq::FundamentalConstraint {

};

struct BfloatConstraint final : public rq::FundamentalConstraint {

};

struct StringConstraint final : public rq::FundamentalConstraint {

};

struct CodeunitConstraint final : public rq::FundamentalConstraint {

};

struct ExpressionAttributeConstraint final : public rq::FundamentalConstraint {

};

struct TypeAttributeConstraint final : public rq::FundamentalConstraint {

};

struct ExpressionAttributeType : public rq::SimpleSymbol {

};

struct LabelingType final : public rq::ExpressionAttributeType {

};

struct VisibilityType final : public rq::ExpressionAttributeType {

};

struct ScopingType final : public rq::ExpressionAttributeType {

};

struct AvailabilityType final : public rq::ExpressionAttributeType {

};

struct PropertyMutabilityType final : public rq::ExpressionAttributeType {

};

struct ExportingType final : public rq::ExpressionAttributeType {

};

struct GenerationTimeType final : public rq::ExpressionAttributeType {

};

struct CapturingType final : public rq::ExpressionAttributeType {

};

struct EvaluationTimeType final : public rq::ExpressionAttributeType {

};

struct InliningType final : public rq::ExpressionAttributeType {

};

struct ManglingType final : public rq::ExpressionAttributeType {

};

struct PackingType final : public rq::ExpressionAttributeType {

};

struct TemplatingType final : public rq::ExpressionAttributeType {

};

struct LikelyhoodType final : public rq::ExpressionAttributeType {

};

struct SupportType final : public rq::ExpressionAttributeType {

};

struct CopyabilityType final : public rq::ExpressionAttributeType {

};

struct AddressStabilityType final : public rq::ExpressionAttributeType {

};

struct CleanupType final : public rq::ExpressionAttributeType {

};

struct TypeAttributeType : public rq::SimpleSymbol {

};

struct MutabilityType final : public rq::TypeAttributeType {

};

struct VolatilityType final : public rq::TypeAttributeType {

};

struct DeterminicityType final : public rq::TypeAttributeType {

};

struct AtomicityType final : public rq::TypeAttributeType {

};

struct NullTerminationType final : public rq::TypeAttributeType {

};

struct PreconditionType final : public rq::TypeAttributeType {

};

struct PostconditionType final : public rq::TypeAttributeType {

};

struct ReflectiveType : public rq::SimpleSymbol {

};

struct SymbolType final : public rq::ReflectiveType {

};

struct ExpressionType final : public rq::ReflectiveType {

};

struct UnscaledPrimitiveType : public rq::SimpleSymbol {

};

struct BooleanType final : public rq::UnscaledPrimitiveType {

};

struct HalfType final : public rq::UnscaledPrimitiveType {

};

struct SingleType final : public rq::UnscaledPrimitiveType {

};

struct DoubleType final : public rq::UnscaledPrimitiveType {

};

struct QuadrupleType final : public rq::UnscaledPrimitiveType {

};

struct Binary16Type final : public rq::UnscaledPrimitiveType {

};

struct Binary32Type final : public rq::UnscaledPrimitiveType {

};

struct Binary64Type final : public rq::UnscaledPrimitiveType {

};

struct Binary128Type final : public rq::UnscaledPrimitiveType {

};

struct Bfloat16Type final : public rq::UnscaledPrimitiveType {

};

struct AsciiType final : public rq::UnscaledPrimitiveType {

};

struct Utf8Type final : public rq::UnscaledPrimitiveType {

};

struct SignedIndexType final : public rq::UnscaledPrimitiveType {

};

struct UnsignedIndexType final : public rq::UnscaledPrimitiveType {

};

struct SignedAddressType final : public rq::UnscaledPrimitiveType {

};

struct UnsignedAddressType final : public rq::UnscaledPrimitiveType {

};

struct VariadicArgumentType final : public rq::SimpleSymbol {

};

struct ScaledIntegerType : public rq::Symbol {

};

struct ScaledSignedIntegerType final : public rq::ScaledIntegerType {

};

struct ScaledUnsignedIntegerType final : public rq::ScaledIntegerType {

};

struct UncountedSubtype : public rq::SimpleSymbol {

};

struct ReferenceSubtype final : public rq::UncountedSubtype {

};

struct PointerSubtype final : public rq::UncountedSubtype {

};

struct FatPointerSubtype final : public rq::UncountedSubtype {

};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {

};

struct Array final : public rq::SimpleSymbol {

};

struct Module final : public rq::SimpleSymbol {

};

struct Import final : public rq::SimpleSymbol {

};

struct ConcatenatedString final : public rq::SimpleSymbol {

};

struct ArithmeticSequence : public rq::SimpleSymbol {

};

struct ArithmeticInterval final : public rq::ArithmeticSequence {

};

struct InfiniteArithmeticSequence final : public rq::ArithmeticSequence {

};

struct FiniteArithmeticSequence final : public rq::ArithmeticSequence {

};

struct LocalDeclaration : public rq::Symbol {

};

struct Label final : public rq::LocalDeclaration {

};

struct LocalVariable : public rq::LocalDeclaration {

};

struct LocalDynamicVariable final : public rq::LocalVariable {

};

struct LocalStaticVariable final : public rq::LocalVariable {

};

struct Parameter : public rq::LocalVariable {

};

struct SignatureParameter final : public rq::Parameter {

};

struct ClassParameter final : public rq::Parameter {

};

struct TemplateParameter final : public rq::Parameter {

};

struct Signature final : public rq::SimpleSymbol {

};

struct Composition final : public rq::SimpleSymbol {

};

struct Synonym final : public rq::SimpleSymbol {

};

struct Polymorph : public rq::SimpleSymbol {

};

struct RangerPolymorph final : public rq::Polymorph {

};

struct ProcedurePolymorph final : public rq::Polymorph {

};

struct ClassPolymorph final : public rq::Polymorph {

};

struct EnumeratorPolymorph final : public rq::Polymorph {

};

struct InterfacePolymorph final : public rq::Polymorph {

};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {

};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {

};

struct SymbolTable : public rq::Symbol {

};

struct Top final : public rq::SymbolTable {

};

struct Scope final : public rq::SymbolTable {

};

struct GlobalDeclaration : public rq::SymbolTable {

};

struct Namespace final : public rq::GlobalDeclaration {

};

struct Class final : public rq::GlobalDeclaration {

};

struct Enumeration final : public rq::GlobalDeclaration {

};

struct Interface final : public rq::GlobalDeclaration {

};

struct GlobalVariable : public rq::GlobalDeclaration {

};

struct GlobalDynamicVariable final : public rq::GlobalVariable {

};

struct GlobalStaticVariable final : public rq::GlobalVariable {

};

struct Ranger : public rq::GlobalDeclaration {

};

struct ForwardRanger final : public rq::Ranger {

};

struct BackwardRanger final : public rq::Ranger {

};

struct Procedure : public rq::GlobalDeclaration {

};

struct Entry final : public rq::Procedure {

};

struct Function final : public rq::Procedure {

};

struct Method final : public rq::Procedure {

};

struct ExtensionFunction final : public rq::Procedure {

};

struct ExtensionMethod final : public rq::Procedure {

};

struct Template : public rq::SymbolTable {

};

struct ClassTemplate final : public rq::Template {

};

struct EnumerationTemplate final : public rq::Template {

};

struct InterfaceTemplate final : public rq::Template {

};

struct GlobalDynamicVariableTemplate final : public rq::Template {

};

struct GlobalStaticVariableTemplate final : public rq::Template {

};

struct ForwardRangerTemplate final : public rq::Template {

};

struct BackwardRangerTemplate final : public rq::Template {

};

struct FunctionTemplate final : public rq::Template {

};

struct MethodTemplate final : public rq::Template {

};

struct ExtensionFunctionTemplate final : public rq::Template {

};

struct ExtensionMethodTemplate final : public rq::Template {

};

struct Specialization : public rq::SymbolTable {

};

struct ClassSpecialization final : public rq::Specialization {

};

struct EnumerationSpecialization final : public rq::Specialization {

};

struct InterfaceSpecialization final : public rq::Specialization {

};

struct GlobalDynamicVariableSpecialization final : public rq::Specialization {

};

struct GlobalStaticVariableSpecialization final : public rq::Specialization {

};

struct ForwardRangerSpecialization final : public rq::Specialization {

};

struct BackwardRangerSpecialization final : public rq::Specialization {

};

struct FunctionSpecialization final : public rq::Specialization {

};

struct MethodSpecialization final : public rq::Specialization {

};

struct ExtensionFunctionSpecialization final : public rq::Specialization {

};

struct ExtensionMethodSpecialization final : public rq::Specialization {

};

} // namespace rq