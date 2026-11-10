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

struct CfgBlock;
struct Instruction;
struct BumpPtrAllocator;
struct ConstantInt;
struct ConstantDetachedArray;
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
struct SimpleBranch;
template <typename ParentParam, rq::SymbolKind KIND_PARAM> struct SimpleLeaf;

// clang-format off
struct Symbol;
  using LiteralType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::LITERAL>;
    using IntegerLiteralType = rq::SimpleLeaf<rq::LiteralType, rq::SymbolKind::INTEGER_LITERAL_TYPE>;
    using FloatLiteralType = rq::SimpleLeaf<rq::LiteralType, rq::SymbolKind::FLOAT_LITERAL_TYPE>;
    using StringLiteralType = rq::SimpleLeaf<rq::LiteralType, rq::SymbolKind::STRING_LITERAL_TYPE>;
    using CodeunitLiteralType = rq::SimpleLeaf<rq::LiteralType, rq::SymbolKind::CODEUNIT_LITERAL_TYPE>;
  using FigurativeType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::FIGURATIVE>;
    using UnknownType = rq::SimpleLeaf<rq::FigurativeType, rq::SymbolKind::UNKNOWN_TYPE>;
    using InferenceType = rq::SimpleLeaf<rq::FigurativeType, rq::SymbolKind::INFERENCE_TYPE>;
    using VoidType = rq::SimpleLeaf<rq::FigurativeType, rq::SymbolKind::VOID_TYPE>;
    using NoReturnType = rq::SimpleLeaf<rq::FigurativeType, rq::SymbolKind::NO_RETURN_TYPE>;
  using ReflectionType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::REFLECTION_TYPE>;
    using ExpressionType = rq::SimpleLeaf<rq::ReflectionType, rq::SymbolKind::EXPRESSION_TYPE>;
    using ExpressionRangeType = rq::SimpleLeaf<rq::ReflectionType, rq::SymbolKind::EXPRESSION_RANGE_TYPE>;
    using SymbolRangeType = rq::SimpleLeaf<rq::ReflectionType, rq::SymbolKind::SYMBOL_RANGE_TYPE>;
  using FittingPrimitiveType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE>;
    using FittingFSIntType = rq::SimpleLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_FSINT_TYPE>;
    using FittingFUIntType = rq::SimpleLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_FUINT_TYPE>;
    using FittingLSIntType = rq::SimpleLeaf<rq::FittingPrimitiveType, rq::SymbolKind::FITTING_LUINT_TYPE>;
    using SymbolRangeType = rq::SimpleLeaf<rq::ReflectionType, rq::SymbolKind::SYMBOL_RANGE_TYPE>;
  using PlatformPrimitiveType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE>;
    using BooleanType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::BOOLEAN_TYPE>;
    using HalfType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::HALF_TYPE>;
    using SingleType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINGLE_TYPE>;
    using DoubleType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::DOUBLE_TYPE>;
    using QuadrupleType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::QUADRUPLE_TYPE>;
    using SIntType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINT_TYPE>;
    using UIntType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::UINT_TYPE>;
    using SSizeType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SSIZE_TYPE>;
    using USizeType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::USIZE_TYPE>;
    using SIndexType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::SINDEX_TYPE>;
    using CharType = rq::SimpleLeaf<rq::PlatformPrimitiveType, rq::SymbolKind::CHAR_TYPE>;
  using StandardPrimitiveType = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE>;
    using Binary16Type = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY16_TYPE>;
    using Binary32Type = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY32_TYPE>;
    using Binary64Type = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BINARY128_TYPE>;
    using BFloat16Type = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BFLOAT16_TYPE>;
    using AsciiType = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ASCII_TYPE>;
    using Utf8Type = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::UTF8_TYPE>;
  struct ScaledPrimitiveType;
    using ScaledSIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_SINT_TYPE>;
    using ScaledUIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_UINT_TYPE>;
    using ScaledFSIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_FSINT_TYPE>;
    using ScaledFUIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_FUINT_TYPE>;
    using ScaledLSIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_LSINT_TYPE>;
    using ScaledLUIntType = rq::SimpleLeaf<rq::ScaledPrimitiveType, rq::SymbolKind::SCALED_LUINT_TYPE>;
  using ModifierValue = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::MODIFIER>;
    using NoModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::NO_MODIFIER>;
    using AnchorModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ANCHOR_MODIFIER>;
    using ResidentModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RESIDENT_MODIFIER>;
    using FlankModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::FLANK_MODIFIER>;
    using TransparentModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::TRANSPARENT_MODIFIER>;
    using OpaqueModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::OPAQUE_MODIFIER>;
    using PrivateModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRIVATE_MODIFIER>;
    using PublicModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PUBLIC_MODIFIER>;
    using ExportModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EXPORT_MODIFIER>;
    using MutableModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MUTABLE_MODIFIER>;
    using PartiallyMutableModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PARTIALLY_MUTABLE_MODIFIER>;
    using ConstantModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONSTANT_MODIFIER>;
    using EagerModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EAGER_MODIFIER>;
    using LazyModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LAZY_MODIFIER>;
    using PostModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::POST_MODIFIER>;
    using PreModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRE_MODIFIER>;
    using AliasModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ALIAS_MODIFIER>;
    using EnumeratorModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ENUMERATOR_MODIFIER>;
    using ConceptModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONCEPT_MODIFIER>;
    using SplitModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SPLIT_MODIFIER>;
    using RuntimeModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RUNTIME_MODIFIER>;
    using ComtimeModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::COMTIME_MODIFIER>;
    using AnytimeModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ANYTIME_MODIFIER>;
    using PresetModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PRESET_MODIFIER>;
    using SingletonModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SINGLETON_MODIFIER>;
    using CaptureModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CAPTURE_MODIFIER>;
    using StatelessModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::STATELESS_MODIFIER>;
    using LinkedModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LINKED_MODIFIER>;
    using InlineModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::INLINE_MODIFIER>;
    using PureModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PURE_MODIFIER>;
    using ImpureModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::IMPURE_MODIFIER>;
    using StandardMangleModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::STANDARD_MANGLE_MODIFIER>;
    using MangleModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MANGLE_MODIFIER>;
    using PadModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PAD_MODIFIER>;
    using PackModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::PACK_MODIFIER>;
    using EquivocalModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EQUIVOCAL_MODIFIER>;
    using LikelyModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LIKELY_MODIFIER>;
    using UnlikelyModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::UNLIKELY_MODIFIER>;
    using SupportedModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::SUPPORTED_MODIFIER>;
    using DepreciatedModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::DEPRECIATED_MODIFIER>;
    using ExperimentalModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::EXPERIMENTAL_MODIFIER>;
    using MovableModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MOVABLE_MODIFIER>;
    using CopyableModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::COPYABLE_MODIFIER>;
    using InvariadicModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::INVARIADIC_MODIFIER>;
    using VariadicModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::VARIADIC_MODIFIER>;
    using BestLocationModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::BEST_LOCATION_MODIFIER>;
    using LocationModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::LOCATION_MODIFIER>;
    using TemplateModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::TEMPLATE_MODIFIER>;
    using OverloadModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::OVERLOAD_MODIFIER>;
    using ConstraintModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::CONSTRAINT_MODIFIER>;
    using ManualModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::MANUAL_MODIFIER>;
    using AutoModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::AUTO_MODIFIER>;
    using DirectModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::DIRECT_MODIFIER>;
    using VirualModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::VIRTUAL_MODIFIER>;
    using RangerModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::RANGER_MODIFIER>;
    using RequireModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::REQUIRE_MODIFIER>;
    using EnsureModifier = rq::SimpleLeaf<rq::StandardPrimitiveType, rq::SymbolKind::ENSURE_MODIFIER>;
  using QualifierValue = rq::SimpleBranch<rq::Symbol, rq::SymbolInfoFlags::QUALIFIER>;
    using NoMutQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_MUT_QUALIFIER>;
    using PartialMutQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::PARTIAL_MUT_QUALIFIER>;
    using MutQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::MUT_QUALIFIER>;
    using NoVolatileQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_VOLATILE_QUALIFIER>;
    using VolatileQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::VOLATILE_QUALIFIER>;
    using NoAtomicQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_ATOMIC_QUALIFIER>;
    using AtomicQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::ATOMIC_QUALIFIER>;
    using NoNullTerminateQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_NULL_TERMINATE_QUALIFIER>;
    using NullTerminateQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NULL_TERMINATE_QUALIFIER>;
    using NoMarginQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_MARGIN_QUALIFIER>;
    using MarginQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::MARGIN_QUALIFIER>;
    using NoThrowsQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::NO_THROWS_QUALIFIER>;
    using ThrowsQualifier = rq::SimpleLeaf<rq::Qualifier, rq::SymbolKind::THROWS_QUALIFIER>;
  struct BuiltinConcept;
    using AnyConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ANY_CONCEPT>;
    using TypeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::TYPE_CONCEPT>;
    using ComtimeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::COMTIME_CONCEPT>;
    using RuntimeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::RUNTIME_CONCEPT>;
    using AnytimeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ANYTIME_CONCEPT>;
    using RangeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::RANGE_CONCEPT>;
    using RouteConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ROUTE_CONCEPT>;
    using FigurativeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FIGURATIVE_CONCEPT>;
    using ReflectionConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::REFLECTION_CONCEPT>;
    using PrimitiveConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PRIMITIVE_CONCEPT>;
    using FittingPrimitiveConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FITTING_PRIMITIVE_CONCEPT>;
    using PlatformPrimitiveConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PLATFORM_PRIMITIVE_CONCEPT>;
    using StandardPrimitiveConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::STANDARD_PRIMITIVE_CONCEPT>;
    using ScaledPrimitiveConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SCALED_PRIMITIVE_CONCEPT>;
    using SignedConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNED_CONCEPT>;
    using UnsignedConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::UNSIGNED_CONCEPT>;
    using IntConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INT_CONCEPT>;
    using ExactIntConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::EXACT_INT_CONCEPT>;
    using FastIntConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FAST_INT_CONCEPT>;
    using LeastIntConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LEAST_INT_CONCEPT>;
    using FloatConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FLOAT_CONCEPT>;
    using BinaryConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::BINARY_CONCEPT>;
    using BfloatConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::BFLOAT_CONCEPT>;
    using ConceptConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_CONCEPT>;
    using SubtypeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SUBTYPE_CONCEPT>;
    using ArrayConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ARRAY_CONCEPT>;
    using RefConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::REF_CONCEPT>;
    using SliceConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SLICE_CONCEPT>;
    using SplitConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SPLIT_CONCEPT>;
    using ModuleConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MODULE_CONCEPT>;
    using ImportConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::IMPORT_CONCEPT>;
    using PortalConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PORTAL_CONCEPT>;
    using RealizationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::REALIZATION_CONCEPT>;
    using ConformityConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONFORMITY_CONCEPT>;
    using AdaptionConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTION_CONCEPT>;
    using TripConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::TRIP_CONCEPT>;
    using JuxtConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::JUXT_CONCEPT>;
    using SpecializationSetConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SPECIALIZATION_SET_CONCEPT>;
    using AdapterSpecializationSetConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_SPECIALIZATION_SET_CONCEPT>;
    using ProcSpecializationSetConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PROC_SPECIALIZATION_SET_CONCEPT>;
    using ArithmeticSequenceConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ARITHMETIC_SEQUENCE_CONCEPT>;
    using ArithmeticIntervalConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ARITHMETIC_INTERVAL_CONCEPT>;
    using InfiniteARithmeticSequenceConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_CONCEPT>;
    using FiniteArithmeticSequenceConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_CONCEPT>;
    using EagerDeclarationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_DECLARATION_CONCEPT>;
    using AnchorConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ANCHOR_CONCEPT>;
    using EnumeratorConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUMERATOR_CONCEPT>;
    using VariableConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIABLE_CONCEPT>;
    using EagerVariableConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_VARIABLE_CONCEPT>;
    using PostEagerVariableConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::POST_EAGER_VARIABLE_CONCEPT>;
    using PreEagerVariableConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PRE_EAGER_VARIABLE_CONCEPT>;
    using ParameterConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PARAMETER_CONCEPT>;
    using SignatureParameterConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNATURE_PARAMETER_CONCEPT>;
    using LayoutParameterConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAYOUT_PARAMETER_CONCEPT>;
    using ParameterListConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PARAMETER_LIST_CONCEPT>;
    using SignatureConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNATURE_CONCEPT>;
    using LayoutParameterConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAYOUT_PARAMETER_CONCEPT>;
    using ParameterListConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PARAMETER_LIST_CONCEPT>;
    using SignatureConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SIGNATURE_CONCEPT>;
    using LayoutConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAYOUT_CONCEPT>;
    using PlacementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PLACEMENT_CONCEPT>;
    using CompositionConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::COMPOSITION_CONCEPT>;
    using SynonymConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SYNONYM_CONCEPT>;
    using SymbolTableConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SYMBOL_TABLE_CONCEPT>;
    using NamespaceConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::NAMESPACE_CONCEPT>;
    using EagerScopingStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::EAGER_SCOPING_STATEMENT_CONCEPT>;
    using IfStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::IF_STATEMENT_CONCEPT>;
    using ElseIfStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ELSE_IF_STATEMENT_CONCEPT>;
    using ElseStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ELSE_STATEMENT_CONCEPT>;
    using SwitchStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SWITCH_STATEMENT_CONCEPT>;
    using CaseStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CASE_STATEMENT_CONCEPT>;
    using DefaultStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::DEFAULT_STATEMENT_CONCEPT>;
    using ForStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::FOR_STATEMENT_CONCEPT>;
    using WhileStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::WHILE_STATEMENT_CONCEPT>;
    using ScopeStatementConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SCOPE_STATEMENT_CONCEPT>;
    using ImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::IMPLEMENTATION_CONCEPT>;
    using OverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::OVERLOAD_CONCEPT>;
    using SpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SPECIALIZATION_CONCEPT>;
    using TemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::TEMPLATE_CONCEPT>;
    using ClassImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_IMPLEMENTATION_CONCEPT>;
    using ClassOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_OVERLOAD_CONCEPT>;
    using ClassSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_SPECIALIZATION_CONCEPT>;
    using ClassTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CLASS_TEMPLATE_CONCEPT>;
    using EnumImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_IMPLEMENTATION_CONCEPT>;
    using EnumOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_OVERLOAD_CONCEPT>;
    using EnumSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_SPECIALIZATION_CONCEPT>;
    using EnumTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENUM_TEMPLATE_CONCEPT>;
    using InterfaceImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_IMPLEMENTATION_CONCEPT>;
    using InterfaceOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_OVERLOAD_CONCEPT>;
    using InterfaceSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_SPECIALIZATION_CONCEPT>;
    using InterfaceTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INTERFACE_TEMPLATE_CONCEPT>;
    using AdapterImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_IMPLEMENTATION_CONCEPT>;
    using AdapterOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_OVERLOAD_CONCEPT>;
    using AdapterSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_SPECIALIZATION_CONCEPT>;
    using AdapterTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADAPTER_TEMPLATE_CONCEPT>;
    using ProcImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PROC_IMPLEMENTATION_CONCEPT>;
    using ProcOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PROC_OVERLOAD_CONCEPT>;
    using ProcSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PROC_SPECIALIZATION_CONCEPT>;
    using ProcTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PROC_TEMPLATE_CONCEPT>;
    using LazyVariableImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_IMPLEMENTATION_CONCEPT>;
    using LazyVariableOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_OVERLOAD_CONCEPT>;
    using LazyVariableSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_SPECIALIZATION_CONCEPT>;
    using LazyVariableTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_VARIABLE_TEMPLATE_CONCEPT>;
    using AliasImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_IMPLEMENTATION_CONCEPT>;
    using AliasOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_OVERLOAD_CONCEPT>;
    using AliasSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_SPECIALIZATION_CONCEPT>;
    using AliasTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ALIAS_TEMPLATE_CONCEPT>;
    using ConceptImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_IMPLEMENTATION_CONCEPT>;
    using ConceptOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_OVERLOAD_CONCEPT>;
    using ConceptSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_SPECIALIZATION_CONCEPT>;
    using ConceptTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONCEPT_TEMPLATE_CONCEPT>;
    using VariantImplementationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_IMPLEMENTATION_CONCEPT>;
    using VariantOverloadConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_OVERLOAD_CONCEPT>;
    using VariantSpecializationConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_SPECIALIZATION_CONCEPT>;
    using VariantTemplateConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIANT_TEMPLATE_CONCEPT>;
    using AttributeConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ATTRIBUTE_CONCEPT>;
    using ModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MODIFIER_CONCEPT>;
    using AnchorModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ANCHOR_MODIFIER_CONCEPT>;
    using ContainerModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONTAINER_MODIFIER_CONCEPT>;
    using VisibilityModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VISIBILITY_MODIFIER_CONCEPT>;
    using AccessModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ACCESS_MODIFIER_CONCEPT>;
    using MutabilityModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MUTABILITY_MODIFIER_CONCEPT>;
    using EvaluationTimeModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::EVALUATION_TIME_MODIFIER_CONCEPT>;
    using DialectModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::DIALECT_MODIFIER_CONCEPT>;
    using TenureModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::TENURE_MODIFIER_CONCEPT>;
    using InitializationTimeModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::INITIALIZATION_TIME_MODIFIER_CONCEPT>;
    using ClosureModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CLOSURE_MODIFIER_CONCEPT>;
    using LinkageModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LINKAGE_MODIFIER_CONCEPT>;
    using SideEffectsModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SIDE_EFFECTS_MODIFIER_CONCEPT>;
    using MangleModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MANGLE_MODIFIER_CONCEPT>;
    using PackModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::PACK_MODIFIER_CONCEPT>;
    using BranchTrendModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::BRANCH_TREND_MODIFIER_CONCEPT>;
    using SupportNoticeModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::SUPPORT_NOTICE_MODIFIER_CONCEPT>;
    using AddressStabilityModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ADDRESS_STABILITY_MODIFIER_CONCEPT>;
    using VariadicModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VARIADIC_MODIFIER_CONCEPT>;
    using OffsetModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::OFFSET_MODIFIER_CONCEPT>;
    using LazyDeclarationModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::LAZY_DECLARATION_MODIFIER_CONCEPT>;
    using ConstraintModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::CONSTRAINT_MODIFIER_CONCEPT>;
    using DeductionModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::DEDUCTION_MODIFIER_CONCEPT>;
    using VirtualityModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VIRTUALITY_MODIFIER_CONCEPT>;
    using RangerModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::RANGER_MODIFIER_CONCEPT>;
    using RequireModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::REQUIRE_MODIFIER_CONCEPT>;
    using EnsureModifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ENSURE_MODIFIER_CONCEPT>;
    using QualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::QUALIFIER_CONCEPT>;
    using MutQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MUT_QUALIFIER_CONCEPT>;
    using VolatileQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::VOLATILE_QUALIFIER_CONCEPT>;
    using AtomicQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::ATOMIC_QUALIFIER_CONCEPT>;
    using NullTerminateQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::NULL_TERMINATE_QUALIFIER_CONCEPT>;
    using DetatchQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::DETATCH_QUALIFIER_CONCEPT>;
    using MarginQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::MARGIN_QUALIFIER_CONCEPT>;
    using ThrowsQualifierConcept = rq::SimpleLeaf<rq::BuiltinConcept, rq::SymbolKind::THROWS_QUALIFIER_CONCEPT>;
  struct QualifiedType;
  struct Subentity;
    struct Subtype;
      struct ArraySubtype;
      struct SimpleSubtype;
        using RefSubtype = rq::SimpleLeaf<rq::SimpleSubtype, rq::SymbolKind::REF_SUBTYPE>;
        using PtrSubtype = rq::SimpleLeaf<rq::SimpleSubtype, rq::SymbolKind::PTR_SUBTYPE>;
        using SliceSubtype = rq::SimpleLeaf<rq::SimpleSubtype, rq::SymbolKind::SLICE_SUBTYPE>;
        using SplitSubtype = rq::SimpleLeaf<rq::SimpleSubtype, rq::SymbolKind::SPLIT_SUBTYPE>;
        using InferenceCountArraySubtype = rq::SimpleLeaf<rq::SimpleSubtype, rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE>;
    struct Subsymbol;
      using GenericSubsymbol = rq::SimpleLeaf<rq::Subsymbol, rq::SymbolKind::GENERIC_SUBSYMBOL>;
  struct Adaption;
  struct Realization;
  struct Conformity;
  struct JuxtListType;
  struct Trip;
  struct SynonymType;
  struct ArithmeticSequenceType;
    using ArithmeticIntervalType = rq::SimpleLeaf<rq::ArithmeticSequenceType, rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE>;
    using InfiniteArithmeticSequenceType = rq::SimpleLeaf<rq::ArithmeticSequenceType, rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE>;
    using FiniteArithmeticSequenceType = rq::SimpleLeaf<rq::ArithmeticSequenceType, rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE>;
  struct Import;
  struct SpecializationSetArgument;
  struct SpecializationSet;
    struct ProcSpecializationSet;
    struct AdapterSpecializationSet;
  struct Parameter;
  struct CompositionType;
  struct ParameterList;
    struct SignatureType;
    struct LayoutType;
  struct PlacementType;
  struct Polymorph;
    struct ClassPolymorph;
    struct EnumPolymorph;
    struct InterfacePolymorph;
    struct AdapterPolymorph;
    struct ProcPolymorph;
    struct LazyVariablePolymorph;
    struct AliasPolymorph;
    struct VariantPolymorph;
    struct ConceptPolymorph;
  struct TableMember;
    struct Portal;
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
            struct ProcImplementation;
              struct ProcOverload;
              struct ProcSpecialization;
            struct AliasImplementation;
              struct AliasOverload;
              struct AliasSpecialization;
            struct ConceptImplementation;
              struct ConceptOverload;
              struct ConceptSpecialization;
            struct VariantImplementation;
              struct VariantOverload;
              struct VariantSpecialization;
          struct Template;
            struct EnumTemplate;
            struct ClassTemplate;
            struct InterfaceTemplate;
            struct AdapterTemplate;
            struct ProcTemplate;
            struct LazyVariableTemplate;
            struct AliasTemplate;
            struct ConceptTemplate;
            struct VariantTemplate;
