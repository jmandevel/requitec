#pragma once

namespace rq {

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  switch (kind) {
  case S::NONE:
    break;

  // LITERALS
  case S::INTEGER_LITERAL_TYPE:
    return "IntegerLiteralType";
  case S::FLOAT_LITERAL_TYPE:
    return "FloatLiteralType";
  case S::STRING_LITERAL_TYPE:
    return "StringLiteralType";
  case S::CODEUNIT_LITERAL_TYPE:
    return "CodeunitLiteralType";

  // CONTEXTUAL VALUE
  case S::UNKNOWN_VALUE:
    return "UnknownValue";
  case S::VALUE_VALUE:
    return "ValueValue";
  case S::INDEX_VALUE:
    return "IndexValue";

  // CONTEXTUAL TYPE
  case S::UNKNOWN_TYPE:
    return "UnknownType";
  case S::INFERENCE_TYPE:
    return "InferenceType";
  case S::VOID_TYPE:
    return "VoidType";
  case S::NO_RETURN_TYPE:
    return "NoReturnType";

  // MODIFIER TYPES
  case S::MODIFIER_TYPE:
    return "ModifierType";

  // QUALIFIER TYPES
  case S::VAR_QUALIFIER_TYPE:
    return "VarQualifierType";
  case S::VOLATILE_QUALIFIER_TYPE:
    return "VolatileQualifierType";
  case S::ATOMIC_QUALIFIER_TYPE:
    return "AtomicQualifierType";
  case S::NULL_TERMINATE_QUALIFIER_TYPE:
    return "NullTerminateQualifierType";

  // REFLECTION TYPES
  case S::SYMBOL_TYPE:
    return "SymbolType";
  case S::SYMBOL_RANGE_TYPE:
    return "SymbolRangeType";
  case S::EXPRESSION_TYPE:
    return "ExpressionType";
  case S::EXPRESSION_RANGE_TYPE:
    return "ExpressionRangeType";

  // PLATFORM PRIMITIVE TYPES
  case S::BOOLEAN_TYPE:
    return "BooleanType";
  case S::HALF_TYPE:
    return "HalfType";
  case S::SINGLE_TYPE:
    return "SingleType";
  case S::DOUBLE_TYPE:
    return "DoubleType";
  case S::QUADRUPLE_TYPE:
    return "QuadrupleType";
  case S::SIGNED_INTEGER_TYPE:
    return "SignedIntegerType";
  case S::UNSIGNED_INTEGER_TYPE:
    return "UnsignedIntegerType";
  case S::SIGNED_INDEX_TYPE:
    return "SignedIndexType";
  case S::UNSIGNED_INDEX_TYPE:
    return "UnsignedIndexType";
  case S::SIGNED_ADDRESS_TYPE:
    return "SignedAddressType";
  case S::UNSIGNED_ADDRESS_TYPE:
    return "UnsignedAddressType";
  case S::CHAR_TYPE:
    return "CharType";

  // FITTING PRIMITIVE TYPES
  case S::FAST_SIGNED_INTEGER_TYPE:
    return "FastSignedIntegerType";
  case S::FAST_UNSIGNED_INTEGER_TYPE:
    return "FastUnsignedIntegerType";
  case S::LEAST_SIGNED_INTEGER_TYPE:
    return "LeastSignedIntegerType";
  case S::LEAST_UNSIGNED_INTEGER_TYPE:
    return "LeastUnsignedIntegerType";

  // STANDARD PRIMITIVE TYPE
  case S::BINARY16_TYPE:
    return "Binary16Type";
  case S::BINARY32_TYPE:
    return "Binary32Type";
  case S::BINARY64_TYPE:
    return "Binary64Type";
  case S::BINARY128_TYPE:
    return "Binary128Type";
  case S::BFLOAT16_TYPE:
    return "BFloat16Type";
  case S::ASCII_TYPE:
    return "AsciiType";
  case S::UTF8_TYPE:
    return "Utf8Type";

  // VARIADIC ARGUMENTS
  case S::VARIADIC_ARGUMENTS_TYPE:
    return "VariadicArgumentsType";
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return "ScaledSignedIntegerType";
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return "ScaledUnsignedIntegerType";

    // SUBTYPES
  case S::ARRAY_SUBTYPE:
    return "ArraySubtype";
  case S::GREATEST_SUBTYPE:
    return "GreatestSubtype";
  case S::REFERENCE_SUBTYPE:
    return "ReferenceSubtype";
  case S::POINTER_SUBTYPE:
    return "PointerSubtype";
  case S::SLICE_SUBTYPE:
    return "SliceSubtype";
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return "InferenceCountArraySubtype";

  // MODULES
  case S::MODULE:
    return "Module";

  // IMPORTS
  case S::IMPORT:
    return "Import";

  // CONFORMITY
  case S::CONFORMITY:
    return "Conformity";

  // ADAPTION
  case S::ADAPTION:
    return "Adaption";

  // JUXT LIST
  case S::JUXT_LIST_TYPE:
    return "JuxtListType";
  case S::JUXT_LIST_ITEM:
    return "JuxtListItem";

  // SPECIALIZATION SET ARGUMENT
  case S::SPECIALIZATION_SET_ARGUMENT:
    return "SpecializationSetArgument";

  // SPECIALIZATION SET
  case S::SPECIALIATION_SET:
    return "SpecializationSet";
  case S::ADAPTER_SPECIALIZATION_SET:
    return "AdapterSpecializationSet";
  case S::FUNCTION_SPECIALIZATION_SET:
    return "FunctionSpecializationSet";

  // ARITHMETIC SEQUENCES
  case S::ARITHMETIC_INTERVAL_TYPE:
    return "ArithmeticIntervalType";
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "InfiniteArithmeticSequenceType";
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return "FiniteArithmeticSequenceType";

  // EAGER DECLARATIONS
  case S::ANCHOR:
    return "Anchor";
  case S::ENUMERATOR:
    return "Enumerator";
  case S::EAGER_VARIABLE:
    return "EagerVariable";

  // PARAMETERS
  case S::PARAMETER:
    return "Parameter";

  // PARAMETER LISTS
  case S::SIGNATURE_TYPE:
    return "SignatureType";
  case S::LAYOUT_TYPE:
    return "LayoutType";

  // PLACEMENTS
  case S::PLACEMENT_TYPE:
    return "PlacementType";

  // COMPOSITIONS
  case S::COMPOSITION_COMPONENT:
    return "CompositionComponent";
  case S::COMPOSITION_TYPE:
    return "CompositionType";

  // SYNONYMS
  case S::SYNONYM_TYPE:
    return "SynonymType";

  // SYMBOL TABLES
  case S::C_TABLE:
    return "CTable";
  case S::TOP_TABLE:
    return "TopTable";

  // EAGER STATEMENTS
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

  // NAMED TABLE
  case S::NAMESPACE:
    return "Namespace";

  // OVERLOADS
  case S::CLASS_OVERLOAD:
    return "ClassOverload";
  case S::ENUM_OVERLOAD:
    return "EnumOverload";
  case S::INTERFACE_OVERLOAD:
    return "InterfaceOverload";
  case S::ADAPTER_OVERLOAD:
    return "AdapterOverload";
  case S::FUNCTION_OVERLOAD:
    return "FunctionOverload";
  case S::LAZY_VARIABLE_OVERLOAD:
    return "LazyVariableOverload";

  // SPECIALIZATIONS
  case S::CLASS_SPECIALIZATION:
    return "ClassSpecialization";
  case S::ENUM_SPECIALIZATION:
    return "EnumSpecialization";
  case S::INTERFACE_SPECIALIZATION:
    return "InterfaceSpecialization";
  case S::ADAPTER_SPECIALIZATION:
    return "AdapterSpecialization";
  case S::FUNCTION_SPECIALIZATION:
    return "FunctionSpecialization";
  case S::LAZY_VARIABLE_SPECIALIZATION:
    return "LazyVariableSpecialization";

  // TEMPLATES
  case S::CLASS_TEMPLATE:
    return "ClassTemplate";
  case S::ENUM_TEMPLATE:
    return "EnumTemplate";
  case S::INTERFACE_TEMPLATE:
    return "InterfaceTemplate";
  case S::ADAPTER_TEMPLATE:
    return "AdapterTemplate";
  case S::FUNCTION_TEMPLATE:
    return "FunctionTemplate";
  case S::LAZY_VARIABLE_TEMPLATE:
    return "LazyVariableTemplate";

  // POLYMORPHS
  case S::CLASS_POLYMORPH:
    return "ClassPolymorph";
  case S::ENUM_POLYMORPH:
    return "EnumPolymorph";
  case S::INTERFACE_POLYMORPH:
    return "InterfacePolymorph";
  case S::ADAPTER_POLYMORPH:
    return "AdapterPolymorph";
  case S::FUNCTION_POLYMORPH:
    return "FunctionPolymorph";
  case S::LAZY_VARIABLE_POLYMORPH:
    return "LazyVariablePolymorph";

  // WEIGHT LEVELS
  case S::CLASS_WEIGHT_LEVEL:
    return "ClassWeightLevel";
  case S::ENUM_WEIGHT_LEVEL:
    return "EnumWeightLevel";
  case S::INTERFACE_WEIGHT_LEVEL:
    return "InterfaceWeightLevel";
  case S::ADAPTER_WEIGHT_LEVEL:
    return "AdapterWeightLevel";
  case S::FUNCTION_WEIGHT_LEVEL:
    return "FunctionWeightLevel";
  case S::LAZY_VARIABLE_WEIGHT_LEVEL:
    return "LazyVariableWeightLevel";

  case S::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolInfoFlags
getInfoFlags(rq::SymbolKind kind) {
  using S = rq::SymbolKind;
  using SIF = rq::SymbolInfoFlags;
  switch (kind) {
  case S::NONE:
    break;
  case S::INTEGER_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::FLOAT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::STRING_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::CODEUNIT_LITERAL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::LITERAL | SIF::IS_TYPE;
  case S::UNKNOWN_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::VALUE_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::INDEX_VALUE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_VALUE;
  case S::UNKNOWN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::INFERENCE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::VOID_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::NO_RETURN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::CONTEXTUAL_TYPE | SIF::IS_TYPE;
  case S::MODIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::VAR_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::VOLATILE_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::ATOMIC_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::NULL_TERMINATE_QUALIFIER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::QUALIFIER_TYPE | SIF::IS_TYPE;
  case S::SYMBOL_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE | SIF::IS_TYPE;
  case S::SYMBOL_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE | SIF::IS_TYPE;
  case S::EXPRESSION_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE | SIF::IS_TYPE;
  case S::EXPRESSION_RANGE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::REFLECTION_TYPE | SIF::IS_TYPE;
  case S::BOOLEAN_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE;
  case S::HALF_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SINGLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::DOUBLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::QUADRUPLE_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE;
  case S::SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_INDEX_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SIGNED_ADDRESS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_INDEX_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::UNSIGNED_ADDRESS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::CHAR_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::PLATFORM_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_CODEUNIT_TYPE;
  case S::FAST_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::FAST_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::LEAST_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::LEAST_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::FITTING_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::BINARY16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY32_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY64_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BINARY128_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BINARY_TYPE;
  case S::BFLOAT16_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_FLOAT_TYPE | SIF::IS_BFLOAT_TYPE;
  case S::ASCII_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_CODEUNIT_TYPE;
  case S::UTF8_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::STANDARD_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_CODEUNIT_TYPE;
  case S::VARIADIC_ARGUMENTS_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::IS_TYPE;
  case S::SCALED_SIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_SIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::SCALED_UNSIGNED_INTEGER_TYPE:
    return SIF::SIMPLE_SYMBOL | SIF::SCALED_PRIMITIVE_TYPE | SIF::IS_TYPE |
           SIF::IS_UNSIGNED_TYPE | SIF::IS_INTEGER_TYPE;
  case S::ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::GREATEST_SUBTYPE:
    return SIF::SUBTYPE | SIF::IS_TYPE;
  case S::REFERENCE_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_TYPE;
  case S::POINTER_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_TYPE;
  case S::SLICE_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_TYPE;
  case S::INFERENCE_COUNT_ARRAY_SUBTYPE:
    return SIF::SUBTYPE | SIF::SIMPLE_SUBTYPE | SIF::IS_TYPE;
  case S::MODULE:
    return SIF::NONE;
  case S::IMPORT:
    return SIF::NONE;
  case S::CONFORMITY:
    return SIF::NONE;
  case S::ADAPTION:
    return SIF::NONE;
  case S::JUXT_LIST_TYPE:
    return SIF::IS_TYPE;
  case S::JUXT_LIST_ITEM:
    return SIF::NONE;
  case S::SPECIALIZATION_SET_ARGUMENT:
    return SIF::NONE;
  case S::SPECIALIATION_SET:
    return SIF::SPECIALIZATION_SET;
  case S::ADAPTER_SPECIALIZATION_SET:
    return SIF::SPECIALIZATION_SET;
  case S::FUNCTION_SPECIALIZATION_SET:
    return SIF::SPECIALIZATION_SET;
  case S::ARITHMETIC_INTERVAL_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::INFINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::FINITE_ARITHMETIC_SEQUENCE_TYPE:
    return SIF::ARITHMETIC_SEQUENCE_TYPE | SIF::IS_TYPE;
  case S::ANCHOR:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION;
  case S::ENUMERATOR:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION;
  case S::EAGER_VARIABLE:
    return SIF::TABLE_MEMBER | SIF::EAGER_DECLARATION;
  case S::PARAMETER:
    return SIF::PARAMETER_LIST;
  case S::SIGNATURE_TYPE:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;
  case S::LAYOUT_TYPE:
    return SIF::PARAMETER_LIST | SIF::IS_TYPE;
  case S::PLACEMENT_TYPE:
    return SIF::IS_TYPE;
  case S::COMPOSITION_COMPONENT:
    return SIF::NONE;
  case S::COMPOSITION_TYPE:
    return SIF::IS_TYPE;
  case S::SYNONYM_TYPE:
    return SIF::IS_TYPE;
  case S::STELLARSCOPE_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;
  case S::C_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE;
  case S::TOP_TABLE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::IS_FRAME_SCOPE;
  case S::IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::ELSE_IF_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::ELSE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::SWITCH_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::CASE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::DEFAULT_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::FOR_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::WHILE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::SPIN_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::WEAVE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::SCOPE_STATEMENT:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::EAGER_SCOPE;
  case S::NAMESPACE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE;
  case S::CLASS_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::CLASS_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ENUM_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::ENUM_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::INTERFACE_IMPLEMENTATION | SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::ADAPTER_IMPLEMENTATION | SIF::IS_OBJECT_SCOPE;
  case S::FUNCTION_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::FUNCTION_IMPLEMENTATION | SIF::IS_FRAME_SCOPE;
  case S::LAZY_VARIABLE_OVERLOAD:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::LAZY_VARIABLE_IMPLEMENTATION;
  case S::CLASS_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::CLASS_IMPLEMENTATION | SIF::IS_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::ENUM_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::ENUM_IMPLEMENTATION | SIF::IS_TYPE |
           SIF::IS_OBJECT_SCOPE;
  case S::INTERFACE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::INTERFACE_IMPLEMENTATION |
           SIF::IS_TYPE | SIF::IS_OBJECT_SCOPE;
  case S::ADAPTER_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::ADAPTER_IMPLEMENTATION |
           SIF::IS_OBJECT_SCOPE;
  case S::FUNCTION_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::FUNCTION_IMPLEMENTATION |
           SIF::IS_FRAME_SCOPE;
  case S::LAZY_VARIABLE_SPECIALIZATION:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::IMPLEMENTATION |
           SIF::SPECIALIZATION_SET | SIF::LAZY_VARIABLE_IMPLEMENTATION;
  case S::CLASS_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::ENUM_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::INTERFACE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::ADAPTER_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::FUNCTION_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE | SIF::IS_FRAME_SCOPE;
  case S::LAZY_VARIABLE_TEMPLATE:
    return SIF::TABLE_MEMBER | SIF::SYMBOL_TABLE | SIF::NAMED_TABLE |
           SIF::LAZY_DECLARATION | SIF::TEMPLATE;
  case S::CLASS_POLYMORPH:
    return SIF::POLYMORPH;
  case S::ENUM_POLYMORPH:
    return SIF::POLYMORPH;
  case S::INTERFACE_POLYMORPH:
    return SIF::POLYMORPH;
  case S::ADAPTER_POLYMORPH:
    return SIF::POLYMORPH;
  case S::FUNCTION_POLYMORPH:
    return SIF::POLYMORPH;
  case S::LAZY_VARIABLE_POLYMORPH:
    return SIF::POLYMORPH;
  case S::CLASS_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::ENUM_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::INTERFACE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::ADAPTER_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::FUNCTION_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::LAZY_VARIABLE_WEIGHT_LEVEL:
    return SIF::WEIGHT_LEVEL;
  case S::LAST:
    break;
  }
  return SIF::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSymbol(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SIMPLE_SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextual(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::CONTEXTUAL_TYPE |
                                   rq::SymbolInfoFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsContextualValue(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CONTEXTUAL_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsReflectionType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::REFLECTION_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags,
                        rq::SymbolInfoFlags::FITTING_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE |
                            rq::SymbolInfoFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsStandardPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::STANDARD_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PLATFORM_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsAttributeType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ATTRIBUTE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledPrimitiveType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SCALED_PRIMITIVE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSubtype(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SIMPLE_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ARITHMETIC_SEQUENCE_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSpecializationSet(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SPECIALIZATION_SET);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterList(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::PARAMETER_LIST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableMember(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TABLE_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerDeclaration(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::EAGER_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPolymorph(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::POLYMORPH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsWeightLevel(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::WEIGHT_LEVEL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEagerScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::EAGER_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamedTable(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::NAMED_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLazyDeclarataion(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::LAZY_DECLARATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsClassImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::CLASS_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsEnumImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ENUM_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsInterfaceImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::INTERFACE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsAdapterImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::ADAPTER_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsFunctionImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::FUNCTION_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsLazyVariableImplementation(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags,
                       rq::SymbolInfoFlags::LAZY_VARIABLE_IMPLEMENTATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumericType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasSome(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE |
                                   rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_SIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_UNSIGNED_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_INTEGER_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BINARY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloatType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_BFLOAT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitType(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_CODEUNIT_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFrameScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_FRAME_SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsObjectScope(rq::SymbolKind kind) {
  const rq::SymbolInfoFlags flags = rq::getInfoFlags(kind);
  return rq::getHasAll(flags, rq::SymbolInfoFlags::IS_OBJECT_SCOPE);
}

RQ_ALWAYS_INLINE Symbol::Symbol(rq::SymbolKind kind)
    : rq::Entity(rq::getSymbolId(kind)) {}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind Symbol::getKind() const {
  return static_cast<rq::SymbolKind>(this->_id - rq::SYMBOL_OFFSET);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
Symbol::getDerivedExpressionPtr() {
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *
Symbol::getDerivedExpressionPtr() const {
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] inline rq::SymbolInfoFlags Symbol::getInfoFlags() const {
  return rq::getInfoFlags(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSimpleSymbol() const {
  return rq::getIsSimpleSymbol(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextual() const {
  return rq::getIsContextual(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualType() const {
  return rq::getIsContextualType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsContextualValue() const {
  return rq::getIsContextualValue(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLiteralType() const {
  return rq::getIsLiteralType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsReflectionType() const {
  return rq::getIsReflectionType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPrimitiveType() const {
  return rq::getIsPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFittingPrimitiveType() const {
  return rq::getIsFittingPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsStandardPrimitiveType() const {
  return rq::getIsStandardPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPlatformPrimitiveType() const {
  return rq::getIsPlatformPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsAttributeType() const {
  return rq::getIsAttributeType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsScaledPrimitiveType() const {
  return rq::getIsScaledPrimitiveType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSubtype() const {
  return rq::getIsSubtype(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSimpleSubtype() const {
  return rq::getIsSimpleSubtype(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsArithmeticSequenceType() const {
  return rq::getIsArithmeticSequenceType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSpecializationSet() const {
  return rq::getIsSpecializationSet(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsParameterList() const {
  return rq::getIsParameterList(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTableMember() const {
  return rq::getIsTableMember(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsEagerDeclaration() const {
  return rq::getIsEagerDeclaration(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsPolymorph() const {
  return rq::getIsPolymorph(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsWeightLevel() const {
  return rq::getIsWeightLevel(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsTemplate() const {
  return rq::getIsTemplate(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSymbolTable() const {
  return rq::getIsSymbolTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsEagerScope() const {
  return rq::getIsEagerScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNamedTable() const {
  return rq::getIsNamedTable(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsLazyDeclarataion() const {
  return rq::getIsLazyDeclarataion(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsImplementation() const {
  return rq::getIsImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsClassImplementation() const {
  return rq::getIsClassImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsEnumImplementation() const {
  return rq::getIsEnumImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsInterfaceImplementation() const {
  return rq::getIsInterfaceImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsAdapterImplementation() const {
  return rq::getIsAdapterImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsFunctionImplementation() const {
  return rq::getIsFunctionImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
Symbol::getIsLazyVariableImplementation() const {
  return rq::getIsLazyVariableImplementation(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsType() const {
  return rq::getIsType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsNumericType() const {
  return rq::getIsNumericType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsSignedType() const {
  return rq::getIsSignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsUnsignedType() const {
  return rq::getIsUnsignedType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsIntegerType() const {
  return rq::getIsIntegerType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFloatType() const {
  return rq::getIsFloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBinaryType() const {
  return rq::getIsBinaryType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsBfloatType() const {
  return rq::getIsBfloatType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsCodeunitType() const {
  return rq::getIsCodeunitType(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsFrameScope() const {
  return rq::getIsFrameScope(this->getKind());
}

[[nodiscard]] RQ_ALWAYS_INLINE bool Symbol::getIsObjectScope() const {
  return rq::getIsObjectScope(this->getKind());
}

[[nodiscard]] inline bool Symbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  const rq::EntityId id = entity.getId();
  return id >= rq::SYMBOL_OFFSET && id < rq::CONSTANT_OFFSET;
}

RQ_ALWAYS_INLINE SimpleSymbol::SimpleSymbol(rq::SymbolKind kind)
    : Symbol(kind) {
  RQ_ASSERT(rq::getIsSimpleSymbol(kind), "not simple symbol");
}

[[nodiscard]] inline bool SimpleSymbol::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsSimpleSymbol(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Literal::Literal(rq::SymbolKind kind) : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsLiteralType(kind), "not literal");
}

[[nodiscard]] inline bool Literal::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsLiteralType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE IntegerLiteral::IntegerLiteral()
    : Literal(rq::SymbolKind::INTEGER_LITERAL_TYPE) {}

[[nodiscard]] inline bool
IntegerLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::INTEGER_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE FloatLiteral::FloatLiteral()
    : Literal(rq::SymbolKind::FLOAT_LITERAL_TYPE) {}

[[nodiscard]] inline bool FloatLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::FLOAT_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE StringLiteral::StringLiteral()
    : Literal(rq::SymbolKind::STRING_LITERAL_TYPE) {}

[[nodiscard]] inline bool StringLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::STRING_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE CodeunitLiteral::CodeunitLiteral()
    : Literal(rq::SymbolKind::CODEUNIT_LITERAL_TYPE) {}

[[nodiscard]] inline bool
CodeunitLiteral::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::CODEUNIT_LITERAL_TYPE);
}

RQ_ALWAYS_INLINE Contextual::Contextual(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsContextual(kind), "not contextual");
}

[[nodiscard]] inline bool Contextual::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextual(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ContextualType::ContextualType(rq::SymbolKind kind)
    : Contextual(kind) {
  RQ_ASSERT(rq::getIsContextualType(kind), "not contextual type");
}

[[nodiscard]] inline bool
ContextualType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextualType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE InferenceType::InferenceType()
    : ContextualType(rq::SymbolKind::INFERENCE_TYPE) {}

[[nodiscard]] inline bool InferenceType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::INFERENCE_TYPE);
}

RQ_ALWAYS_INLINE VoidType::VoidType()
    : ContextualType(rq::SymbolKind::VOID_TYPE) {}

[[nodiscard]] inline bool VoidType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::VOID_TYPE);
}

RQ_ALWAYS_INLINE NoReturnType::NoReturnType()
    : ContextualType(rq::SymbolKind::NO_RETURN_TYPE) {}

[[nodiscard]] inline bool NoReturnType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::NO_RETURN_TYPE);
}

RQ_ALWAYS_INLINE UnknownType::UnknownType()
    : ContextualType(rq::SymbolKind::UNKNOWN_TYPE) {}

[[nodiscard]] inline bool UnknownType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::UNKNOWN_TYPE);
}

RQ_ALWAYS_INLINE ContextualValue::ContextualValue(rq::SymbolKind kind)
    : Contextual(kind) {
  RQ_ASSERT(rq::getIsContextualValue(kind), "not contextual value");
}

[[nodiscard]] inline bool
ContextualValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsContextualValue(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ValueValue::ValueValue()
    : ContextualValue(rq::SymbolKind::VALUE_VALUE) {}

[[nodiscard]] inline bool ValueValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::VALUE_VALUE);
}

RQ_ALWAYS_INLINE IndexValue::IndexValue()
    : ContextualValue(rq::SymbolKind::INDEX_VALUE) {}

[[nodiscard]] inline bool IndexValue::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::INDEX_VALUE);
}

RQ_ALWAYS_INLINE ReflectionType::ReflectionType(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsReflectionType(kind), "not reflection type");
}

[[nodiscard]] inline bool
ReflectionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsReflectionType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE ExpressionType::ExpressionType()
    : ReflectionType(rq::SymbolKind::EXPRESSION_TYPE) {}

[[nodiscard]] inline bool
ExpressionType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::EXPRESSION_TYPE);
}

RQ_ALWAYS_INLINE SymbolRangeType::SymbolRangeType()
    : ReflectionType(rq::SymbolKind::SYMBOL_RANGE_TYPE) {}

[[nodiscard]] inline bool
SymbolRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SYMBOL_RANGE_TYPE);
}

RQ_ALWAYS_INLINE ExpressionRangeType::ExpressionRangeType()
    : ReflectionType(rq::SymbolKind::EXPRESSION_RANGE_TYPE) {}

[[nodiscard]] inline bool
ExpressionRangeType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::EXPRESSION_RANGE_TYPE);
}

RQ_ALWAYS_INLINE PrimitiveType::PrimitiveType(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsPrimitiveType(kind), "not primitive type");
}

[[nodiscard]] inline bool PrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsPrimitiveType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE FittingPrimitiveType::FittingPrimitiveType(rq::SymbolKind kind)
    : PrimitiveType(kind) {
  RQ_ASSERT(rq::getIsFittingPrimitiveType(kind), "not fitting primitive type");
}

[[nodiscard]] inline bool
FittingPrimitiveType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsFittingPrimitiveType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE FastSignedIntegerType::FastSignedIntegerType()
    : FittingPrimitiveType(rq::SymbolKind::FAST_SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
FastSignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::FAST_SIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE FastUnsignedIntegerType::FastUnsignedIntegerType()
    : FittingPrimitiveType(rq::SymbolKind::FAST_UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
FastUnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::FAST_UNSIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE LeastSignedIntegerType::LeastSignedIntegerType()
    : FittingPrimitiveType(rq::SymbolKind::LEAST_SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
LeastSignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::LEAST_SIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE LeastUnsignedIntegerType::LeastUnsignedIntegerType()
    : FittingPrimitiveType(rq::SymbolKind::LEAST_UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
LeastUnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::LEAST_UNSIGNED_INTEGER_TYPE);
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
  return rq::getIsStandardPrimitiveType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE Binary16Type::Binary16Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY16_TYPE) {}

[[nodiscard]] inline bool Binary16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY16_TYPE);
}

RQ_ALWAYS_INLINE Binary32Type::Binary32Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY32_TYPE) {}

[[nodiscard]] inline bool Binary32Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY32_TYPE);
}

RQ_ALWAYS_INLINE Binary64Type::Binary64Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY64_TYPE) {}

[[nodiscard]] inline bool Binary64Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY64_TYPE);
}

RQ_ALWAYS_INLINE Binary128Type::Binary128Type()
    : StandardPrimitiveType(rq::SymbolKind::BINARY128_TYPE) {}

[[nodiscard]] inline bool Binary128Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BINARY128_TYPE);
}

RQ_ALWAYS_INLINE BFloat16Type::BFloat16Type()
    : StandardPrimitiveType(rq::SymbolKind::BFLOAT16_TYPE) {}

[[nodiscard]] inline bool BFloat16Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BFLOAT16_TYPE);
}

RQ_ALWAYS_INLINE AsciiType::AsciiType()
    : StandardPrimitiveType(rq::SymbolKind::ASCII_TYPE) {}

[[nodiscard]] inline bool AsciiType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::ASCII_TYPE);
}

RQ_ALWAYS_INLINE Utf8Type::Utf8Type()
    : StandardPrimitiveType(rq::SymbolKind::UTF8_TYPE) {}

[[nodiscard]] inline bool Utf8Type::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::UTF8_TYPE);
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
  return rq::getIsPlatformPrimitiveType(entity.getUnsafeSymbolKind());
}

RQ_ALWAYS_INLINE HalfType::HalfType()
    : PlatformPrimitiveType(rq::SymbolKind::HALF_TYPE) {}

[[nodiscard]] inline bool HalfType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::HALF_TYPE);
}

RQ_ALWAYS_INLINE SingleType::SingleType()
    : PlatformPrimitiveType(rq::SymbolKind::SINGLE_TYPE) {}

[[nodiscard]] inline bool SingleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SINGLE_TYPE);
}

RQ_ALWAYS_INLINE DoubleType::DoubleType()
    : PlatformPrimitiveType(rq::SymbolKind::DOUBLE_TYPE) {}

[[nodiscard]] inline bool DoubleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::DOUBLE_TYPE);
}

RQ_ALWAYS_INLINE QuadrupleType::QuadrupleType()
    : PlatformPrimitiveType(rq::SymbolKind::QUADRUPLE_TYPE) {}

[[nodiscard]] inline bool DoubleType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::QUADRUPLE_TYPE);
}

RQ_ALWAYS_INLINE BooleanType::BooleanType()
    : PlatformPrimitiveType(rq::SymbolKind::BOOLEAN_TYPE) {}

[[nodiscard]] inline bool BooleanType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::BOOLEAN_TYPE);
}

RQ_ALWAYS_INLINE UnsignedIntegerType::UnsignedIntegerType()
    : PlatformPrimitiveType(rq::SymbolKind::UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
UnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::UNSIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE SignedIntegerType::SignedIntegerType()
    : PlatformPrimitiveType(rq::SymbolKind::SIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
SignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE UnsignedAddressType::UnsignedAddressType()
    : PlatformPrimitiveType(rq::SymbolKind::UNSIGNED_INTEGER_TYPE) {}

[[nodiscard]] inline bool
UnsignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() ==
         rq::getSymbolId(rq::SymbolKind::UNSIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE SignedAddressType::SignedAddressType()
    : PlatformPrimitiveType(rq::SymbolKind::SIGNED_ADDRESS_TYPE) {}

[[nodiscard]] inline bool
SignedAddressType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_ADDRESS_TYPE);
}

RQ_ALWAYS_INLINE CharType::CharType()
    : PlatformPrimitiveType(rq::SymbolKind::CHAR_TYPE) {}

[[nodiscard]] inline bool CharType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::CHAR_TYPE);
}

RQ_ALWAYS_INLINE QualifierType::QualifierType(rq::SymbolKind kind)
    : SimpleSymbol(kind) {
  RQ_ASSERT(rq::getIsAttributeType(kind), "not qualifier type");
}

[[nodiscard]] inline bool QualifierType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsAttributeType(entity.getUnsafeSymbolKind());
}

// TODO

RQ_ALWAYS_INLINE SymbolType::SymbolType()
    : ReflectionType(rq::SymbolKind::SYMBOL_TYPE) {}

[[nodiscard]] inline bool SymbolType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SYMBOL_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &
Subtype::getChild() const {
  return rq::dereferencePtr(this->_child_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &Subtype::getChild() {
  return rq::dereferencePtr(this->_child_ptr);
}

RQ_ALWAYS_INLINE ArraySubtype::ArraySubtype(std::size_t count,
                                            rq::ConstantSymbol &child)
    : Subtype(rq::SymbolKind::ARRAY_SUBTYPE, child), _count(count) {}

[[nodiscard]] RQ_ALWAYS_INLINE std::size_t ArraySubtype::getCount() const {
  return this->_count;
}

inline void ArraySubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  return rq::profileArraySubtype(inout_id, this->getChild(), this->getCount());
}

RQ_ALWAYS_INLINE void profileArraySubtype(llvm::FoldingSetNodeID &inout_id,
                                          const rq::ConstantSymbol &child,
                                          std::size_t count) {
  inout_id.AddPointer(&child);
  inout_id.AddInteger(count);
}

RQ_ALWAYS_INLINE SimpleSubtype::SimpleSubtype(rq::SymbolKind kind,
                                              rq::ConstantSymbol &child)
    : Subtype(kind, child) {}

inline void SimpleSubtype::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileSimpleSubtype(inout_id, this->getKind(), this->getChild());
}

RQ_ALWAYS_INLINE void profileSimpleSubtype(llvm::FoldingSetNodeID &inout_id,
                                           rq::SymbolKind kind,
                                           const rq::ConstantSymbol &child) {
  RQ_ASSERT(rq::getIsSimpleSubtype(kind), "not simple subtype");
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(&child);
}

RQ_ALWAYS_INLINE ReferenceSubtype::ReferenceSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::REFERENCE_SUBTYPE, child) {}

RQ_ALWAYS_INLINE PointerSubtype::PointerSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::POINTER_SUBTYPE, child) {}

RQ_ALWAYS_INLINE InferenceCountArraySubtype::InferenceCountArraySubtype(
    rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::INFERENCE_COUNT_ARRAY_SUBTYPE, child) {}

RQ_ALWAYS_INLINE SliceSubtype::SliceSubtype(rq::ConstantSymbol &child)
    : SimpleSubtype(rq::SymbolKind::SLICE_SUBTYPE, child) {}

RQ_ALWAYS_INLINE IntegerType::IntegerType(rq::SymbolKind kind, unsigned scale,
                                          std::uint64_t synonym_id)
    : Symbol(kind), _scale(scale), _synonym_id(synonym_id) {}

[[nodiscard]] RQ_ALWAYS_INLINE bool IntegerType::getIsSizeScaled() const {
  return this->_scale == rq::IntegerType::SIZE_SCALE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool IntegerType::getIsIndexScaled() const {
  return this->_scale == rq::IntegerType::INDEX_SCALE;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned IntegerType::getScale() const {
  return this->_scale;
}

[[nodiscard]] RQ_ALWAYS_INLINE unsigned IntegerType::getSynonymId() const {
  return this->_synonym_id;
}

[[nodiscard]] inline bool IntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return rq::getIsIntegerType(entity.getUnsafeSymbolKind());
}

inline void IntegerType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileIntegerType(inout_id, this->getKind(), this->getScale(),
                         this->getSynonymId());
}

RQ_ALWAYS_INLINE void profileIntegerType(llvm::FoldingSetNodeID &inout_id,
                                         rq::SymbolKind kind, unsigned scale,
                                         unsigned synonym_id) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddInteger(scale);
  inout_id.AddInteger(scale);
}

[[nodiscard]] inline bool SignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::SIGNED_INTEGER_TYPE);
}

[[nodiscard]] inline bool UnsignedIntegerType::classof(const rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  return entity.getId() == rq::getSymbolId(rq::SymbolKind::UNSIGNED_INTEGER_TYPE);
}

RQ_ALWAYS_INLINE Adaption::Adaption(rq::InterfaceImplementation &interface,
                                    rq::AdapterImplementation &adapter)
    : Symbol(rq::SymbolKind::ADAPTION), _interface_ptr(&interface),
      _adapter_ptr(&adapter) {}

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

inline void Adaption::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileAdaption(inout_id, this->getInterface(), this->getAdapter());
}

void profileAdaption(llvm::FoldingSetNodeID &inout_id,
                     const rq::InterfaceImplementation &interface,
                     const rq::AdapterImplementation &adapter) {
  inout_id.AddPointer(&interface);
  inout_id.AddPointer(&adapter);
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

inline void Conformity::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileConformity(inout_id, this->getInterface(), this->getAdapter());
}

void profileConformity(llvm::FoldingSetNodeID &inout_id,
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

inline void JuxtListItem::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileJuxtListItem(inout_id, this->_next_ptr, this->getType());
}

RQ_ALWAYS_INLINE JuxtListType::JuxtListType(rq::JuxtListItem &first)
    : Symbol(rq::SymbolKind::JUXT_LIST_TYPE), _first_ptr(&first) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::JuxtListItem &
JuxtListType::getFirst() const {
  return rq::dereferencePtr(this->_first_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::JuxtListItem &JuxtListType::getFirst() {
  return rq::dereferencePtr(this->_first_ptr);
}

inline void JuxtListType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileJuxtListType(inout_id, this->_first_ptr);
}

void profileJuxtListType(llvm::FoldingSetNodeID &inout_id,
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

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  using MK = rq::ModuleKind;
  switch (kind) {
  case MK::NONE:
    break;
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
    : Symbol(rq::SymbolKind::MODULE), _module_kind(detail.getModuleKind()),
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

[[nodiscard]] RQ_ALWAYS_INLINE bool
Module::getImportsModule(const rq::Module &module) const {
  return this->_imported_ptrs.contains(&module);
}

void Module::addImport(rq::Import &import) {
  this->_imported_ptrs.insert(import.getModulePtr());
  import._next_ptr = this->_first_ptr;
  this->_first_ptr = &import;
}

RQ_ALWAYS_INLINE
ArithmeticSequenceType::ArithmeticSequenceType(
    rq::SymbolKind kind, rq::ConstantSymbol &child,
    rq::ArithmeticCondition condition, rq::ArithmeticStep step)
    : Symbol(kind), _child_ptr(&child), _condition(condition), _step(step) {
  RQ_ASSERT(rq::getIsArithmeticSequenceType(kind),
            "not arithmetic sequence type");
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

RQ_ALWAYS_INLINE Import::Import() : Symbol(rq::SymbolKind::IMPORT) {}

RQ_ALWAYS_INLINE void
Import::setModifierFuseFlags(rq::ModifierFuseFlags flags) {
  RQ_ASSERT(this->_modifier_fuse_flags == rq::ModifierFuseFlags::NONE,
            "flags already set");
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
  this->_imported_ptr = &imported;
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

[[nodiscard]] rq::Module *Import::getModulePtr() { return this->_module_ptr; }

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

inline void Parameter::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileParameter(inout_id, this->getKind(), this->_next_ptr,
                       this->getName(), this->getType(),
                       this->getModifierFuseFlags(), this->getParameterFlags(),
                       this->getDefaultPtr());
}

inline void profileParameter(llvm::FoldingSetNodeID &inout_id,
                             rq::SymbolKind kind, const rq::Parameter *next_ptr,
                             rq::Name name, const rq::ConstantSymbol &type,
                             rq::ModifierFuseFlags modifier_fuse_flags,
                             rq::ParameterInfoFlags param_flags,
                             const rq::Entity *default_ptr) {
  inout_id.AddInteger(rq::getUnderlyingValue(kind));
  inout_id.AddPointer(next_ptr);
  inout_id.Add(name);
  inout_id.AddPointer(&type);
  inout_id.AddInteger(rq::getUnderlyingValue(modifier_fuse_flags));
  inout_id.AddInteger(rq::getUnderlyingValue(param_flags));
  if (default_ptr == nullptr) {
    inout_id.AddPointer(nullptr);
    return;
  }
  const rq::Entity &entity = rq::dereferencePtr(default_ptr);
  if (llvm::isa<rq::Constant>(entity)) {
    const rq::Constant &constant = llvm::cast<rq::Constant>(entity);
    inout_id.AddPointer(&constant);
    return;
  } else if (llvm::isa<rq::Instruction>(entity)) {
    const rq::Instruction &instruction = llvm::cast<rq::Instruction>(entity);
    inout_id.AddPointer(&instruction);
    return;
  }
  RQ_UNREACHABLE();
}

RQ_ALWAYS_INLINE
CompositionComponent::CompositionComponent(rq::CompositionComponent *next_ptr)
    : Symbol(rq::SymbolKind::COMPOSITION_COMPONENT), _next_ptr(next_ptr) {}

inline void
CompositionComponent::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileCompositionComponent(inout_id, this->_next_ptr);
}

RQ_ALWAYS_INLINE void
profileCompositionComponent(llvm::FoldingSetNodeID &inout_id,
                            rq::CompositionComponent *next_ptr) {
  inout_id.AddPointer(next_ptr);
}

RQ_ALWAYS_INLINE
CompositionType::CompositionType(rq::CompositionComponent *first_ptr)
    : Symbol(rq::SymbolKind::COMPOSITION_TYPE), _first_ptr(first_ptr) {}

inline void CompositionType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileCompositionType(inout_id, this->_first_ptr);
}

RQ_ALWAYS_INLINE void
profileCompositionType(llvm::FoldingSetNodeID &inout_id,
                       const rq::CompositionComponent *first_ptr) {
  inout_id.AddPointer(first_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
ParameterListDetail::getFoundPositionalParametersEnd() const {
  return rq::getHasAll(this->_found_parameter_marks,
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
  RQ_ASSERT(!this->getFoundPositionalParametersEnd(),
            "unhandled duplicate positional end");
  RQ_ASSERT(this->getFoundLockedParametersBegin(), "unhandled locked");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::POSITIONAL;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendNonpositionalParametersBegin() {
  RQ_ASSERT(!this->getFoundNonpositionalParametersBegin(),
            "unhandled duplicate nonpositional begin");
  RQ_ASSERT(this->getFoundLockedParametersBegin(), "unhandled locked");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::NONPOSITIONAL;
}

RQ_ALWAYS_INLINE void ParameterListDetail::appendLockedParametersBegin() {
  RQ_ASSERT(!this->getFoundLockedParametersBegin(),
            "unhandled duplicate locked begin");
  this->_found_parameter_marks |= rq::ParameterInfoFlags::LOCKED;
}

RQ_ALWAYS_INLINE void
ParameterListDetail::appendParameter(rq::Name name, rq::ConstantSymbol &type,
                                     rq::ModifierFuseFlags modifier_fuse_flags,
                                     rq::Entity *default_ptr) {
  RQ_ASSERT(this->_flags != rq::ParameterInfoFlags::NONE,
            "unhandled no parameter flags set");
  this->_parameter_detail_list.emplace_back(name, type, modifier_fuse_flags,
                                            this->_flags, default_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE std::span<const rq::ParameterDetail>
ParameterListDetail::getParameterDetailSpan() const {
  return this->_parameter_detail_list;
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
      rq::ConstNextIterator<rq::Parameter>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Parameter>
ParameterList::getConstParameterSubrange() const {
  return rq::ConstNextSubrange<rq::Parameter>(
      rq::ConstNextIterator<rq::Parameter>(this->_first_ptr),
      rq::ConstNextIterator<rq::Parameter>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Parameter>
ParameterList::getParameterSubrange() {
  return rq::NextSubrange<rq::Parameter>(
      rq::NextIterator<rq::Parameter>(this->_first_ptr),
      rq::NextIterator<rq::Parameter>(nullptr));
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

inline void LayoutType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profileLayoutType(inout_id, this->_first_ptr);
}

RQ_ALWAYS_INLINE void
profileLayoutType(llvm::FoldingSetNodeID &inout_id,
                  const rq::Parameter *first_parameter_ptr) {
  inout_id.AddPointer(first_parameter_ptr);
}

RQ_ALWAYS_INLINE
PlacementType::PlacementType(rq::FunctionImplementation &function)
    : Symbol(rq::SymbolKind::PLACEMENT_TYPE), _function_ptr(&function) {}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionImplementation &
PlacementType::getFunction() const {
  return rq::dereferencePtr(this->_function_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionImplementation &
PlacementType::getFunction() {
  return rq::dereferencePtr(this->_function_ptr);
}

inline void PlacementType::Profile(llvm::FoldingSetNodeID &inout_id) const {
  rq::profilePlacementType(inout_id, this->getFunction());
}

RQ_ALWAYS_INLINE void
profilePlacementType(llvm::FoldingSetNodeID &inout_id,
                     const rq::FunctionImplementation &function) {
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
  rq::NextSubrange<rq::Template>(
      rq::NextIterator<rq::Template>(this->_first_ptr),
      rq::NextIterator<rq::Template>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
WeightLevel::getTemplateSubrange() const {
  rq::ConstNextSubrange<rq::Template>(
      rq::ConstNextIterator<rq::Template>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Template>
WeightLevel::getConstTemplateSubrange() const {
  rq::ConstNextSubrange<rq::Template>(
      rq::ConstNextIterator<rq::Template>(this->_first_ptr),
      rq::ConstNextIterator<rq::Template>(nullptr));
}

RQ_ALWAYS_INLINE InterfaceWeightLevel::InterfaceWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::INTERFACE_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
InterfaceWeightLevel::setInterfacePolymorph(rq::InterfacePolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::InterfacePolymorph *
InterfaceWeightLevel::getInterfacePolymorphPtr() const {
  return llvm::cast_or_null<rq::InterfacePolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::InterfacePolymorph *
InterfaceWeightLevel::getInterfacePolymorphPtr() {
  return llvm::cast_or_null<rq::InterfacePolymorph>(this->_polymorph_ptr);
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
      rq::NextIterator<rq::Template, rq::InterfaceTemplate>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfaceWeightLevel::getInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>
    InterfaceWeightLevel::getConstInterfaceTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::InterfaceTemplate>(
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::InterfaceTemplate>(nullptr));
}

RQ_ALWAYS_INLINE FunctionWeightLevel::FunctionWeightLevel(unsigned weight)
    : WeightLevel(rq::SymbolKind::FUNCTION_WEIGHT_LEVEL, weight) {}

RQ_ALWAYS_INLINE void
FunctionWeightLevel::setFunctionPolymorph(rq::FunctionPolymorph &polymorph) {
  rq::assignSingleValue(this->_polymorph_ptr, &polymorph);
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::FunctionPolymorph *
FunctionWeightLevel::getFunctionPolymorphPtr() const {
  return llvm::cast_or_null<rq::FunctionPolymorph>(this->_polymorph_ptr);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::FunctionPolymorph *
FunctionWeightLevel::getFunctionPolymorphPtr() {
  return llvm::cast_or_null<rq::FunctionPolymorph>(this->_polymorph_ptr);
}

RQ_ALWAYS_INLINE void
FunctionWeightLevel::addFunctionTemplate(rq::FunctionTemplate &template_) {
  template_._next_ptr = this->_first_ptr;
  this->_first_ptr = &template_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Template, rq::FunctionTemplate>
    FunctionWeightLevel::getFunctionTemplateSubrange() {
  return rq::NextSubrange<rq::Template, rq::FunctionTemplate>(
      rq::NextIterator<rq::Template, rq::FunctionTemplate>(this->_first_ptr),
      rq::NextIterator<rq::Template, rq::FunctionTemplate>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>
    FunctionWeightLevel::getFunctionTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>(
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>
    FunctionWeightLevel::getConstFunctionTemplateSubrange() const {
  return rq::ConstNextSubrange<rq::Template, rq::FunctionTemplate>(
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>(
          this->_first_ptr),
      rq::ConstNextIterator<rq::Template, rq::FunctionTemplate>(nullptr));
}

RQ_ALWAYS_INLINE Polymorph::Polymorph(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsPolymorph(kind), "not kind");
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation>(
      rq::NextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::Implementation>
Polymorph::getConstOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation>(
      rq::ConstNextIterator<rq::Implementation>(this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::NextSubrange<rq::WeightLevel>
Polymorph::getWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel>(
      rq::NextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
Polymorph::getWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNextSubrange<rq::WeightLevel>
Polymorph::getConstWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel>(this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel>(nullptr));
}

RQ_ALWAYS_INLINE ClassPolymorph::ClassPolymorph(rq::SymbolKind kind)
    : Polymorph(kind) {}

RQ_ALWAYS_INLINE void
ClassPolymorph::addClassOverload(rq::ClassOverload &class_) {
  class_._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &class_;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() {
  return rq::NextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::NextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::ClassOverload>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>
    ClassPolymorph::getConstClassOverloadSubrange() const {
  return rq::ConstNextSubrange<rq::Implementation, rq::ClassOverload>(
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::ClassOverload>(nullptr));
}

RQ_ALWAYS_INLINE void
ClassPolymorph::addClassWeightLevel(rq::ClassWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getClassWeightLevelSubrange() {
  return rq::NextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::NextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::ClassWeightLevel>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getClassWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(nullptr));
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>
    ClassPolymorph::getConstClassWeightLevelSubrange() const {
  return rq::ConstNextSubrange<rq::WeightLevel, rq::ClassWeightLevel>(
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::ClassWeightLevel>(nullptr));
}

[[nodiscard]] inline bool ClassPolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::CLASS_POLYMORPH;
}

RQ_ALWAYS_INLINE EnumPolymorph::EnumPolymorph(rq::SymbolKind kind)
    : Polymorph(kind) {}
RQ_ALWAYS_INLINE void
EnumPolymorph::addEnumOverload(rq::EnumOverload &overload) {
  overload._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &overload;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() {
  return {rq::NextIterator<rq::Implementation, rq::EnumOverload>(
              this->_first_overload_ptr),
          rq::NextIterator<rq::Implementation, rq::EnumOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::EnumOverload>
    EnumPolymorph::getEnumOverloadConstSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::EnumOverload>(nullptr)};
}

RQ_ALWAYS_INLINE void
EnumPolymorph::addEnumWeightLevel(rq::EnumWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getEnumWeightLevelSubrange() {
  return {rq::NextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
              this->_first_weight_level_ptr),
          rq::NextIterator<rq::WeightLevel, rq::EnumWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getEnumWeightLevelSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::EnumWeightLevel>
    EnumPolymorph::getEnumWeightLevelConstSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::EnumWeightLevel>(nullptr)};
}

[[nodiscard]] inline bool EnumPolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::ENUM_POLYMORPH;
}

RQ_ALWAYS_INLINE InterfacePolymorph::InterfacePolymorph(rq::SymbolKind kind)
    : Polymorph(kind) {}
RQ_ALWAYS_INLINE void
InterfacePolymorph::addInterfaceOverload(rq::InterfaceOverload &overload) {
  overload._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &overload;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() {
  return {rq::NextIterator<rq::Implementation, rq::InterfaceOverload>(
              this->_first_overload_ptr),
          rq::NextIterator<rq::Implementation, rq::InterfaceOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getInterfaceOverloadSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
              nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::InterfaceOverload>
    InterfacePolymorph::getConstInterfaceOverloadSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::InterfaceOverload>(
              nullptr)};
}

RQ_ALWAYS_INLINE void InterfacePolymorph::addInterfaceWeightLevel(
    rq::InterfaceWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getInterfaceWeightLevelSubrange() {
  return {rq::NextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
              this->_first_weight_level_ptr),
          rq::NextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getInterfaceWeightLevelSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
              nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::InterfaceWeightLevel>
    InterfacePolymorph::getConstInterfaceWeightLevelSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::InterfaceWeightLevel>(
              nullptr)};
}

[[nodiscard]] inline bool InterfacePolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::INTERFACE_POLYMORPH;
}

RQ_ALWAYS_INLINE LazyVariablePolymorph::LazyVariablePolymorph()
    : Polymorph(rq::SymbolKind::LAZY_VARIABLE_POLYMORPH) {}
RQ_ALWAYS_INLINE void LazyVariablePolymorph::addLazyVariableOverload(
    rq::LazyVariableOverload &overload) {
  overload._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &overload;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() {
  return {
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>(
          this->_first_overload_ptr),
      rq::NextIterator<rq::Implementation, rq::LazyVariableOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
              nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::LazyVariableOverload>
    LazyVariablePolymorph::getLazyVariableOverloadConstSubrange() const {
  return {rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
              this->_first_overload_ptr),
          rq::ConstNextIterator<rq::Implementation, rq::LazyVariableOverload>(
              nullptr)};
}

RQ_ALWAYS_INLINE void LazyVariablePolymorph::addLazyVariableWeightLevel(
    rq::LazyVariableWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getLazyVariableWeightLevelSubrange() {
  return {
      rq::NextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
          this->_first_weight_level_ptr),
      rq::NextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getLazyVariableWeightLevelSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
              nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::LazyVariableWeightLevel>
    LazyVariablePolymorph::getLazyVariableWeightLevelConstSubrange() const {
  return {rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
              this->_first_weight_level_ptr),
          rq::ConstNextIterator<rq::WeightLevel, rq::LazyVariableWeightLevel>(
              nullptr)};
}

[[nodiscard]] inline bool
LazyVariablePolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::LAZY_VARIABLE_POLYMORPH;
}

RQ_ALWAYS_INLINE AdapterPolymorph::AdapterPolymorph()
    : Polymorph(rq::SymbolKind::ADAPTER_POLYMORPH) {}
RQ_ALWAYS_INLINE void
AdapterPolymorph::addAdapterOverload(rq::AdapterOverload &overload) {
  overload._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &overload;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() {
  return {rq::NextIterator<rq::Implementation, rq::AdapterOverload>(
              this->_first_overload_ptr),
          rq::NextIterator<rq::Implementation, rq::AdapterOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getAdapterOverloadSubrange() const {
  return {
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::AdapterOverload>
    AdapterPolymorph::getConstAdapterOverloadSubrange() const {
  return {
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::AdapterOverload>(nullptr)};
}

RQ_ALWAYS_INLINE void
AdapterPolymorph::addAdapterWeightLevel(rq::AdapterWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getAdapterWeightLevelSubrange() {
  return {rq::NextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
              this->_first_weight_level_ptr),
          rq::NextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getAdapterWeightLevelSubrange() const {
  return {
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::AdapterWeightLevel>
    AdapterPolymorph::getConstAdapterWeightLevelSubrange() const {
  return {
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::AdapterWeightLevel>(nullptr)};
}

[[nodiscard]] inline bool AdapterPolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::ADAPTER_POLYMORPH;
}

RQ_ALWAYS_INLINE FunctionPolymorph::FunctionPolymorph()
    : Polymorph(rq::SymbolKind::FUNCTION_POLYMORPH) {}
RQ_ALWAYS_INLINE void
FunctionPolymorph::addFunctionOverload(rq::FunctionOverload &overload) {
  overload._next_ptr = this->_first_overload_ptr;
  this->_first_overload_ptr = &overload;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::Implementation, rq::FunctionOverload>
    FunctionPolymorph::getFunctionOverloadSubrange() {
  return {rq::NextIterator<rq::Implementation, rq::FunctionOverload>(
              this->_first_overload_ptr),
          rq::NextIterator<rq::Implementation, rq::FunctionOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::FunctionOverload>
    FunctionPolymorph::getFunctionOverloadSubrange() const {
  return {
      rq::ConstNextIterator<rq::Implementation, rq::FunctionOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::FunctionOverload>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::Implementation, rq::FunctionOverload>
    FunctionPolymorph::getConstFunctionOverloadSubrange() const {
  return {
      rq::ConstNextIterator<rq::Implementation, rq::FunctionOverload>(
          this->_first_overload_ptr),
      rq::ConstNextIterator<rq::Implementation, rq::FunctionOverload>(nullptr)};
}

RQ_ALWAYS_INLINE void FunctionPolymorph::addFunctionWeightLevel(
    rq::FunctionWeightLevel &weight_level) {
  weight_level._next_ptr = this->_first_weight_level_ptr;
  this->_first_weight_level_ptr = &weight_level;
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::NextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
    FunctionPolymorph::getFunctionWeightLevelSubrange() {
  return {rq::NextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(
              this->_first_weight_level_ptr),
          rq::NextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
    FunctionPolymorph::getFunctionWeightLevelSubrange() const {
  return {
      rq::ConstNextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(nullptr)};
}

[[nodiscard]] RQ_ALWAYS_INLINE
    rq::ConstNextSubrange<rq::WeightLevel, rq::FunctionWeightLevel>
    FunctionPolymorph::getConstFunctionWeightLevelSubrange() const {
  return {
      rq::ConstNextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(
          this->_first_weight_level_ptr),
      rq::ConstNextIterator<rq::WeightLevel, rq::FunctionWeightLevel>(nullptr)};
}

[[nodiscard]] inline bool FunctionPolymorph::classof(rq::Entity *entity_ptr) {
  const rq::Entity &entity = rq::dereferencePtr(entity_ptr);
  if (!llvm::isa<rq::Symbol>(entity)) {
    return false;
  }
  return llvm::cast<const rq::Symbol>(entity).getKind() ==
         rq::SymbolKind::FUNCTION_POLYMORPH;
}

RQ_ALWAYS_INLINE TableMember::TableMember(rq::SymbolKind kind) : Symbol(kind) {
  RQ_ASSERT(rq::getIsTableMember(kind), "not table member");
}

RQ_ALWAYS_INLINE void TableMember::setContainer(rq::SymbolTable &container) {
  this->_container_ptr = &container;
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *TableMember::getContainerPtr() {
  return this->_container_ptr;
}

[[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *
TableMember::getContainerPtr() const {
  return this->_container_ptr;
}

RQ_ALWAYS_INLINE EagerDeclaration::EagerDeclaration(rq::SymbolKind kind)
    : TableMember(kind) {
  RQ_ASSERT(rq::getIsEagerDeclaration(kind), "not eager declaration");
}

RQ_ALWAYS_INLINE void EagerDeclaration::setName(rq::Name name) {
  RQ_ASSERT(this->getName().getIsEmpty(), "name already set");
  RQ_ASSERT(!name.getIsEmpty(), "name empty");
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

} // namespace rq