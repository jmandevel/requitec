#include <requite/context.hpp>

namespace requite {

bool Context::evaluateConstantUnsigned(unsigned& out_unsigned, requite::Scope& scope, requite::Expression &value_expression) {
    switch (const requite::Opcode opcode = value_expression.getOpcode()) {
        case requite::Opcode::__INTEGER_LITERAL: {
            out_unsigned = 32;
        } return true;      
        case requite::Opcode::ADDRESS_DEPTH: {
            out_unsigned = this->getPointerDepth();
        } return true;
        case requite::Opcode::ADDRESS_SIZE: {
            out_unsigned = this->getPointerDepth() / 8;
        } return true;
        default:
            break;
    }
    return false;
}

}