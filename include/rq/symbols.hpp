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
  EXPRESSION_TYPE_ATTRIBUTE_TYPE = rq::getBit(7),
  TYPE_ATTRIBUTE_TYPE = rq::getBit(8),
  REFLECTIVE_TYPE = rq::getBit(9),
  PLATFORM_PRIMITIVE_TYPE = rq::getBit(10),
  STANDARD_PRIMITIVE_TYPE = rq::getBit(11),
  SCALED_PRIMITIVE_TYPE = rq::getBit(12),
  SUBTYPE = rq::getBit(13),
  UNCOUNTED_SUBTYPE = rq::getBit(14),
  ARITHMETIC_SEQUENCE = rq::getBit(15),
  LOCAL_DECLARATION = rq::getBit(16),
  LOCAL_VARIABLE = rq::getBit(17),
  SYMBOL_PARAMETER = rq::getBit(18),
  SYMBOL_PARAMETER_LIST = rq::getBit(19),
  TYPE_PARAMETER = rq::getBit(20),
  TYPE_PARAMETER_LIST = rq::getBit(21),
  SYMBOL_TABLE = rq::getBit(22),
  LOCAL_STATEMENT = rq::getBit(23),
  NAMED_TABLE = rq::getBit(24),
  GLOBAL_DECLARATION = rq::getBit(25),
  GLOBAL_VARIABLE = rq::getBit(26),
  CALLABLE = rq::getBit(27),
  RANGER = rq::getBit(28),
  PROCEDURE = rq::getBit(29),
  TEMPLATE = rq::getBit(30),
  POLYMORPH = rq::getBit(31),

  // SYMBOL DETAILS
  IS_TYPE = rq::getBit(32),
  IS_SIGNED_TYPE = rq::getBit(33),
  IS_UNSIGNED_TYPE = rq::getBit(34),
  IS_INTEGER_TYPE = rq::getBit(35),
  IS_FLOAT_TYPE = rq::getBit(36),
  IS_BINARY_TYPE = rq::getBit(37),
  IS_BFLOAT_TYPE = rq::getBit(38),
  IS_CODEUNIT_TYPE = rq::getBit(39),
  HAS_EXPRESSION_ATTRIBUTES = rq::getBit(40),
  LOCAL_TABLE = rq::getBit(41)
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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalDeclaration(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGlobalVariable(rq::SymbolKind kind);
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCallable(rq::SymbolKind kind);
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

// clang-format off
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
      struct OpaqueType;
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
      struct SliceSubtype;
      struct InferenceCountArraySubtype;
  struct Module;
  struct Import;
  struct JuxtapositionalListItem;
  struct JuxtapositionalList;
  struct ArithmeticSequenceType;
    struct ArithmeticIntervalType;
    struct ArithmeticSequenceType;
    struct FiniteArithmeticSequenceType;
  struct LocalDeclaration;
    struct Label;
    struct Anchor;
    struct LocalVariable;
      struct LocalDynamicVariable;
      struct LocalStaticVariable;
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
  struct CompositionType;
  struct SynonymType;
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
    struct NamedTable;
      struct Namespace;
      struct GlobalDeclaration;
        struct ClassType;
        struct EnumerationType;
        struct Interface;
        struct GlobalVariable;
          struct GlobalDynamicVariable;
          struct GlobalStaticVariable;
        struct Callable;
        struct Destructor;
        struct Main;
        struct Ranger;
          struct ForwardRanger;
          struct BackwardRanger;
        struct Procedure;
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
  struct Polymorph;
    struct ForwardRangerPolymorph;
    struct BackwardRangerPolymorph;
    struct FunctionPolymorph;
    struct MethodPolymorph;
    struct ExtensionMethodPolymorph;
    struct ClassPolymorph;
    struct EnumerationPolymorph;
    struct InterfacePolymorph;
    struct GlobalStaticVariablePolymorph;
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

struct LiteralType : public rq::SimpleSymbol {
  using Self = rq::LiteralType;

  explicit RQ_ALWAYS_INLINE LiteralType(rq::SymbolKind kind);

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

  explicit RQ_ALWAYS_INLINE Contextual(rq::SymbolKind kind);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ContextualValue : public rq::Contextual {
  using Self = rq::ContextualValue;

  explicit RQ_ALWAYS_INLINE ContextualValue(rq::SymbolKind kind);

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

struct ExpressionAttributeType : public rq::SimpleSymbol {
  using Self = rq::ExpressionAttributeType;

  explicit RQ_ALWAYS_INLINE ExpressionAttributeType(rq::SymbolKind kind);

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

  explicit RQ_ALWAYS_INLINE TypeAttributeType(rq::SymbolKind kind);

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

struct PlatformPrimitiveType : public rq::SimpleSymbol {
  using Self = rq::PlatformPrimitiveType;

  explicit RQ_ALWAYS_INLINE PlatformPrimitiveType(rq::SymbolKind kind);

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

struct CharType final : public rq::PlatformPrimitiveType {
  using Self = rq::CharType;

  explicit RQ_ALWAYS_INLINE CharType();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct StandardPrimitiveType : rq::SimpleSymbol {
  using Self = rq::StandardPrimitiveType;

  explicit RQ_ALWAYS_INLINE StandardPrimitiveType(rq::SymbolKind kind);

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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileScaledPrimitiveType(llvm::FoldingSetNodeID &id,
                                                 rq::SymbolKind kind,
                                                 rq::ScaleKind scale_kind,
                                                 unsigned scale,
                                                 std::uint64_t synonum_id);

struct ScaledSignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledSignedIntegerType;

  explicit RQ_ALWAYS_INLINE ScaledSignedIntegerType(rq::ScaleKind kind,
                                                    unsigned scale,
                                                    std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ScaledUnsignedIntegerType final : public rq::ScaledPrimitiveType {
  using Self = rq::ScaledUnsignedIntegerType;

  explicit RQ_ALWAYS_INLINE ScaledUnsignedIntegerType(rq::ScaleKind kind,
                                                      unsigned scale,
                                                      std::uint64_t synonym_id);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &id,
                                          const rq::SymbolConstant &child,
                                          std::uint64_t count);

struct UncountedSubtype : public rq::Subtype, public llvm::FoldingSetNode {
  using Self = rq::UncountedSubtype;

  explicit RQ_ALWAYS_INLINE UncountedSubtype(rq::SymbolKind kind,
                                             rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void profileUncountedSubtype(llvm::FoldingSetNodeID &id,
                                              rq::SymbolKind kind,
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

struct SliceSubtype final : public rq::UncountedSubtype {
  using Self = rq::SliceSubtype;

  explicit RQ_ALWAYS_INLINE SliceSubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InferenceCountArraySubtype final : public rq::UncountedSubtype {
  using Self = rq::InferenceCountArraySubtype;

  explicit RQ_ALWAYS_INLINE
  InferenceCountArraySubtype(rq::SymbolConstant &child);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListItem(llvm::FoldingSetNodeID &id,
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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileJuxtapositionalListType(llvm::FoldingSetNodeID &id,
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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileArithmeticSequenceType(llvm::FoldingSetNodeID &id, rq::SymbolKind kind,
                          const rq::SymbolConstant &child,
                          rq::ArithmeticSequenceCondition condition,
                          rq::ArithmeticSequenceStep step);

struct ArithmeticIntervalType final : public rq::ArithmeticSequenceType {
  using Self = rq::ArithmeticIntervalType;

  explicit RQ_ALWAYS_INLINE
  ArithmeticIntervalType(rq::SymbolConstant &child,
                     rq::ArithmeticSequenceCondition condition);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InfiniteArithmeticSequenceType final : public rq::ArithmeticSequenceType {
  using Self = rq::InfiniteArithmeticSequenceType;

  explicit RQ_ALWAYS_INLINE
  InfiniteArithmeticSequenceType(rq::SymbolConstant &child,
                             rq::ArithmeticSequenceStep step);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FiniteArithmeticSequenceType final : public rq::ArithmeticSequenceType {
  using Self = rq::FiniteArithmeticSequenceType;

  explicit RQ_ALWAYS_INLINE
  FiniteArithmeticSequenceType(rq::SymbolConstant &child,
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

struct Enumerator final : public rq::LocalVariable {
  using Self = rq::Enumerator;

  const rq::Expression *_default_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  Enumerator(llvm::StringRef name, const rq::Expression &name_expression,
             rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
             rq::ExpressionFlags flags,
             rq::Expression *default_value_expression_ptr);

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
  getDefaultValueExpressionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumerationType &
  getEnumerationType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumerationType &getEnumerationType();

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
profileTypeParameter(llvm::FoldingSetNodeID &id, rq::SymbolKind kind,
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
  ProcedureType(rq::TypeParameter *first_parameter_ptr,
                unsigned parameter_count, unsigned positional_parameter_count,
                unsigned nonpositional_parameter_count,
                rq::SymbolConstant &return_type,
                rq::SymbolConstant *reciever_type_ptr);
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

  inline void Profile(llvm::FoldingSetNodeID &id, rq::SymbolKind kind,
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

  inline void Profile(llvm::FoldingSetNodeID &id) const;
};

RQ_ALWAYS_INLINE void
profileTupleType(llvm::FoldingSetNodeID &id,
                 const rq::TupleParameter *first_parameter_ptr);

struct PlacementType final : public rq::Symbol, llvm::FoldingSetNode {
  using Self = rq::PlacementType;

  rq::Procedure *_procedure_ptr;

  explicit RQ_ALWAYS_INLINE PlacementType(rq::Procedure &procedure);
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
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::CompositionComponent *
  getNextComponentPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::CompositionComponent *
  getNextComponentPtr();

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

struct Namespace final : public rq::NamedTable {
  using Self = rq::Namespace;

  explicit RQ_ALWAYS_INLINE Namespace(rq::SymbolTable &containing_table,
                                      llvm::StringRef name);

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

struct ClassType final : public rq::GlobalDeclaration {
  using Self = rq::ClassType;

  rq::Layout *_class_layout_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  ClassType(rq::SymbolTable &containing_table, llvm::StringRef name,
            rq::SymbolTable &hosting_table, const rq::Expression &expression,
            const rq::Expression &name_expression, rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationType final : public rq::GlobalDeclaration {
  using Self = rq::EnumerationType;

  const rq::Expression *_underlying_expression_ptr{nullptr};
  rq::SymbolConstant *_underlying_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE EnumerationType(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Interface final : public rq::GlobalDeclaration {
  using Self = rq::Interface;

  explicit RQ_ALWAYS_INLINE
  Interface(rq::SymbolTable &containing_table, llvm::StringRef name,
            rq::SymbolTable &hosting_table, const rq::Expression &expression,
            const rq::Expression &name_expression, rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalVariable : public rq::GlobalDeclaration {
  using Self = rq::GlobalVariable;

  rq::SymbolConstant *_type_ptr{nullptr};
  const rq::Expression *_initial_value_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  GlobalVariable(rq::SymbolKind kind, rq::SymbolTable &containing_table,
                 llvm::StringRef name, rq::SymbolTable &hosting_table,
                 const rq::Expression &expression,
                 const rq::Expression &name_expression,
                 rq::ExpressionFlags flags,
                 const rq::Expression &initial_value_expression);

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
      const rq::Expression &initial_value_expression);

  RQ_ALWAYS_INLINE void setValue(rq::Entity &value);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *getValuePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *getValuePtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Callable : public rq::GlobalDeclaration {
  using Self = rq::Callable;

  rq::Instruction *_instruction_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Callable(rq::SymbolKind kind,
                                     rq::SymbolTable &containing_table,
                                     llvm::StringRef name,
                                     rq::SymbolTable &hosting_table,
                                     const rq::Expression &expression,
                                     const rq::Expression *name_expression_ptr,
                                     rq::ExpressionFlags flags);

  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction *
  getInstructionPtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction *getInstructionPtr();

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Destructor final : public rq::Callable {
  using Self = rq::Destructor;

  explicit RQ_ALWAYS_INLINE Destructor(rq::SymbolTable &containing_table,
                                       rq::SymbolTable &hosting_table,
                                       const rq::Expression &expression,
                                       rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Main final : public rq::Callable {
  using Self = rq::Main;

  explicit RQ_ALWAYS_INLINE Main(rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags flags);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Ranger : public rq::Callable {
  using Self = rq::Ranger;

  rq::Ranger *_next_ptr{nullptr};
  const rq::Expression *_reciever_type_expression_ptr;
  rq::SymbolConstant *_reciever_type_ptr{nullptr};
  const rq::Expression *_element_type_expression_ptr;
  rq::SymbolConstant *_element_type_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE
  Ranger(rq::SymbolKind kind, rq::SymbolTable &containing_table,
         rq::SymbolTable &hosting_table, const rq::Expression &expression,
         rq::ExpressionFlags flags,
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

struct ForwardRanger final : public rq::Ranger {
  using Self = rq::ForwardRanger;

  explicit RQ_ALWAYS_INLINE
  ForwardRanger(rq::SymbolTable &containing_table,
                rq::SymbolTable &hosting_table,
                const rq::Expression &expression, rq::ExpressionFlags flags,
                const rq::Expression &reciever_type_expression,
                const rq::Expression &element_type_expression);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BackwardRanger final : public rq::Ranger {
  using Self = rq::BackwardRanger;

  explicit RQ_ALWAYS_INLINE
  BackwardRanger(rq::SymbolTable &containing_table,
                 rq::SymbolTable &hosting_table,
                 const rq::Expression &expression, rq::ExpressionFlags flags,
                 const rq::Expression &reciever_type_expression,
                 const rq::Expression &element_type_expression);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Procedure : public rq::Callable {
  using Self = rq::Procedure;

  rq::Procedure *_next_ptr{nullptr};
  rq::Signature *_signature_ptr{nullptr};
  const rq::Expression *_signature_expression_ptr;

  explicit RQ_ALWAYS_INLINE
  Procedure(rq::SymbolKind kind, rq::SymbolTable &containing_table,
            llvm::StringRef name, rq::SymbolTable &hosting_table,
            const rq::Expression &expression,
            const rq::Expression *name_expression_ptr,
            rq::ExpressionFlags flags,
            const rq::Expression &signature_expression);

  RQ_ALWAYS_INLINE void setSignature(rq::Signature &signature);
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature *getSignaturePtr() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature *getSignaturePtr();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getSignatureExpression() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Function final : public rq::Procedure {
  using Self = rq::Function;

  explicit RQ_ALWAYS_INLINE
  Function(rq::SymbolTable &containing_table, llvm::StringRef name,
           rq::SymbolTable &hosting_table, const rq::Expression &expression,
           const rq::Expression &name_expression, rq::ExpressionFlags flags,
           const rq::Expression &signature_expression);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct Method final : public rq::Procedure {
  using Self = rq::Method;

  explicit RQ_ALWAYS_INLINE
  Method(rq::SymbolTable &containing_table, llvm::StringRef name,
         rq::SymbolTable &hosting_table, const rq::Expression &expression,
         const rq::Expression &name_expression, rq::ExpressionFlags flags,
         const rq::Expression &signature_expression);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExtensionMethod final : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  explicit RQ_ALWAYS_INLINE ExtensionMethod(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &signature_expression);

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

struct ClassTemplate final : public rq::Template {
  using Self = rq::ClassTemplate;

  explicit RQ_ALWAYS_INLINE ClassTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationTemplate final : public rq::Template {
  using Self = rq::EnumerationTemplate;

  explicit RQ_ALWAYS_INLINE EnumerationTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfaceTemplate final : public rq::Template {
  using Self = rq::InterfaceTemplate;

  explicit RQ_ALWAYS_INLINE InterfaceTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariableTemplate final : public rq::Template {
  using Self = rq::GlobalDynamicVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariableTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariableTemplate final : public rq::Template {
  using Self = rq::GlobalStaticVariableTemplate;

  explicit RQ_ALWAYS_INLINE GlobalStaticVariableTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ForwardRangerTemplate final : public rq::Template {
  using Self = rq::ForwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE ForwardRangerTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BackwardRangerTemplate final : public rq::Template {
  using Self = rq::BackwardRangerTemplate;

  explicit RQ_ALWAYS_INLINE BackwardRangerTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionTemplate final : public rq::Template {
  using Self = rq::FunctionTemplate;

  explicit RQ_ALWAYS_INLINE FunctionTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MethodTemplate final : public rq::Template {
  using Self = rq::MethodTemplate;

  explicit RQ_ALWAYS_INLINE MethodTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExtensionMethodTemplate final : public rq::Template {
  using Self = rq::ExtensionMethodTemplate;

  explicit RQ_ALWAYS_INLINE ExtensionMethodTemplate(
      rq::SymbolTable &containing_table, llvm::StringRef name,
      rq::SymbolTable &hosting_table, const rq::Expression &expression,
      const rq::Expression &name_expression, rq::ExpressionFlags flags,
      const rq::Expression &layout_expression,
      const rq::Expression *constraint_expression_ptr,
      const rq::Expression *weight_expression_ptr, unsigned weight);

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct WeightLevel final {
  using Self = rq::WeightLevel;

  unsigned _weight;
  rq::WeightLevel *_next_ptr{nullptr};
  rq::Template *_first_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE WeightLevel(unsigned weight);
  WeightLevel(const Self &) = delete;
  WeightLevel(Self &&) = delete;
  ~WeightLevel() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getWeight() const;
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

  rq::WeightLevel *_highest_weight_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE Polymorph(rq::SymbolKind kind);

  inline void addTemplate(rq::BumpPtrAllocator &allocator,
                          rq::Template &template_);
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

struct ForwardRangerPolymorph final : public rq::Polymorph {
  using Self = rq::ForwardRangerPolymorph;

  rq::ForwardRanger *_first_ranger_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ForwardRangerPolymorph();

  inline void addForwardRanger(rq::ForwardRanger &forward_ranger);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Ranger, rq::ForwardRanger>,
                            rq::NextIterator<rq::Ranger, rq::ForwardRanger>,
                            std::ranges::subrange_kind::unsized>
      getForwardRangerSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Ranger, rq::ForwardRanger>,
      rq::ConstNextIterator<rq::Ranger, rq::ForwardRanger>,
      std::ranges::subrange_kind::unsized>
  getForwardRangerSubrange() const;
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Template, rq::ForwardRangerTemplate>,
      rq::NextIterator<rq::Template, rq::ForwardRangerTemplate>,
      std::ranges::subrange_kind::unsized>
  getForwardRangerTemplateSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Template, rq::ForwardRangerTemplate>,
      rq::ConstNextIterator<rq::Template, rq::ForwardRangerTemplate>,
      std::ranges::subrange_kind::unsized>
  getForwardRangerTemplateSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct BackwardRangerPolymorph final : public rq::Polymorph {
  using Self = rq::BackwardRangerPolymorph;

  rq::BackwardRanger *_first_ranger_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE BackwardRangerPolymorph();

  inline void addBackwardRanger(rq::BackwardRanger &backward_ranger);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Ranger, rq::BackwardRanger>,
                            rq::NextIterator<rq::Ranger, rq::BackwardRanger>,
                            std::ranges::subrange_kind::unsized>
      getBackwardRangerSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Ranger, rq::BackwardRanger>,
      rq::ConstNextIterator<rq::Ranger, rq::BackwardRanger>,
      std::ranges::subrange_kind::unsized>
  getBackwardRangerSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct FunctionPolymorph final : public rq::Polymorph {
  using Self = rq::FunctionPolymorph;

  rq::Function *_first_function_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE FunctionPolymorph();

  inline void addFunction(rq::Function &function);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Procedure, rq::Function>,
                            rq::NextIterator<rq::Procedure, rq::Function>,
                            std::ranges::subrange_kind::unsized>
      getFunctionSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Procedure, rq::Function>,
                            rq::ConstNextIterator<rq::Procedure, rq::Function>,
                            std::ranges::subrange_kind::unsized>
      getFunctionSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct MethodPolymorph final : public rq::Polymorph {
  using Self = rq::MethodPolymorph;

  rq::Method *_first_method_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE MethodPolymorph();

  inline void addMethod(rq::Method &method);

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::NextIterator<rq::Procedure, rq::Method>,
                            rq::NextIterator<rq::Procedure, rq::Method>,
                            std::ranges::subrange_kind::unsized>
      getMethodSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstNextIterator<rq::Procedure, rq::Method>,
                            rq::ConstNextIterator<rq::Procedure, rq::Method>,
                            std::ranges::subrange_kind::unsized>
      getMethodSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ExtensionMethodPolymorph final : public rq::Polymorph {
  using Self = rq::ExtensionMethodPolymorph;

  rq::ExtensionMethod *_first_extension_method_ptr{nullptr};

  explicit RQ_ALWAYS_INLINE ExtensionMethodPolymorph();

  inline void addExtensionMethod(rq::ExtensionMethod &extension_method);

  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::NextIterator<rq::Procedure, rq::ExtensionMethod>,
      rq::NextIterator<rq::Procedure, rq::ExtensionMethod>,
      std::ranges::subrange_kind::unsized>
  getExtensionMethodSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstNextIterator<rq::Procedure, rq::ExtensionMethod>,
      rq::ConstNextIterator<rq::Procedure, rq::ExtensionMethod>,
      std::ranges::subrange_kind::unsized>
  getExtensionMethodSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct ClassPolymorph final : public rq::Polymorph {
  using Self = rq::ClassPolymorph;

  rq::BumpPtrList<rq::ClassType> _class_type_list{};

  explicit RQ_ALWAYS_INLINE ClassPolymorph();

  inline void addClassType(rq::BumpPtrAllocator &allocator,
                           rq::ClassType &class_type);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasClassType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMultipleClassType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassType &getClassType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassType &getClassType() const;

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::ClassType>,
                            rq::BumpPtrListIterator<rq::ClassType>,
                            std::ranges::subrange_kind::unsized>
      getClassTypeSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstBumpPtrListIterator<rq::ClassType>,
                            rq::ConstBumpPtrListIterator<rq::ClassType>,
                            std::ranges::subrange_kind::unsized>
      getClassTypeSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct EnumerationPolymorph final : public rq::Polymorph {
  using Self = rq::EnumerationPolymorph;

  rq::BumpPtrList<rq::EnumerationType> _enumeration_type_list{};

  explicit RQ_ALWAYS_INLINE EnumerationPolymorph();

  inline void addEnumerationType(rq::BumpPtrAllocator &allocator,
                                 rq::EnumerationType &type);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEnumerationType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMultipleEnumerationType() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EnumerationType &getEnumerationType();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumerationType &
  getEnumerationType() const;

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::EnumerationType>,
                            rq::BumpPtrListIterator<rq::EnumerationType>,
                            std::ranges::subrange_kind::unsized>
      getEnumerationTypeSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstBumpPtrListIterator<rq::EnumerationType>,
                            rq::ConstBumpPtrListIterator<rq::EnumerationType>,
                            std::ranges::subrange_kind::unsized>
      getEnumerationTypeSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct InterfacePolymorph final : public rq::Polymorph {
  using Self = rq::InterfacePolymorph;

  rq::BumpPtrList<rq::Interface> _interface_list{};

  explicit RQ_ALWAYS_INLINE InterfacePolymorph();

  inline void addInterface(rq::BumpPtrAllocator &allocator,
                           rq::Interface &interface);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMultipleInterface() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Interface &getInterface();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Interface &getInterface() const;

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::Interface>,
                            rq::BumpPtrListIterator<rq::Interface>,
                            std::ranges::subrange_kind::unsized>
      getInterfaceSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstBumpPtrListIterator<rq::Interface>,
                            rq::ConstBumpPtrListIterator<rq::Interface>,
                            std::ranges::subrange_kind::unsized>
      getInterfaceSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalDynamicVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalDynamicVariablePolymorph;

  rq::BumpPtrList<rq::GlobalDynamicVariable> _global_dynamic_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalDynamicVariablePolymorph();

  inline void addGlobalDynamicVariable(rq::BumpPtrAllocator &allocator,
                                       rq::GlobalDynamicVariable &variable);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobalDynamicVariable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMultipleGlobalDynamicVariable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalDynamicVariable &
  getGlobalDynamicVariable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalDynamicVariable &
  getGlobalDynamicVariable() const;

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::GlobalDynamicVariable>,
                            rq::BumpPtrListIterator<rq::GlobalDynamicVariable>,
                            std::ranges::subrange_kind::unsized>
      getGlobalDynamicVariableSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::GlobalDynamicVariable>,
      rq::ConstBumpPtrListIterator<rq::GlobalDynamicVariable>,
      std::ranges::subrange_kind::unsized>
  getGlobalDynamicVariableSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

struct GlobalStaticVariablePolymorph final : public rq::Polymorph {
  using Self = rq::GlobalStaticVariablePolymorph;

  rq::BumpPtrList<rq::GlobalStaticVariable> _global_static_variable_list{};

  explicit RQ_ALWAYS_INLINE GlobalStaticVariablePolymorph();

  inline void addGlobalStaticVariable(rq::BumpPtrAllocator &allocator,
                                      rq::GlobalStaticVariable &variable);

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobalStaticVariable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMultipleGlobalStaticVariable() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::GlobalStaticVariable &
  getGlobalStaticVariable();
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::GlobalStaticVariable &
  getGlobalStaticVariable() const;

  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::BumpPtrListIterator<rq::GlobalStaticVariable>,
                            rq::BumpPtrListIterator<rq::GlobalStaticVariable>,
                            std::ranges::subrange_kind::unsized>
      getGlobalStaticVariableSubrange();
  [[nodiscard]] RQ_ALWAYS_INLINE std::ranges::subrange<
      rq::ConstBumpPtrListIterator<rq::GlobalStaticVariable>,
      rq::ConstBumpPtrListIterator<rq::GlobalStaticVariable>,
      std::ranges::subrange_kind::unsized>
  getGlobalStaticVariableSubrange() const;

  [[nodiscard]] static inline bool classof(const rq::Entity *entity_ptr);
};

} // namespace rq

#include <rq/detail/symbols.hpp>