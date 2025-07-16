#pragma once

namespace requite {

enum class BlockType {
    NONE,
    IF,
    ELSE_IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT_CASE,
    FOR,
    WHILE,
    DO_WHILE,
    LOOP,
    SCOPE
};

}