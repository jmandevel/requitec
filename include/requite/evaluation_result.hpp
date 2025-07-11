#pragma once

namespace requite {

enum class EvaluationResult {
    LITERAL,
    GENERATED,
    GENERATED_NOT_DONE,
    ERROR
};

[[nodiscard]] constexpr bool getIsGenerated(requite::EvaluationResult result);

}

#include <requite/detail/evaluation_result.hpp>