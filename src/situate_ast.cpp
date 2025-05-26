// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/expression_walker.hpp>
#include <requite/module.hpp>
#include <requite/opcode.hpp>
#include <requite/situator.hpp>

namespace requite {

bool Context::situateAst(requite::Module &module) {
  requite::Situator Situator(*this, module);
  const bool is_ok = Situator.situateAst();
  return is_ok;
}

Situator::Situator(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module), _is_ok(true) {}

requite::Context &Situator::getContext() { return this->_context_ref.get(); }

const requite::Context &Situator::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Situator::getModule() { return this->_module_ref.get(); }

const requite::Module &Situator::getModule() const {
  return this->_module_ref.get();
}

void Situator::setNotOk() { this->_is_ok = false; }

bool Situator::getIsOk() const { return this->_is_ok; }

void Situator::setIsOk() { this->_is_ok = true; }

bool Situator::situateAst() {
  this->insertModuleRoot();
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(root.getOpcode() == requite::Opcode::MODULE);
  this->situateExpression<requite::Situation::ROOT_STATEMENT>(root);
  return this->getIsOk();
}

void Situator::insertModuleRoot() {
  requite::Module &module = this->getModule();
  if (!module.getHasExpression() ||
      module.getExpression().getOpcode() != requite::Opcode::MODULE) {
    llvm::SmallString<16> buffer;
    llvm::raw_svector_ostream buffer_stream(buffer);
    buffer.clear();
    buffer_stream << module.getPath();
    llvm::StringRef name = buffer_stream.str();
    requite::Expression &module_expression =
        requite::Expression::makeOperation(requite::Opcode::MODULE);
    module_expression.setSourceInsertedAt(module.getTextPtr());
    requite::Expression &name_expression =
        requite::Expression::makeIdentifier(name);
    name_expression.setSourceInsertedAt(module.getTextPtr());
    module_expression.setBranch(name_expression);
    if (module.getHasExpression()) {
      name_expression.setNext(module.replaceExpression(module_expression));
    } else {
      module.setExpression(module_expression);
    }
  }
  if (module.getExpression().getHasNext()) {
    requite::Expression &last_module_branch =
        this->getModule().getExpression().getLastBranch();
    last_module_branch.setNext(module.getExpression().popNext());
  }
}

} // namespace requite