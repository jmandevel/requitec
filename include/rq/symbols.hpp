#pragma once

#include <rq/bump_ptr_list.hpp>
#include <rq/entity.hpp>
#include <rq/expressions.hpp>
#include <rq/next_iterator.hpp>
#include <rq/see.hpp>
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

#include <ranges>
#include <vector>

namespace rq {

struct SymbolConstant;
struct Instruction;

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind opcode);

enum class SymbolFlags : std::uint64_t {
  NONE = 0,

  // SYMBOL CLASSIFICATION
  SIMPLE_SYMBOL = rq::getBit(2),
  LITERAL = rq::getBit(3),
  CONTEXTUAL = rq::getBit(4),
  CONTEXTUAL_VALUE = rq::getBit(5),
  CONTEXTUAL_TYPE = rq::getBit(6),
  EXPRESSION_ATTRIBUTE_TYPE = rq::getBit(7),
  TYPE_ATTRIBUTE_TYPE = rq::getBit(8),
  REFLECTIVE_TYPE = rq::getBit(9),
  PLATFORM_PRIMITIVE_TYPE = rq::getBit(10),
  STANDARD_PRIMITIVE_TYPE = rq::getBit(11),
  SCALED_PRIMITIVE_TYPE = rq::getBit(12),
  SUBTYPE = rq::getBit(13),
  UNCOUNTED_SUBTYPE = rq::getBit(14),
  ARITHMETIC_SEQUENCE = rq::getBit(15),
  WEIGHT_LEVEL = rq::getBit(16),
  LOCAL_DECLARATION = rq::getBit(17),
  LOCAL_VARIABLE = rq::getBit(18),
  SYMBOL_PARAMETER = rq::getBit(19),
  SYMBOL_PARAMETER_LIST = rq::getBit(20),
  TYPE_PARAMETER = rq::getBit(21),
  TYPE_PARAMETER_LIST = rq::getBit(22),
  SYMBOL_TABLE = rq::getBit(23),
  LOCAL_STATEMENT = rq::getBit(24),
  NAMED_TABLE = rq::getBit(25),
  GLOBAL_DECLARATION = rq::getBit(26),
  INSTANCE = rq::getBit(27),
  GLOBAL_VARIABLE = rq::getBit(28),
  RANGER = rq::getBit(29),
  PROCEDURE = rq::getBit(30),
  TEMPLATE = rq::getBit(31),
  POLYMORPH = rq::getBit(32),

  // SYMBOL DETAILS
  IS_TYPE = rq::getBit(33),
  IS_SIGNED_TYPE = rq::getBit(34),
  IS_UNSIGNED_TYPE = rq::getBit(35),
  IS_INTEGER_TYPE = rq::getBit(36),
  IS_FLOAT_TYPE = rq::getBit(37),
  IS_BINARY_TYPE = rq::getBit(38),
  IS_BFLOAT_TYPE = rq::getBit(39),
  IS_CODEUNIT_TYPE = rq::getBit(40),
  HAS_EXPRESSION_ATTRIBUTES = rq::getBit(41),
  LOCAL_TABLE = rq::getBit(42)
};

template <> struct is_flags<rq::SymbolFlags> final : std::true_type {};

[[nodiscard]] inline rq::SymbolFlags getFlags(rq::SymbolKind kind);

