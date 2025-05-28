#include <requite/tabulator.hpp>

namespace requite {

Tabulator::Tabulator(requite::Context &context) : _context_ref(context) {}

requite::Context &Tabulator::getContext() { return this->_context_ref.get(); }

const requite::Context &Tabulator::getContext() const {
  return this->_context_ref.get();
}

std::vector<requite::RootSymbol> &Tabulator::getUnamedSymbols() {
  return this->_unamed_symbols;
}

const std::vector<requite::RootSymbol> &Tabulator::getUnamedSymbols() const {
  return this->_unamed_symbols;
}

bool Tabulator::getIsOk() { return this->_is_ok; }

void Tabulator::setNotOk() { this->_is_ok = false; }

} // namespace requite