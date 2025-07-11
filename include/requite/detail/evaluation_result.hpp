#pragma once

namespace requite {

constexpr bool getIsGenerated(requite::EvaluationResult result) {
  switch (result) {
  case requite::EvaluationResult::LITERAL:
    return false;
  case requite::EvaluationResult::GENERATED:
    return true;
  case requite::EvaluationResult::GENERATED_NOT_DONE:
    return false;
  case requite::EvaluationResult::ERROR:
    return true;
  }
  return false;
}

} // namespace requite