#pragma once

#include <rq/bump_ptr_allocator.hpp>

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    return "None";
  case S::INTEGER_LITERAL_TYPE:
    return "IntegerLiteral";
  case S::FLOAT_LITERAL_TYPE:
    return "FloatLiteral";
  case S::STRING_LITERAL_TYPE:
    return "StringLiteral";
  case S::CODEUNIT_LITERAL_TYPE:
    return "CodeunitLiteral";
  case S::UNKNOWN_TYPE:
    return "Unknown";
  case S::INFERENCE_TYPE:
    return "Inference";
  case S::VOID_TYPE:
    return "Void";
  case S::NO_RETURN_TYPE:
    return "NoReturn";
  case S::MODIFIER_TYPE:
    return "Modifier";
  case S::QUALIFIER_TYPE:
    return "Qualifier";
  case S::SYMBOL_TYPE:
    return "Symbol";
  case S::SYMBOL_RANGE_TYPE:
    return "SymbolRange";
  case S::EXPRESSION_TYPE:
    return "Expression";
  case S::EXPRESSION_RANGE_TYPE:
    return "ExpressionRange";
  case S::FSINT:
    return "FSInt";
  case S::FUINT:
    return "FUInt";
  case S::LSINT:
    return "LSInt";
  case S::LUINT:
    return "LUInt";
  case S::BOOLEAN_TYPE:
    return "Bool";
  case S::HALF_TYPE:
    return "Half";
  case S::SINGLE_TYPE:
    return "Single";
  case S::DOUBLE_TYPE:
    return "Double";
  case S::QUADRUPLE_TYPE:
    return "Quadruple";
  case S::SINT:
    return "SInt";
  case S::UINT:
    return "UInt";
  case S::SSIZE:
    return "SSize";
  case S::USIZE:
    return "USize";
  case S::SINDEX:
    return "SIndex";
  case S::UINDEX:
    return "UIndex";
  case S::CHAR_TYPE:
    return "Char";
  case S::BINARY16_TYPE:
    return "Binary16";
  case S::BINARY32_TYPE:
    return "Binary32";
  case S::BINARY64_TYPE:
    return "Binary64";
  case S::BINARY128_TYPE:
    return "Binary128";
  case S::BFLOAT16_TYPE:
    return "BFloat16";
  case S::ASCII_TYPE:
    return "Ascii";
  case S::UTF8_TYPE:
    return "Utf8";
  case S::SCALED_SINT:
    return "ScaledSInt";
  case S::SCALED_UINT:
    return "ScaledUInt";
  case S::SCALED_FSINT:
    return "ScaledFSInt";
  case S::SCALED_FUINT:
    return "ScaledFUInt";
  case S::SCALED_LSINT:
    return "ScaledLSInt";
  case S::SCALED_LUINT:
    return "ScaledLUInt";
  case S::DYNAMIC_VARIADIC_ARGUMENTS_TYPE:
    return "DynamicVariadicArguments";
  case S::ARRAY_SUBTYPE:
    return "Array";
  case S::REF_SUBTYPE:
    return "Ref";
  case S::PTR_SUBTYPE:
    return "Ptr";
  case S::SLICE_SUBTYPE:
    return "Slice";
  case S::SPLIT_SUBTYPE:
    return "Split";
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return "InferenceCountArray";
  case S::MODULE:
    return "Module";
  case S::IMPORT:
    return "Import";
  case S::REALIZATION:
    return "Realization";
  case S::CONFORMITY:
    return "Conformity";
  case S::ADAPTION:
    return "Adaption";
  case S::JUXT_LIST_TYPE:
    return "JuxtListType";
  case S::JUXT_LIST_ITEM:
    return "JuxtListItem";
  case S::SPECIALIZATION_SET_ARGUMENT:
    return "SpecializationSetArgument";
  case S::SPECIALIATION_SET:
    return "SpecializationSet";
  case S::ADAPTER_SPECIALIZATION_SET:
    return "AdapterSpecializationSet";
  case S::PROCEDURE_SPECIALIZATION_SET:
    return "ProcedureSpecializationSet";
  case S::ARITHMETIC_INTERVAL_TYPE:
    return "ArithmeticInterval";
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "InfiniteArithmeticSequence";
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "InfiniteArithmeticSequence";
  case S::ANCHOR:
    return "Anchor";
  case S::ENUMERATOR:
    return "Enumerator";
  case S::DYNAMIC_EAGER_VARIABLE:
    return "DynamicEagerVariable";
  case S::STATIC_EAGER_VARIABLE:
    return "StaticEagerVariable";
  case S::PARAMETER:
    return "Parameter";
  case S::SIGNATURE_TYPE:
    return "SignatureType";
  case S::LAYOUT_TYPE:
    return "LayoutType";
  case S::PLACEMENT_TYPE:
    return "PlacementType";
  case S::COMPOSITION_COMPONENT:
    return "CompositionComponent";
  case S::COMPOSITION_TYPE:
    return "CompositionType";
  case S::SYNONYM_TYPE:
    return "SynonymnType";
  case S::ALIAS:
    return "Alias";
  case S::PORTAL:
    return "Portal";
  case S::C_TABLE:
    return "CTable";
  case S::NAMESPACE:
    return "Namespace";
  case S::IF_STATEMENT:
    return "IfStatement";
  case S::ELSE_IF_STATEMENT:
    return "ElseIfStatement";
  case S::ELSE_STATEMENT:
    return "ElseStatement";
  case S::SWITCH_STATEMENT:
    return "SwitchStatement";
  case S::CASE_STATEMENT:
    return "CaseStatement";
  case S::DEFAULT_STATEMENT:
    return "DefaultStatement";
  case S::FOR_STATEMENT:
    return "ForStatement";
  case S::WHILE_STATEMENT:
    return "WhileStatement";
  case S::SPIN_STATEMENT:
    return "SpinStatement";
  case S::WEAVE_STATEMENT:
    return "WeaveStatement";
  case S::SCOPE_STATEMENT:
    return "ScopeStatement";
  case S::CLASS_OVERLOAD:
    return "ClassOverload";
  case S::ENUM_OVERLOAD:
    return "EnumOverload";
  case S::INTERFACE_OVERLOAD:
    return "InterfaceOverload";
  case S::ADAPTER_OVERLOAD:
    return "AdapterOverload";
  case S::PROCEDURE_OVERLOAD:
    return "ProcedureOverload";
  case S::LAZY_VARIABLE_OVERLOAD:
    return "LazyVariableOverload";
  case S::CLASS_TEMPLATE:
    return "ClassTemplate";
  case S::ENUM_TEMPLATE:
    return "EnumTemplate";
  case S::INTERFACE_TEMPLATE:
    return "InterfaceTemplate";
  case S::ADAPTER_TEMPLATE:
    return "AdapterTemplate";
  case S::PROCEDURE_TEMPLATE:
    return "ProcedureTemplate";
  case S::LAZY_VARIABLE_TEMPLATE:
    return "LazyVariableTemplate";
  case S::CLASS_SPECIALIZATION:
    return "ClassSpecialization";
  case S::ENUM_SPECIALIZATION:
    return "EnumSpecialization";
  case S::INTERFACE_SPECIALIZATION:
    return "InterfaceSpecialization";
  case S::ADAPTER_SPECIALIZATION:
    return "AdapterSpecialization";
  case S::PROCEDURE_SPECIALIZATION:
    return "ProcedureSpecialization";
  case S::LAZY_VARIABLE_SPECIALIZATION:
    return "LazyVariableSpecialization";
  case S::CLASS_POLYMORPH:
    return "ClassPolymorph";
  case S::ENUM_POLYMORPH:
    return "EnumPolymorph";
  case S::INTERFACE_POLYMORPH:
    return "InterfacePolymorph";
  case S::ADAPTER_POLYMORPH:
    return "AdapterPolymorph";
  case S::PROCEDURE_POLYMORPH:
    return "ProcedurePolymorph";
  case S::LAZY_VARIABLE_POLYMORPH:
    return "LazyVariablePolymorph";
  case S::CLASS_WEIGHT_LEVEL:
    return "ClassWeightLevel";
  case S::ENUM_WEIGHT_LEVEL:
    return "EnumWeightLevel";
  case S::INTERFACE_WEIGHT_LEVEL:
    return "InterfaceWeightLevel";
  case S::ADAPTER_WEIGHT_LEVEL:
    return "AdapterWeightLevel";
  case S::PROCEDURE_WEIGHT_LEVEL:
    return "ProcedureWeightLevel";
  case S::LAZY_VARIABLE_WEIGHT_LEVEL:
    return "LazyVariableWeightLevel";
  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline llvm::StringRef getName(rq::EvaluationState state) {
  using ES = rq::EvaluationState;
  switch (state) {
  case ES::NONE:
    break;
  case ES::SURVEYED:
    return "surveyed";
  case ES::DECLARING:
    return "declaring";
  case ES::DECLARED:
    return "declared";
  case ES::IMPLEMENTING:
    return "implementing";
  case ES::IMPLEMENTED:
    return "implemented";
  case ES::ERROR:
    return "error";
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind) : Entity(rq::getId(kind)) {
  RQ_ASSERT(kind > rq::SymbolKind::NONE && kind < rq::SymbolKind::LAST,
            "not symbol kind");
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::SymbolKind kind)
    : Symbol(kind) {
  RQ_ASSERT(rq::getIsSimpleSymbol(kind), "not simple symbol");
}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsSimpleSymbol();
}

