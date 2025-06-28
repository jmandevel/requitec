// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/procedure.hpp>
#include <requite/procedure_iterator.hpp>

namespace requite {

ProcedureIterator::ProcedureIterator(requite::Procedure &procedure)
    : _procedure_ptr(&procedure) {}

requite::ProcedureIterator &ProcedureIterator::operator++() {
  requite::Procedure &procedure = requite::getRef(this->_procedure_ptr);
  this->_procedure_ptr = procedure._next_ptr;
  return *this;
}

requite::ProcedureIterator ProcedureIterator::operator++(int) {
  REQUITE_ASSERT(this->_procedure_ptr != nullptr);
  requite::ProcedureIterator temp = *this;
  ++(*this);
  return temp;
}

bool ProcedureIterator::operator==(
    const requite::ProcedureIterator &it) const {
  return this->_procedure_ptr == it._procedure_ptr;
}

bool ProcedureIterator::operator!=(
    const requite::ProcedureIterator &it) const {
  return this->_procedure_ptr != it._procedure_ptr;
}

requite::Procedure &ProcedureIterator::operator*() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &ProcedureIterator::operator*() const {
  return requite::getRef(this->_procedure_ptr);
}

requite::Procedure *ProcedureIterator::operator->() {
  return this->_procedure_ptr;
}

const requite::Procedure *ProcedureIterator::operator->() const {
  return this->_procedure_ptr;
}

} // namespace requite