[[nodiscard]] inline rq::ExpressionFlags
getValidExpressionFlags(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttributeType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttributeType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledPrimitive(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUncountedSubtype(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalDeclaration(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalVariable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolParameter(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameter(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolParameterList(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeParameterList(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalStatement(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstance(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclaration(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalVariable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind);

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionAttributes(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLocalTable(rq::SymbolKind kind);

[[nodiscard]] constexpr rq::SymbolKind getInstanceKind(rq::SymbolKind kind);
[[nodiscard]] constexpr rq::SymbolKind getWeightLevelKind(rq::SymbolKind kind);
[[nodiscard]] constexpr rq::SymbolKind getTemplateKind(rq::SymbolKind kind);
[[nodiscard]] constexpr rq::SymbolKind getPolymorphKind(rq::SymbolKind kind);

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
  struct WeightLevel;
    template<rq::SymbolKind KIND_PARAM> struct DerivedUncountedWeightLevel;
  struct JuxtapositionalListItem;
  struct JuxtapositionalList;
  struct ArithmeticSequenceType;
    template<rq::SymbolKind KIND_PARAM> struct DerivedArithmeticSequenceType;
  struct LocalDeclaration;
    struct Label;
    struct Anchor;
    struct LocalVariable;
      struct LocalDynamicVariable;
      struct LocalStaticVariable;
      struct TemplateArgument;
      struct Enumerator;
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
  struct PlacementType;
  struct CompositionComponent;
  struct CompositionType;
  struct SynonymType;
  struct SymbolTable;
    struct Top;
    struct LocalStatement;
    struct NamedTable;
      struct GlobalDeclaration;
        struct Destructor;
        struct Main;
        struct Instance;
          struct ClassType;
          struct EnumerationType;
          struct Interface;
          struct Adapter;
          struct GlobalVariable;
            struct GlobalDynamicVariable;
            struct GlobalStaticVariable;
          struct Ranger;
          struct Procedure;
      struct Template;
  struct Polymorph;
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolFlags getFlags() const;
  [[nodiscard]] inline rq::ExpressionFlags getDerivedExpressionFlags() const;
  [[nodiscard]] inline const rq::Expression *getDerivedExpressionPtr() const;
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
using OutValue = rq::DerivedSimpleSymbol<rq::SymbolKind::OUT_VALUE>;
using ThisValue = rq::DerivedSimpleSymbol<rq::SymbolKind::THIS_VALUE>;
using ResultValue = rq::DerivedSimpleSymbol<rq::SymbolKind::RESULT_VALUE>;
using ValueValue = rq::DerivedSimpleSymbol<rq::SymbolKind::VALUE_VALUE>;
using IndexValue = rq::DerivedSimpleSymbol<rq::SymbolKind::INDEX_VALUE>;
using DiscriminantValue =
    rq::DerivedSimpleSymbol<rq::SymbolKind::DISCRIMINANT_VALUE>;
using CommandLineArgumentsValue =
    rq::DerivedSimpleSymbol<rq::SymbolKind::COMMAND_LINE_ARGUMENTS_VALUE>;
using CallsiteValue = rq::DerivedSimpleSymbol<rq::SymbolKind::CALLSITE_VALUE>;
using InferenceType = rq::DerivedSimpleSymbol<rq::SymbolKind::INFERENCE_TYPE>;
using VoidType = rq::DerivedSimpleSymbol<rq::SymbolKind::VOID_TYPE>;
using NoReturnType = rq::DerivedSimpleSymbol<rq::SymbolKind::NO_RETURN_TYPE>;
using AnchorAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::ANCHOR_ATTRIBUTE_TYPE>;
using OpaqueAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::OPAQUE_ATTRIBUTE_TYPE>;
using GlobalAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::GLOBAL_ATTRIBUTE_TYPE>;
using AccessAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::ACCESS_ATTRIBUTE_TYPE>;
using PartialMutateAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::PARTIAL_MUTATE_ATTRIBUTE_TYPE>;
using StaticAttributeType =
    rq::DerivedSimpleSymbol<rq::SymbolKind::STATIC_ATTRIBUTE_TYPE>;
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

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void profileScaledPrimitiveType(llvm::FoldingSetNodeID &out_id,
                                                 rq::SymbolKind kind,
                                                 rq::ScaleKind scale_kind,
                                                 unsigned scale,
                                                 std::uint64_t synonum_id);

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

  rq::SymbolConstant *_child_ptr;

  explicit RQ_ALWAYS_INLINE Subtype(rq::SymbolKind kind,
                                    rq::SymbolConstant &child);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ArraySubtype final : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::ArraySubtype;

  std::uint64_t _count;

  explicit RQ_ALWAYS_INLINE ArraySubtype(rq::SymbolConstant &child,
                                         std::uint64_t count);

  [[nodiscard]] RQ_ALWAYS_INLINE std::uint64_t getCount() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &out_id,
                                          const rq::SymbolConstant &child,
                                          std::uint64_t count);

struct UncountedSubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::SymbolKind kind,
                                             rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &out_id,
                                              rq::SymbolKind kind,
                                              const rq::SymbolConstant &child);

template <rq::SymbolKind KIND_PARAM>
struct DerivedUncountableSubtype final : public rq::UncountedSubtype {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedUncountableSubtype<KIND>;

  explicit RQ_ALWAYS_INLINE
  DerivedUncountableSubtype(rq::SymbolKind kind, rq::SymbolConstant &child);

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
  const rq::Expression *_expression_ptr;
  llvm::StringRef _path;
  llvm::StringRef _buffer;

  explicit RQ_ALWAYS_INLINE Module(rq::ModuleFactory &&factory);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getBuffer() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;

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

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct JuxtapositionalListItem final : public rq::Symbol,
                                       public llvm::FoldingSetNode {
  using Self = rq::JuxtapositionalListItem;

  rq::JuxtapositionalListItem *_next_ptr;
  rq::SymbolConstant *_type_ptr;

  explicit RQ_ALWAYS_INLINE
  JuxtapositionalListItem(rq::SymbolConstant &type,
                          rq::JuxtapositionalListItem *next_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListItem(llvm::FoldingSetNodeID &out_id,
                               const rq::SymbolConstant &type,
                               const rq::JuxtapositionalListItem *next_ptr);

struct JuxtapositionalListType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::JuxtapositionalList;

  rq::JuxtapositionalListItem *_first_item_ptr;

  explicit RQ_ALWAYS_INLINE
  JuxtapositionalListType(rq::JuxtapositionalListItem &first_item);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::JuxtapositionalListItem>,
                            rq::NextIterator<rq::JuxtapositionalListItem>,
                            std::ranges::subrange_kind::unsized>
      getJuxtapositionalListItemSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::JuxtapositionalListItem>,
                            rq::ConstNextIterator<rq::JuxtapositionalListItem>,
                            std::ranges::subrange_kind::unsized>
      getJuxtapositionalListItemSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListType(llvm::FoldingSetNodeID &out_id,
                               const rq::JuxtapositionalListItem &first_item);

struct ArithmeticSequenceType : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceType;

  rq::SymbolConstant *_child_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  explicit RQ_ALWAYS_INLINE
  ArithmeticSequenceType(rq::SymbolKind kind, rq::SymbolConstant &child,
                         rq::ArithmeticSequenceCondition condition,
                         rq::ArithmeticSequenceStep step);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getChild() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getChild();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &out_id, rq::SymbolKind kind,
    const rq::SymbolConstant &child, rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step);

template <rq::SymbolKind KIND_PARAM>
struct DerivedArithmeticSequenceType final : public rq::ArithmeticSequenceType {
  static constexpr rq::SymbolKind KIND = KIND_PARAM;
  using Self = rq::DerivedArithmeticSequenceType<KIND>;

  explicit RQ_ALWAYS_INLINE
  DerivedArithmeticSequenceType(rq::SymbolKind kind, rq::SymbolConstant &child,
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

struct LocalDeclaration : public rq::Symbol {
  using Self = rq::LocalDeclaration;

  llvm::StringRef _name;
  const rq::Expression *_name_expression_ptr;
  rq::SymbolTable *_containing_table_ptr;
  rq::SymbolTable *_hosting_table_ptr;

  explicit RQ_ALWAYS_INLINE
  LocalDeclaration(rq::SymbolKind kind, llvm::StringRef name,
                   const rq::Expression *name_expression_ptr,
                   rq::SymbolTable &containing_table,
                   rq::SymbolTable &hosting_table);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getNameExpressionPtr() const;
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
  LocalVariable(rq::SymbolKind kind, llvm::StringRef name,
                const rq::Expression *name_expression_ptr,
                rq::SymbolTable &containing_table,
                rq::SymbolTable &hosting_table, rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  RQ_ALWAYS_INLINE void setType(rq::SymbolConstant &type);
  RQ_ALWAYS_INLINE void replaceType(rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *getTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getTypePtr();

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

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolicValue &getValue() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolicValue &getValue();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Parameter : public rq::Symbol {
  using Self = rq::Parameter;

  rq::Parameter *_next_ptr;
  llvm::StringRef _name;
  rq::SymbolConstant *_type_ptr;

  explicit RQ_ALWAYS_INLINE Parameter(rq::SymbolKind kind,
                                      rq::Parameter *next_ptr,
                                      llvm::StringRef name,
                                      rq::SymbolConstant &type);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &getType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *
  getNextParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *getNextParameterPtr();

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
      rq::SymbolKind kind, rq::SymbolParameter *next_ptr, llvm::StringRef name,
      rq::SymbolConstant &type, rq::SymbolTable &containing_table,
      rq::SymbolTable &hosting_table, rq::ExpressionFlags expression_flags,
      bool is_positional, bool is_nonpositional, bool is_locked,
      const rq::Expression &expression, const rq::Expression &name_expression,
      const rq::Expression &type_expression,
      const rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
  getNextSymbolParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
  getNextSymbolParameterPtr();
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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDefaultValueExpressionPtr() const;

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

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
  getNextSignatureParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
  getNextSignatureParameterPtr();

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

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
  getNextLayoutParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
  getNextLayoutParameterPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct TypeParameter : public rq::Parameter, public llvm::FoldingSetNode {
  using Self = rq::TypeParameter;

  unsigned _location;
  bool _is_positional : 1;

  explicit RQ_ALWAYS_INLINE
  TypeParameter(rq::SymbolKind kind, rq::TypeParameter *next_ptr,
                llvm::StringRef name, rq::SymbolConstant &type,
                unsigned location, bool is_positional);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
  getNextTypeParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *getNextTypeParameterPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLocation() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNonpositional() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPositionPassable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypePassable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamePassable() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void
profileTypeParameter(llvm::FoldingSetNodeID &out_id, rq::SymbolKind kind,
                     const rq::TypeParameter *next_ptr, llvm::StringRef name,
                     const rq::SymbolConstant &type, unsigned location,
                     bool is_positional);

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
  ParameterList(rq::SymbolKind kind, rq::Parameter *first_parameter_ptr,
                unsigned parameter_count, unsigned positional_parameter_count,
                unsigned nonpositional_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Parameter *
  getFirstParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Parameter *getFirstParameterPtr();
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

struct SymbolParameterList : public rq::ParameterList {
  using Self = rq::SymbolParameterList;

  const rq::Expression *_expression_ptr;
  unsigned _locked_parameter_count;

  explicit RQ_ALWAYS_INLINE SymbolParameterList(
      rq::SymbolKind kind, rq::SymbolParameter *first_parameter_ptr,
      unsigned parameter_count, unsigned positional_parameter_count,
      unsigned nonpositional_parameter_count, const rq::Expression &expression,
      unsigned locked_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLockedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolParameter *
  getFirstSymbolParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolParameter *
  getFirstSymbolParameterPtr();
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
            const rq::Expression &expression, unsigned locked_parameter_count,
            rq::SymbolConstant &return_type,
            rq::SymbolConstant *reciever_type_ptr,
            const rq::Expression *precondition_expression_ptr,
            const rq::Expression *postcondition_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
  getRecieverTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getRecieverTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getPreconditionExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getPostconditionExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureParameter *
  getFirstSignatureParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureParameter *
  getFirstSignatureParameterPtr();
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
                                   const rq::Expression &expression,
                                   unsigned locked_parameter_count);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::LayoutParameter *
  getFirstLayoutParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::LayoutParameter *
  getFirstLayoutParameterPtr();
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
  TypeParameterList(rq::SymbolKind kind, rq::TypeParameter *first_parameter_ptr,
                    unsigned parameter_count,
                    unsigned positional_parameter_count,
                    unsigned nonpositional_parameter_count);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeParameter *
  getFirstTypeParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeParameter *getFirstTypeParameterPtr();
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
  ProcedureType(rq::ProcedureParameter *first_parameter_ptr,
                unsigned parameter_count, unsigned positional_parameter_count,
                unsigned nonpositional_parameter_count,
                rq::SymbolConstant &return_type,
                rq::SymbolConstant *reciever_type_ptr);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant &
  getReturnType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant &getReturnType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
  getRecieverTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getRecieverTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureParameter *
  getFirstProcedureParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureParameter *
  getFirstProcedureParameterPtr();
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

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

inline void
profileProcedureType(llvm::FoldingSetNodeID &out_id,
                     const rq::ProcedureParameter *first_parameter_ptr,
                     const rq::SymbolConstant &return_type,
                     const rq::SymbolConstant *reciever_type_ptr);

struct TupleType final : rq::TypeParameterList, public llvm::FoldingSetNode {
  using Self = rq::TupleType;

  unsigned _type_keyed_parameter_count;

  explicit RQ_ALWAYS_INLINE TupleType(rq::TupleParameter *first_parameter_ptr,
                                      unsigned parameter_count,
                                      unsigned positional_parameter_count,
                                      unsigned nonpositional_parameter_count,
                                      unsigned type_keyed_parameter_count);
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getNameKeyedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getTypeKeyedParameterCount() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TupleParameter *
  getFirstTupleParameterPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TupleParameter *
  getFirstTupleParameterPtr();
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

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &out_id,
                 const rq::TupleParameter *first_parameter_ptr);

struct PlacementType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::Procedure *_procedure_ptr;

  explicit RQ_ALWAYS_INLINE PlacementType(rq::Procedure &procedure);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Procedure &getProcedure() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Procedure &getProcedure();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void profilePlacement(llvm::FoldingSetNodeID &out_id,
                                       const rq::Procedure &procedure);

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

  inline void Profile(llvm::FoldingSetNodeID &out_id) const;
};

inline void
profileCompositionComponent(llvm::FoldingSetNodeID &out_id,
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

  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &out_id) const;
};

RQ_ALWAYS_INLINE void
profileCompositionType(llvm::FoldingSetNodeID &out_id,
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

  explicit RQ_ALWAYS_INLINE SymbolTable(rq::SymbolKind kind,
                                        rq::SymbolTable *containing_table_ptr);

  inline void release();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
  getContainingTablePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *getContainingTablePtr();
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
  rq::ExpressionFlags _expression_flags;

  explicit RQ_ALWAYS_INLINE LocalStatement(rq::SymbolKind kind,
                                           rq::SymbolTable &containing_table,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct NamedTable : rq::SymbolTable {
  using Self = rq::NamedTable;

  llvm::StringRef _name;
  llvm::StringRef _mangled_name{};

  explicit RQ_ALWAYS_INLINE NamedTable(rq::SymbolKind kind,
                                       rq::SymbolTable &containing_table,
                                       llvm::StringRef name);

  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const;
  RQ_ALWAYS_INLINE void setMangledName(llvm::StringRef mangled_name);
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getMangledName() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDeclaration : public rq::NamedTable {
  using Self = rq::GlobalDeclaration;

  rq::SymbolTable *_hosting_table_ptr;
  const rq::Expression *_expression_ptr;
  const rq::Expression *_name_expression_ptr;
  rq::ExpressionFlags _flags;

  explicit RQ_ALWAYS_INLINE
  GlobalDeclaration(rq::SymbolKind kind, rq::SymbolTable &containing_table,
                    llvm::StringRef name, rq::SymbolTable &hosting_table,
                    const rq::Expression &expression,
                    const rq::Expression *name_expression_ptr,
                    rq::ExpressionFlags flags);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getContainingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getContainingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &getHostingTable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHostingTable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getNameExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags getExpressionFlags() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMember() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Destructor final : public rq::GlobalDeclaration {
  using Self = rq::Destructor;

  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Destructor(rq::SymbolTable &containing_table,
                                       rq::SymbolTable &hosting_table,
                                       const rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Main final : public rq::GlobalDeclaration {
  using Self = rq::Main;

  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Main(rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Instance : public rq::GlobalDeclaration {
  using Self = rq::Instance;

  rq::Polymorph *_polymorph_ptr;
  rq::Template *_template_ptr;
  rq::TemplateArgument *_first_argument_ptr;
  rq::Instance *_next_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Instance(rq::SymbolKind kind, rq::SymbolTable &containing_table,
           llvm::StringRef name, rq::SymbolTable &hosting_table,
           const rq::Expression &expression,
           const rq::Expression *name_expression_ptr, rq::ExpressionFlags flags,
           rq::Polymorph &polymorph, rq::Template *template_ptr,
           rq::TemplateArgument *first_argument_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph &getPolymorph() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph &getPolymorph();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template *getTemplatePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template *getTemplatePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateArgument *
  getFirstTemplateArgumentPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateArgument *
  getFirstTemplateArgumentPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassType final : public rq::Instance {
  using Self = rq::ClassType;

  rq::Layout *_class_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  ClassType(rq::SymbolTable &containing_table, llvm::StringRef name,
            rq::SymbolTable &hosting_table, const rq::Expression &expression,
            const rq::Expression &name_expression, rq::ExpressionFlags flags,
            rq::Polymorph &polymorph, rq::Template *template_ptr,
            rq::TemplateArgument *first_argument_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationType final : public rq::Instance {
  using Self = rq::EnumerationType;

  const rq::Expression *_underlying_expression_ptr{nullptr};
  rq::SymbolConstant *_underlying_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumerationType(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      rq::Polymorph &polymorph, rq::Template *template_ptr,
      rq::TemplateArgument *first_argument_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Interface final : public rq::Instance {
  using Self = rq::Interface;

  explicit RQ_ALWAYS_INLINE
  Interface(rq::SymbolTable &containing_table, llvm::StringRef name,
            rq::SymbolTable &hosting_table, const rq::Expression &expression,
            const rq::Expression &name_expression, rq::ExpressionFlags flags,
            rq::Polymorph &polymorph, rq::Template *template_ptr,
            rq::TemplateArgument *first_argument_ptr);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Adapter final : public rq::Instance {
  using Self = rq::Adapter;

  rq::Interface *_interface_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Adapter(rq::SymbolTable &containing_table, llvm::StringRef name,
          rq::SymbolTable &hosting_table, const rq::Expression &expression,
          const rq::Expression &name_expression, rq::ExpressionFlags flags,
          rq::Polymorph &polymorph, rq::Template *template_ptr,
          rq::TemplateArgument *first_argument_ptr);

  RQ_ALWAYS_INLINE void setInterface(rq::Interface &interface);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface *getInterfacePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Interface *getInterfacePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalVariable : public rq::Instance {
  using Self = rq::GlobalVariable;

  rq::SymbolConstant *_type_ptr{nullptr};
  const rq::Expression *_initial_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE GlobalVariable(
      rq::SymbolKind kind, rq::SymbolTable &containing_table,
      llvm::StringRef name, rq::SymbolTable &hosting_table,
      const rq::Expression &expression, const rq::Expression &name_expression,
      rq::ExpressionFlags flags, rq::Polymorph &polymorph,
      rq::Template *template_ptr, rq::TemplateArgument *first_argument_ptr,
      const rq::Expression *initial_value_expression_ptr);

  RQ_ALWAYS_INLINE void setType(rq::SymbolConstant &type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *getTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getInitialValueExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalDynamicVariable;

  rq::Entity *_initial_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariable(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      rq::Polymorph &polymorph, rq::Template *template_ptr,
      rq::TemplateArgument *first_argument_ptr,
      const rq::Expression &initial_value_expression);

  RQ_ALWAYS_INLINE void setInitialValue(rq::Entity &initial_value);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getInitialValuePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getInitialValuePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariable final : public rq::GlobalVariable {
  using Self = rq::GlobalStaticVariable;

  rq::Entity *_value_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariable(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      rq::Polymorph &polymorph, rq::Template *template_ptr,
      rq::TemplateArgument *first_argument_ptr,
      const rq::Expression &initial_value_expression);

  RQ_ALWAYS_INLINE void setValue(rq::Entity &value);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getValuePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getValuePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Ranger : public rq::Instance {
  using Self = rq::Ranger;

  const rq::Expression *_reciever_type_expression_ptr;
  rq::SymbolConstant *_reciever_type_ptr{nullptr};
  const rq::Expression *_element_type_expression_ptr;
  rq::SymbolConstant *_element_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Ranger(rq::SymbolKind kind, rq::SymbolTable &containing_table,
         llvm::StringRef name, rq::SymbolTable &hosting_table,
         const rq::Expression &expression,
         const rq::Expression &name_expression, rq::ExpressionFlags flags,
         rq::Polymorph &polymorph, rq::Template *template_ptr,
         rq::TemplateArgument *first_argument_ptr,
         const rq::Expression &reciever_type_expression,
         const rq::Expression &element_type_expression);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getRecieverTypeExpression() const;
  RQ_ALWAYS_INLINE void setRecieverType(rq::SymbolConstant &reciever_type);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
  getRecieverTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getRecieverTypePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getElementTypeExpression() const;
  RQ_ALWAYS_INLINE void setElementType(rq::SymbolConstant &element);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolConstant *
  getElementTypePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolConstant *getElementTypePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Procedure : public rq::Instance {
  using Self = rq::Procedure;

  rq::Signature *_signature_ptr{nullptr};
  const rq::Expression *_signature_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  Procedure(rq::SymbolKind kind, rq::SymbolTable &containing_table,
            llvm::StringRef name, rq::SymbolTable &hosting_table,
            const rq::Expression &expression,
            const rq::Expression &name_expression, rq::ExpressionFlags flags,
            rq::Polymorph &polymorph, rq::Template *template_ptr,
            rq::TemplateArgument *first_argument_ptr,
            const rq::Expression &signature_expression);

  RQ_ALWAYS_INLINE void setSignature(rq::Signature &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature *getSignaturePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature *getSignaturePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getSignatureExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Template : public rq::GlobalDeclaration {
  using Self = rq::Template;

  const rq::Expression *_layout_expression_ptr;
  rq::Layout *_layout_ptr{nullptr};
  rq::Template *_next_ptr{nullptr};
  const rq::Expression *_constraint_expression_ptr;
  const rq::Expression *_weight_expression_ptr;
  unsigned _weight;

  explicit RQ_ALWAYS_INLINE
  Template(rq::SymbolKind kind, rq::SymbolTable &containing_table,
           llvm::StringRef name, rq::SymbolTable &hosting_table,
           const rq::Expression &expression,
           const rq::Expression &name_expression, rq::ExpressionFlags flags,
           const rq::Expression &layout_expression,
           const rq::Expression *constraint_expression_ptr,
           const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getLayoutExpression() const;
  RQ_ALWAYS_INLINE void setLayout(rq::Layout &layout);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout *getLayoutPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout *getLayoutPtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getConstraintExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getWeightExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getWeight() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WeightLevel final : rq::Symbol {
  using Self = rq::WeightLevel;

  unsigned _weight;
  rq::Polymorph *_polymorph_ptr{nullptr};
  rq::WeightLevel *_next_ptr{nullptr};
  rq::Template *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE WeightLevel(rq::SymbolKind kind, unsigned weight,
                                        rq::Polymorph &polymorph);
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getWeight() const;
  [[nodiscard]] const rq::Polymorph &getPolymorph() const;
  [[nodiscard]] rq::Polymorph &getPolymorph();
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

  rq::Instance *_first_instance_ptr{nullptr};
  rq::WeightLevel *_highest_weight_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::SymbolKind kind);

  inline void addTemplate(rq::BumpPtrAllocator &allocator,
                          rq::Template &template_);
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Instance>,
                            rq::NextIterator<rq::Instance>,
                            std::ranges::subrange_kind::unsized>
      getInstanceSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Instance>,
                            rq::ConstNextIterator<rq::Instance>,
                            std::ranges::subrange_kind::unsized>
      getInstanceSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::WeightLevel>,
                            rq::NextIterator<rq::WeightLevel>,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::WeightLevel>,
                            rq::ConstNextIterator<rq::WeightLevel>,
                            std::ranges::subrange_kind::unsized>
      getWeightSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>