#pragma once

#include <requite/assert.hpp>

namespace requite {

template <typename TypeParam, typename... ArgNParam>
TypeParam &Context::allocate(ArgNParam... arg_n) {
  TypeParam *ptr = this->_arena.Allocate<TypeParam>(arg_n...);
  return requite::getRef(ptr);
}

inline requite::SavedString Context::saveString(llvm::StringRef string) {
  llvm::StringRef ref = this->_string_saver.save(string);
  requite::SavedString saved(ref);
  return saved;
}

} // namespace requite