// clang-format on

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit RQ_ALWAYS_INLINE Symbol(rq::SymbolKind kind);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

template <typename ParentParam, rq::SymbolInfoFlags FLAG_PARAM>
struct SimpleBranch : public ParentParam {
  using Parent = ParentParam;
  static constexpr rq::SymbolInfoFlags FLAG = FLAG_PARAM;
  using Self = rq::SimpleBranch<Parent, FLAG>;

  template <typename... ParentConstructorArgsNParam>
  explicit RQ_ALWAYS_INLINE
  SimpleBranch(rq::SymbolKind kind, ParentConstructorArgsNParam &&...n_args);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

template <typename ParentParam, rq::SymbolKind KIND_PARAM>
struct SimpleLeaf final : public ParentParam {
  using Parent = ParentParam;
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::SimpleLeaf<Parent, KIND>;

  template <typename... ParentConstructorArgsNParam>
  explicit RQ_ALWAYS_INLINE SimpleLeaf(ParentConstructorArgsNParam &&...n_args);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ScaledPrimitiveType : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledPrimitiveType;

  unsigned _depth;

  explicit RQ_ALWAYS_INLINE ScaledPrimitiveType(rq::SymbolKind kind,
                                                unsigned depth);

  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getDepth() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             rq::SymbolKind kind, unsigned depth);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id);
};

