#pragma once

#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/iterators.hpp>
#include <rq/name.hpp>
#include <rq/node_list.hpp>
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
struct BumpPtrAllocator;
struct Value;

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind);

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

[[nodiscard]] inline llvm::StringRef getName(rq::EvaluationState state);

template <typename ParentParam, rq::SymbolInfoFlags FLAG_PARAM>
struct SimpleSymbolBranch;
template <typename ParentParam, rq::SymbolKind KIND_PARAM>
struct SimpleSymbolLeaf;

// clang-format off
struct Symbol;
  using LiteralType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::LITERAL>;
    using IntegerLiteralType = rq::SimpleSymbolLeaf<rq::LiteralType, rq::SymbolKind::INTEGER_LITERAL_TYPE>;
    using FloatLiteralType = rq::SimpleSymbolLeaf<rq::LiteralType, rq::SymbolKind::FLOAT_LITERAL_TYPE>;
    using StringLiteralType = rq::SimpleSymbolLeaf<rq::LiteralType, rq::SymbolKind::STRING_LITERAL_TYPE>;
    using CodeunitLiteralType = rq::SimpleSymbolLeaf<rq::LiteralType, rq::SymbolKind::CODEUNIT_LITERAL_TYPE>;
  using FigurativeType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::FIGURATIVE>;
    using UnknownType = rq::SimpleSymbolLeaf<rq::FigurativeType, rq::SymbolKind::UNKNOWN_TYPE>;
    using InferenceType = rq::SimpleSymbolLeaf<rq::FigurativeType, rq::SymbolKind::INFERENCE_TYPE>;
    using VoidType = rq::SimpleSymbolLeaf<rq::FigurativeType, rq::SymbolKind::VOID_TYPE>;
    using NoReturnType = rq::SimpleSymbolLeaf<rq::FigurativeType, rq::SymbolKind::NO_RETURN_TYPE>;
  using ReflectionType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::REFLECTION_TYPE>;
    using ExpressionType = rq::SimpleSymbolLeaf<rq::ReflectionType, rq::SymbolKind::EXPRESSION_TYPE>;
    using ExpressionRangeType = rq::SimpleSymbolLeaf<rq::ReflectionType, rq::SymbolKind::EXPRESSION_RANGE_TYPE>;
    using SymbolRangeType = rq::SimpleSymbolLeaf<rq::ReflectionType, rq::SymbolKind::SYMBOL_RANGE_TYPE>;
  using FittingPrimitiveType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE>;
    using FittingFSIntType = rq::SimpleSymbolLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_FSINT_TYPE>;
    using FittingFUIntType = rq::SimpleSymbolLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_FUINT_TYPE>;
    using FittingLSIntType = rq::SimpleSymbolLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_LUINT_TYPE>;
    using SymbolRangeType = rq::SimpleSymbolLeaf<rq::ReflectionType, rq::SymbolKind::SYMBOL_RANGE_TYPE>;
  using PlatformPrimitiveType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE>;
    using BooleanType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::BOOLEAN_TYPE>;
    using HalfType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::HALF_TYPE>;
    using SingleType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINGLE_TYPE>;
    using DoubleType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::DOUBLE_TYPE>;
    using QuadrupleType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::QUADRUPLE_TYPE>;
    using SIntType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINT_TYPE>;
    using UIntType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::UINT_TYPE>;
    using SSizeType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SSIZE_TYPE>;
    using USizeType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::USIZE_TYPE>;
    using SIndexType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINDEX_TYPE>;
    using CharType = rq::SimpleSymbolLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::CHAR_TYPE>;
  using StandardPrimitiveType = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE>;
    using Binary16Type = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY16_TYPE>;
    using Binary32Type = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY32_TYPE>;
    using Binary64Type = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY128_TYPE>;
    using BFloat16Type = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BFLOAT16_TYPE>;
    using AsciiType = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ASCII_TYPE>;
    using Utf8Type = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::UTF8_TYPE>;
  struct ScaledPrimitiveType;
    struct ScaledSIntType;
    struct ScaledUIntType;
    struct ScaledFSIntType;
    struct ScaledFUIntType;
    struct ScaledLSIntType;
    struct ScaledLUIntType;
  using ModifierValue = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::MODIFIER>;
    using NoModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::NO_MODIFIER>;
    using AnchorModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ANCHOR_MODIFIER>;
    using ResidentModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RESIDENT_MODIFIER>;
    using FlankModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::FLANK_MODIFIER>;
    using TransparentModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::TRANSPARENT_MODIFIER>;
    using OpaqueModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::OPAQUE_MODIFIER>;
    using PrivateModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRIVATE_MODIFIER>;
    using PublicModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PUBLIC_MODIFIER>;
    using ExportModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EXPORT_MODIFIER>;
    using MutableModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MUTABLE_MODIFIER>;
    using PartiallyMutableModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PARTIALLY_MUTABLE_MODIFIER>;
    using ConstantModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONSTANT_MODIFIER>;
    using EagerModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EAGER_MODIFIER>;
    using LazyModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LAZY_MODIFIER>;
    using PostModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::POST_MODIFIER>;
    using PreModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRE_MODIFIER>;
    using AliasModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ALIAS_MODIFIER>;
    using EnumeratorModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ENUMERATOR_MODIFIER>;
    using ConceptModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONCEPT_MODIFIER>;
    using SplitModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SPLIT_MODIFIER>;
    using RuntimeModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RUNTIME_MODIFIER>;
    using GentimeModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::GENTIME_MODIFIER>;
    using AnytimeModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ANYTIME_MODIFIER>;
    using PresetModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRESET_MODIFIER>;
    using SingletonModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SINGLETON_MODIFIER>;
    using CaptureModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CAPTURE_MODIFIER>;
    using StatelessModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::STATELESS_MODIFIER>;
    using LinkedModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LINKED_MODIFIER>;
    using InlineModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::INLINE_MODIFIER>;
    using PureModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PURE_MODIFIER>;
    using ImpureModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::IMPURE_MODIFIER>;
    using StandardMangleModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::STANDARD_MANGLE_MODIFIER>;
    using MangleModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MANGLE_MODIFIER>;
    using PadModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PAD_MODIFIER>;
    using PackModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PACK_MODIFIER>;
    using EquivocalModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EQUIVOCAL_MODIFIER>;
    using LikelyModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LIKELY_MODIFIER>;
    using UnlikelyModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::UNLIKELY_MODIFIER>;
    using SupportedModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SUPPORTED_MODIFIER>;
    using DepreciatedModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::DEPRECIATED_MODIFIER>;
    using ExperimentalModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EXPERIMENTAL_MODIFIER>;
    using UnstableAddressModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::UNSTABLE_ADDRESS_MODIFIER>;
    using StableAddressModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::STABLE_ADDRESS_MODIFIER>;
    using InvariadicModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::INVARIADIC_MODIFIER>;
    using VariadicModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::VARIADIC_MODIFIER>;
    using BestLocationModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BEST_LOCATION_MODIFIER>;
    using LocationModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LOCATION_MODIFIER>;
    using TemplateModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::TEMPLATE_MODIFIER>;
    using OverloadModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::OVERLOAD_MODIFIER>;
    using ConstraintModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONSTRAINT_MODIFIER>;
    using DefaultWeightModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::DEFAULT_WEIGHT_MODIFIER>;
    using WeightModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::WEIGHT_MODIFIER>;
    using ManualModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MANUAL_MODIFIER>;
    using AutoModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::AUTO_MODIFIER>;
    using DirectModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::DIRECT_MODIFIER>;
    using VirualModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::VIRTUAL_MODIFIER>;
    using RangerModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RANGER_MODIFIER>;
    using RequireModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::REQUIRE_MODIFIER>;
    using EnsureModifier = rq::SimpleSymbolLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ENSURE_MODIFIER>;
  using QualifierValue = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::QUALIFIER>;
    using NoMutQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_MUT_QUALIFIER>;
    using PartialMutQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::PARTIAL_MUT_QUALIFIER>;
    using MutQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::MUT_QUALIFIER>;
    using NoVolatileQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_VOLATILE_QUALIFIER>;
    using VolatileQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::VOLATILE_QUALIFIER>;
    using NoAtomicQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_ATOMIC_QUALIFIER>;
    using AtomicQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::ATOMIC_QUALIFIER>;
    using NoNullTerminateQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_NULL_TERMINATE_QUALIFIER>;
    using NullTerminateQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NULL_TERMINATE_QUALIFIER>;
    using NoMarginQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_MARGIN_QUALIFIER>;
    using MarginQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::MARGIN_QUALIFIER>;
    using NoThrowsQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::NO_THROWS_QUALIFIER>;
    using ThrowsQualifier = rq::SimpleSymbolLeaf<rq::Qualifier, rq::SymbolKind::THROWS_QUALIFIER>;
  using BuiltinConcept = rq::SimpleSymbolBranch<rq::Symbol, rq::SymbolInfoFlags::BUILTIN_CONCEPT>;
    using AnyConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ANY_CONCEPT>;
    using TypeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::TYPE_CONCEPT>;
    using GentimeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::GENTIME_CONCEPT>;
    using RuntimeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::RUNTIME_CONCEPT>;
    using AnytimeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ANYTIME_CONCEPT>;
    using RangeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::RANGE_CONCEPT>;
    using RouteConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ROUTE_CONCEPT>;
    using FigurativeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FIGURATIVE_CONCEPT>;
    using ReflectionConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::REFLECTION_CONCEPT>;
    using PrimtiveConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PRIMITIVE_CONCEPT>;
    using FittingPrimitiveConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FITTING_PRIMITIVE_CONCEPT>;
    using PlatformPrimitiveConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PLATFORM_PRIMITIVE_CONCEPT>;
    using StandardPrimitiveConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::STANDARD_PRIMITIVE_CONCEPT>;
    using ScaledPrimitiveConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SCALED_PRIMITIVE_CONCEPT>;
    using SignedConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNED_CONCEPT>;
    using UnsignedConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::UNSIGNED_CONCEPT>;
    using IntConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INT_CONCEPT>;
    using ExactIntConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::EXACT_INT_CONCEPT>;
    using FastIntConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FAST_INT_CONCEPT>;
    using LeastIntConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LEAST_INT_CONCEPT>;
    using FloatConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FLOAT_CONCEPT>;
    using BinaryConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::BINARY_CONCEPT>;
    using BFloatConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::BFLOAT_CONCEPT>;
    using ConceptConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_CONCEPT>;
    using SubtypeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SUBTYPE_CONCEPT>;
    using ArrayConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ARRAY_CONCEPT>;
    using RefConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::REF_CONCEPT>;
    using PtrConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PTR_CONCEPT>;
    using SliceConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SLICE_CONCEPT>;
    using SplitConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SPLIT_CONCEPT>;
    using ModuleConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MODULE_CONCEPT>;
    using ImportConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::IMPORT_CONCEPT>;
    using PortalConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PORTAL_CONCEPT>;
    using RealizationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::REALIZATION_CONCEPT>;
    using ConformityConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONFORMITY_CONCEPT>;
    using AdaptionConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTION_CONCEPT>;
    using TripConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::TRIP_CONCEPT>;
    using JuxtConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::JUXT_CONCEPT>;
    using SpecializationSetConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SPECIALIZATION_SET_CONCEPT>;
    using AdapterSpecializationSetConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_SPECIALIZATION_SET_CONCEPT>;
    using ProcedureSpecializationSetConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PROCEDURE_SPECIALIZATION_SET_CONCEPT>;
    using ArithmeticSequenceConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ARITHMETIC_SEQUENCE_CONCEPT>;
    using ArithmeticIntervalConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ARITHMETIC_INTERVAL_CONCEPT>;
    using InfiniteArithmeticSequenceConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_CONCEPT>;
    using FiniteArithmeticSequenceConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_CONCEPT>;
    using EagerDeclarationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_DECLARATION_CONCEPT>;
    using AnchorConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ANCHOR_CONCEPT>;
    using EnumeratorConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUMERATOR_CONCEPT>;
    using VariableConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIABLE_CONCEPT>;
    using EagerVariableConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_VARIABLE_CONCEPT>;
    using PostEagerVariableConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::POST_EAGER_VARIABLE_CONCEPT>;
    using PreEagerVariableConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PRE_EAGER_VARIABLE_CONCEPT>;
    using ParameterConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PARAMETER_CONCEPT>;
    using SignatureParameterConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNATURE_PARAMETER_CONCEPT>;
    using LayoutParameterConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAYOUT_PARAMETER_CONCEPT>;
    using ParameterListConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PARAMETER_LIST_CONCEPT>;
    using SignatureConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNATURE_CONCEPT>;
    using LayoutConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAYOUT_CONCEPT>;
    using PlacementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PLACEMENT_CONCEPT>;
    using CompositionConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::COMPOSITION_CONCEPT>;
    using SynonymConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SYNONYM_CONCEPT>;
    using SymbolTableConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SYMBOL_TABLE_CONCEPT>;
    using NamespaceConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::NAMESPACE_CONCEPT>;
    using EagerScopingStatementConcept= rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_SCOPING_STATEMENT_CONCEPT>;
    using IfStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::IF_STATEMENT_CONCEPT>;
    using ElseIfStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ELSE_IF_STATEMENT_CONCEPT>;
    using ElseStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ELSE_STATEMENT_CONCEPT>;
    using SwitchStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SWITCH_STATEMENT_CONCEPT>;
    using CaseStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CASE_STATEMENT_CONCEPT>;
    using DefaultStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::DEFAULT_STATEMENT_CONCEPT>;
    using ForStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::FOR_STATEMENT_CONCEPT>;
    using WhileStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::WHILE_STATEMENT_CONCEPT>;
    using ScopeStatementConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SCOPE_STATEMENT_CONCEPT>;
    using ImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::IMPLEMENTATION_CONCEPT>;
    using OverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::OVERLOAD_CONCEPT>;
    using SpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SPECIALIZATION_CONCEPT>;
    using TemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::TEMPLATE_CONCEPT>;
    using ClassImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_IMPLEMENTATION_CONCEPT>;
    using ClassOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_OVERLOAD_CONCEPT>;
    using ClassSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_SPECIALIZATION_CONCEPT>;
    using ClassTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_TEMPLATE_CONCEPT>;
    using EnumImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_IMPLEMENTATION_CONCEPT>;
    using EnumOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_OVERLOAD_CONCEPT>;
    using EnumSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_SPECIALIZATION_CONCEPT>;
    using EnumTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_TEMPLATE_CONCEPT>;
    using InterfaceImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_IMPLEMENTATION_CONCEPT>;
    using InterfaceOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_OVERLOAD_CONCEPT>;
    using InterfaceSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_SPECIALIZATION_CONCEPT>;
    using InterfaceTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_TEMPLATE_CONCEPT>;
    using AdapterImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_IMPLEMENTATION_CONCEPT>;
    using AdapterOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_OVERLOAD_CONCEPT>;
    using AdapterSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_SPECIALIZATION_CONCEPT>;
    using AdapterTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_TEMPLATE_CONCEPT>;
    using ProcedureImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PROCEDURE_IMPLEMENTATION_CONCEPT>;
    using ProcedureOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PROCEDURE_OVERLOAD_CONCEPT>;
    using ProcedureSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PROCEDURE_SPECIALIZATION_CONCEPT>;
    using ProcedureTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PROCEDURE_TEMPLATE_CONCEPT>;
    using LazyVariableIMplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_IMPLEMENTATION_CONCEPT>;
    using LazyVariableOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_OVERLOAD_CONCEPT>;
    using LazyVariableSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_SPECIALIZATION_CONCEPT>;
    using LazyVariableTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_TEMPLATE_CONCEPT>;
    using AliasImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_IMPLEMENTATION_CONCEPT>;
    using AliasOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_OVERLOAD_CONCEPT>;
    using AliasSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_SPECIALIZATION_CONCEPT>;
    using AliasTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_TEMPLATE_CONCEPT>;
    using ConceptImplementationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_IMPLEMENTATION_CONCEPT>;
    using ConceptOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_OVERLOAD_CONCEPT>;
    using ConceptSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_SPECIALIZATION_CONCEPT>;
    using ConceptTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_TEMPLATE_CONCEPT>;
    using VariantImplmentationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_IMPLEMENTATION_CONCEPT>;
    using VariantOverloadConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_OVERLOAD_CONCEPT>;
    using VariantSpecializationConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_SPECIALIZATION_CONCEPT>;
    using VariantTemplateConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_TEMPLATE_CONCEPT>;
    using AttributeConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ATTRIBUTE_CONCEPT>;
    using ModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MODIFIER_CONCEPT>;
    using AnchorModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ANCHOR_MODIFIER_CONCEPT>;
    using ContainerModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONTAINER_MODIFIER_CONCEPT>;
    using VisibilityModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VISIBILITY_MODIFIER_CONCEPT>;
    using AccessModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ACCESS_MODIFIER_CONCEPT>;
    using MutabilityModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MUTABILITY_MODIFIER_CONCEPT>;
    using EvaluationTimeModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::EVALUATION_TIME_MODIFIER_CONCEPT>;
    using DialectModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::DIALECT_MODIFIER_CONCEPT>;
    using TenureModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::TENURE_MODIFIER_CONCEPT>;
    using InitializationTimeModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::INITIALIZATION_TIME_MODIFIER_CONCEPT>;
    using ClosureModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CLOSURE_MODIFIER_CONCEPT>;
    using LinkageModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LINKAGE_MODIFIER_CONCEPT>;
    using SideEffectsModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SIDE_EFFECTS_MODIFIER_CONCEPT>;
    using MangleModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MANGLE_MODIFIER_CONCEPT>;
    using PackModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::PACK_MODIFIER_CONCEPT>;
    using BranchTrendModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::BRANCH_TREND_MODIFIER_CONCEPT>;
    using SupportNoticeModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::SUPPORT_NOTICE_MODIFIER_CONCEPT>;
    using AddressStabilityModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ADDRESS_STABILITY_MODIFIER_CONCEPT>;
    using VariadicModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIADIC_MODIFIER_CONCEPT>;
    using OffsetModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::OFFSET_MODIFIER_CONCEPT>;
    using LazyDeclarationModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_DECLARATION_MODIFIER_CONCEPT>;
    using ConstraintModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::CONSTRAINT_MODIFIER_CONCEPT>;
    using WeightModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::WEIGHT_MODIFIER_CONCEPT>;
    using DeductionModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::DEDUCTION_MODIFIER_CONCEPT>;
    using VirtualityModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VIRTUALITY_MODIFIER_CONCEPT>;
    using RangerModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::RANGER_MODIFIER_CONCEPT>;
    using RequireModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::REQUIRE_MODIFIER_CONCEPT>;
    using EnsureModifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ENSURE_MODIFIER_CONCEPT>;
    using QualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::QUALIFIER_CONCEPT>;
    using MutQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MUT_QUALIFIER_CONCEPT>;
    using VolatileQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::VOLATILE_QUALIFIER_CONCEPT>;
    using AtomicQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::ATOMIC_QUALIFIER_CONCEPT>;
    using NullTerminateQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::NULL_TERMINATE_QUALIFIER_CONCEPT>;
    using MarginQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::MARGIN_QUALIFIER_CONCEPT>;
    using ThrowsQualifierConcept = rq::SimpleSymbolLeaf<rq::BuiltinConcept, rq::SymbolKind::THROWS_QUALIFIER_CONCEPT>;
  struct Subtype;
    struct ArraySubtype;
    struct SimpleSubtype;
      struct RefSubtype;
      struct PointerSubtype;
      struct InferenceCountArraySubtype;
      struct SliceSubtype;
      struct SplitSubtype;
      struct ExistentialSubtype;
  struct Adaption;
  struct Realization;
  struct Conformity;
  struct JuxtListItem;
  struct JuxtListType;
  struct SynonymType;
  struct ArithmeticSequenceType;
    struct ArithmeticIntervalType;
    struct InfiniteArithmeticSequenceType;
    struct FiniteArithmeticSequenceTYpe;
  struct Import;
  struct Portal;
  struct SpecializationSetArgument;
  struct SpecializationSet;
    struct ProcedureSpecializationSet;
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
    struct ProcedureWeightLevel;
    struct AdapterWeightLevel;
    struct EnumWeightLevel;
    struct ClassWeightLevel;
    struct LazyVariableWeightLevel;
    struct AliasWeightLevel;
    struct ConceptWeightLevel;
  struct Polymorph;
    struct EnumPolymorph;
    struct ClassPolymorph;
    struct InterfacePolymorph;
    struct AdapterPolymorph;
    struct ProcedurePolymorph;
    struct LazyVariablePolymorph;
    struct AliasPolymorph;
    struct ConceptPolymorph;
  struct TableMember;
    struct EagerDeclaration;
      struct Anchor;
      struct Enumerator;
      struct EagerVariable;
        struct PostEagerVariable;
        struct PreEagerVariable;
    struct SymbolTable;
      struct CTable;
      struct Module;
      struct EagerScope;
        struct ScopeStatement;
        struct WhileStatement;
        struct ForStatement;
        struct DefaultStatement;
        struct CaseStatement;
        struct SwitchStatement;
        struct IfStatement;
        struct ElseIfStatement;
        struct ElseStatement;
      struct NamedTable;
        struct Node;
        struct LazyDeclaration;
          struct Implementation;
            struct EnumImplementation;
              struct EnumOverload;
              struct EnumSpecialization;
            struct LazyVariableImplementation;
              struct LazyVariableOverload;
              struct LazyVariableSpecialization;
            struct AdapterImplementation;
              struct AdapterOverload;
              struct AdapterSpecialization;
            struct InterfaceImplementation;
              struct InterfaceOverload;
              struct InterfaceSpecialization;
            struct ClassImplementation;
              struct ClassOverload;
              struct ClassSpecialization;
            struct ProcedureImplementation;
              struct ProcedureOverload;
              struct ProcedureSpecialization;
            struct AliasImplementation;
              struct AliasOverload;
              struct AliasSpecialization;
            struct ConceptImplementation;
              struct ConceptOverload;
              struct ConceptSpecialization;
          struct Template;
            struct EnumTemplate;
            struct ClassTemplate;
            struct InterfaceTemplate;
            struct AdapterTemplate;
            struct ProcedureTemplate;
            struct LazyVariableTemplate;
            struct AliasTemplate;
            struct ConceptTemplate;
