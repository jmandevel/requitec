#pragma once

namespace rq {

[[nodiscard]] RQ_ALWAYS_INLINE rq::OpcodeFlags getFlags(rq::Opcode opcode) {
    using O = rq::Opcode;
    using OF = rq::OpcodeFlags;
    switch (opcode) {
        case O::NONE:
            return OF::NONE;

        // LITERALS
        case O::SY_INTEGER_LITERAL:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
        case O::SY_FLOAT_LITERAL:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
        case O::SY_STRING_LITERAL:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;
        case O::SY_CODEUNIT_LITERAL:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_LITERAL;

        // CONTEXTUAL NAME
        case O::SY_NO_NAME:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL;

        // CONTEXTUAL VALUE
        case O::SY_OUT:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_THIS:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_RESULT:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_VALUE:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_INDEX:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_DISCRIMINANT:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_COMMAND_LINE_ARGUMENTS:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;
        case O::SY_CALLSITE:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_VALUE;

        // CONTEXTUAL TYPE
        case O::SY_INFERENCE:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;
        case O::SY_VOID:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;
        case O::SY_NO_RETURN:
            return OF::SYMBOL | OF::SY_SIMPLE_SYMBOL | OF::SY_CONTEXTUAL_TYPE;

        // EXPRESSION ATTRIBUTES
        case O::SY_LABELING:
            return ;
        case O::SY_VISIBILITY:
            return ;
        case O::SY_SCOPING:
            return ;
        case O::SY_AVAILABILITY:
            return ;
        case O::SY_PROPERTY_MUTABILITY:
            return ;
        case O::SY_EXPORTING:
            return ;
        case O::SY_GENERATION_TIME:
            return ;
        case O::SY_CAPTURING:
            return ;
        case O::SY_EVALUATION_TIME:
            return ;
        case O::SY_INLINING:
            return ;
        case O::SY_MANGLING:
            return ;
        case O::SY_PACKING:
            return ;
        case O::SY_TEMPLATING:
            return ;
        case O::SY_LIKELYHOOD:
            return ;
        case O::SY_SUPPORT:
            return ;
        case O::SY_ADDRESS_STABILITY:
            return ;
        case O::SY_VARIADICNESS:
            return ;

        // TYPE ATTRIBUTES
        case O::SY_VARIABILITY:
            return ;
        case O::SY_VOLATILITY:
            return ;
        case O::SY_ATOMICITY:
            return ;
        case O::SY_NULL_TERMINATION:
            return ;
        case O::SY_PRECONDITION:
            return ;
        case O::SY_POSTCONDITION:
            return ;

        // REFLECTIVE
        case O::SY_SYMBOL:
            return ;
        case O::SY_EXPRESSION:
            return ;

        // UNSCALED PRIMITIVE
        case O::SY_BOOLEAN:
            return ;
        case O::SY_HALF:
            return ;
        case O::SY_SINGLE:
            return ;
        case O::SY_DOUBLE:
            return ;
        case O::SY_QUADRUPLE:
            return ;
        case O::SY_BINARY16:
            return ;
        case O::SY_BINARY32:
            return ;
        case O::SY_BINARY64:
            return ;
        case O::SY_BINARY128:
            return ;
        case O::SY_BFLOAT16:
            return ;
        case O::SY_ASCII:
            return ;
        case O::SY_UTF8:
            return ;
        case O::SY_SIGNED_INTEGER:
            return ;
        case O::SY_UNSIGNED_INTEGER:
            return ;
        case O::SY_SIGNED_INDEX:
            return ;
        case O::SY_UNSIGNED_INDEX:
            return ;
        case O::SY_SIGNED_ADDRESS:
            return ;
        case O::SY_UNSIGNED_ADDRESS:
            return ;

        // VARIADIC ARGUMENTS
        case O::SY_VARIADIC_ARGUMENTS:
            return ;

        // SCALED INTEGERS
        case O::SY_SCALED_SIGNED_INTEGER:
            return ;
        case O::SY_SCALED_UNSIGNED_INTEGER:
            return ;

        // UNCOUNTED SUBTYPES
        case O::SY_REFERENCE:
            return ;
        case O::SY_POINTER:
            return ;
        case O::SY_FAT_POINTER:
            return ;
        case O::SY_INFERENCE_COUNT_ARRAY:
            return ;

        // ARRAY
        case O::SY_ARRAY:
            return ;

        // MODULES
        case O::SY_MODULE:
            return ;

        // IMPORTS
        case O::SY_IMPORT:
            return ;

        // CONCATENATED STRING
        case O::SY_CONCATENATED_STRING:
            return ;

        // ARITHMETIC SEQUENCES
        case O::SY_ARITHMETIC_INTERVAL:
            return ;
        case O::SY_INFINITE_ARITHMETIC_SEQUENCE:
            return ;
        case O::SY_FINITE_ARITHMETIC_SEQUENCE:
            return ;

        // LOCAL VARIABLES
        case O::SY_LOCAL_DYNAMIC_VARIABLE:
            return ;
        case O::SY_LOCAL_STATIC_VARIABLE:
            return ;

        // PARAMETERS => local variable
        case O::SY_STATIC_PARAMETER:
            return ;
        case O::SY_DYNAMIC_PARAMETER:
            return ;

        // PARAMETER LISTS
        case O::SY_SIGNATURE:
            return ;
        case O::SY_LAYOUT:
            return ;

        // PLACEMENTS
        case O::SY_PLACEMENT:
            return ;

        // COMPOSITIONS
        case O::SY_COMPOSITION:
            return ;

        // SYNONYMS
        case O::SY_SYNONYM:
            return ;

        // POLYMORPHS
        case O::SY_RANGER_POLYMORPH:
            return ;
        case O::SY_PROCEDURE_POLYMORPH:
            return ;
        case O::SY_CLASS_POLYMORPH:
            return ;
        case O::SY_ENUMERATION_POLYMORPH:
            return ;
        case O::SY_INTERFACE_POLYMORPH:
            return ;
        case O::SY_GLOBAL_DYNAMIC_VARIABLE_POLYMORPH:
            return ;
        case O::SY_GLOBAL_STATIC_VARIABLE_POLYMORPH:
            return ;

        // SYMBOL TABLES
        case O::SY_TOP:
            return ;
        case O::SY_SCOPE:
            return ;

        // GLOBAL DECLARATION => symbol table
        case O::SY_NAMESPACE:
            return ;
        case O::SY_CLASS:
            return ;
        case O::SY_ENUMERATION:
            return ;
        case O::SY_ENUMERATOR:
            return ;
        case O::SY_INTERFACE:
            return ;

        // GLOBAL VARIABLE => global declaration => symbol table
        case O::SY_GLOBAL_DYNAMIC_VARIABLE:
            return ;
        case O::SY_GLOBAL_STATIC_VARIABLE:
            return ;

        // RANGERS => global declaration => symbol table
        case O::SY_FORWARD_RANGER:
            return ;
        case O::SY_BACKWARD_RANGER:
            return ;

        // DESTRUCTOR => global declaration
        case O::SY_DESTRUCTOR:
            return ;

        // PROCEDURES => global declaration => symbol table
        case O::SY_ENTRY:
            return ;
        case O::SY_FUNCTION:
            return ;
        case O::SY_METHOD:
            return ;
        case O::SY_EXTENSION_FUNCTION:
            return ;
        case O::SY_EXTENSION_METHOD:
            return ;

        // TEMPLATES
        case O::SY_CLASS_TEMPLATE:
            return ;
        case O::SY_ENUMERATION_TEMPLATE:
            return ;
        case O::SY_INTERFACE_TEMPLATE:
            return ;
        case O::SY_GLOBAL_DYNAMIC_VARIABLE_TEMPLATE:
            return ;
        case O::SY_GLOBAL_STATIC_VARIABLE_TEMPLATE:
            return ;
        case O::SY_FORWARD_RANGER_TEMPLATE:
            return ;
        case O::SY_BACKWARD_RANGER_TEMPLATE:
            return ;
        case O::SY_FUNCTION_TEMPLATE:
            return ;
        case O::SY_METHOD_TEMPLATE:
            return ;
        case O::SY_EXTENSION_FUNCTION_TEMPLATE:
            return ;
        case O::SY_EXTENSION_METHOD_TEMPLATE:
            return ;

        case O::CT_INTEGER:
            return ;
        case O::CT_FLOAT:
            return ;
        case O::CT_EXPRESSION:
            return ;
        case O::CT_SYMBOL:
            return ;
        case O::CT_BOOLEAN:
            return ;
        case O::CT_STRING:
            return ;
        case O::CT_ARRAY:
            return ;
        case O::CT_EXPRESSION_ATTRIBUTE:
            return ;
        case O::CT_TYPE_ATTRIBUTE:
            return ;
    }
    RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode) {
    const rq::OpcodeFlags flags = rq::getFlags(opcode);
    return rq::getHasAll(flags, rq::OpcodeFlags::SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode) {
    const rq::OpcodeFlags flags = rq::getFlags(opcode);
    return rq::getHasAll(flags, rq::OpcodeFlags::CONSTANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode) {
    const rq::OpcodeFlags flags = rq::getFlags(opcode);
    return rq::getHasNone(flags, rq::OpcodeFlags::INSTRUCTION_NONE_MASK);
}

}