struct BuiltinConcept : public rq::Symbol {
  using Self = rq::BuiltinConcept;

  rq::Entity *_predecessor_ptr;

  explicit RQ_ALWAYS_INLINE BuiltinConcept(rq::SymbolKind kind,
                                           rq::Entity &predecessors);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getPredecessor() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getPredecessor();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct QualifiedType final : public rq::Symbol {
  using Self = rq::QualifiedType;

  rq::Symbol *_type_ptr;
  rq::QualifierFuseFlags _qualifier_fuse_flags;
  unsigned _margin;

  explicit RQ_ALWAYS_INLINE QualifiedType(rq::Symbol &type,
                                          rq::QualifierFuseFlags qualifiers,
                                          unsigned margin);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifierFuseFlags getFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getMargin() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::Symbol &type,
                             rq::QualifierFuseFlags qualifiers,
                             unsigned margin);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id);
};

struct Subentity : public rq::Symbol {
  using Self = rq::Subentity;

  rq::Entity *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subentity(rq::SymbolKind kind, rq::Entity &child);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getChild();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Subtype : public rq::Subentity {
  using Self = rq::Subentity;

  explicit RQ_ALWAYS_INLINE Subtype(rq::SymbolKind kind,
                                    rq::QualifiedType &child);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getChildType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getChildType();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ArraySubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::ArraySubtype;

  std::size_t *_count;

  explicit RQ_ALWAYS_INLINE ArraySubtype(rq::QualifiedType &child,
                                         std::size_t &count);

  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::QualifiedType &child, std::size_t count);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id);
};

