// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/module.hpp>

namespace requite {

bool Module::operator==(const Self &rhs) const { return this == &rhs; }

bool Module::operator!=(const Self &rhs) const { return this != &rhs; }

requite::Table &Module::getTable() { return this->_table; }

const requite::Table &Module::getTable() const { return this->_table; }

requite::Source &Module::getSource() { return this->_source; }

const requite::Source &Module::getSource() const { return this->_source; }

bool Module::getHasEntryPoint() const {
  return this->_entry_point_ptr != nullptr;
}

void Module::setEntryPoint(requite::Procedure &procedure) {
  requite::setSingleRef(this->_entry_point_ptr, procedure);
}

requite::Procedure &Module::getEntryPoint() {
  return requite::getRef(this->_entry_point_ptr);
}

const requite::Procedure &Module::getEntryPoint() const {
  return requite::getRef(this->_entry_point_ptr);
}

} // namespace requite