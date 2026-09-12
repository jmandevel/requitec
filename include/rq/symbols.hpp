#pragma once

#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/iterators.hpp>
#include <rq/node_list.hpp>
#include <rq/static_value.hpp>
#include <rq/tokens.hpp>
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

#include <compare>
#include <ranges>
#include <vector>

namespace llvm {
class Value;
}

namespace rq {

struct ConstantSymbol;
struct CfgBlock;
struct Instruction;

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind);

enum class SymbolInfoFlags : std::uint64_t {
  NONE = 0,

  // SYMBOL CLASSIFICATION
  SIMPLE_SYMBOL = rq::getBit(0),
  LITERAL = rq::getBit(1),
  REFLECTIVE_TYPE = rq::getBit(2),
  FITTING_PRIMITIVE_TYPE = rq::getBit(3),
  STANDARD_PRIMITIVE_TYPE = rq::getBit(4),
  PLATFORM_PRIMITIVE_TYPE = rq::getBit(5),
  MODIFIER_TYPE = rq::getBit(6),
  QUALIFIER_TYPE = rq::getBit(7),
  SCALED_PRIMITIVE_TYPE = rq::getBit(8),
  SUBTYPE = rq::getBit(9),
  ARITHMETIC_SEQUENCE_TYPE = rq::getBit(10),
  SPECIALIZATION_SET = rq::getBit(11),
  PARAMETER_LIST = rq::getBit(12),
  TABLE_MEMBER = rq::getBit(13),
  LOCAL_DECLARATION = rq::getBit(14),
  POLYMORPH = rq::getBit(15),
  WEIGHT_LEVEL = rq::getBit(16),
  TEMPLATE = rq::getBit(17),
  SYMBOL_TABLE = rq::getBit(18),
  LOCAL_SCOPE = rq::getBit(19),
  NAMED_TABLE = rq::getBit(20),
  GLOBAL_DECLARATION = rq::getBit(21),
  IMPLEMENTATION = rq::getBit(22),
  CLASS_IMPLEMENTATION = rq::getBit(23),
  ENUM_IMPLEMENTATION = rq::getBit(24),
  INTERFACE_IMPLEMENTATION = rq::getBit(25),
  ADAPTER_IMPLEMENTATION = rq::getBit(26),
  CONSTRUCTOR_IMPLEMENTATION = rq::getBit(27),
  FUNCTION_IMPLEMENTATION = rq::getBit(28),
  GLOBAL_VARIABLE_IMPLEMENTATION = rq::getBit(29),

  // SYMBOL DETAIL
  IS_TYPE = rq::getBit(53),
  IS_SIGNED_TYPE = rq::getBit(54),
  IS_UNSIGNED_TYPE = rq::getBit(55),
  IS_INTEGER_TYPE = rq::getBit(56),
  IS_FLOAT_TYPE = rq::getBit(57),
  IS_BINARY_TYPE = rq::getBit(58),
  IS_BFLOAT_TYPE = rq::getBit(59),
  IS_CODEUNIT_TYPE = rq::getBit(60),
  IS_FRAME_SCOPE = rq::getBit(61),
  IS_OBJECT_SCOPE = rq::getBit(62)
};

RQ_DEFINE_FLAGS(rq::SymbolInfoFlags);