RQ_ALWAYS_INLINE Literal::Literal(rq::SymbolKind kind) : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsLiteralType(kind), "not literal");
}

[[nodiscard]] inline bool Literal::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsLiteralType();
}

RQ_ALWAYS_INLINE IntegerLiteral::IntegerLiteral()
    : Literal(rq::SymbolKind::INTEGER_LITERAL_TYPE) {}

[[nodiscard]] inline bool
IntegerLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTEGER_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE FloatLiteral::FloatLiteral()
    : Literal(rq::SymbolKind::FLOAT_LITERAL_TYPE) {}

[[nodiscard]] inline bool FloatLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::FLOAT_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE StringLiteral::StringLiteral()
    : Literal(rq::SymbolKind::STRING_LITERAL_TYPE) {}

[[nodiscard]] inline bool StringLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::STRING_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE CodeunitLiteral::CodeunitLiteral()
    : Literal(rq::SymbolKind::CODEUNIT_LITERAL_TYPE) {}

[[nodiscard]] inline bool
CodeunitLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CODEUNIT_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE Figurative::Figurative(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsFigurative(kind), "not figurative");
}

[[nodiscard]] inline bool Figurative::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsFigurative();
}

RQ_ALWAYS_INLINE InferenceType::InferenceType()
    : Figurative(rq::SymbolKind::INFERENCE_TYPE) {}

[[nodiscard]] inline bool InferenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INFERENCE_TYPE);
}

RQ_ALWAYS_INLINE VoidType::VoidType() : Figurative(rq::SymbolKind::VOID_TYPE) {}

[[nodiscard]] inline bool VoidType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::VOID_TYPE);
}

RQ_ALWAYS_INLINE NoReturnType::NoReturnType()
    : Figurative(rq::SymbolKind::NO_RETURN_TYPE) {}

[[nodiscard]] inline bool NoReturnType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::NO_RETURN_TYPE);
}

RQ_ALWAYS_INLINE UnknownType::UnknownType()
    : Figurative(rq::SymbolKind::UNKNOWN_TYPE) {}

[[nodiscard]] inline bool UnknownType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::UNKNOWN_TYPE);
}

RQ_ALWAYS_INLINE ReflectionType::ReflectionType(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsReflectionType(kind), "not reflection");
}

[[nodiscard]] inline bool
ReflectionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsReflectionType();
}

RQ_ALWAYS_INLINE SymbolType::SymbolType()
    : ReflectionType(rq::SymbolKind::SYMBOL_TYPE) {}

[[nodiscard]] inline bool SymbolType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SYMBOL_TYPE);
}

RQ_ALWAYS_INLINE ExpressionType::ExpressionType()
    : ReflectionType(rq::SymbolKind::EXPRESSION_TYPE) {}

[[nodiscard]] inline bool
ExpressionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::EXPRESSION_TYPE);
}

RQ_ALWAYS_INLINE SymbolRangeType::SymbolRangeType()
    : ReflectionType(rq::SymbolKind::SYMBOL_RANGE_TYPE) {}

[[nodiscard]] inline bool
SymbolRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SYMBOL_RANGE_TYPE);
}

RQ_ALWAYS_INLINE ExpressionRangeType::ExpressionRangeType()
    : ReflectionType(rq::SymbolKind::EXPRESSION_RANGE_TYPE) {}

[[nodiscard]] inline bool
ExpressionRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::EXPRESSION_RANGE_TYPE);
}

RQ_ALWAYS_INLINE PrimitiveType::PrimitiveType(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsPrimitiveType(kind), "not primitive type");
}

[[nodiscard]] inline bool PrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsPrimitiveType();
}

RQ_ALWAYS_INLINE
StandardPrimitiveType::StandardPrimitiveType(rq::SymbolKind kind)
    : PrimitiveType(kind) {
  RQ_ASSERT(rq::getIsStandardPrimitiveType(kind),
            "not standard primitive type");
}

[[nodiscard]] inline bool
StandardPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsStandardPrimitiveType();
}

RQ_ALWAYS_INLINE Binary16Type::Binary16Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY16_TYPE) {}

[[nodiscard]] inline bool Binary16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BINARY16_TYPE);
}

RQ_ALWAYS_INLINE Binary32Type::Binary32Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY32_TYPE) {}

[[nodiscard]] inline bool Binary32Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BINARY32_TYPE);
}

RQ_ALWAYS_INLINE Binary64Type::Binary64Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY64_TYPE) {}

[[nodiscard]] inline bool Binary64Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BINARY64_TYPE);
}

RQ_ALWAYS_INLINE Binary128Type::Binary128Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY128_TYPE) {}

[[nodiscard]] inline bool Binary128Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BINARY128_TYPE);
}

RQ_ALWAYS_INLINE BFloat16Type::BFloat16Type()
    : StandardPrimitiveType(rq::SymbolKind::BFLOAT16_TYPE) {}

[[nodiscard]] inline bool BFloat16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BFLOAT16_TYPE);
}

RQ_ALWAYS_INLINE AsciiType::AsciiType()
    : StandardPrimitiveType(rq::SymbolKind::ASCII_TYPE) {}

[[nodiscard]] inline bool AsciiType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ASCII_TYPE);
}

RQ_ALWAYS_INLINE Utf8Type::Utf8Type()
    : StandardPrimitiveType(rq::SymbolKind::UTF8_TYPE) {}

[[nodiscard]] inline bool Utf8Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::UTF8_TYPE);
}

RQ_ALWAYS_INLINE
PlatformPrimitiveType::PlatformPrimitiveType(rq::SymbolKind kind)
    : PrimitiveType(kind) {
  RQ_ASSERT(rq::getIsPlatformPrimitiveType(kind),
            "not platform primitive type");
}

[[nodiscard]] inline bool
PlatformPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsPlatformPrimitiveType();
}

RQ_ALWAYS_INLINE HalfType::HalfType()
    : PlatformPrimitiveType(rq::SymbolKind::HALF_TYPE) {}

[[nodiscard]] inline bool HalfType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::HALF_TYPE);
}

RQ_ALWAYS_INLINE SingleType::SingleType()
    : PlatformPrimitiveType(rq::SymbolKind::SINGLE_TYPE) {}

[[nodiscard]] inline bool SingleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SINGLE_TYPE);
}

RQ_ALWAYS_INLINE DoubleType::DoubleType()
    : PlatformPrimitiveType(rq::SymbolKind::DOUBLE_TYPE) {}

[[nodiscard]] inline bool DoubleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::DOUBLE_TYPE);
}

RQ_ALWAYS_INLINE QuadrupleType::QuadrupleType()
    : PlatformPrimitiveType(rq::SymbolKind::QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool QuadrupleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::QUADRUPLE_TYPE);
}

RQ_ALWAYS_INLINE BooleanType::BooleanType()
    : PlatformPrimitiveType(rq::SymbolKind::BOOLEAN_TYPE) {}

[[nodiscard]] inline bool BooleanType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::BOOLEAN_TYPE);
}

RQ_ALWAYS_INLINE CharType::CharType()
    : PlatformPrimitiveType(rq::SymbolKind::CHAR_TYPE) {}

[[nodiscard]] inline bool CharType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CHAR_TYPE);
}

RQ_ALWAYS_INLINE USizeType::USizeType()
    : PlatformPrimitiveType(rq::SymbolKind::USIZE) {}

[[nodiscard]] inline bool USizeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::USIZE);
}

RQ_ALWAYS_INLINE SSizeType::SSizeType()
    : PlatformPrimitiveType(rq::SymbolKind::SSIZE) {}

