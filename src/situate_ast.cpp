// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/expression_walker.hpp>
#include <requite/opcode.hpp>
#include <requite/situator.hpp>
#include <requite/source.hpp>

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
  requite::Source& source = this->getModule().getSource();
  requite::Expression &base = source.getRoot();
  REQUITE_ASSERT(base.getOpcode() == requite::Opcode::MODULE);
  this->situateExpression<requite::Situation::ROOT_STATEMENT>(base);
  return this->getIsOk();
}

void Situator::insertModuleRoot() {
  requite::Source &source = this->getModule().getSource();
  if (!source.getHasRoot() ||
      source.getRoot().getOpcode() != requite::Opcode::MODULE) {
    llvm::SmallString<16> buffer;
    llvm::raw_svector_ostream buffer_stream(buffer);
    buffer.clear();
    buffer_stream << source.getPath();
    llvm::StringRef name = buffer_stream.str();
    requite::Expression &module_expression =
        requite::Expression::makeOperation(requite::Opcode::MODULE);
    module_expression.setSourceInsertedAt(source.getTextPtr());
    requite::Expression &name_expression =
        requite::Expression::makeIdentifier(name);
    name_expression.setSourceInsertedAt(source.getTextPtr());
    module_expression.setBranch(name_expression);
    if (source.getHasRoot()) {
      name_expression.setNext(source.replaceRoot(module_expression));
    } else {
      source.setRoot(module_expression);
    }
  }
  if (source.getRoot().getHasNext()) {
    requite::Expression &last_module_branch =
        this->getModule().getSource().getRoot().getLastBranch();
    last_module_branch.setNext(source.getRoot().popNext());
  }
}

} // namespace requite