[[nodiscard]] inline rq::SymbolInfoFlags getInfoFlags(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralSymbol(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFittingPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardFittingType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsQualifierType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsModifierType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecializationSet(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalScope(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDeclarataion(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsClassImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsEnumImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsInterfaceImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsAdapterImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsConstructorImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFunctionImplementation(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalVariableImplementation(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope(rq::SymbolKind kind);

enum class EvaluationState : std::uint_fast8_t {
  NONE,
  SURVEYED,
  DECLARING,
  DECLARED,
  IMPLEMENTING,
  IMPLEMENTED,
  ERROR
};

[[nodiscard]] RQ_ALWAYS_INLINE auto operator<=>(rq::EvaluationState rhs,
                                                rq::EvaluationState lhs) {
  return rq::getUnderlyingValue(rhs) <=> rq::getUnderlyingValue(lhs);
}

// clang-format off
struct Symbol;
  struct SimpleSymbol;
    struct Literal;
      struct IntegerLiteral;
      struct FloatLiteral;
      struct StringLiteral;
      struct CodeunitLiteral;
    struct Contextual;
      struct ContextualType;
        struct InferenceType;
        struct VoidType;
        struct NoReturnType;
        struct UnknownType;
      struct ContextualValue;
        struct UnknownValue;
        struct ValueValue;
        struct IndexValue;
    struct ReflectiveType;
      struct SymbolType;
      struct ExpressionType;
      struct SymbolRangeType;
      struct ExpressionRangeType;
    struct PrimitiveType;
      struct FittingPrimitiveType;
        struct FastSignedIntegerType;
        struct FastUnsignedIntegerType;
        struct LeastSignedIntegerType;
        struct LeastUnsignedIntegerType;
      struct StandardPrimitiveType;
        struct Binary16Type;
        struct Binary32Type;
        struct Binary64Type;
        struct Binary128Type;
        struct BFloat16Type;
        struct AsciiType;
        struct Utf8Type;
      struct PlatformPrimitiveType;
        struct HalfType;
        struct SingleType;
        struct DoubleType;
        struct QuadrupleType;
        struct BooleanType;
        struct UnsignedIntegerType;
        struct SignedIntegerType;
        struct UnsignedIndexType;
        struct SignedIndexType;
        struct UnsignedAddressType;
        struct SignedAddressType;
        struct CharType;
    struct QualifierType;
      struct VarQualifierType;
      struct VolatileQualifierType;
      struct AtomicQualifierType;
      struct NullTerminateQualifierType;
    struct ModifierType;
      struct AnchorModifierType;
      struct ContainerModifierType;
      struct VisibilityModifierType;
      struct AccessModifierType;
      struct MutateModifierType;
      struct CohortModifierType;
      struct CaptureModifierType;
      struct LinkageModifierType;
      struct MangleModifierType;
      struct PackModifierType;
      struct BranchTrendModifierType;
      struct SupportNoticeModifierType;
      struct StableAddressModifierType;
      struct VariadicModifierType;
      struct OffsetModifierType;
      struct TemplateModifierType;
      struct ConstraintModifierType;
      struct WeightModifierType;
      struct DeductionModifierType;
      struct VirtualityModifierType;
      struct RangerModifierType;
      struct RequireModifierType;
      struct EnsureModifierType;
  struct Subtype;
    struct ArraySubtype;
    struct ReferenceSubtype;
    struct PointerSubtype;
    struct InferenceCountArraySubtype;
    struct GreatestSubtype;
    struct SliceSubtype;
  struct ScaledPrimitiveType;
    struct ScaledUnsignedIntegerType;
    struct ScaledSignedIntegerType;
  struct Adaption;
  struct Conformity;
  struct JuxtListItem;
  struct JuxtListType;
  struct SynonymType;
  struct Module;
  struct ArithmeticSequenceType;
    struct ArithmeticIntervalType;
    struct InfiniteArithmeticSequenceType;
    struct FiniteArithmeticSequenceTYpe;
  struct Import;
  struct SpecializationSetArgument;
  struct SpecializationSet;
    struct FunctionSpecializationSet;
    struct AdapterSpecializationSet;
  struct Parameter;
  struct CompositionComponent;
  struct CompositionType;
  struct ParameterList;
    struct SignatureType;
    struct LayoutType;
  struct PlacementType;
  struct WeightLevel;
    struct InterfaceWeightLevel;
    struct FunctionWeightLevel;
    struct AdapterWeightLevel;
    struct EnumWeightLevel;
    struct ClassWeightLevel;
    struct GlobalVariableWeightLevel;
  struct Polymorph;
    struct EnumPolymorph;
    struct ClassPolymorph;
    struct InterfacePolymorph;
    struct AdapterPolymorph;
    struct FunctionPolymorph;
    struct GlobalVariablePolymorph;
  struct TableMember;
    struct LocalDeclaration;
      struct Anchor;
      struct Enumerator;
      struct LocalVariable;
    struct SymbolTable;
      struct CTable;
      struct TopTable;
      struct LocalScope;
        struct ScopeStatement;
        struct WeaveStatement;
        struct SpinStatement;
        struct WhileStatement;
        struct ForStatement;
        struct DefaultStatement;
        struct CaseStatement;
        struct SwitchStatement;
        struct IfStatement;
        struct ElseIfStatement;
        struct ElseStatement;
      struct NamedTable;
        struct Namespace;
        struct GlobalDeclaration;
          struct Implementation;
            struct EnumImplementation;
              struct EnumOverload;
              struct EnumSpecialization;
            struct GlobalVariableImplementation;
              struct GlobalVariableOverload;
              struct GlobalVariableSpecialization;
            struct AdapterImplementation;
              struct AdapterOverload;
              struct AdapterSpecialization;
            struct InterfaceImplementation;
              struct InterfaceOverload;
              struct InterfaceSpecialization;
            struct ConstructorOverload;
              struct LayoutConstructorOverload;
            struct ClassImplementation;
              struct ClassOverload;
              struct ClassSpecialization;
            struct FunctionImplementation;
              struct FunctionOverload;
              struct FunctionSpecialization;
          struct Template;
            struct EnumTemplate;
            struct ClassTemplate;
            struct InterfaceTemplate;
            struct GlobalVariableTemplate;
            struct AdapterTemplate;
            struct FunctionTemplate;
// clang-format on

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind getKind() const;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getDerivedExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDerivedExpressionPtr() const;

  [[nodiscard]] inline rq::SymbolInfoFlags getInfoFlags();

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFittingPrimitiveType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStandardFittingType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformPrimitiveType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsQualifierType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModifierType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledPrimitiveType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequenceType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecializationSet();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalScope();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclarataion();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInterfaceImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAdapterImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstructorImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunctionImplementation();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalVariableImplementation();

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SimpleSymbol : public rq::Symbol {
  using Self = rq::SimpleSymbol;

  explicit RQ_ALWAYS_INLINE SimpleSymbol(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Literal : public rq::SimpleSymbol {
  using Self = rq::Literal;

  explicit RQ_ALWAYS_INLINE Literal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IntegerLiteral final : public rq::Literal {
  using Self = rq::IntegerLiteral;

  explicit RQ_ALWAYS_INLINE IntegerLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FloatLiteral final : public rq::Literal {
  using Self = rq::FloatLiteral;

  explicit RQ_ALWAYS_INLINE FloatLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StringLiteral final : public rq::Literal {
  using Self = rq::StringLiteral;

  explicit RQ_ALWAYS_INLINE StringLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CodeunitLiteral final : public rq::Literal {
  using Self = rq::CodeunitLiteral;

  explicit RQ_ALWAYS_INLINE CodeunitLiteral();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Contextual : public rq::SimpleSymbol {
  using Self = rq::Contextual;

  explicit RQ_ALWAYS_INLINE Contextual(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContextualType : public rq::Contextual {
  using Self = rq::ContextualType;

  explicit RQ_ALWAYS_INLINE ContextualType(rq::SymbolKind kind);

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

struct UnknownType final : public rq::ContextualType {
  using Self = rq::UnknownType;

  explicit RQ_ALWAYS_INLINE UnknownType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;

  explicit RQ_ALWAYS_INLINE ContextualValue(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnknownValue final : public rq::ContextualValue {
  using Self = rq::UnknownValue;

  explicit RQ_ALWAYS_INLINE UnknownValue();

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

struct ReflectiveType : public rq::SimpleSymbol {
  using Self = rq::ReflectiveType;

  explicit RQ_ALWAYS_INLINE ReflectiveType(rq::SymbolKind kind);

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

struct SymbolRangeType final : public rq::ReflectiveType {
  using Self = rq::SymbolRangeType;

  explicit RQ_ALWAYS_INLINE SymbolRangeType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExpressionRangeType final : public rq::ReflectiveType {
  using Self = rq::ExpressionRangeType;

  explicit RQ_ALWAYS_INLINE ExpressionRangeType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PrimitiveType : public rq::SimpleSymbol {
  using Self = rq::PrimitiveType;

  explicit RQ_ALWAYS_INLINE PrimitiveType(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FittingPrimitiveType : public rq::PrimitiveType {
  using Self = rq::FittingPrimitiveType;

  explicit RQ_ALWAYS_INLINE FittingPrimitiveType(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FastSignedIntegerType final : public rq::FittingPrimitiveType {
  using Self = rq::FastSignedIntegerType;

  explicit RQ_ALWAYS_INLINE FastSignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FastUnsignedIntegerType final : public rq::FittingPrimitiveType {
  using Self = rq::FastUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE FastUnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LeastSignedIntegerType final : public rq::FittingPrimitiveType {
  using Self = rq::LeastSignedIntegerType;

  explicit RQ_ALWAYS_INLINE LeastSignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LeastUnsignedIntegerType final : public rq::FittingPrimitiveType {
  using Self = rq::LeastUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE LeastUnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StandardPrimitiveType : public rq::PrimitiveType {
  using Self = rq::StandardPrimitiveType;

  explicit RQ_ALWAYS_INLINE StandardPrimitiveType();

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

struct BFloat16Type final : public rq::StandardPrimitiveType {
  using Self = rq::BFloat16Type;

  explicit RQ_ALWAYS_INLINE BFloat16Type();

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

struct PlatformPrimitiveType : public rq::PrimitiveType {
  using Self = rq::PlatformPrimitiveType;

  explicit RQ_ALWAYS_INLINE PlatformPrimitiveType(rq::SymbolKind kind);

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

struct BooleanType final : public rq::PlatformPrimitiveType {
  using Self = rq::BooleanType;

  explicit RQ_ALWAYS_INLINE BooleanType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE UnsignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedIntegerType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIntegerType;

  explicit RQ_ALWAYS_INLINE SignedIntegerType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedIndexType;

  explicit RQ_ALWAYS_INLINE UnsignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedIndexType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedIndexType;

  explicit RQ_ALWAYS_INLINE SignedIndexType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct UnsignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::UnsignedAddressType;

  explicit RQ_ALWAYS_INLINE UnsignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignedAddressType final : public rq::PlatformPrimitiveType {
  using Self = rq::SignedAddressType;

  explicit RQ_ALWAYS_INLINE SignedAddressType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CharType final : public rq::PlatformPrimitiveType {
  using Self = rq::CharType;

  explicit RQ_ALWAYS_INLINE CharType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct QualifierType : public rq::SimpleSymbol {
  using Self = rq::QualifierType;

  explicit RQ_ALWAYS_INLINE QualifierType(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VarQualifierType final : public rq::QualifierType {
  using Self = rq::VarQualifierType;

  explicit RQ_ALWAYS_INLINE VarQualifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VolatileQualifierType final : public rq::QualifierType {
  using Self = rq::VolatileQualifierType;

  explicit RQ_ALWAYS_INLINE VolatileQualifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AtomicQualifierType final : public rq::QualifierType {
  using Self = rq::AtomicQualifierType;

  explicit RQ_ALWAYS_INLINE AtomicQualifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NullTerminateQualifierType final : public rq::QualifierType {
  using Self = rq::NullTerminateQualifierType;

  explicit RQ_ALWAYS_INLINE NullTerminateQualifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ModifierType : public rq::SimpleSymbol {
  using Self = rq::ModifierType;

  explicit RQ_ALWAYS_INLINE ModifierType(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AnchorModifierType final : public rq::ModifierType {
  using Self = AnchorModifierType;

  explicit RQ_ALWAYS_INLINE AnchorModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContainerModifierType final : public rq::ModifierType {
  using Self = ContainerModifierType;

  explicit RQ_ALWAYS_INLINE ContainerModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VisibilityModifierType final : public rq::ModifierType {
  using Self = VisibilityModifierType;

  explicit RQ_ALWAYS_INLINE VisibilityModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AccessModifierType final : public rq::ModifierType {
  using Self = AccessModifierType;

  explicit RQ_ALWAYS_INLINE AccessModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MutateModifierType final : public rq::ModifierType {
  using Self = MutateModifierType;

  explicit RQ_ALWAYS_INLINE MutateModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CohortModifierType final : public rq::ModifierType {
  using Self = CohortModifierType;

  explicit RQ_ALWAYS_INLINE CohortModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CaptureModifierType final : public rq::ModifierType {
  using Self = CaptureModifierType;

  explicit RQ_ALWAYS_INLINE CaptureModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LinkageModifierType final : public rq::ModifierType {
  using Self = LinkageModifierType;

  explicit RQ_ALWAYS_INLINE LinkageModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MangleModifierType final : public rq::ModifierType {
  using Self = MangleModifierType;

  explicit RQ_ALWAYS_INLINE MangleModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PackModifierType final : public rq::ModifierType {
  using Self = PackModifierType;

  explicit RQ_ALWAYS_INLINE PackModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BranchTrendModifierType final : public rq::ModifierType {
  using Self = BranchTrendModifierType;

  explicit RQ_ALWAYS_INLINE BranchTrendModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SupportNoticeModifierType final : public rq::ModifierType {
  using Self = SupportNoticeModifierType;

  explicit RQ_ALWAYS_INLINE SupportNoticeModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StableAddressModifierType final : public rq::ModifierType {
  using Self = StableAddressModifierType;

  explicit RQ_ALWAYS_INLINE StableAddressModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VariadicModifierType final : public rq::ModifierType {
  using Self = VariadicModifierType;

  explicit RQ_ALWAYS_INLINE VariadicModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct OffsetModifierType final : public rq::ModifierType {
  using Self = OffsetModifierType;

  explicit RQ_ALWAYS_INLINE OffsetModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TemplateModifierType final : public rq::ModifierType {
  using Self = TemplateModifierType;

  explicit RQ_ALWAYS_INLINE TemplateModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstraintModifierType final : public rq::ModifierType {
  using Self = ConstraintModifierType;

  explicit RQ_ALWAYS_INLINE ConstraintModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WeightModifierType final : public rq::ModifierType {
  using Self = WeightModifierType;

  explicit RQ_ALWAYS_INLINE WeightModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DeductionModifierType final : public rq::ModifierType {
  using Self = DeductionModifierType;

  explicit RQ_ALWAYS_INLINE DeductionModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct VirtualityModifierType final : public rq::ModifierType {
  using Self = VirtualityModifierType;

  explicit RQ_ALWAYS_INLINE VirtualityModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct RangerModifierType final : public rq::ModifierType {
  using Self = RangerModifierType;

  explicit RQ_ALWAYS_INLINE RangerModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct RequireModifierType final : public rq::ModifierType {
  using Self = RequireModifierType;

  explicit RQ_ALWAYS_INLINE RequireModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnsureModifierType final : public rq::ModifierType {
  using Self = EnsureModifierType;

  explicit RQ_ALWAYS_INLINE EnsureModifierType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Subtype : public rq::SimpleSymbol, public llvm::FoldingSetNode {
  using Self = rq::Subtype;

  rq::ConstantSymbol *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::SymbolKind kind,
                                    rq::ConstantSymbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileSubtype(rq::SymbolKind kind,
                                     rq::ConstantSymbol &child);

struct ArraySubtype final : public rq::Subtype {
  using Self = rq::ArraySubtype;

  std::size_t _count;

  explicit RQ_ALWAYS_INLINE ArraySubtype(std::size_t count,
                                         rq::ConstantSymbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(rq::ConstantSymbol &child,
                                          std::size_t count);

struct ReferenceSubtype final : public rq::Subtype {
  using Self = rq::ReferenceSubtype;

  explicit RQ_ALWAYS_INLINE ReferenceSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PointerSubtype final : public rq::Subtype {
  using Self = rq::PointerSubtype;

  explicit RQ_ALWAYS_INLINE PointerSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InferenceCountArraySubtype final : public rq::Subtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE
  InferenceCountArraySubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GreatestSubtype final : public rq::Subtype {
  using Self = rq::GreatestSubtype;

  unsigned _greatest_depth;

  explicit RQ_ALWAYS_INLINE GreatestSubtype(unsigned greatest_depth,
                                            rq::ConstantSymbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getGreatestDepth() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileGreatestSubtype(llvm::FoldingSetNodeID &inout_id,
                                             rq::ConstantSymbol &type,
                                             unsigned greatest_depth);

struct SliceSubtype final : public rq::Subtype {
  using Self = rq::SliceSubtype;

  explicit RQ_ALWAYS_INLINE SliceSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ScaleKind : std::uint_fast8_t { EXACT, FAST, LEAST };

struct ScaledPrimitiveType : public rq::SimpleSymbol,
                             public llvm::FoldingSetNode {
  using Self = rq::ScaledPrimitiveType;

  rq::ScaleKind _scale_kind;
  unsigned _scale;
  unsigned _synonym_id;

  explicit RQ_ALWAYS_INLINE ScaledPrimitiveType(rq::SymbolKind kind,
                                                rq::ScaleKind scale_kind,
                                                unsigned scale,
                                                std::uint64_t synonym_id);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaleKind getScaleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getScale() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getSynonymId() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileScaledPrimitiveType(llvm::FoldingSetNodeID &inout_id,
                           rq::SymbolKind kind, rq::ScaleKind scale_kind,
                           unsigned scale, unsigned synonym_id);

struct ScaledUnsignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledUnsignedIntegerType;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ScaledSignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledSignedIntegerType;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Conformity final : public rq::SimpleSymbol, public llvm::FoldingSetNode {
  using Self = rq::Conformity;

  rq::InterfaceImplementation *_interface_ptr;
  rq::AdapterImplementation *_adapter_ptr;

  explicit RQ_ALWAYS_INLINE Conformity(rq::InterfaceImplementation &interface,
                                       rq::AdapterImplementation &adapter);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
  getAdapter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &getAdapter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

void profileConformity(llvm::FoldingSetNodeID &inout_id,
                       const rq::InterfaceImplementation &interface,
                       const rq::AdapterImplementation &adapter);

struct JuxtListItem final : public rq::SimpleSymbol,
                            public llvm::FoldingSetNode {
  using Self = rq::JuxtListItem;

  rq::JuxtListItem *_next_ptr;
  rq::ConstantSymbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE JuxtListItem(rq::JuxtListItem &next,
                                         rq::ConstantSymbol &type);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

void profileJustListItem(llvm::FoldingSetNodeID &inout_id,
                         const rq::JuxtListItem *next_ptr,
                         const rq::ConstantSymbol &type);

struct JuxtListType final : public rq::SimpleSymbol,
                            public llvm::FoldingSetNode {
  using Self = rq::JuxtListType;

  rq::JuxtListItem *_first_ptr;

  explicit RQ_ALWAYS_INLINE JuxtListType(rq::JuxtListItem &first);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::JuxtListItem &getFirst() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::JuxtListItem &getFirst();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

void profileJuxtListType(const rq::JuxtListItem *first_ptr);

struct SynonymType final : public rq::SimpleSymbol {
  using Self = rq::SynonymType;

  rq::Symbol *_original_ptr;

  explicit RQ_ALWAYS_INLINE SynonymType(rq::Symbol &original);

  [[nodiscard]] const rq::Symbol &getOriginal() const;
  [[nodiscard]] rq::Symbol &getOriginal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

struct ModuleDetail final {
  using Self = rq::ModuleDetail;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr{nullptr};
  llvm::StringRef _path;
  llvm::StringRef _buffer;
  std::vector<rq::Token> _tokens{};

  explicit RQ_ALWAYS_INLINE ModuleDetail(rq::ModuleKind kind,
                                         llvm::StringRef path,
                                         llvm::StringRef buffer);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getKind() const;
  RQ_ALWAYS_INLINE void setOrChangeExpression(rq::Expression *expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &getTokens();
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
  getTokens() const;
};

struct Module final : public rq::SimpleSymbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;
  rq::Import *_first_ptr{nullptr};
  llvm::SmallPtrSet<rq::Module *, 4> _imported_ptrs{};

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleDetail &&detail);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getImportsModule(const rq::Module &module) const;
  void addImport(rq::Import &import);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArithmeticSequenceType : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceType;

  rq::ConstantSymbol *_child_ptr;
  rq::ArithmeticCondition _condition_kind;
  rq::ArithmeticStep _step_kind;

  explicit RQ_ALWAYS_INLINE
  ArithmeticSequenceType(rq::SymbolKind kind, rq::ConstantSymbol &child,
                         rq::ArithmeticCondition condition,
                         rq::ArithmeticStep step);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticCondition getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticStep getStep() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
    const rq::ConstantSymbol &child, rq::ArithmeticCondition condition,
    rq::ArithmeticStep step);

struct ArithmeticIntervalType final : public rq::ArithmeticSequenceType {
  using Self = rq::ArithmeticIntervalType;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InfiniteArithmeticSequenceType final
    : public rq::ArithmeticSequenceType {
  using Self = rq::InfiniteArithmeticSequenceType;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FiniteArithmeticSequenceType final : public rq::ArithmeticSequenceType {
  using Self = rq::FiniteArithmeticSequenceTYpe;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Import final : public rq::SimpleSymbol {
  using Self = rq::Import;

  rq::Import *_next_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::Expression *_expression_ptr{nullptr};
  rq::Module *_imported_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Import();

  void setModifierFuseFlags(rq::ModifierFuseFlags flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  void setExpression(rq::Expression &expression);
  [[nodiscard]] const rq::Expression &getExpression() const;
  [[nodiscard]] rq::Expression &getExpression();
  void setImported(rq::Module &imported);
  [[nodiscard]] const rq::Module &getImported() const;
  [[nodiscard]] rq::Module &getImported();
  void setModule(rq::Module &module);
  [[nodiscard]] const rq::Module &getModule() const;
  [[nodiscard]] rq::Module &getModule();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Name final {
  using Self = rq::Name;

  // used for dense map operations. not for valid names.
  static constexpr rq::Keyword DENSE_MAP_KEY_EMPTY =
      rq::Keyword::INTEGER_LITERAL;
  static constexpr rq::Keyword DENSE_MAP_KEY_TOMBSTONE =
      rq::Keyword::FLOAT_LITERAL;

  llvm::StringRef _text{};
  rq::Keyword _keyword{rq::Keyword::NONE};

  explicit RQ_ALWAYS_INLINE Name() = default;
  explicit RQ_ALWAYS_INLINE Name(llvm::StringRef text) : _text(text) {}
  explicit RQ_ALWAYS_INLINE Name(rq::Keyword keyword) : _keyword(keyword) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_text.empty() && this->_keyword == rq::Keyword::NONE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getText() const {
    return this->_text;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getKeyword() const {
    return this->_keyword;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const rq::Name &rhs) const {
    return this->getKeyword() == rhs.getKeyword() &&
           this->getText() == rhs.getText();
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const rq::Name &rhs) const {
    return this->getKeyword() != rhs.getKeyword() &&
           this->getText() != rhs.getText();
  }

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const {
    inout_id.AddString(this->getText());
    inout_id.AddInteger(rq::getUnderlyingValue(this->getKeyword()));
  }
};

struct SpecializationSetArgument final : public rq::SimpleSymbol,
                                         public llvm::FoldingSetNode {
  using Self = rq::SpecializationSetArgument;

  rq::Name _name{};
  rq::Entity *_value_ptr{nullptr};
  rq::SpecializationSetArgument *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  SpecializationSetArgument(rq::Name name, rq::Entity &value,
                            rq::SpecializationSetArgument *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileSpecializationSetArgument(llvm::FoldingSetNodeID &inout_id,
                                 rq::Name name, rq::Entity &value,
                                 rq::SpecializationSetArgument *next_ptr);

struct SpecializationSet : public rq::SimpleSymbol,
                           public llvm::FoldingSetNode {
  using Self = rq::SpecializationSet;

  rq::SpecializationSetArgument *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  SpecializationSet(rq::SpecializationSetArgument *first_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileSpecializationSet(llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
                         rq::SpecializationSetArgument *first_ptr);

struct FunctionSpecializationSet final : public rq::SpecializationSet {
  using Self = rq::FunctionSpecializationSet;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterSpecializationSet final : public rq::SpecializationSet {
  using Self = rq::AdapterSpecializationSet;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ParameterInfoFlags : std::uint_fast8_t {
  NONE = 0,

  POSITIONAL = rq::getBit(0),
  NONPOSITIONAL = rq::getBit(1),
  LOCKED = rq::getBit(2)
};

struct ParameterDetail final {
  using Self = rq::ParameterDetail;

  rq::Name _name;
  rq::ConstantSymbol *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::CfgBlock *_default_ptr;

  explicit RQ_ALWAYS_INLINE
  ParameterDetail(rq::Name name, rq::ConstantSymbol &type,
                  rq::ModifierFuseFlags modifier_fuse_flags,
                  rq::ParameterInfoFlags param_info_flags,
                  rq::CfgBlock *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] const rq::ConstantSymbol &getType() const;
  [[nodiscard]] rq::ConstantSymbol &getType();
  [[nodiscard]] rq::ModifierFuseFlags getModifierFuseFlags() const;
  [[nodiscard]] rq::ParameterInfoFlags getParameterInfoFlags() const;
  [[nodiscard]] const rq::CfgBlock *getDefaultPtr() const;
  [[nodiscard]] rq::CfgBlock *getDefaultPtr();
};

struct Parameter final : public rq::SimpleSymbol, public llvm::FoldingSetNode {
  using Self = rq::Parameter;

  rq::Parameter *_next_ptr;
  rq::Name _name;
  rq::ConstantSymbol *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::CfgBlock *_default_ptr;

  explicit RQ_ALWAYS_INLINE Parameter(rq::Parameter *next_ptr, rq::Name name,
                                      rq::ConstantSymbol &type,
                                      rq::ModifierFuseFlags modifier_flags,
                                      rq::ParameterInfoFlags param_flags,
                                      rq::CfgBlock *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
  getParameterFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CfgBlock *getDefaultPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CfgBlock *getDefaultPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileParameter(
    llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
    const rq::Parameter *next_ptr, rq::Name name,
    const rq::ConstantSymbol &type, rq::ModifierFuseFlags modifier_fuse_flags,
    rq::ParameterInfoFlags param_flags, const rq::CfgBlock *default_ptr);

struct CompositionComponent final : public rq::SimpleSymbol,
                                    public llvm::FoldingSetNode {
  using Self = rq::CompositionComponent;

  rq::CompositionComponent *_next_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionComponent(rq::CompositionComponent *next_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionComponent(llvm::FoldingSetNodeID &inout_id,
                            rq::CompositionComponent *next_ptr);

struct CompositionType final : public rq::SimpleSymbol {
  using Self = rq::CompositionType;

  rq::CompositionComponent *_first_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionType(rq::CompositionComponent *first_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionType(llvm::FoldingSetNodeID &inout_id,
                       rq::CompositionComponent *next_ptr);

struct ParameterListDetail final {
  using Self = ParameterListDetail;

  llvm::SmallVector<rq::ParameterDetail, 2> _parameter_detail_list{};
  rq::ParameterInfoFlags _flags{rq::ParameterInfoFlags::POSITIONAL};
  rq::ParameterInfoFlags _found_parameter_marks{rq::ParameterInfoFlags::NONE};

  explicit RQ_ALWAYS_INLINE ParameterListDetail() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getFoundPositionalParametersEnd() const;
  RQ_ALWAYS_INLINE void appendPositionalParametersEnd();
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getFoundNonpositionalParametersBegin() const;
  RQ_ALWAYS_INLINE void appendNonpositionalParametersBegin();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getFoundLockedParametersBegin() const;
  RQ_ALWAYS_INLINE void appendLockedParametersBegin();
  RQ_ALWAYS_INLINE void
  appendParameter(rq::Name name, rq::ConstantSymbol &type,
                  rq::ModifierFuseFlags modifier_fuse_flags,
                  rq::Expression *default_value_expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE std::span<const rq::ParameterDetail>
  getParameterDetailSpan() const;
};

struct ParameterList : public rq::SimpleSymbol {
  using Self = rq::ParameterList;

  rq::Parameter *_first_ptr;

  explicit RQ_ALWAYS_INLINE ParameterList(rq::SymbolKind kind,
                                          rq::Parameter *first_parameter_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
  getParameterSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
  getConstParameterSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Parameter>
  getParameterSubrange();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignatureType final : public rq::ParameterList,
                             public llvm::FoldingSetNode {
  using Self = rq::SignatureType;

  rq::ConstantSymbol *_return_type_ptr;

  explicit RQ_ALWAYS_INLINE SignatureType(rq::Parameter *first_parameter_ptr,
                                          rq::ConstantSymbol &return_type);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getReturnType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileSignatureType(rq::Parameter *first_parameter_ptr,
                                           rq::ConstantSymbol &return_type);

struct LayoutType final : public rq::ParameterList {
  using Self = rq::LayoutType;

  explicit RQ_ALWAYS_INLINE LayoutType(rq::Parameter *first_parameter_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileLayoutType(rq::Parameter *first_parameter_ptr);

struct PlacementType final : public rq::SimpleSymbol,
                             public llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::FunctionImplementation *_function_ptr;

  explicit RQ_ALWAYS_INLINE PlacementType(rq::FunctionImplementation &function);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profilePlacementType(rq::FunctionImplementation &function);

struct WeightLevel : public rq::SimpleSymbol {
  using Self = rq::WeightLevel;

  rq::Polymorph *_polymorph_ptr{nullptr};
  unsigned _weight;
  rq::Template *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE WeightLevel(rq::SymbolKind kind, unsigned weight);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph *getPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph *getPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getWeight() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template>
  getTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
  getTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
  getConstTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceWeightLevel final : public rq::WeightLevel {
  using Self = rq::InterfaceWeightLevel;

  explicit RQ_ALWAYS_INLINE InterfaceWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void
  setInterfacePolymorph(rq::InterfacePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfacePolymorph *
  getInterfacePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfacePolymorph *
  getInterfacePolymorphPtr();
  RQ_ALWAYS_INLINE void addInterfaceTemplate(rq::InterfaceTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::InterfaceTemplate>
      getInterfaceTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
      getInterfaceTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
      getConstInterfaceTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionWeightLevel final : public rq::WeightLevel {
  using Self = rq::FunctionWeightLevel;

  explicit RQ_ALWAYS_INLINE FunctionWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void setFunctionPolymorph(rq::FunctionPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionPolymorph *
  getFunctionPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionPolymorph *
  getFunctionPolymorphPtr() const;
  RQ_ALWAYS_INLINE void addFunctionTemplate(rq::FunctionTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::FunctionTemplate>
      getFunctionTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>
      getFunctionTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>
      getConstFunctionTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterWeightLevel final : public rq::WeightLevel {
  using Self = rq::AdapterWeightLevel;

  explicit RQ_ALWAYS_INLINE AdapterWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void setAdapterPolylmorph(rq::AdapterPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterPolymorph *getAdapterPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterPolymorph *
  getAdapterPolymorphPtr() const;
  RQ_ALWAYS_INLINE void addAdapterTemplate(rq::AdapterTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::AdapterTemplate>
      getAdapterTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
      getAdapterTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
      getConstAdapterTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumWeightLevel final : public rq::WeightLevel {
  using Self = rq::EnumWeightLevel;

  explicit RQ_ALWAYS_INLINE EnumWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void setEnumPolymorph(rq::EnumPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *getEnumPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
  getEnumPolymorphPtr() const;
  RQ_ALWAYS_INLINE void addEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::EnumTemplate>
      getEnumTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
      getEnumTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
      getEnumTemplateConstSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassWeightLevel final : public rq::WeightLevel {
  using Self = rq::ClassWeightLevel;

  explicit RQ_ALWAYS_INLINE ClassWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void setClassPolymorph(rq::ClassPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassPolymorph *getClassPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassPolymorph *
  getClassPolymorphPtr() const;
  RQ_ALWAYS_INLINE void addClassTemplate(rq::ClassTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
      getConstClassTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalVariableWeightLevel final : public rq::WeightLevel {
  using Self = rq::GlobalVariableWeightLevel;

  explicit RQ_ALWAYS_INLINE GlobalVariableWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void
  setGlobalVariablePolymorph(rq::GlobalVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalVariablePolymorph *
  getGlobalVariablePolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalVariablePolymorph *
  getGlobalVariablePolymorphPtr() const;
  RQ_ALWAYS_INLINE void
  addGlobalVariableTemplate(rq::GlobalVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::GlobalVariableTemplate>
      getGlobalVariableTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::GlobalVariableTemplate>
      getGlobalVariableTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::GlobalVariableTemplate>
      getGlobalVariableTemplateConstSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct Polymorph : public rq::SimpleSymbol {
  using Self = rq::Polymorph;

  rq::Implementation *_first_overload_ptr{nullptr};
  rq::WeightLevel *_first_weight_level_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Implementation>
  getOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
  getOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
  getConstOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::WeightLevel>
  getWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
  getWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
  getConstWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  explicit RQ_ALWAYS_INLINE ClassPolymorph(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void addClassOverload(rq::ClassOverload &class_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::ClassOverload>
      getClassOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
      getClassOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
      getConstClassOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addClassWeightLevel(rq::ClassWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::ClassWeightLevel>
      getClassWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ClassWeightLevel>
      getClassWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ClassWeightLevel>
      getConstClassWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumPolymorph final : public rq::Polymorph {
  using Self = rq::EnumPolymorph;

  explicit RQ_ALWAYS_INLINE EnumPolymorph(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void addEnumOverload(rq::EnumOverload &class_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadConstSubrange() const;
  RQ_ALWAYS_INLINE void addEnumWeightLevel(rq::EnumWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::EnumWeightLevel>
      getEnumWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumWeightLevel>
      getEnumWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumWeightLevel>
      getEnumWeightLevelConstSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  explicit RQ_ALWAYS_INLINE InterfacePolymorph(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void addInterfaceOverload(rq::InterfaceOverload &interface);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>
      getInterfaceOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
      getInterfaceOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
      getConstInterfaceOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addInterfaceWeightLevel(rq::InterfaceWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::InterfaceWeightLevel>
      getInterfaceWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::InterfaceWeightLevel>
      getInterfaceWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::InterfaceWeightLevel>
      getConstInterfaceWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalVariablePolymorph;

  explicit RQ_ALWAYS_INLINE GlobalVariablePolymorph();

  RQ_ALWAYS_INLINE void
  addGlobalVariableOverload(rq::GlobalVariableOverload &variable);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::GlobalVariableOverload>
      getGlobalVariableOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::GlobalVariableOverload>
      getGlobalVariableOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::GlobalVariableOverload>
      getGlobalVariableOverloadConstSubrange() const;
  RQ_ALWAYS_INLINE void
  addGlobalVariableWeightLevel(rq::GlobalVariableWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::GlobalVariableWeightLevel>
      getGlobalVariableWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::GlobalVariableWeightLevel>
      getGlobalVariableWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::GlobalVariableWeightLevel>
      getGlobalVariableWeightLevelConstSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterPolymorph final : public rq::Polymorph {
  using Self = rq::AdapterPolymorph;

  explicit RQ_ALWAYS_INLINE AdapterPolymorph();

  RQ_ALWAYS_INLINE void addAdapterOverload(rq::AdapterOverload &adapter);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::AdapterOverload>
      getAdapterOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
      getAdapterOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
      getConstAdapterOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addAdapterWeightLevel(rq::AdapterWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getAdapterWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getAdapterWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getConstAdapterWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionPolymorph final : public rq::Polymorph {
  using Self = rq::FunctionPolymorph;

  explicit RQ_ALWAYS_INLINE FunctionPolymorph();
  RQ_ALWAYS_INLINE void addFunctionOverload(rq::FunctionOverload &function);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::FunctionOverload>
      getFunctionOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::FunctionOverload>
      getFunctionOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::FunctionOverload>
      getConstFunctionOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addFunctionWeightLevel(rq::FunctionWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
      getFunctionWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
      getFunctionWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
      getConstFunctionWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct TableMember : public rq::SimpleSymbol {
  using Self = rq::TableMember;

  rq::SymbolTable *_container_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE TableMember(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setContainer(rq::SymbolTable &container);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getContainerPtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct LocalDeclaration : public rq::TableMember {
  using Self = rq::LocalDeclaration;

  rq::Name _name{};
  rq::SymbolTable *_host_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalDeclaration(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  RQ_ALWAYS_INLINE void setHost(rq::SymbolTable &host);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getHostPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getHostPtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::ConstantSymbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalVariable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void
  setLowFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *getTypePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct LocalDynamicVariable : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  llvm::Value *_llvm_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalDynamicVariable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLlvmValue(llvm::Value &llvm_value);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *getLlvmValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *getLlvmValuePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct LocalStaticVariable : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  rq::Gendex<rq::StaticValue> _static_value{};

  explicit RQ_ALWAYS_INLINE LocalStaticVariable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void
  setStaticValue(rq::Gendex<rq::StaticValue> static_value);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue *getStaticValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue *
  getStaticValuePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct SymbolTable : public rq::TableMember {
  using Self = rq::SymbolTable;
};

struct CTable final : public rq::SymbolTable {
  using Self = rq::CTable;
};

struct TopTable final : public rq::SymbolTable {
  using Self = rq::TopTable;
};

struct LocalScope : public rq::SymbolTable {
  using Self = rq::LocalScope;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::Module *_module_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LocalScope(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  RQ_ALWAYS_INLINE void
  setModifierFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getModulePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getModulePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ScopeStatement final : public rq::LocalScope {
  using Self = rq::ScopeStatement;

  explicit RQ_ALWAYS_INLINE ScopeStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct WeaveStatement final : public rq::LocalScope {
  using Self = rq::WeaveStatement;

  explicit RQ_ALWAYS_INLINE WeaveStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct SpinStatement final : public rq::LocalScope {
  using Self = rq::SpinStatement;

  explicit RQ_ALWAYS_INLINE SpinStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct WhileStatement final : public rq::LocalScope {
  using Self = rq::WhileStatement;

  explicit RQ_ALWAYS_INLINE WhileStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ForStatement final : public rq::LocalScope {
  using Self = rq::ForStatement;

  explicit RQ_ALWAYS_INLINE ForStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct DefaultStatement final : public rq::LocalScope {
  using Self = rq::DefaultStatement;

  explicit RQ_ALWAYS_INLINE DefaultStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct CaseStatement final : public rq::LocalScope {
  using Self = rq::CaseStatement;

  explicit RQ_ALWAYS_INLINE CaseStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct SwitchStatement final : public rq::LocalScope {
  using Self = rq::SwitchStatement;

  explicit RQ_ALWAYS_INLINE SwitchStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct IfStatement final : public rq::LocalScope {
  using Self = rq::IfStatement;

  explicit RQ_ALWAYS_INLINE IfStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ElseIfStatement final : public rq::LocalScope {
  using Self = rq::ElseIfStatement;

  explicit RQ_ALWAYS_INLINE ElseIfStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ElseStatement final : public rq::LocalScope {
  using Self = rq::ElseStatement;

  explicit RQ_ALWAYS_INLINE ElseStatement();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct NamedTable : public rq::SymbolTable {
  using Self = rq::NamedTable;

  rq::Name _name;

  explicit RQ_ALWAYS_INLINE NamedTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool classof(rq::Entity *entity_ptr);
};

struct Namespace final : public rq::NamedTable {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool classof(rq::Entity *entity_ptr);
};

struct GlobalDeclaration : public rq::NamedTable {
  using Self = rq::GlobalDeclaration;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::SymbolTable *_host_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};
  llvm::StringRef _mangled_name{};

  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  RQ_ALWAYS_INLINE void
  setModifierFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setHostTable(rq::SymbolTable &host);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getHostTablePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getHostTablePtr() const;
  RQ_ALWAYS_INLINE void setModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getModulePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getModulePtr() const;
  void setMangledName(llvm::StringRef mangled_name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct Implementation : public rq::GlobalDeclaration {
  using Self = rq::Implementation;

  rq::Symbol *_parent_ptr{nullptr};
  rq::Implementation *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Implementation(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *getParentPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *getParentPtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumImplementation : public rq::Implementation {
  using Self = rq::EnumImplementation;

  rq::ConstantSymbol *_underlying_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setUnderlyingType(rq::ConstantSymbol *underlying_type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getUnderlyingTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
  getUnderlyingTypePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumOverload final : public rq::EnumImplementation {
  using Self = rq::EnumOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE EnumOverload();

  RQ_ALWAYS_INLINE void setParentEnumPolymorph(rq::EnumPolymorph &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *getParentEnumPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
  getParentEnumPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumSpecialization final : public rq::EnumImplementation {
  using Self = rq::EnumSpecialization;

  explicit RQ_ALWAYS_INLINE EnumSpecialization();

  RQ_ALWAYS_INLINE void setParentEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *getParentEnumTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
  getParentEnumTemplatePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalVariableImplementation : public rq::Implementation {
  using Self = rq::GlobalVariableImplementation;

  rq::ConstantSymbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalVariableImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *getTypePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalVariableOverload : public rq::GlobalVariableImplementation {
  using Self = rq::GlobalVariableOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE GlobalVariableOverload();

  RQ_ALWAYS_INLINE void
  setParentGlobalVariablePolymorph(rq::GlobalVariablePolymorph &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalVariablePolymorph *getParentPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalVariablePolymorph *
  getParentPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalVariableSpecialization : public rq::GlobalVariableImplementation {
  using Self = rq::GlobalVariableSpecialization;

  explicit RQ_ALWAYS_INLINE GlobalVariableSpecialization();

  RQ_ALWAYS_INLINE void
  setParentGlobalVariableTemplate(rq::GlobalVariableTemplate &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalVariableTemplate *
  getParentGlobalVariableTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalVariableTemplate *
  getParentGlobalVariableTemplatePtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterImplementation : public rq::Implementation {
  using Self = rq::AdapterImplementation;

  rq::Conformity *_conformity_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setConformity(rq::Conformity &conformity);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Conformity *getConformityPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Conformity *getConformityPtr() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterOverload final : public rq::AdapterImplementation {
  using Self = rq::AdapterOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE AdapterOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterSpecialization final : public rq::AdapterImplementation {
  using Self = rq::AdapterSpecialization;

  explicit RQ_ALWAYS_INLINE AdapterSpecialization();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceImplementation : public rq::Implementation {
  using Self = rq::InterfaceImplementation;

  explicit RQ_ALWAYS_INLINE InterfaceImplementation();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceOverload final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE InterfaceOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceSpecialization final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceSpecialization;

  explicit RQ_ALWAYS_INLINE InterfaceSpecialization();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ConstructorOverload : public rq::Implementation {
  using Self = rq::ConstructorOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ConstructorOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct LayoutConstructorOverload final : public rq::ConstructorOverload {
  using Self = rq::LayoutConstructorOverload;

  explicit RQ_ALWAYS_INLINE LayoutConstructorOverload();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassImplementation : public rq::Implementation {
  using Self = rq::ClassImplementation;

  explicit RQ_ALWAYS_INLINE ClassImplementation();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassOverload final : public rq::ClassImplementation {
  using Self = rq::ClassOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ClassOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassSpecialization final : public rq::ClassImplementation {
  using Self = rq::ClassSpecialization;

  explicit RQ_ALWAYS_INLINE ClassSpecialization();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionImplementation : public rq::Implementation {
  using Self = rq::FunctionImplementation;

  explicit RQ_ALWAYS_INLINE FunctionImplementation();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionOverload final : public rq::FunctionImplementation {
  using Self = rq::FunctionOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE FunctionOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionSpecialization final : public rq::FunctionImplementation {
  using Self = rq::FunctionSpecialization;

  explicit RQ_ALWAYS_INLINE FunctionSpecialization();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct Template : public rq::GlobalDeclaration {
  using Self = rq::Template;

  explicit RQ_ALWAYS_INLINE Template();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumTemplate final : public rq::Template {
  using Self = rq::EnumTemplate;

  explicit RQ_ALWAYS_INLINE EnumTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalVariableTemplate final : public rq::Template {
  using Self = rq::GlobalVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalVariableTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterTemplate final : public rq::Template {
  using Self = rq::AdapterTemplate;

  explicit RQ_ALWAYS_INLINE AdapterTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>