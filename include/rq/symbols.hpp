#pragma once

#include <rq/bump_ptr_list.hpp>
#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/iterators.hpp>
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

namespace rq {

struct ConstantSymbol;
struct Instruction;

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind opcode);

enum class SymbolInfoFlags : std::uint64_t {
  NONE = 0,

  // SYMBOL CLASSIFICATION
  SIMPLE_SYMBOL = rq::getBit(0),
  LITERAL = rq::getBit(1),
  CONTEXTUAL_VALUE = rq::getBit(2),
  CONTEXTUAL_TYPE = rq::getBit(3),
  LOW_ATTRIBUTE_TYPE = rq::getBit(4),
  HIGH_ATTRIBUTE_TYPE = rq::getBit(5),
  REFLECTIVE_TYPE = rq::getBit(6),
  PRIMITIVE_TYPE = rq::getBit(7),
  PLATFORM_PRIMITIVE_TYPE = rq::getBit(8),
  FITTING_PRIMITIVE_TYPE = rq::getBit(9),
  STANDARD_PRIMITIVE_TYPE = rq::getBit(10),
  SCALED_PRIMITIVE_TYPE = rq::getBit(11),
  SUBTYPE = rq::getBit(12),
  COUNTED_SUBTYPE = rq::getBit(13),
  ARITHMETIC_SEQUENCE_TYPE = rq::getBit(14),
  LOCAL_DECLARATION = rq::getBit(15),
  LOCAL_VARIABLE = rq::getBit(16),
  ARGUMENT = rq::getBit(17),
  PARAMETER_LIST = rq::getBit(18),
  SYMBOL_TABLE = rq::getBit(19),
  LOCAL_STATEMENT = rq::getBit(20),
  NAMED_TABLE = rq::getBit(21),
  GLOBAL_DECLARATION = rq::getBit(22),
  VARIANT = rq::getBit(23),
  CLASS_VARIANT = rq::getBit(24),
  ENUM_VARIANT = rq::getBit(25),
  INTERFACE_VARIANT = rq::getBit(26),
  ADAPTER_VARIANT = rq::getBit(27),
  CONSTRUCTOR_VARIANT = rq::getBit(28),
  FUNCTION_VARIANT = rq::getBit(29),
  GLOBAL_DYNAMIC_VARIABLE_VARIANT = rq::getBit(30),
  GLOBAL_STATIC_VARIABLE_VARIANT = rq::getBit(31),
  OVERLOAD = rq::getBit(32),
  SPECIALIZATION = rq::getBit(33),
  GLOBAL_VARIABLE_OVERLOAD = rq::getBit(34),
  TEMPLATE = rq::getBit(35),
  POLYMORPH = rq::getBit(36),
  WEIGHT_LEVEL = rq::getBit(37),
  OVERRIDE = rq::getBit(38),
  OVERLOAD_OVERRIDE = rq::getBit(39),
  TEMPLATE_OVERRIDE = rq::getBit(40),

  // SYMBOL DETAILS
  IS_TYPE = rq::getBit(41),
  IS_SIGNED_TYPE = rq::getBit(42),
  IS_UNSIGNED_TYPE = rq::getBit(43),
  IS_INTEGER_TYPE = rq::getBit(44),
  IS_FLOAT_TYPE = rq::getBit(45),
  IS_BINARY_TYPE = rq::getBit(46),
  IS_BFLOAT_TYPE = rq::getBit(47),
  IS_CODEUNIT_TYPE = rq::getBit(48),
  HAS_LOW_ATTRIBUTES = rq::getBit(49),
  IS_FRAME_SCOPE = rq::getBit(50),
  IS_OBJECT_SCOPE = rq::getBit(51)
};

RQ_DEFINE_FLAGS(rq::SymbolInfoFlags);

