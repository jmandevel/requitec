#pragma once

namespace requite {

inline SavedString::SavedString(llvm::StringRef string) : _string(string) {}

inline llvm::StringRef &SavedString::getString() { return this->_string; }

inline const llvm::StringRef &SavedString::getString() const {
  return this->_string;
}

} // namespace requite