struct SimpleSubtype : public rq::Subtype {
  using Self = rq::SimpleSubtype;

  explicit RQ_ALWAYS_INLINE SimpleSubtype(rq::SymbolKind kind,
                                          rq::QualifiedType &child);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::QualifiedType &child);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id);
};

struct Subsymbol : public rq::Subentity {
  using Self = rq::Subsymbol;

  explicit RQ_ALWAYS_INLINE Subsymbol(rq::SymbolKind kind, rq::Symbol &child);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getChildSymbol() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getChildSymbol();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::Symbol &child);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id);
};

struct Adaption final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Adaption;

  rq::QualifiedType *_reciever_ptr;
  rq::InterfaceImplementation *_interface_ptr;
  rq::AdapterImplementation *_adapter_ptr;

  explicit RQ_ALWAYS_INLINE Adaption(rq::QualifiedType &reciever,
                                     rq::InterfaceImplementation &interface,
                                     rq::AdapterImplementation &adapter);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getReciever() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getReciever();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
  getAdapter() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &getAdapter();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::InterfaceImplementation &interface,
                             const rq::AdapterImplementation &adapter);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct Realization final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Realization; // Reciever==>Interface

  rq::QualifiedType *_reciever_ptr;
  rq::InterfaceImplementation *_interface_ptr;

  explicit RQ_ALWAYS_INLINE Realization(rq::QualifiedType &reciever,
                                        rq::InterfaceImplementation &interface);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getReciever() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getReciever();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
  getInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &getInterface();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &inout_id,
                      const rq::QualifiedType &reciever,
                      const rq::InterfaceImplementation &interface);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::InterfaceImplementation &interface,
                             const rq::AdapterImplementation &adapter);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct JuxtListType final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::JuxtListType;

  rq::NodeList<rq::QualifiedType> _type_list;

  explicit RQ_ALWAYS_INLINE
  JuxtListType(rq::NodeList<rq::QualifiedType> &&type_list);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListSubrange<rq::QualifiedType>
  getItemSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::QualifiedType>
  getItemSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::QualifiedType>
  getConstItemSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             rq::ConstNodeListRef<rq::QualifiedType> type_list);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct Trip final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::Trip;

  rq::NodeList<rq::Entity> _entity_list;

  explicit RQ_ALWAYS_INLINE Trip(rq::NodeList<rq::Entity> &&entity_list);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListSubrange<rq::Entity>
  getItemSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::Entity>
  getItemSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListSubrange<rq::Entity>
  getConstItemSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             rq::ConstNodeListRef<rq::Entity> entity_list);
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct SynonymType final : public rq::Symbol {
  using Self = rq::SynonymType;

  rq::Symbol *_original_ptr;

  explicit RQ_ALWAYS_INLINE SynonymType(rq::Symbol &original);

  [[nodiscard]] const rq::Symbol &getOriginal() const;
  [[nodiscard]] rq::Symbol &getOriginal();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ArithmeticSequenceType : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceType;

  rq::QualifiedType *_child_ptr;
  rq::ArithmeticCondition _condition;
  rq::ArithmeticStep _step;

  explicit RQ_ALWAYS_INLINE
  ArithmeticSequenceType(rq::SymbolKind kind, rq::QualifiedType &child,
                         rq::ArithmeticCondition condition,
                         rq::ArithmeticStep step);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticCondition getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticStep getStep() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::QualifiedType &child,
                             rq::ArithmeticCondition condition,
                             rq::ArithmeticStep step);
  inline void Profile(llvm::FoldingSetNodeID &inout_id) const;
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id, rq::Name name,
                             const rq::Entity &value,
                             rq::SpecializationSetArgument *next_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             rq::SymbolKind kind,
                             const rq::SpecializationSetArgument *first_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct ProcSpecializationSet final : public rq::SpecializationSet {
  using Self = rq::ProcSpecializationSet;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AdapterSpecializationSet final : public rq::SpecializationSet {
  using Self = rq::AdapterSpecializationSet;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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
  rq::QualifiedType *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::Entity *_default_ptr;

  explicit RQ_ALWAYS_INLINE
  ParameterDetail(rq::Name name, rq::QualifiedType &type,
                  rq::ModifierFuseFlags modifier_fuse_flags,
                  rq::ParameterInfoFlags param_info_flags,
                  rq::Entity *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getType();
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
  rq::QualifiedType *_type_ptr;
  rq::ModifierFuseFlags _modifier_fuse_flags;
  rq::ParameterInfoFlags _param_info_flags;
  rq::Entity *_default_ptr;

  explicit RQ_ALWAYS_INLINE Parameter(rq::Parameter *next_ptr, rq::Name name,
                                      rq::QualifiedType &type,
                                      rq::ModifierFuseFlags modifier_flags,
                                      rq::ParameterInfoFlags param_flags,
                                      rq::Entity *default_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
  getParameterFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getDefaultPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getDefaultPtr();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::Parameter *next_ptr, rq::Name name,
                             const rq::QualifiedType &type,
                             rq::ModifierFuseFlags modifier_fuse_flags,
                             rq::ParameterInfoFlags param_flags,
                             const rq::Entity *default_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct CompositionType final : public rq::Symbol {
  using Self = rq::CompositionType;

  rq::NodeList<rq::InterfaceImplementation> _interface_list;

  explicit RQ_ALWAYS_INLINE
  CompositionType(rq::NodeList<rq::InterfaceImplementation> &&interface_list);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void
  Profile(llvm::FoldingSetNodeID &inout_id,
          rq::ConstNodeListRef<rq::InterfaceImplementation> interface_list);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

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
  appendParameter(rq::Name name, rq::QualifiedType &type,
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct SignatureType final : public rq::ParameterList,
                             public llvm::FoldingSetNode {
  using Self = rq::SignatureType;

  rq::QualifiedType *_return_type_ptr;

  explicit RQ_ALWAYS_INLINE SignatureType(rq::Parameter *first_parameter_ptr,
                                          rq::QualifiedType &return_type);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType &getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType &getReturnType();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                      const rq::Parameter *first_parameter_ptr,
                      const rq::QualifiedType &return_type);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct LayoutType final : public rq::ParameterList {
  using Self = rq::LayoutType;

  explicit RQ_ALWAYS_INLINE LayoutType(rq::Parameter *first_parameter_ptr);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::Parameter *first_parameter_ptr);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct PlacementType final : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::ProcImplementation *_proc_ptr;

  explicit RQ_ALWAYS_INLINE PlacementType(rq::ProcImplementation &function);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcImplementation &getProc() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcImplementation &getProc();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const rq::ProcImplementation &function);

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const;
};

struct Polymorph : public rq::Symbol {
  using Self = rq::Polymorph;

  rq::Implementation *_first_overload_ptr{nullptr};
  rq::Template *_first_template_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Implementation>
  getOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
  getOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
  getConstOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template>
  getTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
  getTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
  getConstTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

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
  RQ_ALWAYS_INLINE void addClassTemplate(rq::ClassTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
      getClassTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
      getConstClassTemplateSubrang() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct EnumPolymorph final : public rq::Polymorph {
  using Self = rq::EnumPolymorph;

  explicit RQ_ALWAYS_INLINE EnumPolymorph();

  RQ_ALWAYS_INLINE void addEnumOverload(rq::EnumOverload &enum_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getEnumOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
      getConstEnumOverloadSubrange() const;
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ProcPolymorph final : public rq::Polymorph {
  using Self = rq::ProcPolymorph;

  explicit RQ_ALWAYS_INLINE ProcPolymorph();

  RQ_ALWAYS_INLINE void addProcOverload(rq::ProcOverload &function);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::ProcOverload>
      getProcOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>
      getProcOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ProcOverload>
      getConstProcOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addProcTemplate(rq::ProcTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::ProcTemplate>
      getProcTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>
      getProcTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ProcTemplate>
      getConstProcTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AliasPolymorph final : public rq::Polymorph {
  using Self = rq::AliasPolymorph;

  explicit RQ_ALWAYS_INLINE AliasPolymorph();

  RQ_ALWAYS_INLINE void addAliasOverload(rq::AliasOverload &alias);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::AliasOverload>
      getAliasOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::AliasOverload>
      getAliasOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::AliasOverload>
      getConstAliasOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addAliasTemplate(rq::AliasTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::AliasTemplate>
      getAliasTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::AliasTemplate>
      getAliasTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::AliasTemplate>
      getConstAliasTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct VariantPolymorph final : public rq::Polymorph {
  using Self = rq::VariantPolymorph;

  explicit RQ_ALWAYS_INLINE VariantPolymorph();

  RQ_ALWAYS_INLINE void addConceptOverload(rq::VariantOverload &concept_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::VariantOverload>
      getVariantOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::VariantOverload>
      getVariantOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::VariantOverload>
      getConstVariantOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addVariantTemplate(rq::VariantTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::VariantTemplate>
      getVariantTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::VariantTemplate>
      getVariantTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::VariantTemplate>
      getConstVariantTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ConceptPolymorph final : public rq::Polymorph {
  using Self = rq::ConceptPolymorph;

  explicit RQ_ALWAYS_INLINE ConceptPolymorph();

  RQ_ALWAYS_INLINE void addConceptOverload(rq::ConceptOverload &concept_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Implementation, rq::ConceptOverload>
      getConceptOverloadSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ConceptOverload>
      getConceptOverloadSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Implementation, rq::ConceptOverload>
      getConstConceptOverloadSubrange() const;
  RQ_ALWAYS_INLINE void addConceptTemplate(rq::ConceptTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::NextSubrange<rq::Template, rq::ConceptTemplate>
      getConceptTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ConceptTemplate>
      getConceptTemplateSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      rq::ConstNextSubrange<rq::Template, rq::ConceptTemplate>
      getConceptConceptTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct TableMember : public rq::Symbol {
  using Self = rq::TableMember;

  rq::SymbolTable *_container_ptr{nullptr};
  rq::Module *_module_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE TableMember(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setContainer(rq::SymbolTable &container);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainerPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *getContainerPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Portal final : public rq::TableMember {
  using Self = rq::Portal;

  rq::SymbolTable *_dest_ptr{nullptr};
  rq::NodeList<rq::SymbolTable> _without_list{};
  rq::Expression *_expression_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Portal();

  RQ_ALWAYS_INLINE void setDest(rq::SymbolTable &dest);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getDest() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getDest();
  RQ_ALWAYS_INLINE void
  setWithoutList(rq::NodeList<rq::SymbolTable> &&without_list);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::SymbolTable>
  getWithoutList() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListRef<rq::SymbolTable>
  getWithoutList();
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Anchor final : public rq::EagerDeclaration {
  using Self = rq::Anchor;

  rq::EagerScope *_vessel_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Anchor();

  RQ_ALWAYS_INLINE void setVessel(rq::EagerScope &scope);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScope *getVesselPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EagerScope *getVesselPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Enumerator final : public rq::EagerDeclaration {
  using Self = rq::Enumerator;

  rq::ConstantInt *_word_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Enumerator();

  RQ_ALWAYS_INLINE void setWord(rq::ConstantInt &word);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantInt *getWordPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantInt *getWordPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct EagerVariable : public rq::EagerDeclaration {
  using Self = rq::EagerVariable;

  rq::ModifierFuseFlags _modifier_fuse_flags{rq::ModifierFuseFlags::NONE};
  rq::QualifiedType *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EagerVariable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void
  setLowFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
  getModifierFuseFlags() const;
  RQ_ALWAYS_INLINE void setType(rq::QualifiedType &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *getTypePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct PostEagerVariable : public rq::EagerVariable {
  using Self = rq::PostEagerVariable;

  llvm::Value *_llvm_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE PostEagerVariable();

  RQ_ALWAYS_INLINE void setLlvmValue(llvm::Value &llvm_value);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *getLlvmValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *getLlvmValuePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct PreEagerVariable : public rq::EagerVariable {
  using Self = rq::PreEagerVariable;

  rq::Value *_value_ptr{};

  explicit RQ_ALWAYS_INLINE PreEagerVariable();

  RQ_ALWAYS_INLINE void setValue(rq::Value &value);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Value *getValuePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Value *getValuePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct CTable final : public rq::SymbolTable {
  using Self = rq::CTable;

  explicit RQ_ALWAYS_INLINE CTable();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ScopeStatement final : public rq::EagerScope {
  using Self = rq::ScopeStatement;

  explicit RQ_ALWAYS_INLINE ScopeStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct WhileStatement final : public rq::EagerScope {
  using Self = rq::WhileStatement;

  explicit RQ_ALWAYS_INLINE WhileStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ForStatement final : public rq::EagerScope {
  using Self = rq::ForStatement;

  explicit RQ_ALWAYS_INLINE ForStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct DefaultStatement final : public rq::EagerScope {
  using Self = rq::DefaultStatement;

  explicit RQ_ALWAYS_INLINE DefaultStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct CaseStatement final : public rq::EagerScope {
  using Self = rq::CaseStatement;

  explicit RQ_ALWAYS_INLINE CaseStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct SwitchStatement final : public rq::EagerScope {
  using Self = rq::SwitchStatement;

  explicit RQ_ALWAYS_INLINE SwitchStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct IfStatement final : public rq::EagerScope {
  using Self = rq::IfStatement;

  explicit RQ_ALWAYS_INLINE IfStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ElseIfStatement final : public rq::EagerScope {
  using Self = rq::ElseIfStatement;

  explicit RQ_ALWAYS_INLINE ElseIfStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ElseStatement final : public rq::EagerScope {
  using Self = rq::ElseStatement;

  explicit RQ_ALWAYS_INLINE ElseStatement();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct NamedTable : public rq::SymbolTable {
  using Self = rq::NamedTable;

  rq::Name _name;

  explicit RQ_ALWAYS_INLINE NamedTable(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setName(rq::Name name);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Name getName() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Namespace final : public rq::NamedTable {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace();

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Implementation : public rq::LazyDeclaration {
  using Self = rq::Implementation;

  rq::Symbol *_parent_ptr{nullptr};
  rq::Implementation *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Implementation(rq::SymbolKind kind);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *getParentPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *getParentPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct EnumImplementation : public rq::Implementation {
  using Self = rq::EnumImplementation;

  rq::QualifiedType *_underlying_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setUnderlyingType(rq::QualifiedType *underlying_type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *getUnderlyingTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
  getUnderlyingTypePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct EnumSpecialization final : public rq::EnumImplementation {
  using Self = rq::EnumSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumSpecialization();

  RQ_ALWAYS_INLINE void setParentEnumTemplate(rq::EnumTemplate &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumTemplate *getParentEnumTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumTemplate *
  getParentEnumTemplatePtr() const;
  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct LazyVariableImplementation : public rq::Implementation {
  using Self = rq::LazyVariableImplementation;

  rq::QualifiedType *_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LazyVariableImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setType(rq::QualifiedType &type);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *getTypePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
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

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct LazyVariableSpecialization : public rq::LazyVariableImplementation {
  using Self = rq::LazyVariableSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE LazyVariableSpecialization();

  RQ_ALWAYS_INLINE void
  setParentLazyVariableTemplate(rq::LazyVariableTemplate &parent);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariableTemplate *
  getParentLazyVariableTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariableTemplate *
  getParentLazyVariableTemplatePtr() const;
  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AdapterImplementation : public rq::Implementation {
  using Self = rq::AdapterImplementation;

  rq::Realization *_realization_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setRealization(rq::Realization &realization);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Realization *getRealizationPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Realization *
  getRealizationPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AdapterOverload final : public rq::AdapterImplementation {
  using Self = rq::AdapterOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE AdapterOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AdapterSpecialization final : public rq::AdapterImplementation {
  using Self = rq::AdapterSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE AdapterSpecialization();

  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct InterfaceImplementation : public rq::Implementation {
  using Self = rq::InterfaceImplementation;

  explicit RQ_ALWAYS_INLINE InterfaceImplementation(rq::SymbolKind kind);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct InterfaceOverload final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE InterfaceOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct InterfaceSpecialization final : public rq::InterfaceImplementation {
  using Self = rq::InterfaceSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE InterfaceSpecialization();

  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ClassImplementation : public rq::Implementation {
  using Self = rq::ClassImplementation;

  rq::QualifiedType *_best_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setBestLayout(rq::QualifiedType &best);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::QualifiedType *getBestLayoutPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::QualifiedType *
  getBestLayoutPtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ClassOverload final : public rq::ClassImplementation {
  using Self = rq::ClassOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ClassOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;
  RQ_ALWAYS_INLINE void changeBestLayout(rq::QualifiedType &best);

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ClassSpecialization final : public rq::ClassImplementation {
  using Self = rq::ClassSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ClassSpecialization();

  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ProcImplementation : public rq::Implementation {
  using Self = rq::ProcImplementation;

  rq::SignatureType *_signature_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcImplementation(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setSignatureType(rq::SignatureType &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureType *getSignatureTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureType *
  getSignatureTypePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ProcOverload final : public rq::ProcImplementation {
  using Self = rq::ProcOverload;

  rq::NodeList<rq::Expression *> _prototype_expression_ptr_list{};

  explicit RQ_ALWAYS_INLINE ProcOverload();

  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<rq::Expression *> &
  getPrototypeExpressionPtrList();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListRef<rq::Expression *>
  getPrototypeExpressionPtrList() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ProcSpecialization final : public rq::ProcImplementation {
  using Self = rq::ProcSpecialization;

  rq::ConstantDetachedArray *_template_tuple_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ProcSpecialization();

  RQ_ALWAYS_INLINE void
  setTemplateTuple(rq::ConstantDetachedArray &template_tuple);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantDetachedArray *
  getTemplateTuplePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantDetachedArray *
  getTemplateTuplePtr() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct Template : public rq::LazyDeclaration {
  using Self = rq::Template;

  rq::Template *_next_ptr{nullptr};
  rq::Implementation *_first_specialization_ptr{nullptr};
  rq::LayoutType *_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Template(rq::SymbolKind kind);

  RQ_ALWAYS_INLINE void setLayout(rq::LayoutType &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutType *getLayoutPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutType *getLayoutPtr() const;
  [[nodiscard]] rq::NextSubrange<rq::Implementation>
  getSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation>
  getSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation>
  getConstSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct EnumTemplate final : public rq::Template {
  using Self = rq::EnumTemplate;

  explicit RQ_ALWAYS_INLINE EnumTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation, rq::EnumSpecialization>
  getEnumSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::EnumSpecialization>
  getEnumSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::EnumSpecialization>
  getConstEnumSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation, rq::ClassSpecialization>
  getClassSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::ClassSpecialization>
  getClassSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::ClassSpecialization>
  getConstClassSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation,
                                 rq::InterfaceSpecialization>
  getInterfaceSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::InterfaceSpecialization>
  getInterfaceSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::InterfaceSpecialization>
  getConstInterfaceSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct LazyVariableTemplate final : public rq::Template {
  using Self = rq::LazyVariableTemplate;

  explicit RQ_ALWAYS_INLINE LazyVariableTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation,
                                 rq::LazyVariableSpecialization>
  getLazyVariableTemplateSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::LazyVariableSpecialization>
  getLazyVariableTemplateSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::LazyVariableSpecialization>
  getConstLazyVariableTemplateSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct AdapterTemplate final : public rq::Template {
  using Self = rq::AdapterTemplate;

  explicit RQ_ALWAYS_INLINE AdapterTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation, rq::AdapterSpecialization>
  getAdapterSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::AdapterSpecialization>
  getAdapterSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::AdapterSpecialization>
  getConstAdapterSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

struct ProcTemplate final : public rq::Template {
  using Self = rq::ProcTemplate;

  explicit RQ_ALWAYS_INLINE ProcTemplate();

  [[nodiscard]] rq::NextSubrange<rq::Implementation, rq::ProcSpecialization>
  getProcSpecializationSubrange();
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::ProcSpecialization>
  getProcSpecializationSubrange() const;
  [[nodiscard]] rq::ConstNextSubrange<rq::Implementation,
                                      rq::ProcSpecialization>
  getConstProcSpecializationSubrange() const;

  [[nodiscard]] static RQ_ALWAYS_INLINE bool
  classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>