[[nodiscard]] inline bool SSizeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SSIZE);
}

RQ_ALWAYS_INLINE UIndexType::UIndexType()
    : PlatformPrimitiveType(rq::SymbolKind::UINDEX) {}

[[nodiscard]] inline bool UIndexType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::UINDEX);
}

RQ_ALWAYS_INLINE SIndexType::SIndexType()
    : PlatformPrimitiveType(rq::SymbolKind::SINDEX) {}

[[nodiscard]] inline bool SIndexType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SINDEX);
}

RQ_ALWAYS_INLINE QualifierType::QualifierType()
    : AttributeType(rq::SymbolKind::QUALIFIER_TYPE) {}

[[nodiscard]] inline bool QualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::QUALIFIER_TYPE);
}

RQ_ALWAYS_INLINE ModifierType::ModifierType()
    : AttributeType(rq::SymbolKind::MODIFIER_TYPE) {}

[[nodiscard]] inline bool ModifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::MODIFIER_TYPE);
}

RQ_ALWAYS_INLINE Subtype::Subtype(rq::SymbolKind kind,
                                  rq::ConstantSymbol &child)
    : Symbol(kind), _child_ptr(&child) {
  RQ_ASSERT(rq::getIsSubtype(kind), "not subtype");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Subtype::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Subtype::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] inline bool Subtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsSubtype();
}

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(std::size_t count,
                                            rq::ConstantSymbol &child)
    : Subtype(rq::SymbolKind::ARRAY_SUBTYPE, child), _count(count) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::size_t ArraySubtype::getCount() const {
  return this->_count;
}

[[nodiscard]] inline bool ArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ARRAY_SUBTYPE);
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileArraySubtype(inout_id, this->getChild(), this->getCount());
}

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &inout_id,
                                          const rq::ConstantSymbol &child,
                                          std::size_t count) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(count);
}

RQ_ALWAYS_INLINE SimpleSubtype::SimpleSubtype(rq::SymbolKind kind,
                                              rq::ConstantSymbol &child)
    : Subtype(kind, child) {
  RQ_ASSERT(rq::getIsSimpleSubtype(kind), "not simple subtype");
}

[[nodiscard]] inline bool SimpleSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsSimpleSubtype();
}

inline void SimpleSubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileSimpleSubtype(inout_id, this->getKind(), this->getChild());
}

RQ_ALWAYS_INLINE void profileSimpleSubtype(llvm::FoldingSetNodeID &inout_id,
                                           rq::SymbolKind kind,
                                           const rq::ConstantSymbol &child) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(&child);
}

RQ_ALWAYS_INLINE RefSubtype::RefSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::REF_SUBTYPE, child) {}

[[nodiscard]] inline bool RefSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::REF_SUBTYPE);
}

RQ_ALWAYS_INLINE PtrSubtype::PtrSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::PTR_SUBTYPE, child) {}

[[nodiscard]] inline bool PtrSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PTR_SUBTYPE);
}

RQ_ALWAYS_INLINE InferenceCountArraySubtype::InferenceCountArraySubtype(
    rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE, child) {}

[[nodiscard]] inline bool
InferenceCountArraySubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE);
}

RQ_ALWAYS_INLINE SliceSubtype::SliceSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::SLICE_SUBTYPE, child) {}

[[nodiscard]] inline bool SliceSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SLICE_SUBTYPE);
}

RQ_ALWAYS_INLINE SplitSubtype::SplitSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::SPLIT_SUBTYPE, child) {}

[[nodiscard]] inline bool SplitSubtype::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SPLIT_SUBTYPE);
}

RQ_ALWAYS_INLINE Adaption::Adaption(rq::ConstantSymbol &reciever,
                                    rq::InterfaceImplementation &interface,
                                    rq::AdapterImplementation &adapter)
    : Symbol(rq::SymbolKind::ADAPTION), _reciever_ptr(&reciever),
      _interface_ptr(&interface), _adapter_ptr(&adapter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Adaption::getReciever() const {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Adaption::getReciever() {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Adaption::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Adaption::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
Adaption::getAdapter() const {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &
Adaption::getAdapter() {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] inline bool Adaption::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTION);
}

inline void Adaption::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileAdaption(inout_id, this->getInterface(), this->getAdapter());
}

RQ_ALWAYS_INLINE void
profileAdaption(llvm::FoldingSetNodeID &inout_id,
                const rq::InterfaceImplementation &interface,
                const rq::AdapterImplementation &adapter) {
  inout_id.AddPointer(&interface);
  inout_id.AddPointer(&adapter);
}

RQ_ALWAYS_INLINE
Realization::Realization(rq::ConstantSymbol &reciever,
                         rq::InterfaceImplementation &interface)
    : Symbol(rq::SymbolKind::REALIZATION), _reciever_ptr(&reciever),
      _interface_ptr(&interface) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Realization::getReciever() const {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Realization::getReciever() {
  return rq::dereferencePtr(this->_reciever_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Realization::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Realization::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] inline bool Realization::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::REALIZATION);
}

inline void Realization::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileRealization(inout_id, this->getReciever(), this->getInterface());
}

RQ_ALWAYS_INLINE void
profileRealization(llvm::FoldingSetNodeID &inout_id,
                   const rq::ConstantSymbol &reciever,
                   const rq::InterfaceImplementation &interface) {
  inout_id.AddPointer(&reciever);
  inout_id.AddPointer(&interface);
}

RQ_ALWAYS_INLINE Conformity::Conformity(rq::InterfaceImplementation &interface,
                                        rq::AdapterImplementation &adapter)
    : Symbol(rq::SymbolKind::CONFORMITY), _interface_ptr(&interface),
      _adapter_ptr(&adapter) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfaceImplementation &
Conformity::getInterface() const {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfaceImplementation &
Conformity::getInterface() {
  return rq::dereferencePtr(this->_interface_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterImplementation &
Conformity::getAdapter() const {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterImplementation &
Conformity::getAdapter() {
  return rq::dereferencePtr(this->_adapter_ptr);
}

[[nodiscard]] inline bool Conformity::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CONFORMITY);
}

inline void Conformity::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileConformity(inout_id, this->getInterface(), this->getAdapter());
}

RQ_ALWAYS_INLINE void
profileConformity(llvm::FoldingSetNodeID &inout_id,
                  const rq::InterfaceImplementation &interface,
                  const rq::AdapterImplementation &adapter) {
  inout_id.AddPointer(&interface);
  inout_id.AddPointer(&adapter);
}

RQ_ALWAYS_INLINE JuxtListItem::JuxtListItem(rq::JuxtListItem &next,
                                            rq::ConstantSymbol &type)
    : Symbol(rq::SymbolKind::JUXT_LIST_ITEM), _next_ptr(&next),
      _type_ptr(&type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
JuxtListItem::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &JuxtListItem::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] inline bool JuxtListItem::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::JUXT_LIST_ITEM);
}

inline void JuxtListItem::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileJuxtListItem(inout_id, this->_next_ptr, this->getType());
}

RQ_ALWAYS_INLINE void profileJuxtListItem(llvm::FoldingSetNodeID &inout_id,
                                          const rq::JuxtListItem *next_ptr,
                                          const rq::ConstantSymbol &type) {
  inout_id.AddPointer(next_ptr);
  inout_id.AddPointer(&type);
}

RQ_ALWAYS_INLINE JuxtListType::JuxtListType(rq::JuxtListItem &first)
    : Symbol(rq::SymbolKind::JUXT_LIST_TYPE), _first_ptr(&first) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::JuxtListItem>
JuxtListType::getItemSubrange() {
  return rq::NextSubrange<rq::JuxtListItem>(
      rq::NextIterator<rq::JuxtListItem>(this->_first_ptr),
      rq::NextIterator<rq::JuxtListItem>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::JuxtListItem>
JuxtListType::getItemSubrange() const {
  return rq::ConstNextSubrange<rq::JuxtListItem>(
      rq::ConstNextIterator<rq::JuxtListItem>(this->_first_ptr),
      rq::ConstNextIterator<rq::JuxtListItem>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::JuxtListItem>
JuxtListType::getConstItemSubrange() const {
  return rq::ConstNextSubrange<rq::JuxtListItem>(
      rq::ConstNextIterator<rq::JuxtListItem>(this->_first_ptr),
      rq::ConstNextIterator<rq::JuxtListItem>());
}

[[nodiscard]] inline bool JuxtListType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::JUXT_LIST_TYPE);
}

inline void JuxtListType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileJuxtListType(inout_id, this->_first_ptr);
}

RQ_ALWAYS_INLINE void profileJuxtListType(llvm::FoldingSetNodeID &inout_id,
                                          const rq::JuxtListItem *first_ptr) {
  inout_id.AddPointer(first_ptr);
}

RQ_ALWAYS_INLINE SynonymType::SynonymType(rq::Symbol &original)
    : Symbol(rq::SymbolKind::SYNONYM_TYPE), _original_ptr(&original) {}

[[nodiscard]] const rq::Symbol &SynonymType::getOriginal() const {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] rq::Symbol &SynonymType::getOriginal() {
  return rq::dereferencePtr(this->_original_ptr);
}

[[nodiscard]] inline bool SynonymType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SYNONYM_TYPE);
}

