#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/unreachable.hpp>

namespace requite {

bool Contextualizer0::expandMacroExpression(requite::Expression &expression) {
  if (expression.getOpcode() != requite::Opcode::_EXPAND_MACRO_VALUE) {
    return true;
  }
  requite::Expression &value_expression = expression.getBranch();
  REQUITE_UNREACHABLE(); // TODO expand
  if (!this->getContext().situateTree(this->getModule(), expression)) {
    return false;
  }
  return true;
}

bool Contextualizer0::expandMacroBranchTree(requite::Expression &expression) {
  if (!expression.getHasBranch()) {
    return true;
  }
  requite::Expression &branch = expression.getBranch();
  return this->expandMacroExpression(branch);
}

bool Contextualizer0::expandMacroTree(requite::Expression &expression) {
    if (!this->expandMacroExpression(expression)) {
        return false;
    }
    if (!expression.getHasBranch()) {
        return true;
    }
    requite::Expression& branch = expression.getBranch();
    if (!this->expandMacroForest(branch)) {
        return false;
    }
    return true;
}

bool Contextualizer0::expandMacroForest(requite::Expression &expression) {
    if (!this->expandMacroExpression(expression)) {
        return false;
    }
    bool is_ok = true;
    if (expression.getHasBranch()) {
        requite::Expression& branch = expression.getBranch();
        if (!this->expandMacroForest(branch)) {
            is_ok = false;
        }
    }
    if (!expression.getHasNext()) {
        return is_ok;
    }
    requite::Expression& next = expression.getNext();
    if (!this->expandMacroForest(next)) {
        is_ok = false;
    }
    return is_ok;
}

} // namespace requite