// clang-format on

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind getKind() const;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getDerivedExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDerivedExpressionPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

template <typename ParentParam, rq::SymbolInfoFlags FLAG_PARAM>
struct SimpleSymbolBranch : public ParentParam {
  using Parent = ParentParam;
  static constexpr rq::SymbolInfoFlags FLAG = FLAG_PARAM;
  using Self = rq::SimpleSymbolBranch<Parent, FLAG>;

  explicit RQ_ALWAYS_INLINE SimpleSymbolBranch(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

template <typename ParentParam, rq::SymbolKind KIND_PARAM>
struct SimpleSymbolLeaf final : public ParentParam {
  using Parent = ParentParam;
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::SimpleSymbolLeaf<Parent, KIND>;

  explicit RQ_ALWAYS_INLINE SimpleSymbolLeaf();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Subtype : public rq::Symbol {
  using Self = rq::Subtype;

  rq::ConstantSymbol *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::SymbolKind kind,
                                    rq::ConstantSymbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArraySubtype final : public rq::Subtype {
  using Self = rq::ArraySubtype;

  std::size_t _count;

  explicit RQ_ALWAYS_INLINE ArraySubtype(std::size_t count,
                                         rq::ConstantSymbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &inout_id,
                                          const rq::ConstantSymbol &child,
                                          std::size_t count);

struct SimpleSubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::SimpleSubtype;

  explicit RQ_ALWAYS_INLINE SimpleSubtype(rq::SymbolKind kind,
                                          rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileSimpleSubtype(llvm::FoldingSetNodeID &inout_id,
                                           rq::SymbolKind kind,
                                           const rq::ConstantSymbol &child);

struct RefSubtype final : public rq::SimpleSubtype {
  using Self = rq::RefSubtype;

  explicit RQ_ALWAYS_INLINE RefSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PtrSubtype final : public rq::SimpleSubtype {
  using Self = rq::PtrSubtype;

  explicit RQ_ALWAYS_INLINE PtrSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InferenceCountArraySubtype final : public rq::SimpleSubtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE
  InferenceCountArraySubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SliceSubtype final : public rq::SimpleSubtype {
  using Self = rq::SliceSubtype;

  explicit RQ_ALWAYS_INLINE SliceSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SplitSubtype final : public rq::SimpleSubtype {
  using Self = rq::SplitSubtype;

  explicit RQ_ALWAYS_INLINE SplitSubtype(rq::ConstantSymbol &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Adaption final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Adaption;

  rq::ConstantSymbol *_reciever_ptr;
  rq::InterfaceImplementation *_interface_ptr;
  rq::AdapterImplementation *_adapter_ptr;

  explicit RQ_ALWAYS_INLINE Adaption(rq::ConstantSymbol &reciever,
                                     rq::InterfaceImplementation &interface,
                                     rq::AdapterImplementation &adapter);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getReciever() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getReciever();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
  getAdapter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &getAdapter();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileAdaption(llvm::FoldingSetNodeID &inout_id,
                const rq::InterfaceImplementation &interface,
                const rq::AdapterImplementation &adapter);

struct Realization final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Realization; // Reciever==>Interface

  rq::ConstantSymbol *_reciever_ptr;
  rq::InterfaceImplementation *_interface_ptr;

  explicit RQ_ALWAYS_INLINE Realization(rq::ConstantSymbol &reciever,
                                        rq::InterfaceImplementation &interface);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getReciever() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getReciever();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileRealization(llvm::FoldingSetNodeID &inout_id,
                   const rq::ConstantSymbol &reciever,
                   const rq::InterfaceImplementation &interface);

struct Conformity final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Conformity; // A-->Interface

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

RQ_ALWAYS_INLINE void
profileConformity(llvm::FoldingSetNodeID &inout_id,
                  const rq::InterfaceImplementation &interface,
                  const rq::AdapterImplementation &adapter);

struct JuxtListItem final : public rq::Symbol, public llvm::FoldingSetNode {
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

RQ_ALWAYS_INLINE void profileJuxtListItem(llvm::FoldingSetNodeID &inout_id,
                                          const rq::JuxtListItem *next_ptr,
                                          const rq::ConstantSymbol &type);

struct JuxtListType final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::JuxtListType;

  rq::JuxtListItem *_first_ptr;

  explicit RQ_ALWAYS_INLINE JuxtListType(rq::JuxtListItem &first);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::JuxtListItem>
  getItemSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::JuxtListItem>
  getItemSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::JuxtListItem>
  getConstItemSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileJuxtListType(llvm::FoldingSetNodeID &inout_id,
                                          const rq::JuxtListItem *first_ptr);

struct SynonymType final : public rq::Symbol {
  using Self = rq::SynonymType;

  rq::Symbol *_original_ptr;

  explicit RQ_ALWAYS_INLINE SynonymType(rq::Symbol &original);

  [[nodiscard]] const rq::Symbol &getOriginal() const;
  [[nodiscard]] rq::Symbol &getOriginal();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArithmeticSequenceType : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceType;

  rq::ConstantSymbol *_child_ptr;
  rq::ArithmeticCondition _condition;
  rq::ArithmeticStep _step;

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
    llvm::FoldingSetNodeID &inout_id, const rq::ConstantSymbol &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step);

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

struct SpecializationSetArgument final : public rq::Symbol,
                                         public llvm::FoldingSetNode {
  using Self = rq::SpecializationSetArgument;

  rq::Name _name;
  rq::Entity *_value_ptr;
  rq::SpecializationSetArgument *_next_ptr;

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
                                 rq::Name name, const rq::Entity &value,
                                 rq::SpecializationSetArgument *next_ptr);

struct SpecializationSet : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SpecializationSet;

  rq::SpecializationSetArgument *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  SpecializationSet(rq::SymbolKind kind,
                    rq::SpecializationSetArgument *first_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::SpecializationSetArgument>
  getArgumentSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::SpecializationSetArgument>
      getArgumentSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::SpecializationSetArgument>
      getConstArgumentSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileSpecializationSet(llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
                         const rq::SpecializationSetArgument *first_ptr);

struct ProcedureSpecializationSet final : public rq::SpecializationSet {
  using Self = rq::ProcedureSpecializationSet;

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

RQ_DEFINE_FLAGS(rq::ParameterInfoFlags);

struct ParameterDetail final {
  using Self = rq::ParameterDetail;

  rq::Name _name;
  rq::ConstantSymbol *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::Entity *_default_ptr;

  explicit RQ_ALWAYS_INLINE
  ParameterDetail(rq::Name name, rq::ConstantSymbol &type,
                  rq::ModifierFuseFlags modifier_fuse_flags,
                  rq::ParameterInfoFlags param_info_flags,
                  rq::Entity *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
  getParameterInfoFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getDefaultPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getDefaultPtr();
};

struct Parameter final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Parameter;

  rq::Parameter *_next_ptr;
  rq::Name _name;
  rq::ConstantSymbol *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::Entity *_default_ptr;

  explicit RQ_ALWAYS_INLINE Parameter(rq::Parameter *next_ptr, rq::Name name,
                                      rq::ConstantSymbol &type,
                                      rq::ModifierFuseFlags modifier_flags,
                                      rq::ParameterInfoFlags param_flags,
                                      rq::Entity *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
  getParameterFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getDefaultPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getDefaultPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void profileParameter(
    llvm::FoldingSetNodeID &inout_id, const rq::Parameter *next_ptr,
    rq::Name name, const rq::ConstantSymbol &type,
    rq::ModifierFuseFlags modifier_fuse_flags,
    rq::ParameterInfoFlags param_flags, const rq::Entity *default_ptr);

struct CompositionComponent final : public rq::Symbol,
                                    public llvm::FoldingSetNode {
  using Self = rq::CompositionComponent;

  rq::CompositionComponent *_next_ptr;
  rq::InterfaceImplementation *_interface_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionComponent(rq::CompositionComponent *next_ptr,
                       rq::InterfaceImplementation &interface);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionComponent(llvm::FoldingSetNodeID &inout_id,
                            const rq::CompositionComponent *next_ptr,
                            const rq::InterfaceImplementation &interface);

struct CompositionType final : public rq::Symbol {
  using Self = rq::CompositionType;

  rq::CompositionComponent *_first_ptr;

  explicit RQ_ALWAYS_INLINE
  CompositionType(rq::CompositionComponent *first_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionType(llvm::FoldingSetNodeID &inout_id,
                       const rq::CompositionComponent *first_ptr);

struct ParameterListDetail final {
  using Self = ParameterListDetail;

  llvm::SmallVector<rq::ParameterDetail, 2> _parameter_detail_list{};
  rq::ParameterInfoFlags _flags{rq::ParameterInfoFlags::POSITIONAL};
  rq::ParameterInfoFlags _found_parameter_marks{rq::ParameterInfoFlags::NONE};

  explicit RQ_ALWAYS_INLINE ParameterListDetail() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getFoundPositionalParametersEnd() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getFoundNonpositionalParametersBegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getFoundLockedParametersBegin() const;
  RQ_ALWAYS_INLINE void appendPositionaParametersEnd();
  RQ_ALWAYS_INLINE void appendNonpositionalParametersBegin();
  RQ_ALWAYS_INLINE void appendLockedParametersBegin();
  RQ_ALWAYS_INLINE void
  appendParameter(rq::Name name, rq::ConstantSymbol &type,
                  rq::ModifierFuseFlags modifier_fuse_flags,
                  rq::Entity *default_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE std::span<const rq::ParameterDetail>
  getParameterDetailSpan() const;
};

struct ParameterList : public rq::Symbol {
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

RQ_ALWAYS_INLINE void
profileSignatureType(llvm::FoldingSetNodeID &inout_id,
                     const rq::Parameter *first_parameter_ptr,
                     const rq::ConstantSymbol &return_type);

struct LayoutType final : public rq::ParameterList {
  using Self = rq::LayoutType;

  explicit RQ_ALWAYS_INLINE LayoutType(rq::Parameter *first_parameter_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profileLayoutType(llvm::FoldingSetNodeID &inout_id,
                  const rq::Parameter *first_parameter_ptr);

struct PlacementType final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::ProcedureImplementation *_procedure_ptr;

  explicit RQ_ALWAYS_INLINE
  PlacementType(rq::ProcedureImplementation &function);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureImplementation &
  getProcedure() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureImplementation &getProcedure();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

RQ_ALWAYS_INLINE void
profilePlacementType(llvm::FoldingSetNodeID &inout_id,
                     const rq::ProcedureImplementation &function);

struct WeightLevel : public rq::Symbol {
  using Self = rq::WeightLevel;

  rq::Polymorph *_polymorph_ptr{nullptr};
  unsigned _weight;
  rq::Template *_first_ptr{nullptr};
  rq::WeightLevel *_next_ptr{nullptr};

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

struct ProcedureWeightLevel final : public rq::WeightLevel {
  using Self = rq::ProcedureWeightLevel;

  explicit RQ_ALWAYS_INLINE ProcedureWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void
  setProcedurePolymorph(rq::ProcedurePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedurePolymorph *
  getProcedurePolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedurePolymorph *
  getProcedurePolymorphPtr() const;
  RQ_ALWAYS_INLINE void addProcedureTemplate(rq::ProcedureTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::ProcedureTemplate>
      getProcedureTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>
      getProcedureTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>
      getConstProcedureTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterWeightLevel final : public rq::WeightLevel {
  using Self = rq::AdapterWeightLevel;

  explicit RQ_ALWAYS_INLINE AdapterWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void setAdapterPolymorph(rq::AdapterPolymorph &polymorph);
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
      getConstEnumTemplateSubrange() const;

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

struct LazyVariableWeightLevel final : public rq::WeightLevel {
  using Self = rq::LazyVariableWeightLevel;

  explicit RQ_ALWAYS_INLINE LazyVariableWeightLevel(unsigned weight);

  RQ_ALWAYS_INLINE void
  setLazyVariablePolymorph(rq::LazyVariablePolymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariablePolymorph *
  getLazyVariablePolymorphPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariablePolymorph *
  getLazyVariablePolymorphPtr() const;
  RQ_ALWAYS_INLINE void
  addLazyVariableTemplate(rq::LazyVariableTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::LazyVariableTemplate>
      getLazyVariableTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
      getLazyVariableTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
      getConstLazyVariableTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Polymorph : public rq::Symbol {
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

template <typename WeightLevel, typename Polymorph>
[[nodiscard]] RQ_ALWAYS_INLINE WeightLevel &
addWeightLevel(rq::BumpPtrAllocator &allocator, Polymorph &polymorph,
               unsigned weight);

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

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
  [[nodiscard]] inline rq::ClassWeightLevel &
  getClassWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getClassWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getClassWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
      getConstClassWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumPolymorph final : public rq::Polymorph {
  using Self = rq::EnumPolymorph;

  explicit RQ_ALWAYS_INLINE EnumPolymorph();

  RQ_ALWAYS_INLINE void addEnumOverload(rq::EnumOverload &class_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getConstEnumOverloadSubrange() const;
  [[nodiscard]] inline rq::EnumWeightLevel &
  getEnumWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
      getEnumWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
      getEnumWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
      getConstEnumWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

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
  [[nodiscard]] inline rq::InterfaceWeightLevel &
  getInterfaceWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
      getInterfaceWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
      getInterfaceWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
      getConstInterfaceWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyVariablePolymorph final : public rq::Polymorph {
  using Self = rq::LazyVariablePolymorph;

  explicit RQ_ALWAYS_INLINE LazyVariablePolymorph();

  RQ_ALWAYS_INLINE void
  addLazyVariableOverload(rq::LazyVariableOverload &variable);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>
      getLazyVariableOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
      getLazyVariableOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
      getConstLazyVariableOverloadSubrange() const;
  [[nodiscard]] inline rq::LazyVariableWeightLevel &
  getLazyVariableWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
      getLazyVariableWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
      getLazyVariableWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
      getConstLazyVariableWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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
  [[nodiscard]] inline rq::AdapterWeightLevel &
  getAdapterWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getAdapterWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getAdapterWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
      getConstAdapterWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedurePolymorph final : public rq::Polymorph {
  using Self = rq::ProcedurePolymorph;

  explicit RQ_ALWAYS_INLINE ProcedurePolymorph();

  RQ_ALWAYS_INLINE void addProcedureOverload(rq::ProcedureOverload &function);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::ProcedureOverload>
      getProcedureOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>
      getProcedureOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>
      getConstProcedureOverloadSubrange() const;
  [[nodiscard]] inline rq::ProcedureWeightLevel &
  getProcedureWeightLevel(rq::BumpPtrAllocator &allocator, unsigned weight);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
      getProcedureWeightLevelSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
      getProcedureWeightLevelSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
      getConstProcedureWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TableMember : public rq::Symbol {
  using Self = rq::TableMember;

  rq::SymbolTable *_container_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE TableMember(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setContainer(rq::SymbolTable &container);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getContainerPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Import final : public rq::Symbol {
  using Self = rq::Import;

  rq::Import *_next_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::Expression *_expression_ptr{nullptr};
  rq::Module *_imported_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};
  rq::Portal *_first_portal_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Import();

  RQ_ALWAYS_INLINE void setModifierFuseFlags(rq::ModifierFuseFlags flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  RQ_ALWAYS_INLINE void setImported(rq::Module &imported);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getImportedPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getImportedPtr();
  RQ_ALWAYS_INLINE void setModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getModulePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getModulePtr();
  RQ_ALWAYS_INLINE void addPortal(rq::Portal &portal);
  [[nodiscard]] rq::NextSubrange<rq::Portal> getPortalSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Portal> getPortalSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Portal>
  getConstPortalSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EagerDeclaration : public rq::TableMember {
  using Self = rq::EagerDeclaration;

  rq::Name _name{};
  rq::SymbolTable *_host_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EagerDeclaration(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  RQ_ALWAYS_INLINE void setHost(rq::SymbolTable &host);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getHostPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getHostPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Anchor final : public rq::EagerDeclaration {
  using Self = rq::Anchor;

  rq::EagerScope *_vessel_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Anchor();

  RQ_ALWAYS_INLINE void setVessel(rq::EagerScope &scope);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScope *getVesselPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EagerScope *getVesselPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Enumerator final : public rq::EagerDeclaration {
  using Self = rq::Enumerator;

  rq::ConstantWord *_word_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumerator();

  RQ_ALWAYS_INLINE void setWord(rq::ConstantWord &word);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantWord *getWordPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantWord *getWordPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EagerVariable : public rq::EagerDeclaration {
  using Self = rq::EagerVariable;

  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::ConstantSymbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EagerVariable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void
  setLowFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *getTypePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PostEagerVariable : public rq::EagerVariable {
  using Self = rq::PostEagerVariable;

  llvm::Value *_llvm_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE PostEagerVariable();

  RQ_ALWAYS_INLINE void setLlvmValue(llvm::Value &llvm_value);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *getLlvmValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *getLlvmValuePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct PreEagerVariable : public rq::EagerVariable {
  using Self = rq::PreEagerVariable;

  rq::Value *_value_ptr{};

  explicit RQ_ALWAYS_INLINE PreEagerVariable();

  RQ_ALWAYS_INLINE void setValue(rq::Value &value);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value *getValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value *getValuePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = rq::SymbolTable &;
  using pointer = rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTable *_symbol_table_ptr{nullptr};

  explicit SymbolTableIterator() = default;
  explicit RQ_ALWAYS_INLINE
  SymbolTableIterator(rq::SymbolTable *symbol_table_ptr);
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &operator*();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *operator->();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = const rq::SymbolTable;
  using reference = const rq::SymbolTable &;
  using pointer = const rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTable *_symbol_table_ptr{nullptr};

  explicit ConstSymbolTableIterator() = default;
  explicit RQ_ALWAYS_INLINE
  ConstSymbolTableIterator(const rq::SymbolTable *symbol_table_ptr);
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const;
};

struct SymbolTable : public rq::TableMember {
  using Self = rq::SymbolTable;

  llvm::DenseMap<rq::Name, rq::NodeList<rq::TableMember>> _member_map{};
  rq::SymbolTable *_conatiner_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setContainer(rq::SymbolTable &container);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getContainerPtr() const;
  inline void addMember(rq::BumpPtrAllocator &allocator, rq::Name name,
                        rq::TableMember &member);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstNodeListRef<rq::TableMember>
  lookupList(rq::Name name) const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::SymbolTableIterator>
  getInclusiveAscendingSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
  getInclusiveAscendingSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
  getConstInclusiveAscendingSubrange() const;

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

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  RQ_ALWAYS_INLINE void setOrChangeExpression(rq::Expression *expression_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &getTokens();
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
  getTokens() const;
};

struct Module final : public rq::SymbolTable {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  rq::Expression *_expression_ptr;
  llvm::StringRef _path;
  llvm::StringRef _buffer;
  rq::Import *_first_import_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleDetail &&detail);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();
  RQ_ALWAYS_INLINE void addImport(rq::Import &import);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Import>
  getImportSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
  getImportSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
  getConstImportSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CTable final : public rq::SymbolTable {
  using Self = rq::CTable;

  explicit RQ_ALWAYS_INLINE CTable();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EagerScope : public rq::SymbolTable {
  using Self = rq::EagerScope;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};

  explicit RQ_ALWAYS_INLINE EagerScope(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *getExpressionPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *getExpressionPtr() const;
  RQ_ALWAYS_INLINE void
  setModifierFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ScopeStatement final : public rq::EagerScope {
  using Self = rq::ScopeStatement;

  explicit RQ_ALWAYS_INLINE ScopeStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WhileStatement final : public rq::EagerScope {
  using Self = rq::WhileStatement;

  explicit RQ_ALWAYS_INLINE WhileStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ForStatement final : public rq::EagerScope {
  using Self = rq::ForStatement;

  explicit RQ_ALWAYS_INLINE ForStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct DefaultStatement final : public rq::EagerScope {
  using Self = rq::DefaultStatement;

  explicit RQ_ALWAYS_INLINE DefaultStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct CaseStatement final : public rq::EagerScope {
  using Self = rq::CaseStatement;

  explicit RQ_ALWAYS_INLINE CaseStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct SwitchStatement final : public rq::EagerScope {
  using Self = rq::SwitchStatement;

  explicit RQ_ALWAYS_INLINE SwitchStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct IfStatement final : public rq::EagerScope {
  using Self = rq::IfStatement;

  explicit RQ_ALWAYS_INLINE IfStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ElseIfStatement final : public rq::EagerScope {
  using Self = rq::ElseIfStatement;

  explicit RQ_ALWAYS_INLINE ElseIfStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ElseStatement final : public rq::EagerScope {
  using Self = rq::ElseStatement;

  explicit RQ_ALWAYS_INLINE ElseStatement();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NamedTable : public rq::SymbolTable {
  using Self = rq::NamedTable;

  rq::Name _name;

  explicit RQ_ALWAYS_INLINE NamedTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Namespace final : public rq::NamedTable {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyDeclaration : public rq::NamedTable {
  using Self = rq::LazyDeclaration;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::SymbolTable *_host_ptr{nullptr};
  llvm::StringRef _mangled_name{};
  rq::Module *_module_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LazyDeclaration(rq::SymbolKind kind);

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
  void setMangledName(llvm::StringRef mangled_name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;
  RQ_ALWAYS_INLINE void setModule(rq::Module &module);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *getModulePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module *getModulePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Implementation : public rq::LazyDeclaration {
  using Self = rq::Implementation;

  rq::Symbol *_parent_ptr{nullptr};
  rq::Implementation *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Implementation(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *getParentPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *getParentPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumImplementation : public rq::Implementation {
  using Self = rq::EnumImplementation;

  rq::ConstantSymbol *_underlying_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setUnderlyingType(rq::ConstantSymbol *underlying_type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getUnderlyingTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
  getUnderlyingTypePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumSpecialization final : public rq::EnumImplementation {
  using Self = rq::EnumSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumSpecialization();

  RQ_ALWAYS_INLINE void setParentEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *getParentEnumTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
  getParentEnumTemplatePtr() const;
  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyVariableImplementation : public rq::Implementation {
  using Self = rq::LazyVariableImplementation;

  rq::ConstantSymbol *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LazyVariableImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setType(rq::ConstantSymbol &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *getTypePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyVariableOverload : public rq::LazyVariableImplementation {
  using Self = rq::LazyVariableOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE LazyVariableOverload();

  RQ_ALWAYS_INLINE void
  setParentLazyVariablePolymorph(rq::LazyVariablePolymorph &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariablePolymorph *getParentPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariablePolymorph *
  getParentPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyVariableSpecialization : public rq::LazyVariableImplementation {
  using Self = rq::LazyVariableSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LazyVariableSpecialization();

  RQ_ALWAYS_INLINE void
  setParentLazyVariableTemplate(rq::LazyVariableTemplate &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableTemplate *
  getParentLazyVariableTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariableTemplate *
  getParentLazyVariableTemplatePtr() const;
  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterImplementation : public rq::Implementation {
  using Self = rq::AdapterImplementation;

  rq::Realization *_realization_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setRealization(rq::Realization &realization);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Realization *getRealizationPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Realization *
  getRealizationPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterOverload final : public rq::AdapterImplementation {
  using Self = rq::AdapterOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE AdapterOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterSpecialization final : public rq::AdapterImplementation {
  using Self = rq::AdapterSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterSpecialization();

  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceImplementation : public rq::Implementation {
  using Self = rq::InterfaceImplementation;

  explicit RQ_ALWAYS_INLINE InterfaceImplementation(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceOverload final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE InterfaceOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceSpecialization final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE InterfaceSpecialization();

  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassImplementation : public rq::Implementation {
  using Self = rq::ClassImplementation;

  rq::ConstantSymbol *_best_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setBestLayout(rq::ConstantSymbol &best);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *getBestLayoutPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
  getBestLayoutPtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassOverload final : public rq::ClassImplementation {
  using Self = rq::ClassOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ClassOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;
  RQ_ALWAYS_INLINE void changeBestLayout(rq::ConstantSymbol &best);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassSpecialization final : public rq::ClassImplementation {
  using Self = rq::ClassSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassSpecialization();

  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedureImplementation : public rq::Implementation {
  using Self = rq::ProcedureImplementation;

  rq::SignatureType *_signature_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcedureImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setSignatureType(rq::SignatureType &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureType *getSignatureTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureType *
  getSignatureTypePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedureOverload final : public rq::ProcedureImplementation {
  using Self = rq::ProcedureOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ProcedureOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedureSpecialization final : public rq::ProcedureImplementation {
  using Self = rq::ProcedureSpecialization;

  rq::ConstantArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcedureSpecialization();

  RQ_ALWAYS_INLINE void setTemplateTuple(rq::ConstantArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantArray *getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Template : public rq::LazyDeclaration {
  using Self = rq::Template;

  rq::Template *_next_ptr{nullptr};
  rq::WeightLevel *_first_weight_level_ptr{nullptr};
  rq::LayoutType *_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Template(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLayout(rq::LayoutType &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutType *getLayoutPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutType *getLayoutPtr() const;
  [[nodiscard]] rq::NextSubrange<rq::WeightLevel> getWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel>
  getWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel>
  getConstWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumTemplate final : public rq::Template {
  using Self = rq::EnumTemplate;

  explicit RQ_ALWAYS_INLINE EnumTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
  getEnumWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
  getEnumWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
  getConstEnumWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
  getClassWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
  getClassWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
  getConstClassWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
  getInterfaceWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
  getInterfaceWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
  getConstInterfaceWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct LazyVariableTemplate final : public rq::Template {
  using Self = rq::LazyVariableTemplate;

  explicit RQ_ALWAYS_INLINE LazyVariableTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
  getLazyVariableWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel,
                                      rq::LazyVariableWeightLevel>
  getLazyVariableWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel,
                                      rq::LazyVariableWeightLevel>
  getConstLazyVariableWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct AdapterTemplate final : public rq::Template {
  using Self = rq::AdapterTemplate;

  explicit RQ_ALWAYS_INLINE AdapterTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
  getAdapterWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
  getAdapterWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
  getConstAdapterWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ProcedureTemplate final : public rq::Template {
  using Self = rq::ProcedureTemplate;

  explicit RQ_ALWAYS_INLINE ProcedureTemplate();

  [[nodiscard]] rq::NextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
  getProcedureWeightLevelSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
  getProcedureWeightLevelSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
  getConstProcedureWeightLevelSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>