RQ_ALWAYS_INLINE
ArithmeticSequenceType::ArithmeticSequenceType(
    rq::SymbolKind kind, rq::ConstantSymbol &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step)
    : Symbol(kind), _child_ptr(&child), _condition(condition), _step(step) {
  using S = rq::SymbolKind;
  RQ_ASSERT(rq::getIsArithmeticSequenceType(kind), "not arithmetic sequence");
  RQ_ASSERT(kind != S::ARITHMETIC_INTERVAL_TYPE ||
                step != rq::ArithmeticStep::NONE,
            "interval must have no step");
  RQ_ASSERT(kind != S::INFINITE_ARITHMETIC_SEQUENCE_TYPE ||
                condition != rq::ArithmeticCondition::NONE,
            "inifinite must have no condition");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
ArithmeticSequenceType::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &
ArithmeticSequenceType::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticCondition
ArithmeticSequenceType::getCondition() const {
  return this->_condition;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticStep
ArithmeticSequenceType::getStep() const {
  return this->_step;
}

[[nodiscard]] inline bool
ArithmeticSequenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsArithmeticSequenceType();
}

inline void
ArithmeticSequenceType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileArithmeticSequenceType(inout_id, this->getChild(),
                                    this->getCondition(), this->getStep());
}

RQ_ALWAYS_INLINE void profileArithmeticSequenceType(
    llvm::FoldingSetNodeID &inout_id, const rq::ConstantSymbol &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(rq::getUnderlyingValue(condition));
  inout_id.AddInteger(rq::getUnderlyingValue(step));
}

[[nodiscard]] inline bool
ArithmeticIntervalType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ARITHMETIC_INTERVAL_TYPE);
}

[[nodiscard]] inline bool
InfiniteArithmeticSequenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::INFINITE_ARITHMETIC_SEQUENCE_TYPE);
}

[[nodiscard]] inline bool
FiniteArithmeticSequenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::FINITE_ARITHMETIC_SEQUENCE_TYPE);
}

RQ_ALWAYS_INLINE
SpecializationSetArgument::SpecializationSetArgument(
    rq::Name name, rq::Entity &value, rq::SpecializationSetArgument *next_ptr)
    : Symbol(rq::SymbolKind::SPECIALIZATION_SET_ARGUMENT), _name(name),
      _value_ptr(&value), _next_ptr(next_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name
SpecializationSetArgument::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &
SpecializationSetArgument::getValue() const {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
SpecializationSetArgument::getValue() {
  return rq::dereferencePtr(this->_value_ptr);
}

[[nodiscard]] inline bool
SpecializationSetArgument::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::SPECIALIZATION_SET_ARGUMENT);
}

inline void
SpecializationSetArgument::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileSpecializationSetArgument(inout_id, this->getName(),
                                       this->getValue(), this->_next_ptr);
}

RQ_ALWAYS_INLINE void
profileSpecializationSetArgument(llvm::FoldingSetNodeID &inout_id,
                                 rq::Name name, const rq::Entity &value,
                                 rq::SpecializationSetArgument *next_ptr) {
  inout_id.Add(name);
  inout_id.AddPointer(&value);
  inout_id.AddPointer(next_ptr);
}

RQ_ALWAYS_INLINE
SpecializationSet::SpecializationSet(rq::SymbolKind kind,
                                     rq::SpecializationSetArgument *first_ptr)
    : Symbol(kind), _first_ptr(first_ptr) {
  RQ_ASSERT(rq::getIsSpecializationSet(kind), "not specialization set");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::SpecializationSetArgument>
SpecializationSet::getArgumentSubrange() {
  return rq::NextSubrange<rq::SpecializationSetArgument>(
      rq::NextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::NextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]]
RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::SpecializationSetArgument>
SpecializationSet::getArgumentSubrange() const {
  return rq::ConstNextSubrange<rq::SpecializationSetArgument>(
      rq::ConstNextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::ConstNextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::SpecializationSetArgument>
    SpecializationSet::getConstArgumentSubrange() const {
  return rq::ConstNextSubrange<rq::SpecializationSetArgument>(
      rq::ConstNextIterator<rq::SpecializationSetArgument>(this->_first_ptr),
      rq::ConstNextIterator<rq::SpecializationSetArgument>());
}

[[nodiscard]] inline bool
SpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsSpecializationSet();
}

inline void SpecializationSet::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileSpecializationSet(inout_id, this->getKind(), this->_first_ptr);
}

RQ_ALWAYS_INLINE void
profileSpecializationSet(llvm::FoldingSetNodeID &inout_id, rq::SymbolKind kind,
                         const rq::SpecializationSetArgument *first_ptr) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(first_ptr);
}

[[nodiscard]] inline bool
ProcedureSpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::PROCEDURE_SPECIALIZATION_SET);
}

[[nodiscard]] inline bool
AdapterSpecializationSet::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::ADAPTER_SPECIALIZATION_SET);
}