[[nodiscard]] inline rq::SymbolInfoFlags getInfoFlags(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLowAttributeType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsHighAttributeType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFittingPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArgument(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclaration(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInterfaceVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsAdapterVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsConstructorVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunctionVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalDynamicVariableVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalStaticVariableVariant(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverload(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSpecialization(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsGlobalVariableOverload(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverride(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOverloadOverride(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateOverride(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLowAttributes(rq::SymbolKind kind);
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
  return rq::getUNDERLYING_VALUE(rhs) <=> rq::getUNDERLYING_VALUE(lhs);
}

// clang-format off
struct Symbol;
  struct SimpleSymbol;
    template<rq::SymbolKind KIND_PARAM> struct DerivedSimpleSymbol;
  struct ScaledPrimitiveType;
    template<rq::SymbolKind KIND_PARAM> struct DerivedScaledPrimitiveType;
  struct Subtype;
    struct ArraySubtype;
    struct UncountedSubtype;
      template<rq::SymbolKind KIND_PARAM> struct DerivedUncountedSubtype;
  struct Module;
  struct Import;
  struct Conformity;
  struct Block;
  struct WeightLevel;
    template<rq::SymbolKind KIND_PARAM> struct DerivedUncountedWeightLevel;
  struct JuxtapositionalListItem;
  struct JuxtapositionalList;
  struct ArithmeticSequenceType;
    template<rq::SymbolKind KIND_PARAM> struct DerivedArithmeticSequenceType;
  struct LocalDeclaration;
    struct Anchor;
    struct Enumerator;
    struct LocalVariable;
      struct LocalDynamicVariable;
      struct LocalStaticVariable;
      struct Capture;
      struct Argument;
        struct TemplateArgument;
        struct FunctionArgument;
  struct Parameter;
  struct ParameterList;
    struct SignatureType;
    struct LayoutType;
  struct PlacementType;
  struct CompositionComponent;
  struct CompositionType;
  struct SynonymType;
  struct SymbolTable;
    struct Top;
    struct C;
    struct LocalScope;
      template<rq::SymbolKind KIND_PARAM> struct DerivedLocalScope;
    struct NamedTable;
      struct Namespace;
      struct GlobalDeclaration;
        struct Variant;
          struct ClassVariant;
            struct ClassOverload;
            struct ClassSpecialization;
          struct EnumVariant;
            struct EnumOverload;
            struct EnumSpecialization;
          struct InterfaceVariant;
            struct InterfaceOverload;
            struct InterfaceSpecialization;
          struct AdapterVariant;
            struct AdapterOverload;
            struct AdapterSpecialization;
          struct ConstructorVariant;
            struct ConstructorOverload;
            struct LayoutConstructorOverload;
          struct FunctionVariant;
            struct FunctionOverload;
            struct FunctionSpecialization;
          struct GlobalVariableVariant;
            struct GlobalDynamicVariableVariant;
              struct GlobalDynamicVariableOverload;
              struct GlobalDynnamicVariableSpecialization;
            struct GlobalStaticVariableVariant;
              struct GlobalStaticVariableOverload;
              struct GlobalStaticVariableSpecialization;
      struct Template;
        struct ClassTemplate;
        struct EnumTemplate;
        struct InterfaceTemplate;
        struct AdapterTemplate;
        struct FunctionTemplate;
        struct GlobalDynamicVariableTemplate;
        struct GlobalStaticVariableTemplate;
  struct Polymorph;
    struct ClassPolymorph;
    struct EnumPolymorph;
    struct InterfacePolymorph;
    struct AdapterPolymorph;
    struct FunctionPolymorph;
    struct GlobalDynamicVariablePolymorph;
    struct GlobalStaticVariablePolymorph;
  struct WeightLevel;
    struct ClassWeightLevel;
    struct EnumWeightLevel;
    struct InterfaceWeightLevel;
    struct AdapterWeightLevel;
    struct FunctionWeightLevel;
    struct GlobalDynamicVariableWeightLevel;
    struct GlobalStaticVariableWeightLevel;
  struct Override;
    struct OverloadOverride;
      struct AdapterOverloadOverride;
      struct FunctionOverloadOverride;
    struct TemplateOverride;
      struct ClassTemplateOverride;
      struct EnumTemplateOverride;
      struct InterfaceTemplateOverride;
      struct AdapterTemplateOverride;
      struct FunctionTemplateOverride;
      struct GlobalDynamicVariableTemplateOverride;
      struct GlobalStaticVariableTemplateOverride;
// clang-format on

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::SymbolKind kind);

  Symbol(const Self &) = delete;
  Symbol(Self &&) = delete;
  ~Symbol() = default;
  Self &operator=(Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] bool operator==(const Self &) const;
  [[nodiscard]] bool operator!=(const Self &) const;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind getKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolInfoFlags getInfoFlags() const;
  [[nodiscard]] inline rq::LowFuseFlags getDerivedLowFuseFlags() const;
  [[nodiscard]] inline const rq::Expression *getDerivedExpressionPtr() const;
  [[nodiscard]] inline rq::Expression *getDerivedExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsObliqueLiteralType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLowAttributeType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHighAttributeType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformPrimitiveType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStandardPrimitiveType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLowAttributes() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope() const;

  [[nodiscard]] inline bool getIsCompleteType() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SimpleSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SimpleSymbol;

  explicit RQ_ALWAYS_INLINE SimpleSymbol(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

template <rq::SymbolKind KIND_PARAM>
struct DerivedSimpleSymbol final : public rq::SimpleSymbol {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedSimpleSymbol<KIND>;

  explicit RQ_ALWAYS_INLINE DerivedSimpleSymbol();

  [[nodiscard]] static inline bool classof(const Entity *entity_ptr);
};

using IntegerLiteral =
    rq::DerivedSimpleSymbol<rq::SymbolKind::INTEGER_LITERAL_TYPE>;
using FloatLiteral =
    rq::DerivedSimpleSymbol<rq::SymbolKind::FLOAT_LITERAL_TYPE>;
using StringLiteral =
    rq::DerivedSimpleSymbol<rq::SymbolKind::STRING_LITERAL_TYPE>;
using CodeunitLiteral =
    rq::DerivedSimpleSymbol<rq::SymbolKind::CODEUNIT_LITERAL_TYPE>;
using ValueValue = rq::DerivedSimpleSymbol<rq::SymbolKind::VALUE_VALUE>;
using IndexValue = rq::DerivedSimpleSymbol<rq::SymbolKind::INDEX_VALUE>;
using InferenceType = rq::DerivedSimpleSymbol<rq::SymbolKind::INFERENCE_TYPE>;
using VoidType = rq::DerivedSimpleSymbol<rq::SymbolKind::VOID_TYPE>;
using NoReturnType = rq::DerivedSimpleSymbol<rq::SymbolKind::NO_RETURN_TYPE>;
using AnchorAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::ANCHOR_ATTRIBUTE_TYPE>;
using OpaqueAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::OPAQUE_ATTRIBUTE_TYPE>;
using GlobalAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::GLOBAL_ATTRIBUTE_TYPE>;
using PublicAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::PUBLIC_ATTRIBUTE_TYPE>;
using PartialMutateAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::PARTIAL_MUTATE_ATTRIBUTE_TYPE>;
using StaticAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::STATIC_ATTRIBUTE_TYPE>;
using DelayAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::DELAY_ATTRIBUTE_TYPE>;
using CaptureAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::CAPTURE_ATTRIBUTE_TYPE>;
using InlineAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::INLINE_ATTRIBUTE_TYPE>;
using MangleAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::MANGLE_ATTRIBUTE_TYPE>;
using PackAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::PACK_ATTRIBUTE_TYPE>;
using BranchTrendAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::BRANCH_TREND_ATTRIBUTE_TYPE>;
using SupportStatusAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::SUPPORT_STATUS_ATTRIBUTE_TYPE>;
using StableAddressAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::STABLE_ADDRESS_ATTRIBUTE_TYPE>;
using VariadicAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::VARIADIC_ATTRIBUTE_TYPE>;
using LocationAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::LOCATION_ATTRIBUTE_TYPE>;
using TemplateAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::TEMPLATE_ATTRIBUTE_TYPE>;
using ConstraintAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::CONSTRAINT_ATTRIBUTE_TYPE>;
using WeightAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::WEIGHT_ATTRIBUTE_TYPE>;
using AutoAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::AUTO_ATTRIBUTE_TYPE>;
using RequireAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::REQUIRE_ATTRIBUTE_TYPE>;
using EnsureAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::ENSURE_ATTRIBUTE_TYPE>;
using VarAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::VAR_ATTRIBUTE_TYPE>;
using VolatileAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::VOLATILE_ATTRIBUTE_TYPE>;
using AtomicAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::ATOMIC_ATTRIBUTE_TYPE>;
using NullTerminateAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::NULL_TERMINATE_ATTRIBUTE_TYPE>;
using SymbolType = rq::DerivedSimpleSymbol<rq::SymbolKind::SYMBOL_TYPE>;
using SymbolRangeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::SYMBOL_RANGE_TYPE>;
using ExpressionType = rq::DerivedSimpleSymbol<rq::SymbolKind::EXPRESSION_TYPE>;
using BooleanType = rq::DerivedSimpleSymbol<rq::SymbolKind::BOOLEAN_TYPE>;
using HalfType = rq::DerivedSimpleSymbol<rq::SymbolKind::HALF_TYPE>;
using SingleType = rq::DerivedSimpleSymbol<rq::SymbolKind::SINGLE_TYPE>;
using DoubleType = rq::DerivedSimpleSymbol<rq::SymbolKind::DOUBLE_TYPE>;
using QuadrupleType = rq::DerivedSimpleSymbol<rq::SymbolKind::QUADRUPLE_TYPE>;
using SignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::SIGNED_INTEGER_TYPE>;
using UnsignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::UNSIGNED_INTEGER_TYPE>;
using SignedIndexType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::SIGNED_INDEX_TYPE>;
using UnsignedIndexType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::UNSIGNED_INDEX_TYPE>;
using SignedAddressType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::SIGNED_ADDRESS_TYPE>;
using UnsignedAddressType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::UNSIGNED_ADDRESS_TYPE>;
using CharType = rq::DerivedSimpleSymbol<rq::SymbolKind::CHAR_TYPE>;
using FastSignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::FAST_SIGNED_INTEGER_TYPE>;
using FastUnsignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::FAST_UNSIGNED_INTEGER_TYPE>;
using LeastSignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::LEAST_SIGNED_INTEGER_TYPE>;
using LeastUnsignedIntegerType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::LEAST_UNSIGNED_INTEGER_TYPE>;
using Binary16Type = rq::DerivedSimpleSymbol<rq::SymbolKind::BINARY16_TYPE>;
using Binary32Type = rq::DerivedSimpleSymbol<rq::SymbolKind::BINARY32_TYPE>;
using Binary64Type = rq::DerivedSimpleSymbol<rq::SymbolKind::BINARY64_TYPE>;
using Binary128Type = rq::DerivedSimpleSymbol<rq::SymbolKind::BINARY128_TYPE>;
using Bfloat16Type = rq::DerivedSimpleSymbol<rq::SymbolKind::BFLOAT16_TYPE>;
using AsciiType = rq::DerivedSimpleSymbol<rq::SymbolKind::ASCII_TYPE>;
using Utf8Type = rq::DerivedSimpleSymbol<rq::SymbolKind::UTF8_TYPE>;
using VariadicArgumentsType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::VARIADIC_ARGUMENTS_TYPE>;

enum class ScaleKind { EXACT, FAST, LEAST };

struct ScaledPrimitiveType : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledPrimitiveType;

  rq::ScaleKind _scale_kind;
  unsigned _scale;
  std::uint64_t _synonym_id;

  explicit RQ_ALWAYS_INLINE ScaledPrimitiveType(rq::SymbolKind kind,
                                                rq::ScaleKind scale_kind,
                                                unsigned scale,
                                                std::uint64_t synonym_id);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaleKind getScaleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getScale() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getSynonymTypeId() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileScaledPrimitiveType(llvm::FoldingSetNodeID &inout_id,
                           rq::SymbolKind kind, rq::ScaleKind scale_kind,
                           unsigned scale, std::uint64_t synonum_id);

template <rq::SymbolKind KIND_PARAM>
struct DerivedScaledPrimitiveType final : public rq::ScaledPrimitiveType {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedScaledPrimitiveType<KIND>;

  explicit RQ_ALWAYS_INLINE
  DerivedScaledPrimitiveType(rq::ScaleKind scale_kind, unsigned scale,
                             std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

using ScaledSignedIntegerType =
    rq::DerivedScaledPrimitiveType<rq::SymbolKind::SCALED_SIGNED_INTEGER_TYPE>;
using ScaledUnsignedIntegerType = rq::DerivedScaledPrimitiveType<
    rq::SymbolKind::SCALED_UNSIGNED_INTEGER_TYPE>;

struct Subtype : public rq::Symbol {
  using Self = rq::Subtype;

  rq::ConstantSymbol *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::SymbolKind kind,
                                    rq::ConstantSymbol &child);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArraySubtype final : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::ArraySubtype;

  std::uint64_t _count;

  explicit RQ_ALWAYS_INLINE ArraySubtype(rq::ConstantSymbol &child,
                                         std::uint64_t count);

  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &inout_id,
                                          const rq::ConstantSymbol &child,
                                          std::uint64_t count);

struct UncountedSubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::SymbolKind kind,
                                             rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &inout_id,
                                              rq::SymbolKind kind,
                                              const rq::ConstantSymbol &child);

template <rq::SymbolKind KIND_PARAM>
struct DerivedUncountableSubtype final : public rq::UncountedSubtype {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedUncountableSubtype<KIND>;

  explicit RQ_ALWAYS_INLINE
  DerivedUncountableSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

using ReferenceSubtype =
    rq::DerivedUncountableSubtype<rq::SymbolKind::REFERENCE_SUBTYPE>;
using PointerSubtype =
    rq::DerivedUncountableSubtype<rq::SymbolKind::POINTER_SUBTYPE>;
using SliceSubtype =
    rq::DerivedUncountableSubtype<rq::SymbolKind::SLICE_SUBTYPE>;
using InferenceCountArraySubtype = rq::DerivedUncountableSubtype<
    rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE>;

enum class ModuleKind : std::uint8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind);

constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

struct ModuleFactory final {
  using Self = rq::ModuleFactory;

  rq::ModuleKind _module_kind{rq::ModuleKind::NONE};
  rq::Expression *_expression_ptr{nullptr};
  llvm::StringRef _path{};
  llvm::StringRef _buffer{};
  std::vector<rq::Token> _tokens{};

  explicit RQ_ALWAYS_INLINE ModuleFactory() = default;
  explicit RQ_ALWAYS_INLINE ModuleFactory(rq::ModuleKind kind,
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

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr;
  llvm::StringRef _path;
  llvm::StringRef _buffer;

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleFactory &&factory);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Import final : public rq::Symbol {
  using Self = rq::Import;

  rq::LowFuseFlags _low_flags;
  rq::Expression *_expression_ptr;
  rq::Module *_imported_ptr;
  rq::Module *_module_ptr;

  explicit RQ_ALWAYS_INLINE Import(rq::LowFuseFlags flags,
                                   rq::Expression &expression,
                                   rq::Module &module, rq::Module &imported);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getLowFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getImported() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getImported();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Conformity final : public rq::Symbol {
  using Self = rq::Conformity;

  rq::InterfaceOverload *_interface_ptr;
  rq::Symbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE Conformity(rq::InterfaceOverload &interface,
                                       rq::Symbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceOverload &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceOverload &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

namespace llvm {
class BasicBlock;
}

namespace rq {

struct Block final : public rq::Symbol {
  using Self = rq::Block;

  rq::Instruction *_outer_instruction_ptr;
  llvm::BasicBlock *_llvm_block_ptr{nullptr};
  rq::Block *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Block(rq::Instruction *outer_instruction_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction *
  getOuterInstructionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction *getOuterInstructionPtr();
  RQ_ALWAYS_INLINE void setLlvmBlock(llvm::BasicBlock &llvm_block);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::BasicBlock &getLlvmBlock() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::BasicBlock &getLlvmBlock();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct JuxtapositionalListItem final : public rq::Symbol,
                                       public llvm::FoldingSetNode {
  using Self = rq::JuxtapositionalListItem;

  rq::JuxtapositionalListItem *_next_ptr;
  rq::ConstantSymbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE
  JuxtapositionalListItem(rq::ConstantSymbol &type,
                          rq::JuxtapositionalListItem *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListItem(llvm::FoldingSetNodeID &inout_id,
                               const rq::ConstantSymbol &type,
                               const rq::JuxtapositionalListItem *next_ptr);

struct JuxtapositionalListType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::JuxtapositionalList;

  rq::JuxtapositionalListItem *_first_item_ptr;

  explicit RQ_ALWAYS_INLINE
  JuxtapositionalListType(rq::JuxtapositionalListItem &first_item);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::JuxtapositionalListItem>
  getJuxtapositionalListItemSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::JuxtapositionalListItem>
      getJuxtapositionalListItemSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::JuxtapositionalListItem>
      getConstJuxtapositionalListItemSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListType(llvm::FoldingSetNodeID &inout_id,
                               const rq::JuxtapositionalListItem &first_item);

struct ArithmeticSequenceType : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceType;

  rq::ConstantSymbol *_child_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  explicit RQ_ALWAYS_INLINE
  ArithmeticSequenceType(rq::SymbolKind kind, rq::ConstantSymbol &child,
                         rq::ArithmeticSequenceCondition condition,
                         rq::ArithmeticSequenceStep step);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
    const rq::ConstantSymbol &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step);

template <rq::SymbolKind KIND_PARAM>
struct DerivedArithmeticSequenceType final : public rq::ArithmeticSequenceType {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedArithmeticSequenceType<KIND>;

  explicit RQ_ALWAYS_INLINE
  DerivedArithmeticSequenceType(rq::ConstantSymbol &child,
                                rq::ArithmeticSequenceCondition condition,
                                rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

using ArithmeticIntervalType =
    rq::DerivedArithmeticSequenceType<rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE>;
using InfiniteArithmeticSequenceType = rq::DerivedArithmeticSequenceType<
    rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE>;
using FiniteArithmeticSequenceType = rq::DerivedArithmeticSequenceType<
    rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE>;

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
    inout_id.AddInteger(rq::getUNDERLYING_VALUE(this->getKeyword()));
  }
};

} // namespace rq

namespace llvm {
[[nodiscard]] RQ_ALWAYS_INLINE llvm::hash_code
hash_value(const rq::Name &value) {
  return llvm::hash_combine(value.getText(), value.getKeyword());
};

template <> struct DenseMapInfo<rq::Name> {
  static rq::Name getEmptyKey() {
    return rq::Name(rq::Name::DENSE_MAP_KEY_EMPTY);
  }

  static rq::Name getTombstoneKey() {
    return rq::Name(rq::Name::DENSE_MAP_KEY_TOMBSTONE);
  }

  static unsigned getHashValue(const rq::Name &value) {
    return static_cast<unsigned>(hash_value(value));
  }

  static bool isEqual(const rq::Name &lhs, const rq::Name &rhs) {
    return lhs.getText() == rhs.getText() &&
           lhs.getKeyword() == rhs.getKeyword();
  }
};
} // namespace llvm

namespace rq {

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;

  rq::Name _name;
  rq::SymbolTable *_container_ptr;
  rq::SymbolTable *_host_ptr;
  rq::Module *_module_ptr;

  explicit RQ_ALWAYS_INLINE LocalDeclaration(rq::SymbolKind kind, rq::Name name,
                                             rq::SymbolTable &container,
                                             rq::SymbolTable &host,
                                             rq::Module &module);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getContainer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainer();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getHost() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHost();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Anchor final : public rq::LocalDeclaration {
  using Self = rq::Anchor;

  rq::SymbolTable *_vessel_ptr;

  explicit RQ_ALWAYS_INLINE Anchor(rq::Name name, rq::SymbolTable &container,
                                   rq::Module &module, rq::SymbolTable &vessel);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getVessel() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getVessel();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalVariable : public rq::LocalDeclaration {
  using Self = rq::LocalVariable;

  rq::LowFuseFlags _low_flags;
  rq::ConstantSymbol *_type_ptr;

  explicit RQ_ALWAYS_INLINE
  LocalVariable(rq::SymbolKind kind, rq::Name name, rq::SymbolTable &container,
                rq::SymbolTable &host, rq::Module &module,
                rq::LowFuseFlags flags, rq::ConstantSymbol &type);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getLowFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();
  RQ_ALWAYS_INLINE void completeType(rq::ConstantSymbol &type);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

namespace llvm {
struct Value;
}

namespace rq {

struct LocalDynamicVariable final : public rq::LocalVariable {
  using Self = rq::LocalDynamicVariable;

  llvm::Value *_llvm_location_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  LocalDynamicVariable(rq::Name name, rq::SymbolTable &container,
                       rq::SymbolTable &host, rq::Module &module,
                       rq::LowFuseFlags flags, rq::ConstantSymbol &type);

  RQ_ALWAYS_INLINE void setLlvmLocation(llvm::Value &llvm_location);
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *getLlvmLocationPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *getLlvmLocationPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalStaticVariable final : public rq::LocalVariable {
  using Self = rq::LocalStaticVariable;

  rq::Gendex<rq::StaticValue> _value;

  explicit RQ_ALWAYS_INLINE
  LocalStaticVariable(rq::Name name, rq::SymbolTable &container,
                      rq::SymbolTable &host, rq::Module &module,
                      rq::LowFuseFlags flags, rq::ConstantSymbol &type,
                      rq::Gendex<rq::StaticValue> gendex);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Gendex<rq::StaticValue> &
  getValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Gendex<rq::StaticValue> &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Capture final : public rq::LocalVariable {
  using Self = rq::Capture;

  rq::Constant *_value_ptr;

  explicit RQ_ALWAYS_INLINE Capture(rq::SymbolTable &container,
                                    rq::SymbolTable &host, rq::Module &module,
                                    rq::ConstantSymbol &type,
                                    rq::Constant &value);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Constant &getValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Constant &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Argument : public rq::LocalVariable {
  using Self = rq::Argument;

  rq::Parameter *_parameter_ptr;

  explicit RQ_ALWAYS_INLINE Argument(rq::SymbolKind kind, rq::Name name,
                                     rq::SymbolTable &host, rq::Module &module,
                                     rq::Parameter &parameter);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter &getParameter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter &getParameter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TemplateArgument final : public rq::Argument {
  using Self = rq::TemplateArgument;

  rq::Entity *_value_ptr;
  rq::Parameter *_parameter_ptr;

  explicit RQ_ALWAYS_INLINE
  TemplateArgument(rq::Name name, rq::SymbolTable &container,
                   rq::SymbolTable &host, rq::Module &module,
                   rq::LowFuseFlags flags, rq::ConstantSymbol &type,
                   rq::Entity &value, rq::Parameter &parameter);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionArgument final : public rq::Argument {
  using Self = rq::FunctionArgument;

  explicit RQ_ALWAYS_INLINE FunctionArgument(rq::Name name,
                                             rq::SymbolTable &host,
                                             rq::Module &module,
                                             rq::Parameter &parameter);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

enum class ParameterInfoFlags : std::uint8_t {
  NONE = 0,

  POSITIONAL = rq::getBit(0),
  NONPOSITIONAL = rq::getBit(1),
  LOCKED = rq::getBit(2)
};

struct Parameter : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::Parameter;

  rq::Parameter *_next_ptr;
  rq::Name _name;
  rq::ConstantSymbol *_type_ptr;
  rq::LowFuseFlags _low_flags;
  rq::ParameterInfoFlags _param_flags;
  rq::Expression *_default_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  Parameter(rq::Parameter *next_ptr, rq::Name name, rq::ConstantSymbol &type,
            rq::LowFuseFlags low_flags, rq::ParameterInfoFlags param_flags,
            rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getLowFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
  getParameterFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDefaultValueExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getDefaultValueExpressionPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileSignatureType(llvm::FoldingSetNodeID &inout_id,
                     const rq::Parameter *next_ptr, rq::Name name,
                     const rq::ConstantSymbol &type, rq::LowFuseFlags low_flags,
                     rq::ParameterInfoFlags param_flags,
                     const rq::Expression *default_value_expression_ptr);

struct ParameterList : public rq::Symbol {
  using Self = rq::ParameterList;

  rq::Parameter *_first_parameter_ptr;
  unsigned _parameter_count;
  unsigned _positional_parameter_count;
  unsigned _nonpositional_parameter_count;
  unsigned _locked_parameter_count;

  explicit RQ_ALWAYS_INLINE ParameterList(rq::SymbolKind kind,
                                          rq::Parameter *first_parameter_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *
  getFirstParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *getFirstParameterPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getPositionalParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned
  getNonpositionalParameterCount() const;
  [[nodiscard]] inline const rq::Parameter *
  getParameterPtrOfName(rq::Name name) const;
  [[nodiscard]] inline rq::Parameter *getParameterPtrOfName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Parameter>
  getParameterSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
  getParameterSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
  getConstParameterSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SignatureType final : public rq::ParameterList, llvm::FoldingSetNode {
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

RQ_ALWAYS_INLINE void
profileSignatureType(llvm::FoldingSetNodeID &inout_id,
                     const rq::Parameter *first_parameter_ptr,
                     const rq::ConstantSymbol &return_type);

struct LayoutType final : public rq::ParameterList, llvm::FoldingSetNode {
  using Self = rq::LayoutType;

  explicit RQ_ALWAYS_INLINE LayoutType(rq::Parameter *first_parameter_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNode &inout_id);
};

RQ_ALWAYS_INLINE void
profileLayoutType(llvm::FoldingSetNodeID &inout_id,
                  const rq::Parameter *first_parameter_ptr);

struct PlacementType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::FunctionOverload *_function_ptr;

  explicit RQ_ALWAYS_INLINE PlacementType(rq::FunctionOverload &function);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionOverload &
  getFunction() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionOverload &getFunction();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &inout_id,
                                       const rq::FunctionOverload &function);

// TODO composition factory

struct CompositionComponent final : public rq::Symbol,
                                    public llvm::FoldingSetNode {
  using Self = rq::CompositionComponent;

  rq::CompositionComponent *_next_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionComponent(rq::CompositionComponent *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent *
  getNextComponentPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent *
  getNextComponentPtr();

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

inline void
profileCompositionComponent(llvm::FoldingSetNodeID &inout_id,
                            const rq::CompositionComponent *next_component_ptr);

struct CompositionType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::CompositionType;

  rq::CompositionComponent *_first_component_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionType(rq::CompositionComponent &first_component);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent &
  getFirstComponent() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent &getFirstComponent();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::CompositionComponent>
  getComponentSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::CompositionComponent>
  getComponentSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::CompositionComponent>
  getConstComponentSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionType(llvm::FoldingSetNodeID &inout_id,
                       const rq::CompositionComponent &first_component);

struct SynonymType final : public rq::Symbol {
  using Self = rq::SynonymType;

  rq::Symbol *_original_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE SynonymType(rq::Symbol &original);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getOriginal() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getOriginal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = rq::SymbolTable &;
  using pointer = rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTable *_symbol_table_ptr = nullptr;

  SymbolTableIterator() = default;
  explicit SymbolTableIterator(rq::SymbolTable *symbol_table_ptr)
      : _symbol_table_ptr(symbol_table_ptr) {}
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_symbol_table_ptr == it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_symbol_table_ptr != it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &operator*() {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *operator->() {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_symbol_table_ptr == nullptr;
  }
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = const rq::SymbolTable;
  using reference = const rq::SymbolTable &;
  using pointer = rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTable *_symbol_table_ptr = nullptr;

  ConstSymbolTableIterator() = default;
  explicit ConstSymbolTableIterator(const rq::SymbolTable *symbol_table_ptr)
      : _symbol_table_ptr(symbol_table_ptr) {}
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_symbol_table_ptr == it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_symbol_table_ptr != it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_symbol_table_ptr == nullptr;
  }
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;

  llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>> _member_map{};
  rq::SymbolTable *_container_ptr{nullptr};
  rq::FunctionVariant *_function_container_ptr{nullptr};
  rq::Variant *_object_container_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setContainer(rq::SymbolTable &container);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getContainerPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionVariant *
  getFunctionContainerPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionVariant *getFunctionContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Variant *
  getObjectContainerPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Variant *getObjectContainerPtr();
  inline void addMember(rq::BumpPtrAllocator &allocator, rq::Name name,
                        rq::Symbol &symbol);
  [[nodiscard]] RQ_ALWAYS_INLINE const
      llvm::DenseMap<rq::Name, rq::BumpPtrList<rq::Symbol>> &
      getMap() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstBumpPtrListRef<rq::Symbol>
  lookupList(rq::Name name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
  lookupList(rq::Name name);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::SymbolTableIterator>
  getInclusiveAscendingSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
  getInclusiveAscendingSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
  getConstInclusiveAscendingSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE auto getSymbolListSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE auto getSymbolListSubrange();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct C final : rq::SymbolTable {
  using Self = rq::C;

  explicit RQ_ALWAYS_INLINE C();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  rq::SymbolTable &table = rq::dereferencePtr(this->_symbol_table_ptr);
  this->_symbol_table_ptr = table._container_ptr;
  return *this;
}

rq::SymbolTableIterator SymbolTableIterator::operator++(int) {
  rq::SymbolTableIterator temp = *this;
  rq::SymbolTable &table = rq::dereferencePtr(this->_symbol_table_ptr);
  this->_symbol_table_ptr = table._container_ptr;
  return temp;
}

rq::ConstSymbolTableIterator &ConstSymbolTableIterator::operator++() {
  const rq::SymbolTable &table = rq::dereferencePtr(this->_symbol_table_ptr);
  this->_symbol_table_ptr = table._container_ptr;
  return *this;
}

rq::ConstSymbolTableIterator ConstSymbolTableIterator::operator++(int) {
  rq::ConstSymbolTableIterator temp = *this;
  const rq::SymbolTable &table = rq::dereferencePtr(this->_symbol_table_ptr);
  this->_symbol_table_ptr = table._container_ptr;
  return temp;
}

struct Top final : public rq::SymbolTable {
  using Self = rq::Top;

  explicit RQ_ALWAYS_INLINE Top();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LocalScope : public rq::SymbolTable {
  using Self = rq::LocalScope;

  rq::Expression *_expression_ptr;
  rq::LowFuseFlags _low_flags;
  rq::Module *_module_ptr;

  explicit RQ_ALWAYS_INLINE LocalScope(rq::SymbolKind kind,
                                       rq::SymbolTable &container,
                                       rq::Expression &expression,
                                       rq::LowFuseFlags flags,
                                       rq::Module &module);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getLowFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

template <rq::SymbolKind KIND_PARAM>
struct DerivedLocalScope final : public rq::LocalScope {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedLocalScope<KIND>;

  explicit RQ_ALWAYS_INLINE DerivedLocalScope(rq::SymbolTable &container,
                                              rq::Expression &expression,
                                              rq::LowFuseFlags flags,
                                              rq::Module &module);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

using IfStatement = rq::DerivedLocalScope<rq::SymbolKind::IF_STATEMENT>;
using ElseIfStatement =
    rq::DerivedLocalScope<rq::SymbolKind::ELSE_IF_STATEMENT>;
using ElseStatement = rq::DerivedLocalScope<rq::SymbolKind::ELSE_STATEMENT>;
using SwitchStatement = rq::DerivedLocalScope<rq::SymbolKind::SWITCH_STATEMENT>;
using CaseStatement = rq::DerivedLocalScope<rq::SymbolKind::CASE_STATEMENT>;
using DefaultStatement =
    rq::DerivedLocalScope<rq::SymbolKind::DEFAULT_STATEMENT>;
using ForStatement = rq::DerivedLocalScope<rq::SymbolKind::FOR_STATEMENT>;
using WhileStatement = rq::DerivedLocalScope<rq::SymbolKind::WHILE_STATEMENT>;
using SpinStatement = rq::DerivedLocalScope<rq::SymbolKind::SPIN_STATEMENT>;
using WeaveStatement = rq::DerivedLocalScope<rq::SymbolKind::WEAVE_STATEMENT>;
using ScopeStatement = rq::DerivedLocalScope<rq::SymbolKind::SCOPE_STATEMENT>;

struct NamedTable : public rq::SymbolTable {
  using Self = rq::NamedTable;

  rq::Name _name{};

  explicit RQ_ALWAYS_INLINE NamedTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDeclaration : public rq::NamedTable {
  using Self = rq::GlobalDeclaration;

  rq::Expression *_expression_ptr{nullptr};
  rq::Expression *_name_expression_ptr{nullptr};
  rq::LowFuseFlags _low_flags{rq::LowFuseFlags::NONE};
  rq::SymbolTable *_host_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};
  llvm::StringRef _mangled_name{};
  rq::Expression *_mangled_name_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalDeclaration(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  RQ_ALWAYS_INLINE void setNameExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getNameExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getNameExpressionPtr();
  RQ_ALWAYS_INLINE void setLowFuseFlags(rq::LowFuseFlags low_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LowFuseFlags getLwFusesFlags() const;
  RQ_ALWAYS_INLINE void setHost(rq::SymbolTable &host);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getHostPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getHostPtr();
  RQ_ALWAYS_INLINE void setModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getModulePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getModulePtr();
  RQ_ALWAYS_INLINE void setMangledName(llvm::StringRef name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;
  RQ_ALWAYS_INLINE void setMangledNameExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getMangledNameExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getMangledNameExpressionPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Variant : public rq::NamedTable {
  using Self = rq::Variant;

  rq::Variant *_next_ptr{nullptr};
  rq::Polymorph *_polymorph_ptr{nullptr};
  rq::Template *_template_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Variant(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph *getPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph *getPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *getTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template *getTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassVariant : public rq::Variant {
  using Self = rq::ClassVariant;

  rq::LayoutType *_layout_ptr{nullptr};
  rq::ConstructorVariant *_first_constructor_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLayoutType(rq::LayoutType &layout);
  [[nodiscard]] const rq::LayoutType *getLayoutTypePtr() const;
  [[nodiscard]] rq::LayoutType *getLayoutTypePtr();
  RQ_ALWAYS_INLINE void addConstructorVariant(rq::ConstructorVariant &variant);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::ConstructorVariant>
  getConstructorVariantSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::ConstructorVariant>
  getConstructorVariantSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::ConstructorVariant>
  getConstConstructorVariantSubrange() const;
  RQ_ALWAYS_INLINE void setClassPolymorph(rq::ClassPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassPolymorph *
  getClassPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassPolymorph *getClassPolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassOverload final : public rq::ClassVariant {
  using Self = rq::ClassOverload;

  explicit RQ_ALWAYS_INLINE ClassOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassSpecialization final : public rq::ClassVariant {
  using Self = rq::ClassSpecialization;

  explicit RQ_ALWAYS_INLINE ClassSpecialization();

  RQ_ALWAYS_INLINE void setClassTemplate(rq::ClassTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassTemplate *
  getClassTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassTemplate *getClassTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumVariant : public rq::Variant {
  using Self = rq::EnumVariant;

  rq::ConstantSymbol *_underlying_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setUNDERLYING_VALUEType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
  getUNDERLYING_VALUETypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getUNDERLYING_VALUETypePtr();
  RQ_ALWAYS_INLINE void setEnumPolymorph(rq::EnumPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
  getEnumPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *getEnumPolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumOverload final : public rq::EnumVariant {
  using Self = rq::EnumOverload;

  explicit RQ_ALWAYS_INLINE EnumOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumSpecialization final : public rq::EnumVariant {
  using Self = rq::EnumSpecialization;

  explicit RQ_ALWAYS_INLINE EnumSpecialization();

  RQ_ALWAYS_INLINE void setEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
  getEnumTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *getEnumTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceVariant : public rq::Variant {
  using Self = rq::InterfaceVariant;

  explicit RQ_ALWAYS_INLINE InterfaceVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void
  setInterfacePolymorph(rq::InterfacePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfacePolymorph *
  getInterfacePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfacePolymorph *
  getInterfacePolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceOverload final : public rq::InterfaceVariant {
  using Self = rq::InterfaceOverload;

  explicit RQ_ALWAYS_INLINE InterfaceOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceSpecialization final : public rq::InterfaceVariant {
  using Self = rq::InterfaceSpecialization;

  explicit RQ_ALWAYS_INLINE InterfaceSpecialization();

  RQ_ALWAYS_INLINE void setInterfaceTemplate(rq::InterfaceTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceTemplate *
  getInterfaceTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceTemplate *getInterfaceTemplate();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterVariant : public rq::Variant {
  using Self = rq::AdapterVariant;

  rq::Conformity *_conformity_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setConformity(rq::Conformity &conformity);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Conformity *getConformityPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Conformity *getConformityPtr();
  RQ_ALWAYS_INLINE void setAdapterPolymorph(rq::AdapterPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterPolymorph *
  getAdapterPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterPolymorph *getAdapterPolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterOverload final : public rq::AdapterVariant {
  using Self = rq::AdapterOverload;

  explicit RQ_ALWAYS_INLINE AdapterOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterSpecialization final : public rq::AdapterVariant {
  using Self = rq::AdapterVariant;

  explicit RQ_ALWAYS_INLINE AdapterSpecialization();

  RQ_ALWAYS_INLINE void setAdapterTemplate(rq::AdapterTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterTemplate *
  getAdapterTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterTemplate *getAdapterTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstructorVariant : public rq::Variant {
  using Self = rq::ConstructorVariant;

  rq::ConstructorVariant *_next_ptr{nullptr};
  rq::LayoutType *_layout_ptr{nullptr};
  rq::ClassVariant *_class_variant_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ConstructorVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLayoutType(rq::LayoutType &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutType *getLayoutTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutType *getLayoutTypePtr();
  RQ_ALWAYS_INLINE vodi setClassVariant(rq::ClassVariant &variant);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassVariant *
  getClassVariantPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassVariant *getClassVariantPtr();
  RQ_ALWAYS_INLINE void setClassPolymorph(rq::ClassPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassPolymorph *
  getClassPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassPolymorph *getClassPolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ConstructorOverload final : public rq::ConstructorVariant {
  using Self = rq::ConstructorOverload;

  explicit RQ_ALWAYS_INLINE ConstructorOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LayoutConstructorOverload final : public rq::LayoutConstructorOverload {
  using Self = rq::LayoutConstructorOverload;

  explicit RQ_ALWAYS_INLINE LayoutConstructorOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionVariant : public rq::Variant {
  using Self = rq::FunctionVariant;

  rq::SignatureType *_signature_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE FunctionVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setSignatureType(rq::SignatureType &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureType *
  getSignatureTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureType *getSignatureTypePtr();
  RQ_ALWAYS_INLINE void setFunctionPolymorph(rq::FunctionPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionPolymorph *
  getFunctionPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionPolymorph *
  getFunctionPolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionOverload final : public rq::FunctionVariant {
  using Self = rq::FunctionOverload;

  explicit RQ_ALWAYS_INLINE FunctionOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionSpecialization final : public rq::FunctionVariant {
  using Self = rq::FunctionSpecialization;

  explicit RQ_ALWAYS_INLINE FunctionSpecialization();

  RQ_ALWAYS_INLINE void setFunctionTemplate(rq::FunctionTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionTemplate *
  getFunctionTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionTemplate *getFunctionTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalVariableVariant : public rq::Variant {
  using Self = rq::GlobalVariableVariant;

  rq::ConstantSymbol *_type_ptr{nullptr};
  rq::Expression *_type_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalVariableVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *getTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getTypePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableVariant : public rq::GlobalVariableVariant {
  using Self = rq::GlobalDynamicVariableVariant;

  rq::Expression *_initial_rvalue_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setGlobalDynamicVariablePolymorph(
      rq::GlobalDynamicVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariablePolymorph *
  getGlobalDynamicVariablePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariablePolymorph *
  getGlobalDynamicVariablePolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableOverload final
    : public rq::GlobalDynamicVariableVariant {
  using Self = rq::GlobalDynamicVariableOverload;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableSpecialization final
    : public rq::GlobalDynamicVariableVariant {
  using Self = rq::GlobalDynamicVariableSpecialization;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableSpecialization();

  RQ_ALWAYS_INLINE void setGlobalDynamicVariableTemplate(
      rq::GlobalDynamicVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariableTemplate *
  getGlobalDynamicVariableTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariableTemplate *
  getGlobalDynamicVariableTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableVariant : public rq::GlobalVariableVariant {
  using Self = rq::GlobalStaticVariableVariant;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableVariant(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setGlobalStaticVariablePolymorph(
      rq::GlobalStaticVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStaticVariablePolymorph *
  getGlobalStaticVariablePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStaticVariablePolymorph *
  getGlobalStaticVariablePolymorphPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableOverload final
    : public rq::GlobalStaticVariableVariant {
  using Self = rq::GlobalStaticVariableOverload;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableOverload();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableSpecialization final
    : public rq::GlobalStaticVariableVariant {
  using Self = rq::GlobalStaticVariableVariant;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableSpecialization();

  RQ_ALWAYS_INLINE void setGlobalStaticVariableTemplatePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Template : public rq::GlobalDeclaration {
  using Self = rq::Template;

  rq::Template *_next_ptr{nullptr};
  rq::Variant *_first_variant_ptr{nullptr};
  rq::LayoutType *_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Template(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLayoutType(rq::LayoutType &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutType *getLayoutTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutType *getLayoutTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSpecialization() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Variant>
  getSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Variant>
  getSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Variant>
  getConstSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  rq::ConstructorVariant *_first_constructor_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  RQ_ALWAYS_INLINE void
  addClassSpecialization(rq::ClassSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::ClassSpecialization>
      getClassSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::ClassSpecialization>
      getClassSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::ClassSpecialization>
      getConstClassSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getConstClassWeightLevelSubrange() const;
  RQ_ALWAYS_INLINE void addConstructorVariant(rq::ConstructorVariant &variant);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::ConstructorVariant>
  getConstructorVariantSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::ConstructorVariant>
  getConstructorVariantSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::ConstructorVariant>
  getConstConstructorVariantSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumTemplate final : public rq::Template {
  using Self = rq::EnumTemplate;

  explicit RQ_ALWAYS_INLINE EnumTemplate();

  RQ_ALWAYS_INLINE void
  addEnumSpecialization(rq::EnumSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::EnumSpecialization>
      getEnumSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::EnumSpecialization>
      getEnumSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::EnumSpecialization>
      getConstEnumSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  RQ_ALWAYS_INLINE void
  addInterfaceSpecialization(rq::InterfaceSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::InterfaceSpecialization>
      getInterfaceSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::InterfaceSpecialization>
      getInterfaceSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::InterfaceSpecialization>
      getConstInterfaceSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterTemplate final : public rq::Template {
  using Self = rq::AdapterTemplate;

  explicit RQ_ALWAYS_INLINE AdapterTemplate();

  RQ_ALWAYS_INLINE void
  addAdapterSpecialization(rq::AdapterSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::AdapterSpecialization>
      getAdapterSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::AdapterSpecialization>
      getAdapterSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::AdapterSpecialization>
      getConstAdapterSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate();

  RQ_ALWAYS_INLINE void
  addFunctionSpecialization(rq::FunctionSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::FunctionSpecialization>
      getFunctionSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::FunctionSpecialization>
      getFunctionSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::FunctionSpecialization>
      getConstFunctionSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplate();

  RQ_ALWAYS_INLINE void addGlobalDynamicVariableSpecialization(
      rq::GlobalDynamicVariableSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::GlobalDynamicVariableSpecialization>
      getGlobalDynamicVariableSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant,
                            rq::GlobalDynamicVariableSpecialization>
      getGlobalDynamicVariableSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant,
                            rq::GlobalDynamicVariableSpecialization>
      getConstGlobalDynamicVariableSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplate();

  RQ_ALWAYS_INLINE void addGlobalStaticVariableSpecialization(
      rq::GlobalStaticVariableSpecialization &specialization);

  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::GlobalStaticVariableSpecialization>
      getGlobalStaticVariableSpecializationSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalStaticVariableSpecialization>
      getGlobalStaticVariableSpecializationSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalStaticVariableSpecialization>
      getConstGlobalStaticVariableSpecializationSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Polymorph : public rq::Symbol {
  using Self = rq::Polymorph;

  rq::Variant *_first_overload_ptr{nullptr};
  rq::WeightLevel *_first_weight_level_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverload() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Variant>
  getOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Variant>
  getNextOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Variant>
  getConstNextOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasWeightLevel() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::WeightLevel>
  getWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
  getWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
  getConstWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

  RQ_ALWAYS_INLINE void addClassOverload(rq::ClassOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::ClassOverload>
      getClassOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::ClassOverload>
      getClassOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::ClassOverload>
      getConstClassOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addClassWeightLevel(rq::ClassWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getClassWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getConstClassWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumPolymorph final : public rq::Polymorph {
  using Self = rq::EnumPolymorph;

  explicit RQ_ALWAYS_INLINE EnumPolymorph();

  RQ_ALWAYS_INLINE void addEnumOverload(rq::EnumOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Variant, rq::EnumOverload>
  getEnumOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::EnumOverload>
      getEnumOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::EnumOverload>
      getConstEnumOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addEnumWeightLevel(rq::EnumWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
      getEnumWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
      getConstEnumWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

  RQ_ALWAYS_INLINE void addInterfaceOverload(rq::InterfaceOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::InterfaceOverload>
      getInterfaceOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::InterfaceOverload>
      getInterfaceOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::InterfaceOverload>
      getConstInterfaceOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addInterfaceWeightLevel(rq::InterfaceWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
      getInterfaceWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
      getConstInterfaceWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterPolymorph final : public rq::Polymorph {
  using Self = rq::AdapterPolymorph;

  rq::AdapterOverloadOverride *_first_override_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterPolymorph();

  RQ_ALWAYS_INLINE void addAdapterOverload(rq::AdapterOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::AdapterOverload>
      getAdapterOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::AdapterOverload>
      getAdapterOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::AdapterOverload>
      getConstAdapterOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addAdapterWeightLevel(rq::AdapterWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getAdapterWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getConstAdapterWeightLevelSubrange() const;
  RQ_ALWAYS_INLINE void
  addAdapterOverloadOverride(rq::AdapterOverloadOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::AdapterOverloadOverride>
  getAdapterOverloadOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::AdapterOverloadOverride>
      getAdapterOverloadOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::AdapterOverloadOverride>
      getConstAdapterOverrideSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionPolymorph final : public rq::Polymorph {
  using Self = rq::FunctionPolymorph;

  rq::FunctionOverloadOverride *_first_override_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE FunctionPolymorph();

  RQ_ALWAYS_INLINE void addFunctionOverload(rq::FunctionOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::FunctionOverload>
      getFunctionOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::FunctionOverload>
      getFunctionOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::FunctionOverload>
      getConstFunctionOverloadSubrange() const;
  RQ_ALWAYS_INLINE void
  addFunctionWeightLevel(rq::FunctionWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
      getFunctionWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
      getConstFunctionWeightLevelSubrange() const;
  RQ_ALWAYS_INLINE void
  addFunctionOverloadOverride(rq::FunctionOverloadOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::FunctionOverloadOverride>
  getFunctionOverloadOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::FunctionOverloadOverride>
      getFunctionOverloadOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::FunctionOverloadOverride>
      getConstFunctionOverloadOverrideSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariablePolymorph();

  RQ_ALWAYS_INLINE void
  addGlobalDynamicVariableOverload(rq::GlobalDynamicVariableOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::GlobalDynamicVariableOverload>
      getGlobalDynamicVariableOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalDynamicVariableOverload>
      getGlobalDynamicVariableOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalDynamicVariableOverload>
      getConstGlobalDynamicVariableOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addGlobalDynamicVariableWeightLevel(
      rq::GlobalDynamicVariableWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::GlobalDynamicVariableWeightLevel>
      getGlobalDynamicVariableWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel,
                            rq::GlobalDynamicVariableWeightLevel>
      getConstGlobalDynamicVariableWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariablePolymorph();

  RQ_ALWAYS_INLINE void
  addGlobalStaticVariableOverload(rq::GlobalStaticVariableOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Variant, rq::GlobalStaticVariableOverload>
      getGlobalStaticVariableOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalStaticVariableOverload>
      getGlobalStaticVariableOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Variant, rq::GlobalStaticVariableOverload>
      getConstGlobalStaticVariableOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addGlobalStaticVariableWeightLevel(
      rq::GlobalStaticVariableWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::GlobalStaticVariableWeightLevel>
      getGlobalStaticVariableWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel,
                            rq::GlobalStaticVariableWeightLevel>
      getConstGlobalStaticVariableWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct WeightLevel : public rq::Symbol {
  using Self = rq::WeightLevel;

  rq::WeightLevel *_next_ptr{nullptr};
  rq::Template *_first_template_ptr{nullptr};
  rq::TemplateOverride *_first_override_ptr{nullptr};
  rq::Polymorph *_polymorph_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE WeightLevel(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextIterator<rq::Template>
  getTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextIterator<rq::Template>
  getTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextIterator<rq::Template>
  getConstTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverride() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::TemplateOverride>
  getTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::TemplateOverride>
  getTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::TemplateOverride>
  getConstTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph *getPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph *getPolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassWeightLevel final : public rq::WeightLevel {
  using Self = rq::ClassWeightLevel;

  explicit RQ_ALWAYS_INLINE ClassWeightLevel();

  RQ_ALWAYS_INLINE void addClassTemplate(rq::ClassTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>
      getConstClassTemplateSubrange() const;
  RQ_ALWAYS_INLINE void
  addClassTemplateOverride(rq::ClassTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride, rq::ClassTemplateOverride>
      getClassTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::ClassTemplateOverride>
      getClassTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::ClassTemplateOverride>
      getConstClassTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setClassPolymorph(rq::ClassPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassPolymorph *
  getClassPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassPolymorph *getClassPolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumWeightLevel final : public rq::WeightLevel {
  using Self = rq::EnumWeightLevel;

  explicit RQ_ALWAYS_INLINE EnumWeightLevel();

  RQ_ALWAYS_INLINE void addEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::EnumTemplate>
      getEnumTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>
      getEnumTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>
      getConstEnumTemplateSubrange() const;
  RQ_ALWAYS_INLINE void
  addEnumTemplateOverride(rq::EnumTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride, rq::EnumTemplateOverride>
      getEnumTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::EnumTemplateOverride>
      getEnumTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::EnumTemplateOverride>
      getConstEnumTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setEnumPolymorph(rq::EnumPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
  getEnumPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *getEnumPolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceWeightLevel final : public rq::WeightLevel {
  using Self = rq::InterfaceWeightLevel;

  explicit RQ_ALWAYS_INLINE InterfaceWeightLevel();

  RQ_ALWAYS_INLINE void addInterfaceTemplate(rq::InterfaceTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>
      getInterfaceTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>
      getInterfaceTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>
      getConstInterfaceTemplateSubrange() const;
  RQ_ALWAYS_INLINE void
  addInterfaceTemplateOverride(rq::InterfaceTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride, rq::InterfaceTemplateOverride>
      getInterfaceTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::InterfaceTemplateOverride>
      getInterfaceTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::InterfaceTemplateOverride>
      getConstInterfaceTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void
  setInterfacePolymorph(rq::InterfacePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfacePolymorph *
  getInterfacePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfacePolymorph *
  getInterfacePolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterWeightLevel final : public rq::WeightLevel {
  using Self = rq::AdapterWeightLevel;

  explicit RQ_ALWAYS_INLINE AdapterWeightLevel();

  RQ_ALWAYS_INLINE void addAdapterTemplate(rq::AdapterTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::AdapterTemplate>
      getAdapterTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>
      getAdapterTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>
      getConstAdapterTemplateSubrange() const;
  RQ_ALWAYS_INLINE void
  addAdapterTemplateOverride(rq::AdapterTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride, rq::AdapterTemplateOverride>
      getAdapterTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::AdapterTemplateOverride>
      getAdapterTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::AdapterTemplateOverride>
      getConstAdapterTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setAdapterPolymorph(rq::AdapterPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterPolymorph *
  getAdapterPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterPolymorph *getAdapterPolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionWeightLevel final : public rq::WeightLevel {
  using Self = rq::FunctionWeightLevel;

  explicit RQ_ALWAYS_INLINE FunctionWeightLevel();

  RQ_ALWAYS_INLINE void addFunctionTemplate(rq::FunctionTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::FunctionTemplate>
      getFunctionTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>
      getFunctionTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>
      getConstFunctionTemplateSubrange() const;
  RQ_ALWAYS_INLINE void
  addFunctionTemplateOverride(rq::FunctionTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride, rq::FunctionTemplateOverride>
      getFunctionTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::FunctionTemplateOverride>
      getFunctionTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride, rq::FunctionTemplateOverride>
      getConstFunctionTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setFunctionPolymorph(rq::FunctionPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionPolymorph *
  getFunctionPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionPolymorph *
  getFunctionPolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableWeightLevel final : public rq::WeightLevel {
  using Self = rq::GlobalDynamicVariableWeightLevel;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableWeightLevel();

  RQ_ALWAYS_INLINE void addGlobalDynamicVariableTemplate(
      rq::GlobalDynamicVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::GlobalDynamicVariableTemplate>
      getGlobalDynamicVariableTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::GlobalDynamicVariableTemplate>
      getGlobalDynamicVariableTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::GlobalDynamicVariableTemplate>
      getConstGlobalDynamicVariableTemplateSubrange() const;
  RQ_ALWAYS_INLINE void addGlobalDynamicVariableTemplateOverride(
      rq::GlobalDynamicVariableTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride,
                       rq::GlobalDynamicVariableTemplateOverride>
      getGlobalDynamicVariableTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride,
                            rq::GlobalDynamicVariableTemplateOverride>
      getGlobalDynamicVariableTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride,
                            rq::GlobalDynamicVariableTemplateOverride>
      getConstGlobalDynamicVariableTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setGlobalDynamicVariablePolymorph(
      rq::GlobalDynamicVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariablePolymorph *
  getGlobalDynamicVariablePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariablePolymorph *
  getGlobalDynamicVariablePolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalStaticVariableWeightLevel final : public rq::WeightLevel {
  using Self = rq::GlobalStaticVariableWeightLevel;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableWeightLevel();

  RQ_ALWAYS_INLINE void
  addGlobalStaticVariableTemplate(rq::GlobalStaticVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::Template, rq::GlobalStaticVariableTemplate>
      getGlobalStaticVariableTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::GlobalStaticVariableTemplate>
      getGlobalStaticVariableTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::Template, rq::GlobalStaticVariableTemplate>
      getConstGlobalStaticVariableTemplateSubrange() const;
  RQ_ALWAYS_INLINE void addGlobalStaticVariableTemplateOverride(
      rq::GlobalStaticVariableTemplateOverride &override);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextIterator<rq::TemplateOverride,
                       rq::GlobalStaticVariableTemplateOverride>
      getGlobalStaticVariableTemplateOverrideSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride,
                            rq::GlobalStaticVariableTemplateOverride>
      getGlobalStaticVariableTemplateOverrideSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextIterator<rq::TemplateOverride,
                            rq::GlobalStaticVariableTemplateOverride>
      getConstGlobalStaticVariableTemplateOverrideSubrange() const;
  RQ_ALWAYS_INLINE void setGlobalStaticVariablePolymorph(
      rq::GlobalStaticVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStaticVariablePolymorph *
  getGlobalStaticVariablePolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStaticVariablePolymorph *
  getGlobalStaticVariablePolymorphPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct Override : public rq::Symbol {
  using Self = rq::Override;

  rq::Symbol *_parent_ptr{nullptr};
  rq::GlobalDeclaration *_overriden_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Override(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *getParentSymbolPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *getParentSymbolPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDeclaration *
  getOverridenGlobalDeclarationPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDeclaration *
  getOverridenGlobalDeclarationPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct OverloadOverride : public rq::Override {
  using Self = rq::OverloadOverride;

  explicit RQ_ALWAYS_INLINE OverloadOverride(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph *
  getParentPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph *getParentPolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Variant *
  getOverridenOverloadPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Variant *getOverridenOverloadPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterOverloadOverride final : public rq::OverloadOverride {
  using Self = rq::AdapterOverloadOverride;

  explicit RQ_ALWAYS_INLINE AdapterOverloadOverride();

  RQ_ALWAYS_INLINE void
  setParentAdapterPolymorph(rq::AdapterPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterPolymorph *
  getParentAdapterPolymorphPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterPolymorph *
  getParentAdapterPolymorphPtr();
  RQ_ALWAYS_INLINE void
  setOverridenAdapterOverload(rq::AdapterOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterOverload &
  getOverridenAdapterOverload() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterOverload &
  getOverridenAdapterOverload();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionOverloadOverride final : public rq::OverloadOverride {
  using Self = rq::FunctionOverloadOverride;

  explicit RQ_ALWAYS_INLINE FunctionOverloadOverride();

  RQ_ALWAYS_INLINE void
  setParentFunctionPolymorph(rq::FunctionPolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionPolymorph *
  getParentFunctionPolymorph() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionPolymorph *
  getParentFunctionPolymorphPtr();
  RQ_ALWAYS_INLINE void
  setOverridenFunctionOverload(rq::FunctionOverload &overload);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionOverload *
  getOverridenFunctionOverloadPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionOverload *
  getOverridenFunctionOverloadPtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct TemplateOverride : public rq::Override {
  using Self = rq::TemplateOverride;

  explicit RQ_ALWAYS_INLINE TemplateOverride(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::WeightLevel *
  getParentWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WeightLevel *getParentWeightLevelPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *
  getOverridenTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template *getOverridenTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct ClassTemplateOverride final : public rq::TemplateOverride {
  using Self = rq::ClassTemplateOverride;

  explicit RQ_ALWAYS_INLINE ClassTemplateOverride();

  RQ_ALWAYS_INLINE void
  setParentClassWeightLevel(rq::ClassWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassWeightLevel *
  getParentClassWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassWeightLevel *
  getParentClassWeightLevelPtr();
  RQ_ALWAYS_INLINE void setOverridenClassTemplate(rq::ClassTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassTemplate *
  getOverridenClassTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassTemplate *
  getOverridenClassTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct EnumTemplateOverride final : public rq::TemplateOverride {
  using Self = rq::EnumTemplateOverride;

  explicit RQ_ALWAYS_INLINE EnumTemplateOverride();

  RQ_ALWAYS_INLINE void setParentEnumWeightLevel(rq::WeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumWeightLevel *
  getParentEnumWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumWeightLevel *
  getParentEnumWeightLevelPtr();
  RQ_ALWAYS_INLINE void setOverridenEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
  getOverridenEnumTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *
  getOverridenEnumTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct InterfaceTemplateOverride final : public rq::TemplateOverride {
  using Self = rq::InterfaceTemplateOverride;

  explicit RQ_ALWAYS_INLINE InterfaceTemplateOverride();

  RQ_ALWAYS_INLINE void
  setParentInterfaceWeightLevel(rq::InterfaceWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceWeightLevel *
  getParentInterfaceWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceWeightLevel *
  getParentInterfaceWeightLevelPtr();
  RQ_ALWAYS_INLINE void
  setOverridenInterfaceTemplate(rq::InterfaceTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceTemplate *
  getOverridenInterfaceTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceTemplate *
  getOverridenInterfaceTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct AdapterTemplateOverride final : public rq::TemplateOverride {
  using Self = rq::AdapterTemplateOverride;

  explicit RQ_ALWAYS_INLINE AdapterTemplateOverride();

  RQ_ALWAYS_INLINE void
  setParentAdapterWeightLevel(rq::AdapterWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterWeightLevel *
  getParentAdapterWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterWeightLevel *
  getParentAdapterWeightLevelPtr();
  RQ_ALWAYS_INLINE void
  setOverridenAdapterTemplate(rq::AdapterTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterTemplate *
  getOverridenAdapterTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterTemplate *
  getOverridenAdapterTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct FunctionTemplateOverride final : public rq::TemplateOverride {
  using Self = rq::FunctionTemplateOverride;

  explicit RQ_ALWAYS_INLINE FunctionTemplateOverride();

  RQ_ALWAYS_INLINE void
  setParentFunctionWeightLevel(rq::FunctionWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionWeightLevel *
  getParentFunctionWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionWeightLevel *
  getParentFunctionWeightLevelPtr();
  RQ_ALWAYS_INLINE void
  setOverridenFunctionTemplate(rq::FunctionTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionTemplate *
  getOverridenFunctionTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionTemplate *
  getOverridenFunctionTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableTemplateOverride final
    : public rq::TemplateOverride {
  using Self = rq::GlobalDynamicVariableTemplateOverride;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplateOverride();

  RQ_ALWAYS_INLINE void setParentGlobalDynamicVariableWeightLevel(
      rq::GlobalDynamicVariableWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariableWeightLevel *
  getParentGlobalDynamicVariableWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariableWeightLevel *
  getParentGlobalDynamicVariableWeightLevelPtr();
  RQ_ALWAYS_INLINE void setOverridenGlobalDynamicVariableTemplate(
      rq::GlobalDynamicVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariableTemplate *
  getOverridenGlobalDynamicVariableTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariableTemplate *
  getOverridenGlobalDynamicVariableTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

struct GlobalStaticVariableTemplateOverride final
    : public rq::TemplateOverride {
  using Self = rq::GlobalStaticVariableTemplateOverride;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplateOverride();

  RQ_ALWAYS_INLINE void setParentGlobalStaticVariableWeightLevel(
      rq::GlobalStaticVariableWeightLevel &weight_level);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStaticVariableWeightLevel *
  getParentGlobalStaticVariableWeightLevelPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStaticVariableWeightLevel *
  getParentGlobalStaticVariableWeightLevelPtr();
  RQ_ALWAYS_INLINE void setOverridenGlobalStaticVariableTemplate(
      rq::GlobalStaticVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStaticVariableTemplate *
  getOverridenGlobalStaticVariableTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStaticVariableTemplate *
  getOverridenGlobalStaticVariableTemplatePtr();

  [[nodiscard]] static inline bool classof(rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>