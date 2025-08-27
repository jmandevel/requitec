#pragma once

namespace requite {

enum class RootType {
    SYMBOL,
    VARIADIC_ARGUMENTS,
    UNSIGNED,
    SIGNED,
    BFLOAT16,
    BINARY16,
    BINARY32,
    BINARY64,
    BINARY128,
    BOOLEAN,
    VOID,
    NULL_TYPE,
    SIGNATURE,
    RANGE,
    LAYOUT,
    OBJECT,
    VARIANT
};

[[nodiscard]] inline constexpr bool getIsAggregate(requite::RootType type);
[[nodiscard]] inline constexpr bool getIsInteger(requite::RootType type);
[[nodiscard]] inline constexpr bool getIsFloat(requite::RootType type);

}