RQ_ALWAYS_INLINE
ParameterDetail::ParameterDetail(rq::Name name, rq::ConstantSymbol &type,
                                 rq::ModifierFuseFlags modifier_fuse_flags,
                                 rq::ParameterInfoFlags param_info_flags,
                                 rq::Entity *default_ptr)
    : _name(name), _type_ptr(&type), _modifier_fuse_flags(modifier_fuse_flags),
      _param_info_flags(param_info_flags), _default_ptr(default_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name ParameterDetail::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
ParameterDetail::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &ParameterDetail::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
ParameterDetail::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
ParameterDetail::getParameterInfoFlags() const {
  return this->_param_info_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
ParameterDetail::getDefaultPtr() const {
  return this->_default_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *ParameterDetail::getDefaultPtr() {
  return this->_default_ptr;
}

RQ_ALWAYS_INLINE Parameter::Parameter(rq::Parameter *next_ptr, rq::Name name,
                                      rq::ConstantSymbol &type,
                                      rq::ModifierFuseFlags modifier_flags,
                                      rq::ParameterInfoFlags param_flags,
                                      rq::Entity *default_ptr)
    : Symbol(rq::SymbolKind::PARAMETER), _next_ptr(next_ptr), _name(name),
      _type_ptr(&type), _modifier_fuse_flags(modifier_flags),
      _param_info_flags(param_flags), _default_ptr(default_ptr) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name Parameter::getName() const {
  return this->_name;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Parameter::getType() const {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Parameter::getType() {
  return rq::dereferencePtr(this->_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
Parameter::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterInfoFlags
Parameter::getParameterFlags() const {
  return this->_param_info_flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity *
Parameter::getDefaultPtr() const {
  return this->_default_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *Parameter::getDefaultPtr() {
  return this->_default_ptr;
}

[[nodiscard]] inline bool Parameter::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PARAMETER);
}

inline void Parameter::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileParameter(inout_id, this->_next_ptr, this->getName(),
                       this->getType(), this->getModifierFuseFlags(),
                       this->getParameterFlags(), this->getDefaultPtr());
}

RQ_ALWAYS_INLINE void profileParameter(
    llvm::FoldingSetNodeID &inout_id, const rq::Parameter *next_ptr,
    rq::Name name, const rq::ConstantSymbol &type,
    rq::ModifierFuseFlags modifier_fuse_flags,
    rq::ParameterInfoFlags param_flags, const rq::Entity *default_ptr) {
  inout_id.AddPointer(next_ptr);
  inout_id.Add(name);
  inout_id.AddPointer(&type);
  inout_id.AddInteger(rq::getUnderlyingValue(modifier_fuse_flags));
  inout_id.AddInteger(rq::getUnderlyingValue(param_flags));
  inout_id.AddPointer(default_ptr);
}

RQ_ALWAYS_INLINE
CompositionComponent::CompositionComponent(
    rq::CompositionComponent *next_ptr, rq::InterfaceImplementation &interface)
    : Symbol(rq::SymbolKind::COMPOSITION_COMPONENT), _next_ptr(next_ptr),
      _interface_ptr(&interface) {}

[[nodiscard]] inline bool
CompositionComponent::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::COMPOSITION_COMPONENT);
}

inline void
CompositionComponent::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileCompositionComponent(inout_id, this->_next_ptr,
                                  this->getInterface());
}

RQ_ALWAYS_INLINE void
profileCompositionComponent(llvm::FoldingSetNodeID &inout_id,
                            const rq::CompositionComponent *next_ptr,
                            const rq::InterfaceImplementation &interface) {
  inout_id.AddPointer(next_ptr);
  inout_id.AddPointer(&interface);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundPositionalParametersEnd() const {
  return rq::getHasNone(this->_found_parameter_marks,
                        rq::ParameterInfoFlags::POSITIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundNonpositionalParametersBegin() const {
  return rq::getHasAll(this->_found_parameter_marks,
                       rq::ParameterInfoFlags::NONPOSITIONAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundLockedParametersBegin() const {
  return rq::getHasAll(this->_found_parameter_marks,
                       rq::ParameterInfoFlags::LOCKED);
}

RQ_ALWAYS_INLINE void ParameterListDetail::appendPositionaParametersEnd() {
  RQ_ASSERT(!this->getFoundPositionalParametersEnd(), "already found");
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "locked already began");
  this->_found_parameter_marks &= ~rq::ParameterInfoFlags::POSITIONAL;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendNonpositionalParametersBegin() {
  RQ_ASSERT(!this->getFoundNonpositionalParametersBegin(), "already found");
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "locked already began");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::NONPOSITIONAL;
}

RQ_ALWAYS_INLINE void ParameterListDetail::appendLockedParametersBegin() {
  RQ_ASSERT(!this->getFoundLockedParametersBegin(), "already found");
  this->_found_parameter_marks = rq::ParameterInfoFlags::LOCKED;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendParameter(rq::Name name, rq::ConstantSymbol &type,
                                     rq::ModifierFuseFlags modifier_fuse_flags,
                                     rq::Entity *default_ptr) {
  this->_parameter_detail_list.emplace_back(name, type, modifier_fuse_flags,
                                            this->_found_parameter_marks,
                                            default_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE std::span<const rq::ParameterDetail>
ParameterListDetail::getParameterDetailSpan() const {
  return std::span<const rq::ParameterDetail>(
      this->_parameter_detail_list.data(), this->_parameter_detail_list.size());
}

RQ_ALWAYS_INLINE
ParameterList::ParameterList(rq::SymbolKind kind,
                             rq::Parameter *first_parameter_ptr)
    : Symbol(kind), _first_ptr(first_parameter_ptr) {
  RQ_ASSERT(rq::getIsParameterList(kind), "not parameter list");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
ParameterList::getParameterSubrange() const {
  return rq::ConstNextSubrange<rq::Parameter>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_ptr),
      rq::ConstNextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
ParameterList::getConstParameterSubrange() const {
  return rq::ConstNextSubrange<rq::Parameter>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_ptr),
      rq::ConstNextIterator<rq::Parameter>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Parameter>
ParameterList::getParameterSubrange() {
  return rq::NextSubrange<rq::Parameter>(
      rq::NextIterator<rq::Parameter>(this->_first_ptr),
      rq::NextIterator<rq::Parameter>());
}

[[nodiscard]] inline bool ParameterList::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsParameterList();
}

RQ_ALWAYS_INLINE
SignatureType::SignatureType(rq::Parameter *first_parameter_ptr,
                             rq::ConstantSymbol &return_type)
    : ParameterList(rq::SymbolKind::SIGNATURE_TYPE, first_parameter_ptr),
      _return_type_ptr(&return_type) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
SignatureType::getReturnType() const {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &
SignatureType::getReturnType() {
  return rq::dereferencePtr(this->_return_type_ptr);
}

[[nodiscard]] inline bool SignatureType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::SIGNATURE_TYPE);
}

inline void SignatureType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileSignatureType(inout_id, this->_first_ptr, this->getReturnType());
}

RQ_ALWAYS_INLINE void
profileSignatureType(llvm::FoldingSetNodeID &inout_id,
                     const rq::Parameter *first_parameter_ptr,
                     const rq::ConstantSymbol &return_type) {
  inout_id.AddPointer(first_parameter_ptr);
  inout_id.AddPointer(&return_type);
}

RQ_ALWAYS_INLINE LayoutType::LayoutType(rq::Parameter *first_parameter_ptr)
    : ParameterList(rq::SymbolKind::LAYOUT_TYPE, first_parameter_ptr) {}

[[nodiscard]] inline bool LayoutType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAYOUT_TYPE);
}

inline void LayoutType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileLayoutType(inout_id, this->_first_ptr);
}

RQ_ALWAYS_INLINE void
profileLayoutType(llvm::FoldingSetNodeID &inout_id,
                  const rq::Parameter *first_parameter_ptr) {
  inout_id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE
PlacementType::PlacementType(rq::ProcedureImplementation &function)
    : Symbol(rq::SymbolKind::PLACEMENT_TYPE), _procedure_ptr(&function) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedureImplementation &
PlacementType::getProcedure() const {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedureImplementation &
PlacementType::getProcedure() {
  return rq::dereferencePtr(this->_procedure_ptr);
}

[[nodiscard]] inline bool PlacementType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PLACEMENT_TYPE);
}

inline void PlacementType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profilePlacementType(inout_id, this->getProcedure());
}

RQ_ALWAYS_INLINE void
profilePlacementType(llvm::FoldingSetNodeID &inout_id,
                     const rq::ProcedureImplementation &function) {
  inout_id.AddPointer(&function);
}

RQ_ALWAYS_INLINE WeightLevel::WeightLevel(rq::SymbolKind kind, unsigned weight)
    : Symbol(kind), _weight(weight) {
  RQ_ASSERT(rq::getIsWeightLevel(kind), "not weight level");
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Polymorph *
WeightLevel::getPolymorphPtr() const {
  return this->_polymorph_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Polymorph *WeightLevel::getPolymorphPtr() {
  return this->_polymorph_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned WeightLevel::getWeight() const {
  return this->_weight;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template>
WeightLevel::getTemplateSubrange() {
  return rq::NextSubrange<rq::Template>(
      rq::NextIterator<rq::Template>(this->_first_ptr),
      rq::NextIterator<rq::Template>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
WeightLevel::getTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template>(
      rq::ConstNextIterator<rq::Template>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
WeightLevel::getConstTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template>(
      rq::ConstNextIterator<rq::Template>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template>());
}

[[nodiscard]] inline bool WeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsWeightLevel();
}

RQ_ALWAYS_INLINE InterfaceWeightLevel::InterfaceWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::INTERFACE_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
InterfaceWeightLevel::setInterfacePolymorph(rq::InterfacePolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfacePolymorph *
InterfaceWeightLevel::getInterfacePolymorphPtr() const {
  return llvm::cast<rq::InterfacePolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfacePolymorph *
InterfaceWeightLevel::getInterfacePolymorphPtr() {
  return llvm::cast<rq::InterfacePolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
InterfaceWeightLevel::addInterfaceTemplate(rq::InterfaceTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfaceWeightLevel::getInterfaceTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfaceWeightLevel::getInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfaceWeightLevel::getConstInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>());
}

[[nodiscard]] inline bool
InterfaceWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE ProcedureWeightLevel::ProcedureWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::PROCEDURE_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
ProcedureWeightLevel::setProcedurePolymorph(rq::ProcedurePolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ProcedurePolymorph *
ProcedureWeightLevel::getProcedurePolymorphPtr() const {
  return llvm::cast<rq::ProcedurePolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ProcedurePolymorph *
ProcedureWeightLevel::getProcedurePolymorphPtr() {
  return llvm::cast<rq::ProcedurePolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
ProcedureWeightLevel::addProcedureTemplate(rq::ProcedureTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::ProcedureTemplate>
    ProcedureWeightLevel::getProcedureTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::ProcedureTemplate>(
      rq::NextIterator<rq::Template, rq::ProcedureTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::ProcedureTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>
    ProcedureWeightLevel::getProcedureTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ProcedureTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::ProcedureTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>
    ProcedureWeightLevel::getConstProcedureTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ProcedureTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ProcedureTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::ProcedureTemplate>());
}

[[nodiscard]] inline bool
ProcedureWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE AdapterWeightLevel::AdapterWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::ADAPTER_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
AdapterWeightLevel::setAdapterPolymorph(rq::AdapterPolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::AdapterPolymorph *
AdapterWeightLevel::getAdapterPolymorphPtr() const {
  return llvm::cast<rq::AdapterPolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::AdapterPolymorph *
AdapterWeightLevel::getAdapterPolymorphPtr() {
  return llvm::cast<rq::AdapterPolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
AdapterWeightLevel::addAdapterTemplate(rq::AdapterTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterWeightLevel::getAdapterTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::NextIterator<rq::Template, rq::AdapterTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterWeightLevel::getAdapterTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>
    AdapterWeightLevel::getConstAdapterTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::AdapterTemplate>(
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::AdapterTemplate>());
}

[[nodiscard]] inline bool
AdapterWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE EnumWeightLevel::EnumWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::ENUM_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
EnumWeightLevel::setEnumPolymorph(rq::EnumPolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::EnumPolymorph *
EnumWeightLevel::getEnumPolymorphPtr() const {
  return llvm::cast<rq::EnumPolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EnumPolymorph *
EnumWeightLevel::getEnumPolymorphPtr() {
  return llvm::cast<rq::EnumPolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
EnumWeightLevel::addEnumTemplate(rq::EnumTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::EnumTemplate>
EnumWeightLevel::getEnumTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::EnumTemplate>(
      rq::NextIterator<rq::Template, rq::EnumTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
    EnumWeightLevel::getEnumTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>(
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>
    EnumWeightLevel::getConstEnumTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::EnumTemplate>(
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::EnumTemplate>());
}

[[nodiscard]] inline bool
EnumWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE ClassWeightLevel::ClassWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::CLASS_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
ClassWeightLevel::setClassPolymorph(rq::ClassPolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassPolymorph *
ClassWeightLevel::getClassPolymorphPtr() const {
  return llvm::cast<rq::ClassPolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ClassPolymorph *
ClassWeightLevel::getClassPolymorphPtr() {
  return llvm::cast<rq::ClassPolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
ClassWeightLevel::addClassTemplate(rq::ClassTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Template, rq::ClassTemplate>
ClassWeightLevel::getClassTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::ClassTemplate>(
      rq::NextIterator<rq::Template, rq::ClassTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
    ClassWeightLevel::getClassTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>
    ClassWeightLevel::getConstClassTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::ClassTemplate>(
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::ClassTemplate>());
}

[[nodiscard]] inline bool
ClassWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE
LazyVariableWeightLevel::LazyVariableWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::LAZY_VARIABLE_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void LazyVariableWeightLevel::setLazyVariablePolymorph(
    rq::LazyVariablePolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::LazyVariablePolymorph *
LazyVariableWeightLevel::getLazyVariablePolymorphPtr() {
  return llvm::cast<rq::LazyVariablePolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::LazyVariablePolymorph *
LazyVariableWeightLevel::getLazyVariablePolymorphPtr() const {
  return llvm::cast<rq::LazyVariablePolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void LazyVariableWeightLevel::addLazyVariableTemplate(
    rq::LazyVariableTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariableWeightLevel::getLazyVariableTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::NextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_ptr),
      rq::NextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariableWeightLevel::getLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>
    LazyVariableWeightLevel::getConstLazyVariableTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::LazyVariableTemplate>(
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::LazyVariableTemplate>());
}

[[nodiscard]] inline bool
LazyVariableWeightLevel::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getId(rq::SymbolKind::LAZY_VARIABLE_WEIGHT_LEVEL);
}

RQ_ALWAYS_INLINE Polymorph::Polymorph(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsPolymorph(kind), "not polymorph");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation>(
      rq::NextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getConstOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::WeightLevel>
Polymorph::getWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel>(
      rq::NextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
Polymorph::getWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
Polymorph::getConstWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel>());
}

[[nodiscard]] inline bool Polymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsPolymorph();
}

template <typename WeightLevel, typename Polymorph>
[[nodiscard]] RQ_ALWAYS_INLINE WeightLevel &
addWeightLevel(rq::BumpPtrAllocator &allocator, Polymorph &polymorph,
               unsigned weight) {
  if (polymorph._first_weight_level_ptr == nullptr) {
    WeightLevel &level = allocator.allocateValue<WeightLevel>(weight);
    polymorph._first_weight_level_ptr = &level;
    return level;
  }
  WeightLevel &first = llvm::cast<WeightLevel>(
      rq::dereferencePtr(polymorph._first_weight_level_ptr));
  if (first.getWeight() < weight) {
    WeightLevel &level = allocator.allocateValue<WeightLevel>(weight);
    level._next_ptr = &first;
    polymorph._first_weight_level_ptr = &level;
    return level;
  }
  for (rq::WeightLevel &next_base : polymorph.getWeightLevelSubrange()) {
    WeightLevel &next = llvm::cast<WeightLevel>(next_base);
    if (next._next_ptr == nullptr) {
      WeightLevel &level = allocator.allocateValue<WeightLevel>(weight);
      next._next_ptr = &level;
      return level;
    }
    WeightLevel &after =
        llvm::cast<WeightLevel>(rq::dereferencePtr(next._next_ptr));
    if (after.getWeight() < weight) {
      WeightLevel &level = allocator.allocateValue<WeightLevel>(weight);
      level._next_ptr = &after;
      next._next_ptr = &level;
      return level;
    }
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE ClassPolymorph::ClassPolymorph()
    : Polymorph(rq::SymbolKind::CLASS_POLYMORPH) {}

RQ_ALWAYS_INLINE void
ClassPolymorph::addClassOverload(rq::ClassOverload &class_) {
  rq::assignSingleValue(class_._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &class_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::NextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getConstClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>());
}

[[nodiscard]] inline rq::ClassWeightLevel &
ClassPolymorph::getClassWeightLevel(rq::BumpPtrAllocator &allocator,
                                    unsigned weight) {
  return rq::addWeightLevel<rq::ClassWeightLevel>(allocator, *this, weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getClassWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::ClassWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getClassWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getConstClassWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>());
}

[[nodiscard]] inline bool
ClassPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::CLASS_POLYMORPH);
}

RQ_ALWAYS_INLINE EnumPolymorph::EnumPolymorph()
    : Polymorph(rq::SymbolKind::ENUM_POLYMORPH) {}

RQ_ALWAYS_INLINE void EnumPolymorph::addEnumOverload(rq::EnumOverload &enum_) {
  rq::assignSingleValue(enum_._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &enum_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::NextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getConstEnumOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>());
}

[[nodiscard]] inline rq::EnumWeightLevel &
EnumPolymorph::getEnumWeightLevel(rq::BumpPtrAllocator &allocator,
                                  unsigned weight) {
  return rq::addWeightLevel<rq::EnumWeightLevel>(allocator, *this, weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getEnumWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::EnumWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getEnumWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getConstEnumWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>());
}

[[nodiscard]] inline bool EnumPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUM_POLYMORPH);
}

RQ_ALWAYS_INLINE InterfacePolymorph::InterfacePolymorph()
    : Polymorph(rq::SymbolKind::INTERFACE_POLYMORPH) {}

RQ_ALWAYS_INLINE void
InterfacePolymorph::addInterfaceOverload(rq::InterfaceOverload &interface) {
  rq::assignSingleValue(interface._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &interface;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::NextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getConstInterfaceOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>());
}

[[nodiscard]] inline rq::InterfaceWeightLevel &
InterfacePolymorph::getInterfaceWeightLevel(rq::BumpPtrAllocator &allocator,
                                            unsigned weight) {
  return rq::addWeightLevel<rq::InterfaceWeightLevel>(allocator, *this, weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getInterfaceWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getInterfaceWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getConstInterfaceWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>());
}

[[nodiscard]] inline bool
InterfacePolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::INTERFACE_POLYMORPH);
}

RQ_ALWAYS_INLINE LazyVariablePolymorph::LazyVariablePolymorph()
    : Polymorph(rq::SymbolKind::LAZY_VARIABLE_POLYMORPH) {}

RQ_ALWAYS_INLINE void LazyVariablePolymorph::addLazyVariableOverload(
    rq::LazyVariableOverload &lazyvariable) {
  rq::assignSingleValue(lazyvariable._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &lazyvariable;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getConstLazyVariableOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>());
}

[[nodiscard]] inline rq::LazyVariableWeightLevel &
LazyVariablePolymorph::getLazyVariableWeightLevel(
    rq::BumpPtrAllocator &allocator, unsigned weight) {
  return rq::addWeightLevel<rq::LazyVariableWeightLevel>(allocator, *this,
                                                         weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getLazyVariableWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getLazyVariableWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getConstLazyVariableWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>());
}

[[nodiscard]] inline bool
LazyVariablePolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::LAZY_VARIABLE_POLYMORPH);
}

RQ_ALWAYS_INLINE AdapterPolymorph::AdapterPolymorph()
    : Polymorph(rq::SymbolKind::ADAPTER_POLYMORPH) {}

RQ_ALWAYS_INLINE void
AdapterPolymorph::addAdapterOverload(rq::AdapterOverload &adapter) {
  rq::assignSingleValue(adapter._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &adapter;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::NextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getConstAdapterOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>());
}

[[nodiscard]] inline rq::AdapterWeightLevel &
AdapterPolymorph::getAdapterWeightLevel(rq::BumpPtrAllocator &allocator,
                                        unsigned weight) {
  return rq::addWeightLevel<rq::AdapterWeightLevel>(allocator, *this, weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getAdapterWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::AdapterWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getAdapterWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getConstAdapterWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>());
}

[[nodiscard]] inline bool
AdapterPolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ADAPTER_POLYMORPH);
}

RQ_ALWAYS_INLINE ProcedurePolymorph::ProcedurePolymorph()
    : Polymorph(rq::SymbolKind::PROCEDURE_POLYMORPH) {}

RQ_ALWAYS_INLINE void
ProcedurePolymorph::addProcedureOverload(rq::ProcedureOverload &procedure) {
  rq::assignSingleValue(procedure._next_ptr, this->_first_overload_ptr);
  this->_first_overload_ptr = &procedure;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ProcedureOverload>
    ProcedurePolymorph::getProcedureOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ProcedureOverload>(
      rq::NextIterator<rq::Implementation, rq::ProcedureOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ProcedureOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>
    ProcedurePolymorph::getProcedureOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcedureOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcedureOverload>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>
    ProcedurePolymorph::getConstProcedureOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ProcedureOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ProcedureOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ProcedureOverload>());
}

[[nodiscard]] inline rq::ProcedureWeightLevel &
ProcedurePolymorph::getProcedureWeightLevel(rq::BumpPtrAllocator &allocator,
                                            unsigned weight) {
  return rq::addWeightLevel<rq::ProcedureWeightLevel>(allocator, *this, weight);
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
    ProcedurePolymorph::getProcedureWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
    ProcedurePolymorph::getProcedureWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>());
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>
    ProcedurePolymorph::getConstProcedureWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ProcedureWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ProcedureWeightLevel>());
}

[[nodiscard]] inline bool
ProcedurePolymorph::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PROCEDURE_POLYMORPH);
}

RQ_ALWAYS_INLINE TableMember::TableMember(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsTableMember(kind), "not table member");
}

RQ_ALWAYS_INLINE void TableMember::setContainer(rq::SymbolTable &container) {
  rq::assignSingleValue(this->_container_ptr, &container);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *TableMember::getContainerPtr() {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
TableMember::getContainerPtr() const {
  return this->_container_ptr;
}

[[nodiscard]] inline bool TableMember::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsTableMember();
}

RQ_ALWAYS_INLINE Import::Import() : Symbol(rq::SymbolKind::IMPORT) {}

RQ_ALWAYS_INLINE void
Import::setModifierFuseFlags(rq::ModifierFuseFlags flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "already set");
  this->_modifier_fuse_flags = flags;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
Import::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

RQ_ALWAYS_INLINE void Import::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Import::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *Import::getExpressionPtr() {
  return this->_expression_ptr;
}

RQ_ALWAYS_INLINE void Import::setImported(rq::Module &imported) {
  rq::assignSingleValue(this->_imported_ptr, &imported);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *
Import::getImportedPtr() const {
  return this->_imported_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module *Import::getImportedPtr() {
  return this->_imported_ptr;
}

RQ_ALWAYS_INLINE void Import::setModule(rq::Module &module) {
  rq::assignSingleValue(this->_module_ptr, &module);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Module *Import::getModulePtr() const {
  return this->_module_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Module *Import::getModulePtr() {
  return this->_module_ptr;
}

RQ_ALWAYS_INLINE void Import::addPortal(rq::Portal &portal) {
  rq::assignSingleValue(portal._next_ptr, this->_first_portal_ptr);
  this->_first_portal_ptr = &portal;
}

[[nodiscard]] rq::NextSubrange<rq::Portal> Import::getPortalSubrange() {
  return rq::NextSubrange<rq::Portal>(
      rq::NextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::NextIterator<rq::Portal>());
}

[[nodiscard]] rq::ConstNextSubrange<rq::Portal>
Import::getPortalSubrange() const {
  return rq::ConstNextSubrange<rq::Portal>(
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr));
}

[[nodiscard]] rq::ConstNextSubrange<rq::Portal>
Import::getConstPortalSubrange() const {
  return rq::ConstNextSubrange<rq::Portal>(
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr),
      rq::ConstNextIterator<rq::Portal>(this->_first_portal_ptr));
}

[[nodiscard]] inline bool Import::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::IMPORT);
}

RQ_ALWAYS_INLINE EagerDeclaration::EagerDeclaration(rq::SymbolKind kind)
    : TableMember(kind) {}

RQ_ALWAYS_INLINE void EagerDeclaration::setName(rq::Name name) {
  RQ_ASSERT(name.getIsEmpty(), "already set");
  this->_name = name;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Name EagerDeclaration::getName() const {
  return this->_name;
}

RQ_ALWAYS_INLINE void EagerDeclaration::setHost(rq::SymbolTable &host) {
  rq::assignSingleValue(this->_host_ptr, &host);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *EagerDeclaration::getHostPtr() {
  return this->_host_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
EagerDeclaration::getHostPtr() const {
  return this->_host_ptr;
}

[[nodiscard]] inline bool
EagerDeclaration::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsEagerDeclaration();
}

RQ_ALWAYS_INLINE Route::Route(rq::SymbolKind kind) : EagerDeclaration(kind) {
  RQ_ASSERT(rq::getIsRoute(kind), "not route");
}

RQ_ALWAYS_INLINE void Route::setPath(rq::Expression &path) {
  rq::assignSingleValue(this->_path_ptr, &path);
}
[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *Route::getPathPtr() const {
  return this->_path_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *Route::getPathPtr() {
  return this->_path_ptr;
}

RQ_ALWAYS_INLINE void Route::setExpression(rq::Expression &expression) {
  rq::assignSingleValue(this->_expression_ptr, &expression);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Route::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *Route::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] inline bool Route::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsRoute();
}

RQ_ALWAYS_INLINE Alias::Alias() : Route(rq::SymbolKind::ALIAS) {}

[[nodiscard]] inline bool Alias::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ALIAS);
}

RQ_ALWAYS_INLINE Portal::Portal() : Route(rq::SymbolKind::PORTAL) {}

[[nodiscard]] inline bool Portal::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::PORTAL);
}

RQ_ALWAYS_INLINE Anchor::Anchor() : EagerDeclaration(rq::SymbolKind::ANCHOR) {}

RQ_ALWAYS_INLINE void Anchor::setVessel(rq::EagerScope &scope) {
  rq::assignSingleValue(this->_vessel_ptr, &scope);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::EagerScope *Anchor::getVesselPtr() {
  return this->_vessel_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::EagerScope *
Anchor::getVesselPtr() const {
  return this->_vessel_ptr;
}

[[nodiscard]] inline bool Anchor::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ANCHOR);
}

RQ_ALWAYS_INLINE Enumerator::Enumerator()
    : EagerDeclaration(rq::SymbolKind::ENUMERATOR) {}

RQ_ALWAYS_INLINE void Enumerator::setWord(rq::ConstantWord &word) {
  rq::assignSingleValue(this->_word_ptr, &word);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantWord *Enumerator::getWordPtr() {
  return this->_word_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantWord *
Enumerator::getWordPtr() const {
  return this->_word_ptr;
}

[[nodiscard]] inline bool Enumerator::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::ENUMERATOR);
}

RQ_ALWAYS_INLINE EagerVariable::EagerVariable(rq::SymbolKind kind)
    : EagerDeclaration(kind) {
  RQ_ASSERT(rq::getIsEagerVariable(kind), "not eager variable");
}

RQ_ALWAYS_INLINE void
EagerVariable::setLowFuseFlags(rq::ModifierFuseFlags modifier_fuse_flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "already set");
  this->_modifier_fuse_flags = modifier_fuse_flags;
}
[[nodiscard]] RQ_ALWAYS_INLINE rq::ModifierFuseFlags
EagerVariable::getModifierFuseFlags() const {
  return this->_modifier_fuse_flags;
}

RQ_ALWAYS_INLINE void EagerVariable::setType(rq::ConstantSymbol &type) {
  rq::assignSingleValue(this->_type_ptr, &type);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol *EagerVariable::getTypePtr() {
  return this->_type_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol *
EagerVariable::getTypePtr() const {
  return this->_type_ptr;
}

[[nodiscard]] inline bool EagerVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsEagerVariable();
}

RQ_ALWAYS_INLINE EagerDynamicVariable::EagerDynamicVariable()
    : EagerVariable(rq::SymbolKind::DYNAMIC_EAGER_VARIABLE) {}

RQ_ALWAYS_INLINE void
EagerDynamicVariable::setLlvmValue(llvm::Value &llvm_value) {
  rq::assignSingleValue(this->_llvm_value_ptr, &llvm_value);
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::Value *
EagerDynamicVariable::getLlvmValuePtr() {
  return this->_llvm_value_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value *
EagerDynamicVariable::getLlvmValuePtr() const {
  return this->_llvm_value_ptr;
}

[[nodiscard]] inline bool
EagerDynamicVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::DYNAMIC_EAGER_VARIABLE);
}

RQ_ALWAYS_INLINE EagerStaticVariable::EagerStaticVariable()
    : EagerVariable(rq::SymbolKind::STATIC_EAGER_VARIABLE) {}

RQ_ALWAYS_INLINE void
EagerStaticVariable::setStaticValue(rq::Gendex<rq::StaticValue> static_value) {
  RQ_ASSERT(!this->_static_value.getHasData(), "already set");
  this->_static_value = static_value;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue *
EagerStaticVariable::getStaticValuePtr() {
  return &this->_static_value.getData();
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue *
EagerStaticVariable::getStaticValuePtr() const {
  return &this->_static_value.getData();
}

[[nodiscard]] inline bool
EagerStaticVariable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::STATIC_EAGER_VARIABLE);
}

RQ_ALWAYS_INLINE
SymbolTableIterator::SymbolTableIterator(rq::SymbolTable *symbol_table_ptr)
    : _symbol_table_ptr(symbol_table_ptr) {}

RQ_ALWAYS_INLINE rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  this->_symbol_table_ptr =
      rq::dereferencePtr(this->_symbol_table_ptr)._container_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::SymbolTableIterator SymbolTableIterator::operator++(int) {
  return ++*this;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableIterator::operator==(const Self &it) const {
  return this->_symbol_table_ptr == it._symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
SymbolTableIterator::operator!=(const Self &it) const {
  return this->_symbol_table_ptr != it._symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
SymbolTableIterator::operator*() {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
SymbolTableIterator::operator*() const {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *
SymbolTableIterator::operator->() {
  return this->_symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
SymbolTableIterator::operator->() const {
  return this->_symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool SymbolTableIterator::getIsDone() const {
  return this->_symbol_table_ptr == nullptr;
}

RQ_ALWAYS_INLINE
ConstSymbolTableIterator::ConstSymbolTableIterator(
    const rq::SymbolTable *symbol_table_ptr)
    : _symbol_table_ptr(symbol_table_ptr) {}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator &
ConstSymbolTableIterator::operator++() {
  this->_symbol_table_ptr =
      rq::dereferencePtr(this->_symbol_table_ptr)._conatiner_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator
ConstSymbolTableIterator::operator++(int) {
  return ++*this;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::operator==(const Self &it) const {
  return this->_symbol_table_ptr == it._symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::operator!=(const Self &it) const {
  return this->_symbol_table_ptr != it._symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
ConstSymbolTableIterator::operator*() const {
  return rq::dereferencePtr(this->_symbol_table_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
ConstSymbolTableIterator::operator->() const {
  return this->_symbol_table_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ConstSymbolTableIterator::getIsDone() const {
  return this->_symbol_table_ptr == nullptr;
}

RQ_ALWAYS_INLINE SymbolTable::SymbolTable(rq::SymbolKind kind)
    : TableMember(kind) {
  RQ_ASSERT(rq::getIsSymbolTable(kind), "not symbol table");
}

RQ_ALWAYS_INLINE void SymbolTable::setContainer(rq::SymbolTable &container) {
  rq::assignSingleValue(this->_conatiner_ptr, &container);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *SymbolTable::getContainerPtr() {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
SymbolTable::getContainerPtr() const {
  return this->_container_ptr;
}

inline void SymbolTable::addMember(rq::BumpPtrAllocator &allocator,
                                   rq::Name name, rq::TableMember &member) {
  auto [it, _] = this->_member_map.try_emplace(name);
  auto &list = it->getSecond();
  list.insertFront(allocator, member);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstNodeListRef<rq::TableMember>
SymbolTable::lookupList(rq::Name name) const {
  auto found = this->_member_map.find(name);
  if (found == this->_member_map.end()) {
    return {};
  }
  return found->getSecond();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::SymbolTableIterator>
SymbolTable::getInclusiveAscendingSubrange() {
  return rq::Subrange<rq::SymbolTableIterator>(rq::SymbolTableIterator(this),
                                               rq::SymbolTableIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
SymbolTable::getInclusiveAscendingSubrange() const {
  return rq::Subrange<rq::ConstSymbolTableIterator>(
      rq::ConstSymbolTableIterator(this), rq::ConstSymbolTableIterator());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Subrange<rq::ConstSymbolTableIterator>
SymbolTable::getConstInclusiveAscendingSubrange() const {
  return rq::Subrange<rq::ConstSymbolTableIterator>(
      rq::ConstSymbolTableIterator(this), rq::ConstSymbolTableIterator());
}

[[nodiscard]] inline bool SymbolTable::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getIsSymbolTable();
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  using MK = rq::ModuleKind;
  switch (kind) {
  case MK::NONE:
    return "none";
  case MK::SOURCE:
    return "source";
  case MK::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE ModuleDetail::ModuleDetail(rq::ModuleKind kind,
                                                     llvm::StringRef path,
                                                     llvm::StringRef buffer)
    : _module_kind(kind), _path(path), _buffer(buffer) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind
ModuleDetail::getModuleKind() const {
  return this->_module_kind;
}

RQ_ALWAYS_INLINE void
ModuleDetail::setOrChangeExpression(rq::Expression *expression_ptr) {
  this->_expression_ptr = expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
ModuleDetail::getExpressionPtr() const {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
ModuleDetail::getExpressionPtr() {
  return this->_expression_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleDetail::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef ModuleDetail::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &
ModuleDetail::getTokens() {
  return this->_tokens;
}

[[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
ModuleDetail::getTokens() const {
  return this->_tokens;
}

RQ_ALWAYS_INLINE Module::Module(rq::ModuleDetail &&detail)
    : SymbolTable(rq::SymbolKind::MODULE), _module_kind(detail.getModuleKind()),
      _expression_ptr(detail.getExpressionPtr()), _path(detail.getPath()),
      _buffer(detail.getBuffer()) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind Module::getModuleKind() const {
  return this->_module_kind;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getPath() const {
  return this->_path;
}

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef Module::getBuffer() const {
  return this->_buffer;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
Module::getExpression() const {
  return rq::dereferencePtr(this->_expression_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &Module::getExpression() {
  return rq::dereferencePtr(this->_expression_ptr);
}

RQ_ALWAYS_INLINE void Module::addImport(rq::Import &import) {
  rq::assignSingleValue(import._next_ptr, this->_first_import_ptr);
  this->_first_import_ptr = &import;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Import>
Module::getImportSubrange() {
  return rq::NextSubrange<rq::Import>(
      rq::NextIterator<rq::Import>(this->_first_import_ptr),
      rq::NextIterator<rq::Import>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
Module::getImportSubrange() const {
  return rq::ConstNextSubrange<rq::Import>(
      rq::ConstNextIterator<rq::Import>(this->_first_import_ptr),
      rq::ConstNextIterator<rq::Import>());
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Import>
Module::getConstImportSubrange() const {
  return rq::ConstNextSubrange<rq::Import>(
      rq::ConstNextIterator<rq::Import>(this->_first_import_ptr),
      rq::ConstNextIterator<rq::Import>());
}

[[nodiscard]] inline bool Module::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getId(rq::SymbolKind::MODULE);
}

} // namespace rq