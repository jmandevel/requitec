#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/unreachable.hpp>

namespace requite {

bool Contextualizer0::expandExpression(requite::Expression &expression) {
  if (expression.getOpcode() != requite::Opcode::_EXPAND_VALUE) {
    return true;
  }
  requite::Expression &value_expression = expression.getBranch();
  REQUITE_UNREACHABLE(); // TODO expand
  if (!this->getContext().situateTree(this->getModule(), expression)) {
    return false;
  }
  return true;
}

bool Contextualizer0::expandBranchTree(requite::Expression &expression) {
  if (!expression.getHasBranch()) {
    return true;
  }
  requite::Expression &branch = expression.getBranch();
  return this->expandExpression(branch);
}

bool Contextualizer0::expandTree(requite::Expression &expression) {
    if (!this->expandExpression(expression)) {
        return false;
    }
    if (!expression.getHasBranch()) {
        return true;
    }
    requite::Expression& branch = expression.getBranch();
    if (!this->expandForest(branch)) {
        return false;
    }
    return true;
}

bool Contextualizer0::expandForest(requite::Expression &expression) {
    if (!this->expandExpression(expression)) {
        return false;
    }
    bool is_ok = true;
    if (expression.getHasBranch()) {
        requite::Expression& branch = expression.getBranch();
        if (!this->expandForest(branch)) {
            is_ok = false;
        }
    }
    if (!expression.getHasNext()) {
        return is_ok;
    }
    requite::Expression& next = expression.getNext();
    if (!this->expandForest(next)) {
        is_ok = false;
    }
    return is_ok;
}

} // namespace requite