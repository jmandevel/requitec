#pragma once

#include <cstdint>

namespace rq {

enum class Opcode : std::uint32_t {
    NONE,
    MAKE_TEMP,
    ASSIGN_CONST,
    ASSIGN_VAR,
    ADD_CONST,
    ADD_VAR,
    SUBTRACT_CONST,
    SUBTRACT_VAR,
    MULTIPLY_CONST,
    MULTIPLY_VAR,
    DIVIDE_CONST,
    DIVIDE_VAR,
    MODULUS_CONST,
    MODULUS_VAR,
    PUSH_CONST,
    PUSH_VAR,
    POP,
    CALL,
    RETURN
};

struct Instruction final {
    using Self = rq::Instruction;

    rq::Opcode _opcode = rq::Opcode::NONE;
    std::uint32_t a;
    std::uint32_t b;

    // TODO
};

}