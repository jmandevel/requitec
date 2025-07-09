#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/expression.hpp>
#include <requite/unreachable.hpp>

namespace requite {

bool Contextualizer0::expandTree(requite::Expression &expression,
                                 requite::Situation situation,
                                 requite::LogMode log_mode) {
  return true; // TODO
}

bool Contextualizer0::expandForest(requite::Expression &expression,
                                   requite::Situation situation,
                                   requite::LogMode log_mode) {
  return true; // TODO
}

bool Contextualizer0::expandNameBranch(requite::Expression &expression,
                                       requite::Situation situation,
                                       requite::LogMode log_mode) {
  return true; // TODO
}

} // namespace requite