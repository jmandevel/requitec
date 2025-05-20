// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attributes.hpp>
#include <requite/symbol.hpp>

namespace requite {

Symbol::Symbol(requite::RootSymbol &root) : _root(root) {}

Symbol::Symbol(requite::RootSymbol &root,
               llvm::ArrayRef<requite::SubSymbol> subs)
    : _root(root), _subs(subs.begin(), subs.end()) {}

bool Symbol::operator==(const requite::Symbol &rhs) const {
  return rhs._root == this->_root && rhs._subs == this->_subs;
}

bool Symbol::operator!=(const requite::Symbol &rhs) const {
  return rhs._root != this->_root || rhs._subs != this->_subs;
}

bool Symbol::getIsEmpty() const {
  return this->_root.getIsNone() &&
         !this->_root_attributes.getHasAnyAttribute() && this->_subs.empty();
}

requite::RootSymbol &Symbol::getRoot() { return this->_root; }

const requite::RootSymbol &Symbol::getRoot() const { return this->_root; }

requite::Attributes &Symbol::getRootAttributes() {
  return this->_root_attributes;
}

const requite::Attributes &Symbol::getRootAttributes() const {
  return this->_root_attributes;
}

std::vector<requite::SubSymbol> &Symbol::getSubs() { return this->_subs; }

const std::vector<requite::SubSymbol> &Symbol::getSubs() const {
  return this->_subs;
}

void Symbol::wrapSymbol(const requite::Symbol &symbol) {
  REQUITE_ASSERT(this->getRoot().getIsNone());
  for (const requite::SubSymbol &sub : symbol.getSubs()) {
    this->getSubs().emplace_back(sub);
  }
  this->getRoot() = requite::RootSymbol(symbol.getRoot());
}

void Symbol::applyAttributes(const requite::Attributes &attributes) {
  REQUITE_ASSERT(!this->_root_attributes.getHasAnyAttribute());
  // NOTE:
  //  when resolving, we don't know if an ascription is applied to a sub or a
  //  root. store in root for now, and we will move to a new sub later if
  //  needed.
  this->_root_attributes = attributes;
}

requite::SubSymbol &Symbol::makeSubSymbol() {
  requite::SubSymbol &sub = this->getSubs().emplace_back();
  // NOTE:
  //  we found a sub, so move the attributes from the root to the new subtype
  //  as explained in Symbol::applyAttirbutes() comment.
  sub.getAttributes() = this->getRootAttributes();
  this->_root_attributes.clear();
  return sub;
}

} // namespace requite