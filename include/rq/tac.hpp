#pragma once

#include <cstdint>

namespace rq {

enum class Opcode : std::uint32_t {
    NONE,
    // TODO
};

struct Instruction final {
    using Self = rq::Instruction;

    rq::Opcode _opcode = rq::Opcode::NONE;
    std::uint32_t a;
    std::uint32_t b;
    std::uint32_t c;

    // TODO
};

}