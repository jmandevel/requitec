// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/module.hpp>
#include <requite/opcode.hpp>
#include <requite/situator.hpp>

#include <llvm/Support/Path.h>

namespace requite {

bool Context::situateAst(requite::Module &module) {
  requite::Situator situator(*this, module);
  if (!situator.situateAst()) {
    return false;
  }
  return true;
}

bool Situator::situateAst() {
  this->insertModuleRoot();
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(root.getOpcode() == requite::Opcode::_MODULE_ROOT);
  this->situateExpression<requite::Situation::ROOT_STATEMENT>(root);
  return this->getIsOk();
}

void Situator::insertModuleRoot() {
  requite::Module &module = this->getModule();
  if (!module.getHasExpression()) {
    requite::Expression &root_expression =
        requite::Expression::makeOperation(requite::Opcode::_MODULE_ROOT);
    requite::Expression &name_expression = requite::Expression::makeString(
        this->getContext().saveString(module.getName()));
    root_expression.setBranch(name_expression);
    module.setExpression(root_expression);
    return;
  }
  requite::Expression &root_expression = module.getExpression();
  if (root_expression.getOpcode() != requite::Opcode::_MODULE_ROOT) {
    requite::Expression &new_root_expression =
        requite::Expression::makeOperation(requite::Opcode::_MODULE_ROOT);
    requite::Expression &name_expression = requite::Expression::makeString(
        this->getContext().saveString(module.getName()));
    new_root_expression.setBranch(name_expression);
    name_expression.setNext(root_expression);
    module.changeExpression(new_root_expression);
    return;
  }
  requite::Expression &branch = root_expression.getBranch();
  if (requite::getCanBeStringLiteral(branch.getOpcode())) {
    branch.changeDataText(this->getContext().saveString(module.getName()));
  }
  // if branch is not situation, error is caught in situateExpression() later
}

} // namespace requite