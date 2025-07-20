#pragma once

namespace requite {

template <typename UserParam>
void LookupTableEntry::addSymbol(UserParam &user) {
  this->_symbols.emplace_back(user);
}

} // namespace requite