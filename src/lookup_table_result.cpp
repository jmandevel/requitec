#include <requite/lookup_table_result.hpp>
#include <requite/assert.hpp>

namespace requite {

LookupTableResult::LookupTableResult(requite::LookupTableEntry &entry)
    : _entry_ptr(&entry) {}

bool LookupTableResult::getHasEntry() const {
  return this->_entry_ptr != nullptr;
}

requite::LookupTableEntry &LookupTableResult::getEntry() {
  return requite::getRef(this->_entry_ptr);
}

const requite::LookupTableEntry &LookupTableResult::getEntry() const {
  return requite::getRef(this->_entry_ptr);
}

} // namespace requite